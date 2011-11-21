//system
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <exception>

#include "table.h"
#include "ConfigFile.h"

//LUMI
const double luminosity = 4320.0;
const double luminosityUncertainty = 1.045; //4.5% as recommended for LP
const double scaleUncertainty = 0.2;        //suggested by Yuri, see mail June 30th. !Note:obsolete, is not used for limit calculation
const string WinoScanString = "Wino"; //sub-string in signalAcceptance input file, specifying if the number of generated events has to be set manually (because of generator cuts).

std::string ToString(double d, std::string s=""){
  if (d==0&&s!="") return s;
  std::stringstream ss;
  ss << d;
  return ss.str();
}


struct point {
	//general stuff, independent from selection
	double gluino;
	double squark;
	double chi;
	double xsec;
	double xsecNLO;
	double lumi;
	int    totalGenerated;

        struct bin {
        //event yields
	double signal;
	double bgd_qcd;
	double bgd_ewk;
	double bgd_fsr;
	double qcd_contamination;
	double ewk_contamination;
	double data;
        //uncertainties
	double u_NLO;      //signal scale
	double u_pdfxsec;  //signal PDF xsec
	double u_pdfacc;   //signal PDF acceptance
	double u_sig_stat; //signal stat
	double u_lumi;
	double u_qcd;
	double u_qcd_stat;
	double u_ewk;
	double u_ewk_stat;
	double u_fsr;
	double u_fsr_stat;
	};
	std::vector<bin> bins;
};

class points {
public:
	point* Get(double gl, double sq, double chi) {
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it)
			if (it->gluino == gl && it->squark == sq && it->chi == chi)
				return &(*it);
		return 0;
	}
	void Add(point p) {
		p_.push_back(p);
	}
	void Reset() {
		p_.clear();
	}
	void Write(const std::string dir) {
	        using namespace Table;
	        using namespace std;
		for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
			ofstream ofile;
			stringstream ss;
			ss << dir << "_" << it->squark << "_" << it->gluino << "_"
					<< it->chi << ".txt";
			ofile.open(ss.str().c_str());
			ofile << "# gluino = " << it->gluino << "\n";
			ofile << "# squark = " << it-> squark << "\n";
			ofile << "# chi1 = " << it-> chi << "\n";
			ofile << "# Xsection.LO = " << it-> xsec << "\n";
			ofile << "# Xsection.NLO = " << it-> xsecNLO << "\n";
			ofile << "# Luminosity = " << it-> lumi << "\n";
                        int n_channels    = it->bins.size();
			int n_backgrounds = 3;
			int n_nuisance    = 8;
			double d=0,b=0,s=0,cont=0;
			for (int bin=1; bin<=n_channels; ++bin){
			  d+=it->bins[bin-1].data;
			  b+=it->bins[bin-1].bgd_qcd + it->bins[bin-1].bgd_ewk + it->bins[bin-1].bgd_fsr;
			  s+=it->bins[bin-1].signal;
			  cont+=it->bins[bin-1].qcd_contamination + it->bins[bin-1].ewk_contamination;
			}
			ofile << "# data = " << d << "\n";
			ofile << "# background = " << b << "\n";
			ofile << "# signal = " << s << "\n";
			ofile << "# signal.contamination = " << cont << "\n";
			ofile << "# acceptance = " << s/(it->lumi * it->xsecNLO ) << "\n";

			ofile << "imax " << setw(2) << n_channels    << "  number of channels" << endl;
			ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
			ofile << "kmax " << setw(2) << n_nuisance    << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
			ofile << "------------" << endl;  

                        //observed events in all channels
			TTable observed("## observed events");\
			observed.SetStyle(Empty);
			observed.SetDelimiter("  ");
			observed.AddColumn<string>(""); for (int bin=1; bin<=n_channels; ++bin) observed.AddColumn<int>("");
			observed << "bin";         for (int bin=1; bin<=n_channels; ++bin) observed << bin;
			observed << "observation"; 
			for (int bin=1; bin<=n_channels; ++bin) {
			  stringstream ss;
			  ss << "n" << bin;
			  observed << (int)it->bins[bin-1].data;
			}  
			ofile << observed << "------------\n" << endl;  

			//expected events in all channels for signal and all backgrounds
			TTable exp("## expected events");
			exp.SetStyle(Empty);
			exp.SetDelimiter("  ");
			exp.AddColumn<string>(""); 
			exp.SetMinumumWidth(20,0);//make first column at least 20 chars
			  for (int bin=1; bin<=n_channels; ++bin) 
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) 
        		      exp.AddColumn<string>("");
			exp << "bin"; 
			  for (int bin=1; bin<=n_channels; ++bin)
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) {
        		       stringstream ss;
        		       ss << bin;
        		       exp << ss.str();
			    }
			exp << "process";
			for (int bin=1; bin<=n_channels; ++bin) {
			  exp << "signal" << "qcd" << "ewk" << "fsr";
			}  
			exp << "process"; 
			for (int bin=1; bin<=n_channels; ++bin) 
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) {
        		       stringstream ss;
        		       ss << (sample-1);
        		       exp << ss.str();
			    }	
			exp << "rate"; 
			for (int bin=1; bin<=n_channels; ++bin) {
			  exp << ToString(it->bins[bin-1].signal  
			                  - it->bins[bin-1].qcd_contamination 
					  - it->bins[bin-1].ewk_contamination )
			      << ToString(it->bins[bin-1].bgd_qcd)
			      << ToString(it->bins[bin-1].bgd_ewk)
			      << ToString(it->bins[bin-1].bgd_fsr);
			      
			}  
			ofile << exp << "------------" << std::endl;  
			
			TTable sys("");
			sys.SetStyle(Empty);
			sys.SetDelimiter("  ");
			sys.AddColumn<string>(""); 
			sys.SetMinumumWidth(20,0);//make first column at least 20 chars
			  for (int bin=1; bin<=n_channels; ++bin) 
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) 
        		      sys.AddColumn<string>("");
			sys << "U_Sig lnN";
 		        for (int b=0; b<n_channels; ++b) {
			  //Signal total systmatic uncertainty not including statistical and lumi:
			  double u_sig = sqrt( pow(it->bins[b].u_NLO-1.,2) +
      			                       pow(it->bins[b].u_pdfxsec-1.,2) +
      			                       pow(it->bins[b].u_pdfacc-1.,2));
        		  sys << ToString(u_sig,"-") // signal
			      << "-" << "-" << "-"; //qcd, ewk, fsr
			}      
			sys << "U_Lumi lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << ToString(it->bins[b].u_lumi,"-") // signal
			      << "-" // qcd data-driven
			      << "-" // ewk data-driven
			      << ToString(it->bins[b].u_lumi,"-");//fsr
			sys << "U_qcd lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << "-" // signal
			      << ToString(it->bins[b].u_qcd,"-") << "-" << "-"; //qcd, ewk, fsr
			sys << "U_ewk lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << "-" // signal
			      << "-" << ToString(it->bins[b].u_ewk,"-") << "-"; //qcd, ewk, fsr
			sys << "U_fsr lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << "-" // signal
			      << "-" << "-" << ToString(it->bins[b].u_fsr,"-"); //qcd, ewk, fsr
			      
			//Now the statistical uncertainties:...................................
			//signal-statistical
			for (int bin=0; bin<n_channels; ++bin) {    
 		          for (int b=0; b<n_channels; ++b) {
			    if (bin==b) sys << ToString(it->bins[b].u_sig_stat,"-"); 
			    else        sys << "-";
			    sys << "-" << "-" << "-"; // qcd, ewk, fsr
			  }
			}      
			//qcd-statistical
			for (int bin=0; bin<n_channels; ++bin) {    
 		          for (int b=0; b<n_channels; ++b) {
   		            sys << "-";//signal
			    if (bin==b) sys << ToString(it->bins[b].u_qcd_stat,"-");
			    else        sys << "-";
			    sys << "-" << "-"; //ewk, fsr
			  }
			}      
			//ewk-statistical
			for (int bin=0; bin<n_channels; ++bin) {    
 		          for (int b=0; b<n_channels; ++b){
			    sys <<"-"<< "-";//signal, qcd 
   		            if (bin==b) sys << ToString(it->bins[b].u_ewk_stat,"-");
			    else        sys << "-";
			    sys << "-"; //fsr
			  }  		
			}      
			//fsr-statistical
			for (int bin=0; bin<n_channels; ++bin) {    
 		          for (int b=0; b<n_channels; ++b){
			    sys <<"-"<< "-"<<"-";//signal, qcd, ewk 
   		            if (bin==b) sys << ToString(it->bins[b].u_fsr_stat,"-");
			    else        sys << "-";
			  }
			}      
			ofile << sys << "------------" << std::endl;  
			//cout << "WRITE GL:" << it->gluino << ", SQ:" << it->squark << endl;
			ofile.close();
		}
	}
	std::vector<point> * Bla(){return &p_;}
private:
	std::vector<point> p_;
} Points;

void ReadSignalAcceptance(std::string sig_file, std::string dat_file="", std::string fsr_file="") {
  ConfigFile * cfg = new ConfigFile(sig_file);
  ConfigFile * dat_cfg = (dat_file==""?cfg:new ConfigFile(dat_file));
  ConfigFile * fsr_cfg = (fsr_file==""?dat_cfg:new ConfigFile(fsr_file));
  int n=1;
  do {
    point p;
    std::stringstream ss;
    ss << "signalpoint" << n++;
    p.gluino = cfg->read<double>(ss.str()+"_gluino",-1.);
    p.squark = cfg->read<double>(ss.str()+"_squark",-1.);
    p.chi    = cfg->read<double>(ss.str()+"_NLSP",-1.);
    if (p.gluino== -1.||p.squark== -1.||p.chi== -1.) break;

    p.totalGenerated = cfg->read<int>(ss.str()+"_totalGenEvents",-1);
    //override the totalGenEvents number in case of the neutralino scan (because of generator cuts):
    if (sig_file.find( WinoScanString )!=string::npos) {
      p.totalGenerated = 60000;
    }
    
    p.lumi = luminosity;
    std::vector<double> data  = bag_of<double>(dat_cfg->read<std::string>("selEvents"));
    std::vector<double> qcd   = bag_of<double>(dat_cfg->read<std::string>("bkgQCDBinned"));
    std::vector<double> u_qcd = bag_of<double>(dat_cfg->read<std::string>("bkgQCDBinnedSYSTUP"));
    std::vector<double> u_qcd_stat = bag_of<double>(dat_cfg->read<std::string>("bkgQCDBinnedSTATERR"));
    std::vector<double> ewk   = bag_of<double>(dat_cfg->read<std::string>("bkgEWKBinned"));
    std::vector<double> u_ewk = bag_of<double>(dat_cfg->read<std::string>("bkgEWKBinnedSYSTUP"));
    std::vector<double> u_ewk_stat = bag_of<double>(dat_cfg->read<std::string>("bkgEWKBinnedSTATERR"));
    std::vector<double> fsr   = bag_of<double>(fsr_cfg->read<std::string>("bkgFSRBinned"));
    std::vector<double> u_fsr = bag_of<double>(fsr_cfg->read<std::string>("bkgFSRBinnedSYSTUP"));
    std::vector<double> u_fsr_stat = bag_of<double>(fsr_cfg->read<std::string>("bkgFSRBinnedSTATERR"));
    std::vector<double> sig   = bag_of<double>(cfg->read<std::string>(ss.str()+"_selEvents"));
    std::vector<double> u_sig = bag_of<double>(cfg->read<std::string>(ss.str()+"_selEventsSYSTUP"));
    std::vector<double> sig_qcd = bag_of<double>(cfg->read<std::string>(ss.str()+"_bkgQCDBinned"));
    std::vector<double> sig_ewk = bag_of<double>(cfg->read<std::string>(ss.str()+"_bkgEWKBinned"));
    int n_channels = data.size();
    if ((int)qcd.size()!=n_channels ||(int)ewk.size()!=n_channels ||(int)fsr.size()!=n_channels ||(int)sig.size()!=n_channels) {
      std::cerr << "ERROR: inconstitent number of channels at signalpoint "<<n-1
                << "; data=" << data.size()
                << "; qcd=" << qcd.size()
                << "; ewk=" << ewk.size()
                << "; fsr=" << fsr.size()
                << "; sig=" << sig.size()
		<<std::endl;
      continue;
    }
    for (int c=0;c<n_channels;++c){
      point::bin channel;
      channel.signal             = (sig.size()?sig[c]:0);//This is in <number of generated events>, needs to be
                                                         //weighted according to Lumi*xsec/n_generated, do that 
							 //when the x-sections are read in!
      channel.bgd_qcd            = (qcd.size()?      qcd[c]:0);
      channel.bgd_ewk            = (ewk.size()?      ewk[c]:0);
      channel.bgd_fsr            = (fsr.size()?      fsr[c]:0);
      channel.qcd_contamination  = (sig_qcd.size()?  sig_qcd[c]:0);
      channel.ewk_contamination  = (sig_ewk.size()?  sig_ewk[c]:0);
      channel.data               = (data.size()?     data[c]:0);
      //store uncertainties factorial, i.e. 1 for no unc, 1.1 for 10% unc, 2.0 for 100% etc:
      channel.u_sig_stat         = (u_sig.size()&&sig.size()?           u_sig[c]/sig[c]:0);
      channel.u_lumi             = luminosityUncertainty;
      channel.u_qcd              = (u_qcd.size()&&channel.bgd_qcd?      u_qcd[c]/channel.bgd_qcd           :0);
      channel.u_qcd_stat         = (u_qcd_stat.size()&&channel.bgd_qcd? 1.+ u_qcd_stat[c]/channel.bgd_qcd  :0);
      channel.u_ewk              = (u_ewk.size()&&channel.bgd_ewk?      u_ewk[c]/channel.bgd_ewk           :0);
      channel.u_ewk_stat         = (u_ewk_stat.size()&&channel.bgd_ewk? 1.+ u_ewk_stat[c]/channel.bgd_ewk  :0);
      channel.u_fsr              = (u_fsr.size()&&channel.bgd_fsr?      u_fsr[c]/channel.bgd_fsr           :0);
      channel.u_fsr_stat         = (u_fsr_stat.size()&&channel.bgd_fsr? 1.+ u_fsr_stat[c]/channel.bgd_fsr  :0);
      if (channel.signal>0. && channel.bgd_qcd+channel.bgd_ewk+channel.bgd_fsr>0.1) 
        p.bins.push_back( channel );
    }
    //std::cout << "acc "<<n-1<<"  gluino = "<<p.gluino<<"  squark = "<<p.squark<<"  chi = "<<p.chi <<std::endl;
    Points.Add(p);
  } while(1);  
  if (sig_file.find( WinoScanString )!=string::npos) {
      std::cout << "Run on "<< WinoScanString<<" scan, using GeneratedEvents = 60000"<< std::endl;
  }
  if (fsr_cfg!=dat_cfg) delete fsr_cfg;
  if (dat_cfg!=cfg) delete dat_cfg;
  delete cfg;
}

void AddXsec(std::string filelist) {
	std::cout << "READ XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	p.lumi = luminosity;
	double LO_dn, LO_up, NLO_up, NLO_dn;
	while (1) {
	   masses_file >> p.squark >> p.gluino >> p.chi >> p.xsec >> LO_up >> LO_dn >> p.xsecNLO
	   	   >> NLO_up >> NLO_dn;
	   if (!masses_file.good()) break;
	   //std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
	   //std::cout << "sq" << p.squark << std::endl;
	   //std::cout << "gl" << p.gluino << std::endl;
	   //std::cout << "neutr" << p.chi << std::endl;
	   point * a = 0;
	   a = Points.Get(p.gluino, p.squark, p.chi);
	   if (a){
	     a->xsec    = p.xsec;
	     a->xsecNLO = p.xsecNLO;
	     for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
	       bin->signal *= luminosity*p.xsecNLO/a->totalGenerated;
	       bin->u_NLO = 1.0 + NLO_up / p.xsecNLO; //assume that 'u_NLO_up' is the absolute uncertainty in the same units as 'xsecNLO'
	     }  
	   }
	   //else Points.Add(p); //We don't actually want x-sections for points for which we don't have event yields

	}

}

void AddPDFxsec(const std::string filelist, double neutralinomass=0) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	double u_pdf;
	while (1) {
		if (neutralinomass == 0) {
			masses_file >> p.gluino >> p.squark >> p.chi >> u_pdf;
		} else {
			masses_file >> p.gluino >> p.squark >> u_pdf;
			p.chi = neutralinomass;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, p.chi);
		if (a)
	          for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin)
		    bin->u_pdfxsec = 1.0 + 0.01 * u_pdf;

	}
	masses_file.close();
}
void AddPDFAcceptance(const std::string filelist, double neutralinomass=0) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	double u_pdfacc;
	while (1) {
		if (neutralinomass == 0) {
			masses_file >> p.gluino >> p.squark >> p.chi >> u_pdfacc;
		} else {
			masses_file >> p.gluino >> p.squark >> u_pdfacc;
			p.chi = neutralinomass;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, p.chi);
		if (a)
	          for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin)
		     bin->u_pdfacc = 1.0 + 0.01 * u_pdfacc;

	}
	masses_file.close();
}


int main(void) {
   ////////////////Bino Limits
/*
   Points.Reset();
   ReadSignalAcceptance("inputWinter11/signalAcceptance375.dat", "inputWinter11/data.txt");
   AddXsec("inputWinter11/NLOProspinoXsecs_Bino_Neutr375.txt");
   AddPDFxsec("inputWinter11/PDFcross.txt");
   AddPDFAcceptance("inputWinter11/PDFacceptance.txt");
   Points.Write("limits/GMSBBino100");
*/
   ////////////////Wino Limits
   Points.Reset();
   ReadSignalAcceptance("inputWinter11/signalAcceptanceWino_Nov21_V17ft.dat", "inputWinter11/data.txt");
   AddXsec("inputWinter11/NLOProspinoXsecs_Wino_Neutr375.txt");
   AddPDFxsec("inputWinter11/PDFcross.txt");
   AddPDFAcceptance("inputWinter11/PDFacceptance.txt");
   Points.Write("limits/GMSBWino100");

/*
   //Limits with Neutralino Mass 150
   //Wino
   Points.Reset();
   ReadSignalAcceptance("inputWinter11/signalAcceptanceWinoNeutr150_V10.dat");
   AddXsec("inputWinter11/NLOProspinoXsecs_Wino_Neutr150.txt");
   AddPDFxsec("inputWinter11/PDFcross.txt");
   AddPDFAcceptance("inputWinter11/PDFacceptance.txt");
   Points.Write("limits/GMSBWino200");

   //Bino
   Points.Reset();
   ReadSignalAcceptance("inputWinter11/signalAcceptanceBinoNeutr150_V10.dat");
   AddXsec("inputWinter11/NLOProspinoXsecs_Bino_Neutr150.txt");
   AddPDFxsec("inputWinter11/PDFcross.txt");
   AddPDFAcceptance("inputWinter11/PDFacceptance.txt");
   Points.Write("limits/GMSBBino200");
*/
}
