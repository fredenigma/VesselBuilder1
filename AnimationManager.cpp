#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "AnimationManager.h"
#include "AttachmentManager.h"
//#include "TouchdownPointsManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

AnimationManager::AnimationManager(VesselBuilder1* _VB1) {
	VB1 = _VB1;
	anim_defs.clear();
	animcomp_defs.clear();
	ManualArmMoving = false;
	CurrentManualAnim = -1;
	interval = 1;
	return;
}
AnimationManager::~AnimationManager() {
	VB1 = NULL;
	return;
}

UINT AnimationManager::AddAnimDef() {
	char cbuf[256] = { '\0' };
	UINT index = anim_defs.size();
	sprintf(cbuf, "Anim_%i", index);
	string name(cbuf);
	return AddAnimDef(name, 10, AnimCycleType::GOANDSTOP, OAPI_KEY_K, 0);
}
UINT AnimationManager::AddAnimDef(string name, double duration, AnimCycleType Cycle, DWORD Key, double defstate) {
	LogV("Adding Animation:%s duration:%.3f Cycle:%i Key:%i defstate:%.3f", name.c_str(), duration, (int)Cycle, Key, defstate);
	ANIM_DEF ad = ANIM_DEF();
	ad.name = name;
	ad.duration = duration;
	ad.speed = 1 / duration;
	ad.CycleType = Cycle;
	ad.Key = Key;
	ad.idx = VB1->CreateAnimation(defstate);
	*ad.state_ptr = defstate;
	UINT index = anim_defs.size();
	anim_defs.push_back(ad);
	return index;
}
void AnimationManager::DeleteAnimDef(def_idx d_idx) {
	LogV("Deleting Animation %i", d_idx);
	anim_idx a_idx = anim_defs[d_idx].idx;
	ANIMATION *anims;
	UINT nanims = VB1->GetAnimPtr(&anims);
	for (UINT i = 0; i < anims[a_idx].ncomp; i++) {
		def_idx dc_idx = GetAnimCompDefIdx(anims[a_idx].comp[i]);
		InvalidateComponent(dc_idx);
	}
	delete anim_defs[d_idx].state_ptr; //? not so sure it won't trigger issues
	VB1->DelAnimation(a_idx);
	anim_defs[d_idx].Valid = false;
	LogV("Deleted");
	return;
}
void AnimationManager::InvalidateComponent(def_idx d_idx) {
	animcomp_defs[d_idx].Valid = false;
	return;
}
double AnimationManager::GetAnimDefState(def_idx d_idx) {
	anim_idx a_idx = anim_defs[d_idx].idx;
	ANIMATION *anims;
	VB1->GetAnimPtr(&anims);
	return anims[a_idx].defstate;
}
void AnimationManager::SetAnimDefState(def_idx d_idx, double newdefstate) {
	anim_idx a_idx = anim_defs[d_idx].idx;
	ANIMATION *anims;
	VB1->GetAnimPtr(&anims);
	anims[a_idx].state = newdefstate;
	anims[a_idx].defstate = newdefstate;
	*anim_defs[d_idx].state_ptr = newdefstate;
	return;
}
bool AnimationManager::IsAnimValid(def_idx d_idx) {
	return anim_defs[d_idx].Valid;
}
void AnimationManager::SetAnimName(def_idx d_idx, string newname) {
	anim_defs[d_idx].name = newname;
	return;
}
string AnimationManager::GetAnimName(def_idx d_idx) {
	return anim_defs[d_idx].name;
}
void AnimationManager::SetAnimCycle(def_idx d_idx, AnimCycleType Cycle) {
	anim_defs[d_idx].CycleType = Cycle;
	return;
}
AnimCycleType AnimationManager::GetAnimCycle(def_idx d_idx) {
	return anim_defs[d_idx].CycleType;
}
void AnimationManager::SetAnimDuration(def_idx d_idx, double newduration) {
	anim_defs[d_idx].duration = newduration;
	anim_defs[d_idx].speed = 1 / newduration;
	return;
}
double AnimationManager::GetAnimDuration(def_idx d_idx) {
	return anim_defs[d_idx].duration;
}
void AnimationManager::SetAnimKey(def_idx d_idx, DWORD newKey) {
	anim_defs[d_idx].Key = newKey;
	return;
}
DWORD AnimationManager::GetAnimKey(def_idx d_idx) {
	return anim_defs[d_idx].Key;
}
void AnimationManager::SetAnimParams(def_idx d_idx, double duration, AnimCycleType Cycle, DWORD Key) {
	anim_defs[d_idx].duration = duration;
	anim_defs[d_idx].speed = 1 / duration;
	anim_defs[d_idx].CycleType = Cycle;
	anim_defs[d_idx].Key = Key;
	return;
}
void AnimationManager::GetAnimParams(def_idx d_idx, double &duration, AnimCycleType &Cycle, DWORD &Key) {
	duration = anim_defs[d_idx].duration;
	Cycle = anim_defs[d_idx].CycleType;
	Key = anim_defs[d_idx].Key;
	return;
}
void AnimationManager::SetAnimationState(def_idx d_idx, double newstate) {
	anim_idx a_idx = anim_defs[d_idx].idx;
	VB1->SetAnimation(a_idx, newstate);
	*anim_defs[d_idx].state_ptr = newstate;
		
	return;
}
double AnimationManager::GetAnimationState(def_idx d_idx) {
	anim_idx a_idx = anim_defs[d_idx].idx;
	return VB1->GetAnimation(a_idx);
}

UINT AnimationManager::AddAnimCompDef(def_idx d_idx, MGROUP_TRANSFORM::TYPE type) { 
	char cbuf[256] = { '\0' };
	int index = animcomp_defs.size();
	sprintf(cbuf, "AnimComp_%i", index);
	string name(cbuf);
	vector<UINT>empty;
	empty.clear();
	return AddAnimCompDef(d_idx, name, 0, 1, 0, 0, empty, -1, (int)type, _V(0, 0, 0), _V(0, 0, 1), _V(1, 1, 1), _V(0, 0, 0), 0);
}
UINT AnimationManager::AddAnimCompDef(def_idx d_idx, string name, double state0, double state1, int mesh, int ngrps, vector<UINT>grps, int parent, int type, VECTOR3 ref, VECTOR3 axis, VECTOR3 scale, VECTOR3 shift, double angle) {
	LogV("Adding AnimComp: name:%s",name.c_str());
	ANIMCOMP_DEF acd = ANIMCOMP_DEF();
	acd.name = name;
	acd.ref = ref;
	acd.axis = axis;
	MGROUP_TRANSFORM *mgt;
	bool wgrps = false;
	UINT *grp = NULL;
	if ((ngrps > 0)&&(mesh!=LOCALVERTEXLIST)) {
		grp = new UINT[ngrps];
		for (UINT i = 0; i < ngrps; i++) {
			grp[i] = grps[i];
		}
		wgrps = true;
	}
	switch (type) {		//type 1 Rotate 2 translate 3 scale
	case 1:
	{
		mgt = new MGROUP_ROTATE(mesh, grp, ngrps, ref, axis, angle);
				
		break;
	}
	case 2:
	{
		mgt = new MGROUP_TRANSLATE(mesh, grp, ngrps, shift);
		
		break;
	}
	case 3:
	{
		mgt = new MGROUP_SCALE(mesh, grp, ngrps, ref, scale);
		break;
	}
	}
	ANIMATIONCOMPONENT_HANDLE parent_ach = GetAnimCompDefACH(parent);
	anim_idx a_idx = anim_defs[d_idx].idx;
	acd.ach = VB1->AddAnimationComponent(a_idx, state0, state1, mgt, parent_ach);
	acd.a_idx = a_idx;
	UINT index = animcomp_defs.size();
	animcomp_defs.push_back(acd);
	return index;
}
void AnimationManager::DeleteAnimCompDef(def_idx d_idx) {
	LogV("Deleting AnimComp:%i", d_idx);
	VB1->DelAnimationComponent(animcomp_defs[d_idx].a_idx, animcomp_defs[d_idx].ach);
	animcomp_defs[d_idx].Valid = false;
	LogV("Deleted");
	return;
}
bool AnimationManager::IsAnimCompDefValid(def_idx d_idx) {
	return animcomp_defs[d_idx].Valid;
}
void AnimationManager::SetAnimCompDefName(def_idx d_idx, string newname) {
	animcomp_defs[d_idx].name = newname;
	return;
}
string AnimationManager::GetAnimCompDefName(def_idx d_idx) {
	return animcomp_defs[d_idx].name;
}
void AnimationManager::SetAnimCompDefState0(def_idx d_idx, double newState0) {
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->state0 = newState0;
	return;
}
double AnimationManager::GetAnimCompDefState0(def_idx d_idx) {
	return ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->state0;
}
void AnimationManager::SetAnimCompDefState1(def_idx d_idx, double newState1) {
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->state1 = newState1;
	return;
}
double AnimationManager::GetAnimCompDefState1(def_idx d_idx) {
	return ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->state1;
}
void AnimationManager::SetAnimCompDefRef(def_idx d_idx, VECTOR3 newRef) {
	animcomp_defs[d_idx].ref = newRef;
	if(((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->Type()==MGROUP_TRANSFORM::ROTATE){
		((MGROUP_ROTATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->ref = newRef;
	}
	else if (((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->Type() == MGROUP_TRANSFORM::SCALE) {
		((MGROUP_SCALE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->ref = newRef;
	}
	return;
}
VECTOR3 AnimationManager::GetAnimCompDefRef(def_idx d_idx) {
	return animcomp_defs[d_idx].ref;
	/*if (((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->Type() == MGROUP_TRANSFORM::ROTATE) {
		return ((MGROUP_ROTATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->ref;
	}
	else if (((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->Type() == MGROUP_TRANSFORM::SCALE) {
		return ((MGROUP_SCALE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->ref;
	}
	else {
		return _V(0, 0, 0);
	}*/
}
void AnimationManager::SetAnimCompDefScale(def_idx d_idx, VECTOR3 newScale) {
	((MGROUP_SCALE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->scale = newScale;
	return;
}
VECTOR3 AnimationManager::GetAnimCompDefScale(def_idx d_idx) {
	return ((MGROUP_SCALE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->scale;
}
void AnimationManager::SetAnimCompDefShift(def_idx d_idx, VECTOR3 newShift) {
	((MGROUP_TRANSLATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->shift = newShift;
	return;
}
VECTOR3 AnimationManager::GetAnimCompDefShift(def_idx d_idx) {
	return ((MGROUP_TRANSLATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->shift;
}
void AnimationManager::SetAnimCompDefAxis(def_idx d_idx, VECTOR3 newAxis) {
	animcomp_defs[d_idx].axis = newAxis;
	((MGROUP_ROTATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->axis = newAxis;
	return;
}
VECTOR3 AnimationManager::GetAnimCompDefAxis(def_idx d_idx) {
	return animcomp_defs[d_idx].axis;
	//return ((MGROUP_ROTATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->axis;
}
void AnimationManager::SetAnimCompDefAngle(def_idx d_idx, double newAngle) {
	((MGROUP_ROTATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->angle = newAngle;
	return;
}
double AnimationManager::GetAnimCompDefAngle(def_idx d_idx) {
	return ((MGROUP_ROTATE*)((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans)->angle;
}
void AnimationManager::SetAnimCompDefMesh(def_idx d_idx, msh_idx m_idx) {
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->mesh = m_idx;
	return;
}
msh_idx AnimationManager::GetAnimCompDefMesh(def_idx d_idx) {
	return ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->mesh;
}
void AnimationManager::AddAnimCompDefGroups(def_idx d_idx, vector<UINT>addgroups) {
	int oldngrps = ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp;
	int addngrps = addgroups.size();
	int newngrps = oldngrps + addngrps;
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp = newngrps;
	UINT *newgrp = new UINT[newngrps];
	for (UINT i = 0; i < oldngrps; i++) {
		newgrp[i] = ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp[i];
	}
	for (UINT j = oldngrps; j < newngrps; j++) {
		newgrp[j] = addgroups[j - oldngrps];
	}
	if (((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp) {
		delete[]((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp;
	}
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp = newgrp;

	return;
}
void AnimationManager::RemoveAnimCompDefGroups(def_idx d_idx, vector<UINT>remgroups) {
	int size_remgrps = remgroups.size();
	if (size_remgrps <= 0) { return; }
	int size_oldgrps = ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp;
	
	vector<UINT>working_grps;
	working_grps.clear();
	for (UINT i = 0; i < size_oldgrps; i++) {
		working_grps.push_back(((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp[i]);
	}
	for (UINT i = 0; i < working_grps.size(); i++) {
		if (VB1->IsUintInVector(working_grps[i], remgroups)) {
			working_grps[i] = (UINT)-1;
		}
	}

	vector<UINT>::iterator it = working_grps.begin();
	while(it != working_grps.end()) {
		if ((*it) == (UINT)-1) {
			it = working_grps.erase(it);
		}else{
			++it;
		}
	}

	int newsize = size_oldgrps - size_remgrps;
	UINT *grps = new UINT[newsize];
	for (UINT i = 0; i < newsize; i++) {
		grps[i] = working_grps[i];
	}
	if (((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp) {
		delete[]((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp;
	}
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp = grps;
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp = newsize;


	return;
}
vector<UINT> AnimationManager::GetAnimCompDefGroups(def_idx d_idx) {
	vector<UINT> grps;
	grps.clear();
	int ngrps = ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp;
	if (((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->mesh == LOCALVERTEXLIST) {
		return grps;
	}
	for (UINT i = 0; i < ngrps; i++) {
		grps.push_back(((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp[i]);
	}
	return grps;
}
int AnimationManager::GetAnimCompDefNGroups(def_idx d_idx) {
	return ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp;
}

void AnimationManager::ResetAnimCompDefGroups(def_idx d_idx) {
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp = NULL;
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp = 0;
	return;
}

void AnimationManager::SetAnimCompDefParent(def_idx d_idx, ANIMATIONCOMPONENT_HANDLE parent_ach) {
	ANIMATIONCOMP* old_parent = ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->parent;
	if (old_parent != NULL) {
		int old_parent_nchildren = old_parent->nchildren;
		UINT oldme_index = 0;
		for (UINT i = 0; i < old_parent_nchildren; i++) {
			if (old_parent->children[i] == (ANIMATIONCOMP*)animcomp_defs[d_idx].ach) {
				oldme_index = i;
			}
		}
		ANIMATIONCOMP** old_parent_newchildren = new ANIMATIONCOMP*[old_parent_nchildren - 1];
		for (UINT j = 0; j < old_parent_nchildren - 1; j++) {
			if (j < oldme_index) {
				old_parent_newchildren[j] = old_parent->children[j];
			}
			else {
				old_parent_newchildren[j] = old_parent->children[j - 1];
			}
		}
		old_parent->nchildren = old_parent_nchildren - 1;
		//if (old_parent->children) {
		//	for (UINT k = 0; k < old_parent_nchildren; k++) {
		//		old_parent->children[k] = NULL;
		//	}
			//delete[] old_parent->children;
		//}
		old_parent->children = old_parent_newchildren;

	}
	

	ANIMATIONCOMP* newParent = NULL;
	if (parent_ach == NULL) {
		((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->parent = NULL;
	}
	else {
		
		newParent = (ANIMATIONCOMP*)parent_ach;
		((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->parent = newParent;
		UINT new_parent_nchildren = newParent->nchildren;
		ANIMATIONCOMP** newparent_newchildren = new ANIMATIONCOMP*[new_parent_nchildren + 1];
		for (UINT k = 0; k < new_parent_nchildren; k++) {
			newparent_newchildren[k] = newParent->children[k];
			/*for (UINT q = 0; q < animcomp_defs.size(); q++) {
				if (newparent_newchildren[k] == (ANIMATIONCOMP*)animcomp_defs[q].ach) {
					LogV("child k:%i name:%s", k, animcomp_defs[q].name.c_str());
				}
			}*/
		}
		newparent_newchildren[new_parent_nchildren] = (ANIMATIONCOMP*)animcomp_defs[d_idx].ach;
	/*	if (newParent->children) {
			for (UINT z = 0; z < new_parent_nchildren; z++) {
				newParent->children[z] = NULL;
			}
		}*/
		newParent->nchildren = new_parent_nchildren + 1;
		newParent->children = newparent_newchildren;
		

	}
	return;
}
ANIMATIONCOMPONENT_HANDLE AnimationManager::GetAnimCompDefParent(def_idx d_idx) {
	return ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->parent;	
}
UINT AnimationManager::GetAnimDefsCount(){
	return anim_defs.size();
/*	UINT anim_counter = 0;
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i].Valid) {
			anim_counter++;
		}
	}
	return anim_counter;*/
}
UINT AnimationManager::GetAnimCompDefsCount(){
	return animcomp_defs.size();
}

ANIMATIONCOMPONENT_HANDLE AnimationManager::GetAnimCompDefACH(int d_idx) {
	if ((d_idx < 0)||(d_idx >=animcomp_defs.size())) {
		return NULL;
	}
	else {
		return animcomp_defs[d_idx].ach;
	}
}
UINT AnimationManager::GetAnimCompDefIdx(ANIMATIONCOMPONENT_HANDLE ach) {
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		if (ach == GetAnimCompDefACH(i)) {
			return i;
		}
	}
	return (UINT)-1;
}
UINT AnimationManager::GetAnimCompDefSeqIdx(def_idx d_idx) {
	UINT a_counter = 0;
	for (UINT i = 0; i < GetAnimDefsCount(); i++) {
		if (anim_defs[i].idx == animcomp_defs[d_idx].a_idx) {
			return a_counter;
		}
		if (IsAnimValid(i)) {
			a_counter++;
		}
	}
	return (UINT)-1;
}
int AnimationManager::GetParentCompDefIdx(ANIMATIONCOMPONENT_HANDLE parent_ach) {
	if (parent_ach == NULL) { return -1; }
	UINT ac_counter = 0;
	for (UINT i = 0; i < GetAnimCompDefsCount(); i++) {
		if (GetAnimCompDefACH(i) == parent_ach) {
			return ac_counter;
		}
		if (IsAnimCompDefValid(i)) {
			ac_counter++;
		}
	}
	return -1;
}


bool AnimationManager::IsAnimRunning(def_idx d_idx) {
	return anim_defs[d_idx].running;
}
bool AnimationManager::IsAnimBackward(def_idx d_idx) {
	return anim_defs[d_idx].backward;
}
double AnimationManager::GetAnimSpeed(def_idx d_idx) {
	return anim_defs[d_idx].speed;
}
void AnimationManager::StartAnimation(def_idx d_idx) {
	anim_defs[d_idx].running = true;
}
void AnimationManager::StartAnimationForward(def_idx d_idx) {
	anim_defs[d_idx].running = true;
	anim_defs[d_idx].backward = false;
	return;
}
void AnimationManager::StartAnimationBackward(def_idx d_idx) {
	anim_defs[d_idx].running = true;
	anim_defs[d_idx].backward = true;
	return;
}
void AnimationManager::StopAnimation(def_idx d_idx) {
	anim_defs[d_idx].running = false;
}
void AnimationManager::SetAnimationBackward(def_idx d_idx, bool backward) {
	anim_defs[d_idx].backward = backward;
	return;
}
void AnimationManager::ResetAnimation(def_idx d_idx) {
	double defstate = GetAnimDefState(d_idx);
	SetAnimationState(d_idx, defstate);
	return;
}
void AnimationManager::StartManualArm() {
	ManualArmMoving = true;
	if (CurrentManualAnim == -1) {
		for (UINT i = 0; i < anim_defs.size(); i++) {
			if ((GetAnimKey(i)== 0) && (GetAnimCycle(i) == AnimCycleType::MANUAL)) {
				CurrentManualAnim = i;
				return;
			}
		}
		StopManualArm();
	}
	return;
}
void AnimationManager::StopManualArm() {
	ManualArmMoving = false;
	sprintf(oapiDebugString(), "");
	return;
}
bool AnimationManager::ManualArmActive() {
	return ManualArmMoving;
}

bool AnimationManager::IsAnimCompArmTip(def_idx d_idx) {
	return animcomp_defs[d_idx].ArmTip;
}
void AnimationManager::SetIsAnimCompArmTip(def_idx d_idx, bool set) {
	animcomp_defs[d_idx].ArmTip = set;
	return;
}
void AnimationManager::SetAnimCompDefAttTip(def_idx d_idx, def_idx AttIdx) {
	animcomp_defs[d_idx].TipAttIdx = AttIdx;
	return;
}
void AnimationManager::SetAnimCompDefTips(def_idx d_idx, VECTOR3 v1, VECTOR3 v2, VECTOR3 v3) {
	if (animcomp_defs[d_idx].Tip) {
		delete[] animcomp_defs[d_idx].Tip;
	}
	animcomp_defs[d_idx].Tip = new VECTOR3[3];
	animcomp_defs[d_idx].Tip[0] = v1;
	animcomp_defs[d_idx].Tip[1] = v2;
	animcomp_defs[d_idx].Tip[2] = v3;
	animcomp_defs[d_idx].oldTip[0] = v1;
	animcomp_defs[d_idx].oldTip[1] = v2;
	animcomp_defs[d_idx].oldTip[2] = v3;
	return;
}

void AnimationManager::SetAnimCompDefArmTip(def_idx d_idx, def_idx AttIdx) {
	SetIsAnimCompArmTip(d_idx, true);
	SetAnimCompDefAttTip(d_idx, AttIdx);
	SetAnimCompDefMesh(d_idx, LOCALVERTEXLIST);
	ResetAnimCompDefGroups(d_idx);
	VECTOR3 pos, dir, rot;
	VB1->AttMng->GetAttDefPosDirRot(AttIdx, pos, dir, rot);
	SetAnimCompDefTips(d_idx, pos, pos + dir, pos + rot);
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->grp = MAKEGROUPARRAY(animcomp_defs[d_idx].Tip);
	((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->ngrp = 3;
	return;
}
def_idx AnimationManager::GetAnimCompDefAttTip(def_idx d_idx) {
	return animcomp_defs[d_idx].TipAttIdx;
}


int AnimationManager::ConsumeAnimBufferedKey(DWORD key, bool down, char *kstate) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == GetAnimKey(i)) {
			if ((GetAnimCycle(i) == AnimCycleType::MANUAL) || (GetAnimCycle(i) == AnimCycleType::AUTOMATIC)) { continue; }
			if (IsAnimRunning(i)) {
				StopAnimation(i);
			}
			else {
				StartAnimationForward(i);
			}
			return 1;
		}
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == GetAnimKey(i)) {
			if ((GetAnimCycle(i)== AnimCycleType::MANUAL) || (GetAnimCycle(i) == AnimCycleType::AUTOMATIC)) { continue; }
			if (IsAnimRunning(i)) {
				StopAnimation(i);
			}
			else {
				StartAnimationBackward(i);
			}
			return 1;
		}
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_SPACE) {
		if (ManualArmActive()) {
			StopManualArm();
		}
		else {
			StartManualArm();
		}
		return 1;
	}

	if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_LEFT) {
		if (ManualArmActive()) {
			vector<UINT> manualAnimsIdx;
			manualAnimsIdx.clear();
			for (UINT i = 0; i < anim_defs.size(); i++) {
				if ((GetAnimKey(i) == 0) && (GetAnimCycle(i) == AnimCycleType::MANUAL)) {
					manualAnimsIdx.push_back(i);
				}
			}
			if (manualAnimsIdx.size() <= 0) {
				StopManualArm();
			}
			vector<UINT>::iterator it = find(manualAnimsIdx.begin(), manualAnimsIdx.end(), CurrentManualAnim);
			int index = distance(manualAnimsIdx.begin(), it);
			if (index != 0) {
				CurrentManualAnim = manualAnimsIdx[index - 1];
			}
		}
		return 1;
	}

	if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_RIGHT) {
		if (ManualArmActive()) {

			vector<UINT> manualAnimsIdx;
			manualAnimsIdx.clear();
			for (UINT i = 0; i < anim_defs.size(); i++) {
				if ((GetAnimKey(i) == 0) && (GetAnimCycle(i) == AnimCycleType::MANUAL)) {
					manualAnimsIdx.push_back(i);
				}
			}
			if (manualAnimsIdx.size() <= 0) {
				StopManualArm();
			}
			vector<UINT>::iterator it = find(manualAnimsIdx.begin(), manualAnimsIdx.end(), CurrentManualAnim);
			int index = distance(manualAnimsIdx.begin(), it);
			if (index != manualAnimsIdx.size() - 1) {
				CurrentManualAnim = manualAnimsIdx[index + 1];
			}
		}
		return 1;
	}


	return 0;
}
void AnimationManager::ConsumeAnimDirectKey(char *kstate) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (GetAnimCycle(i) != AnimCycleType::MANUAL) { continue; }
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && KEYDOWN(kstate, GetAnimKey(i))) {
			double speed = GetAnimSpeed(i);
			double state = GetAnimationState(i);
			state += speed*oapiGetSimStep();
			if (state > 1) { state = 1; }
			SetAnimationState(i, state);
			RESETKEY(kstate, GetAnimKey(i));

		}
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && KEYDOWN(kstate, GetAnimKey(i))) {
			double speed = (-1)*GetAnimSpeed(i);
			double state = GetAnimationState(i);
			state += speed*oapiGetSimStep();
			if (state < 0) { state = 0; }
			SetAnimationState(i, state);
			RESETKEY(kstate, GetAnimKey(i));

		}
	}
	if (ManualArmActive()) {
		if (KEYMOD_SHIFT(kstate)) {
			if (KEYDOWN(kstate, OAPI_KEY_UP)) {
				double speed = GetAnimSpeed(CurrentManualAnim);
				double state = GetAnimationState(CurrentManualAnim);
				state += speed*oapiGetSimStep();
				if (state > 1) { state = 1; }
				SetAnimationState(CurrentManualAnim, state);
				RESETKEY(kstate, OAPI_KEY_UP);
				return;
			}
			if (KEYDOWN(kstate, OAPI_KEY_DOWN)) {
				double speed = (-1)* GetAnimSpeed(CurrentManualAnim);
				double state = GetAnimationState(CurrentManualAnim);
				state += speed*oapiGetSimStep();
				if (state < 0) { state = 0; }
				SetAnimationState(CurrentManualAnim, state);
				RESETKEY(kstate, OAPI_KEY_DOWN);
				return;
			}
		}

	}
	return;
}


void AnimationManager::AnimationPreStep(double simt, double simdt, double mjd) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (IsAnimRunning(i)) {
			double speed = GetAnimSpeed(i);
			double state = GetAnimationState(i);
			if (IsAnimBackward(i)) {
				speed *= -1;
			}
			state += speed*oapiGetSimStep();

			if (state> 1) {
				switch (GetAnimCycle(i)) {
				case GOANDSTOP:
				{
					state = 1;
					StopAnimation(i);
					break;
				}
				case RESTART:
				{
					state = 0;
					break;
				}
				case REVERSE:
				{
					SetAnimationBackward(i, true);
					break;
				}


				}
			}
			else if (state< 0) {
				switch (GetAnimCycle(i)) {
				case GOANDSTOP:
				{
					state = 0;
					StopAnimation(i);
					SetAnimationBackward(i,false);
					break;
				}
				case RESTART:
				{
					state = 1;

					break;
				}
				case REVERSE:
				{
					SetAnimationBackward(i, false);
					break;
				}


				}
			}
			SetAnimationState(i, state);
			
		}

	}
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		if (IsAnimCompArmTip(i)) {
			if ((!VB1->AreVector3Equal(animcomp_defs[i].Tip[0], animcomp_defs[i].oldTip[0])) || (!VB1->AreVector3Equal(animcomp_defs[i].Tip[1], animcomp_defs[i].oldTip[1])) || (!VB1->AreVector3Equal(animcomp_defs[i].Tip[2], animcomp_defs[i].oldTip[2]))) {
				animcomp_defs[i].oldTip[0] = animcomp_defs[i].Tip[0];
				animcomp_defs[i].oldTip[1] = animcomp_defs[i].Tip[1];
				animcomp_defs[i].oldTip[2] = animcomp_defs[i].Tip[2];
				def_idx attidx = GetAnimCompDefAttTip(i);
				ATTACHMENTHANDLE ah = VB1->AttMng->GetAttDefAH(attidx);
				VB1->SetAttachmentParams(ah, animcomp_defs[i].Tip[0], animcomp_defs[i].Tip[1] - animcomp_defs[i].Tip[0], animcomp_defs[i].Tip[2] - animcomp_defs[i].Tip[0]);
				*VB1->AttMng->att_defs[attidx].pos_ptr = animcomp_defs[i].Tip[0];
				*VB1->AttMng->att_defs[attidx].dir_ptr = animcomp_defs[i].Tip[1] - animcomp_defs[i].Tip[0];
				*VB1->AttMng->att_defs[attidx].antidir_ptr = - animcomp_defs[i].Tip[1] + animcomp_defs[i].Tip[0];
				*VB1->AttMng->att_defs[attidx].antirot_ptr = -animcomp_defs[i].Tip[2] + animcomp_defs[i].Tip[0];

			}
		}
	}
	if (ManualArmActive()) {
		sprintf(oapiDebugString(), "Manual Anim:%s %.3f", GetAnimName(CurrentManualAnim).c_str(), GetAnimationState(CurrentManualAnim));
	}
	/*interval -= oapiGetSimStep();
	if (interval < 0) {
		interval = 1;
		for (UINT i = 0; i < animcomp_defs.size(); i++) {
			LogV("animcomp:%i nchildren:%i", i, ((ANIMATIONCOMP*)animcomp_defs[i].ach)->nchildren);
		}
	}*/
	


	return;
}

void AnimationManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Animations Section");
	UINT anim_counter = 0;
	char cbuf[256] = { '\0' };
	int id;
	sprintf(cbuf, "ANIM_%i_ID", anim_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		sprintf(cbuf, "ANIM_%i_NAME", anim_counter);
		char namebuf[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		double defstate, duration;
		DWORD key;
		AnimCycleType Cycle;
		sprintf(cbuf, "ANIM_%i_DEFSTATE", anim_counter);
		oapiReadItem_float(fh, cbuf, defstate);
		sprintf(cbuf, "ANIM_%i_DURATION", anim_counter);
		oapiReadItem_float(fh, cbuf, duration);
		sprintf(cbuf, "ANIM_%i_KEY", anim_counter);
		int kk;
		oapiReadItem_int(fh, cbuf, kk);
		key = (DWORD)kk;
		int Cyc;
		sprintf(cbuf, "ANIM_%i_CYCLE", anim_counter);
		oapiReadItem_int(fh, cbuf, Cyc);
		Cycle = (AnimCycleType)Cyc;

		AddAnimDef(name, duration, Cycle, key, defstate);

		anim_counter++;
		sprintf(cbuf, "ANIM_%i_ID", anim_counter);
	}

	for (UINT i = 0; i < 256; i++) {
		cbuf[i] = '\0';
	}
	UINT animcompdef_counter = 0;
	sprintf(cbuf, "ANIMCOMP_%i_ID", animcompdef_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		int seq, parent_idx, armatt, mesh, type_int, ngrps;
		bool arm_tip;
		double state0, state1, angle;
		VECTOR3 ref, axis, scale, shift;
		vector<UINT>grps;
		MGROUP_TRANSFORM::TYPE type;
		sprintf(cbuf, "ANIMCOMP_%i_NAME", animcompdef_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "ANIMCOMP_%i_SEQ", animcompdef_counter);
		oapiReadItem_int(fh, cbuf, seq);
		sprintf(cbuf, "ANIMCOMP_%i_STATE0", animcompdef_counter);
		oapiReadItem_float(fh, cbuf, state0);
		sprintf(cbuf, "ANIMCOMP_%i_STATE1", animcompdef_counter);
		oapiReadItem_float(fh, cbuf, state1);
		sprintf(cbuf, "ANIMCOMP_%i_MESH", animcompdef_counter);
		oapiReadItem_int(fh, cbuf, mesh);
		sprintf(cbuf, "ANIMCOMP_%i_NGRPS", animcompdef_counter);
		oapiReadItem_int(fh, cbuf, ngrps);

		grps.clear();
		if (ngrps > 0) {
			sprintf(cbuf, "ANIMCOMP_%i_GRPS", animcompdef_counter);
			char grpsbuf[256] = { '\0' };
			oapiReadItem_string(fh, cbuf, grpsbuf);
			string grpsbuf_s(grpsbuf);
			grps = VB1->readVectorUINT(grpsbuf_s);
			if ((grps.size() == 1) && (grps[0] == -1)){
				mesh = LOCALVERTEXLIST;
			}
		}
		sprintf(cbuf, "ANIMCOMP_%i_TYPE", animcompdef_counter);
		oapiReadItem_int(fh, cbuf, type_int);
		type = (MGROUP_TRANSFORM::TYPE)type_int;
		if (type_int == 1) { //ROTATE
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcompdef_counter);
			oapiReadItem_vec(fh, cbuf, ref);
			sprintf(cbuf, "ANIMCOMP_%i_AXIS", animcompdef_counter);
			oapiReadItem_vec(fh, cbuf, axis);
			sprintf(cbuf, "ANIMCOMP_%i_ANGLE", animcompdef_counter);
			oapiReadItem_float(fh, cbuf, angle);
			angle *= RAD;
		}
		else if (type_int == 2) { //TRANSLATE
			sprintf(cbuf, "ANIMCOMP_%i_SHIFT", animcompdef_counter);
			oapiReadItem_vec(fh, cbuf, shift);
		}
		else if (type_int == 3) { //SCALE
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcompdef_counter);
			oapiReadItem_vec(fh, cbuf, ref);
			sprintf(cbuf, "ANIMCOMP_%i_SCALE", animcompdef_counter);
			oapiReadItem_vec(fh, cbuf, scale);
		}
		sprintf(cbuf, "ANIMCOMP_%i_ARMTIP", animcompdef_counter);
		if (!oapiReadItem_bool(fh, cbuf, arm_tip)) { arm_tip = false; }
		if (arm_tip) {
			sprintf(cbuf, "ANIMCOMP_%i_ARMATT", animcompdef_counter);
			oapiReadItem_int(fh, cbuf, armatt);
		}

		sprintf(cbuf, "ANIMCOMP_%i_PARENT", animcompdef_counter);
		oapiReadItem_int(fh, cbuf, parent_idx);
		UINT index = AddAnimCompDef(seq, name, state0, state1, mesh, ngrps, grps, parent_idx, type_int, ref, axis, scale, shift, angle);
		if (arm_tip) {
			SetAnimCompDefArmTip(index, armatt);
		}


		animcompdef_counter++;
		sprintf(cbuf, "ANIMCOMP_%i_ID", animcompdef_counter);
	}

	LogV("Parsing Animation Section Completed");
	LogV("Found: %i Animations and %i Animation Components", anim_counter, animcompdef_counter);
	return;
}
void AnimationManager::WriteCfg(FILEHANDLE fh) {

	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------ANIMATIONS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");

	char cbuf[256] = { '\0' };
	char cbuf2[256] = { '\0' };
	UINT anim_counter = 0;
	for (UINT i = 0; i < GetAnimDefsCount(); i++) {
		if (!IsAnimValid(i)) { continue; }
		sprintf(cbuf, "ANIM_%i_ID", anim_counter);
		oapiWriteItem_int(fh, cbuf, anim_counter);
		sprintf(cbuf, "ANIM_%i_NAME", anim_counter);
		sprintf(cbuf2, "%s", GetAnimName(i).c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIM_%i_DEFSTATE", anim_counter);
		oapiWriteItem_float(fh, cbuf, GetAnimDefState(i));
		sprintf(cbuf, "ANIM_%i_DURATION", anim_counter);
		oapiWriteItem_float(fh, cbuf, GetAnimDuration(i));
		sprintf(cbuf, "ANIM_%i_KEY", anim_counter);
		oapiWriteItem_int(fh, cbuf, GetAnimKey(i));
		sprintf(cbuf, "ANIM_%i_CYCLE", anim_counter);
		oapiWriteItem_int(fh, cbuf, (int)GetAnimCycle(i));
		oapiWriteLine(fh, " ");
		anim_counter++;

	}

	UINT animcompdef_counter = 0;
	for (UINT i = 0; i < GetAnimCompDefsCount(); i++) {
		if (!IsAnimCompDefValid(i)) { continue; }
		sprintf(cbuf, "ANIMCOMP_%i_ID", animcompdef_counter);
		oapiWriteItem_int(fh, cbuf, animcompdef_counter);
		sprintf(cbuf, "ANIMCOMP_%i_SEQ", animcompdef_counter);
		oapiWriteItem_int(fh, cbuf, GetAnimCompDefSeqIdx(i));
		sprintf(cbuf, "ANIMCOMP_%i_NAME", animcompdef_counter);
		sprintf(cbuf2, "%s", GetAnimCompDefName(i).c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIMCOMP_%i_STATE0", animcompdef_counter);
		oapiWriteItem_float(fh, cbuf, GetAnimCompDefState0(i));
		sprintf(cbuf, "ANIMCOMP_%i_STATE1", animcompdef_counter);
		oapiWriteItem_float(fh, cbuf, GetAnimCompDefState1(i));
		bool isArmTip = IsAnimCompArmTip(i);
		sprintf(cbuf, "ANIMCOMP_%i_ARMTIP", animcompdef_counter);
		oapiWriteItem_bool(fh, cbuf, isArmTip);
		if (isArmTip) {
			sprintf(cbuf, "ANIMCOMP_%i_ARMATT", animcompdef_counter);
			oapiWriteItem_int(fh, cbuf, GetAnimCompDefAttTip(i));
		}
		sprintf(cbuf, "ANIMCOMP_%i_MESH", animcompdef_counter);
		oapiWriteItem_int(fh, cbuf, GetAnimCompDefMesh(i));
		sprintf(cbuf, "ANIMCOMP_%i_TYPE", animcompdef_counter);
		int type = (int)GetAnimCompDefType(i);
		oapiWriteItem_int(fh, cbuf, type);
		if (type == 1) { //ROTATE
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcompdef_counter);
			oapiWriteItem_vec(fh, cbuf, GetAnimCompDefRef(i));
			sprintf(cbuf, "ANIMCOMP_%i_AXIS", animcompdef_counter);
			oapiWriteItem_vec(fh, cbuf, GetAnimCompDefAxis(i));
			sprintf(cbuf, "ANIMCOMP_%i_ANGLE", animcompdef_counter);
			oapiWriteItem_float(fh, cbuf, GetAnimCompDefAngle(i)*DEG);
		}
		else if (type == 2) { //TRANSLATE
			sprintf(cbuf, "ANIMCOMP_%i_SHIFT", animcompdef_counter);
			oapiWriteItem_vec(fh, cbuf, GetAnimCompDefShift(i));
		}
		else if (type == 3) { //SCALE
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcompdef_counter);
			oapiWriteItem_vec(fh, cbuf, GetAnimCompDefRef(i));
			sprintf(cbuf, "ANIMCOMP_%i_SCALE", animcompdef_counter);
			oapiWriteItem_vec(fh, cbuf, GetAnimCompDefScale(i));
		}
		sprintf(cbuf, "ANIMCOMP_%i_NGRPS", animcompdef_counter);
		int ngrps = GetAnimCompDefNGroups(i);
		oapiWriteItem_int(fh, cbuf, ngrps);
		vector<UINT> grps = GetAnimCompDefGroups(i);
		string line = VB1->WriteVectorUINT(grps);
		sprintf(cbuf2, "%s", line.c_str());
		sprintf(cbuf, "ANIMCOMP_%i_GRPS", animcompdef_counter);
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIMCOMP_%i_PARENT", animcompdef_counter);
		oapiWriteItem_int(fh, cbuf, GetParentCompDefIdx(GetAnimCompDefParent(i)));
		oapiWriteLine(fh, " ");
		animcompdef_counter++;
	}


	



	return;
}
UINT AnimationManager::GetAnimNComps(def_idx d_idx) {
	anim_idx a_idx = anim_defs[d_idx].idx;
	ANIMATION* anims;
	VB1->GetAnimPtr(&anims);
	return anims[a_idx].ncomp;
}
ANIMATIONCOMPONENT_HANDLE AnimationManager::GetAnimComp(def_idx d_idx, UINT comp_idx) {
	anim_idx a_idx = anim_defs[d_idx].idx;
	ANIMATION* anims;
	VB1->GetAnimPtr(&anims);
	return anims[a_idx].comp[comp_idx];
}
MGROUP_TRANSFORM::TYPE AnimationManager::GetAnimCompDefType(def_idx d_idx) {
	return ((ANIMATIONCOMP*)animcomp_defs[d_idx].ach)->trans->Type();
}
int AnimationManager::AnimationRunStatus(def_idx d_idx) {
	if ((IsAnimRunning(d_idx)) && (!IsAnimBackward(d_idx))) { return 1; }
	else if ((IsAnimRunning(d_idx)) && (IsAnimBackward(d_idx))) { return -1; }
	else { return 0; }
}

void AnimationManager::Clear() {
	LogV("Clearing Animation Section");
	ANIMATION* anims;
	UINT nanims = VB1->GetAnimPtr(&anims);
	for (UINT i = 0; i < nanims; i++) {
		VB1->DelAnimation(i);
		//delete anim_defs[i].state_ptr;
	}
	for (UINT i = 0; i < anim_defs.size(); i++) {
		delete anim_defs[i].state_ptr;
	}
	anim_defs.clear();
	animcomp_defs.clear();
	LogV("Clearing Animation Section Completed");
	return;
}

anim_idx AnimationManager::GetAnimIdx(def_idx d_idx) {
	return anim_defs[d_idx].idx;
}

double* AnimationManager::GetAnimStatePtr(def_idx d_idx) {
	return anim_defs[d_idx].state_ptr;
}