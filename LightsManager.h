#pragma once

struct BEACONS_DEF {
	string name;
	VECTOR3* pos;
	VECTOR3 *col;
	BEACONLIGHTSPEC *bs;
	BEACONS_DEF() {
		name.clear();
		bs = NULL;
		pos = NULL;
		col = NULL;
	}
};

struct POINTLIGHTS_DEF {
	string name;
	LightEmitter* LE;
	COLOUR4 diffuse, specular, ambient;
	POINTLIGHTS_DEF() {
		name.clear();
		LE = NULL;
		diffuse.r = 0.9;
		diffuse.g = 0.8;
		diffuse.b = 1;
		diffuse.a = 0;
		specular = diffuse;
		ambient.r = 0;
		ambient.g = 0;
		ambient.b = 0;
		ambient.a = 0;
	}
};
class LightsManager {
public:
	LightsManager(VesselBuilder1 *_VB1);
	~LightsManager();
	VesselBuilder1 *VB1;
	vector<BEACONS_DEF>beacons_def;
	UINT AddBeaconDef();
	UINT AddBeaconDef(string name, DWORD shape, VECTOR3 pos, VECTOR3 col, double size, double falloff, double period, double duration, double tofs);
	void DeleteBeaconDef(def_idx d_idx);
	void SetBeaconName(def_idx d_idx, string newName);
	string GetBeaconName(def_idx d_idx);
	DWORD GetBeaconShape(def_idx d_idx);
	void SetBeaconShape(def_idx d_idx, DWORD newshape);
	VECTOR3 GetBeaconPos(def_idx d_idx);
	void SetBeaconPos(def_idx d_idx, VECTOR3 newpos);
	void SetBeaconCol(def_idx d_idx, VECTOR3 newcol);
	VECTOR3 GetBeaconCol(def_idx d_idx);
	void SetBeaconSize(def_idx d_idx, double newSize);
	double GetBeaconSize(def_idx d_idx);
	void SetBeaconFalloff(def_idx d_idx, double newFalloff);
	double GetBeaconFalloff(def_idx d_idx);
	void SetBeaconPeriod(def_idx d_idx, double newPeriod);
	double GetBeaconPeriod(def_idx d_idx);
	void SetBeaconDuration(def_idx d_idx, double newDuration);
	double GetBeaconDuration(def_idx d_idx);
	void SetBeaconTofs(def_idx d_idx, double newTofs);
	double GetBeaconTofs(def_idx d_idx);
	bool AllBcnsActive;
	void ToggleBeaconActive(def_idx d_idx);
	void ToggleAllBeaconsActive();
	void ActivateAllBeacons(bool activate);
	bool AreAllBeaconsActive();
	bool IsBeaconActive(def_idx d_idx);
	void GetBeaconParams(def_idx d_idx, DWORD &shape, VECTOR3 &pos, VECTOR3 &col, double &size, double &falloff, double &period, double &duration, double &tofs);
	UINT GetBeaconCount();
	void ClearBeaconsDefs();
	
	vector<POINTLIGHTS_DEF> pointlights_def;
	UINT AddPointLightDef();
	UINT AddPointLightDef(string name, VECTOR3 position, VECTOR3 attenuation, double range, COLOUR4 diffuse, COLOUR4 specular, COLOUR4 ambient);
	void DeletePointLight(def_idx d_idx);
	void SetPointLightName(def_idx d_idx, string newname);
	string GetPointLightName(def_idx d_idx);
	double GetPointLightRange(def_idx d_idx);
	void SetPointLightRange(def_idx d_idx, double newRange);
	VECTOR3 GetPointLightAttenuation(def_idx d_idx);
	void SetPointLightAttenuation(def_idx d_idx, VECTOR3 attenuation);
	VECTOR3 GetPointLightPosition(def_idx d_idx);
	void SetPointLightPosition(def_idx d_idx, VECTOR3 newPos);
	UINT GetPointLightsCount();

	

	


























	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	
};
