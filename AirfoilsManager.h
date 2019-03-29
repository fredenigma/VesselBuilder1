#pragma once
enum AIRFOILS_DEFAULTS{NODEF,DELTAGLIDER,SHUTTLE,CAPSULE};

class LiftCoeffDef {
public:
	LiftCoeffDef();
	~LiftCoeffDef();
	VBVector<double>AOA;
	VBVector<double>CL;
	VBVector<double>CM;
	
	AIRFOILS_DEFAULTS def_Model;
	double A;
	bool AddPoint(double aoa, double cl,double cm);
	bool RemovePoint(UINT index);
	UINT GetAOAIndex(double aoa);
	double GetCL(double aoa);
	double GetCM(double aoa);
	double GetCD(double aoa,double M);
	void SetA(double _A);
	UINT GetPointsCount();
	double GetTableAOA(UINT index);
	double GetTableCL(UINT index);
	double GetTableCM(UINT index);
	AIRFOIL_ORIENTATION align;
	void SetAlign(AIRFOIL_ORIENTATION _align);
	AIRFOIL_ORIENTATION GetAlign();
	void SetDefModel(AIRFOILS_DEFAULTS ad);
	AIRFOILS_DEFAULTS GetDefModel();
	void InitDefModels();
	VBVector<double>AOA_DGV;
	VBVector<double>CL_DGV;
	VBVector<double>CM_DGV;
	VBVector<double>AOA_DGH;
	VBVector<double>CL_DGH;
	VBVector<double>CM_DGH;
	VBVector<double>AOA_SHV;
	VBVector<double>CL_SHV;
	VBVector<double>CM_SHV;
	VBVector<double>AOA_SHH;
	VBVector<double>CL_SHH;
	VBVector<double>CM_SHH;
	VBVector<double>AOA_CAPV;
	VBVector<double>CL_CAPV;
	VBVector<double>CM_CAPV;
	VBVector<double>AOA_CAPH;
	VBVector<double>CL_CAPH;
	VBVector<double>CM_CAPH;
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
	VBVector<AIRFOIL_DEFS> airfoil_defs;
	UINT CreateAirfoilDef(string name,AIRFOIL_ORIENTATION align,VECTOR3 ref,double c, double S, double A);
	UINT CreateAirfoilDef(AIRFOIL_ORIENTATION align);
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
	void GetAirfoilDefParams(def_idx d_idx, VECTOR3 &ref, double &c, double &S, double &A);
	AIRFOIL_ORIENTATION GetAirfoilDefOrientation(def_idx d_idx);
	UINT GetAirfoilDefCount();
	UINT GetAirfoilDefPointsCount(def_idx d_idx);
	void GetAirfoilDefPoint(def_idx d_idx, UINT point_indx, double &aoa, double &cl, double &cm);
	bool AddPointAirfoiDef(def_idx d_idx, double aoa, double cl, double cm);
	bool RemovePointAirfoilDef(def_idx d_idx, UINT point_indx);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	AIRFOILS_DEFAULTS GetAirfoilDefModel(def_idx d_idx);
	void SetAirfoilDefModel(def_idx d_idx, AIRFOILS_DEFAULTS adf);
	void Clear();
};