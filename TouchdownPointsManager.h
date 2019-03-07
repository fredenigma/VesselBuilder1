#pragma once

class TouchdownPointsManager {
public:
	TouchdownPointsManager(VesselBuilder1 *_VB1);
	~TouchdownPointsManager();
	VesselBuilder1 *VB1;
	vector<TOUCHDOWNVTX> set1;
	vector<TOUCHDOWNVTX> set2;
	UINT change_anim_idx;
	double *state_chk;
	UINT GetSetPointsCount(UINT set);
	vector<TOUCHDOWNVTX> GetSet(UINT set);
	VECTOR3 GetPointPos(UINT set, UINT idx);
	double GetPointStiffnes(UINT set, UINT idx);
	double GetPointDamping(UINT set, UINT idx);
	double GetPointMu(UINT set, UINT idx);
	double GetPointMuLng(UINT set, UINT idx);
	void AddPoint(UINT set, TOUCHDOWNVTX tdvtx);
	void RemovePoint(UINT set, UINT idx);
	void EnableSecondSet(bool enable);
	void SetChangeOverAnimation(anim_idx a_idx);
	UINT GetChangeOverAnimation();
	void ApplySet(UINT set);
	bool IsSecondSetEnabled();
	void ClearSet(UINT set);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	bool Set2Enabled;
	UINT CurrentSet;
	void SetCurrentSet(UINT set);
	UINT GetCurrentSet();
	UINT GetPointsCount(UINT set);
	void TouchDownPointsPreStep(double simt, double simdt, double mjd);
	void Clear();
};