#pragma once

class GeneralSettingsManager {
public:
	GeneralSettingsManager(VesselBuilder1 *_VB1);
	~GeneralSettingsManager();
	VesselBuilder1 *VB1;
	void SetEmptyMass(double EM);
	double GetEmptyMass();
	void SetSize(double sz);
	double GetSize();
	void SetPMI(VECTOR3 PMI);
	VECTOR3 GetPMI();
	void SetCrossSections(VECTOR3 cs);
	VECTOR3 GetCrossSections();
	void SetGravityGradientDamping(double ggd);
	double GetGravityGradientDamping();
	void SetRotDrag(VECTOR3 rd);
	VECTOR3 GetRotDrag();
	void Clear();
	bool IsMETEnabled();
	void EnableMET(bool enable);
	bool ShowInHUD;
	bool GetShowMet() { return ShowInHUD; }
	//enum SHOWMET{NOSHOW,HUD,NOTE}show_met;
	//SHOWMET GetShowMet();
	//void SetShowMet(SHOWMET show);
	void SetShowMet(bool show);
	bool HudTxtenabled;
	bool HUDTextEnabled() { return HudTxtenabled; }
	void EnableHUDText(bool enable) { HudTxtenabled = enable; }
	void SetHUDText(string _text);
	string GetHUDText();
	string HUDTxt;
};