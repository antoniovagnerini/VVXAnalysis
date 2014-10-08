/** \class SelectedEventCountProducer
 *   An event counter that can store the number of events in the lumi block, counting events that pass a certain path/filter
 *
 *  $Date: 2013/03/15 13:37:31 $
 *  $Revision: 1.3 $
 *  \author R. Bellan - UNITO <riccardo.bellan@cern.ch>
 */

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include <DataFormats/Common/interface/TriggerResults.h>
#include <FWCore/Common/interface/TriggerNames.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

class SelectedEventCountProducer : public edm::EDProducer {

public:
  explicit SelectedEventCountProducer(const edm::ParameterSet&);
  ~SelectedEventCountProducer(){};

private:
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void beginLuminosityBlock(edm::LuminosityBlock &, const edm::EventSetup &);
  virtual void endLuminosityBlock(edm::LuminosityBlock &, const edm::EventSetup &);

  // ----------member data ---------------------------
  unsigned int eventsProcessedInLumi_;
  std::vector<std::string> filterNames_;
};

using namespace edm;
using namespace std;

SelectedEventCountProducer::SelectedEventCountProducer(const edm::ParameterSet& iConfig)
  : filterNames_(iConfig.getParameter<std::vector<std::string> >("names")){
  
  produces<edm::MergeableCounter, edm::InLumi>();
}


void SelectedEventCountProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  edm::Handle<edm::TriggerResults> triggerResults;
  edm::TriggerNames triggerNames;
  
  if (iEvent.getByLabel(edm::InputTag("TriggerResults"), triggerResults)) {
    triggerNames = iEvent.triggerNames(*triggerResults);
  } else {
    cout << "ERROR: failed to get TriggerResults" << endl;
  }
  
  //for(uint i = 0; i < triggerNames.size(); ++i)
  //  cout<<  triggerNames.triggerName(i)<<endl;
  
  bool passAllRequirements = true;

  foreach(const std::string &filterName, filterNames_){
    unsigned i = triggerNames.triggerIndex(filterName);
    if (i == triggerNames.size()){
      cout << "ERROR: FilterController::isTriggerBit: path does not exist! " << filterName << endl;
      abort();
    }
    passAllRequirements &= triggerResults->accept(i);
  }

  if(passAllRequirements) ++eventsProcessedInLumi_;
}

void SelectedEventCountProducer::beginLuminosityBlock(LuminosityBlock & theLuminosityBlock, const EventSetup & theSetup) {
  
  eventsProcessedInLumi_ = 0;
}

void SelectedEventCountProducer::endLuminosityBlock(LuminosityBlock & theLuminosityBlock, const EventSetup & theSetup) {

  LogTrace("SelectedEventCounting") << "endLumi: adding " << eventsProcessedInLumi_ << " events" << endl;
  auto_ptr<edm::MergeableCounter> numEventsPtr(new edm::MergeableCounter);
  numEventsPtr->value = eventsProcessedInLumi_;
  theLuminosityBlock.put(numEventsPtr);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SelectedEventCountProducer);
