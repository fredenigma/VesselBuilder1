#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "GeneralSettingsManager.h"
#include "MET.h"

GeneralSettingsManager::GeneralSettingsManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	HUDTxt.clear();
	HudTxtenabled = false;
//	show_met = NOSHOW;
}
GeneralSettingsManager::~GeneralSettingsManager() {
	VB1 = NULL;
}
void GeneralSettingsManager::SetEmptyMass(double EM) {
	VB1->SetEmptyMass(EM);
	return;
}
double GeneralSettingsManager::GetEmptyMass() {
	return VB1->GetEmptyMass();
}
void GeneralSettingsManager::SetSize(double sz) {
	VB1->SetSize(sz);
	return;
}
double GeneralSettingsManager::GetSize() {
	return VB1->GetSize();
}
void GeneralSettingsManager::SetPMI(VECTOR3 PMI) {
	VB1->SetPMI(PMI);
	return;
}
VECTOR3 GeneralSettingsManager::GetPMI() {
	VECTOR3 PMI;
	VB1->GetPMI(PMI);
	return PMI;
}
void GeneralSettingsManager::SetCrossSections(VECTOR3 cs) {
	VB1->SetCrossSections(cs);
	return;
}
VECTOR3 GeneralSettingsManager::GetCrossSections() {
	VECTOR3 CS;
	VB1->GetCrossSections(CS);
	return CS;
}
void GeneralSettingsManager::SetGravityGradientDamping(double ggd) {
	VB1->SetGravityGradientDamping(ggd);
	return;
}
double GeneralSettingsManager::GetGravityGradientDamping() {
	return VB1->GetGravityGradientDamping();
}
void GeneralSettingsManager::SetRotDrag(VECTOR3 rd) {
	VB1->SetRotDrag(rd);
	return;
}
VECTOR3 GeneralSettingsManager::GetRotDrag() {
	VECTOR3 rd;
	VB1->GetRotDrag(rd);
	return rd;
}
void GeneralSettingsManager::EnableMET(bool enable) {
	VB1->Met->Enable(enable);
	return;
}
bool GeneralSettingsManager::IsMETEnabled() {
	return VB1->Met->Enabled();
}
/*GeneralSettingsManager::SHOWMET GeneralSettingsManager::GetShowMet() {
	return show_met;
}*/
//void GeneralSettingsManager::SetShowMet(GeneralSettingsManager::SHOWMET show) {
void GeneralSettingsManager::SetShowMet(bool show) {
	ShowInHUD = show;
	return;
}
void GeneralSettingsManager::SetHUDText(string _text) {
	HUDTxt = _text;
	return;
}
string GeneralSettingsManager::GetHUDText() {
	return HUDTxt;
}


void GeneralSettingsManager::Clear() {
	HUDTxt.clear();
//	VB1->Met->Enable(false);
	SetShowMet(false);
	
	return;
}