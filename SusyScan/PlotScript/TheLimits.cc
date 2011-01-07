#include "TheLimits.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <fstream>
#include <iostream>
#include <cmath>


void TheLimits::match()
{
  for (std::vector<SusyScan*>::iterator it=_scan.begin();it!=_scan.end();++it){
    bool match = false;
    for (std::vector<GeneratorMasses*>::const_iterator gt=_masses.begin();gt!=_masses.end();++gt){
      if ((*it)->Mzero==(*gt)->Mzero && 
          (*it)->Mhalf==(*gt)->Mhalf && 
	  (*it)->Azero==(*gt)->Azero &&
	  (*it)->TanBeta==(*gt)->TanBeta &&
	  (*it)->Mu==(*gt)->Mu ) {
        (*it)->M1 = (*gt)->M1;
        (*it)->M2 = (*gt)->M2;
        (*it)->M3 = (*gt)->M3;
        (*it)->MGL = (*gt)->MGL;
        (*it)->MUL = (*gt)->MUL;
        (*it)->MB1 = (*gt)->MB1;
        (*it)->MSN = (*gt)->MSN;
        (*it)->MNTAU = (*gt)->MNTAU;
        (*it)->MZ1 = (*gt)->MZ1;
        (*it)->MW1 = (*gt)->MW1;
        (*it)->MHL = (*gt)->MHL;
        (*it)->MUR = (*gt)->MUR;
        (*it)->MB2 = (*gt)->MB2;
        (*it)->MEL = (*gt)->MEL;
        (*it)->MTAU1 = (*gt)->MTAU1;
        (*it)->MZ2 = (*gt)->MZ2;
        (*it)->MW2 = (*gt)->MW2;
        (*it)->MHH = (*gt)->MHH;
        (*it)->MDL = (*gt)->MDL;
        (*it)->MT1 = (*gt)->MT1;
        (*it)->MER = (*gt)->MER;
        (*it)->MTAU2 = (*gt)->MTAU2;
        (*it)->MZ3 = (*gt)->MZ3;
        (*it)->MHA = (*gt)->MHA;
        (*it)->MDR = (*gt)->MDR;
        (*it)->MT2 = (*gt)->MT2;
        (*it)->MZ4 = (*gt)->MZ4;
        (*it)->MHp = (*gt)->MHp;
	match = true;
      }	  
    }
    //if (!match) std::cout << "No match for M0="<<(*it)->Mzero
    //                      << ", M12="<<(*it)->Mhalf<<std::endl;
  }
}

void TheLimits::FillGeneratorMasses(std::string file)
{
   std::ifstream masses_file;
   masses_file.open(file.c_str());
   while (1) {
      GeneratorMasses * p = new GeneratorMasses;
      masses_file >> p->Mzero
                  >> p->Mhalf
                  >> p->TanBeta
                  >> p->Mu  
                  >> p->Azero
                  >> p->Mtop  
                  >> p->muQ	  
                  >> p->Q	  
                  >> p->M1	  
                  >> p->M2
                  >> p->M3	  
                  >> p->MGL	  
                  >> p->MUL	  
                  >> p->MB1	  
                  >> p->MSN	  
                  >> p->MNTAU	  
                  >> p->MZ1	  
                  >> p->MW1	  
                  >> p->MHL	  
                  >> p->MUR	  
                  >> p->MB2	  
                  >> p->MEL	  
                  >> p->MTAU1	  
                  >> p->MZ2	  
                  >> p->MW2	  
                  >> p->MHH	  
                  >> p->MDL	  
                  >> p->MT1	  
                  >> p->MER	  
                  >> p->MTAU2	  
                  >> p->MZ3	  
                  >> p->MHA	  
                  >> p->MDR	  
                  >> p->MT2	  
                  >> p->MZ4	  
                  >> p->MHp;

      if (!masses_file.good()) break;
      if (fabs(p->Mu)!=1.) {
         std::cerr << "check lines near m0=" << p->Mzero << ", m1/2=" << p->Mhalf << std::endl;
         break;
      }	
      _masses.push_back( p );
   }
}