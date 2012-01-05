//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Dec 31 13:50:03 2011 by ROOT version 5.18/00
// from TTree AnaTree/
// found on file: QCDpseudo.root
//////////////////////////////////////////////////////////

#ifndef NtupleSelector_h
#define NtupleSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

class NtupleSelector : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           RunNr;
   Int_t           EvtNr;
   Int_t           LumiB;
   Float_t         Weight;
   Int_t           NrecoJetGen;
   Float_t         recoJetGenPx[200];   //[NrecoJetGen]
   Float_t         recoJetGenPy[200];   //[NrecoJetGen]
   Float_t         recoJetGenPz[200];   //[NrecoJetGen]
   Float_t         recoJetGenE[200];   //[NrecoJetGen]
   Float_t         recoJetGenPt[200];   //[NrecoJetGen]
   Float_t         recoJetGenPhi[200];   //[NrecoJetGen]
   Float_t         recoJetGenEta[200];   //[NrecoJetGen]
   Float_t         recoMetGen;
   Float_t         recoMetGenPhi;
   Int_t           NrecoJet;
   Float_t         recoJetPx[200];   //[NrecoJet]
   Float_t         recoJetPy[200];   //[NrecoJet]
   Float_t         recoJetPz[200];   //[NrecoJet]
   Float_t         recoJetE[200];   //[NrecoJet]
   Float_t         recoJetPt[200];   //[NrecoJet]
   Float_t         recoJetPhi[200];   //[NrecoJet]
   Float_t         recoJetEta[200];   //[NrecoJet]
   Float_t         recoMetCal;
   Float_t         recoMetCalPhi;
   Int_t           NVtx;
   Float_t         VtxZ[200];   //[NVtx]
   Int_t           NrecoMu;
   Float_t         recoMuQ[200];   //[NrecoMu]
   Float_t         recoMuPx[200];   //[NrecoMu]
   Float_t         recoMuPy[200];   //[NrecoMu]
   Float_t         recoMuPz[200];   //[NrecoMu]
   Float_t         recoMuEn[200];   //[NrecoMu]
   Float_t         recoMuPt[200];   //[NrecoMu]
   Float_t         recoMuPhi[200];   //[NrecoMu]
   Float_t         recoMuEta[200];   //[NrecoMu]
   Int_t           NrecoEle;
   Float_t         recoEleQ[200];   //[NrecoEle]
   Float_t         recoElePx[200];   //[NrecoEle]
   Float_t         recoElePy[200];   //[NrecoEle]
   Float_t         recoElePz[200];   //[NrecoEle]
   Float_t         recoEleEn[200];   //[NrecoEle]
   Float_t         recoElePt[200];   //[NrecoEle]
   Float_t         recoElePhi[200];   //[NrecoEle]
   Float_t         recoEleEta[200];   //[NrecoEle]
   Int_t           NrecoPho;
   Float_t         recoPhoPx[200];   //[NrecoPho]
   Float_t         recoPhoPy[200];   //[NrecoPho]
   Float_t         recoPhoPz[200];   //[NrecoPho]
   Float_t         recoPhoEn[200];   //[NrecoPho]
   Float_t         recoPhoPt[200];   //[NrecoPho]
   Float_t         recoPhoPhi[200];   //[NrecoPho]
   Float_t         recoPhoEta[200];   //[NrecoPho]

   // List of branches
   TBranch        *b_RunNr;   //!
   TBranch        *b_EvtNr;   //!
   TBranch        *b_LumiB;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_NrecoJetGen;   //!
   TBranch        *b_recoJetGenPx;   //!
   TBranch        *b_recoJetGenPy;   //!
   TBranch        *b_recoJetGenPz;   //!
   TBranch        *b_recoJetGenE;   //!
   TBranch        *b_recoJetGenPt;   //!
   TBranch        *b_recoJetGenPhi;   //!
   TBranch        *b_recoJetGenEta;   //!
   TBranch        *b_recoMetGen;   //!
   TBranch        *b_recoMetGenPhi;   //!
   TBranch        *b_NrecoJet;   //!
   TBranch        *b_recoJetPx;   //!
   TBranch        *b_recoJetPy;   //!
   TBranch        *b_recoJetPz;   //!
   TBranch        *b_recoJetE;   //!
   TBranch        *b_recoJetPt;   //!
   TBranch        *b_recoJetPhi;   //!
   TBranch        *b_recoJetEta;   //!
   TBranch        *b_recoMetCal;   //!
   TBranch        *b_recoMetCalPhi;   //!
   TBranch        *b_NVtx;   //!
   TBranch        *b_VtxZ;   //!
   TBranch        *b_NrecoMu;   //!
   TBranch        *b_recoMuQ;   //!
   TBranch        *b_recoMuPx;   //!
   TBranch        *b_recoMuPy;   //!
   TBranch        *b_recoMuPz;   //!
   TBranch        *b_recoMuEn;   //!
   TBranch        *b_recoMuPt;   //!
   TBranch        *b_recoMuPhi;   //!
   TBranch        *b_recoMuEta;   //!
   TBranch        *b_NrecoEle;   //!
   TBranch        *b_recoEleQ;   //!
   TBranch        *b_recoElePx;   //!
   TBranch        *b_recoElePy;   //!
   TBranch        *b_recoElePz;   //!
   TBranch        *b_recoEleEn;   //!
   TBranch        *b_recoElePt;   //!
   TBranch        *b_recoElePhi;   //!
   TBranch        *b_recoEleEta;   //!
   TBranch        *b_NrecoPho;   //!
   TBranch        *b_recoPhoPx;   //!
   TBranch        *b_recoPhoPy;   //!
   TBranch        *b_recoPhoPz;   //!
   TBranch        *b_recoPhoEn;   //!
   TBranch        *b_recoPhoPt;   //!
   TBranch        *b_recoPhoPhi;   //!
   TBranch        *b_recoPhoEta;   //!

   NtupleSelector(TTree * /*tree*/ =0) { }
   virtual ~NtupleSelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   //ClassDef(NtupleSelector,0);
};

#endif

#ifdef NtupleSelector_cxx
void NtupleSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunNr", &RunNr, &b_RunNr);
   fChain->SetBranchAddress("EvtNr", &EvtNr, &b_EvtNr);
   fChain->SetBranchAddress("LumiB", &LumiB, &b_LumiB);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("NrecoJetGen", &NrecoJetGen, &b_NrecoJetGen);
   fChain->SetBranchAddress("recoJetGenPx", &recoJetGenPx, &b_recoJetGenPx);
   fChain->SetBranchAddress("recoJetGenPy", &recoJetGenPy, &b_recoJetGenPy);
   fChain->SetBranchAddress("recoJetGenPz", &recoJetGenPz, &b_recoJetGenPz);
   fChain->SetBranchAddress("recoJetGenE", &recoJetGenE, &b_recoJetGenE);
   fChain->SetBranchAddress("recoJetGenPt", &recoJetGenPt, &b_recoJetGenPt);
   fChain->SetBranchAddress("recoJetGenPhi", &recoJetGenPhi, &b_recoJetGenPhi);
   fChain->SetBranchAddress("recoJetGenEta", &recoJetGenEta, &b_recoJetGenEta);
   fChain->SetBranchAddress("recoMetGen", &recoMetGen, &b_recoMetGen);
   fChain->SetBranchAddress("recoMetGenPhi", &recoMetGenPhi, &b_recoMetGenPhi);
   fChain->SetBranchAddress("NrecoJet", &NrecoJet, &b_NrecoJet);
   fChain->SetBranchAddress("recoJetPx", recoJetPx, &b_recoJetPx);
   fChain->SetBranchAddress("recoJetPy", recoJetPy, &b_recoJetPy);
   fChain->SetBranchAddress("recoJetPz", recoJetPz, &b_recoJetPz);
   fChain->SetBranchAddress("recoJetE", recoJetE, &b_recoJetE);
   fChain->SetBranchAddress("recoJetPt", recoJetPt, &b_recoJetPt);
   fChain->SetBranchAddress("recoJetPhi", recoJetPhi, &b_recoJetPhi);
   fChain->SetBranchAddress("recoJetEta", recoJetEta, &b_recoJetEta);
   fChain->SetBranchAddress("recoMetCal", &recoMetCal, &b_recoMetCal);
   fChain->SetBranchAddress("recoMetCalPhi", &recoMetCalPhi, &b_recoMetCalPhi);
   fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
   fChain->SetBranchAddress("VtxZ", &VtxZ, &b_VtxZ);
   fChain->SetBranchAddress("NrecoMu", &NrecoMu, &b_NrecoMu);
   fChain->SetBranchAddress("recoMuQ", &recoMuQ, &b_recoMuQ);
   fChain->SetBranchAddress("recoMuPx", &recoMuPx, &b_recoMuPx);
   fChain->SetBranchAddress("recoMuPy", &recoMuPy, &b_recoMuPy);
   fChain->SetBranchAddress("recoMuPz", &recoMuPz, &b_recoMuPz);
   fChain->SetBranchAddress("recoMuEn", &recoMuEn, &b_recoMuEn);
   fChain->SetBranchAddress("recoMuPt", &recoMuPt, &b_recoMuPt);
   fChain->SetBranchAddress("recoMuPhi", &recoMuPhi, &b_recoMuPhi);
   fChain->SetBranchAddress("recoMuEta", &recoMuEta, &b_recoMuEta);
   fChain->SetBranchAddress("NrecoEle", &NrecoEle, &b_NrecoEle);
   fChain->SetBranchAddress("recoEleQ", &recoEleQ, &b_recoEleQ);
   fChain->SetBranchAddress("recoElePx", &recoElePx, &b_recoElePx);
   fChain->SetBranchAddress("recoElePy", &recoElePy, &b_recoElePy);
   fChain->SetBranchAddress("recoElePz", &recoElePz, &b_recoElePz);
   fChain->SetBranchAddress("recoEleEn", &recoEleEn, &b_recoEleEn);
   fChain->SetBranchAddress("recoElePt", &recoElePt, &b_recoElePt);
   fChain->SetBranchAddress("recoElePhi", &recoElePhi, &b_recoElePhi);
   fChain->SetBranchAddress("recoEleEta", &recoEleEta, &b_recoEleEta);
   fChain->SetBranchAddress("NrecoPho", &NrecoPho, &b_NrecoPho);
   fChain->SetBranchAddress("recoPhoPx", &recoPhoPx, &b_recoPhoPx);
   fChain->SetBranchAddress("recoPhoPy", &recoPhoPy, &b_recoPhoPy);
   fChain->SetBranchAddress("recoPhoPz", &recoPhoPz, &b_recoPhoPz);
   fChain->SetBranchAddress("recoPhoEn", &recoPhoEn, &b_recoPhoEn);
   fChain->SetBranchAddress("recoPhoPt", &recoPhoPt, &b_recoPhoPt);
   fChain->SetBranchAddress("recoPhoPhi", &recoPhoPhi, &b_recoPhoPhi);
   fChain->SetBranchAddress("recoPhoEta", &recoPhoEta, &b_recoPhoEta);
}

Bool_t NtupleSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef NtupleSelector_cxx
