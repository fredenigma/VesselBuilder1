#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "AirfoilsManager.h"


/*void VLiftCoeff(VESSEL *v, double aoa, double M, double Re, void *context, double *cl, double *cm, double *cd) {
	

	int i;
	const int nabsc = 9;
	static const double AOA[nabsc] = { -180 * RAD,-60 * RAD,-30 * RAD, -2 * RAD, 15 * RAD,20 * RAD,25 * RAD,60 * RAD,180 * RAD };
	static const double CL[nabsc] = { 0,      0,   -0.4,      0,    0.7,     1,   0.8,     0,      0 };
	static const double CM[nabsc] = { 0,      0,  0.014, 0.0039, -0.006,-0.008,-0.010,     0,      0 };
	for (i = 0; i < nabsc - 1 && AOA[i + 1] < aoa; i++);
	double f = (aoa - AOA[i]) / (AOA[i + 1] - AOA[i]);
	*cl = CL[i] + (CL[i + 1] - CL[i]) * f;  // aoa-dependent lift coefficient
	*cm = CM[i] + (CM[i + 1] - CM[i]) * f;  // aoa-dependent moment coefficient
	double saoa = sin(aoa);
	double pd = 0.015 + 0.4*saoa*saoa;  // profile drag
	*cd = pd + oapiGetInducedDrag(*cl, 1.5, 0.7) + oapiGetWaveDrag(M, 0.75, 1.0, 1.1, 0.04);
	// profile drag + (lift-)induced drag + transonic/supersonic wave (compressibility) drag
	return;
}

void HLiftCoeff(VESSEL *v, double beta, double M, double Re, void *context, double *cl, double *cm, double *cd) {
	int i;
	const int nabsc = 8;
	static const double BETA[nabsc] = { -180 * RAD,-135 * RAD,-90 * RAD,-45 * RAD,45 * RAD,90 * RAD,135 * RAD,180 * RAD };
	static const double CL[nabsc] = { 0,    +0.3,      0,   -0.3,  +0.3,     0,   -0.3,      0 };
	for (i = 0; i < nabsc - 1 && BETA[i + 1] < beta; i++);
	*cl = CL[i] + (CL[i + 1] - CL[i]) * (beta - BETA[i]) / (BETA[i + 1] - BETA[i]);
	*cm = 0.0;
	*cd = 0.015 + oapiGetInducedDrag(*cl, 1.5, 0.6) + oapiGetWaveDrag(M, 0.75, 1.0, 1.1, 0.04);
	return;
}
*/
void LiftCoeff(VESSEL *v, double aoa, double M, double Re, void *context, double *cl, double *cm, double *cd) {
	*cl = ((LiftCoeffDef*)context)->GetCL(aoa);
	*cm = ((LiftCoeffDef*)context)->GetCM(aoa);
	*cd = ((LiftCoeffDef*)context)->GetCD(aoa,M);
	return;
}

LiftCoeffDef::LiftCoeffDef() {
	AOA.clear();
	CL.clear();
	CM.clear();
	AddPoint(-180 * RAD, 0, 0);
	AddPoint(0, 0, 0);
	AddPoint(180 * RAD, 0, 0);
	return;
}
LiftCoeffDef::~LiftCoeffDef() {}

UINT LiftCoeffDef::GetAOAIndex(double aoa) {
	if (aoa < -180 * RAD) {
		return 0;
	}else if(aoa > 180 * RAD) { 
		return AOA.size(); 
	}
	for (UINT i = 0; i < AOA.size(); i++) {
		if (aoa < AOA[i]) {
			return i;
		}
	}
	return AOA.size();
}
void LiftCoeffDef::AddPoint(double aoa, double cl, double cm) {
	if ((aoa < -180 * RAD) || (aoa > 180 * RAD)) { return; }
	UINT index = GetAOAIndex(aoa);
	AOA.insert(AOA.begin()+index, aoa);
	CL.insert(CL.begin() + index, cl);
	CM.insert(CM.begin() + index, cm);


	return;
}
void LiftCoeffDef::RemovePoint(UINT index) {
	if (index >= AOA.size()) { return; }
	AOA.erase(AOA.begin() + index);
	CL.erase(CL.begin() + index);
	CM.erase(CM.begin() + index);
	return;
}
double LiftCoeffDef::GetCL(double aoa) {
	if (aoa == -180 * RAD) { return CL[0]; }
	else if (aoa == 180 * RAD) { return CL.back(); }
	UINT index = GetAOAIndex(aoa);
	double increment = (aoa - AOA[index - 1]) / (AOA[index] - AOA[index - 1]);
	double cl = CL[index - 1] + increment*(CL[index] - CL[index - 1]);
	return cl;
}
double LiftCoeffDef::GetCM(double aoa) {
	if (aoa == -180 * RAD) { return CM [0]; }
	else if (aoa == 180 * RAD) { return CM.back(); }
	UINT index = GetAOAIndex(aoa);
	double increment = (aoa - AOA[index - 1]) / (AOA[index] - AOA[index - 1]);
	double cm = CM[index - 1] + increment*(CM[index] - CM[index - 1]);
	return cm;
}
double LiftCoeffDef::GetCD(double aoa,double M) {
	double saoa = sin(aoa);
	double pd = 0.015 + 0.4*saoa*saoa;  
	double cd = pd + oapiGetInducedDrag(GetCL(aoa), A, 0.7) + oapiGetWaveDrag(M, 0.75, 1.0, 1.1, 0.04);
	return 0;
}
void LiftCoeffDef::SetA(double _A) {
	A = _A;
	return;
}



AirfoilsManager::AirfoilsManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	airfoil_defs.clear();
	return;
}
AirfoilsManager::~AirfoilsManager() { VB1 = NULL; }

void AirfoilsManager::CreateAirfoilDef(string name, AIRFOIL_ORIENTATION align, VECTOR3 ref, double c, double S, double A) {
	AIRFOIL_DEFS ad = AIRFOIL_DEFS();
	ad.name = name;
	ad.LCD = new LiftCoeffDef;
	ad.LCD->SetA(A);
	ad.airfoil_h = VB1->CreateAirfoil3(align, ref, LiftCoeff, ad.LCD, c, S, A);
	airfoil_defs.push_back(ad);
	return;
}
void AirfoilsManager::SetAirfoilDefName(def_idx d_idx, string newname) {
	airfoil_defs[d_idx].name = newname;
}
string AirfoilsManager::GetAirfoilDefName(def_idx d_idx) {
	return airfoil_defs[d_idx].name;
}
void AirfoilsManager::CreateAirfoilDef(AIRFOIL_ORIENTATION align) {
	VECTOR3 ref = _V(0, 0, 0);
	double c = 10;
	double S = 100;
	double A = 1.5;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Airfoil_%i", airfoil_defs.size());
	string name(cbuf);
	return CreateAirfoilDef(name,align, ref, c, S, A);
}
void AirfoilsManager::DeleteAirfoilDef(def_idx d_idx) {
	VB1->DelAirfoil(airfoil_defs[d_idx].airfoil_h);
	delete airfoil_defs[d_idx].LCD;
	airfoil_defs[d_idx].LCD = NULL;
	airfoil_defs.erase(airfoil_defs.begin() + d_idx);
	return;
}
void AirfoilsManager::SetAirfoilDefRef(def_idx d_idx, VECTOR3 newref) {
	VB1->EditAirfoil(airfoil_defs[d_idx].airfoil_h, 0x01, newref, 0, 0, 0, 0);
	return;
}
void AirfoilsManager::SetAirfoilDefC(def_idx d_idx, double newC){
	VB1->EditAirfoil(airfoil_defs[d_idx].airfoil_h, 0x04, _V(0,0,0), 0, newC, 0, 0);
	return;
}
void AirfoilsManager::SetAirfoilDefS(def_idx d_idx, double newS) {
	VB1->EditAirfoil(airfoil_defs[d_idx].airfoil_h, 0x08, _V(0, 0, 0), 0, 0, newS, 0);
	return;
}
void AirfoilsManager::SetairfoilDefA(def_idx d_idx, double newA) {
	VB1->EditAirfoil(airfoil_defs[d_idx].airfoil_h, 0x04, _V(0, 0, 0), 0, 0, 0, newA);
	return;
}
VECTOR3 AirfoilsManager::GetAirfoilDefRef(def_idx d_idx) {
	VECTOR3 ref;
	VB1->GetAirfoilParam(airfoil_defs[d_idx].airfoil_h, &ref, NULL, NULL, NULL, NULL, NULL);
	return ref;
}
double AirfoilsManager::GetAirfoilDefC(def_idx d_idx) {
	double c;
	VB1->GetAirfoilParam(airfoil_defs[d_idx].airfoil_h, NULL, NULL, NULL, &c, NULL, NULL);
	return c;
}
double AirfoilsManager::GetAirfoilDefS(def_idx d_idx) {
	double S;
	VB1->GetAirfoilParam(airfoil_defs[d_idx].airfoil_h, NULL, NULL, NULL, NULL, &S, NULL);
	return S;
}
double AirfoilsManager::GetAirfoilDefA(def_idx d_idx) {
	double A;
	VB1->GetAirfoilParam(airfoil_defs[d_idx].airfoil_h, NULL, NULL, NULL, NULL, NULL, &A);
	return A;
}
void AirfoilsManager::GetAirfoilDefParams(def_idx d_idx, VECTOR3 &ref, double &c, double &S, double &A){
	VB1->GetAirfoilParam(airfoil_defs[d_idx].airfoil_h, &ref, NULL, NULL, &c, &S, &A);
	return;
}
void AirfoilsManager::ParseCfgFile(FILEHANDLE fh) {
	return;
}
void AirfoilsManager::WriteCfg(FILEHANDLE fh) {
	return;
}