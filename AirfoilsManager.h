#pragma once


class LiftCoeffDef {
public:
	LiftCoeffDef();
	~LiftCoeffDef();
	vector<double>AOA;
	vector<double>CL;
	vector<double>CM;
	double A;
	void AddPoint(double aoa, double cl,double cm);
	void RemovePoint(UINT index);
	UINT GetAOAIndex(double aoa);
	double GetCL(double aoa);
	double GetCM(double aoa);
	double GetCD(double aoa,double M);
	void SetA(double _A);
};
struct AIRFOIL_DEFS {
	string name;
	AIRFOILHANDLE airfoil_h;
	LiftCoeffDef* LCD;
	AIRFOIL_DEFS() {
		name.clear();
		airfoil_h = NULL;
		LCD = NULL;
	}
};
class AirfoilsManager {
public:
	AirfoilsManager(VesselBuilder1* _VB1);
	~AirfoilsManager();
	VesselBuilder1 *VB1;
	vector<AIRFOIL_DEFS> airfoil_defs;
	void CreateAirfoilDef(string name,AIRFOIL_ORIENTATION align,VECTOR3 ref,double c, double S, double A);
	void CreateAirfoilDef(AIRFOIL_ORIENTATION align);
	void SetAirfoilDefName(def_idx d_idx, string newname);
	string GetAirfoilDefName(def_idx d_idx);
	void DeleteAirfoilDef(def_idx d_idx);
	void SetAirfoilDefRef(def_idx d_idx, VECTOR3 newref);
	void SetAirfoilDefC(def_idx d_idx, double newC);
	void SetAirfoilDefS(def_idx d_idx, double newS);
	void SetairfoilDefA(def_idx d_idx, double newA);
	VECTOR3 GetAirfoilDefRef(def_idx d_idx);
	double GetAirfoilDefC(def_idx d_idx);
	double GetAirfoilDefS(def_idx d_idx);
	double GetAirfoilDefA(def_idx d_idx);
	void GetAirfoilDefParams(def_idx d_idx,VECTOR3 &ref, double &c, double &S, double &A);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
};