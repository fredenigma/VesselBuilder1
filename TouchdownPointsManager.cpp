#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "AnimationManager.h"
#include "TouchdownPointsManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

TouchdownPointsManager::TouchdownPointsManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	set1.clear();
	set2.clear();
	change_anim_idx = (UINT)-1;
	Set2Enabled = false;
	CurrentSet = 0;
	state_chk = NULL;
	return;
}
TouchdownPointsManager::~TouchdownPointsManager() {
	VB1 = NULL;
	return;
}
void TouchdownPointsManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing TouchDown Points Section");
	
	
	for (UINT set = 1; set < 3; set++) {
		char cbuf[256] = { '\0' };
		UINT point_counter = 0;
		VECTOR3 pos;
		sprintf(cbuf, "TDP_%i_%i_POS", set, point_counter);
		while (oapiReadItem_vec(fh, cbuf, pos)) {
			double damping, stiffness, mu, mu_lng;
			sprintf(cbuf, "TDP_%i_%i_STIFFNESS", set, point_counter);
			oapiReadItem_float(fh, cbuf, stiffness);
			sprintf(cbuf, "TDP_%i_%i_DAMPING", set, point_counter);
			oapiReadItem_float(fh, cbuf, damping);
			sprintf(cbuf, "TDP_%i_%i_MU", set, point_counter);
			oapiReadItem_float(fh, cbuf, mu);
			sprintf(cbuf, "TDP_%i_%i_MULNG", set, point_counter);
			oapiReadItem_float(fh, cbuf, mu_lng);
			TOUCHDOWNVTX tdvtx;
			tdvtx.pos = pos;
			tdvtx.damping = damping;
			tdvtx.stiffness = stiffness;
			tdvtx.mu = mu;
			tdvtx.mu_lng = mu_lng;
			AddPoint(set, tdvtx);

			point_counter++;
			sprintf(cbuf, "TDP_%i_%i_POS", set, point_counter);
		}
		
	}
	bool SecondSetEnabled;
	int changeoveranim;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "TDP_SECONDSET_ENABLED");
	if (!oapiReadItem_bool(fh, cbuf, SecondSetEnabled)) {SecondSetEnabled = false;}
	if (SecondSetEnabled) {
		sprintf(cbuf, "TDP_CHANGEOVER_ANIM");
		if (!oapiReadItem_int(fh, cbuf, changeoveranim)) { changeoveranim = -1; };
	}
	else {
		changeoveranim = -1;
	}
	
	EnableSecondSet(SecondSetEnabled);
	if (changeoveranim >= 0) {
		SetChangeOverAnimation(changeoveranim);
	}
	if (GetPointsCount(1) >= 3) {
		SetCurrentSet(1);
	}
	LogV("Parsing TouchDown Points Section Completed");
	return;
}
void TouchdownPointsManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------TOUCHDOWNPOINTS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT set = 1; set < 3; set++) {
		if (set == 2) {
			char cbuf[256] = { '\0' };
			bool set2Enabled = IsSecondSetEnabled();
			if (set2Enabled) {
				sprintf(cbuf, "TDP_SECONDSET_ENABLED");
				oapiWriteItem_bool(fh, cbuf, IsSecondSetEnabled());
				sprintf(cbuf, "TDP_CHANGEOVER_ANIM");
				oapiWriteItem_int(fh, cbuf, GetChangeOverAnimation());
			}
			
		}
		for (UINT i = 0; i < GetPointsCount(set); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "TDP_%i_%i_POS", set, i);
			oapiWriteItem_vec(fh, cbuf, GetPointPos(set, i));
			sprintf(cbuf, "TDP_%i_%i_STIFFNESS",set,i);
			oapiWriteItem_float(fh, cbuf, GetPointStiffnes(set, i));
			sprintf(cbuf, "TDP_%i_%i_DAMPING", set, i);
			oapiWriteItem_float(fh, cbuf, GetPointDamping(set, i));
			sprintf(cbuf, "TDP_%i_%i_MU",set,i);
			oapiWriteItem_float(fh, cbuf, GetPointMu(set, i));
			sprintf(cbuf, "TDP_%i_%i_MULNG",set,i);
			oapiWriteItem_float(fh, cbuf, GetPointMuLng(set, i));
			oapiWriteLine(fh, " ");
		}
		
		
	}
	
	return;
}

UINT TouchdownPointsManager::GetSetPointsCount(UINT set) {
	if (set == 1) {
		return set1.size();
	}
	else {
		return set2.size();
	}
}
vector<TOUCHDOWNVTX> TouchdownPointsManager::GetSet(UINT set) {
	if (set == 1) {
		return set1;
	}
	else {
		return set2;
	}
}
VECTOR3 TouchdownPointsManager::GetPointPos(UINT set, UINT idx) {
	if (set == 1) {
		return set1[idx].pos;
	}
	else {
		return set2[idx].pos;
	}
}
double TouchdownPointsManager::GetPointStiffnes(UINT set, UINT idx) {
	if (set == 1) {
		return set1[idx].stiffness;
	}
	else {
		return set2[idx].stiffness;
	}
}
double TouchdownPointsManager::GetPointDamping(UINT set, UINT idx) {
	if (set == 1) {
		return set1[idx].damping;
	}
	else {
		return set2[idx].damping;
	}
}
double TouchdownPointsManager::GetPointMu(UINT set, UINT idx) {
	if (set == 1) {
		return set1[idx].mu;
	}
	else {
		return set2[idx].mu;
	}
}
double TouchdownPointsManager::GetPointMuLng(UINT set, UINT idx) {
	if (set == 1) {
		return set1[idx].mu_lng;
	}
	else {
		return set2[idx].mu_lng;
	}
}
void TouchdownPointsManager::AddPoint(UINT set, TOUCHDOWNVTX tdvtx) {
	if (set == 1) {
		set1.push_back(tdvtx);
	}
	else {
		set2.push_back(tdvtx);
	}
}
void TouchdownPointsManager::RemovePoint(UINT set, UINT idx) {
	if (set == 1) {
		set1.erase(set1.begin() + idx);
	}
	else {
		set2.erase(set2.begin() + idx);
	}
}
void TouchdownPointsManager::EnableSecondSet(bool enable) {
	Set2Enabled = enable;
	if (!enable) { state_chk = NULL; }
}
void TouchdownPointsManager::SetChangeOverAnimation(anim_idx a_idx) {
	change_anim_idx = a_idx;
	state_chk = VB1->AnimMng->anim_defs[a_idx].state_ptr;
	return;
}
UINT TouchdownPointsManager::GetChangeOverAnimation() {
	return change_anim_idx;
}
bool TouchdownPointsManager::IsSecondSetEnabled() {
	return Set2Enabled;
}
void TouchdownPointsManager::ApplySet(UINT set) {
	vector<TOUCHDOWNVTX> applyset;
	if (set == 1) {
		applyset = set1;
	}
	else {
		applyset = set2;
	}
	DWORD ntdvtx = applyset.size();
	if (ntdvtx < 3) { return; }
	TOUCHDOWNVTX* tdvtx = new TOUCHDOWNVTX[ntdvtx];
	for (UINT i = 0; i < ntdvtx; i++) {
		tdvtx[i] = applyset[i];
	}
	VB1->SetTouchdownPoints(tdvtx, ntdvtx);
	delete[] tdvtx;
}

void TouchdownPointsManager::ClearSet(UINT set) {
	if (set == 1) {
		set1.clear();
	}
	else {
		set2.clear();
	}
	return;
}

void TouchdownPointsManager::SetCurrentSet(UINT set) {
	if (set == 1) {
		CurrentSet = 1;
		ApplySet(1);
	}
	else {
		CurrentSet = 2;
		ApplySet(2);
	}
}
UINT TouchdownPointsManager::GetCurrentSet() {
	return CurrentSet;
}
UINT TouchdownPointsManager::GetPointsCount(UINT set) {
	if (set == 1) {
		return set1.size();
	}
	else {
		return set2.size();
	}
}
void TouchdownPointsManager::TouchDownPointsPreStep(double simt, double simdt, double mjd) {
	if (!IsSecondSetEnabled()||state_chk==NULL) { return; }
	if (*state_chk> 0.999) {
		if (GetCurrentSet() == 1) {
			SetCurrentSet(2);
		}
	}
	else {
		if (GetCurrentSet() == 2) {
			SetCurrentSet(1);
		}
	}
	
	return;
}
void TouchdownPointsManager::Clear() {
	LogV("Clearing TDP");
	ClearSet(1);
	ClearSet(2);
	LogV("Clearing TDP Completed");
	return;
}