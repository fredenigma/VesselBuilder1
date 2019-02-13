#include "StationBuilder1.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "AnimDef.h"
#include "AnimCompDef.h"
#include "AnimationManager.h"

AnimationManager::AnimationManager(StationBuilder1 *_SB1) {
	SB1 = _SB1;
	animcomp_defs.clear();
	anim_defs.clear();
	animcomp_defs.clear();
	AnimEditingMode = false;
}
AnimationManager::~AnimationManager() {
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		if (animcomp_defs[i]->mgt) {
			delete animcomp_defs[i]->mgt;
		}
		if (animcomp_defs[i]) {
			delete animcomp_defs[i];
		}
	}
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i]) {
			delete anim_defs[i];
		}
	}
}

UINT AnimationManager::NextAnimSeq() {
	UINT counter = 0;
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i]->IsValid()) {
			counter++;
		}
	}
	return counter;
}
void AnimationManager::AddAnimDef(double defstate, double duration,DWORD key, string name, AnimCycleType CycleType) {
	AnimDef *acd = new AnimDef;
	acd->defstate = defstate;
	acd->duration = duration;
	acd->speed = 1 / duration;
	acd->key = key;
	acd->SetName(name);
	acd->SetCycleType(CycleType);
	return AddAnimDef(acd);
}
void AnimationManager::AddAnimDef() {
	AnimDef *acd = new AnimDef;
	return AddAnimDef(acd);
}
void AnimationManager::AddAnimDef(AnimDef *ad) {
	ad->SetAnimIdx(SB1->CreateAnimation(ad->GetDefState()));
	ad->SetSeqIdx(anim_defs.size());
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Anim_%i_", ad->GetSeqIdx());
	string ap(cbuf);
	ad->SetPrefix(ap);
	anim_defs.push_back(ad);
	
	return;
}

anim_idx AnimationManager::IdxDef2Ani(def_idx _def_idx) {
	return anim_defs[_def_idx]->GetAnimIdx();
}
def_idx AnimationManager::IdxAni2Def(anim_idx _anim_idx) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i]->GetAnimIdx() == _anim_idx) {
			return i;
		}
	}
	return (def_idx)-1;
}


void AnimationManager::SetAnimDefState(def_idx d_idx, double defstate) {
	//def_idx idx = IdxAni2Def(an_idx);
	anim_idx idx = anim_defs[d_idx]->GetAnimIdx();
	ANIMATION* anim;
	SB1->GetAnimPtr(&anim);
	anim[idx].defstate = defstate;
	anim_defs[d_idx]->SetDefState(defstate);
	return;		
}
void AnimationManager::ResetAnimation(def_idx d_idx) {
	anim_idx idx = IdxDef2Ani(d_idx);
	SB1->SetAnimation(idx, anim_defs[d_idx]->GetDefState());
	return;
}
void AnimationManager::SetAnimationState(def_idx d_idx,double state) {
	anim_idx idx = IdxDef2Ani(d_idx);
	SB1->SetAnimation(idx, state);
	anim_defs[d_idx]->SetState(state);
	return;
}
void AnimationManager::SetAnimDuration(def_idx d_idx, double duration) {
	anim_defs[d_idx]->SetDuration(duration);
	return;
}
double AnimationManager::GetAnimDuration(def_idx d_idx) {
	return anim_defs[d_idx]->GetDuration();
}
double AnimationManager::GetAnimDefState(def_idx d_idx) {
	return anim_defs[d_idx]->GetDefState();
}
void AnimationManager::SetAnimPrefix(def_idx d_idx, string prefix) {
	anim_defs[d_idx]->SetPrefix(prefix);
	return;
}
string AnimationManager::GetAnimPrefix(def_idx d_idx) {
	return anim_defs[d_idx]->GetPrefix();
}
void AnimationManager::SetAnimName(def_idx d_idx, string name) {
	anim_defs[d_idx]->SetName(name);
	return;
}
string AnimationManager::GetAnimName(def_idx d_idx) {
	return anim_defs[d_idx]->GetName();
}
string AnimationManager::GetAnimFullName(def_idx d_idx) {
	return anim_defs[d_idx]->GetFullName();
}
void AnimationManager::SetAnimCycleType(def_idx d_idx, AnimCycleType CycleType) {
	anim_defs[d_idx]->SetCycleType(CycleType);
	return;
}
AnimCycleType AnimationManager::GetAnimCycleType(def_idx d_idx) {
	return anim_defs[d_idx]->GetCycleType();
}
bool AnimationManager::IsAnimValid(def_idx d_idx) {
	return anim_defs[d_idx]->IsValid();
}
void AnimationManager::InvalidateAnim(def_idx d_idx) {
	anim_defs[d_idx]->InvalidAnimDef();
	return;
}
bool AnimationManager::IsAnimCompValid(def_idx d_idx) {
	return animcomp_defs[d_idx]->IsValid();
}
void AnimationManager::InvalidateAnimComp(def_idx d_idx) {
	animcomp_defs[d_idx]->Invalidate();
	return;
}
void AnimationManager::CompsCleanUp() {
	vector<AnimCompDef*>::iterator it = animcomp_defs.begin();
	while (it != animcomp_defs.end()) {
		if (!(*it)->IsValid()) {
			it = animcomp_defs.erase(it);
		}
		else {
			++it;
		}
	}
	
	return;
}
void AnimationManager::DeleteAnimDef(def_idx d_idx) {
	for (UINT i = 0; i < anim_defs[d_idx]->Comps.size(); i++) {
		if (anim_defs[d_idx]->Comps[i]) {
			anim_defs[d_idx]->Comps[i]->Invalidate();
		}
	}
	anim_defs[d_idx]->Comps.clear();
	
	CompsCleanUp();
	anim_idx idx = IdxDef2Ani(d_idx);
	SB1->DelAnimation(idx);
	anim_defs.erase(anim_defs.begin() + d_idx);
	return;
}
void AnimationManager::SetAnimKey(def_idx d_idx, DWORD key) {
	anim_defs[d_idx]->SetKey(key);
	return;
}
UINT AnimationManager::GetAnimNComps(def_idx d_idx) {
	return anim_defs[d_idx]->GetNComps();
}


void AnimationManager::ConsumeAnimKey(DWORD key, bool down, char *kstate) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == anim_defs[i]->GetKey()) {
			if (anim_defs[i]->IsRunning()) {
				anim_defs[i]->Stop();
			}
			else {
				anim_defs[i]->StartForward();
			}
		}
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == anim_defs[i]->GetKey()) {
			if (anim_defs[i]->IsRunning()) {
				anim_defs[i]->Stop();
			}
			else {
				anim_defs[i]->StartBackward();
			}
		}
	}
	return;
}




/*ANIMATIONCOMPONENT_HANDLE AnimationManager::FindParentACH(UINT parent_comp_idx) {
	if (parent_comp_idx == (UINT)-1) { return NULL; }
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		if (animcomp_defs[i]->GetCompIdx()== parent_comp_idx) {
			return animcomp_defs[i]->GetACH();
		}
	}

	return NULL;
}*/

void AnimationManager::AddAnimCompDef(def_idx animdef_idx, MGROUP_TRANSFORM::TYPE type) {
	AnimCompDef *acd;
	acd = new AnimCompDef();
	acd->Init(type);
	return AddAnimCompDef(animdef_idx, acd);
}
void AnimationManager::AddAnimCompDef(def_idx animdef_idx, AnimCompDef* acd) {
	if (acd->ParentACD)
	{
		acd->SetParent(acd->ParentACD->ach);
	}
	else {
		acd->SetParent(NULL);
	}
	acd->AssignToAnimation(anim_defs[animdef_idx]->GetAnimIdx(), SB1);

	char cbuf[256] = { '\0' };
	sprintf(cbuf, "AnimComp_%i_", animcomp_defs.size());
	string prefix(cbuf);
	acd->SetPrefix(prefix);
	animcomp_defs.push_back(acd);
	anim_defs[animdef_idx]->Comps.push_back(acd);
	return;
}
def_idx AnimationManager::IdxSeq2Def(UINT seq_idx) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i]->GetSeqIdx() == seq_idx) {
			return i;
		}
	}
	return (UINT)-1;
}

void AnimationManager::AnimationPreStep(double simt, double simdt, double mjd) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i]->IsRunning()) {
			double speed = anim_defs[i]->GetSpeed();
			double state = anim_defs[i]->GetState();
			if (anim_defs[i]->IsBackward()) {
				speed *= -1;
			}
			state += speed*oapiGetSimStep();

			if (state> 1) {
				switch (anim_defs[i]->GetCycleType()) {
				case GOANDSTOP:
				{
					state=1;
					anim_defs[i]->Stop();
					break;
				}
				case RESTART:
				{
					state = 0;
					break;
				}
				case REVERSE:
				{
					anim_defs[i]->SetBackward(true);
					break;
				}


				}
			}
			else if (state< 0) {
				switch (anim_defs[i]->GetCycleType()) {
				case GOANDSTOP:
				{
					state = 0;
					anim_defs[i]->Stop();
					anim_defs[i]->SetBackward(false);
					break;
				}
				case RESTART:
				{
					state = 1;

					break;
				}
				case REVERSE:
				{
					anim_defs[i]->SetBackward(false);
					break;
				}


				}
			}
			SetAnimationState(i,state);
		}
	}


	return;
}

void AnimationManager::DeleteAnimCompDef(def_idx d_idx) {
	////////////////////TO DO!
	return;
}
void AnimationManager::SetAnimCompDefState0(def_idx d_idx, double newState0) {
	animcomp_defs[d_idx]->SetState0(newState0);
	return;
}
void AnimationManager::SetAnimCompDefState1(def_idx d_idx, double newState1) {
	animcomp_defs[d_idx]->SetState1(newState1);
	return;
}
void AnimationManager::SetAnimCompDefRef(def_idx d_idx, VECTOR3 newRef) {
	animcomp_defs[d_idx]->SetRef(newRef);
	return;
}
void AnimationManager::SetAnimCompDefScale(def_idx d_idx, VECTOR3 newScale) {
	animcomp_defs[d_idx]->SetScale(newScale);
	return;
}
void AnimationManager::SetAnimCompDefShift(def_idx d_idx, VECTOR3 newShift) {
	animcomp_defs[d_idx]->SetShift(newShift);
	return;
}
void AnimationManager::SetAnimCompDefAxis(def_idx d_idx, VECTOR3 newAxis) {
	animcomp_defs[d_idx]->SetAxis(newAxis);
	return;
}
void AnimationManager::SetAnimCompDefAngle(def_idx d_idx, double newAngle) {
	animcomp_defs[d_idx]->SetAngle(newAngle);
	return;
}
void AnimationManager::SetAnimCompDefName(def_idx d_idx, string name) {
	animcomp_defs[d_idx]->SetName(name);
	return;
}
void AnimationManager::SetAnimCompDefPrefix(def_idx d_idx, string prefix) {
	animcomp_defs[d_idx]->SetPrefix(prefix);
	return;
}
void AnimationManager::SetAnimCompDefMesh(def_idx d_idx, msh_idx m_idx) {
	animcomp_defs[d_idx]->SetMesh(m_idx);
	return;
}
void AnimationManager::AnimCompDefAddGroups(def_idx d_idx, vector<UINT>grps) {
	animcomp_defs[d_idx]->AddGroups(grps);
	return;
}
void AnimationManager::AnimCompDefRemoveGroups(def_idx d_idx, vector<UINT>grps) {
	animcomp_defs[d_idx]->RemoveGroups(grps);
	return;
}
string AnimationManager::GetAnimCompDefName(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetName();
}
string AnimationManager::GetAnimCompDefFullName(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetFullName();
}
double AnimationManager::GetAnimCompDefState0(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetState0();
}
double AnimationManager::GetAnimCompDefState1(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetState1();
}
VECTOR3 AnimationManager::GetAnimCompDefRef(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetRef();
}
VECTOR3 AnimationManager::GetAnimCompDefAxis(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetAxis();
}
VECTOR3 AnimationManager::GetAnimCompDefShift(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetShift();
}
VECTOR3 AnimationManager::GetAnimCompDefScale(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetScale();
}
double AnimationManager::GetAnimCompDefAngle(def_idx d_idx){
	return animcomp_defs[d_idx]->GetAngle();
}
msh_idx AnimationManager::GetAnimCompDefMesh(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetMesh();
}
vector<UINT> AnimationManager::GetAnimCompDefGroups(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetGroups();
}
int AnimationManager::GetAnimCompDefNGroups(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetNgrps();
}
void AnimationManager::AssignAnimCompDefToAnimation(def_idx comp_idx, anim_idx _anim_idx, VESSEL *v) {
	animcomp_defs[comp_idx]->AssignToAnimation(_anim_idx, v);
	return;
}
void AnimationManager::SetAnimCompDefParent(def_idx d_idx, ANIMATIONCOMPONENT_HANDLE _parent) {
	animcomp_defs[d_idx]->SetParent(_parent);
	return;
}

UINT AnimationManager::GetAnimCompDefCompIdx(AnimCompDef* acd) {
	size_t pos = find(animcomp_defs.begin(), animcomp_defs.end(), acd) - animcomp_defs.begin();
	if (pos >= animcomp_defs.size()) {
		return (UINT)-1;
	}
	else {
		return (UINT)pos;
	}
	//return animcomp_defs[d_idx]->GetCompIdx();
}
UINT AnimationManager::GetAnimCompDefSeqIdx(def_idx d_idx) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		for (UINT j = 0; j < anim_defs[i]->Comps.size(); j++) {
			if (anim_defs[i]->Comps[j] == animcomp_defs[d_idx]) {
				return i;
			}
		}
	}
	return (UINT)-1;
}
UINT AnimationManager::GetAnimCompDefParentCompIdx(def_idx d_idx) {
	if (animcomp_defs[d_idx]->ParentACD) {
		return GetAnimCompDefCompIdx(animcomp_defs[d_idx]->ParentACD);
	}
	else {
		return (UINT)-1;
	}
	
}
ANIMATIONCOMPONENT_HANDLE AnimationManager::GetAnimCompDefParentACH(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetParentACH();
}
MGROUP_TRANSFORM::TYPE AnimationManager::GetAnimCompDefType(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetType();
}
ANIMATIONCOMPONENT_HANDLE AnimationManager::GetAnimCompDefACH(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetACH();
}

UINT AnimationManager::GetAnimDefCount() {
	UINT counter = 0;
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i]->IsValid()) {
			counter++;
		}
	}
	return counter;
}
UINT AnimationManager::GetAnimCompDefCount() {
	return animcomp_defs.size();
}
DWORD AnimationManager::GetAnimKey(def_idx d_idx) {
	return anim_defs[d_idx]->GetKey();
}
anim_idx AnimationManager::FindAnimDefIndex(UINT comp_idx) {
	return 0;
}

AnimCompDef* AnimationManager::GetAnimComp(def_idx d_idx) {
	return animcomp_defs[d_idx];
}
AnimCompDef* AnimationManager::GetAnimComp(def_idx animd_idx,UINT comp_idx) {
	return anim_defs[animd_idx]->Comps[comp_idx];
}
/*void AnimationManager::UpdateAllCompsSeq(def_idx d_idx) { //DA CHIAMARE QUANDO CANCELLO UNA ANIM!
	for (UINT i = 0; i < anim_defs.size(); i++) {
		for (UINT j = 0; j < animcomp_defs.size(); j++) {
			if (animcomp_defs[j]->anim_idx == anim_defs[i]->GetAnimIdx()) {
				animcomp_defs[j]->SetSeqIdx(anim_defs[i]->GetSeqIdx());
			}
		}
	}
	return;
}*/
void AnimationManager::WriteCfg(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	char cbuf2[256] = { '\0' };
	oapiWriteItem_int(fh, "NUMBER_OF_ANIMATIONS", anim_defs.size());
	for (UINT i = 0; i < anim_defs.size(); i++) {
		sprintf(cbuf, "ANIM_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "ANIM_%i_NAME",i);
		sprintf(cbuf2, "%s", anim_defs[i]->GetName().c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIM_%i_DEFSTATE",i);
		oapiWriteItem_float(fh, cbuf, anim_defs[i]->GetDefState());
		sprintf(cbuf, "ANIM_%i_DURATION",i);
		oapiWriteItem_float(fh, cbuf, anim_defs[i]->GetDuration());
		sprintf(cbuf, "ANIM_%i_KEY", i);
		oapiWriteItem_int(fh, cbuf, anim_defs[i]->GetKey());
		sprintf(cbuf, "ANIM_%i_CYCLE", i);
		oapiWriteItem_int(fh, cbuf, (int)anim_defs[i]->GetCycleType());
		oapiWriteLine(fh, " ");

	}
	oapiWriteItem_int(fh, "NUMBER_OF_ANIMATION_COMPONENTS", animcomp_defs.size());
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		sprintf(cbuf, "ANIMCOMP_%i_SEQ", i);
		oapiWriteItem_int(fh, cbuf, GetAnimCompDefSeqIdx(i));
		sprintf(cbuf, "ANIMCOMP_%i_NAME", i);
		sprintf(cbuf2, "%s", animcomp_defs[i]->GetName().c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIMCOMP_%i_STATE0", i);
		oapiWriteItem_float(fh,cbuf,animcomp_defs[i]->GetState0());
		sprintf(cbuf, "ANIMCOMP_%i_STATE1", i);
		oapiWriteItem_float(fh, cbuf, animcomp_defs[i]->GetState1());
		sprintf(cbuf, "ANIMCOMP_%i_TYPE", i);
		oapiWriteItem_int(fh, cbuf, (int)animcomp_defs[i]->GetType());
		sprintf(cbuf, "ANIMCOMP_%i_REF", i);
		oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetRef());
		sprintf(cbuf, "ANIMCOMP_%i_AXIS", i);
		oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetAxis());
		sprintf(cbuf, "ANIMCOMP_%i_ANGLE", i);
		oapiWriteItem_float(fh, cbuf, animcomp_defs[i]->GetAngle()*DEG);
		sprintf(cbuf, "ANIMCOMP_%i_SCALE", i);
		oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetScale());
		sprintf(cbuf, "ANIMCOMP_%i_SHIFT", i);
		oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetShift());
		sprintf(cbuf, "ANIMCOMP_%i_NGRPS", i);
		int ngrps = animcomp_defs[i]->GetNgrps();
		oapiWriteItem_int(fh, cbuf,ngrps);
		string grpsline(" ");
		if (ngrps > 0)
		{
			for (UINT j = 0; j < ngrps; j++) {
				char add[128] = { '\0' };
				if (j == ngrps - 1) {
					sprintf(add, "%i", animcomp_defs[i]->grps[j]);
				}
				else {
					sprintf(add, "%i, ", animcomp_defs[i]->grps[j]);
				}
				string adds(add);
				grpsline += adds;
			}
		}
		sprintf(cbuf2, "%s", grpsline.c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIMCOMP_%i_PARENT", i);
		oapiWriteItem_int(fh, cbuf, GetAnimCompDefCompIdx(animcomp_defs[i]->ParentACD));
		oapiWriteLine(fh, " ");
	}

	return;
}
void AnimationManager::StartAnim(def_idx d_idx) {
	anim_defs[d_idx]->Start();
	return;
}
void AnimationManager::StopAnim(def_idx d_idx) {
	anim_defs[d_idx]->Stop();
	return;
}
void AnimationManager::ResetAnim(def_idx d_idx) {
	SetAnimationState(d_idx, anim_defs[d_idx]->GetDefState());
	return;
}