#pragma once

enum AnimCycleType { GOANDSTOP, RESTART, REVERSE, MANUAL, AUTOMATIC };

struct ANIM_DEF {
	anim_idx idx;
	string name;
	double duration;
	double speed;
	AnimCycleType CycleType;
	DWORD Key;
	bool running;
	bool backward;
	bool Valid;
	ANIM_DEF() {
		idx = (UINT)-1;
		name.clear();
		duration = 10;
		speed = 0.1;
		CycleType = AnimCycleType::GOANDSTOP;
		Key = OAPI_KEY_K;
		Valid = true;
		running = false;
		backward = false;
	}
};

struct ANIMCOMP_DEF {
	string name;
	ANIMATIONCOMPONENT_HANDLE ach;
	anim_idx a_idx;
	VECTOR3 *Tip;
	VECTOR3 oldTip[3];
	bool ArmTip;
	def_idx TipAttIdx;
	bool Valid;
	VECTOR3 ref;
	VECTOR3 axis;
	ANIMCOMP_DEF() {
		name.clear();
		ach = NULL;
		a_idx = (UINT)-1;
		ArmTip = false;
		TipAttIdx = (UINT)-1;
		Tip = NULL;
		oldTip[0] = _V(0, 0, 0);
		oldTip[1] = _V(0,0,1);
		oldTip[2] = _V(0,1,0);
		ref = _V(0, 0, 0);
		axis = _V(0, 0, 1);
		Valid = true;
	}
};

class AnimationManager {
public:
	AnimationManager(VesselBuilder1* _VB1);
	~AnimationManager();
	VesselBuilder1 *VB1;
	vector<ANIM_DEF>anim_defs;
	UINT AddAnimDef();
	UINT AddAnimDef(string name, double duration, AnimCycleType Cycle, DWORD Key,double defstate);
	void DeleteAnimDef(anim_idx a_idx);
	bool IsAnimValid(anim_idx a_idx);
	void SetAnimDefState(anim_idx a_idx, double newdefstate);
	double GetAnimDefState(anim_idx a_idx);
	void SetAnimName(anim_idx a_idx, string newname);
	string GetAnimName(anim_idx a_idx);
	void SetAnimCycle(anim_idx a_idx, AnimCycleType Cycle);
	AnimCycleType GetAnimCycle(anim_idx a_idx);
	void SetAnimDuration(anim_idx a_idx, double newduration);
	double GetAnimDuration(anim_idx a_idx);
	double GetAnimSpeed(anim_idx a_idx);
	void SetAnimKey(anim_idx a_idx, DWORD newKey);
	DWORD GetAnimKey(anim_idx a_idx);
	void SetAnimParams(anim_idx a_idx, double duration, AnimCycleType Cycle, DWORD Key);
	void GetAnimParams(anim_idx a_idx, double &duration, AnimCycleType &Cycle, DWORD &Key);
	void SetAnimationState(anim_idx a_idx, double newstate);
	double GetAnimationState(anim_idx a_idx);

	vector<ANIMCOMP_DEF>animcomp_defs;
	UINT AddAnimCompDef(anim_idx a_idx, MGROUP_TRANSFORM::TYPE type);
	UINT AddAnimCompDef(anim_idx a_idx, string name, double state0, double state1, int mesh, int ngrps, vector<UINT>grps, int parent, int type, VECTOR3 ref, VECTOR3 axis, VECTOR3 scale, VECTOR3 shift, double angle);
	void DeleteAnimCompDef(def_idx d_idx);
	bool IsAnimCompDefValid(def_idx d_idx);
	void SetAnimCompDefName(def_idx d_idx, string newname);
	string GetAnimCompDefName(def_idx d_idx);
	void SetAnimCompDefState0(def_idx d_idx, double newState0);
	double GetAnimCompDefState0(def_idx d_idx);
	void SetAnimCompDefState1(def_idx d_idx, double newState1);
	double GetAnimCompDefState1(def_idx d_idx);
	void SetAnimCompDefRef(def_idx d_idx, VECTOR3 newRef);
	VECTOR3 GetAnimCompDefRef(def_idx d_idx);
	void SetAnimCompDefScale(def_idx d_idx, VECTOR3 newScale);
	VECTOR3 GetAnimCompDefScale(def_idx d_idx);
	void SetAnimCompDefShift(def_idx d_idx, VECTOR3 newShift);
	VECTOR3 GetAnimCompDefShift(def_idx d_idx);
	void SetAnimCompDefAxis(def_idx d_idx, VECTOR3 newAxis);
	VECTOR3 GetAnimCompDefAxis(def_idx d_idx);
	void SetAnimCompDefAngle(def_idx d_idx, double newAngle);
	double GetAnimCompDefAngle(def_idx d_idx);
	void SetAnimCompDefMesh(def_idx d_idx, msh_idx m_idx);
	msh_idx GetAnimCompDefMesh(def_idx d_idx);
	void AddAnimCompDefGroups(def_idx d_idx, vector<UINT>addgroups);
	vector<UINT>GetAnimCompDefGroups(def_idx d_idx);
	void RemoveAnimCompDefGroups(def_idx d_idx, vector<UINT>remgroups);
	int GetAnimCompDefNGroups(def_idx d_idx);
	void SetAnimCompDefParent(def_idx d_idx, ANIMATIONCOMPONENT_HANDLE parent_ach);
	ANIMATIONCOMPONENT_HANDLE GetAnimCompDefParent(def_idx d_idx);
	UINT GetAnimDefsCount();
	UINT GetAnimCompDefsCount();
	ANIMATIONCOMPONENT_HANDLE GetAnimCompDefACH(int d_idx);
	UINT GetAnimCompDefIdx(ANIMATIONCOMPONENT_HANDLE ach);
	void ResetAnimCompDefGroups(def_idx d_idx);
	UINT GetAnimNComps(anim_idx a_idx);
	ANIMATIONCOMPONENT_HANDLE GetAnimComp(anim_idx a_idx, UINT comp_idx);
	MGROUP_TRANSFORM::TYPE GetAnimCompDefType(def_idx d_idx);
	UINT GetAnimCompDefSeqIdx(def_idx d_idx);
	int GetParentCompDefIdx(ANIMATIONCOMPONENT_HANDLE parent_ach);
	void InvalidateComponent(def_idx d_idx);


	bool IsAnimRunning(anim_idx a_idx);
	bool IsAnimBackward(anim_idx a_idx);
	void StartAnimation(anim_idx a_idx);
	void StartAnimationForward(anim_idx a_idx);
	void StartAnimationBackward(anim_idx a_idx);
	void StopAnimation(anim_idx a_idx);
	void SetAnimationBackward(anim_idx a_idx, bool backward);
	void ResetAnimation(anim_idx a_idx);

	bool ManualArmMoving;
	void StartManualArm();
	void StopManualArm();
	int CurrentManualAnim;
	bool ManualArmActive();

	bool IsAnimCompArmTip(def_idx d_idx);
	void SetIsAnimCompArmTip(def_idx d_idx, bool set);
	void SetAnimCompDefAttTip(def_idx d_idx, def_idx AttIdx);
	void SetAnimCompDefArmTip(def_idx d_idx, def_idx AttIdx);
	void SetAnimCompDefTips(def_idx d_idx, VECTOR3 v1, VECTOR3 v2, VECTOR3 v3);
	def_idx GetAnimCompDefAttTip(def_idx d_idx);

	void AnimationPreStep(double simt, double simdt, double mjd);
	void ConsumeAnimBufferedKey(DWORD key, bool down, char *kstate);
	void ConsumeAnimDirectKey(char *kstate);

	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	int AnimationRunStatus(anim_idx a_idx);

	void Clear();
};