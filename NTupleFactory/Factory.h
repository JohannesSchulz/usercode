// $Id: Factory.h,v 1.9 2013/06/17 16:15:34 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#ifndef FACTORY_H
#define FACTORY_H

#include <vector>
#include <map>

#include "Reflex/Type.h"

class Analyzer;
class Selection;
class Event;
class Sample;
class TChain;

class Factory {
 public:
  Factory():tchain_(0),processed_(0),nentries_(0),processFraction_(1){}
  virtual ~Factory(){}
   
  //Initialization
  void SetSamples(std::vector<Sample*>& s){samples_=s;}
  void AddSample(Sample* s){samples_.push_back(s);}
  void AddAnalyzer(Selection* sel, Analyzer* p);
  void ProcessFraction(float fraction){processFraction_=fraction;}
    
  ///Process gets the next event and passes it through the Factory, i.e. through the selection and plots 
  virtual void Process();

 protected:
  Event * NextEvent();
 
 private:
  std::vector<Sample*> samples_;   
  std::map<Selection*,std::vector<Analyzer*> >  analyses_;  

  TChain * tchain_;
  std::vector<void*> Args_;
  ROOT::Reflex::Type cl_;
  Long64_t processed_, nentries_; 
  float processFraction_;
};

#endif
