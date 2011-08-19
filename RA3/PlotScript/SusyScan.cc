#include "SusyScan.h"
#include "ConfigFile.h"

SusyScan::SusyScan() {
	SetPtr();
	for (std::vector<double*>::iterator it = p.begin(); it != p.end(); ++it)
		**it = 0.;
}
SusyScan::SusyScan(const SusyScan& c) {
	SetPtr();

	std::vector<double*>::iterator it = p.begin();
	std::vector<double*>::const_iterator ct = c.p.begin();
	for (; ct != c.p.end(); ++it, ++ct)
		**it = **ct;
}

SusyScan::SusyScan(std::string filename) {
	SetPtr();
	ConfigFile config(filename);

	Mgluino = config.read<double> ("gluino", 0);
	Msquark = config.read<double> ("squark", 0);
	MNeutr = config.read<double> ("chi1", 0);
	// std::cout <<"MASS:"<< Mgluino << ": "<< Msquark <<std::endl;

	signal_acceptance = config.read<double> ("signal.acceptance", 0);
	signal_contamination = config.read<double> ("signal.contamination", 0);
	triggerEffN90Hits = config.read<double> ("signal.triggAcc2", 0);
	triggerEff = config.read<double> ("signal.triggAcc", 0);
	//std::cout <<"CONT:"<< signal_contamination << ": "<< signal_contamination <<std::endl;

	//Xsection 	= config.read<double>("Xsection", 0);
	NLOXsection = config.read<double> ("NLOXsection", 0);
	NLOXSecUp = config.read<double> ("signal.scale.uncertainty.up", 0);
	NLOXSecDown = config.read<double> ("signal.scale.uncertainty.dn", 0);
	PDFXsectionErr = config.read<double> ("signal.PDF.uncertainty", 0);

	ExpXsecLimit = config.read<double> ("limit.cls.expected", 0);
	ObsXsecLimit = config.read<double> ("limit.cls.observed", 0);

	ExpXsecLimitM1 = config.read<double> ("limit.cls.expected.m1sigma", 0);
	ExpXsecLimitP1 = config.read<double> ("limit.cls.expected.p1sigma", 0);
	ExpXsecLimitM2 = config.read<double> ("limit.cls.expected.m2sigma", 0);
	ExpXsecLimitP2 = config.read<double> ("limit.cls.expected.p2sigma", 0);

	ExpXsecLimitSigCont = config.read<double> ("limitSC.cls.expected", 0);
	ObsXsecLimitSigCont = config.read<double> ("limitSC.cls.observed", 0);

	ExpXsecLimitM1SigCont = config.read<double> (
			"limitSC.cls.expected.m1sigma", 0);
	ExpXsecLimitP1SigCont = config.read<double> (
			"limitSC.cls.expected.p1sigma", 0);

	ExpXsecLimitM2SigCont = config.read<double> (
			"limitSC.cls.expected.m2sigma", 0);
	ExpXsecLimitP2SigCont = config.read<double> (
			"limitSC.cls.expected.p2sigma", 0);

	Luminosity = config.read<double> ("Luminosity", 0);

	background = config.read<double> ("background", 0);

}

void SusyScan::SetPtr() {
	if (p.size() != 0)
		return;

	p.push_back(&Mgluino);
	p.push_back(&Msquark);
	p.push_back(&signal_acceptance);
	p.push_back(&signal_contamination);
	p.push_back(&triggerEffN90Hits);
	p.push_back(&triggerEff);
	p.push_back(&NLOXsection);
	p.push_back(&NLOXSecUp);
	p.push_back(&NLOXSecDown);
	p.push_back(&PDFXsectionErr);

	p.push_back(&ObsXsecLimit);
	p.push_back(&ExpXsecLimit);
	p.push_back(&ExpXsecLimitM1);
	p.push_back(&ExpXsecLimitP1);
	p.push_back(&ExpXsecLimitM2);
	p.push_back(&ExpXsecLimitP2);

	p.push_back(&ObsXsecLimitSigCont);
	p.push_back(&ExpXsecLimitSigCont);

	p.push_back(&ExpXsecLimitM1SigCont);
	p.push_back(&ExpXsecLimitP1SigCont);
	p.push_back(&ExpXsecLimitM2SigCont);
	p.push_back(&ExpXsecLimitP2SigCont);
	p.push_back(&Luminosity);
	p.push_back(&background);
}

const SusyScan SusyScan::operator*(const double f) const {
	SusyScan res(*this);
	for (std::vector<double*>::iterator it = res.p.begin(); it != res.p.end(); ++it)
		**it *= f;
	return res;
}

const SusyScan SusyScan::operator+(const SusyScan& f) const {
	SusyScan res(*this);
	std::vector<double*>::iterator it = res.p.begin();
	std::vector<double*>::const_iterator fi = f.p.begin();
	for (; it != res.p.end(); ++it, ++fi)
		**it += **fi;
	return res;
}
