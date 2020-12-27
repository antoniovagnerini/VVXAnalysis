#include "VVXAnalysis/TreeAnalysis/interface/VVjjHelper.h"
#include "VVXAnalysis/Commons/interface/Comparators.h"
#include "VVXAnalysis/Commons/interface/Constants.h"
#include "VVXAnalysis/Commons/interface/SignalDefinitions.h"
#include "VVXAnalysis/Commons/interface/Utilities.h"
#include "VVXAnalysis/DataFormats/interface/TypeDefs.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <boost/assign/std/vector.hpp> 
using namespace boost::assign;

using namespace std;
using namespace phys;
using namespace physmath;


void VVjjHelper::test() {
  cout << "pippo!!" << endl;
}


void VVjjHelper::LeptonSearch(const vector<Particle> &genparticles, string eventkind){
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~~~~ Function to find Leptons ~~~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  // Reset Data Member containers
  leptons_.clear();
  neutrinos_.clear();
  
  // Get leptons from genParticles
  foreach(const Particle &gen, genparticles){
    if((abs(gen.id()) != 11 && abs(gen.id()) != 13 && abs(gen.id()) != 12 && abs(gen.id()) != 14) || (!(gen.genStatusFlags().test(phys::GenStatusBit::isPrompt)) || !(gen.genStatusFlags().test(phys::GenStatusBit::fromHardProcess)))) 
      continue;

    // Leptons accepted are:
    // - electrons with pt >= 7. and |eta| <= 2.5
    if(abs(gen.id()) == 11 && gen.pt() >= 7. && abs(gen.eta()) <= 2.5){
      leptons_.push_back(gen);
    }
    // - muons with pt >= 5. and |eta| <= 2.4    
    else if(abs(gen.id()) == 13 && gen.pt() >= 5. && abs(gen.eta()) <= 2.4){
      leptons_.push_back(gen);
    }
    // - electronic or muonic neutrinos
    else if(abs(gen.id()) == 12 || abs(gen.id()) == 14){
      neutrinos_.push_back(gen);
    }
  }

  // sorting leptons by pt
  sort(leptons_.begin(), leptons_.end(), PtComparator());

  // different selection for differents event types: 1 for WZ and 0 for ZZ
  int switchcase = -1;
  if(eventkind == "WZ" && leptons_.size() == 3){
    switchcase = 1;
  }
  else if(eventkind == "ZZ" && leptons_.size() == 4){
    switchcase = 0;
  }

  // first leptons selection is the same

  switch(switchcase){
  case 0:{// ZZevent
    // things for ZZ event
  }
    break;
   
  case 1:{// WZ event
    // first lepton must have at least 20GeV pt
    if(leptons_[0].pt() < 20.){
      leptons_.clear();
      //cout << "Leptons_ size " << leptons_.size() << endl;
    }

    // second lepton must have at least 12GeV pt if electron or 10GeV pt if muon
    switch(abs(leptons_[1].id())){
    case 11:{
      if(leptons_[1].pt() < 12.){
	leptons_.clear();
	//cout << "Leptons_ size " << leptons_.size() << endl;
      }
    }
      break;

    case 13:{
      if(leptons_[1].pt() < 10.){
	leptons_.clear();
	//cout << "Leptons_ size " << leptons_.size() << endl;
      }
    }
      break;

    default:{
      cout << "ERROR: second lepton's ID is: " << leptons_[1].id() << endl << endl;
      return;
    }
    }
  }
    break;
     
  default:{
    //cout << "ERROR: lepton's number is: " << leptons_.size() << endl << endl;
    return;
  }
  }

  TLorentzVector Ptot;

  foreach(const Particle neu, neutrinos_)
    Ptot += neu.p4();

  if(leptons_.size() != 0){
    foreach(const Particle lep, leptons_)
      Ptot += lep.p4();
  }
  
  histo_->fill("AllGenlllnu_mass",   "m 3 leptons and #nu",     150, 0, 1500, Ptot.M());
  histo_->fill("AllGenlllnu_trmass", "m_{T} 3 leptons and #nu", 150, 0, 1500, Ptot.Mt());
  
  if(Ptot.M() < 165.){
    leptons_.clear();    
  }
}


DiBosonParticle VVjjHelper::BuildVV(string eventkind){
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~~~~ Function to build DiBoson ~~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
  DiBosonParticle VV;
  DiBosonParticle nihil;

  // different ways to build up the VV couple if event is WZ or ZZ
  int switchcase = -1;
  if(eventkind == "WZ"){
    switchcase = 1;
  }
  else if(eventkind == "ZZ"){
    switchcase = 0;
  }
  
  switch(switchcase){
  case 0:{// ZZ event
    // things
  }
    break;

  case 1:{// WZ event
    // useful variables
    vector<pairBosonParticle> Zls;
    vector<DiBosonParticle> WZs;
    double differenceZ = 0;
    double differenceW = 0;
    unsigned int choice = 0;

    // fill Zl candidates
    for(int i = 0; i < (int)leptons_.size() -1; i++){
      for(int j = i + 1; j < (int)leptons_.size(); j++){
	for(int k = 0; k < (int)leptons_.size(); k++){
	  if(k != i && k != j){
	    if(leptons_[i].id() == -leptons_[j].id()){// same flavour, opposite charge
	      Zls.push_back(pairBosonParticle(BosonParticle(leptons_[i], leptons_[j], 23), leptons_[k]));
	    }
	  }
	}
      }
    }
    
    // Z is made up of the couple which gives a better Zmass 
    sort(Zls.begin(), Zls.end(), pairMassComparator(0, ZMASS));
    differenceZ = fabs(ZMASS - Zls[0].first.p4().M());

    for(int i = 0; i < (int)Zls.size(); i++){
      WZs.push_back(DiBosonParticle(BosonParticle(Zls[i].second, neutrinos_[0], copysign(24, Zls[i].second.charge())), Zls[i].first));
    }
    
    // W is made up of the couple which gives a better Wmass 
    sort(WZs.begin(), WZs.end(), pairMassComparator(0, WMASS));
    differenceW = fabs(WMASS - WZs[0].first().p4().M());
    
    // Best couple has less difference in mass from main boson
    if(differenceZ < differenceW){ // Z is better
      VV = DiBosonParticle(BosonParticle(Zls[0].second, neutrinos_[0], copysign(24, Zls[0].second.charge())), Zls[0].first);
      choice = 1;
      histo_->fill("Helper_choosingWZ", "best WZ couple choice", 10, 0.5, 10.5, choice);
    }
    else{ // W is better
      VV = WZs[0];
      choice = 2;
      histo_->fill("Helper_choosingWZ", "best WZ couple choice", 10, 0.5, 10.5, choice);
    }

    if(fabs(VV.first().mass() - WMASS) > rangeVmass || fabs(VV.second().mass() - ZMASS) > rangeVmass){
      VV = nihil;
      choice = 3;
      histo_->fill("Helper_choosingWZ", "best WZ couple choice", 10, 0.5, 10.5, choice);
    }

    if(isTheSame(Zls[0].first, WZs[0].second())){
      choice = 4;
      histo_->fill("Helper_choosingWZ", "best WZ couple choice", 10, 0.5, 10.5, choice);
    }

  }
    break;

  default:{
    cout << "ERROR: wrong initialisation for eventkind, " << eventkind << endl << endl;
  }
  }

  return VV;
}


void FindLeadingJets(vector<Particle> &jetcollection, vector<Particle> &particlecollection, Particle &Jet0, Particle &Jet1){
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~ Function to find leading jets ~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  vector<Particle> jets;
  bool leptonMatch = false; 

  foreach(const Particle &jet, jetcollection){ // in genJets are included leptons
    foreach(const Particle &gen, particlecollection){ // get jets only
      if(deltaR(gen,jet) < 0.4 && (abs(gen.id()) == 11 || abs(gen.id()) == 13))
	leptonMatch = true;
    }
    
    if(!leptonMatch){ // jets must have rapidity less than 4.7
      if(fabs(jet.eta()) < 4.7)
	jets.push_back(jet);
    }
  }
  
  if(jets.size() < 2){
    return;
  }
  
  // leading jets are those with higher pt
  sort(jets.begin(), jets.end(), PtComparator());
  Jet0 = jets[0];
  Jet1 = jets[1];
  
}


  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~~~~~~~~ Getter functions ~~~~~~~~~~~~~~~~~~~~~~~
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int VVjjHelper::GetAllLeptonsNumber(){
  return leptons_.size() + neutrinos_.size();
}

unsigned int VVjjHelper::GetNeutrinosNumber(){
  return neutrinos_.size();
}
