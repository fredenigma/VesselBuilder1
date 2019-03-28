#pragma once

class SoftDock {
public:
	SoftDock(VesselBuilder1 *_VB1);
	~SoftDock();
	bool SoftDockCheck(OBJHANDLE &hves, DOCKHANDLE &hdock);
	VesselBuilder1* VB1;
	DockManager *DckMng;
	VECTOR3 pos, dir, rot;
	double soft_dock_distance;
	UINT anim_idx;
	void SetSoftDockParams(def_idx d_idx);
	void PreStep();
	OBJHANDLE hvessel;
	DOCKHANDLE h_dock,my_dh;
	bool Active;
	bool SoftDockActive() { return Active; }
	void EnableSoftDock(bool enable);
	void StationKeeping();
	void ConsumeSoftDockKey(DWORD key, bool down, char *kstate);
	bool SDSystemEnabled;
	bool IsSDSysEnabled() { return SDSystemEnabled; }
	void SetSDSystemEnabled(bool set) { SDSystemEnabled = set; }
	bool ChooseDock(def_idx &d_idx,OBJHANDLE &hvessel);
	NOTEHANDLE note_sd;
	bool IsOBJinVector(OBJHANDLE h_obj, vector<OBJHANDLE>vh);
};
