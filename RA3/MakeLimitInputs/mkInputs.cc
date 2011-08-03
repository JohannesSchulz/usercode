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

//cut on MET > 100
const double data = 45;
const double background = 43.8;
const double backgroundUncertainty = sqrt(pow(4.2, 2) + pow(7.6, 2));
//cut on MET > 200
const double data200 = 7;
const double background200 = 6.03256;
const double backgroundUncertainty200 = sqrt(1.66396*1.66396 + 2.43536*2.43536);
//cut on MET > 350
const double data350 = 1;
const double background350 = 0.146571;
const double backgroundUncertainty350 = sqrt(0.0241711 *0.0241711+ 0.0444952*0.0444952);
//LUMI
const double luminosity = 909.0;
const double luminosityUncertainty = 0.06 * luminosity; //6%
const double scaleUncertainty = 0.2; //suggested by Yuri, see mail June 30th.

struct point {
	double gluino;
	double squark;
	double chi;
	double xsec;
	double xsecNLO;
	double lumi;
	double u_lumi;
	double kfactor;
	double u_NLO;
	double u_NLO_up;
	double u_NLO_dn;
	double acc;
	double u_acc;
	double u_pdf;
	double u_pdfacc;
	double u_stat;
	double signal_contamination;
	double signal_controlregion;
	double back;
	double u_back;
	double data;
	double ngen;
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
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
			std::ofstream ofile;
			std::stringstream ss;
			ss << dir << "_" << it->gluino << "_" << it->squark << "_"
					<< it->chi << ".txt";
			ofile.open(ss.str().c_str());
			ofile << "gluino = " << it->gluino << "\n";
			ofile << "squark = " << it-> squark << "\n";
			ofile << "chi1 = " << it-> chi << "\n";
			ofile << "Xsection = " << it-> xsec << "\n";
			ofile << "NLOXsection = " << it-> xsecNLO << "\n";
			ofile << "Luminosity = " << it-> lumi << "\n";
			ofile << "Luminosity.uncertainty = " << it-> u_lumi << "\n";
			ofile << "kfactor = " << it-> kfactor << "\n";
			ofile << "signal.scale.uncertainty = " << it-> u_NLO << "\n";
			ofile << "signal.scale.uncertainty.up = " << it-> u_NLO_up << "\n";
			ofile << "signal.scale.uncertainty.dn = " << it-> u_NLO_dn << "\n";
			ofile << "signal.acceptance = " << it-> acc << "\n";
			ofile << "signal.Stat.uncertainty = " << it->u_stat << "\n";
			ofile << "signal.acceptance.uncertainty = " << it-> u_acc << "\n";
			ofile << "signal.PDF.uncertainty = " << it-> u_pdf << "\n";
			ofile << "signal.PDFacc.uncertainty = " << it-> u_pdfacc << "\n";
			ofile << "background = " << it-> back << "\n";
			ofile << "background.uncertainty = " << it-> u_back << "\n";
			ofile << "data = " << it-> data << "\n";
			ofile << "signal.ngen = " << it->ngen << "\n";

			ofile << "signal.contamination = " << it->signal_contamination
					<< "\n";
			std::cout << "WRITE signal_contamination:"
					<< it->signal_contamination << std::endl;
			std::cout << "WRITE GL:" << it->gluino << std::endl;
			std::cout << "WRITE SQ:" << it->squark << std::endl;
			ofile << "signal.controlregion = " << it->signal_controlregion
					<< "\n";
			ofile.close();
		}
	}
private:
	std::vector<point> p_;
} Points;

void ReadXsec(const std::string filelist, bool useCut2 = false, bool useCut3 =
		false) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	p.data = data;
	p.back = background;
	p.u_back = backgroundUncertainty;
	if (useCut2) {
		p.data = data200;
		p.back = background200;
		p.u_back = backgroundUncertainty200;
	}
	if (useCut3) {
		p.data = data350;
		p.back = background350;
		p.u_back = backgroundUncertainty350;
	}
	if (useCut2 && useCut3) {
		std::cout
				<< "ERROR---------which cut do you want to use for Acceptance?!?"
				<< std::endl;
	}
	p.lumi = luminosity;
	p.u_lumi = luminosityUncertainty;
	p.u_NLO = scaleUncertainty;
	double nAcc;
	double LO_dn, LO_up;
	while (1) {

		masses_file >> p.gluino >> p.squark >> p.chi 
		            >> p.xsec >> LO_up >> LO_dn >> p.xsecNLO >> p.u_NLO_up >> p.u_NLO_dn;
		//xsec in file is given in mb--->transform to pb
                p.kfactor = p.xsecNLO/p.xsec;
		std::cout << "xsec" << p.xsec << std::endl;
		if (p.chi == 375) {
			if (!masses_file.good())
				break;
			point * a = 0;
			a = Points.Get(p.gluino, p.squark, p.chi);
			if (!a) {
				Points.Add(p);
			}
		}

	}
}
void ReadSignalAcceptance(const std::string filelist,
		double applyFilterEffNEvts = 0, bool useCut2 = false, bool useCut3 =
				false) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		double muell;
		double acc200, sigcont200;
		double acc350, sigcont350;
		masses_file >> p.squark >> p.gluino >> p.acc >> p.signal_contamination
				>> p.ngen >> acc200 >> sigcont200 >> acc350 >> sigcont350
				>> muell >> muell >> muell >> muell >> muell >> muell >> muell
				>> muell >> muell >> muell;
		std::cout << "squark:" << p.squark << std::endl;
		std::cout << "gl:" << p.gluino << std::endl;
		if (applyFilterEffNEvts > 0) {
			p.ngen = applyFilterEffNEvts;
		}
		if (useCut2) {
			p.acc = acc200;
			p.signal_contamination = sigcont200;
		}
		if (useCut3) {
			p.acc = acc350;
			p.signal_contamination = sigcont350;
		}
		if (useCut2 && useCut3) {
			std::cout
					<< "ERROR---------which cut do you want to use for Acceptance?!?"
					<< std::endl;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, 375);
		if (a) {
			a->ngen = p.ngen;
			a->u_stat = sqrt(p.acc) / p.ngen;

			a->acc = p.acc / p.ngen;
			double perc_uncert_JES = 0.02;
			double perc_uncert_PU = 0.01;
			double perc_uncert_photon = 0.005;
			double perc_uncert_phDatMC = 0.01;

			a->u_acc = a->acc * sqrt(pow(a->u_stat, 2) + 
			                         pow(a->u_pdfacc, 2)+ 
						 pow(a->u_pdf,2 ) +
						 pow(perc_uncert_JES, 2) + 
						 pow(perc_uncert_PU, 2) + 
						 pow(perc_uncert_photon, 2) + 
						 pow(perc_uncert_phDatMC, 2)); 
			a->signal_contamination = p.signal_contamination / p.ngen;
			std::cout << "lumi=" << a->lumi 
			          << ", xsec=" << a->xsec 
				  << ", eff=" << a->acc 
				  << " +- " << a->u_acc
				  << ", SC=" << a->signal_contamination 
				  << ", Ngen=" << a->ngen 
				  << std::endl;
		}

	}
	masses_file.close();
}
void ReadPDF(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		masses_file >> p.gluino >> p.squark >> p.u_pdf;
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, 375);
		if (a)
			a->u_pdf = 0.01 * p.u_pdf;

	}
	masses_file.close();
}
void ReadPDFAcceptance(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		masses_file >> p.gluino >> p.squark >> p.u_pdfacc;
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, 375);
		if (a)
			a->u_pdfacc = 0.01 * p.u_pdfacc;

	}
	masses_file.close();
}

void ReadkFactor(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	while (1) {
		masses_file >> p.gluino >> p.squark >> p.kfactor;
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, 375);
		if (a)
			a->kfactor = p.kfactor;

	}
	masses_file.close();
}

int main(void) {
	////////////////Bino Limits
	//MET>100
	Points.Reset();
	ReadXsec("NLOProspinoXsecs.txt");
	ReadPDF("xsectionPDFErrors.dat");
	ReadPDFAcceptance("acceptancePDFErrors.dat");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceBinoV4_Jul30.dat");
	Points.Write("limits/GMSBBino100");
	//MET>200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs.txt", true, false);
	ReadPDF("xsectionPDFErrors.dat");
	ReadPDFAcceptance("acceptancePDFErrors.dat");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceBinoV4_Jul30.dat", 0, true, false);
	Points.Write("limits/GMSBBino200");
	//MET>350
	Points.Reset();
	ReadXsec("NLOProspinoXsecs.txt", false, true);
	ReadPDF("xsectionPDFErrors.dat");
	ReadPDFAcceptance("acceptancePDFErrors.dat");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceBinoV4_Jul30.dat", 0, false, true);
	Points.Write("limits/GMSBBino350");
	////////////////Wino Limits
	//MET >100
	Points.Reset();
	ReadXsec("NLOProspinoXsecs.txt");
	ReadPDF("xsectionPDFErrors.dat");
	ReadPDFAcceptance("acceptancePDFErrors.dat");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceWino_V4_Jul30.dat", 60000);
	Points.Write("limits/GMSBWino100");
	//MET >200
	Points.Reset();
	ReadXsec("NLOProspinoXsecs.txt", true, false);
	ReadPDF("xsectionPDFErrors.dat");
	ReadPDFAcceptance("acceptancePDFErrors.dat");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceWino_V4_Jul30.dat", 60000, true, false);
	Points.Write("limits/GMSBWino200");
	//MET >350
	Points.Reset();
	ReadXsec("NLOProspinoXsecs.txt", false, true);
	ReadPDF("xsectionPDFErrors.dat");
	ReadPDFAcceptance("acceptancePDFErrors.dat");
	//ReadkFactor("ProspinoKfactorsDiphotonsAll.txt");
	ReadSignalAcceptance("signalAcceptanceWino_V4_Jul30.dat", 60000, false, true);
	Points.Write("limits/GMSBWino350");
}
