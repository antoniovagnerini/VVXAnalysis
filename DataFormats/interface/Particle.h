#ifndef ZZWAnalysis_DataFormats_Particle_H
#define ZZWAnalysis_DataFormats_Particle_H

/** \class Particle
 *  No description available.
 *
 *  $Date: 2013/03/15 13:37:31 $
 *  $Revision: 1.3 $
 *  \author R. Bellan - UCSB <riccardo.bellan@cern.ch>
 */

#include <TObject.h>
#include <TLorentzVector.h> 
#include "Math/GenVector/LorentzVector.h"

class TreePlanter;

namespace phys {

  
  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

  class Particle: public TObject {
    
    friend class ::TreePlanter;

  public:
    static TLorentzVector convert(const LorentzVector& l) {return TLorentzVector(l.Px(),l.Py(),l.Pz(),l.E());}
    
    /// Constructor
    Particle(const TLorentzVector& p = TLorentzVector(0.,0.,0.,0.), int q =0, int i = 0)
      : p4_(p)
      , charge_(q)
      , id_(i){}

      Particle(const LorentzVector& l, int q =0, int i = 0)
	:p4_(convert(l))
	, charge_(q)
	, id_(i){}

	
    /// Destructor
    virtual ~Particle(){};
    
    // Operations
    TLorentzVector p4() const {return p4_;}
    int id()            const {return id_;}
    int charge()        const {return charge_;}
    double pt()         const {return p4_.Pt();}
    double eta()        const {return p4_.Eta();}
    double phi()        const {return p4_.Phi();}
    // Method that tries to infer the charge of the particle starting from a pdgId in input
    // to be moved?
    static double computeCharge(int pdgId) {
      double charge = 0;
      if(abs(pdgId) == 1       || abs(pdgId) == 3  || abs(pdgId) == 5) // d, s or b
	charge = -1*std::copysign(1/3., pdgId);
      else if(abs(pdgId) == 2  || abs(pdgId) == 4  || abs(pdgId) == 6) // u, c or t
	charge = std::copysign(2/3., pdgId);
      else if(abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15) // e, mu or tau
	charge = -1*std::copysign(1, pdgId);
      else if(abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16 ||                   // ve, vmu, vtau
	      abs(pdgId) == 21 || abs(pdgId) == 22 || abs(pdgId) == 23 || abs(pdgId) == 25) // gluon, gamma, Z, H
	charge = 0;
      else if(abs(pdgId) == 24) // W
	charge = std::copysign(1, pdgId);

      return charge;
    }
 
   private:
    TLorentzVector p4_;
    Int_t charge_;
    
    
  protected:
    Int_t id_;    


    //Particle genParticle_;

  private:
    ClassDef(Particle, 1) //
  };
}

#endif

