#include "VVXAnalysis/Producers/interface/FilterController.h"
#include <ZZAnalysis/AnalysisStep/interface/bitops.h>

using namespace std;
using namespace edm;

FilterController::FilterController(const ParameterSet& pset) :
  PD(pset.getParameter<std::string>("PD")),
  isMC_(pset.getUntrackedParameter<bool>("isMC")),
  theSetup(pset.getParameter<int>("setup")),
  theSampleType(pset.getParameter<int>("sampleType")),
  skimPaths(pset.getParameter<std::vector<std::string> >("skimPaths")),
  MCFilter(pset.getParameter<std::string>("MCFilterPath"))
  
{
  string channel = pset.getUntrackedParameter<string>("channel");
  theChannel = finalState(channel);
  
  // Check for inconsistent configurations
  if ((theSampleType != 2011 && theSampleType != 2012) ||
      ((theSampleType != theSetup) && (!isMC_ || theSampleType!=2011))) {
    cout << "ERROR: FilterController: inconsistent setup" << theSampleType << " " << theSetup << " " <<isMC_ << endl;
    abort();
  }
  
  
  if ((isMC_&&PD!="") || (!isMC_ && (PD!="DoubleEle" && PD!="DoubleMu" && PD!="MuEG"))) {
    cout << "ERROR: FilterController: isMC: " << isMC_ << " PD: " << PD << endl;
    abort();
  }    

  if (!isMC_&&MCFilter!="") {
    cout << "ERROR: FilterController: MCFilter= " << MCFilter << " when isMC=0" 
	 << endl;
    abort();
  }    
  
}

void
FilterController::eventInit(const edm::Event & event) {
  // Initialize trigger results table
  if (event.id()==cachedEvtId) return;
  if (event.getByLabel(InputTag("TriggerResults"), triggerResults)) {
    triggerNames = &(event.triggerNames(*triggerResults));
  } else {
    cout << "ERROR: failed to get TriggerResults" << endl;
  }

//   if (event.getByLabel(InputTag("TriggerResults","","HLT"), triggerResultsHLT)) {
//     triggerNamesHLT = &(event.triggerNames(*triggerResultsHLT));
//   } else {
//     cout << "ERROR: failed to get TriggerResults:HLT" << endl;
//   }

  cachedEvtId = event.id();
}

bool 
FilterController::passMCFilter(const edm::Event & event){
  if (MCFilter=="") return true;
  return passFilter(event, MCFilter);
}

bool 
FilterController::passSkim(const edm::Event & event, short& trigworld){
  bool evtPassSkim = false;
  if (skimPaths.size()==0) {
    evtPassSkim=true;
  } else {
    for (vector<string>::const_iterator name = skimPaths.begin(); name!= skimPaths.end(); ++name) {
      if (passFilter(event, *name)) {
	evtPassSkim = true; 
	break;
      }
    }
  }
  if (evtPassSkim) set_bit_16(trigworld,7);
  return evtPassSkim;
}

bool 
FilterController::passTrigger(const edm::Event & event, short& trigworld){
  bool passDiMu  = passFilter(event, "triggerDiMu");
  bool passDiEle = passFilter(event, "triggerDiEle");
  bool passMuEle  = passFilter(event, "triggerMuEle"); 
  if ((!isMC_) && theSetup == 2011 ) { // follow changes in trigger menu in data 2011 (see wiki)
    int irun=event.id().run(); 
    if (irun>=175973) {
      passMuEle  = passFilter(event, "triggerMuEle3");
    } else if (irun>=167914) {
      passMuEle  = passFilter(event, "triggerMuEle2");
    }
  }
  bool passTriEle = false;
  if (theSetup == 2012) {
    passTriEle = passFilter(event, "triggerTriEle");
  }



  bool evtPassTrigger = false;
  //FIXME: this is assuming that we do not pick EEEE or MMMM from "DoubleOr" files as there is no protection for accidental triggers
  if ((theChannel==EEEE && (passDiEle || passTriEle)) ||
      (theChannel==MMMM && passDiMu)) {
    evtPassTrigger = true;
  }

  if (theChannel==EEMM) {
    if ((PD=="" && (passDiEle || passDiMu || passMuEle)) ||
	(PD=="DoubleEle" && passDiEle) ||
	(PD=="DoubleMu" && passDiMu && !passDiEle) ||
	(PD=="MuEG" && passMuEle && !passDiMu && !passDiEle )) {
      evtPassTrigger = true;
    }
  }

  // Use all triggers together for the CR
  if (theChannel==ZLL || theChannel==ZL) {
    if ((PD=="" && (passDiEle || passDiMu || passMuEle || passTriEle)) ||
	(PD=="DoubleEle" && (passDiEle || passTriEle)) ||
	(PD=="DoubleMu" && passDiMu && !passDiEle && !passTriEle) ||
	(PD=="MuEG" && passMuEle && !passDiMu && !passDiEle && !passTriEle)) {
      evtPassTrigger = true;
    } 
  }
  
  if (evtPassTrigger) set_bit_16(trigworld,0);
  if (passDiMu) set_bit_16(trigworld,1);
  if (passDiEle) set_bit_16(trigworld,2);
  if (passMuEle) set_bit_16(trigworld,3);
  if (passTriEle) set_bit_16(trigworld,4);
  
  return evtPassTrigger;
}

bool
FilterController::passFilter(const edm::Event & event, const string& filterPath, bool fromHLT) {
  eventInit(event);

  edm::Handle<edm::TriggerResults> myTriggerResults;
  const edm::TriggerNames* myTriggerNames = 0;

  if (fromHLT) {
//     myTriggerNames = triggerNamesHLT;
//     myTriggerResults = triggerResultsHLT;
  } else {
    myTriggerNames = triggerNames;
    myTriggerResults = triggerResults;    
  }

  unsigned i =  myTriggerNames->triggerIndex(filterPath);
  if (i== myTriggerNames->size()){
    cout << "ERROR: FilterController::isTriggerBit: path does not exist! " << filterPath << endl;
    abort();
  }
  return myTriggerResults->accept(i);

}




