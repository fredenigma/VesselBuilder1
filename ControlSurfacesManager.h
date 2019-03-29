#pragma once

struct CTRLSURF_DEF {
	CTRLSURFHANDLE csfh;
	string name;
	AIRCTRL_TYPE type;
	double area;
	double dCl;
	VECTOR3 ref;
	int axis;
	double delay;
	UINT anim;
	bool defined;
	CTRLSURF_DEF() {
		csfh = NULL;
		name.clear();
		type = AIRCTRL_AILERON;
		area = 0;
		dCl = 0;
		ref = _V(0, 0, 0);
		axis = 0;
		delay = 1;
		anim = (UINT)-1;
		defined = false;
	}
};

class ControlSurfacesManager {
public:
	ControlSurfacesManager(VesselBuilder1 *_VB1);
	~ControlSurfacesManager();
	VesselBuilder1 *VB1;
	VBVector<CTRLSURF_DEF>ctrlsurf_defs;
	UINT CreateUndefinedCtrlSurfDef();
	UINT CreateUndefinedCtrlSurfDef(string name, AIRCTRL_TYPE type, double area, double dCl, VECTOR3 ref, int axis, double delay, UINT anim);
	void DeleteCtrlSurfDef(def_idx d_idx);
	void UndefineCtrlSurface(def_idx d_idx);
	void DefineCtrlSurface(def_idx d_idx);
	bool IsCtrlSurfDefDefined(def_idx d_idx);
	UINT CtrlSurfDefCount();
	string GetCtrlSurfDefName(def_idx d_idx);
	void SetCtrlSurfDefName(def_idx d_idx, string newname);
	void GetCtrlSurfDefParams(def_idx d_idx, AIRCTRL_TYPE &type, double &area, double &dCl, VECTOR3 &ref, int &axis, double &delay, UINT &anim);
	void SetCtrlSurfDefParams(def_idx d_idx, AIRCTRL_TYPE type, double area, double dCl, VECTOR3 ref, int axis, double delay, UINT anim);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void Clear();
};