#include "StationBuilder1.h"
#include "DialogControl.h"
#include "AnimCompDef.h"



AnimCompDef::AnimCompDef() {
	state0 = 0;
	state1 = 1;
	name.clear();
	//	prefix.clear();
	//	fullname.clear();
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
	valid = true;
	ParentACD = NULL;
	ChildrenACD.clear();
	ArmTip = false;
	TipAttIdx = (UINT)-1;
	Tip[0] = _V(0, 0, 0);
	Tip[1] = _V(0, 0, 1);
	Tip[2] = _V(0, 1, 0);
	oldTip[0] = Tip[0];
	oldTip[1] = Tip[1];
	oldTip[2] = Tip[2];
}
AnimCompDef::~AnimCompDef() {
	if (pgrps) {
		delete[] pgrps;
	}
	if (mgt) {
//		delete mgt;
	}
	return;
}
void AnimCompDef::Reset() {
	state0 = 0;
	state1 = 1;
	name.clear();
	grps.clear();
	pgrps = NULL;
	ref = _V(0, 0, 0);
	shift = _V(0, 0, 0);
	scale = _V(1, 1, 1);
	axis = _V(0, 0, 1);
	angle = 0;
	ach = NULL;
	parent_ach = NULL;
	anim_idx = (UINT)-1;
	mgt = NULL;
	ngrps = -1;
	mesh = 0;
	valid = true;
	ParentACD = NULL;
	ChildrenACD.clear();
	ArmTip = false;
	TipAttIdx = (UINT)-1;
	Tip[0] = _V(0, 0, 0);
	Tip[1] = _V(0, 0, 1);
	Tip[2] = _V(0, 1, 0);
	oldTip[0] = Tip[0];
	oldTip[1] = Tip[1];
	oldTip[2] = Tip[2];
	return;
}
void AnimCompDef::Init(MGROUP_TRANSFORM::TYPE type) {
	state0 = 0;
	state1 = 1;
	name.clear();
	grps.clear();
	pgrps = NULL;
	ref = _V(0, 0, 0);
	shift = _V(0, 0, 0);
	scale = _V(1, 1, 1);
	axis = _V(0, 0, 1);
	angle = 0;
	ach = NULL;
	parent_ach = NULL;
	anim_idx = (UINT)-1;
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
	//fullname = prefix + newname;
	return;
}
string AnimCompDef::GetName() { return name; }
//string AnimCompDef::GetFullName() { return fullname; }

/*void AnimCompDef::SetPrefix(string newprefix) {
	prefix = newprefix;
	fullname = newprefix + name;
	return;
}*/
//string AnimCompDef::GetPrefix() { return prefix; }

void AnimCompDef::SetMesh(UINT msh_idx) {
	mesh = msh_idx;
	((ANIMATIONCOMP*)ach)->trans->mesh = msh_idx;
	/*switch (mgt->Type()) {
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
	}*/
	return;
}
UINT AnimCompDef::GetMesh() { return mesh; }
void AnimCompDef::ResetGroups() {
	grps.clear();
	if (pgrps) {
		delete[] pgrps;
	}
	pgrps = NULL;
	ngrps = 0;
	return;
}
void AnimCompDef::RemakeGroups() {
	ngrps = grps.size();
	if (ngrps > 0) {
		if (pgrps) {
			delete[] pgrps;
		}
		pgrps = new UINT[ngrps];
		for (UINT i = 0; i < grps.size(); i++) {
			pgrps[i] = grps[i];
		}
	}
	else {
		if (pgrps) {
			delete[] pgrps;
		}
		pgrps = NULL;
	}
	
	((ANIMATIONCOMP*)ach)->trans->grp = pgrps;
	((ANIMATIONCOMP*)ach)->trans->ngrp = ngrps;

	return;
}
void AnimCompDef::AddGroups(vector<UINT> addgrps) {
	for (UINT i = 0; i < addgrps.size(); i++) {
		grps.push_back(addgrps[i]);
	}
	RemakeGroups();
	
	return;
}
void AnimCompDef::RemoveGroups(vector<UINT>remgrps) {
	for (UINT i = 0; i < remgrps.size(); i++) {
		vector<UINT>::iterator it = grps.begin();
		while (it != grps.end()) {
			if ((*it) == remgrps[i]) {
				it = grps.erase(it);
			}
			else {
				++it;
			}
		}
	}
	RemakeGroups();
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
	ach = v->AddAnimationComponent(_anim_idx, state0, state1, mgt, NULL);// parent_ach);
	anim_idx = _anim_idx;
	return;
}
//void AnimCompDef::SetParent(ANIMATIONCOMPONENT_HANDLE newParent_ach) {
void AnimCompDef::SetParent(AnimCompDef* newparent_acd) {
	ANIMATIONCOMP* oldparent_ach = ((ANIMATIONCOMP*)ach)->parent;
	if (oldparent_ach != NULL) {
		int oldparent_nchildren = oldparent_ach->nchildren;
		UINT oldme_index = 0;
		for (UINT i = 0; i < oldparent_nchildren; i++) {
			if (oldparent_ach->children[i] == (ANIMATIONCOMP*)ach) {
				oldme_index = i;
			}
		}
		ANIMATIONCOMP** oldparent_childrennew = new ANIMATIONCOMP*[oldparent_nchildren - 1];
		for (UINT j = 0; j < oldparent_nchildren - 1; j++) {
			if (j < oldme_index) {
				oldparent_childrennew[j] = oldparent_ach->children[j];
			}
			else {
				oldparent_childrennew[j] = oldparent_ach->children[j-1];
			}	
		}
		oldparent_ach->nchildren = oldparent_nchildren - 1;
		if (oldparent_ach->children) {
			delete[] oldparent_ach->children;
		}
		oldparent_ach->children = oldparent_childrennew;

	}
	//((ANIMATIONCOMP*)ach)->parent = (ANIMATIONCOMP*)newParent_ach;
	ANIMATIONCOMP* newParent_ach;
	if (newparent_acd == NULL) {
		((ANIMATIONCOMP*)ach)->parent = NULL;
		newParent_ach = NULL;
	}
	else {
		newParent_ach = (ANIMATIONCOMP*)newparent_acd->ach;
		((ANIMATIONCOMP*)ach)->parent = (ANIMATIONCOMP*)newparent_acd->ach;
		if (newParent_ach != NULL) {
			int newparent_nchildren = ((ANIMATIONCOMP*)newParent_ach)->nchildren;
			ANIMATIONCOMP** newparent_children = new ANIMATIONCOMP*[newparent_nchildren + 1];
			for (UINT k = 0; k < newparent_nchildren; k++) {
				newparent_children[k] = ((ANIMATIONCOMP*)newParent_ach)->children[k];
			}
			newparent_children[newparent_nchildren] = (ANIMATIONCOMP*)ach;
			if (((ANIMATIONCOMP*)newParent_ach)->children) {
				delete[]((ANIMATIONCOMP*)newParent_ach)->children;
			}
			((ANIMATIONCOMP*)newParent_ach)->children = newparent_children;
			((ANIMATIONCOMP*)newParent_ach)->nchildren = newparent_nchildren + 1;
		}
	}
	
	parent_ach = newParent_ach;
	
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

bool AnimCompDef::IsArmTip() { return ArmTip; }
void AnimCompDef::GetTips(VECTOR3 &v1, VECTOR3 &v2, VECTOR3 &v3) {
	v1 = Tip[0];
	v2 = Tip[1];
	v3 = Tip[2];
	return;
}

void AnimCompDef::SetAttTip(def_idx _AttIdx) {
	TipAttIdx = _AttIdx;
	//We'll see if we come up with an idea
}
def_idx AnimCompDef::GetAttTip() {
	return TipAttIdx;
}
void AnimCompDef::SetTips(VECTOR3 v1, VECTOR3 v2, VECTOR3 v3) {
	Tip[0] = v1;
	Tip[1] = v2;
	Tip[2] = v3;
	oldTip[0] = Tip[0];
	oldTip[1] = Tip[1];
	oldTip[2] = Tip[2];
	return;
}
void AnimCompDef::SetIsArmTip(bool set) {
	ArmTip = set;
}

