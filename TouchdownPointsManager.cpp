#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "TouchdownPointsManager.h"

TouchdownPointsManager::TouchdownPointsManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	set1.clear();
	set2.clear();
	change_anim_idx = (UINT)-1;
	Set2Enabled = false;
	
	return;
}
TouchdownPointsManager::~TouchdownPointsManager() {
	VB1 = NULL;
	return;
}
void TouchdownPointsManager::ParseCfgFile(FILEHANDLE fh) {
	return;
}
void TouchdownPointsManager::WriteCfg(FILEHANDLE fh) {
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
		set2.erase(set1.begin() + idx);
	}
}
void TouchdownPointsManager::EnableSecondSet(bool enable) {
	Set2Enabled = enable;
}
void TouchdownPointsManager::SetChangeOverAnimation(anim_idx a_idx) {
	change_anim_idx = a_idx;
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