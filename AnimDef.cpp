#include "StationBuilder1.h"
#include "DialogControl.h"
#include "AnimationManager.h"
#include "AnimDef.h"


AnimDef::AnimDef() {
	Reset();
}

AnimDef::~AnimDef() {}

void AnimDef::Reset() {
	_anim_idx = (UINT)-1;
	_seq_idx = (UINT)-1;
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
	valid = true;
	Comps.clear();
}
void AnimDef::SetAnimIdx(anim_idx __anim_idx) {
	_anim_idx = __anim_idx;
	return;
}
anim_idx AnimDef::GetAnimIdx() {
	return _anim_idx;
}
double AnimDef::GetDefState() {
	return defstate;
}
void AnimDef::SetSeqIdx(UINT __seq_idx) {
	_seq_idx = __seq_idx;
}
UINT AnimDef::GetSeqIdx() {
	return _seq_idx;
}
void AnimDef::SetPrefix(string _prefix) {
	prefix = _prefix;
	fullname = prefix + name;
	return;
}
string AnimDef::GetPrefix() {
	return prefix;
}
void AnimDef::SetName(string _name) {
	name = _name;
	fullname = prefix + name;
}
string AnimDef::GetName() {
	return name;
}
string AnimDef::GetFullName() {
	return fullname;
}
void AnimDef::SetCycleType(AnimCycleType _CycleType) {
	CycleType = _CycleType;
}
AnimCycleType AnimDef::GetCycleType() {
	return CycleType;
}
void AnimDef::ValidAnimDef() {
	valid = true;
	return;
}
void AnimDef::InvalidAnimDef() {
	Reset();
	valid = false;
	return;
}
bool AnimDef::IsValid() {
	return valid;
}
void AnimDef::SetDefState(double _defstate) {
	defstate = _defstate;
	return;
}
void AnimDef::SetDuration(double _duration) {
	duration = _duration;
	speed = 1 / duration;
	return;
}
double AnimDef::GetDuration() {
	return duration;
}
double AnimDef::GetSpeed() {
	return speed;
}
bool AnimDef::IsRunning() {
	return running;
}
bool AnimDef::IsBackward() {
	return backward;
}
void AnimDef::Start() {
	running = true;
	return;
}
void AnimDef::Stop() {
	running = false;
	return;
}
void AnimDef::StartForward() {
	running = true;
	backward = false;
}
void AnimDef::StartBackward() {
	backward = true;
	running = true;
}
void AnimDef::SetBackward(bool bw) {
	backward = bw;
}
DWORD AnimDef::GetKey() {
	return key;
}
void AnimDef::SetState(double _state) {
	state = _state;
	return;
}
double AnimDef::GetState() {
	return state;
}
void AnimDef::SetKey(DWORD _key) {
	key = _key;
	return;
}
UINT AnimDef::GetNComps() {
	return Comps.size();
}