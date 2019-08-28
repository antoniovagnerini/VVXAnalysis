#include "VVXAnalysis/TreeAnalysis/interface/WZZAnalyzer.h"
#include "VVXAnalysis/Commons/interface/SignalDefinitions.h"
#include "VVXAnalysis/Commons/interface/Utilities.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <boost/assign/std/vector.hpp> 
using namespace boost::assign;

using std::cout;
using std::endl;


using namespace phys;

Int_t WZZAnalyzer::cut() {
  
  return 1;
}



void WZZAnalyzer::analyze(){

  cout << "-----------------------------------------------------------------"<<endl;
  cout << "Run: " << run << " event: " << event << endl;

  foreach(const phys::Particle& gen, *genParticles){
    if(abs(gen.id()) == 13 )//for muon: mc pdg id = 13 by notation
      theHistograms.fill("genMuonPt","pt of gen muons",20,0,200,gen.pt(),theWeight);
  }
   /*
   *
   foreach(const phys::Lepton& mu, *muons){
    theHistograms.fill("muonsPt1","pt of muons",20,0,200,mu.pt());
    theHistograms.fill("muonsPtEta1","eta vs pt of muons",60,0,200,30,0,2.5, mu.pt(),fabs(mu.eta()));
  }

   foreach(const phys::Particle& gen, *genParticles){
    if(abs(gen.id()) == 13 )//for muon: mc pdg id = 13 by notation
      theHistograms.fill("genMuonEta","eta of gen muons",30,0,2.5,fabs(gen.eta()),theWeight);
  }
  *
  */
  
  
}
