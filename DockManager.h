#pragma once

struct DCK_DEF {
	string name;
	bool IsDockJett;
	DOCKHANDLE dh;
	VECTOR3 *pos_ptr;
	VECTOR3 *antidir_ptr;
	VECTOR3 *antirot_ptr;
	DCK_DEF() {
		name.clear();
		IsDockJett = false;
		dh = NULL;
		pos_ptr = new VECTOR3;
		antidir_ptr = new VECTOR3;
		antirot_ptr = new VECTOR3;
	}
};

class DockManager {
public:
	DockManager(VesselBuilder1 *_VB1);
	~DockManager();
	VesselBuilder1 *VB1;
	vector<DCK_DEF>dock_defs;
	UINT AddDockDef();
	UINT AddDockDef(string name, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot,bool dockjett);
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
	void DockEvent(int dock, OBJHANDLE mate);
	def_idx GetDockIdx(DOCKHANDLE dh);
	VECTOR3* GetDockPosPtr(def_idx d_idx);
	VECTOR3* GetDockAntiDirPtr(def_idx d_idx);
	VECTOR3* GetDockAntiRotPtr(def_idx d_idx);
	void Clear();
};
