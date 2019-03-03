#pragma once
struct VC_POS {
	string name;
	VECTOR3 ofs;
	VECTOR3 dir;
	VC_POS() {
		name.clear();
		ofs = _V(0, 0, 0);
		dir = _V(0, 0, 1);
	}
};
struct VC_MFD {
	VCMFDSPEC mfd_spec;
	int pwr_btns_area_id[3];
	VECTOR3 pwr_btns[3];
	int cols_btn_area_id[12];
	VECTOR3 col_btns[12];
	bool wPwrBtns;
	bool wColsBtns;
	int mfd;
	VC_MFD() {
		mfd_spec = { 0 };
		wPwrBtns = false;
		wColsBtns = false;
		for (UINT i = 0; i < 3; i++) {
			pwr_btns_area_id[i] = 1000 + i;
			pwr_btns[i] = _V(0, 0, 0);
		}
		for (UINT i = 0; i < 12; i++) {
			col_btns[i] = _V(0, 0, 0);
			cols_btn_area_id[i] = 1500 + i;
		}
		mfd = MFD_LEFT;
	}
};
class VCManager {
public:
	VCManager(VesselBuilder1 *_VB1);
	~VCManager();
	VesselBuilder1 *VB1;
	vector<VC_POS>vc_positions;
	vector<VC_MFD>vc_mfds;
	VCHUDSPEC vc_hud;
	int id_counter;
	bool LoadVC(int id);
	bool VCMouseEvent(int id, int event, VECTOR3 &p);
	bool wHUD;
	UINT AddPosition();
	UINT AddPosition(string name, VECTOR3 ofs, VECTOR3 dir);
	void SetPositionName(def_idx d_idx, string newname);
	string GetPositionName(def_idx d_idx);
	void SetPositionPos(def_idx d_idx, VECTOR3 newPos);
	void SetPositionDir(def_idx d_idx, VECTOR3 newDir);
	VECTOR3 GetPositionPos(def_idx d_idx);
	VECTOR3 GetPositionDir(def_idx d_idx);
	void GetPositionParams(def_idx d_idx, VECTOR3 &ofs, VECTOR3 &dir);
	void DeletePosition(def_idx d_idx);
	UINT GetPositionCount();
	UINT AddMFD();
	UINT AddMFD(int mesh, int group, bool wPwrBtns, VECTOR3 pwr_btn, VECTOR3 mnu_btn, bool wColsBtns, VECTOR3 TopLeftBtn, VECTOR3 BottomLeftBtn, VECTOR3 TopRightBtn, VECTOR3 BottomRightBtn);
	void DefineMFDColumnButtons(def_idx d_idx, int col, VECTOR3 first, VECTOR3 last);
	void DefineMFDPwrButtons(def_idx d_idx, VECTOR3 first, VECTOR3 last);
	void SetMFDHasPwrBtns(def_idx d_idx, bool set);
	void SetMFDHasColumnBtns(def_idx d_idx, bool set);
	void SetMFDMesh(def_idx d_idx, int newmesh);
	void SetMFDGroup(def_idx d_idx, int newgroup);
	bool GetMFDHasPwrBtns(def_idx d_idx);
	bool GetMFDHasColBtns(def_idx d_idx);
	DWORD GetMFDMesh(def_idx d_idx);
	DWORD GetMFDGroup(def_idx d_idx);
	void GetMFDPwrVectors(def_idx d_idx,VECTOR3 &pwr_btn, VECTOR3 &mnu_btn);
	void GetMFDColVectors(def_idx d_idx, VECTOR3 &top_left, VECTOR3 &bottom_left, VECTOR3 &top_right, VECTOR3 &bottom_right);
	UINT GetMFDCount();
	void EnableHUD(bool enable);
	bool IsHUDEnabled();
	void SetHUDMesh(DWORD nmesh);
	void SetHUDGroup(DWORD ngroup);
	void SetHUDCenter(VECTOR3 hudcnt);
	void SetHUDSize(double size);
	void GetHUDParams(DWORD &nmesh,DWORD &ngroup,VECTOR3 &hudcnt,double &size);
	void SetHUDParams(int mesh, int group, VECTOR3 hudcnt, double size);
	
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void Clear();
};
