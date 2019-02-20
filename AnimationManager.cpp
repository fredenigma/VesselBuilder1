#include "StationBuilder1.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "AnimDef.h"
#include "AnimCompDef.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"

AnimationManager::AnimationManager(StationBuilder1 *_SB1) {
	SB1 = _SB1;
	animcomp_defs.clear();
	anim_defs.clear();
	animcomp_defs.clear();
	AnimEditingMode = false;
	ManualArmMoving = false;
	CurrentManualAnim = -1;
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
	acd->SetState(defstate);
	acd->SetName(name);
	acd->SetCycleType(CycleType);
	return AddAnimDef(acd);
}
void AnimationManager::AddAnimDef() {
	AnimDef *ad = new AnimDef;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Anim_%i", anim_defs.size());
	string ap(cbuf);
	ad->SetName(ap);
	return AddAnimDef(ad);
}
void AnimationManager::AddAnimDef(AnimDef *ad) {
	ad->SetAnimIdx(SB1->CreateAnimation(ad->GetDefState()));
	ad->SetSeqIdx(anim_defs.size());
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
	anim[idx].state = defstate;
	anim[idx].defstate = defstate;
	
	anim_defs[d_idx]->SetDefState(defstate);
	anim_defs[d_idx]->SetState(defstate);
	
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
	//UpdateTip(d_idx);
	return;
}
void AnimationManager::UpdateTip(def_idx d_idx) {
	bool hastip = false;
	UINT CompIdx = 0;
	for (UINT i = 0; i < anim_defs[d_idx]->Comps.size(); i++) {
		if (anim_defs[d_idx]->Comps[i]->IsArmTip()) {
			hastip = true;
			CompIdx = i;
		}
	}
	if (!hastip) { return; }
	//sprintf(oapiDebugString(), "Att Tip:%i", anim_defs[d_idx]->Comps[CompIdx]->GetAttTip());
	def_idx attidx = anim_defs[d_idx]->Comps[CompIdx]->GetAttTip();
	ATTACHMENTHANDLE ah = SB1->AttMng->GetAttDefAH(attidx);
	//SB1->AttMng->ModifyAttDef(anim_defs[d_idx]->Comps[CompIdx]->GetAttTip(), anim_defs[d_idx]->Comps[CompIdx]->Tip[0], anim_defs[d_idx]->Comps[CompIdx]->Tip[1] - anim_defs[d_idx]->Comps[CompIdx]->Tip[0], anim_defs[d_idx]->Comps[CompIdx]->Tip[2] - anim_defs[d_idx]->Comps[CompIdx]->Tip[0]);
	SB1->SetAttachmentParams(ah, anim_defs[d_idx]->Comps[CompIdx]->Tip[0], anim_defs[d_idx]->Comps[CompIdx]->Tip[1] - anim_defs[d_idx]->Comps[CompIdx]->Tip[0], anim_defs[d_idx]->Comps[CompIdx]->Tip[2] - anim_defs[d_idx]->Comps[CompIdx]->Tip[0]);
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
/*void AnimationManager::SetAnimPrefix(def_idx d_idx, string prefix) {
	anim_defs[d_idx]->SetPrefix(prefix);
	return;
}*/
/*string AnimationManager::GetAnimPrefix(def_idx d_idx) {
	return anim_defs[d_idx]->GetPrefix();
}*/
void AnimationManager::SetAnimName(def_idx d_idx, string name) {
	anim_defs[d_idx]->SetName(name);
	return;
}
string AnimationManager::GetAnimName(def_idx d_idx) {
	return anim_defs[d_idx]->GetName();
}
/*string AnimationManager::GetAnimFullName(def_idx d_idx) {
	return anim_defs[d_idx]->GetFullName();
}*/
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
void AnimationManager::StartManualArm() {
	ManualArmMoving = true;
	if (CurrentManualAnim == -1) {
		for (UINT i = 0; i < anim_defs.size(); i++) {
			if ((anim_defs[i]->GetKey() == 0) && (anim_defs[i]->GetCycleType() == AnimCycleType::MANUAL)) {
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

void AnimationManager::ConsumeAnimBufferedKey(DWORD key, bool down, char *kstate) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == anim_defs[i]->GetKey()) {
			if ((anim_defs[i]->GetCycleType() == AnimCycleType::MANUAL) || (anim_defs[i]->GetCycleType() == AnimCycleType::AUTOMATIC)) { continue; }
			if (anim_defs[i]->IsRunning()) {
				anim_defs[i]->Stop();
			}
			else {
				anim_defs[i]->StartForward();
			}

		}
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == anim_defs[i]->GetKey()) {
			if ((anim_defs[i]->GetCycleType() == AnimCycleType::MANUAL) || (anim_defs[i]->GetCycleType() == AnimCycleType::AUTOMATIC)) { continue; }
			if (anim_defs[i]->IsRunning()) {
				anim_defs[i]->Stop();
			}
			else {
				anim_defs[i]->StartBackward();
			}

		}
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_SPACE) {
		if (ManualArmActive()) {
			StopManualArm();
		}
		else {
			StartManualArm();
		}
	}
	
	if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_LEFT) {
		if (ManualArmActive()) {
			vector<UINT> manualAnimsIdx;
			manualAnimsIdx.clear();
			for (UINT i = 0; i < anim_defs.size(); i++) {
				if ((anim_defs[i]->GetKey() == 0) && (anim_defs[i]->GetCycleType() == AnimCycleType::MANUAL)) {
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
	}
	
	if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_RIGHT) {
		if (ManualArmActive()) {
			
			vector<UINT> manualAnimsIdx;
			manualAnimsIdx.clear();
			for (UINT i = 0; i < anim_defs.size(); i++) {
				if ((anim_defs[i]->GetKey() == 0) && (anim_defs[i]->GetCycleType() == AnimCycleType::MANUAL)) {
					manualAnimsIdx.push_back(i);
				}
			}
			if (manualAnimsIdx.size() <= 0){
				StopManualArm();
			}
			vector<UINT>::iterator it = find(manualAnimsIdx.begin(), manualAnimsIdx.end(), CurrentManualAnim);
			int index = distance(manualAnimsIdx.begin(), it);
			if (index != manualAnimsIdx.size() - 1) {
				CurrentManualAnim = manualAnimsIdx[index + 1];
			}
		}
	}
	

	return;
}
void AnimationManager::ConsumeAnimDirectKey(char *kstate) {
	for (UINT i = 0; i < anim_defs.size(); i++) {
		if (anim_defs[i]->GetCycleType() != AnimCycleType::MANUAL) { continue; }
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && KEYDOWN(kstate, anim_defs[i]->GetKey())) {
			double speed = anim_defs[i]->GetSpeed();
			double state = anim_defs[i]->GetState();
			state += speed*oapiGetSimStep();
			if (state > 1) { state = 1; }
			SetAnimationState(i, state);
			RESETKEY(kstate, anim_defs[i]->GetKey());
			
		}
		if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && KEYDOWN(kstate, anim_defs[i]->GetKey())) {
			double speed =(-1)* anim_defs[i]->GetSpeed();
			double state = anim_defs[i]->GetState();
			state += speed*oapiGetSimStep();
			if (state < 0) { state = 0; }
			SetAnimationState(i, state);
			RESETKEY(kstate, anim_defs[i]->GetKey());
			
		}
	}
	if (ManualArmActive()) {
		if (KEYMOD_SHIFT(kstate)) {
			if (KEYDOWN(kstate, OAPI_KEY_UP)) {
				double speed = anim_defs[CurrentManualAnim]->GetSpeed();
				double state = anim_defs[CurrentManualAnim]->GetState();
				state += speed*oapiGetSimStep();
				if (state > 1) { state = 1; }
				SetAnimationState(CurrentManualAnim, state);
				RESETKEY(kstate, OAPI_KEY_UP);
				return;
			}
			if (KEYDOWN(kstate, OAPI_KEY_DOWN)) {
				double speed = (-1)* anim_defs[CurrentManualAnim]->GetSpeed();
				double state = anim_defs[CurrentManualAnim]->GetState();
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
	AnimCompDef *acd = new AnimCompDef;
	//acd = new AnimCompDef();
	acd->Init(type);
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "AnimComp_%i", animcomp_defs.size());
	string name(cbuf);
	acd->SetName(name);
	return AddAnimCompDef(animdef_idx, acd);
}
void AnimationManager::AddAnimCompDef(def_idx animdef_idx, AnimCompDef* acd) {
	/*if (acd->ParentACD)
	{
		acd->SetParent(acd->ParentACD->ach);
	}
	else {
		acd->SetParent(NULL);
	}*/

	acd->AssignToAnimation(anim_defs[animdef_idx]->GetAnimIdx(), SB1);
	
//	acd->SetPrefix(prefix);
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
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		if (animcomp_defs[i]->IsArmTip()) {
			if ((!SB1->AreVector3Equal(animcomp_defs[i]->Tip[0], animcomp_defs[i]->oldTip[0]))||(!SB1->AreVector3Equal(animcomp_defs[i]->Tip[1], animcomp_defs[i]->oldTip[1]))||(!SB1->AreVector3Equal(animcomp_defs[i]->Tip[2], animcomp_defs[i]->oldTip[2]))) {
				animcomp_defs[i]->oldTip[0] = animcomp_defs[i]->Tip[0];
				animcomp_defs[i]->oldTip[1] = animcomp_defs[i]->Tip[1];
				animcomp_defs[i]->oldTip[2] = animcomp_defs[i]->Tip[2];
				def_idx attidx = animcomp_defs[i]->GetAttTip();
				ATTACHMENTHANDLE ah = SB1->AttMng->GetAttDefAH(attidx);
				SB1->SetAttachmentParams(ah, animcomp_defs[i]->Tip[0], animcomp_defs[i]->Tip[1]- animcomp_defs[i]->Tip[0], animcomp_defs[i]->Tip[2]- animcomp_defs[i]->Tip[0]);
			}
		}
	}
	if (ManualArmActive()) {
		sprintf(oapiDebugString(), "Manual Anim:%s %.3f",anim_defs[CurrentManualAnim]->GetName().c_str(),anim_defs[CurrentManualAnim]->GetState());
	}
	return;
}

void AnimationManager::DeleteAnimCompDef(def_idx d_idx) {
	for (UINT i = 0; i < animcomp_defs[d_idx]->ChildrenACD.size(); i++) {
		if (animcomp_defs[d_idx]->ChildrenACD[i]) {
			def_idx anim_index = GetAnimCompDefSeqIdx(GetAnimCompDefCompIdx(animcomp_defs[d_idx]->ChildrenACD[i]));
			for (UINT j = 0; j < anim_defs[anim_index]->Comps.size(); j++) {
				if (anim_defs[anim_index]->Comps[j] == animcomp_defs[d_idx]) {
					anim_defs[anim_index]->Comps.erase(anim_defs[anim_index]->Comps.begin() + j);
					break;
				}
			}
			animcomp_defs[d_idx]->ChildrenACD[i]->Invalidate();
		}
		
	}
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		for (UINT j = 0; j < animcomp_defs[i]->ChildrenACD.size(); i++) {
			if (animcomp_defs[i]->ChildrenACD[j] == animcomp_defs[d_idx]) {
				animcomp_defs[i]->ChildrenACD.erase(animcomp_defs[i]->ChildrenACD.begin() + j);
				break;
			}
		}
	}
	def_idx anim_index2 = IdxAni2Def(animcomp_defs[d_idx]->anim_idx);
	animcomp_defs[d_idx]->anim_idx;
	

	/*vector<AnimCompDef*>::iterator it = anim_defs[anim_index2]->Comps.begin();
	while (it != anim_defs[anim_index2]->Comps.end()) {
		if (!(*it)->IsValid()) {
			it = animcomp_defs.erase(it);
		}
		else {
			++it;
		}
	}*/

	for (UINT k = 0; k < anim_defs[anim_index2]->Comps.size(); k++) {
		if (anim_defs[anim_index2]->Comps[k]) {
			if (anim_defs[anim_index2]->Comps[k] == animcomp_defs[d_idx]) {
				anim_defs[anim_index2]->Comps.erase(anim_defs[anim_index2]->Comps.begin() + k);

				break;
			}
			
		}
	}
	SB1->DelAnimationComponent(anim_index2, GetAnimCompDefACH(d_idx));
	animcomp_defs[d_idx]->Invalidate();
	CompsCleanUp();
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
/*void AnimationManager::SetAnimCompDefPrefix(def_idx d_idx, string prefix) {
	animcomp_defs[d_idx]->SetPrefix(prefix);
	return;
}*/
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
/*string AnimationManager::GetAnimCompDefFullName(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetFullName();
}*/
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
//void AnimationManager::SetAnimCompDefParent(def_idx d_idx, ANIMATIONCOMPONENT_HANDLE _parent) {
void AnimationManager::SetAnimCompDefParent(def_idx d_idx, AnimCompDef* newParent_acd) {
	animcomp_defs[d_idx]->SetParent(newParent_acd);
	animcomp_defs[d_idx]->ParentACD = newParent_acd;
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		for (UINT j = 0; j < animcomp_defs[i]->ChildrenACD.size(); j++) {
			if (animcomp_defs[i]->ChildrenACD[j] == animcomp_defs[d_idx]) {
				animcomp_defs[i]->ChildrenACD.erase(animcomp_defs[i]->ChildrenACD.begin() + j);
				break;
			}
		}
	}
	if (newParent_acd != NULL) {
		newParent_acd->ChildrenACD.push_back(animcomp_defs[d_idx]);
	}
	
	return;
}

UINT AnimationManager::GetAnimCompDefCompIdx(AnimCompDef* acd) {
	for (UINT i = 0; i < animcomp_defs.size(); i++) {
		if (animcomp_defs[i] == acd) {
			return i;
		}
	}
	return (UINT)-1;
	/*
	size_t pos = find(animcomp_defs.begin(), animcomp_defs.end(), acd) - animcomp_defs.begin();
	if (pos >= animcomp_defs.size()) {
		return (UINT)-1;
	}
	else {
		return (UINT)pos;
	}*/
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
//anim_idx AnimationManager::FindAnimDefIndex(UINT comp_idx) {
//	return 0;
//}

AnimCompDef* AnimationManager::GetAnimComp(def_idx d_idx) {
	return animcomp_defs[d_idx];
}
AnimCompDef* AnimationManager::GetAnimComp(def_idx animd_idx,UINT comp_idx) {
	if (comp_idx < anim_defs[animd_idx]->Comps.size()) {
		return anim_defs[animd_idx]->Comps[comp_idx];
	}
	return NULL;
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
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------ANIMATIONS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");

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
		sprintf(cbuf, "ANIMCOMP_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "ANIMCOMP_%i_SEQ", i);
		oapiWriteItem_int(fh, cbuf, GetAnimCompDefSeqIdx(i));
		sprintf(cbuf, "ANIMCOMP_%i_NAME", i);
		sprintf(cbuf2, "%s", animcomp_defs[i]->GetName().c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIMCOMP_%i_STATE0", i);
		oapiWriteItem_float(fh,cbuf,animcomp_defs[i]->GetState0());
		sprintf(cbuf, "ANIMCOMP_%i_STATE1", i);
		oapiWriteItem_float(fh, cbuf, animcomp_defs[i]->GetState1());
		bool isArmTip = animcomp_defs[i]->IsArmTip();
		sprintf(cbuf, "ANIMCOMP_%i_ARMTIP", i);
		oapiWriteItem_bool(fh, cbuf, isArmTip);
		if (isArmTip) {
			sprintf(cbuf, "ANIMCOMP_%i_ARMATT", i);
			oapiWriteItem_int(fh, cbuf, animcomp_defs[i]->GetAttTip());
		}
		sprintf(cbuf, "ANIMCOMP_%i_MESH", i);
		oapiWriteItem_int(fh, cbuf, animcomp_defs[i]->GetMesh());
		sprintf(cbuf, "ANIMCOMP_%i_TYPE", i);
		int type = (int)animcomp_defs[i]->GetType();
		oapiWriteItem_int(fh, cbuf, type);
		if (type == 1) { //ROTATE
			sprintf(cbuf, "ANIMCOMP_%i_REF", i);
			oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetRef());
			sprintf(cbuf, "ANIMCOMP_%i_AXIS", i);
			oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetAxis());
			sprintf(cbuf, "ANIMCOMP_%i_ANGLE", i);
			oapiWriteItem_float(fh, cbuf, animcomp_defs[i]->GetAngle()*DEG);
		}
		else if (type == 2) { //TRANSLATE
			sprintf(cbuf, "ANIMCOMP_%i_SHIFT", i);
			oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetShift());
		}
		else if (type == 3) { //SCALE
			sprintf(cbuf, "ANIMCOMP_%i_REF", i);
			oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetRef());
			sprintf(cbuf, "ANIMCOMP_%i_SCALE", i);
			oapiWriteItem_vec(fh, cbuf, animcomp_defs[i]->GetScale());
		}
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
			sprintf(cbuf2, "%s", grpsline.c_str());
		}
		else {
			sprintf(cbuf2, "%i", -1);
		}		
		sprintf(cbuf, "ANIMCOMP_%i_GRPS", i);
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
void AnimationManager::AnimCompDefResetGroups(def_idx d_idx) {
	animcomp_defs[d_idx]->ResetGroups();
	return;
}
AnimCompDef* AnimationManager::GetAnimCompDefParent(def_idx d_idx) {
	return animcomp_defs[d_idx]->ParentACD;
}

void AnimationManager::ParseCfgFile(FILEHANDLE fh) {
	UINT anim_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "ANIM_%i_ID", anim_counter);
	int id;

	while (oapiReadItem_int(fh, cbuf, id)) {
		sprintf(cbuf, "ANIM_%i_NAME", anim_counter);
		char anim_name[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, anim_name);
		string name(anim_name);
		double defstate;
		sprintf(cbuf, "ANIM_%i_DEFSTATE", anim_counter);
		oapiReadItem_float(fh, cbuf, defstate);
		double duration;
		sprintf(cbuf, "ANIM_%i_DURATION", anim_counter);
		oapiReadItem_float(fh, cbuf, duration);
		int key;
		sprintf(cbuf, "ANIM_%i_KEY", anim_counter);
		oapiReadItem_int(fh, cbuf, key);
		int cycletype;
		sprintf(cbuf, "ANIM_%i_CYCLE", anim_counter);
		oapiReadItem_int(fh, cbuf, cycletype);
		AddAnimDef(defstate, duration, key, name, (AnimCycleType)cycletype);
		anim_counter++;
		sprintf(cbuf, "ANIM_%i_ID", anim_counter);
	}

	UINT animcomp_counter = 0;
	sprintf(cbuf, "ANIMCOMP_%i_ID", animcomp_counter);
	int ac_id;

	while (oapiReadItem_int(fh, cbuf, ac_id)) {
		int seq;
		sprintf(cbuf, "ANIMCOMP_%i_SEQ", animcomp_counter);
		oapiReadItem_int(fh, cbuf, seq);
		char animcomp_name[256] = { '\0' };
		sprintf(cbuf, "ANIMCOMP_%i_NAME", animcomp_counter);
		oapiReadItem_string(fh, cbuf, animcomp_name);
		string name(animcomp_name);
		sprintf(cbuf, "ANIMCOMP_%i_STATE0", animcomp_counter);
		double state0 = 0;
		oapiReadItem_float(fh, cbuf, state0);
		sprintf(cbuf, "ANIMCOMP_%i_STATE1", animcomp_counter);
		double state1 = 0;
		oapiReadItem_float(fh, cbuf, state1);
		int mesh;
		sprintf(cbuf, "ANIMCOMP_%i_MESH", animcomp_counter);
		oapiReadItem_int(fh, cbuf, mesh);
		int ngrps = -1;
		sprintf(cbuf, "ANIMCOMP_%i_NGRPS", animcomp_counter);
		oapiReadItem_int(fh, cbuf, ngrps);
		vector<UINT>grps;
		grps.clear();
		if (ngrps > 0) {
			char grpsbuf[512] = { '\0' };
			sprintf(cbuf, "ANIMCOMP_%i_GRPS", animcomp_counter);
			oapiReadItem_string(fh, cbuf, grpsbuf);
			string grpsbuf_s(grpsbuf);
			
			string delimiter = ",";
			size_t pos0 = 0;
			size_t pos1 = 0;
			while (pos1 != string::npos) {
				pos1 = grpsbuf_s.find(delimiter, pos0);
				string token = grpsbuf_s.substr(pos0, pos1 - pos0);
				grps.push_back(stoi(token));
				pos0 = pos1 + delimiter.length();
			}
		}
		int parent_idx = -1;
		sprintf(cbuf, "ANIMCOMP_%i_PARENT", animcomp_counter);
		oapiReadItem_int(fh, cbuf, parent_idx);
		




		int type;
		sprintf(cbuf, "ANIMCOMP_%i_TYPE", animcomp_counter);
		oapiReadItem_int(fh, cbuf, type);
		VECTOR3 ref, axis, scale, shift;
		double angle;
		if (type == 1) {
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcomp_counter);
			oapiReadItem_vec(fh, cbuf, ref);
			sprintf(cbuf, "ANIMCOMP_%i_AXIS", animcomp_counter);
			oapiReadItem_vec(fh, cbuf, axis);
			sprintf(cbuf, "ANIMCOMP_%i_ANGLE", animcomp_counter);
			oapiReadItem_float(fh, cbuf, angle);
			angle *= RAD;
		}
		else if (type == 2) {
			sprintf(cbuf, "ANIMCOMP_%i_SHIFT", animcomp_counter);
			oapiReadItem_vec(fh, cbuf, shift);
		}
		else if (type == 3) {
			sprintf(cbuf, "ANIMCOMP_%i_SCALE", animcomp_counter);
			oapiReadItem_vec(fh, cbuf, scale);
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcomp_counter);
			oapiReadItem_vec(fh, cbuf, ref);

		}
		bool isArmTip = false;
		sprintf(cbuf, "ANIMCOMP_%i_ARMTIP", animcomp_counter);
		oapiReadItem_bool(fh, cbuf, isArmTip);
		int AttIdx = -1;
		if (isArmTip) {
			sprintf(cbuf, "ANIMCOMP_%i_ARMATT", animcomp_counter);
			oapiReadItem_int(fh, cbuf, AttIdx);
		}
		


		AddAnimCompDef(seq, name, state0, state1, mesh, ngrps, grps, parent_idx,type, ref, axis, scale, shift, angle);
		if (isArmTip) {
			SetAnimCompDefArmTip(animcomp_defs.size() - 1, AttIdx);
		}
		animcomp_counter++;
		sprintf(cbuf, "ANIMCOMP_%i_ID", animcomp_counter);
	}


	return;
}

void AnimationManager::AddAnimCompDef(def_idx animdef_idx, string name, double state0, double state1, int mesh, int ngrps, vector<UINT>grps, int parent, int type, VECTOR3 ref, VECTOR3 axis, VECTOR3 scale, VECTOR3 shift, double angle) {
	AnimCompDef* acd = new AnimCompDef;
	acd->state0 = state0;
	acd->state1 = state1;
	acd->name = name;
	acd->ngrps = ngrps;
	acd->grps = grps;
	acd->ref = ref;
	acd->scale = scale;
	acd->angle = angle;
	acd->shift = shift;
	acd->axis = axis;
	acd->mesh = mesh;
	switch (type) {
	case 1:
	{
		if (ngrps <= 0) {
			acd->mgt = new MGROUP_ROTATE(mesh, NULL, 0, ref, axis, angle);
		}
		else {
			acd->pgrps = new UINT[ngrps];
			for (UINT i = 0; i < ngrps; i++) {
				acd->pgrps[i] = grps[i];
			}
			acd->mgt = new MGROUP_ROTATE(mesh, acd->pgrps, ngrps, ref, axis, angle);
		}
		break;
	}
	case 2:
	{
		if (ngrps <= 0) {
			acd->mgt = new MGROUP_TRANSLATE(mesh, NULL, 0, shift);
		}
		else {
			acd->pgrps = new UINT[ngrps];
			for (UINT i = 0; i < ngrps; i++) {
				acd->pgrps[i] = grps[i];
			}
			acd->mgt = new MGROUP_TRANSLATE(mesh, acd->pgrps, ngrps, shift);
		}
		break;
	}
	case 3:
	{
		if (ngrps <= 0) {
			acd->mgt = new MGROUP_SCALE(mesh, NULL, 0, ref, scale);
		}
		else {
			acd->pgrps = new UINT[ngrps];
			for (UINT i = 0; i < ngrps; i++) {
				acd->pgrps[i] = grps[i];
			}
			acd->mgt = new MGROUP_SCALE(mesh, acd->pgrps, ngrps,ref, scale);
		}
		break;
	}
	}

	AddAnimCompDef(animdef_idx, acd);
	if (parent > -1) {
		AnimCompDef* parent_acd = GetAnimComp(parent);
		SetAnimCompDefParent(animcomp_defs.size() - 1, parent_acd);
	}
	

}


bool AnimationManager::IsAnimCompDefArmTip(def_idx d_idx) {
	return animcomp_defs[d_idx]->IsArmTip();
}
void AnimationManager::GetAnimCompDefTips(def_idx d_idx, VECTOR3 &v1, VECTOR3 &v2, VECTOR3 &v3) {
	return animcomp_defs[d_idx]->GetTips(v1, v2, v3);
}
def_idx AnimationManager::GetAnimCompDefAttTip(def_idx d_idx) {
	return animcomp_defs[d_idx]->GetAttTip();
}
void AnimationManager::SetAnimCompDefTips(def_idx d_idx, VECTOR3 v1, VECTOR3 v2, VECTOR3 v3) {
	return animcomp_defs[d_idx]->SetTips(v1, v2, v3);
}

void AnimationManager::SetIsAnimCompDefArmTip(def_idx d_idx, bool set) {
	return animcomp_defs[d_idx]->SetIsArmTip(set);
}
void AnimationManager::SetAnimCompDefAttTip(def_idx d_idx, def_idx att_idx) {
	return animcomp_defs[d_idx]->SetAttTip(att_idx);
}
void AnimationManager::SetAnimCompDefArmTip(def_idx d_idx, def_idx AttIdx) {

	animcomp_defs[d_idx]->SetIsArmTip(true);
	animcomp_defs[d_idx]->SetAttTip(AttIdx);
	SetAnimCompDefMesh(d_idx, LOCALVERTEXLIST);
	AnimCompDefResetGroups(d_idx);
	VECTOR3 pos, dir, rot;
	SB1->AttMng->GetAttDefPosDirRot(AttIdx, pos, dir, rot);
	animcomp_defs[d_idx]->SetTips(pos, pos + dir, pos + rot);
	((ANIMATIONCOMP*)animcomp_defs[d_idx]->ach)->trans->grp = MAKEGROUPARRAY(animcomp_defs[d_idx]->Tip);
	((ANIMATIONCOMP*)animcomp_defs[d_idx]->ach)->trans->ngrp = 3;
//	animcomp_defs[d_idx]->ngrps = 3;
	return;
}
/*
for (UINT i = 0; i < animcomp_defs.size(); i++) {
	
	
	sprintf(cbuf, "ANIMCOMP_%i_NGRPS", i);
	int ngrps = animcomp_defs[i]->GetNgrps();
	oapiWriteItem_int(fh, cbuf, ngrps);
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
		sprintf(cbuf2, "%s", grpsline.c_str());
	}
	else {
		sprintf(cbuf2, "%i", -1);
	}
	sprintf(cbuf, "ANIMCOMP_%i_GRPS", i);
	oapiWriteItem_string(fh, cbuf, cbuf2);
	sprintf(cbuf, "ANIMCOMP_%i_PARENT", i);
	oapiWriteItem_int(fh, cbuf, GetAnimCompDefCompIdx(animcomp_defs[i]->ParentACD));
	*/