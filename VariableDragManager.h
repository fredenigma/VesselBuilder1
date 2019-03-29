#pragma once

struct VARD_DEF {
	string name;
	double factor;
	VECTOR3 ref;
	UINT anim_idx;
	bool defined;
	VARD_DEF() {
		name.clear();
		factor = 0;
		ref = _V(0, 0, 0);
		anim_idx = (UINT)-1;
		defined = true;
	}
};

class VariableDragManager {
public:
	VariableDragManager(VesselBuilder1* _VB1);
	~VariableDragManager();
	VesselBuilder1* VB1;
	VBVector<VARD_DEF> vard_def;
	void AddUndefinedVardDef();
	UINT AddVardDef(string name, UINT anim_idx, double factor, VECTOR3 ref);
	UINT GetVardDefCount();
	void DefineVarDef(def_idx d_idx);
	void DefineAll();
	void DeleteVarDef(def_idx d_idx);
	string GetVardName(def_idx d_idx);
	void SetVardName(def_idx d_idx, string newname);
	void GetVardParams(def_idx d_idx, double &factor, VECTOR3 &ref, UINT &anim_idx);
	void SetVardParams(def_idx d_idx, double factor, VECTOR3 ref, UINT anim_index);
	void UnDefineVardDef(def_idx d_idx);
	bool IsVardDefDefined(def_idx d_idx);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void Clear();
};