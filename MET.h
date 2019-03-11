#pragma once

class MET {
public:
	MET();
	MET(double reset_mjd);
	~MET();
	double mjd0;
	double d_secs;
	double d_mjd;
	const double secmjd = 1 / 86400;
	const double mjdsec = 86400;
	void PreStep(double mjd);
	double GetMJD0();
	double GetDMJD();
	double GetDeltaSeconds();
	/*double GetSeconds();
	double GetMinutes();
	double GetHours();*/
	void GetHMS(int &sign,int &hrs,int &mins, int &secs);
	void SetMJD0(double mjd);
	void Reset();
	int MyRound(double x);
};