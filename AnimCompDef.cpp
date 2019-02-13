#include "StationBuilder1.h"
#include "DialogControl.h"
#include "AnimCompDef.h"



AnimCompDef::AnimCompDef() {
	Reset();
}
AnimCompDef::~AnimCompDef() {
	if (pgrps) {
		delete[] pgrps;
	}
	if (mgt) {
		delete mgt;
	}
	return;
}
void AnimCompDef::Reset() {
	state0 = 0;
	state1 = 1;
	name.clear();
	prefix.clear();
	fullname.clear();
	grps.clear();
	pgrps = NULL;
	ref = _V(0, 0, 0);
	shift = _V(0, 0, 0);
	scale = _V(1, 1, 1);
	axis = _V(0, 0, 1);
	angle = 0;
	ach = NULL;
	parent_ach = NULL;
//	parent_comp_idx = (UINT)-1;
	anim_idx = (UINT)-1;
//	comp_idx = (UINT)-1;
//	seq_idx = (UINT)-1;
	if (mgt) {
		delete mgt;
	}
	mgt = NULL;
	ngrps = -1;
	mesh = 0;
	valid = true;
	ParentACD = NULL;
	ChildrenACD.clear();
	return;
}
void AnimCompDef::Init(MGROUP_TRANSFORM::TYPE type) {
	state0 = 0;
	state1 = 1;
	name.clear();
	prefix.clear();
	fullname.clear();
	grps.clear();
	pgrps = NULL;
	ref = _V(0, 0, 0);
	shift = _V(0, 0, 0);
	scale = _V(1, 1, 1);
	axis = _V(0, 0, 1);
	angle = 0;
	ach = NULL;
	parent_ach = NULL;
//	parent_comp_idx = (UINT)-1;
	anim_idx = (UINT)-1;
//	comp_idx = (UINT)-1;
//	seq_idx = (UINT)-1;
	mgt = NULL;
	ngrps = -1;
	mesh = 0;
	ParentACD = NULL;
	switch (type) {
	case MGROUP_TRANSFORM::TRANSLATE:
	{
		mgt = new MGROUP_TRANSLATE(mesh, NULL, 0, shift);
		break;
	}
	case MGROUP_TRANSFORM::ROTATE:
	{
		mgt = new MGROUP_ROTATE(mesh, NULL, 0, ref, axis, angle);
		break;
	}
	case MGROUP_TRANSFORM::SCALE:
	{
		mgt = new MGROUP_SCALE(mesh, NULL, 0, ref, scale);
		break;
	}
	
	}
	return;
}
void AnimCompDef::SetState0(double newState0) {
	state0 = newState0;
	((ANIMATIONCOMP*)ach)->state0 = newState0;
	return;
}
void AnimCompDef::SetState1(double newState1) {
	state1 = newState1;
	((ANIMATIONCOMP*)ach)->state1 = newState1;
	return;
}
double AnimCompDef::GetState0() { return state0; }
double AnimCompDef::GetState1() { return state1; }
void AnimCompDef::GetStates(double &_state0, double &_state1) { _state0 = state0; _state1 = state1; return; }


void AnimCompDef::SetRef(VECTOR3 newRef) {
	ref = newRef;
	if (mgt->Type() == MGROUP_TRANSFORM::ROTATE) {
		((MGROUP_ROTATE*)mgt)->ref = newRef;
	}
	else if (mgt->Type() == MGROUP_TRANSFORM::SCALE) {
		((MGROUP_SCALE*)mgt)->ref = newRef;
	}
	return;
}
VECTOR3 AnimCompDef::GetRef() { return ref; }

void AnimCompDef::SetAxis(VECTOR3 newAxis) {
	axis = newAxis;
	((MGROUP_ROTATE*)mgt)->axis = newAxis;
	return;
}
VECTOR3 AnimCompDef::GetAxis() {
	return axis;
}
void AnimCompDef::SetAngle(double newAngle) {
	angle = newAngle;
	((MGROUP_ROTATE*)mgt)->angle = newAngle;
}
double AnimCompDef::GetAngle() { return angle; }

void AnimCompDef::SetName(string newname) {
	name = newname;
	fullname = prefix + newname;
	return;
}
string AnimCompDef::GetName() { return name; }
string AnimCompDef::GetFullName() { return fullname; }

void AnimCompDef::SetPrefix(string newprefix) {
	prefix = newprefix;
	fullname = newprefix + name;
	return;
}
string AnimCompDef::GetPrefix() { return prefix; }

void AnimCompDef::SetMesh(UINT msh_idx) {
	mesh = msh_idx;
	switch (mgt->Type()) {
	case MGROUP_TRANSFORM::ROTATE:
	{
		((MGROUP_ROTATE*)mgt)->mesh = msh_idx;
		break;
	}
	case MGROUP_TRANSFORM::TRANSLATE:
	{
		((MGROUP_TRANSLATE*)mgt)->mesh = msh_idx;
		break;
	}
	case MGROUP_TRANSFORM::SCALE:
	{
		((MGROUP_SCALE*)mgt)->mesh = msh_idx;
		break;
	}
	}
	return;
}
UINT AnimCompDef::GetMesh() { return mesh; }

void AnimCompDef::AddGroups(vector<UINT> addgrps) {
	for (UINT i = 0; i < addgrps.size(); i++) {
		grps.push_back(addgrps[i]);
	}
	ngrps = grps.size();
	if (pgrps) {
		delete[] pgrps;
	}
	pgrps = new UINT[ngrps];
	for (UINT i = 0; i < grps.size(); i++) {
		pgrps[i] = grps[i];
	}
	((ANIMATIONCOMP*)ach)->trans->grp = pgrps;
	((ANIMATIONCOMP*)ach)->trans->ngrp = ngrps;
	
	
	return;
}

vector<UINT> AnimCompDef::GetGroups() { return grps; }
int AnimCompDef::GetNgrps() { return ngrps; }

VECTOR3 AnimCompDef::GetShift() { return shift; }
VECTOR3 AnimCompDef::GetScale() { return scale; }

void AnimCompDef::GetRotParams(VECTOR3 &_ref, VECTOR3 &_axis, double &_angle) {
	_ref = ref;
	_axis = axis;
	_angle = angle;
	return;
}
void AnimCompDef::GetScaleParams(VECTOR3 &_ref, VECTOR3 &_scale) {
	_ref = ref;
	_scale = scale;
	return;
}
void AnimCompDef::AssignToAnimation(UINT _anim_idx,VESSEL *v) {
	ach = v->AddAnimationComponent(_anim_idx, state0, state1, mgt, parent_ach);
	anim_idx = _anim_idx;
	return;
}
void AnimCompDef::SetParent(ANIMATIONCOMPONENT_HANDLE ach) {
	if (ach == NULL) { return; }
	((ANIMATIONCOMP*)ach)->parent = (ANIMATIONCOMP*)ach;
	parent_ach = ach;
	return;
}

ANIMATIONCOMPONENT_HANDLE AnimCompDef::GetParentACH() {
	return parent_ach;
}
void AnimCompDef::SetScale(VECTOR3 newscale) {
	scale = newscale;
	((MGROUP_SCALE*)mgt)->scale = newscale;
	return;
}
void AnimCompDef::SetShift(VECTOR3 newshift) {
	shift = newshift;
	((MGROUP_TRANSLATE*)mgt)->shift = newshift;
}

MGROUP_TRANSFORM::TYPE AnimCompDef::GetType() {
	return((ANIMATIONCOMP*)ach)->trans->Type();
}
ANIMATIONCOMPONENT_HANDLE AnimCompDef::GetACH() {
	return ach;
}
void AnimCompDef::RemoveGroups(vector<UINT>remgrps) {
	return;
}
bool AnimCompDef::IsValid() {
	return valid;
}
void AnimCompDef::Validate() {//don't use
	valid = true;
	return;
}
void AnimCompDef::Invalidate() {
	for (UINT i = 0; i < ChildrenACD.size(); i++) {
		if (ChildrenACD[i]) {
			ChildrenACD[i]->Invalidate();
		}
	}
	Reset();
	valid = false;
}