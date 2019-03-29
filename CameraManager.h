#pragma once
struct CAM_DEF {
	string name;
	VECTOR3 pos;
	VECTOR3 dir;
	double tilt;
	CAM_DEF() {
		name.clear();
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 1);
		tilt = 0;
	}
};
class CameraManager {
public:
	CameraManager(VesselBuilder1 *_VB1);
	~CameraManager();
	VesselBuilder1 *VB1;
	VBVector<CAM_DEF>cam_defs;
	UINT AddCamDef();
	UINT AddCamDef(string name, VECTOR3 pos, VECTOR3 dir, double tilt);
	void SetCamName(def_idx d_idx, string newname);
	string GetCamName(def_idx d_idx);
	void SetCamPos(def_idx d_idx, VECTOR3 newpos);
	VECTOR3 GetCamPos(def_idx d_idx);
	void SetCamDir(def_idx d_idx, VECTOR3 newdir);
	VECTOR3 GetCamDir(def_idx d_idx);
	void SetCamTilt(def_idx d_idx, double newtilt);
	double GetCamTilt(def_idx d_idx);
	void SetCamParams(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, double tilt);
	void GetCamParams(def_idx d_idx, VECTOR3 &pos, VECTOR3 &dir, double &tilt);
	void SetCurrentCamera(def_idx d_idx);
	void SetNextCamera();
	void SetPreviousCamera();
	int CurrentCamera;
	int GetCurrentCamera();
	UINT GetCamCount();
	void RemoveCamDef(def_idx d_idx);
	int ConsumeCameraBufferedKey(DWORD key, bool down, char *kstate);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void Clear();
};
