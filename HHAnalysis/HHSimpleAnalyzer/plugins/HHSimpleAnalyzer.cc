// -*- C++ -*-
//
// Package:    HHSimpleAnalyzer
// Class:      HHSimpleAnalyzer
// 
/**\class HHSimpleAnalyzer HHSimpleAnalyzer.cc HHAnalysis/HHSimpleAnalyzer/src/HHSimpleAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Wed May 16 14:02:29 CEST 2007
// $Id: HHSimpleAnalyzer.cc,v 1.1.1.4 2008/02/25 15:54:04 auterman Exp $
//
//

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "HHAnalysis/HHSimpleAnalyzer/interface/HHSimpleAnalyzer.h" 

typedef HHSimpleAnalyzer<reco::MuonCollection, reco::Muon> HHSimpleAna;


//define this as a plug-in
DEFINE_FWK_MODULE(HHSimpleAna);