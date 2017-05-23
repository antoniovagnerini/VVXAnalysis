#include "VVXAnalysis/TreeAnalysis/interface/WlllnuAnalyzer.h"
#include "VVXAnalysis/Commons/interface/SignalDefinitions.h"
#include "VVXAnalysis/Commons/interface/Utilities.h"
#include "VVXAnalysis/Commons/interface/Constants.h"
#include "VVXAnalysis/DataFormats/interface/Boson.h"
#include "VVXAnalysis/DataFormats/interface/DiBoson.h"
#include "VVXAnalysis/DataFormats/interface/Lepton.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <boost/assign/std/vector.hpp>
using namespace boost::assign;

using std::cout;
using std::endl;


using namespace phys;

Int_t WlllnuAnalyzer::cut() {
  return 1;
}


void WlllnuAnalyzer::analyze(){
  
  cout << "------------------------------------------------------------------"<<endl;
  cout << "Run: " << run << " event: " << event << endl;
  
  //leptons
  
  std::vector<phys::Particle>  leptons;
  phys::Lepton lep;
  
  foreach(const phys::Particle &gen, *genParticles){

    if( (abs(gen.id()) != 11 && abs(gen.id()) != 13) || (!(gen.genStatusFlags().test(phys::GenStatusBit::isPrompt)) || !(gen.genStatusFlags().test(phys::GenStatusBit::fromHardProcess)))) continue;
    cout << "id: " << gen.id() << " pt: " << gen.pt() << " mass: " << gen.mass() << " Y: " << gen.rapidity() << endl;
    theHistograms.fill("ptAllGenParticle","pt ", 100, 0, 100, gen.pt());
    theHistograms.fill("etaAllGenParticle","eta ", 100, 0, 100, gen.eta());
    theHistograms.fill("massAllGenParticle","mass ", 100, 0, 100, gen.mass());
    theHistograms.fill("YAllGenParticle","Y ", 100, 0, 100, gen.rapidity());
    leptons.push_back(gen);
  }
  //cout << "lepton 0: mass" << leptons.at(0).mass() << endl;

  int finalid = 0;
  foreach(const Particle lep, leptons){
    finalid += lep.id();
  }
  cout << "final id: " << finalid << endl;
  // finalid = 44 -> 4e
  // finalid = 48 -> 2mu2e
  // finalid = 52 -> 4mu

  //----------------------------------------------------------------//  
  
  //z0, z1
  
  phys::Boson<phys::Particle> z0;
  phys::Boson<phys::Particle> z1;
  //double zMass = 91.1876; //Gev
  
  foreach(const phys::Boson<phys::Particle> &gen, *genVBParticles){
   
    if( ZBosonDefinition(gen) && gen.id() == 23 && (abs(gen.daughter(0).id()) == 11 || abs(gen.daughter(0).id()) == 13)){
      if(z0.id() != 23) z0 = gen;
      else if ( abs(z0.mass() - ZMASS) < abs(gen.mass() - ZMASS)){
	//if ( gen.daughter(0).pt() + gen.daughter(1).pt() > z0.daughter(0).pt() + z0.daughter(1).pt() )
	z1 = gen;
      }
      else{ 
	z1 = z0;
	z0 = gen;
      }
      theHistograms.fill("ptz0","pt ", 100, 0, 100, z0.pt());
      theHistograms.fill("ptz1","pt ", 100, 0, 100, z1.pt());
      theHistograms.fill("etaz0","eta ", 100, 0, 100, z0.eta());
      theHistograms.fill("etatz1","eta ", 100, 0, 100, z1.eta());
      theHistograms.fill("massz0","mass ", 100, 0, 100, z0.mass());
      theHistograms.fill("massz1","mass ", 100, 0, 100, z1.pt());
      theHistograms.fill("Yz0","Y ", 100, 0, 100, z0.rapidity());
      theHistograms.fill("Yz1","Y ", 100, 0, 100, z1.rapidity());      
    }
  }
  cout << "\nz0: " << z0.id() << " pt: " << z0.pt() << " mass: " << z0.mass() << " Y: " << z0.rapidity() << endl;
  cout << "z1: " << z1.id() << " pt: " << z1.pt() << " mass: " << z1.mass() << " Y: " << z1.rapidity() << endl;

  //----------------------------------------------------------------//

  //z0, z1 daughters

  cout << "\nz0.daughter(0) " << z0.daughter(0).id() << " pt: " << z0.daughter(0).mass() << endl;
  cout << "z0.daughter(1) " << z0.daughter(1).id() << " pt: " << z0.daughter(1).mass() << endl;
  cout << "z1.daughter(0) " << z1.daughter(0).id() << " pt: " << z1.daughter(0).mass() << endl;
  cout << "z1.daughter(1) " << z1.daughter(1).id() << " pt: " << z1.daughter(1).mass() << endl;

  //deltaR

  //double deltaPhi0 = z0.daughter(0).phi() - z1.phi(); //between z1 and z0's first daughter
  //double deltaPhi1 = z0.daughter(1).phi() - z1.phi(); //between z1 and z0's second daughter
  double deltaEta0 = z0.daughter(0).eta() - z1.eta(); //between z1 and z0's first daughter
  double deltaEta1 = z0.daughter(1).eta() - z1.eta(); //between z1 and z0's first daughter
  
  cout << "deltaR daughter 0: " << deltaR(z1.p4(), z0.daughter(0).p4()) << endl;
  cout << "deltaR daughter 1: " << deltaR(z1.p4(), z0.daughter(1).p4()) << endl; 
  cout << "deltaPhi daughter 0: " << deltaPhi(z0.daughter(0).p4(), z1.p4()) << endl;
  cout << "deltaPhi daughter 1: " << deltaPhi(z0.daughter(1).p4(), z1.p4()) << endl;
  cout << "deltaEta daughter 0: " << deltaEta0 << endl;
  cout << "deltaEta daughter 1: " << deltaEta1 << endl;
  
  //----------------------------------------------------------------//

  //ll
  /*
  std::vector<Boson<phys::Particle> > Zcand;
  
  if(finalid == 48){ //2mu2e
    
    
  }
  */

  
  //----------------------------------------------------------------//  
  
  // ZZ

  DiBoson<phys::Particle,phys::Particle>  ZZ(z0,z1);
  cout << "\n ZZ: " << ZZ.id() << " pt: " << ZZ.pt() << " mass: " << ZZ.mass() << "\n daughters: " << " Y: " << ZZ.rapidity() << ZZ.first().id() << "\t" << ZZ.second().id() << endl; //why daughter(0) instead of first() is not working??

  theHistograms.fill("ptZZ","pt ", 100, 0, 100, ZZ.pt());
  theHistograms.fill("etaZZ","eta ", 100, 0, 100, ZZ.eta());
  theHistograms.fill("massZZ","mass ", 100, 0, 100, ZZ.pt());
  theHistograms.fill("YZZ","Y ", 100, 0, 100, ZZ.rapidity());
  //<phys::Boson<phys::Particle>,phys::Boson<phys::Particle> > 
}
