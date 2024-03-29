#pragma once

struct DCK_DEF {
	string name;
	bool IsDockJett;
	DOCKHANDLE dh;
	VECTOR3 pos, dir, rot;
	VECTOR3 *pos_ptr;
	VECTOR3 *antidir_ptr;
	VECTOR3 *antirot_ptr;
	bool SoftDock;
	double SoftDockDistance;
	def_idx SoftDockAnim;
	DCK_DEF() {
		name.clear();
		IsDockJett = false;
		dh = NULL;
		pos_ptr = new VECTOR3;
		antidir_ptr = new VECTOR3;
		antirot_ptr = new VECTOR3;
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 1);
		rot = _V(0, 1, 0);
		SoftDock = false;
		SoftDockDistance = 0;
		SoftDockAnim = 0;
	}
};

class DockManager {
public:
	DockManager(VesselBuilder1 *_VB1);
	~DockManager();
	VesselBuilder1 *VB1;
	VBVector<DCK_DEF>dock_defs;
	UINT AddDockDef();
	UINT AddDockDef(string name, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot,bool dockjett,bool SoftDock = false, double SoftDockDistance=0, def_idx SoftDockAnim = 0);
	bool DeleteDockDef(def_idx  d_idx);
	void GetDockParams(def_idx d_idx,VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot);
	void SetDockParams(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot);
	string GetDockName(def_idx d_idx);
	void SetDockName(def_idx d_idx, string name);
	bool IsDockJettisonable(def_idx d_idx);
	void SetIsDockJettisonable(def_idx d_idx, bool set);
	void RotateDock(def_idx d_idx, VECTOR3 axis, double angle);
	void MoveDock(def_idx d_idx, VECTOR3 shift);
	void SetDockPos(def_idx d_idx, VECTOR3 pos);
	void SetDockDir(def_idx d_idx, VECTOR3 dir);
	void SetDockRot(def_idx d_idx, VECTOR3 rot);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	UINT GetDockCount();
	DOCKHANDLE GetDH(def_idx d_idx);
	void DockEvent(int dock, OBJHANDLE mate);
	void DockPreStep(double simt, double simdt, double mjd);
	def_idx GetDockIdx(DOCKHANDLE dh);
	VECTOR3* GetDockPosPtr(def_idx d_idx);
	VECTOR3* GetDockAntiDirPtr(def_idx d_idx);
	VECTOR3* GetDockAntiRotPtr(def_idx d_idx);
	UINT GetOrbiterDockIdx(DOCKHANDLE dh);
	VBVector<DOCKHANDLE>docks_to_del;
	VBVector<UINT>docks_jettisoned;

	void SetSoftDockParams(def_idx d_idx, bool enable, double distance, def_idx anim);
	void GetSoftDockParams(def_idx d_idx, bool &enable, double &distance, def_idx &anim);

	void Clear();
};
