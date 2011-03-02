#include "plot.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "PlotTools.h"
#include "TheLimits.h"
#include "GlobalFunctions.h"
#include "StyleSettings.h"

#include "TRint.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TStyle.h"

#include "TChain.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMarker.h"

#include <string>
#include <cmath>
#include <stdio.h>

int plot(int argc, char** argv)
{
   //interactive root session
   //TApplication theApp("App", 0, 0);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }   

   //Style stuff
   /*
   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
   gStyle->SetCanvasColor(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetFrameBorderMode(0);

   gStyle->SetTitleFillColor(0);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetTitleX(0.10);
   gStyle->SetTitleY(0.98);
   gStyle->SetTitleW(0.8);
   gStyle->SetTitleH(0.06);

   gStyle->SetErrorX(0);
   gStyle->SetStatColor(0);
   gStyle->SetStatBorderSize(0);
   gStyle->SetStatX(0);
   gStyle->SetStatY(0);
   gStyle->SetStatW(0);
   gStyle->SetStatH(0);

   gStyle->SetTitleFont(22);
   gStyle->SetLabelFont(22,"X");
   gStyle->SetLabelFont(22,"Y");
   gStyle->SetLabelFont(22,"Z");
   gStyle->SetLabelSize(0.03,"X");
   gStyle->SetLabelSize(0.03,"Y");
   gStyle->SetLabelSize(0.03,"Z");
   */
   util::StyleSettings::paperNoTitle();
   gStyle->SetPadBottomMargin(0.18);
 
   //gROOT->SetStyle("MyStyle");
   TCanvas * c1 = new TCanvas("c1","c1",600,600);
   //c1->SetFillStyle   ( 4000 );
   //c1->SetLeftMargin  ( 0.15 );
   //c1->SetRightMargin ( 0.15 );
   //c1->SetBottomMargin( 0.10 );
   c1->cd();
   
   //Get limits and generator masses ---------------------------------------------------
   TheLimits * genpoints = new TheLimits();
   //genpoints->Fill(argc, argv); 
   genpoints->Fill("limits_MHT_tb10-20110301/filelist.txt"); 

   TheLimits * genpointsHT = new TheLimits();
   genpointsHT->Fill("limits_HT_tb10-20110301/filelist.txt"); 

   //TheLimits * genpointsHT = new TheLimits();
   //genpointsHT->Fill("limits_HT_SigCont/filelist.txt"); 

   //TheLimits * genpointsMHT = new TheLimits();
   //genpointsMHT->Fill("limits_MHT_SigCont/filelist.txt"); 

   
   //Replace read limits with specific numbers
   //genpoints->OverwriteLimits("ABCD_MHT");
   
   genpoints->FillGeneratorMasses("GenScan_tb10.dat");
   genpoints->match();
   genpointsHT->FillGeneratorMasses("GenScan_tb10.dat");
   genpointsHT->match();
   //genpointsHT->FillGeneratorMasses("GenScan_tb10.dat");
   //genpointsHT->match();
   //genpointsMHT->FillGeneratorMasses("GenScan_tb10.dat");
   //genpointsMHT->match();

   //the plotting ----------------------------------------------------------------------
   //plotting helper functions
   PlotTools<SusyScan> * plotTools = new PlotTools<SusyScan>(genpoints->GetScan());
   PlotTools<SusyScan> * plotToolsHT = new PlotTools<SusyScan>(genpointsHT->GetScan());
   //PlotTools<SusyScan> * plotToolsHT = new PlotTools<SusyScan>(genpointsHT->GetScan());
   //PlotTools<SusyScan> * plotToolsMHT = new PlotTools<SusyScan>(genpointsMHT->GetScan());
   PlotTools<GeneratorMasses> * plotMasses = new PlotTools<GeneratorMasses>(genpoints->GetGeneratorMasses());

   //iso mass lines
   TGraph * gl500 = plotMasses->Line(Mzero, Mhalf, MGluino, 500, 0.8 );
   TGraph * gl600 = plotMasses->Line(Mzero, Mhalf, MGluino, 600, 0.8 );
   TGraph * gl700 = plotMasses->Line(Mzero, Mhalf, MGluino, 700, 0.8 );
   TGraph * gl800 = plotMasses->Line(Mzero, Mhalf, MGluino, 800, 0.8 );
   TGraph * gl900 = plotMasses->Line(Mzero, Mhalf, MGluino, 900, 0.8 );
   TGraph * gl1000 = plotMasses->Line(Mzero, Mhalf, MGluino, 1000, 0.8);
   TGraph * sq500 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 500, 1);
   TGraph * sq600 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 600, 1);
   TGraph * sq700 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 700, 1);
   TGraph * sq800 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 800, 1);
   TGraph * sq900 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 900, 1);
   TGraph * sq1000 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 1000, 1);

   TGraph * chi100 = plotMasses->Line(Mzero, Mhalf, MChi1, 50, 20);
   TGraph * cha200 = plotMasses->Line(Mzero, Mhalf, MCha1, 200, 20);
   cha200->SetLineColor(2);

   //the histograms
   c1->SetLogz(1);
   //h->SetMaximum(27);
   //h->SetMinimum(0.01);

/**/
   // cross-section in M0 - M1/2
   TH2F*hxsec = new TH2F("xsec",";m_{0} [GeV]; m_{1/2} [GeV]; cross section [pb]",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hxsec, Mzero, Mhalf, Xsection);
   hxsec->SetMinimum(0.01);
   //sq500->Draw();
   //gl500->Draw();
   hxsec->Draw("colz");
   c1->SaveAs("results/Xsection_m0_m12_tb10.pdf");
   std::string wait;
   //std::cin>>wait;

   // Observed Limit in M0 - M1/2
   TH2F*hobslimit = new TH2F("obslimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Limit [pb]",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hobslimit, Mzero, Mhalf, ObsXsecLimit);
   hobslimit->SetMinimum(0.01);
   hobslimit->Draw("colz");
   c1->SaveAs("results/ObsLimit_m0_m12_tb10.pdf");
   

   // Expected Limit in M0 - M1/2
   TH2F*hexplimit = new TH2F("explimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Limit [pb]",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hexplimit, Mzero, Mhalf, ExpXsecLimit);
   hexplimit->SetMinimum(0.01);
   hexplimit->Draw("colz");
   c1->SaveAs("results/ExpLimit_m0_m12_tb10.pdf");
   
   // Signal Acceptance in M0 - M1/2   
   c1->SetRightMargin ( 0.15 );
   TH2F*hsigacc = new TH2F("sigacc",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     100,0,1009.9,35,100,450);
   hsigacc->SetMinimum(0.01);
   hsigacc->SetMaximum(1.0);
   plotTools->Area(hsigacc, Mzero, Mhalf, SignalAcceptance);
   hsigacc->SetMinimum(0.01);
   hsigacc->SetMaximum(1.0);
   hsigacc->Draw("colz");
   //chi100->Draw();
   //cha200->Draw();
   //gl500 ->Draw();
   // sq500 ->Draw();
   c1->SaveAs("results/SigAccMHT_m0_m12_tb10.pdf");

   
   // Signal Acceptance in M0 - M1/2   
   TH2F*hsigaccHT = new TH2F("sigaccHT",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     100,0,1009.9,35,100,450);
   plotToolsHT->Area(hsigaccHT, Mzero, Mhalf, SignalAcceptance);
   hsigaccHT->SetMinimum(0.01);
   hsigaccHT->SetMaximum(1.0);
   hsigaccHT->Draw("colz");
   c1->SaveAs("results/SigAccHT_m0_m12_tb10.pdf");

   
   // Exp. Limit on Number of Signal Events in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hexplimitnsig = new TH2F("explimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL exp. limit signal events [# ]",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hexplimitnsig, Mzero, Mhalf, ExpNSignLimit);
   hexplimitnsig->SetMinimum(0.0);
   hexplimitnsig->SetMaximum(20);
   hexplimitnsig->Draw("colz");
   c1->SaveAs("results/ExpLimitOnNSig_m0_m12_tb10.pdf");
   
   // Obs. Limit on Number of Signal Events in M0 - M1/2
   TH2F*hobslimitnsig = new TH2F("obslimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL obs. limit signal events [# ]",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hobslimitnsig, Mzero, Mhalf, ObsNSignLimit);
   hobslimitnsig->SetMinimum(0.0);
   hobslimitnsig->SetMaximum(20);
   hobslimitnsig->Draw("colz");
   c1->SaveAs("results/ObsLimitOnNSig_m0_m12_tb10.pdf");
   
   c1->SetLogz(0);
   // Expected Exclusion in M0 - M1/2
   TH2F*hexpexcl = new TH2F("expexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hexpexcl, Mzero, Mhalf, ExpExclCL);
   hexpexcl->Draw("colz");
   c1->SaveAs("results/ExpExclusion_m0_m12_tb10.pdf");
   
   // Observed Exclusion in M0 - M1/2
   TH2F*hobsexcl = new TH2F("obsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hobsexcl, Mzero, Mhalf, ObsExclCL);
   hobsexcl->Draw("colz");
   c1->SaveAs("results/ObsExclusion_m0_m12_tb10.pdf");

   // Observed Exclusion in M0 - M1/2
   TH2F*hPLobsexcl = new TH2F("plobsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hPLobsexcl, Mzero, Mhalf, PLObsExclusion);
   hPLobsexcl->Draw("colz");
   c1->SaveAs("results/PL_ObsExclusion_m0_m12_tb10.pdf");

   // TestContours in M0 - M1/2
   c1->SetRightMargin ( 0.1 );
   TH2F*texcl = new TH2F("texcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   TH2F*tPLexpexcl=(TH2F*)texcl->Clone();
   plotTools->Area(tPLexpexcl, Mzero, Mhalf, PLExpExclusion);
   TH2F*tFCexpexcl=(TH2F*)texcl->Clone();
   plotTools->Area(tFCexpexcl, Mzero, Mhalf, FCExpExclusion);
   TH2F*testExcl=(TH2F*)texcl->Clone();
   plotTools->Area(testExcl, Mzero, Mhalf, NLOMCMCExpExclusion);
   //testExcl->SetMinimum(0.9);
   //testExcl->SetMaximum(1.1);
   std::vector<TGraph*> contours = plotTools->GetContours(testExcl,3); 
   //std::vector<TGraph*> contours = plotTools->GetContours(hobsexcl,3);
   //std::vector<TGraph*> contours = plotTools->GetContours(tPLexpexcl,3);
   //std::vector<TGraph*> contours = plotTools->GetContours(hPLobsexcl,3);
   //hPLexpexcl
   //hexcl->Draw("colz");
   testExcl->Draw("colz");
   //hobsexcl->Draw("colz");
   int col=kBlue-10;
   for (std::vector<TGraph*>::iterator cont=contours.begin(); cont!=contours.end(); ++cont){
     if (! *cont) continue;
     double x, y;
     (*cont)->GetPoint(0, x, y);
     (*cont)->SetLineColor(col);
     (*cont)->Draw("l");
     TLatex l; l.SetTextSize(0.04); l.SetTextColor(col++);
     char val[20];
     sprintf(val,"%d",(int)(cont-contours.begin()));
     l.DrawLatex(x,y,val); 
     //if (cont-contours.begin()>3) break;
   }
   c1->SaveAs("results/ExclusionTestContours_m0_m12_tb10.pdf");


   // Exclusion in M0 - M1/2
   TH2F*hexcl = new TH2F("hexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   TH2F*hs = new TH2F("hs","",100,0,1009.9,35,100,450);
   TGraph * gexpexcl         = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCL,       3,0, 2,2); 
   TGraph * gexpexcl_LO      = plotTools  ->GetContour(hs,Mzero,Mhalf,ExpExclCL,          3,0, 2,4); 
   TGraph * gobsexcl         = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOObsExclCL,       3,0, 2,1);
   TGraph * gMCMCexpexcl     = plotToolsHT->GetContour(hs,Mzero,Mhalf,MCMCExpExclusion,   3,0, 3,2); 
   TGraph * gMCMCobsexcl     = plotToolsHT->GetContour(hs,Mzero,Mhalf,MCMCObsExclusion,   3,0, 3,1); 
   TGraph * gFCexpexclHT     = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOFCExpExclusion,  3,0, 3,2); 
   TGraph * gFCexpexclHT_LO  = plotToolsHT->GetContour(hs,Mzero,Mhalf,FCExpExclusion,     3,0, 3,4); 
   TGraph * gFCexpexclMHT    = plotTools->GetContour(hs,Mzero,Mhalf,NLOFCExpExclusion,    3,0, 3,2); 
   TGraph * gFCexpexclMHT_LO = plotTools->GetContour(hs,Mzero,Mhalf,FCExpExclusion,       3,0, 3,4); 
   TGraph * gFCobsexclHT     = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOFCObsExclusion,  3,0, 3,1); 
   TGraph * gFCobsexclMHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOFCObsExclusion,  3,0, 3,1); 
   TGraph * gCLsExpExclHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCL,	  3,0, 1,2); 
   TGraph * gCLsObsExclHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOObsExclCL,	  3,0, 1,1); 
   TGraph * gCLsExpExclHT_LO = plotToolsHT->GetContour(hs,Mzero,Mhalf,ExpExclCL,	  3,0, 1,4); 
   TGraph * gCLsObsExclHT_LO = plotToolsHT->GetContour(hs,Mzero,Mhalf,ObsExclCL,	  3,0, 1,1); 
   TGraph * gCLsExpExclHTm1  = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCLm1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclHTp1  = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCLp1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclMHTm1 = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCLm1sigma,  3,0, 5,2); 
   TGraph * gCLsExpExclMHTp1 = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCLp1sigma,  3,0, 5,2); 
   TGraph * gCheck1 = (TGraph*)gobsexcl->Clone();
   TGraph * gCheck2 = (TGraph*)gexpexcl->Clone();
   Smooth( gCLsObsExclHT, 33); gCLsObsExclHT->SetLineWidth( 3 );
   Smooth( gCLsExpExclHT, 33 ); gCLsExpExclHT->SetLineWidth( 3 );
   Smooth( gCLsObsExclHT_LO, 33); gCLsObsExclHT_LO->SetLineWidth( 3 );
   Smooth( gCLsExpExclHT_LO, 33 ); gCLsExpExclHT_LO->SetLineWidth( 3 );
   Smooth( gexpexcl, 33 );
   Smooth( gobsexcl, 33 );
   Smooth( gobsexcl, 33 );
   Smooth( gexpexcl_LO, 33 );
   Smooth( gFCexpexclHT, 33 );
   Smooth( gFCexpexclHT_LO, 33 );gFCexpexclHT_LO->SetLineWidth( 3 );
   Smooth( gFCexpexclMHT, 33 );
   Smooth( gFCexpexclMHT_LO, 33 );
   Smooth( gFCobsexclHT, 33 );gFCobsexclHT->SetLineWidth( 3 );
   Smooth( gFCobsexclMHT, 33 );
   Smooth( gCLsExpExclHTm1, 33 );
   Smooth( gCLsExpExclHTp1, 33 );
   Smooth( gCLsExpExclMHTm1, 33 );
   Smooth( gCLsExpExclMHTp1, 33 );
   TGraph * CLsObsNLO = plotToolsHT->ChooseBest(gCLsObsExclHT,gobsexcl, gCLsObsExclHT,gobsexcl);
   TGraph * CLsExpNLO = plotToolsHT->ChooseBest(gCLsExpExclHT,gexpexcl, gCLsExpExclHT,gexpexcl);
   TGraph * CLsExpLO  = plotToolsHT->ChooseBest(gCLsExpExclHT_LO,gexpexcl_LO, gCLsExpExclHT_LO,gexpexcl_LO);
   TGraph * FCExpLO   = plotToolsHT->ChooseBest(gFCexpexclHT_LO,gFCexpexclMHT_LO,gFCexpexclHT_LO,gFCexpexclMHT_LO);
   TGraph * FCObsNLO  = plotToolsHT->ChooseBest(gFCobsexclHT,gFCobsexclMHT,gFCobsexclHT,gFCobsexclMHT);
   TGraph * gCLsExpExclp1 = plotToolsHT->ChooseBest(gCLsExpExclHTp1,gCLsExpExclMHTp1, gCLsExpExclHTp1,gCLsExpExclMHTp1);
   TGraph * gCLsExpExclm1 = plotToolsHT->ChooseBest(gCLsExpExclHTm1,gCLsExpExclMHTm1, gCLsExpExclHTm1,gCLsExpExclMHTm1);
   TGraph * gCLsExp1Sigma    = MakeBand(gCLsExpExclp1, gCLsExpExclm1);
   hexcl->GetYaxis()->SetTitleOffset(1.3);
   hexcl->Draw("colz");
   //set old exclusion Limits
   TGraph* LEP_ch = set_lep_ch(10);
   TGraph* LEP_sl = set_lep_sl(10);//slepton curve
   TGraph* TEV_sg_cdf = set_tev_sg_cdf(10);//squark gluino cdf
   TGraph* TEV_sg_d0 = set_tev_sg_d0(10);//squark gluino d0
   //TGraph* TEV_tlp_cdf = set_tev_tlp_cdf(10);//trilepton cdf
   //TGraph* TEV_tlp_d0 = set_tev_tlp_d0(10);//trilepton d0
   TGraph* stau = set_tev_stau(10);//stau 
   TGraph* TEV_sn_d0_1 = set_sneutrino_d0_1(10);
   TGraph* TEV_sn_d0_2 = set_sneutrino_d0_2(10);
   TGraphErrors* First  = getObserved_NLO_tanBeta10();
   TGraphErrors* Second = getExpected_NLO_tanBeta10();//getLO_jetMultis();
   TGraphErrors* Third  = getLO_tanBeta10();
   First->GetXaxis()->SetRangeUser(0,505);
   First->GetYaxis()->SetRangeUser(80,500);
   First->GetXaxis()->SetTitle("m_{0} (GeV)");
   First->GetYaxis()->SetTitle("m_{1/2} (GeV)");
   TSpline3 *sFirst = new TSpline3("sFirst",First);
   sFirst->SetLineColor(kBlue);
   sFirst->SetLineWidth(1);
   TSpline3 *sSecond = new TSpline3("sSecond",Second);
   sSecond->SetLineColor(kBlue);
   sSecond->SetLineStyle(2);
   sSecond->SetLineWidth(1);
   TSpline3 *sThird = new TSpline3("sThird",Third);
   sThird->SetLineColor(kBlue);
   sThird->SetLineStyle(4);
   sThird->SetLineWidth(1);
   TEV_sn_d0_1->Draw("fsame");
   TEV_sn_d0_2->Draw("fsame");
   TEV_sg_d0->Draw("fsame");
   TEV_sg_cdf->Draw("fsame");
   LEP_ch->Draw("fsame");
   LEP_sl->Draw("fsame");
   stau->Draw("fsame");
   TLatex b; b.SetTextSize(0.02); b.SetTextColor(1);
   b.DrawLatex( 10,380,"#tilde{#tau} LSP"); 
   b.DrawLatex(  6,150,"LEP2"); 
   b.DrawLatex( 22,138,"#tilde{l}^{#pm}"); 
   b.DrawLatex(360,110,"LEP2 #tilde{#chi}^{#pm}"); 
   b.DrawLatex(90,160,"CDF"); 
   //b.DrawLatex(248,129,"D0 #tilde{g}, #tilde{q}"); 
   b.DrawLatex( 80,180,"D0 #tilde{#nu}"); 
   if (gCLsExp1Sigma)    gCLsExp1Sigma->Draw("lf");
   sFirst->Draw("same");
   sSecond->Draw("same");
   sThird->Draw("same");
   //gCheck1->Draw("same");
   //gCheck2->Draw("same");
   //if (gexpexcl)         gexpexcl->Draw("l");
   //if (gobsexcl)         gobsexcl->Draw("l");
   //if (gexpexcl_LO)      gexpexcl_LO->Draw("l");
   //if (gMCMCexpexcl)     gMCMCexpexcl->Draw("l");
   //if (gCLsObsExclHT_LO) gCLsObsExclHT_LO->Draw("l");
   //if (gCLsExpExclHT_LO) gCLsExpExclHT_LO->Draw("l");
   //if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   //if (gCLsExpExclHT)    gCLsExpExclHT->Draw("l");
   //if (gobsexcl)       gobsexcl->Draw("l");
   //if (gPLobsexcl)     gPLobsexcl->Draw("l");
   //if (gFCobsexcl)     gFCobsexcl->Draw("l");
   //if (gMCMCobsexcl)   gMCMCobsexcl->Draw("l");
   //if (gFCexpexcl)     gFCexpexcl->Draw("l");
   //if (gPLexpexcl)     gPLexpexcl->Draw("l");
   //if (gFCexpexcl)     gFCexpexcl->Draw("l");
   
   CLsObsNLO->Draw("l");
   CLsExpNLO->Draw("l");
   CLsExpLO->Draw("l");
   FCExpLO->Draw("l");
   //FCObsNLO->Draw("l");
   
   //gl500->Draw("c"); gl600->Draw("c"); gl700->Draw("c"); gl800->Draw("c"); gl900->Draw("c"); gl1000->Draw("c");
   //sq500->Draw("c"); sq600->Draw("c"); sq700->Draw("c"); sq800->Draw("c"); sq900->Draw("c"); sq1000->Draw("c");
   TLegend * leg = new TLegend(0.6,0.66,0.96,0.90);
   leg->SetBorderSize(0);leg->SetFillColor(0);leg->SetFillStyle(4000);leg->SetTextFont(42);
   TGraph * expLeg = (TGraph*)CLsExpNLO->Clone();expLeg->SetFillStyle(gCLsExp1Sigma->GetFillStyle());expLeg->SetFillColor(gCLsExp1Sigma->GetFillColor());
   leg->SetHeader("CMS preliminary, 95%CL");
   leg->AddEntry(expLeg,   "Expected (CLs, NLO)","l");
   leg->AddEntry(CLsObsNLO,"Observed (CLs, NLO)","l");
   leg->AddEntry(CLsExpLO, "Expected (CLs, LO)","l");
   leg->AddEntry(FCExpLO,  "Expected (FC, LO)","l");
   //if (gobsexcl) leg->AddEntry(gobsexcl,"Observed (MHT, CLs, TLimit)","l");
   //if (gCLsExpExclHT) leg->AddEntry(expLeg       ,"NLO Expected (HT, CLs)","l");
   //if (gCLsObsExclHT) leg->AddEntry(gCLsObsExclHT,"NLO Observed (HT, CLs)","l");
   //if (gCLsObsExclHT_LO) leg->AddEntry(gCLsObsExclHT_LO,"LO Observed (HT, CLs)","l");
   //if (gCLsExpExclHT_LO) leg->AddEntry(gCLsExpExclHT_LO,"LO Expected (HT, CLs)","l");
   //if (gPLobsexcl) leg->AddEntry(gPLobsexcl,"Observed (PL, RooStat)","l");
   //if (gPLexpexcl) leg->AddEntry(gPLexpexcl,"Expected (PL, RooStat)","l");
   //if (gFCobsexcl) leg->AddEntry(gFCobsexcl,"NLO Observed (HT, FC, RooStat)","l");
   //if (gFCexpexcl) leg->AddEntry(gFCexpexcl,"NLO Expected (HT, FC, RooStat)","l");
   //if (gMCMCobsexcl) leg->AddEntry(gMCMCobsexcl,"Observed (MHT, MCMC, RooStat)","l");
   //if (gMCMCexpexcl) leg->AddEntry(gMCMCexpexcl,"LO Expected (HT, MCMC, RooStat)","l");
   //if (gexpexcl) leg->AddEntry(gexpexcl,"NLO Expected (MHT, CLs)","l");
   //if (gobsexcl) leg->AddEntry(gobsexcl,"NLO Observed (MHT, CLs)","l");
   if (sSecond) leg->AddEntry(sSecond,"#alpha_{T} Expected (FC, NLO)");
   if (sFirst)  leg->AddEntry(sFirst, "#alpha_{T} Observed (FC, NLO)");
   if (sThird)  leg->AddEntry(sThird, "#alpha_{T} Expected (FC, LO)");
   leg->Draw();
   gPad->RedrawAxis();
   c1->SaveAs("results/Exclusion_m0_m12_tb10.pdf");


   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hsigcont = new TH2F("sigcont",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     100,0,1009.9,35,100,450);
   plotToolsHT->Area(hsigcont, Mzero, Mhalf, SignalContamination);
   hsigcont->SetMinimum(0.01);
   hsigcont->SetMaximum(20);
   hsigcont->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SignalContamination_HT_m0_m12_tb10.pdf");
   
   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hsigcontMHT = new TH2F("sigcontMHT",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsigcontMHT, Mzero, Mhalf, SignalContamination);
   hsigcontMHT->SetMinimum(0.01);
   hsigcontMHT->SetMaximum(20);
   hsigcontMHT->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SignalContamination_MHT_m0_m12_tb10.pdf");
   
   // Signal JEC Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_jec_mht = new TH2F("sigjecmht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal JEC uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_jec_mht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_mht->SetMinimum(0.0);
   hsig_jec_mht->SetMaximum(0.25);
   hsig_jec_mht->SetContour(10);
   hsig_jec_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SigJEC_MHT_m0_m12_tb10.pdf");
   
   // Signal JEC Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_jec_ht = new TH2F("sigjecht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal JEC uncertainty",
                     100,0,1009.9,35,100,450);
   plotToolsHT->Area(hsig_jec_ht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_ht->SetMinimum(0.0);
   hsig_jec_ht->SetMaximum(0.25);
   hsig_jec_ht->SetContour(10);
   hsig_jec_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SigJEC_HT_m0_m12_tb10.pdf");
   

   // Signal MuIso Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_mht = new TH2F("sigMuIsomht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_MuIso_mht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_mht->SetMinimum(0.0);hsig_MuIso_mht->GetZaxis()->SetTitleOffset(1.3);
   hsig_MuIso_mht->SetMaximum(0.05);
   hsig_MuIso_mht->SetContour(5);
   hsig_MuIso_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SigMuIso_MHT_m0_m12_tb10.pdf");
   
   // Signal MuIso Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_ht = new TH2F("sigMuIsoht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     100,0,1009.9,35,100,450);
   plotToolsHT->Area(hsig_MuIso_ht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_ht->SetMinimum(0.0);hsig_MuIso_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_MuIso_ht->SetMaximum(0.05);
   hsig_MuIso_ht->SetContour(5);
   hsig_MuIso_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SigMuIso_HT_m0_m12_tb10.pdf");
   

   // Signal kFactor Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_kFactorUncert_mht = new TH2F("sigkFactorUncertmht",";m_{0} [GeV]; m_{1/2} [GeV]; signal scale uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_kFactorUncert_mht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactorUncert_mht->SetMinimum(0.1);hsig_kFactorUncert_mht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactorUncert_mht->SetMaximum(0.2);
   hsig_kFactorUncert_mht->SetContour(10);
   hsig_kFactorUncert_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SigkFactorUncert_MHT_m0_m12_tb10.pdf");
   
   // Signal kFactor Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_kFactorUncert_ht = new TH2F("sigkFactorUncertht",";m_{0} [GeV]; m_{1/2} [GeV]; signal scale uncertainty",
                     100,0,1009.9,35,100,450);
   plotToolsHT->Area(hsig_kFactorUncert_ht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactorUncert_ht->SetMinimum(0.1);hsig_kFactorUncert_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactorUncert_ht->SetMaximum(0.2);
   hsig_kFactorUncert_ht->SetContour(10);
   hsig_kFactorUncert_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SigkFactorUncert_HT_m0_m12_tb10.pdf");
   
   // Signal kFactor HT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_ht = new TH2F("sigkFactorht",";m_{0} [GeV]; m_{1/2} [GeV]; NLO signal k-factor",
                     100,0,1009.9,35,100,450);
   plotToolsHT->Area(hsig_kFactor_ht, Mzero, Mhalf, SignalKfactor);
   hsig_kFactor_ht->SetMinimum(1.0);hsig_kFactor_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactor_ht->SetMaximum(2.4);
   hsig_kFactor_ht->SetContour(7);
   hsig_kFactor_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SigkFactor_HT_m0_m12_tb10.pdf");
   
   // Signal kFactor MHT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_mht = new TH2F("sigkFactormht",";m_{0} [GeV]; m_{1/2} [GeV]; NLO signal k-factor",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_kFactor_mht, Mzero, Mhalf, SignalKfactor);
   hsig_kFactor_mht->SetMinimum(1.0);hsig_kFactor_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactor_mht->SetMaximum(2.4);
   hsig_kFactor_mht->SetContour(7);
   hsig_kFactor_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SigkFactor_MHT_m0_m12_tb10.pdf");
   


   //-----------------------------------------------------------------------------------
   c1->SetLogz(1);

   TGraph * mz500 = plotMasses->Line( MSquarkL, MGluino, Mzero, 500, 1);
   TGraph * mh250 = plotMasses->Line( MSquarkL, MGluino, Mhalf, 250, 1);

   // cross-section in squark - gluino mass
   TH2F*hxsec_qg = new TH2F("xsec_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; cross section [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hxsec_qg, MSquarkL, MGluino, Xsection);
   hxsec_qg->SetMinimum(0.01);
   hxsec_qg->Draw("colz");
   mz500->Draw();
   mh250->Draw();
   c1->SaveAs("results/Xsection_mSql_mGl_tb10.pdf");
   
   // Observed Limit in squark - gluino mass
   TH2F*hobslimit_qg = new TH2F("obslimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Limit [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobslimit_qg, MSquarkL, MGluino, ObsXsecLimit);
   hobslimit_qg->SetMinimum(0.01);
   hobslimit_qg->Draw("colz");
   c1->SaveAs("results/ObsLimit_mSql_mGl_tb10.pdf");
   
   // Expected Limit in squark - gluino mass
   TH2F*hexplimit_qg = new TH2F("explimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Limit [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexplimit_qg, MSquarkL, MGluino, ExpXsecLimit);
   hexplimit_qg->SetMinimum(0.01);
   hexplimit_qg->Draw("colz");
   c1->SaveAs("results/ExpLimit_mSql_mGl_tb10.pdf");
   
   // Signal Acceptance in squark - gluino mass
   TH2F*hsigacc_qg = new TH2F("sigacc_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Acceptance",
                     60,200,1400,50,200,1200);
   plotTools->Area(hsigacc_qg, MSquarkL, MGluino, SignalAcceptance);
   hsigacc_qg->SetMinimum(0.01);
   hsigacc_qg->SetMaximum(1.0);
   hsigacc_qg->Draw("colz");
   c1->SaveAs("results/SigAccMHT_mSql_mGl_tb10.pdf");
   
   // Signal Acceptance in squark - gluino mass
   TH2F*hsigaccHT_qg = new TH2F("sigaccHT_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Acceptance",
                     60,200,1400,50,200,1200);
   plotToolsHT->Area(hsigaccHT_qg, MSquarkL, MGluino, SignalAcceptance);
   hsigaccHT_qg->SetMinimum(0.01);
   hsigaccHT_qg->SetMaximum(1.0);
   hsigaccHT_qg->Draw("colz");
   c1->SaveAs("results/SigAccHT_mSql_mGl_tb10.pdf");

   
   // Signal Contamination in squark - gluino mass
   c1->SetLogz(0);
   TH2F*hsigcont_qg = new TH2F("sigacc_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Contamination in bkg yield",
                     60,200,1400,50,200,1200);
   plotTools->Area(hsigcont_qg, MSquarkL, MGluino, SignalContamination);
   hsigcont_qg->SetMinimum(0.0);
   hsigcont_qg->SetMaximum(20.0);
   hsigcont_qg->Draw("colz");
   c1->SaveAs("results/SignalContamination_MHT_mSql_mGl_tb10.pdf");
   
   // Signal Contamination in squark - gluino mass
   TH2F*hsigcontHT_qg = new TH2F("sigaccHT_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Acceptance in bkg yield",
                     60,200,1400,50,200,1200);
   plotToolsHT->Area(hsigcontHT_qg, MSquarkL, MGluino, SignalContamination);
   hsigcontHT_qg->SetMinimum(0.0);
   hsigcontHT_qg->SetMaximum(20.0);
   hsigcontHT_qg->Draw("colz");
   c1->SaveAs("results/SignalContamination_HT_mSql_mGl_tb10.pdf");
/*
   // Signal JEC Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_jec_mht = new TH2F("sigjecmht",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal JEC uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_jec_mht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_mht->SetMinimum(0.0);
   hsig_jec_mht->SetMaximum(0.25);
   hsig_jec_mht->SetContour(10);
   hsig_jec_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SigJEC_MHT_m0_m12_tb10.pdf");
   
   // Signal JEC Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_jec_ht = new TH2F("sigjecht",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal JEC uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_jec_ht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_ht->SetMinimum(0.0);
   hsig_jec_ht->SetMaximum(0.25);
   hsig_jec_ht->SetContour(10);
   hsig_jec_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SigJEC_HT_m0_m12_tb10.pdf");
   

   // Signal MuIso Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_mht = new TH2F("sigMuIsomht",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal #mu_{iso} uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_MuIso_mht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_mht->SetMinimum(0.0);
   hsig_MuIso_mht->SetMaximum(0.05);
   hsig_MuIso_mht->SetContour(5);
   hsig_MuIso_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SigMuIso_MHT_m0_m12_tb10.pdf");
   
   // Signal MuIso Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_ht = new TH2F("sigMuIsoht",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal #mu_{iso} uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_MuIso_ht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_ht->SetMinimum(0.0);
   hsig_MuIso_ht->SetMaximum(0.05);
   hsig_MuIso_ht->SetContour(5);
   hsig_MuIso_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SigMuIso_HT_m0_m12_tb10.pdf");
   

   // Signal kFactor Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_mht = new TH2F("sigkFactormht",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal #mu_{iso} uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_kFactor_mht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactor_mht->SetMinimum(0.1);
   hsig_kFactor_mht->SetMaximum(0.2);
   hsig_kFactor_mht->SetContour(10);
   hsig_kFactor_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SigkFactor_MHT_m0_m12_tb10.pdf");
   
   // Signal kFactor Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_ht = new TH2F("sigkFactorht",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal #mu_{iso} uncertainty",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hsig_kFactor_ht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactor_ht->SetMinimum(0.1);
   hsig_kFactor_ht->SetMaximum(0.2);
   hsig_kFactor_ht->SetContour(10);
   hsig_kFactor_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SigkFactor_HT_m0_m12_tb10.pdf");
   
*/   
   // Exp. Limit on Number of Signal Events in squark - gluino mass
   c1->SetLogz(0);
   TH2F*hexplimitnsig_qg = new TH2F("explimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL exp. limit signal events [# ]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexplimitnsig_qg, MSquarkL, MGluino, ExpNSignLimit);
   hexplimitnsig_qg->SetMinimum(0.0);
   hexplimitnsig_qg->SetMaximum(20);
   hexplimitnsig_qg->Draw("colz");
   c1->SaveAs("results/ExpLimitOnNSig_mSql_mGl_tb10.pdf");
   
   // Obs. Limit on Number of Signal Events in squark - gluino mass
   TH2F*hobslimitnsig_qg = new TH2F("obslimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL obs. limit signal events [# ]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobslimitnsig_qg, MSquarkL, MGluino, ObsNSignLimit);
   hobslimitnsig_qg->SetMinimum(0.0);
   hobslimitnsig_qg->SetMaximum(20);
   hobslimitnsig_qg->Draw("colz");
   c1->SaveAs("results/ObsLimitOnNSig_mSql_mGl_tb10.pdf");
   
   c1->SetLogz(0);
   // Expected Exclusion in squark - gluino mass
   TH2F*hexpexcl_qg = new TH2F("expexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexpexcl_qg, MSquarkL, MGluino, ExpExclCL);
   hexpexcl_qg->Draw("colz");
   c1->SaveAs("results/ExpExclusion_mSql_mGl_tb10.pdf");
   
   // Observed Exclusion in squark - gluino mass
   TH2F*hobsexcl_qg = new TH2F("obsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobsexcl_qg, MSquarkL, MGluino, ObsExclCL);
   //std::vector<TGraph *> vobsexcl_qg = plotTools->GetContours(hobsexcl_qg);
   hobsexcl_qg->Draw("colz");
   //for (std::vector<TGraph *>::iterator g=vobsexcl_qg.begin();g!=vobsexcl_qg.end();++g){
   //  if (! *g) continue;
   // // (*g)->Draw("l");
   //}
   c1->SaveAs("results/ObsExclusion_mSql_mGl_tb10.pdf");
   
   // TestContours in M0 - M1/2
   TH2F*texcl_qg = new TH2F("texcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   TH2F*tPLobsexcl_qg=(TH2F*)texcl_qg->Clone();
   plotTools->Area(tPLobsexcl_qg, MSquarkL, MGluino, PLObsExclusion);
   TH2F*tPLexpexcl_qg=(TH2F*)texcl_qg->Clone();
   plotTools->Area(tPLexpexcl_qg, MSquarkL, MGluino, PLExpExclusion);
   //std::vector<TGraph*> contours_qg = plotTools->GetContours(hexpexcl_qg,3); 
   //std::vector<TGraph*> contours_qg = plotTools->GetContours(hobsexcl_qg,3);
   std::vector<TGraph*> contours_qg = plotTools->GetContours(tPLobsexcl_qg,3);
   //std::vector<TGraph*> contours_qg = plotTools->GetContours(tPLexpexcl_qg,3);
   //hPLexpexcl_qg
   //hexcl_qg->Draw("colz");
   //hexpexcl_qg->Draw("colz");
   //hobsexcl_qg->Draw("colz");
   tPLobsexcl_qg->Draw("colz");
   int col_gl=kBlue-10;
   for (std::vector<TGraph*>::iterator cont=contours_qg.begin(); cont!=contours_qg.end(); ++cont){
     if (! *cont) continue;
     double x, y;
     (*cont)->GetPoint(0, x, y);
     (*cont)->SetLineColor(col_gl);(*cont)->SetLineWidth(3);
     (*cont)->Draw("l");
     TLatex l; l.SetTextSize(0.04); l.SetTextColor(col_gl++);
     char val[20];
     sprintf(val,"%d",(int)(cont-contours_qg.begin()));
     l.DrawLatex(x,y,val); 
     if (cont-contours_qg.begin()>3) break;
   }
   c1->SaveAs("results/ExclusionTestContours_mSql_mGl_tb10.pdf");

   // Observed Exclusion in squark - gluino mass
   TH2F*hPLobsexcl_qg = new TH2F("plobsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hPLobsexcl_qg, MSquarkL, MGluino, PLObsExclusion);
   //hPLobsexcl_qg->Draw("colz");
   //c1->SaveAs("results/PL_ObsExclusion_mSql_mGl_tb10.pdf");
   


   // Exclusion in squark - gluino mass ----------------------------------------
   TH2F*h_qg = new TH2F("hExcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,0,1200,50,0,1200);
   TH2F*hExcl_qg = new TH2F("h_qg","",60,200,1400,50,200,1200);
   TH2F*hexpFC_HT_LO_qg=(TH2F*)hExcl_qg->Clone();
   plotToolsHT->Area(hexpFC_HT_LO_qg, MSquarkL, MGluino, FCExpExclusion);   
   TH2F*hexpFC_HT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotToolsHT->Area(hexpFC_HT_NLO_qg, MSquarkL, MGluino, NLOFCExpExclusion);   
   TH2F*hexpFC_MHT_LO_qg=(TH2F*)hExcl_qg->Clone();
   plotTools->Area(hexpFC_MHT_LO_qg, MSquarkL, MGluino, FCExpExclusion);   
   TH2F*hexpFC_MHT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotTools->Area(hexpFC_MHT_NLO_qg, MSquarkL, MGluino, NLOFCExpExclusion);   
   //
   TH2F*hexpCLs_HT_LO_qg=(TH2F*)hExcl_qg->Clone();
   plotToolsHT->Area(hexpCLs_HT_LO_qg, MSquarkL, MGluino, ExpExclCL);   
   TH2F*hexpCLs_HT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotToolsHT->Area(hexpCLs_HT_NLO_qg, MSquarkL, MGluino, NLOExpExclCL);   
   TH2F*hexpCLs_MHT_LO_qg=(TH2F*)hExcl_qg->Clone();
   plotTools->Area(hexpCLs_MHT_LO_qg, MSquarkL, MGluino, ExpExclCL);   
   TH2F*hexpCLs_MHT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotTools->Area(hexpCLs_MHT_NLO_qg, MSquarkL, MGluino, NLOExpExclCL);   
   //
   TH2F*hexp1p_HT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotToolsHT->Area(hexp1p_HT_NLO_qg, MSquarkL, MGluino, NLOExpExclCLp1sigma);   
   TH2F*hexp1p_MHT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotTools->Area(hexp1p_MHT_NLO_qg, MSquarkL, MGluino, NLOExpExclCLp1sigma);   
   TH2F*hexp1m_HT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotToolsHT->Area(hexp1m_HT_NLO_qg, MSquarkL, MGluino, NLOExpExclCLm1sigma);   
   TH2F*hexp1m_MHT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotTools->Area(hexp1m_MHT_NLO_qg, MSquarkL, MGluino, NLOExpExclCLm1sigma);   

   TH2F*hobsCLs_HT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotToolsHT->Area(hobsCLs_HT_NLO_qg, MSquarkL, MGluino, NLOObsExclCL);   
   TH2F*hobsCLs_MHT_NLO_qg=(TH2F*)hExcl_qg->Clone();
   plotTools->Area(hobsCLs_MHT_NLO_qg, MSquarkL, MGluino, NLOObsExclCL);   
   //
   TH2 * hexpFC_LO_qg   = plotToolsHT->BinWiseOr(hexpFC_HT_LO_qg,   hexpFC_MHT_LO_qg);
   TH2 * hexpCLs_LO_qg  = plotToolsHT->BinWiseOr(hexpCLs_HT_LO_qg,  hexpCLs_MHT_LO_qg);
   TH2 * hexpCLs_NLO_qg = plotToolsHT->BinWiseOr(hexpCLs_HT_NLO_qg, hexpCLs_MHT_NLO_qg);
   TH2 * hobsCLs_NLO_qg = plotToolsHT->BinWiseOr(hobsCLs_HT_NLO_qg, hobsCLs_MHT_NLO_qg);
   TH2 * hexp1p_NLO_qg  = plotToolsHT->BinWiseOr(hexp1p_HT_NLO_qg,  hexp1p_MHT_NLO_qg);
   TH2 * hexp1m_NLO_qg  = plotToolsHT->BinWiseOr(hexp1m_HT_NLO_qg,  hexp1m_MHT_NLO_qg);
   //
   TGraph * gexpFC_LO_qg = plotToolsHT->GetContour(hexpFC_LO_qg,3,0); 
   gexpFC_LO_qg->SetLineStyle(4); gexpFC_LO_qg->SetLineColor(3);gexpFC_LO_qg->SetLineWidth(2);
   TGraph * gexpCLs_LO_qg = plotToolsHT->GetContour(hexpCLs_LO_qg,3,0); 
   gexpCLs_LO_qg->SetLineStyle(4); gexpCLs_LO_qg->SetLineColor(1);gexpCLs_LO_qg->SetLineWidth(2);
   TGraph * gexpCLs_NLO_qg = plotToolsHT->GetContour(hexpCLs_NLO_qg,3,0); 
   gexpCLs_NLO_qg->SetLineStyle(2); gexpCLs_NLO_qg->SetLineColor(1);gexpCLs_NLO_qg->SetLineWidth(2);
   TGraph * gobsCLs_NLO_qg = plotToolsHT->GetContour(hobsCLs_NLO_qg,3,0); 
   gobsCLs_NLO_qg->SetLineStyle(1); gobsCLs_NLO_qg->SetLineColor(1);gobsCLs_NLO_qg->SetLineWidth(2);
   TGraph * gexp1p_NLO_qg = plotToolsHT->GetContour(hexp1p_NLO_qg,3,0); 
   TGraph * gexp1m_NLO_qg = plotToolsHT->GetContour(hexp1m_NLO_qg,3,0); 
   Smooth(gexpFC_LO_qg, 11);
   Smooth(gexpCLs_LO_qg, 11);
   Smooth(gexpCLs_NLO_qg, 11);
   Smooth(gobsCLs_NLO_qg, 11);
   Smooth(gexp1p_NLO_qg, 11);
   Smooth(gexp1m_NLO_qg, 11);

   TGraph * g1Sigma_qg    = MakeBand(gexp1p_NLO_qg, gexp1m_NLO_qg);
   g1Sigma_qg->SetFillColor(5);
   h_qg->GetYaxis()->SetTitleOffset(1.5);
   h_qg->Draw("");
   TGraph * sqLEP = sq_LEP(); sqLEP->Draw("f");
   TGraph * glTEV = gl_TEV(); glTEV->Draw("f");
   TGraph * glCDF = gl_CDF(); glCDF->Draw("f");
   TGraph * glDEZ = gl_DEZ(); glDEZ->Draw("f");

   g1Sigma_qg->Draw("f");
   gexpFC_LO_qg->Draw("l");
   gexpCLs_LO_qg->Draw("l");
   gexpCLs_NLO_qg->Draw("l");
   gobsCLs_NLO_qg->Draw("l");
   //TGraph * glWHT = gl_WHT(); glWHT->Draw("f");
   b.DrawLatex( 800, 50 ,"Tevatron RunI"); 
   b.DrawLatex( 800, 200,"CDF RunII"); 
   b.DrawLatex( 800, 240,"Dzero RunII"); 
   b.DrawLatex( 100,600,"LEP #tilde{q}#tilde{q}"); 
   //
   TLegend * l_qg = new TLegend(0.45,0.7,0.85,0.89);
   l_qg->SetBorderSize(0);l_qg->SetFillColor(0);l_qg->SetTextFont(42);
   l_qg->SetHeader("CMS preliminary, 95% CL");
   TGraph * expL_qg = (TGraph*)gexpCLs_NLO_qg->Clone();expL_qg->SetFillStyle(g1Sigma_qg->GetFillStyle());expL_qg->SetFillColor(g1Sigma_qg->GetFillColor());
   if (expL_qg)        l_qg->AddEntry(expL_qg,       "Expected (CLs, NLO)","lf");
   if (gobsCLs_NLO_qg) l_qg->AddEntry(gobsCLs_NLO_qg,"Observed (CLs, NLO)","l");
   if (gexpCLs_LO_qg)  l_qg->AddEntry(gexpCLs_LO_qg, "Expected (CLs, LO)","l");
   if (gexpFC_LO_qg)   l_qg->AddEntry(gexpFC_LO_qg,  "Expected (FC,  LO)","l");
   l_qg->Draw();
   gPad->RedrawAxis();
   c1->SaveAs("results/Exclusion_mSql_mGl_tb10.pdf");


   TH2F*hexcl_qg = new TH2F("hexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1000);
   TGraph * gexpexcl_qg         = plotTools->GetContour(hexcl_qg,MSquarkL,MGluino,NLOExpExclCL,       3,0, 2,2); 
   TGraph * gexpexcl_qg_LO      = plotTools->GetContour(hexcl_qg,MSquarkL,MGluino,ExpExclCL,       3,0, 2,4); 
   TGraph * gobsexcl_qg         = plotTools->GetContour(hexcl_qg,MSquarkL,MGluino,NLOObsExclCL,       3,0, 2,1);
   TGraph * gMCMCexpexcl_qg     = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,MCMCExpExclusion,3,0, 3,2); 
   TGraph * gMCMCobsexcl_qg     = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,MCMCObsExclusion,3,0, 3,1); 
   TGraph * gCLsExpExclHT_qg    = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOExpExclCL,       3,0, 1,2); 
   TGraph * gCLsExpExclHT_qg_LO = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,ExpExclCL,       3,0, 1,4); 
   TGraph * gCLsObsExclHT_qg    = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOObsExclCL,       3,0, 1,1); 
   TGraph * gCLsExpExclHTm1_qg  = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOExpExclCLm1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclHTp1_qg  = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOExpExclCLp1sigma,3,0, 5,2); 
   Smooth(gexpexcl_qg, 11);
   Smooth(gexpexcl_qg_LO, 11);
   Smooth(gobsexcl_qg, 11);
   Smooth(gCLsExpExclHT_qg, 11);
   Smooth(gCLsExpExclHT_qg_LO, 11);
   Smooth(gCLsObsExclHT_qg, 11);
   Smooth(gCLsExpExclHTm1_qg, 11);
   Smooth(gCLsExpExclHTp1_qg, 11);
   TGraph * gCLsExp1Sigma_qg    = MakeBand(gCLsExpExclHTp1_qg, gCLsExpExclHTm1_qg);
   hexcl_qg->Draw("");
   //
   if (gCLsExp1Sigma_qg) gCLsExp1Sigma_qg->Draw("lf");   
   if (gobsexcl_qg)	 gobsexcl_qg->Draw("l");
   if (gCLsObsExclHT_qg) gCLsObsExclHT_qg->Draw("l");
   if (gexpexcl_qg)	 gexpexcl_qg->Draw("l");
   if (gCLsExpExclHT_qg) gCLsExpExclHT_qg->Draw("l");
   if (gexpexcl_qg_LO)      gexpexcl_qg_LO->Draw("l");
   if (gCLsExpExclHT_qg_LO) gCLsExpExclHT_qg_LO->Draw("l");
   
   TLegend * leg_qg = new TLegend(0.45,0.78,0.85,0.89);
   leg_qg->SetBorderSize(0);leg_qg->SetFillColor(0);
   TGraph * expLeg_qg = (TGraph*)gCLsExpExclHT_qg->Clone();expLeg_qg->SetFillStyle(gCLsExp1Sigma_qg->GetFillStyle());expLeg_qg->SetFillColor(gCLsExp1Sigma_qg->GetFillColor());
   if (gobsexcl_qg) leg_qg->AddEntry(gobsexcl_qg,"NLO Observed (MHT, CLs)","l");
   if (gexpexcl_qg) leg_qg->AddEntry(gexpexcl_qg,"NLO Expected (MHT, CLs)","l");
   if (gexpexcl_qg_LO) leg_qg->AddEntry(gexpexcl_qg_LO,"LO Expected (MHT, CLs)","l");
   if (gCLsObsExclHT_qg) leg_qg->AddEntry(gCLsObsExclHT_qg,"NLO Observed (HT, CLs)","l");
   if (gCLsExpExclHT_qg) leg_qg->AddEntry(expLeg,"NLO Expected (HT, CLs)","lf");
   if (gCLsExpExclHT_qg_LO) leg_qg->AddEntry(gCLsExpExclHT_qg_LO,"LO Expected (HT, CLs)","lf");
   //if (gPLobsexcl_qg) leg_qg->AddEntry(gPLobsexcl_qg,"Observed (PL, RooStat)","l");
   //if (gPLexpexcl_qg) leg_qg->AddEntry(gPLexpexcl_qg,"Expected (PL, RooStat)","l");
   leg_qg->Draw();
   c1->SaveAs("results/Exclusion_mSql_mGl_tb10_details.pdf");
   //c1->SaveAs("plot_tb10.pdf");

   c1->SetLogy(1);
   c1->SetLogx(1);
   TH2F*hUncertScan = new TH2F("hUncertScan",";relative signal uncertainty; number of signal events",
                     10,0.1,0.6,10,0.9,200);
   //hUncertScan->GetXaxis()->SetNdivisions(505, false);		     
   //hUncertScan->SetMinimum(1);	     
   TGraph * gCLsExpUncert = new TGraph(0);
   TGraph * gCLsObsUncert = new TGraph(0);
   TGraph * gPLExpUncert  = new TGraph(0);
   TGraph * gPLObsUncert  = new TGraph(0);
   TGraph * gFCExpUncert  = new TGraph(0);
   TGraph * gFCObsUncert  = new TGraph(0);
   TGraph * gMCMCExpUncert= new TGraph(0);
   TGraph * gMCMCObsUncert= new TGraph(0);
   plotTools->Graph(gCLsExpUncert, SignalRelUncertainty, ExpNSignLimit,1); 
   plotTools->Graph(gCLsObsUncert, SignalRelUncertainty, ObsNSignLimit,1); 
   plotTools->Graph(gPLExpUncert,  SignalRelUncertainty, PLExpNSignLimit,1); 
   plotTools->Graph(gPLObsUncert,  SignalRelUncertainty, PLObsNSignLimit, 1); 
   plotTools->Graph(gFCExpUncert,  SignalRelUncertainty, FCExpNSignLimit,1); 
   plotTools->Graph(gFCObsUncert,  SignalRelUncertainty, FCObsNSignLimit, 1); 
   plotTools->Graph(gMCMCExpUncert,SignalRelUncertainty, MCMCExpNSignLimit,1); 
   plotTools->Graph(gMCMCObsUncert,SignalRelUncertainty, MCMCObsNSignLimit, 1); 
   gCLsExpUncert->SetLineStyle(8);
   gPLObsUncert->SetLineColor(2);
   gPLExpUncert->SetLineColor(2); gPLExpUncert->SetLineStyle(8);
   gFCObsUncert->SetLineColor(3);
   gFCExpUncert->SetLineColor(3); gFCExpUncert->SetLineStyle(8);
   gMCMCObsUncert->SetLineColor(4);
   gMCMCExpUncert->SetLineColor(4); gMCMCExpUncert->SetLineStyle(8);
   hUncertScan->Draw("h");
   gCLsExpUncert->Draw("l");
   gCLsObsUncert->Draw("l");
   //gPLObsUncert->Draw("l");
   gPLExpUncert->Draw("l");
   //gFCObsUncert->Draw("l");
   //gFCExpUncert->Draw("l");
   //gMCMCObsUncert->Draw("l");
   //gMCMCExpUncert->Draw("l");
   TLatex l; l.SetTextSize(0.03); l.SetTextColor(1);
   l.DrawLatex(0.025,25,"excluded"); 
   TMarker m; m.SetMarkerStyle(29); m.SetMarkerColor(2);
   m.DrawMarker(0.128, 28.5);l.DrawLatex(0.14,28.5,"LM1");
   m.DrawMarker(0.147, 77.4);l.DrawLatex(0.16,77.4,"LM0");
 
   TLegend * leg_S = new TLegend(0.5,0.14,0.85,0.38);
   leg_S->SetBorderSize(0);leg_S->SetFillColor(0);
   leg_S->SetHeader("95% CL upper limits");
   leg_S->AddEntry(gCLsExpUncert,"Expected (CLs, TLimit)","l");
   leg_S->AddEntry(gCLsObsUncert,"Observed (CLs, TLimit)","l");
   //leg_S->AddEntry(gPLExpUncert,"Expected (PL, RooStat)","l");
   //leg_S->AddEntry(gPLObsUncert,"Observed (PL, RooStat)","l");
   //leg_S->AddEntry(gFCObsUncert,"Observed (FC, RooStat)","l");
   //leg_S->AddEntry(gFCExpUncert,"Expected (FC, RooStat)","l");
   //leg_S->AddEntry(gMCMCObsUncert,"Observed (MCMC, RooStat)","l");
   //leg_S->AddEntry(gMCMCExpUncert,"Expected (MCMC, RooStat)","l");
   leg_S->Draw();
   c1->SaveAs("results/UncertaintyScan.pdf");

   //theApp.Run();
}


int main(int argc, char** argv)
{
  return plot(argc, argv);
}
