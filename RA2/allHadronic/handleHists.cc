#include "handleHists.h"
#include "ConfigFile.h"
#include "TPostScript.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TFile.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cmath>

using namespace std;


  
TH1 * handleHists::GetHist(const string file, const string hist, const string treename)
{
   TH1F * result;
   
   stringstream ss;
   ss << hist<< "_"<<_plotindex++;
   string name = ss.str(); //root can't handle two hists with same names
   //For debugging: if not all background are available, create an empty hist:
   if (file=="") {
     result = new TH1F(name.c_str(), hist.c_str(), 100, 0.0, 500.0);
     //result->Fill(20);
     return result;
   }//can delete this, as soon as all samples (incl. data) are available

   //open root file
   TFile f( file.c_str() );
   if (f.IsZombie()) {
       cout << "Error opening file '" << file << "'"<< endl;
       exit(-1);
   }
   
   //open requested histogram 'hist', in directory 'treename'
   string dummy_name = hist;
   if (treename!="") dummy_name = treename+"/"+hist;
   result = (TH1F*)f.Get( dummy_name.c_str() );
   if (!result) {
     cerr << "Unable to read histogram '" <<dummy_name<< "' from file '"<<file<< "'!"<<endl;
     exit(1);
   }
   result->SetTitle( hist.c_str() );
   result->SetDirectory(0);
   
   return result;
}

handleHists::handleHists( ConfigFile* config)
{
  _plotindex=0;
  string qcdfile    = config->read<string>("qcd-rootfile","");
  string qcdtree    = config->read<string>("qcd-treename","");
         nqcd       = config->read<string>("qcd-leg-name","QCD");
  string ttbarfile  = config->read<string>("ttbar-rootfile","");
  string ttbartree  = config->read<string>("ttbar-treename","");
         nttbar     = config->read<string>("ttbar-leg-name","t#bar{t}");
  string znunufile  = config->read<string>("znunu-rootfile","");
  string znunutree  = config->read<string>("znunu-treename","");
         nznunu     = config->read<string>("znunu-leg-name","Z#to#nu#nu");
  string signalfile = config->read<string>("signal-rootfile","");
  string signaltree = config->read<string>("signal-treename","");
         nsignal    = config->read<string>("signal-leg-name","Signal");
  string datafile   = config->read<string>("data-rootfile","");
  string datatree   = config->read<string>("data-treename","");
         ndata      = config->read<string>("data-leg-name","Data");
  
  vector<string> vars = bag_of_string(config->read<string>("final variables","HT"));
  vector<string> corr_unc = bag_of_string(config->read<string>("correlated uncertainties","JEC"));
  vector<string> uncorr_unc = bag_of_string(config->read<string>("uncorrelated uncertainties","method"));

  //read in all histograms and store them in the private vectors of this class
  for (vector<string>::const_iterator var=vars.begin(); var!=vars.end(); ++var) {

    //read 'normal' histograms (without syst. uncertainties)
    _data_hists.  push_back( GetHist(datafile, (*var), datatree ) );
    _signal_hists.push_back( GetHist(signalfile, (*var), signaltree) );
    TH1 * qcd = GetHist(qcdfile, (*var), qcdtree);
    TH1 * top = GetHist(ttbarfile, (*var), ttbartree);
    TH1 * z    = GetHist(znunufile, (*var), znunutree);
    _qcd_hists.   push_back( qcd );
    _ttbar_hists. push_back( top );
    _znunu_hists. push_back( z   );
    
    //read background histogram correlated uncertainties:
    for (vector<string>::const_iterator cor=corr_unc.begin(); cor!=corr_unc.end(); ++cor) {
      _syst_up_corr[qcd].push_back( GetHist(qcdfile, (*var)+"_"+(*cor)+"_UP", "finalPlot_JEC_UP") );
      _syst_dn_corr[qcd].push_back( GetHist(qcdfile, (*var)+"_"+(*cor)+"_DN", "finalPlot_JEC_DN") );
      _syst_up_corr[top].push_back( GetHist(ttbarfile, (*var)+"_"+(*cor)+"_UP", "finalPlot_JEC_UP") );
      _syst_dn_corr[top].push_back( GetHist(ttbarfile, (*var)+"_"+(*cor)+"_DN", "finalPlot_JEC_DN") );
      _syst_up_corr[ z ].push_back( GetHist(znunufile, (*var)+"_"+(*cor)+"_UP", "finalPlot_JEC_UP") );
      _syst_dn_corr[ z ].push_back( GetHist(znunufile, (*var)+"_"+(*cor)+"_DN", "finalPlot_JEC_DN") );
    }
    //read background histogram un-correlated uncertainties:
    for (vector<string>::const_iterator uco=uncorr_unc.begin(); uco!=uncorr_unc.end(); ++uco) {
      _syst_up_uncorr[qcd].push_back( GetHist(qcdfile, (*var)+"_"+(*uco)+"_UP", "finalPlot_method_UP") );
      _syst_dn_uncorr[qcd].push_back( GetHist(qcdfile, (*var)+"_"+(*uco)+"_DN", "finalPlot_method_DN") );
      _syst_up_uncorr[top].push_back( GetHist(ttbarfile, (*var)+"_"+(*uco)+"_UP", "finalPlot_method_UP") );
      _syst_dn_uncorr[top].push_back( GetHist(ttbarfile, (*var)+"_"+(*uco)+"_DN", "finalPlot_method_DN") );
      _syst_up_uncorr[ z ].push_back( GetHist(znunufile, (*var)+"_"+(*uco)+"_UP", "finalPlot_method_UP") );
      _syst_dn_uncorr[ z ].push_back( GetHist(znunufile, (*var)+"_"+(*uco)+"_DN", "finalPlot_method_DN") );
    }
    
  }//loop over all 'final variables'

  if ((_data_hists.size()==_signal_hists.size())*(_data_hists.size()==_qcd_hists.size())*
      (_data_hists.size()==_ttbar_hists.size())*(_znunu_hists.size()==_qcd_hists.size())!=1){
     cerr<<"ERROR: Read in different number of data, signal, and background histograms!\n"
	 <<endl;
     exit(1);
  }        
}
    
TH1 * handleHists::GetSyst(const std::string dir, const TH1*h1, const TH1*h2, const TH1*h3)
{
  int bins = h1->GetNbinsX();
  stringstream ss; ss << _plotindex++; 
  string unique; unique = ss.str();
  string name = "syssum_"+unique+(string)h1->GetTitle()+dir;
  TH1 * sum     = new TH1F(name.c_str(),"",bins,h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  name = "relquad_"+unique+(string)h1->GetTitle()+dir;
  TH1 * relquadsum = new TH1F(name.c_str(),"",bins,h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  vector<const TH1*> samples;
  if (h1) samples.push_back(h1);
  if (h2) samples.push_back(h2);
  if (h3) samples.push_back(h3);

  //Calculate the total systematic uncertainty for 'samples':
  for (vector<const TH1*>::const_iterator s=samples.begin(); s!=samples.end(); ++s) {

    //simply add all correlated uncertainties for 's' (assume 100% correlation),
    //the uncertainties are *relative* to the unchanged hists 's'
    vector<TH1*>::const_iterator corr_begin =
       (dir=="UP" ? _syst_up_corr[(*s)].begin() : _syst_dn_corr[(*s)].begin());
    vector<TH1*>::const_iterator corr_end  =
       (dir=="UP" ? _syst_up_corr[(*s)].end() : _syst_dn_corr[(*s)].end());

    for (vector<TH1*>::const_iterator c=corr_begin; c!=corr_end; ++c) {
      sum->Add( *c );
      sum->Add( *s, -1.0 ); //sum up (c-s)
    }
    
    //now add quadratically all uncorrelated uncertainties for 's';
    //this is done bin-by-bin, and for *relative* uncertainties
    vector<TH1*>::const_iterator uncorr_begin =
       (dir=="UP" ? _syst_up_uncorr[*s].begin() : _syst_dn_uncorr[*s].begin()); 
    vector<TH1*>::const_iterator uncorr_end   =
       (dir=="UP" ? _syst_up_uncorr[*s].end() : _syst_dn_uncorr[*s].end());
    for (vector<TH1*>::const_iterator u=uncorr_begin; u!=uncorr_end; ++u) {
      for (int bin=0; bin<=bins; ++bin) {
        double cont = (*u)->GetBinContent(bin) - (*s)->GetBinContent(bin);
        relquadsum->SetBinContent(bin, relquadsum->GetBinContent(bin)+cont*cont);
      }	
    }
  }
  
  //add total correlated and total uncorrelated uncertainties quadratically:
  for (int bin=0; bin<=bins; ++bin) {
    double cont = sum->GetBinContent(bin);
    relquadsum->SetBinContent(bin, relquadsum->GetBinContent(bin)+cont*cont);
    //sqrt(relquadsum):
    cont=0;
    for (vector<const TH1*>::const_iterator s=samples.begin(); s!=samples.end(); ++s) 
      cont+=(*s)->GetBinContent(bin);
    cont += (dir=="UP" ? sqrt(relquadsum->GetBinContent(bin)) : -sqrt(relquadsum->GetBinContent(bin)));  
    sum->SetBinContent(bin, cont );
  }	
  sum->SetLineStyle( 9 ); //dashed
  
  delete relquadsum;
  return sum; //abolute syst. uncertainty, i.e. *not* relative
}

void handleHists::PlotHistograms(const string out)
{
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetTitleFillColor(0); 
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetHistFillColor(0);
  gStyle->SetStatBorderSize(0);
  TPostScript ps(out.c_str(), 111);
  TCanvas c1("c1","",600,600);

  std::vector<TH1*>::const_iterator d=_data_hists.begin(),
                                    s=_signal_hists.begin(),
                                    q=_qcd_hists.begin(),
                                    t=_ttbar_hists.begin(),
                                    z=_znunu_hists.begin();

  for (;d!=_data_hists.end();++d,++s,++q,++t,++z) {
    string var  = (*d)->GetTitle();
    int    bins = (*d)->GetNbinsX();
    double min  = (*d)->GetXaxis()->GetXmin();
    double max  = (*d)->GetXaxis()->GetXmax();
    cout << "...plotting "<< var << " with bins="<<bins<<", min="<<min<<", max="<<max<<endl;  

    (*q)->SetFillColor( kYellow );
    (*t)->SetFillColor( kBlue );
    (*z)->SetFillColor( kGreen );
    (*s)->SetLineColor( kRed );
    (*d)->SetMarkerStyle( 8 );
    (*s)->SetLineWidth( 4 );
    
    string name = "hs_"+var;
    string titel = ";"+var+" [GeV]; events";
    THStack st(name.c_str(),"");
    st.Add( *z );
    st.Add( *t );
    st.Add( *q );
    
    TH1 * syst_up = GetSyst( "UP", *q, *t, *z);
    TH1 * syst_dn = GetSyst( "DN", *q, *t, *z);
 
    double maximum = (*d)->GetMaximum()+sqrt((*d)->GetMaximum());
    if ((*s)->GetMaximum()>maximum) maximum = (*s)->GetMaximum();
    if (st.GetMaximum()>maximum) maximum = st.GetMaximum();
    if (maximum>syst_up->GetMaximum()) syst_up->SetMaximum(maximum);
    syst_up->SetMinimum(0);
    syst_up->SetTitle(titel.c_str());
    syst_up->GetYaxis()->SetTitleOffset(1.4);
  
    TLegend leg(0.5,0.7,0.9,0.9);
    leg.SetFillColor(0);leg.SetBorderSize(1);
    if ( (*d)->Integral()>0. ) leg.AddEntry( (*d), ndata.c_str(),"pe");
    if ( (*s)->Integral()>0. ) leg.AddEntry( (*s), nsignal.c_str(),"l");
    if ( (*q)->Integral()>0. ) leg.AddEntry( (*q), nqcd.c_str(),"f");
    if ( (*t)->Integral()>0. ) leg.AddEntry( (*t), nttbar.c_str(),"f");
    if ( (*z)->Integral()>0. ) leg.AddEntry( (*z), nznunu.c_str(),"f");
    if ( (*q)->Integral()+(*t)->Integral()+(*z)->Integral()>0. ) leg.AddEntry( syst_up, "total sys. uncert.","l");
  
    syst_up->Draw("h");      //syst.uncertainty band upper border
    st.Draw("same");         //stacked backgrounds
    syst_dn->Draw("h,same"); //syst.uncertainty band lower border
    (*s)->Draw("h,same");    //signal
    (*d)->Draw("pe,same");   //data with stat. errors
    leg.Draw();              //legend    
    c1.Update();
    ps.NewPage();

    //delete syst_up, syst_dn; delete leg;
  }
  cout << "Wrote result plots to file '"<<out<<"'"<<endl;
     
}
  
void handleHists::Print()
{
  std::vector<TH1*>::const_iterator d=_data_hists.begin(),
                                    s=_signal_hists.begin(),
                                    q=_qcd_hists.begin(),
                                    t=_ttbar_hists.begin(),
                                    z=_znunu_hists.begin();

  for (;d!=_data_hists.end();++d,++s,++q,++t,++z) {
    string name = "hps_"+(string)(*d)->GetTitle();
    THStack st(name.c_str(),"");
    st.Add( *z );
    st.Add( *t );
    st.Add( *q );
    
    TH1 * syst_up = GetSyst( "UP", *q, *t, *z);
    TH1 * syst_dn = GetSyst( "DN", *q, *t, *z);
    double b = (*q)->Integral()+(*t)->Integral()+(*z)->Integral();
 
    cout << setw(10) << (*d)->GetTitle()
         << "  d: "     << (*d)->Integral()
	 << ", s: "     << (*s)->Integral()
	 << ", b: "     << b
	 << " "         << showpos << syst_up->Integral()-b
	 << " "         << showpos << syst_dn->Integral()-b << noshowpos
         << "(syst.) +-"<< sqrt((*d)->Integral())
	 << "(stat)"
	 << endl;
  }   
 
}
  
handleHists::~handleHists()
{
  //delete all histograms
  /*
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_up_corr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)      delete *i;
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_dn_corr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)     delete *i;
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_up_uncorr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)      delete *i;
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_dn_uncorr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)      delete *i;
  for (vector<TH1*>::iterator it=_qcd_hists.begin(); it!=_qcd_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_ttbar_hists.begin(); it!=_ttbar_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_znunu_hists.begin(); it!=_znunu_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_signal_hists.begin(); it!=_signal_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_data_hists.begin(); it!=_data_hists.end();++it)    delete *it;
  */
  _syst_up_corr.clear(); 
  _syst_dn_corr.clear(); 
  _syst_up_uncorr.clear();
  _syst_dn_uncorr.clear();
  _qcd_hists.clear(); 
  _ttbar_hists.clear(); 
  _znunu_hists.clear(); 
  _signal_hists.clear();
  _data_hists.clear(); 
}