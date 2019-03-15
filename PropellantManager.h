#pragma once
struct PRP_DEF {
	PROPELLANT_HANDLE ph;
	string name;
	bool retain_fuel;
	PRP_DEF() {
		ph = NULL;
		name.clear();
		retain_fuel = false;
	}
};

class PropellantManager {
public:
	PropellantManager(VesselBuilder1 *_VB1);
	~PropellantManager();
	VesselBuilder1 *VB1;
	vector<PRP_DEF> tanks;
	void AddTankDef();
	void AddTankDef(string name, double MaxMass, double efficiency, double currentMass);
	void DelTankDef(def_idx d_idx);
	string GetTankName(def_idx d_idx);
	double GetTankMaxMass(def_idx d_idx);
	double GetTankCurrentMass(def_idx d_idx);
	double GetTankEfficiency(def_idx d_idx);
	void SetTankName(def_idx d_idx, string newname);
	void SetTankMaxMass(def_idx d_idx, double newmaxmass);
	void SetTankCurrentMass(def_idx d_idx, double newcurrentmass);
	void SetTankEfficiency(def_idx d_idx, double newefficiency);
	DWORD GetPropCount();
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void MakePrimary(def_idx d_idx);
	bool IsPrimary(def_idx d_idx);
	PROPELLANT_HANDLE GetTankPH(def_idx d_idx);
	PROPELLANT_HANDLE GetMainTankPH();
	int GetPrpIdx(PROPELLANT_HANDLE ph);
	bool GetPrpRetainFuel(def_idx d_idx);
	void SetPrpRetainFuel(def_idx d_idx, bool set);
	void Clear();
};