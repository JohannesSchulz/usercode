#include "PlotTools.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

#include "TGraph.h"
#include "TF1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TROOT.h"
#include "TLatex.h"

template<class T>
TGraph * PlotTools<T>::Line( double(*x)(const T*), double(*y)(const T*), 
                             double(*func)(const T*), const double mass, const double diff )
{
  TGraph * result = new TGraph(1);
  std::sort(_scan->begin(),_scan->end(),sort_by(x));
  int i=0;
  for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
     if ( fabs(func( *it)-mass)<diff )
       result->SetPoint(i++, x(*it), y(*it));
  } 
  result->SetLineWidth(1);
  result->SetLineColor(kGray);
  return result;
}


template<class T>
void PlotTools<T>::Area( TH2*h, double(*x)(const T*), double(*y)(const T*), 
                         double(*f)(const T*) )
{
  std::cout<< _scan->size() <<",  &scan="<<_scan<<std::endl;
  for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
//	  std::cout<<"x value: "<<x(*it)<< "set bin x"<<h->GetXaxis()->FindBin(x(*it))<<std::endl;
//	  std::cout<<"y value: "<<y(*it)<<"set bin y"<<h->GetYaxis()->FindBin(y(*it))<<std::endl;
//	  std::cout<< "value "<<f(*it)<<std::endl;
//	  if(f(*it)<0.0001){
//		  	  std::cout<<"x value: "<<x(*it)<< "set bin x"<<h->GetXaxis()->FindBin(x(*it))<<std::endl;
//		  	  std::cout<<"y value: "<<y(*it)<<"set bin y"<<h->GetYaxis()->FindBin(y(*it))<<std::endl;
//	  }
	  double oldbincontent=h->GetBinContent( h->GetXaxis()->FindBin(x(*it)),
              h->GetYaxis()->FindBin(y(*it)));
	  if(oldbincontent!=0){
//		  std::cout<< "ERROR===================================old bin content is "<<oldbincontent<<std::endl;
//		  std::cout<<"x value: "<<x(*it)<< "set bin x"<<h->GetXaxis()->FindBin(x(*it))<<std::endl;
//		  	  std::cout<<"y value: "<<y(*it)<<"set bin y"<<h->GetYaxis()->FindBin(y(*it))<<std::endl;
	  }
    h->SetBinContent( h->GetXaxis()->FindBin(x(*it)), 
                      h->GetYaxis()->FindBin(y(*it)), f(*it) );

  } 

}

template<class T>
void PlotTools<T>::Graph( TGraph*g, double(*x)(const T*), double(*y)(const T*),double ymin)
{
  unsigned i = g->GetN();
  std::sort(_scan->begin(),_scan->end(),sort_by(x));
  for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
    if (y(*it)>=ymin) g->SetPoint(i++, x(*it), y(*it));
    //std::cout << i << ": x="<<x(*it)<< ", y="<<y(*it)<< std::endl;
  } 
}

template<class T>
std::vector<TGraph*> PlotTools<T>::GetContours005(TH2*h, int ncont)
{
   double conts[1];
   conts[0] = 0.05;
   if (!h) return std::vector<TGraph*>();
   TH2 * plot = (TH2*)h->Clone();
   plot->SetContour(1, conts);
   plot->SetFillColor(1);
   plot->Draw("CONT Z List");
   gPad->Update();
   TObjArray *contours = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   int ncontours      = contours->GetSize();
   std::vector<TGraph*> result;
   for (int i=0;i<ncontours;++i){
     TList *list = (TList*)contours->At(i);
     TGraph* curv = (TGraph*)list->First();
     if (curv) result.push_back( curv );
     for(int j = 0; j < list->GetSize(); j++){
         curv = (TGraph*)list->After(curv); // Get Next graph
         if (curv) result.push_back( curv );
     }
   }  
   delete plot;
   std::sort(result.begin(),result.end(),sort_TGraph());
   return result;
}

template<class T>
TGraph * PlotTools<T>::GetContour005(TH2*h, int ncont, int flag)
{
   return (TGraph*)GetContours005(h, ncont).at(flag)->Clone();
}

template<class T>
std::vector<TGraph*> PlotTools<T>::GetContours(TH2*h, int ncont)
{
   if (!h) return std::vector<TGraph*>();
   TH2 * plot = (TH2*)h->Clone();
   plot->SetContour(ncont);
   plot->SetFillColor(1);
   plot->Draw("CONT Z List");
   gPad->Update();
   TObjArray *contours = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
   if (!contours) {
     std::cerr<<"ERROR: Found no contours! Is the histogram empty?"<<std::endl;
     return std::vector<TGraph*>();
   }
   int ncontours      = contours->GetSize();
   std::vector<TGraph*> result;
   for (int i=0;i<ncontours;++i){
     TList *list = (TList*)contours->At(i);
     TGraph* curv = (TGraph*)list->First();
     if (curv) result.push_back( curv );
     for(int j = 0; j < list->GetSize(); j++){
         curv = (TGraph*)list->After(curv); // Get Next graph
         if (curv) result.push_back( curv );
     }
   }  
   delete plot;
   std::sort(result.begin(),result.end(),sort_TGraph());
   return result;
}

template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h, int ncont, int flag)
{
   std::vector<TGraph*> gs = GetContours(h, ncont);
   //assert(gs.size()>flag && "ERROR: Requested a non-existing contour index! Check with ExclusionTestContours first!");
   return (gs.size()>flag ? (TGraph*)gs[flag]->Clone() : new TGraph(0));
}

template<class T>
TGraph * PlotTools<T>::GetContour(TH2*h,double(*x)(const T*), double(*y)(const T*), 
                      double(*func)(const T*), int ncont, int flag,
		      int color, int style){
  TH2*hist = (TH2*)h->Clone();
  Area(hist, x, y, func);
  TGraph * graph = GetContour(hist, ncont, flag);
  graph->SetLineColor(color);
  graph->SetLineStyle(style);
  return graph;
}
 
template<class T>
TGraph * PlotTools<T>::GetContour005(TH2*h,double(*x)(const T*), double(*y)(const T*), 
                      double(*func)(const T*), int ncont, int flag,
		      int color, int style){
  TH2*hist = (TH2*)h->Clone();
  Area(hist, x, y, func);
  TGraph * graph = GetContour005(hist, ncont, flag);
  graph->SetLineColor(color);
  graph->SetLineStyle(style);
  return graph;
}
 
template<class T>
void PlotTools<T>::Print(double(*f)(const T*), double(*x)(const T*), double(*y)(const T*), TGraph*g, double p)
{
   for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
     for (int j=0; j<g->GetN(); ++j) {
       double gx, gy;
       g->GetPoint(j, gx, gy);
       if ( (x(*it)-gx)*(x(*it)-gx) +  (y(*it)-gy)*(y(*it)-gy) < p*p)
         std::cout << x(*it) << ", " << y(*it) 
		   << " :: " << f(*it) << std::endl;
     }
   } 

}
template<class T>
void PlotTools<T>::Print(double(*f)(const T*), 
                         double(*x1)(const T*), double(*x2)(const T*), 
                         double(*x3)(const T*), double(*x4)(const T*), 
                         TGraph*g, double p)
{
   for (typename std::vector<T*>::const_iterator it=_scan->begin();it!=_scan->end();++it){
     for (int j=0; j<g->GetN(); ++j) {
       double gx, gy;
       g->GetPoint(j, gx, gy);
       if ( (x1(*it)-gx)*(x1(*it)-gx) +  (x2(*it)-gy)*(x2(*it)-gy) < p*p)
         std::cout << x1(*it) << ", " << x2(*it) << ", "  
	           << x3(*it) << ", " << x4(*it)
		   << " :: " << f(*it) << std::endl;
     }
   } 

}

template<class T>
TGraph * PlotTools<T>::ChooseBest(TGraph*r1,TGraph*r2,TGraph*g1,TGraph*g2,double x,double y)
{
  TGraph * res = new TGraph(0);
  for (int i=0; i<r1->GetN(); ++i) {
    double rx, r1y;
    r1->GetPoint(i,rx,r1y);
    double r2y = r2->Eval(rx);
    double g1y=g1->Eval(rx);
    double resy = (r1y>=r2y?g1y:g2->Eval(rx));
    if (rx>x&&r1y>y)
      res->SetPoint(i,rx,resy);
    else
      res->SetPoint(i,rx,g1y);
  }
  res->SetLineColor(g1->GetLineColor());
  res->SetLineWidth(g1->GetLineWidth());
  res->SetLineStyle(g1->GetLineStyle());
  res->SetFillColor(g1->GetFillColor());
  res->SetFillStyle(g1->GetFillStyle());
  return res;
}

template<class T>
TH2 * PlotTools<T>::BinWiseOr(TH2*h1, TH2*h2)
{
  TH2 * res = (TH2*)h1->Clone();
  for (int x=0; x<=res->GetXaxis()->GetNbins(); ++x)
    for (int y=0; y<=res->GetYaxis()->GetNbins(); ++y)
      if (h2->GetBinContent(x,y)>0.5)
        res->SetBinContent(x,y,h2->GetBinContent(x,y));
  return res;	
}

template<class T>
TGraph * PlotTools<T>::ModifyExpSigma(TGraph*g1, TGraph*g2, TGraph*g3)
{
  TGraph * res = new TGraph(0);
  for (int i=0; i<g1->GetN(); ++i) {
    double x, y;
    g1->GetPoint(i,x,y);
    double y2 = g2->Eval(x);
    double y3 = g3->Eval(x);
    res->SetPoint(i,x,y-y2+y3);
  }
  res->SetLineColor(g1->GetLineColor());
  res->SetLineStyle(g1->GetLineStyle());
  res->SetLineWidth(g1->GetLineWidth());
  res->SetFillColor(g1->GetFillColor());
  res->SetFillStyle(g1->GetFillStyle());
  return res;
}

double GetX(TGraph* g, double x0, double y0){
  double dn=999999999, up=9999999999, minx, maxx;
  for (int i=0; i<g->GetN(); ++i) {
    double x, y;
    g->GetPoint(i,x,y);
    if (y==y0) return x;
    if ( y<y0 && (x-x0)*(x-x0)+(y-y0)*(y-y0)<dn ){
      dn=(x-x0)*(x-x0)+(y-y0)*(y-y0);
      minx=x;
    }
    if ( y>y0 && (x-x0)*(x-x0)+(y-y0)*(y-y0)<up ){
      up=(x-x0)*(x-x0)+(y-y0)*(y-y0);
      maxx=x;
    }
  }
  return (minx+maxx)/2.;
}

template<class T>
TGraph * PlotTools<T>::ModifyExpSigmaY(TGraph*g1, TGraph*g2, TGraph*g3)
{
  TGraph * res = new TGraph(0);
  for (int i=0; i<g1->GetN(); ++i) {
    double x, y;
    g1->GetPoint(i,x,y);
    double x2,y2,x3,y3;
    x2=GetX(g2,x,y);
    x3=GetX(g3,x,y);
    res->SetPoint(i,x-x2+x3,y);
  }
  res->SetLineColor(g1->GetLineColor());
  res->SetLineStyle(g1->GetLineStyle());
  res->SetLineWidth(g1->GetLineWidth());
  res->SetFillColor(g1->GetFillColor());
  res->SetFillStyle(g1->GetFillStyle());
  return res;
}


template<class T>
bool PlotTools<T>::sort_TGraph::operator()(const TGraph*g1, const TGraph*g2)
{ 
   return g1->GetN() > g2->GetN();
}
TGraph * GetExcludedRegion(TGraph * lowerLimit, double min1,double min2){
	TGraph* excludedRegion = new TGraph(lowerLimit->GetN()+2);
	  int nbins = lowerLimit->GetN();

	  for(int i=0; i<nbins+1; i++){
	    double x,y;
	    lowerLimit->GetPoint(i,x,y);
	    excludedRegion->SetPoint(i,x,y);
	  }
	  excludedRegion->SetPoint(nbins,min1,min2);
	  excludedRegion->SetPoint(nbins+1,2000,min2);
	  //excludedRegion->SetPoint(nbins+2,min1,2000);


	  excludedRegion->SetFillStyle(3004);
	  return excludedRegion;

}
TGraph * MakeBand(TGraph *g1, TGraph *g2, bool b){
  //std::cout<<"MAKE BAND!"<<b<<std::endl;
  TGraph * res = new TGraph(g1->GetN()+g2->GetN()+1);
  int p=0;
  double firstx,firsty;
  for (int i=0; i<g1->GetN(); ++i) {
    double x, y;
    g1->GetPoint(i, x, y);
    res->SetPoint(p++, x, y);
    if(i==0){
    	firstx=x;
    	firsty=y;
    }
  }
  for (int i=g2->GetN()-1; i>=0; --i) {
    double x, y;
    g2->GetPoint(i, x, y);
    res->SetPoint(p++, x, y);
  }
  res->SetPoint(p++, firstx, firsty);
  if (res->GetN()==0) return res;
  res->SetLineColor( g2->GetLineColor() );
  res->SetFillColor( g2->GetLineColor() );
  res->SetFillStyle(4050);
  return res;
}
std::string getStringFromInt(int i) {
	char result[100];
	sprintf(result, "%d", i);
	std::string ret = result;
	return " " + ret;
	//---------
}

void drawCmsPrel(double METCut, bool onlyChannelInfo) {
	double intLumi=1.14;
	TLatex as;
	as.SetNDC(true);
	as.SetTextSize(0.035);
	as.SetTextFont(42);//ms.SetTextColor(12);
	std::string out="";
	if(!onlyChannelInfo){
	out="#int #font[12]{L}dt = %.2ffb^{  -1}";
	as.DrawLatex(
				0.32,
				0.93,
				Form(out.c_str(),
						intLumi));
	}

		//1#gamma, >=3 jets, MET>"+getStringFromInt(METCut)+" GeV ";
		if(METCut==0){
			out="1#gamma, >=3 jets";
		}

	else{
		out="1#gamma, >=3 jets, MET>"+getStringFromInt(METCut)+" GeV ";

	}

	as.DrawLatex(
			0.57,
			0.93,
			Form(out.c_str()));
	if(!onlyChannelInfo){
	as.SetTextSize(0.04);
	as.DrawLatex(0.02, 0.93, "CMS preliminary");
	}
}
void Smooth(TGraph * g, int N)
{
  TGraph * old = (TGraph*)g->Clone();
  //int N = (n%2==0?n+1:n);
  if (N>2*g->GetN()) N=2*g->GetN()-1;


  double gauss[N];
  double sigma = (double)N/4.;
  double sum=0;
  double lim=(double)N/2.;
  TF1 *fb = new TF1("fb","gaus(0)",-lim,lim);
  fb->SetParameter(0, 1./(sqrt(2*3.1415)*sigma) );
  fb->SetParameter(1, 0);
  fb->SetParameter(2, sigma);
  for (int i=0; i<N; ++i){
    gauss[i]=fb->Integral(-lim+i,-lim+i+1);
    sum+=gauss[i]; 
  }
  for (int i=0; i<N; ++i)
    gauss[i] /= sum;

  for (int i=0; i<g->GetN(); ++i){
    double avy=0., avx=0., x, x0, y;
    int points=0;
    for (int j=i-N/2; j<=i+N/2; ++j){
      if      (j<0)          old->GetPoint(0, x, y);
      else if (j>=g->GetN()) old->GetPoint(old->GetN()-1, x, y);
      else                   old->GetPoint(j, x, y);
      if (i==j) x0=x;
      avy += y * gauss[points];
      avx += x * gauss[points];
      ++points;
    }
    if (i-N/2<0 || i+N/2>=g->GetN()) g->SetPoint(i, x0, avy);
    else g->SetPoint(i, avx, avy); 
  }
  delete old;
}


void Smooth2D(TGraph * g, int N)
{
  TGraph * old = Close2D(g);
  if (N>2*g->GetN()) N=2*g->GetN()-1;

  double gauss[N];
  double sigma = (double)N/4.;
  double sum=0;
  double lim=(double)N/2.;
  TF1 *fb = new TF1("fb","gaus(0)",-lim,lim);
  fb->SetParameter(0, 1./(sqrt(2*3.1415)*sigma) );
  fb->SetParameter(1, 0);
  fb->SetParameter(2, sigma);
  for (int i=0; i<N; ++i){
    gauss[i]=fb->Integral(-lim+i,-lim+i+1);
    sum+=gauss[i]; 
  }
  for (int i=0; i<N; ++i)
    gauss[i] /= sum;

  for (int i=0; i<g->GetN(); ++i){
    double avy=0., avx=0, x, x0, y;
    int points=0;
    for (int j=i-N/2; j<=i+N/2; ++j){
      //if      (j<0)          old->GetPoint(old->GetN()+j, x, y);
      //else if (j>=g->GetN()) old->GetPoint(j-old->GetN(), x, y);
      if      (j<0)          old->GetPoint(0, x, y);
      else if (j>=g->GetN()) old->GetPoint(old->GetN()-1, x, y);
      else                   old->GetPoint(j, x, y);
      if (i==j) x0=x;
      avy += y * gauss[points];
      avx += x * gauss[points];
      ++points;
    }
    g->SetPoint(i, avx, avy); 
  }
  delete old;
}

TGraph * Close2D(TGraph * g)
{
  TGraph * f = new TGraph(0);
  if (g->GetN()==0) return f;
  double x, y;
  g->GetPoint(0,x,y);
  g->SetPoint(g->GetN(),x,y);
  
  int i=0;
  for (;i<g->GetN();++i){
    g->GetPoint(i,x,y);
    //if (x<450&&y<450) break;
  }  
  int p=0;
  for (int j=i+1;j!=i;++j){
    if (j>=g->GetN()) j=0;
    g->GetPoint(j,x,y);
    //if (y<110+(x-120)*390/442||(x<330&&y<1000)||(x<500&&y<600)) continue;
    f->SetPoint(p++,x,y);
  }  
  return f; 
}


template class PlotTools<SusyScan>;
template class PlotTools<GeneratorMasses>;
