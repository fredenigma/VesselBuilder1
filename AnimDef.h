#pragma once

enum AnimCycleType { GOANDSTOP, RESTART, REVERSE, MANUAL };

/*struct ANIM_DEF {
	UINT anim_idx;
	UINT seq_idx;
	double defstate;
	double state;
	bool running;
	bool backward;
	string name;
	string prefix;
	string fullname;
	double duration;
	double speed;
	DWORD key;
	AnimCycleType CycleType;
	//	vector<ANIM_COMPDEF> animcomps;
	ANIM_DEF() {
		anim_idx = (UINT)-1;
		seq_idx = (UINT)-1;
		defstate = 0;
		state = 0;
		running = false;
		backward = false;
		name.clear();
		prefix.clear();
		fullname.clear();
		duration = 10;
		speed = 0.1;
		key = OAPI_KEY_K;
		CycleType = GOANDSTOP;
		//		animcomps.clear();
	}
};*/

class AnimDef {
public:
	AnimDef();
	~AnimDef();
	anim_idx _anim_idx;
	UINT _seq_idx;
	double defstate;
	double state;
	bool running;
	bool backward;
	string name;
	string prefix;
	string fullname;
	double duration;
	double speed;
	DWORD key;
	AnimCycleType CycleType;
	void SetAnimIdx(anim_idx __anim_idx);
	anim_idx GetAnimIdx();
	double GetDefState();
	void SetSeqIdx(UINT __seq_idx);
	UINT GetSeqIdx();
	void SetPrefix(string prefix);
	string GetPrefix();
	void SetName(string name);
	string GetName();
	string GetFullName();
	void SetCycleType(AnimCycleType CycleType);
	AnimCycleType GetCycleType();
	bool valid;
	void ValidAnimDef();
	void InvalidAnimDef();
	bool IsValid();
	void Reset();
	void SetDefState(double _defstate);
	void SetDuration(double duration);
	double GetDuration();
	double GetSpeed();
	bool IsRunning();
	bool IsBackward();
	void Start();
	void StartForward();
	void Stop();
	void StartBackward();
	void SetBackward(bool bw);
	DWORD GetKey();
	void SetState(double state);
	double GetState();
	vector<AnimCompDef*>Comps;
	void SetKey(DWORD key);
	UINT GetNComps();
};

