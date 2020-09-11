#include "VVXAnalysis/TreeAnalysis/interface/VZZaQGCAnalyzer.h"
#include "VVXAnalysis/Commons/interface/SignalDefinitions.h"
#include "VVXAnalysis/Commons/interface/Utilities.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <boost/assign/std/vector.hpp> 
using namespace boost::assign;

using std::cout;
using std::endl;

using namespace phys;

Int_t VZZaQGCAnalyzer::cut() {
  return 1;
}

double theta1,theta2,theta3,theta4,theta5,theta6,phi1,phi2,phi3,phi4,phi5,phi6,angolo1,angolo2,angolo,dR,dR2,massaz1,massaz2,massaz3,massaz4,ptz1,ptz2,ptz3,ptz4,energiaz1,energiaz2,energiaz3,energiaz4,enlep1,enlep2,enlep3,enlep4,etaz1,etaz2,mass1,mass2;
TLorentzVector a,b,zero;

void VZZaQGCAnalyzer::analyze(){
massaz1=massaz2=energiaz1=energiaz2=ptz1=ptz2=0;
 a=zero;
   foreach(const phys::Boson<phys::Particle> genVBParticle,*genVBParticles){
     if((abs(genVBParticle.daughter(0).id())==11||abs(genVBParticle.daughter(0).id())==13)&&genVBParticle.daughter(0).id()==(-1)*genVBParticle.daughter(1).id()){
      theHistograms.fill("theta bosoni generati","Theta bosoni generati",75 ,0,3.5,genVBParticle.p4().Theta());
      theHistograms.fill("eta bosoni generati","Eta bosoni generati",60,0,6,genVBParticle.eta());
      theta1=genVBParticle.daughter(0).p4().Theta();
      theta2=genVBParticle.daughter(1).p4().Theta();
      phi1=genVBParticle.daughter(0).phi();
      phi2=genVBParticle.daughter(1).phi();
      angolo=acos(sin(theta1)*sin(theta2)*cos(physmath::deltaPhi(phi1,phi2))+cos(theta1)*cos(theta2));
      theHistograms.fill("angolo leptoni generati","Angolo leptoni generati",75,0,3.5,angolo);
      if(genVBParticle.daughter(1).e()>genVBParticle.daughter(0).e()){
	theHistograms.fill("E leptone maggiore","Energia leptone piu' energetico",200,0,2000,genVBParticle.daughter(1).e());
      	theHistograms.fill("E leptone minore","Energia leptone meno energetico",200,0,800,genVBParticle.daughter(0).e());}
      else{theHistograms.fill("E leptone maggiore","Energia leptone piu' energetico",200,0,2000,genVBParticle.daughter(0).e());
	theHistograms.fill("E leptone minore","Energia leptone meno energetico",200,0,800,genVBParticle.daughter(1).e());}
      a+=genVBParticle.p4();
     }}	   
   if(topology.test(0)){
     mass1=sqrt((a.E()*a.E())-(a.Px()*a.Px())-(a.Py()*a.Py())-(a.Pz()*a.Pz()));
       theHistograms.fill("massa dibosoni generati","Massa ZZ generati",78,80,600,mass1);
	 foreach(const phys::Boson<phys::Particle> genVBParticle,*genVBParticles){
	   theHistograms.fill("massa bosoni generati bene","Massa bosoni generati bene",15,70,110,genVBParticle.mass());
	   theHistograms.fill("pt bosoni generati bene","Pt bosoni generati bene",15,0,350,genVBParticle.pt());
	   theHistograms.fill("energia bosoni generati bene","Energia bosoni generati bene",15,0,1400,genVBParticle.e());
	   theHistograms.fill("eta bosoni generati bene","Eta bosoni generati bene",15,0,4.5,genVBParticle.eta());
	   if(genVBParticle.daughter(1).e()>genVBParticle.daughter(0).e()){
	      theHistograms.fill("E leptone maggiore buono","Energia leptone piu' energetico",15,0,1000,genVBParticle.daughter(1).e());
	      theHistograms.fill("E leptone minore buono","Energia leptone meno energetico",15,0,300,genVBParticle.daughter(0).e());}
	   else{theHistograms.fill("E leptone maggiore buono","Energia leptone piu' energetico",15,0,1000,genVBParticle.daughter(0).e());
	      theHistograms.fill("E leptone minore buono","Energia leptone meno energetico",15,0,300,genVBParticle.daughter(1).e());}
	   if(massaz1==0){
		massaz1=genVBParticle.mass();
	        energiaz1=genVBParticle.e();
	        ptz1=genVBParticle.pt();}
	   else{if(abs(genVBParticle.mass()-phys::ZMASS)<abs(massaz1-phys::ZMASS)){
		  massaz2=massaz1;
		  energiaz2=energiaz1;
		  ptz2=ptz1;
		  massaz1=genVBParticle.mass();
		  energiaz1=genVBParticle.e();
		  ptz1=genVBParticle.pt();}
		else{
		  massaz2=genVBParticle.mass();
		  energiaz2=genVBParticle.e();
		  ptz2=genVBParticle.pt();}}}
	    theHistograms.fill("massa Z1 generati","Massa Z1 generati",180,50,130,massaz1);
	    theHistograms.fill("massa Z2 generati","Massa Z2 generati",90,50,130,massaz2);
	    theHistograms.fill("energia Z1 generati","Energia Z1 generati",135,0,2700,energiaz1);
	    theHistograms.fill("energia Z2 generati","Energia Z2 generati",135,0,2700,energiaz2);
	    theHistograms.fill("pt Z1 generati","Pt Z1 generati",150,0,900,ptz1);
	    theHistograms.fill("pt Z2 generati","Pt Z2 generati",150,0,900,ptz2);}
     
   if(ZZ->first().mass()!=0){
   theHistograms.fill("massa Z1 ricostruiti","Massa Z1 ricostruiti",180,50,130,ZZ->first().mass());
   theHistograms.fill("massa Z2 ricostruiti","Massa Z2 ricostruiti",180,50,130,ZZ->second().mass());
   theHistograms.fill("pt Z1 ricostruiti","Pt Z1 ricostruiti",150,0,900,ZZ->first().pt());
   theHistograms.fill("pt Z2 ricostruiti","Pt Z2 ricostruiti",150,0,900,ZZ->second().pt());
   theHistograms.fill("theta bosoni ricostruiti","Theta bosoni ricostruiti",75 ,0,3.5,ZZ->first().p4().Theta());
   theHistograms.fill("theta bosoni ricostruiti","Theta bosoni ricostruiti",75 ,0,3.5,ZZ->second().p4().Theta());
   theHistograms.fill("energia Z1 ricostruiti","Energia Z1 ricostruiti",135,0,2700,ZZ->first().e());
   theHistograms.fill("energia Z2 ricostruiti","Energia Z2 ricostruiti",135,0,2700,ZZ->second().e());
   theHistograms.fill("eta bosoni ricostruiti","Eta bosoni ricostruiti",60,0,6,ZZ->first().eta());
   theHistograms.fill("eta bosoni ricostruiti","Eta bosoni ricostruiti",60,0,6,ZZ->second().eta()); 

   theta3=ZZ->first().daughter(0).p4().Theta();
   theta4=ZZ->first().daughter(1).p4().Theta();
   phi3=ZZ->first().daughter(0).phi();
   phi4=ZZ->first().daughter(1).phi();
   angolo1=acos(sin(theta3)*sin(theta4)*cos(physmath::deltaPhi(phi3,phi4))+cos(theta3)*cos(theta4));
   theHistograms.fill("angolo leptoni ricostruiti","Angolo leptoni ricostruiti",75,0,3.5,angolo1);
   
   theta5=ZZ->second().daughter(0).p4().Theta();
   theta6=ZZ->second().daughter(1).p4().Theta();
   phi5=ZZ->second().daughter(0).phi();
   phi6=ZZ->second().daughter(1).phi();
   angolo2=acos(sin(theta5)*sin(theta6)*cos(physmath::deltaPhi(phi5,phi6))+cos(theta5)*cos(theta6));
   theHistograms.fill("angolo leptoni ricostruiti","Angolo leptoni ricostruiti",75,0,3.5,angolo2);
   
   b=ZZ->first().p4()+ZZ->second().p4();
   mass2=sqrt((b.E()*b.E())-(b.Px()*b.Px())-(b.Py()*b.Py())-(b.Pz()*b.Pz()));
   theHistograms.fill("massa dibosoni ricostruiti","Massa ZZ ricostruiti",78,80,600,mass2);
	
   if(ZZ->first().daughter(1).e()>ZZ->first().daughter(0).e()){
     theHistograms.fill("E leptone maggiore ricostruito","Energia leptone piu' energetico ricostruito",200,0,2000,ZZ->first().daughter(1).e());}
   else{theHistograms.fill("E leptone maggiore ricostruito","Energia leptone piu' energetico ricostruito",200,0,2000,ZZ->first().daughter(0).e());};
   
   if(ZZ->second().daughter(1).e()>ZZ->second().daughter(0).e()){
     theHistograms.fill("E leptone maggiore ricostruito","Energia leptone piu' energetico ricostruito",200,0,2000,ZZ->second().daughter(1).e());}
   else{theHistograms.fill("E leptone maggiore ricostruito","Energia leptone piu' energetico ricostruito",200,0,2000,ZZ->second().daughter(0).e());};
   
   if(ZZ->first().daughter(1).e()>ZZ->first().daughter(0).e()){
     theHistograms.fill("E leptone minore ricostruito","Energia leptone meno energetico ricostruito",200,0,800,ZZ->first().daughter(0).e());}
   else{theHistograms.fill("E leptone minore ricostruito","Energia leptone meno energetico ricostruito",200,0,800,ZZ->first().daughter(1).e());};
   
   if(ZZ->second().daughter(1).e()>ZZ->second().daughter(0).e()){
     theHistograms.fill("E leptone minore ricostruito","Energia leptone meno energetico ricostruito",200,0,800,ZZ->second().daughter(0).e());}
   else{theHistograms.fill("E leptone minore ricostruito","Energia leptone meno energetico ricostruito",200,0,800,ZZ->second().daughter(1).e());};
   
   dR=dR2=9999;
   massaz3=massaz4=ptz1=ptz2=energiaz3=energiaz4=enlep1=enlep2=enlep3=enlep4=etaz1=etaz2=0;
   if(topology.test(0)){
    foreach(const phys::Boson<phys::Particle> genVBParticle,*genVBParticles){
      if((abs(genVBParticle.daughter(0).id())==11||abs(genVBParticle.daughter(0).id())==13)&&genVBParticle.daughter(0).id()==(-1)*genVBParticle.daughter(1).id()){
       if(physmath::deltaR(genVBParticle,ZZ->first())<dR){
	   dR=physmath::deltaR(genVBParticle,ZZ->first());
	   massaz3=genVBParticle.mass();
	   energiaz3=genVBParticle.e();
	   ptz3=genVBParticle.pt();
	   etaz1=genVBParticle.eta();
	   if(genVBParticle.daughter(0).e()>genVBParticle.daughter(1).e()){
	     enlep1=genVBParticle.daughter(0).e();
	     enlep2=genVBParticle.daughter(1).e();}
	   else{enlep1=genVBParticle.daughter(1).e();
	     enlep2=genVBParticle.daughter(0).e();}}}}}
  
   if(dR<0.1){
     theHistograms.fill("confronto massa Z1","Differenza massa generata/ricostruita Z1",40,-8,8,ZZ->first().mass()-massaz3);
     theHistograms.fill("confronto energia Z1","Differenza energia generata/ricostruita Z1",200,-40,40,ZZ->first().e()-energiaz3);
     theHistograms.fill("confronto pt Z1","Differenza pt generata/ricostruita Z1",150,-30,30,ZZ->first().pt()-ptz3);
     theHistograms.fill("confronto eta Z1","Differenza eta generata/ricostruita Z1",50,-0.1,0.1,ZZ->first().eta()-etaz1);
     theHistograms.fill("massa bosoni ricostruiti bene","Massa bosoni ricostruiti bene",15,70,110,massaz3);
     theHistograms.fill("pt bosoni ricostruiti bene","Pt bosoni ricostruiti bene",15,0,350,ptz3);
     theHistograms.fill("energia bosoni ricostruiti bene","Energia bosoni ricostruiti bene",15,0,1400,energiaz3);
     theHistograms.fill("eta bosoni ricostruiti bene","Eta bosoni ricostruiti bene",15,0,4.5,etaz1);
     if(ZZ->first().daughter(0).e()>ZZ->first().daughter(1).e()){
       theHistograms.fill("confronto leptone maggiore Z1","DeltaE leptone maggiore Z1",100,-20,20,ZZ->first().daughter(0).e()-enlep1);
       theHistograms.fill("confronto leptone minore Z1","DeltaE leptone minore Z1",100,-20,20,ZZ->first().daughter(1).e()-enlep2);
       theHistograms.fill("E leptone maggiore buono ricostruito","Energia leptone piu' energetico",15,0,1000,ZZ->first().daughter(0).e());
       theHistograms.fill("E leptone minore buono ricostruito","Energia leptone meno energetico",15,0,300,ZZ->first().daughter(1).e());}
     else{theHistograms.fill("confronto leptone maggiore Z1","DeltaE leptone maggiore Z1",100,-20,20,ZZ->first().daughter(1).e()-enlep1);
       theHistograms.fill("confronto leptone minore Z1","DeltaE leptone minore Z1",100,-20,20,ZZ->first().daughter(0).e()-enlep2);
       theHistograms.fill("E leptone maggiore buono ricostruito","Energia leptone piu' energetico",15,0,1000,ZZ->first().daughter(1).e());
       theHistograms.fill("E leptone minore buono ricostruito","Energia leptone meno energetico",15,0,300,ZZ->first().daughter(0).e());}}
       foreach(const phys::Boson<phys::Particle> genVBParticle,*genVBParticles){
     if((abs(genVBParticle.daughter(0).id())==11||abs(genVBParticle.daughter(0).id())==13)&&genVBParticle.daughter(0).id()==(-1)*genVBParticle.daughter(1).id()){
           if(physmath::deltaR(genVBParticle,ZZ->second())<dR2){
	   dR2=physmath::deltaR(genVBParticle,ZZ->second());
	   massaz4=genVBParticle.mass();
	   energiaz4=genVBParticle.e();
           ptz4=genVBParticle.pt();
	   etaz2=genVBParticle.eta();
	   if(genVBParticle.daughter(0).e()>genVBParticle.daughter(1).e()){
	   enlep3=genVBParticle.daughter(0).e();
	   enlep4=genVBParticle.daughter(1).e();}
         else{enlep3=genVBParticle.daughter(1).e();
	   enlep4=genVBParticle.daughter(0).e();}}}}
     if(dR2<0.1){
       theHistograms.fill("confronto massa Z2","Differenza massa generata/ricostruita Z2",40,-8,8,ZZ->second().mass()-massaz4);
       theHistograms.fill("confronto energia Z2","Differenza energia generata/ricostruita Z2",200,-40,40,ZZ->second().e()-energiaz4);
       theHistograms.fill("confronto pt Z2","Differenza pt generata/ricostruita Z2",150,-30,30,ZZ->second().pt()-ptz4);
       theHistograms.fill("confronto eta Z2","Differenza eta generata/ricostruita Z2",50,-0.1,0.1,ZZ->second().eta()-etaz2);
       theHistograms.fill("massa bosoni ricostruiti bene","Massa bosoni ricostruiti bene",15,70,110,massaz4);
       theHistograms.fill("pt bosoni ricostruiti bene","Pt bosoni ricostruiti bene",15,0,350,ptz4);
       theHistograms.fill("energia bosoni ricostruiti bene","Energia bosoni ricostruiti bene",15,0,1400,energiaz4);
       theHistograms.fill("eta bosoni ricostruiti bene","Eta bosoni ricostruiti bene",15,0,4.5,etaz2);
       if(ZZ->second().daughter(0).e()>ZZ->second().daughter(1).e()){
       theHistograms.fill("confronto leptone maggiore Z2","DeltaE leptone maggiore Z2",100,-20,20,ZZ->second().daughter(0).e()-enlep3);
       theHistograms.fill("confronto leptone minore Z2","DeltaE leptone minore Z2",100,-20,20,ZZ->second().daughter(1).e()-enlep4);
       theHistograms.fill("E leptone maggiore buono ricostruito","Energia leptone piu' energetico",15,0,1000,ZZ->second().daughter(0).e());
       theHistograms.fill("E leptone minore buono ricostruito","Energia leptone meno energetico",15,0,300,ZZ->second().daughter(1).e());}
     else{theHistograms.fill("confronto leptone maggiore Z2","DeltaE leptone maggiore Z2",100,-20,20,ZZ->second().daughter(1).e()-enlep3);
       theHistograms.fill("confronto leptone minore Z2","DeltaE leptone minore Z2",100,-20,20,ZZ->second().daughter(0).e()-enlep4);
        theHistograms.fill("E leptone maggiore buono ricostruito","Energia leptone piu' energetico",15,0,1000,ZZ->second().daughter(1).e());
       theHistograms.fill("E leptone minore buono ricostruito","Energia leptone meno energetico",15,0,300,ZZ->second().daughter(0).e());} }
	 }}
