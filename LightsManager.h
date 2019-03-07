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

struct LIGHTS_DEF {
	string name;
	LightEmitter* LE;
	bool AttachedtoAttachment;
	def_idx Att_idx;
	VECTOR3* pos_ptr;
	VECTOR3* dir_ptr;
	LIGHTS_DEF() {
		name.clear();
		LE = NULL;
		pos_ptr = new VECTOR3;
		dir_ptr = new VECTOR3;
		AttachedtoAttachment = false;
		Att_idx = (UINT)-1;
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
	void ActivateBeacon(def_idx d_idx, bool activate);
	bool IsBeaconActive(def_idx d_idx);
	void GetBeaconParams(def_idx d_idx, DWORD &shape, VECTOR3 &pos, VECTOR3 &col, double &size, double &falloff, double &period, double &duration, double &tofs);
	UINT GetBeaconCount();
	void ClearBeaconsDefs();
	
	vector<LIGHTS_DEF> lights_def;
	UINT AddLightDef(LightEmitter::TYPE type);
	UINT AddLightDef(LightEmitter::TYPE type, COLOUR4 diffuse, COLOUR4 specular, COLOUR4 ambient);
	UINT AddLightDef(LightEmitter::TYPE type, string name, VECTOR3 position, LightEmitter::VISIBILITY vis, VECTOR3 direction, VECTOR3 attenuation, double range, double umbra, double penumbra, COLOUR4 diffuse, COLOUR4 specular, COLOUR4 ambient);
	void DeleteLight(def_idx d_idx);
	void SetLightName(def_idx d_idx, string newname);
	string GetLightName(def_idx d_idx);
	LightEmitter::TYPE GetLightType(def_idx d_idx);
	double GetLightRange(def_idx d_idx);
	void SetLightRange(def_idx d_idx, double newRange);
	LightEmitter::VISIBILITY GetLightVisibility(def_idx d_idx);
	void SetLightVisibility(def_idx d_idx, LightEmitter::VISIBILITY vis);
	VECTOR3 GetLightAttenuation(def_idx d_idx);
	void SetLightAttenuation(def_idx d_idx, VECTOR3 attenuation);
	VECTOR3 GetLightPosition(def_idx d_idx);
	void SetLightPosition(def_idx d_idx, VECTOR3 newPos);
	COLOUR4 GetLightDiffuseColour(def_idx d_idx);
	COLOUR4 GetLightAmbientColour(def_idx d_idx);
	COLOUR4 GetLightSpecularColour(def_idx d_idx);
	void GetLightColours(def_idx d_idx, COLOUR4 &diffuse, COLOUR4 &ambient, COLOUR4 &specular);
	void SetLightDirection(def_idx d_idx, VECTOR3 newDirection);
	VECTOR3 GetLightDirection(def_idx d_idx);
	void SetLightAperture(def_idx d_idx, double umbra, double penumbra);
	void GetLightAperture(def_idx d_idx, double &umbra, double &penumbra);
	void SetLightToAttachment(def_idx d_idx, bool set, def_idx att_idx);
	bool IsLightAttachedToAttachment(def_idx d_idx);
	def_idx GetLigthAttachment(def_idx d_idx);
	bool IsLightActive(def_idx d_idx);
	void ActivateLight(def_idx d_idx, bool active);
	void ToggleLight(def_idx d_idx);
	UINT GetLightsCount();

	int ConsumeLightsBufferedKey(DWORD key, bool down, char *kstate);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	UINT CurrentLight;
	vector<UINT>GetLightsOn();
	vector<UINT>GetBeaconsOn();

};
