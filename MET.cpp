#include "VesselBuilder1.h"
#include "MET.h"
#include <math.h>

MET::MET() {
	mjd0 = oapiGetSimMJD();
	d_secs = 0;
	d_mjd = 0;
	enabled = false;
	return;
}
MET::MET(double reset_mjd) {
	mjd0 = reset_mjd;
	d_secs = 0;
	d_mjd = 0;
	enabled = true;
	return;
}
MET::~MET() {}

int MET::MyRound(double x) {
	return(int)x - (x < (int)x);
}
void MET::PreStep(double simdt, double mjd) {
	d_mjd = mjd - mjd0;
	d_secs = (d_mjd)*mjdsec;
	//d_secs += simdt;
	return;
}
double MET::GetMJD0() {
	return mjd0;
}
double MET::GetDMJD() {
	return d_mjd;
}
double MET::GetDeltaSeconds() {
	return d_secs;
}

void MET::GetHMS(int &sign, int &hrs, int &mins, int &secs) {

	/*int seconds, hours, minutes;
	if (d_secs >= 0) {
		seconds = (int)d_secs;
	}
	else {
		seconds = (int)(d_secs);
	}
	
	minutes = seconds / 60;
	hours = minutes / 60;
	hrs = abs(int(hours));
	mins = abs(int(minutes % 60));
	secs = abs(int(seconds % 60));*/
	double secs_input = MyRound(d_secs);
	double hours = abs(secs_input) / 3600;
	hrs = int(hours);
	double remainder_hrs = hours - (double)hrs;
	double minutes = remainder_hrs * 60;
	mins = int(minutes);
	double remainder_mins = minutes - (double)mins;
	double seconds = remainder_mins * 60;
	secs = (int)std::round(seconds);

	if (d_secs >= 0) {
		sign = 1;
	}
	else {
		sign = -1;
	}
//	sprintf(oapiDebugString(), "seconds:%.3f secs:%i secs_input:%.3f", seconds, secs,secs_input);
	return;
}
void MET::SetMJD0(double mjd) {
	mjd0 = mjd;
	d_secs = (oapiGetSimMJD()-mjd0)*mjdsec;
}
void MET::Reset() {
	mjd0 = oapiGetSimMJD();
	d_secs = 0;
	return;
}