#ifndef ZZWAnalyzer_h
#define ZZWAnalyzer_h

/** \class ZZWAnalyzer
 *  Concrete class for ZZW analysis
 *
 *  $Date: 2013/03/15 13:37:31 $
 *  $Revision: 1.4 $
 *  \author R. Bellan - UNITO <riccardo.bellan@cern.ch>
 */


#include "EventAnalyzer.h"
#include "RegistrableAnalysis.h"

class ZZWAnalyzer: public EventAnalyzer, RegistrableAnalysis<ZZWAnalyzer>{

public:

 ZZWAnalyzer(std::string filename, double lumi = 1., double externalXSection = -1., bool doBasicPlots = false)
   : EventAnalyzer(*(new Selector<ZZWAnalyzer>(*this)),
		   filename, lumi, externalXSection, doBasicPlots){}

  virtual ~ZZWAnalyzer(){}

  virtual void analyze();

  virtual Int_t cut();

 private:
  friend class Selector<ZZWAnalyzer>; 
  template< class PAR >
    bool ZBosonDefinition(phys::Boson<PAR> cand) const {
    return fabs(cand.p4().M() - ZMASS) < 20;
  }
  template< class PAR >
    bool WBosonDefinition(phys::Boson<PAR> cand) const {
    bool massRange = fabs(cand.p4().M() - WMASS) < 40;
    bool jetPt = cand.daughter(0).pt() > 40 || cand.daughter(1).pt() > 40;
    return massRange && jetPt;
  }

};
#endif

