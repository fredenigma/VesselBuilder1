#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "AirfoilsManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

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
	//AddPoint(-180 * RAD, 0, 0);
	//AddPoint(0, 0, 0);
	//AddPoint(180 * RAD, 0, 0);
	align = LIFT_VERTICAL;
	def_Model = NODEF;
	InitDefModels();
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
bool LiftCoeffDef::AddPoint(double aoa, double cl, double cm) {
	if ((aoa < -180 * RAD) || (aoa > 180 * RAD)) { return false; }
	UINT index = GetAOAIndex(aoa);
	AOA.insert(AOA.begin()+index, aoa);
	CL.insert(CL.begin() + index, cl);
	CM.insert(CM.begin() + index, cm);
	return true;
}
bool LiftCoeffDef::RemovePoint(UINT index) {
	if (index >= AOA.size()) { return false; }
	AOA.erase(AOA.begin() + index);
	CL.erase(CL.begin() + index);
	CM.erase(CM.begin() + index);
	return true;
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
	double cd;
	if (align == LIFT_VERTICAL) {
		double saoa = sin(aoa);
		double pd = 0.015 + 0.4*saoa*saoa;
		cd = pd + oapiGetInducedDrag(GetCL(aoa), A, 0.7) + oapiGetWaveDrag(M, 0.75, 1.0, 1.1, 0.04);
	}
	else {
		cd = 0.015 + oapiGetInducedDrag(GetCL(aoa), A, 0.6) + oapiGetWaveDrag(M, 0.75, 1.0, 1.1, 0.04);
	}
	
	return cd;
}
void LiftCoeffDef::SetA(double _A) {
	A = _A;
	return;
}
void LiftCoeffDef::SetAlign(AIRFOIL_ORIENTATION _align) {
	align = _align;
	return;
}
AIRFOIL_ORIENTATION LiftCoeffDef::GetAlign() {
	return align;
}
UINT LiftCoeffDef::GetPointsCount() {
	return AOA.size();
}
double LiftCoeffDef::GetTableAOA(UINT index) {
	return AOA[index];
}
double LiftCoeffDef::GetTableCL(UINT index) {
	return CL[index];
}
double LiftCoeffDef::GetTableCM(UINT index) {
	return CM[index];
}
void LiftCoeffDef::SetDefModel(AIRFOILS_DEFAULTS ad) {
	def_Model = ad;
	switch (ad) {
	case NODEF:
	{
		break;
	}
	case DELTAGLIDER:
	{
		AOA.clear();
		CL.clear();
		CM.clear();
		if (GetAlign() == AIRFOIL_ORIENTATION::LIFT_VERTICAL) {
			AOA = AOA_DGV;
			CL = CL_DGV;
			CM = CM_DGV;
		}
		else {
			AOA = AOA_DGH;
			CL = CL_DGH;
			CM = CM_DGH;
		}
		break;
	}
	case SHUTTLE:
	{
		AOA.clear();
		CL.clear();
		CM.clear();
		if (GetAlign() == AIRFOIL_ORIENTATION::LIFT_VERTICAL) {
			AOA = AOA_SHV;
			CL = CL_SHV;
			CM = CM_SHV;
		}
		else {
			AOA = AOA_SHH;
			CL = CL_SHH;
			CM = CM_SHH;
		}
		break;
	}
	}
	return;
}
AIRFOILS_DEFAULTS LiftCoeffDef::GetDefModel() {
	return def_Model;
}
void LiftCoeffDef::InitDefModels() {
	AOA_DGV.clear();
	CL_DGV.clear();
	CM_DGV.clear();
	AOA_SHV.clear();
	CL_SHV.clear();
	CM_SHV.clear();
	AOA_CAPV.clear();
	CL_CAPV.clear();
	CM_CAPV.clear();
	AOA_DGH.clear();
	CL_DGH.clear();
	CM_DGH.clear();
	AOA_SHH.clear();
	CL_SHH.clear();
	CM_SHH.clear();
	AOA_CAPH.clear();
	CL_CAPH.clear();
	CM_CAPH.clear();

	AOA_DGV.push_back(-180 * RAD);
	AOA_DGV.push_back(-60 * RAD);
	AOA_DGV.push_back(-30 * RAD);
	AOA_DGV.push_back(-2 * RAD);
	AOA_DGV.push_back(15 * RAD);
	AOA_DGV.push_back(20 * RAD);
	AOA_DGV.push_back(25 * RAD);
	AOA_DGV.push_back(60 * RAD);
	AOA_DGV.push_back(180 * RAD);
	CL_DGV.push_back(0);
	CL_DGV.push_back(0);
	CL_DGV.push_back(-0.4);
	CL_DGV.push_back(0);
	CL_DGV.push_back(0.7);
	CL_DGV.push_back(1);
	CL_DGV.push_back(0.8);
	CL_DGV.push_back(0);
	CL_DGV.push_back(0);
	CM_DGV.push_back(0);
	CM_DGV.push_back(0);
	CM_DGV.push_back(0.014);
	CM_DGV.push_back(0.0039);
	CM_DGV.push_back(-0.006);
	CM_DGV.push_back(-0.008);
	CM_DGV.push_back(-0.010);
	CM_DGV.push_back(0);
	CM_DGV.push_back(0);

	AOA_DGH.push_back(-180 * RAD);
	AOA_DGH.push_back(-135 * RAD);
	AOA_DGH.push_back(-90 * RAD);
	AOA_DGH.push_back(-45 * RAD);
	AOA_DGH.push_back(45 * RAD);
	AOA_DGH.push_back(90 * RAD);
	AOA_DGH.push_back(135 * RAD);
	AOA_DGH.push_back(180 * RAD);
	CL_DGH.push_back(0);
	CL_DGH.push_back(0.3);
	CL_DGH.push_back(0);
	CL_DGH.push_back(-0.3);
	CL_DGH.push_back(0.3);
	CL_DGH.push_back(0);
	CL_DGH.push_back(-0.3);
	CL_DGH.push_back(0);
	for (UINT i = 0; i < 8; i++) {
		CM_DGH.push_back(0);
	}
	AOA_SHV.push_back(-180 * RAD);
	AOA_SHV.push_back(-165 * RAD);
	AOA_SHV.push_back(-150 * RAD);
	AOA_SHV.push_back(-135 * RAD);
	AOA_SHV.push_back(-120 * RAD);
	AOA_SHV.push_back(-105 * RAD);
	AOA_SHV.push_back(-90 * RAD);
	AOA_SHV.push_back(-75 * RAD);
	AOA_SHV.push_back(-60 * RAD);
	AOA_SHV.push_back(-45 * RAD);
	AOA_SHV.push_back(-30 * RAD);
	AOA_SHV.push_back(-15 * RAD);
	AOA_SHV.push_back(0 * RAD);
	AOA_SHV.push_back(15 * RAD);
	AOA_SHV.push_back(30 * RAD);
	AOA_SHV.push_back(45 * RAD);
	AOA_SHV.push_back(60 * RAD);
	AOA_SHV.push_back(75 * RAD);
	AOA_SHV.push_back(90 * RAD);
	AOA_SHV.push_back(105 * RAD);
	AOA_SHV.push_back(120 * RAD);
	AOA_SHV.push_back(135 * RAD);
	AOA_SHV.push_back(150 * RAD);
	AOA_SHV.push_back(165 * RAD);
	AOA_SHV.push_back(180 * RAD);
	CL_SHV.push_back(0.1);
	CL_SHV.push_back(0.17);
	CL_SHV.push_back(0.2);
	CL_SHV.push_back(0.2);
	CL_SHV.push_back(0.17);
	CL_SHV.push_back(0.1);
	CL_SHV.push_back(0);
	CL_SHV.push_back(-0.11);
	CL_SHV.push_back(-0.24);
	CL_SHV.push_back(-0.38);
	CL_SHV.push_back(-0.5);
	CL_SHV.push_back(-0.5);
	CL_SHV.push_back(-0.02);
	CL_SHV.push_back(0.6355);
	CL_SHV.push_back(0.63);
	CL_SHV.push_back(0.46);
	CL_SHV.push_back(0.28);
	CL_SHV.push_back(0.13);
	CL_SHV.push_back(0);
	CL_SHV.push_back(-0.16);
	CL_SHV.push_back(-0.26);
	CL_SHV.push_back(-0.29);
	CL_SHV.push_back(-0.24);
	CL_SHV.push_back(-0.1);
	CL_SHV.push_back(0.1);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0.002);
	CM_SHV.push_back(0.004);
	CM_SHV.push_back(0.0025);
	CM_SHV.push_back(0.0012);
	CM_SHV.push_back(0);
	CM_SHV.push_back(-0.0012);
	CM_SHV.push_back(-0.0007);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	CM_SHV.push_back(0);
	
	AOA_SHH.push_back(-180 * RAD);
	AOA_SHH.push_back(-157.5 * RAD);
	AOA_SHH.push_back(-135 * RAD);
	AOA_SHH.push_back(-112.5 * RAD);
	AOA_SHH.push_back(-90 * RAD);
	AOA_SHH.push_back(-67.5 * RAD);
	AOA_SHH.push_back(-45 * RAD);
	AOA_SHH.push_back(-22.5 * RAD);
	AOA_SHH.push_back(0 * RAD);
	AOA_SHH.push_back(22.5 * RAD);
	AOA_SHH.push_back(45 * RAD);
	AOA_SHH.push_back(67.5 * RAD);
	AOA_SHH.push_back(90 * RAD);
	AOA_SHH.push_back(112.5 * RAD);
	AOA_SHH.push_back(135 * RAD);
	AOA_SHH.push_back(157.5 * RAD);
	AOA_SHH.push_back(180 * RAD);
	
	CL_SHH.push_back(0);
	CL_SHH.push_back(0.2);
	CL_SHH.push_back(0.3);
	CL_SHH.push_back(0.2);
	CL_SHH.push_back(0);
	CL_SHH.push_back(-0.2);
	CL_SHH.push_back(-0.3);
	CL_SHH.push_back(-0.2);
	CL_SHH.push_back(0);
	CL_SHH.push_back(0.2);
	CL_SHH.push_back(0.3);
	CL_SHH.push_back(0.2);
	CL_SHH.push_back(0);
	CL_SHH.push_back(-0.2);
	CL_SHH.push_back(-0.3);
	CL_SHH.push_back(-0.2);
	CL_SHH.push_back(0);
	for (UINT i = 0; i < 17; i++) {
		CM_SHH.push_back(0);
	}
	return;
}


AirfoilsManager::AirfoilsManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	airfoil_defs.clear();
	return;
}
AirfoilsManager::~AirfoilsManager() { VB1 = NULL; }

UINT AirfoilsManager::CreateAirfoilDef(string name, AIRFOIL_ORIENTATION align, VECTOR3 ref, double c, double S, double A) {
	LogV("Creating Airfoi:%s", name.c_str());
	AIRFOIL_DEFS ad = AIRFOIL_DEFS();
	ad.name = name;
	ad.LCD = new LiftCoeffDef;
	ad.LCD->SetA(A);
	ad.LCD->SetAlign(align);
	ad.airfoil_h = VB1->CreateAirfoil3(align, ref, LiftCoeff, ad.LCD, c, S, A);
	UINT index = airfoil_defs.size();
	airfoil_defs.push_back(ad);
	return index;
}
void AirfoilsManager::SetAirfoilDefName(def_idx d_idx, string newname) {
	airfoil_defs[d_idx].name = newname;
}
string AirfoilsManager::GetAirfoilDefName(def_idx d_idx) {
	return airfoil_defs[d_idx].name;
}
UINT AirfoilsManager::CreateAirfoilDef(AIRFOIL_ORIENTATION align) {
	VECTOR3 ref = _V(0, 0, 0);
	double c = 10;
	double S = 100;
	double A = 1.5;
	char cbuf[256] = { '\0' };
	if (align == AIRFOIL_ORIENTATION::LIFT_VERTICAL) {
		sprintf(cbuf, "V_Airfoil_%i", airfoil_defs.size());
	}
	else {
		sprintf(cbuf, "H_Airfoil_%i", airfoil_defs.size());
	}
	
	string name(cbuf);
	return CreateAirfoilDef(name,align, ref, c, S, A);
}
void AirfoilsManager::DeleteAirfoilDef(def_idx d_idx) {
	LogV("Deleting Airfoil:%i", d_idx);
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
	VB1->EditAirfoil(airfoil_defs[d_idx].airfoil_h, 0x10, _V(0, 0, 0), 0, 0, 0, newA);
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
AIRFOIL_ORIENTATION AirfoilsManager::GetAirfoilDefOrientation(def_idx d_idx) {
	return airfoil_defs[d_idx].LCD->GetAlign();
}
void AirfoilsManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Airfoil Section");
	char cbuf[256] = { '\0' };
	char namebuf[256] = { '\0' };
	int id;
	UINT airfoil_counter = 0;
	sprintf(cbuf, "AIRFOIL_%i_ID", airfoil_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		VECTOR3 ref;
		double c, S, A;
		sprintf(cbuf, "AIRFOIL_%i_NAME", airfoil_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "AIRFOIL_%i_ORIENTATION", airfoil_counter);
		int al;
		oapiReadItem_int(fh, cbuf, al);
		AIRFOIL_ORIENTATION align = (AIRFOIL_ORIENTATION)al;
		sprintf(cbuf, "AIRFOIL_%i_REF", airfoil_counter);
		oapiReadItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "AIRFOIL_%i_C", airfoil_counter);
		oapiReadItem_float(fh, cbuf, c);
		sprintf(cbuf, "AIRFOIL_%i_S", airfoil_counter);
		oapiReadItem_float(fh, cbuf, S);
		sprintf(cbuf, "AIRFOIL_%i_A", airfoil_counter);
		oapiReadItem_float(fh, cbuf, A);
		UINT index = CreateAirfoilDef(name, align, ref, c, S, A);
		UINT point_counter = 0;
		sprintf(cbuf, "AIRFOIL_%i_POINT_%i_AOA", airfoil_counter, point_counter);
		double aoa, cl, cm;
		while (oapiReadItem_float(fh, cbuf, aoa)) {
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CL", airfoil_counter, point_counter);
			oapiReadItem_float(fh, cbuf, cl);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CM", airfoil_counter, point_counter);
			oapiReadItem_float(fh, cbuf, cm);
			aoa *= RAD;
			AddPointAirfoiDef(index, aoa, cl, cm);
			point_counter++;
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_AOA", airfoil_counter, point_counter);
		}

		airfoil_counter++;
		sprintf(cbuf, "AIRFOIL_%i_ID", airfoil_counter);
	}
	
	
	LogV("Parsing Airfoil Section Completed, found %i definitions",airfoil_counter);
	
	
	

	return;
}
void AirfoilsManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------AIRFOILS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < GetAirfoilDefCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "AIRFOIL_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "AIRFOIL_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetAirfoilDefName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		VECTOR3 ref;
		double c, S, A;
		AIRFOIL_ORIENTATION align = GetAirfoilDefOrientation(i);
		GetAirfoilDefParams(i, ref, c, S, A);
		sprintf(cbuf, "AIRFOIL_%i_ORIENTATION", i);
		oapiWriteItem_int(fh, cbuf, (int)align);
		sprintf(cbuf, "AIRFOIL_%i_REF", i);
		oapiWriteItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "AIRFOIL_%i_C", i);
		oapiWriteItem_float(fh, cbuf, c);
		sprintf(cbuf, "AIRFOIL_%i_S", i);
		oapiWriteItem_float(fh, cbuf, S);
		sprintf(cbuf, "AIRFOIL_%i_A", i);
		oapiWriteItem_float(fh, cbuf, A);
		for (UINT j = 0; j < GetAirfoilDefPointsCount(i); j++) {
			double aoa, cl, cm;
			GetAirfoilDefPoint(i, j, aoa, cl, cm);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_AOA", i, j);
			oapiWriteItem_float(fh, cbuf, aoa*DEG);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CL", i, j);
			oapiWriteItem_float(fh, cbuf, cl);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CM", i, j);
			oapiWriteItem_float(fh, cbuf, cm);
		}
		oapiWriteLine(fh, " ");
	}
	return;
}
UINT AirfoilsManager::GetAirfoilDefCount() {
	return airfoil_defs.size();
}
UINT AirfoilsManager::GetAirfoilDefPointsCount(def_idx d_idx) {
	return airfoil_defs[d_idx].LCD->GetPointsCount();
}
void AirfoilsManager::GetAirfoilDefPoint(def_idx d_idx, UINT point_indx, double &aoa, double &cl, double &cm) {
	aoa = airfoil_defs[d_idx].LCD->GetTableAOA(point_indx);
	cl = airfoil_defs[d_idx].LCD->GetTableCL(point_indx);
	cm = airfoil_defs[d_idx].LCD->GetTableCM(point_indx);
	return;
}
bool AirfoilsManager::AddPointAirfoiDef(def_idx d_idx, double aoa, double cl, double cm) {
	return airfoil_defs[d_idx].LCD->AddPoint(aoa, cl, cm);
}
bool AirfoilsManager::RemovePointAirfoilDef(def_idx d_idx, UINT point_indx) {
	return airfoil_defs[d_idx].LCD->RemovePoint(point_indx);
}

AIRFOILS_DEFAULTS AirfoilsManager::GetAirfoilDefModel(def_idx d_idx) {
	return airfoil_defs[d_idx].LCD->GetDefModel();
}
void AirfoilsManager::SetAirfoilDefModel(def_idx d_idx, AIRFOILS_DEFAULTS adf) {
	airfoil_defs[d_idx].LCD->SetDefModel(adf);
	return;
}
void AirfoilsManager::Clear() {
	LogV("Clearing Airfoils");
	VB1->ClearAirfoilDefinitions();
	airfoil_defs.clear();
	LogV("Clearing Airfoils Completed");
	return;
}