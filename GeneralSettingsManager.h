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
	
};