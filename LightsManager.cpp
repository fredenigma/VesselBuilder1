#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "LightsManager.h"

LightsManager::LightsManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	beacons_def.clear();
	AllBcnsActive = false;
	pointlights_def.clear();
	return;
}
LightsManager::~LightsManager() {
	VB1 = NULL;

}
UINT LightsManager::AddBeaconDef() {
	UINT index = beacons_def.size();
	char namebuf[256] = { '\0' };
	sprintf(namebuf, "Beacon_%i", index);
	string name(namebuf);
	return AddBeaconDef(name, BEACONSHAPE_COMPACT, _V(0, 0, 0), _V(1, 1, 1), 0.4, 0.5, 1.5, 0.1, 0);
}
UINT LightsManager::AddBeaconDef(string name,DWORD shape, VECTOR3 _pos, VECTOR3 _col, double size, double falloff, double period, double duration, double tofs) {
	BEACONS_DEF bd = BEACONS_DEF();
	bd.pos = new VECTOR3;
	bd.col = new VECTOR3;
	*bd.pos = _pos;
	*bd.col = _col;
	bd.name = name;
	bd.bs = new BEACONLIGHTSPEC;
	bd.bs->shape = shape;
	bd.bs->col = bd.col;
	bd.bs->pos = bd.pos;
	bd.bs->size = size;
	bd.bs->falloff = falloff;
	bd.bs->period = period;
	bd.bs->duration = duration;
	bd.bs->tofs = tofs;
	//bd.bs->active = true;
	bd.bs->active = false;
	UINT index = beacons_def.size();
	beacons_def.push_back(bd);
	VB1->AddBeacon(bd.bs);
	return index;
}
void LightsManager::DeleteBeaconDef(def_idx d_idx) {
	VB1->DelBeacon(beacons_def[d_idx].bs);
	delete beacons_def[d_idx].bs;
	delete beacons_def[d_idx].col;
	delete beacons_def[d_idx].pos;
	beacons_def.erase(beacons_def.begin() + d_idx);
	return;
}
void LightsManager::SetBeaconName(def_idx d_idx, string newName) {
	beacons_def[d_idx].name = newName;
	return;
}
string LightsManager::GetBeaconName(def_idx d_idx) {
	return beacons_def[d_idx].name;
}
DWORD LightsManager::GetBeaconShape(def_idx d_idx) {
	return beacons_def[d_idx].bs->shape;
}
void LightsManager::SetBeaconShape(def_idx d_idx, DWORD newshape) {
	beacons_def[d_idx].bs->shape = newshape;
	return;
}
VECTOR3 LightsManager::GetBeaconPos(def_idx d_idx) {
	VECTOR3 pos = *beacons_def[d_idx].bs->pos;
	return pos;
}
void LightsManager::SetBeaconPos(def_idx d_idx, VECTOR3 newpos) {
	*beacons_def[d_idx].pos = newpos;
	//beacons_def[d_idx].bs->pos = &pos;
	return;
}
void LightsManager::SetBeaconCol(def_idx d_idx, VECTOR3 newcol) {
	*beacons_def[d_idx].col = newcol;
	//beacons_def[d_idx].bs->col = &col;
	return;
}
VECTOR3 LightsManager::GetBeaconCol(def_idx d_idx) {
	VECTOR3 col = *beacons_def[d_idx].bs->col;
	return col;
}
void LightsManager::SetBeaconSize(def_idx d_idx, double newSize) {
	beacons_def[d_idx].bs->size = newSize;
	return;
}
double LightsManager::GetBeaconSize(def_idx d_idx) {
	return beacons_def[d_idx].bs->size;
}
void LightsManager::SetBeaconFalloff(def_idx d_idx, double newFalloff) {
	beacons_def[d_idx].bs->falloff = newFalloff;
	return;
}
double LightsManager::GetBeaconFalloff(def_idx d_idx) {
	return beacons_def[d_idx].bs->falloff;
}
void LightsManager::SetBeaconPeriod(def_idx d_idx, double newPeriod) {
	beacons_def[d_idx].bs->period = newPeriod;
	return;
}
double LightsManager::GetBeaconPeriod(def_idx d_idx) {
	return beacons_def[d_idx].bs->period;
}
void LightsManager::SetBeaconDuration(def_idx d_idx, double newDuration) {
	beacons_def[d_idx].bs->duration = newDuration;
	return;
}
double LightsManager::GetBeaconDuration(def_idx d_idx) {
	return beacons_def[d_idx].bs->duration;
}
void LightsManager::SetBeaconTofs(def_idx d_idx, double newTofs) {
	beacons_def[d_idx].bs->tofs = newTofs;
	return;
}
double LightsManager::GetBeaconTofs(def_idx d_idx) {
	return beacons_def[d_idx].bs->tofs;
}
void LightsManager::ToggleBeaconActive(def_idx d_idx) {
	bool Active = beacons_def[d_idx].bs->active;
	beacons_def[d_idx].bs->active = !Active;
	return;
}
bool LightsManager::IsBeaconActive(def_idx d_idx) {
	return beacons_def[d_idx].bs->active;
}
void LightsManager::GetBeaconParams(def_idx d_idx, DWORD &shape, VECTOR3 &pos, VECTOR3 &col, double &size, double &falloff, double &period, double &duration, double &tofs) {
	shape = GetBeaconShape(d_idx);
	pos = GetBeaconPos(d_idx);
	col = GetBeaconCol(d_idx);
	size = GetBeaconSize(d_idx);
	falloff = GetBeaconFalloff(d_idx);
	period = GetBeaconPeriod(d_idx);
	duration = GetBeaconDuration(d_idx);
	tofs = GetBeaconTofs(d_idx);
	return;
}
UINT LightsManager::GetBeaconCount() {
	return beacons_def.size();
}
void LightsManager::ClearBeaconsDefs() {
	VB1->ClearBeacons();
	for (UINT i = 0; i < beacons_def.size(); i++) {
		delete beacons_def[i].bs;
		delete beacons_def[i].pos;
		delete beacons_def[i].col;
	}
	beacons_def.clear();
	return;
}

void LightsManager::ToggleAllBeaconsActive() {
	if (!AllBcnsActive) {
		for (UINT i = 0; i < GetBeaconCount(); i++) {
			beacons_def[i].bs->active = true;
		}
		AllBcnsActive = true;
	}
	else {
		for (UINT i = 0; i < GetBeaconCount(); i++) {
			beacons_def[i].bs->active = false;
		}
		AllBcnsActive = false;
	}
	return;	
}
bool LightsManager::AreAllBeaconsActive() {
	return AllBcnsActive;
}
void LightsManager::ActivateAllBeacons(bool activate) {
	for (UINT i = 0; i < GetBeaconCount(); i++) {
		beacons_def[i].bs->active = activate;
	}
	AllBcnsActive = activate;

}


UINT LightsManager::AddPointLightDef() {
	UINT index = pointlights_def.size();
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "PointLight_%i", index);
	string name(cbuf);
	COLOUR4 col_d = { 0.9,0.8,1,0 };
	COLOUR4 col_s = { 0.9,0.8,1,0 };
	COLOUR4 col_a = { 0,0,0,0 };
	return AddPointLightDef(name, _V(0, 0, 0), _V(1e-3, 0, 2e-3), 200, col_d, col_s, col_a);
}
UINT LightsManager::AddPointLightDef(string name, VECTOR3 position, VECTOR3 attenuation, double range, COLOUR4 diffuse, COLOUR4 specular, COLOUR4 ambient) {
	POINTLIGHTS_DEF pld = POINTLIGHTS_DEF();
	pld.name = name;
	pld.diffuse = diffuse;
	pld.specular = specular;
	pld.ambient = ambient;
	//pld.PL = //new PointLight(VB1->GetHandle(), position, range, attenuation.x, attenuation.y, attenuation.z, diffuse, specular, ambient);
	pld.LE = VB1->AddPointLight(position, range, attenuation.x, attenuation.y, attenuation.z, diffuse, specular, ambient);
	UINT index = pointlights_def.size();
	pointlights_def.push_back(pld);
	return index;
}
void LightsManager::DeletePointLight(def_idx d_idx) {
	VB1->DelLightEmitter(pointlights_def[d_idx].LE);
	pointlights_def.erase(pointlights_def.begin() + d_idx);
	return;
}
void LightsManager::SetPointLightName(def_idx d_idx, string newname) {
	pointlights_def[d_idx].name = newname;
	return;
}
string LightsManager::GetPointLightName(def_idx d_idx) {
	return pointlights_def[d_idx].name;
}
double LightsManager::GetPointLightRange(def_idx d_idx) {
	return ((PointLight*)pointlights_def[d_idx].LE)->GetRange();
}
void LightsManager::SetPointLightRange(def_idx d_idx, double newRange) {
	((PointLight*)pointlights_def[d_idx].LE)->SetRange(newRange);
	return;
}
VECTOR3 LightsManager::GetPointLightAttenuation(def_idx d_idx) {
	VECTOR3 att;
	const double *att_d=((PointLight*)pointlights_def[d_idx].LE)->GetAttenuation();
	att.x = att_d[0];
	att.y = att_d[1];
	att.z = att_d[2];
	return att;
}
void LightsManager::SetPointLightAttenuation(def_idx d_idx, VECTOR3 attenuation) {
	((PointLight*)pointlights_def[d_idx].LE)->SetAttenuation(attenuation.x, attenuation.y, attenuation.z);
	return;
}
VECTOR3 LightsManager::GetPointLightPosition(def_idx d_idx) {
	return pointlights_def[d_idx].LE->GetPosition();
}
void LightsManager::SetPointLightPosition(def_idx d_idx, VECTOR3 newPos) {
	pointlights_def[d_idx].LE->SetPosition(newPos);
	return;
}
UINT LightsManager::GetPointLightsCount() {
	return pointlights_def.size();
}





























void LightsManager::ParseCfgFile(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	UINT beacon_counter = 0;
	int id;
	sprintf(cbuf, "BCN_%i_ID", beacon_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "BCN_%i_NAME", beacon_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		DWORD shape;
		VECTOR3 pos, col;
		double size, falloff, period, duration, tofs;
		sprintf(cbuf, "BCN_%i_SHAPE", beacon_counter);
		int sh;
		oapiReadItem_int(fh, cbuf, sh);
		shape = (DWORD)sh;
		sprintf(cbuf, "BCN_%i_POS", beacon_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "BCN_%i_COL", beacon_counter);
		oapiReadItem_vec(fh, cbuf, col);
		sprintf(cbuf, "BCN_%i_SIZE", beacon_counter);
		oapiReadItem_float(fh, cbuf, size);
		sprintf(cbuf, "BCN_%i_FALLOFF", beacon_counter);
		oapiReadItem_float(fh, cbuf, falloff);
		sprintf(cbuf, "BCN_%i_PERIOD", beacon_counter);
		oapiReadItem_float(fh, cbuf, period);
		sprintf(cbuf, "BCN_%i_DURATION", beacon_counter);
		oapiReadItem_float(fh, cbuf, duration);
		sprintf(cbuf, "BCN_%i_TOFS", beacon_counter);
		oapiReadItem_float(fh, cbuf, tofs);
		AddBeaconDef(name, shape, pos, col, size, falloff, period, duration, tofs);
		beacon_counter++;
		sprintf(cbuf, "BCN_%i_ID", beacon_counter);
	}
	
	
	
	
	
	
	
	
	

	return;
}
void LightsManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------LIGHTS: BEACONS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < GetBeaconCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "BCN_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "BCN_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetBeaconName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "BCN_%i_SHAPE", i);
		oapiWriteItem_int(fh, cbuf, GetBeaconShape(i));
		sprintf(cbuf, "BCN_%i_POS", i);
		oapiWriteItem_vec(fh, cbuf, GetBeaconPos(i));
		sprintf(cbuf, "BCN_%i_COL", i);
		oapiWriteItem_vec(fh, cbuf, GetBeaconCol(i));
		sprintf(cbuf, "BCN_%i_SIZE", i);
		oapiWriteItem_float(fh, cbuf, GetBeaconSize(i));
		sprintf(cbuf, "BCN_%i_FALLOFF", i);
		oapiWriteItem_float(fh, cbuf, GetBeaconFalloff(i));
		sprintf(cbuf, "BCN_%i_PERIOD", i);
		oapiWriteItem_float(fh, cbuf, GetBeaconPeriod(i));
		sprintf(cbuf, "BCN_%i_DURATION", i);
		oapiWriteItem_float(fh, cbuf, GetBeaconDuration(i));
		sprintf(cbuf, "BCN_%i_TOFS", i);
		oapiWriteItem_float(fh, cbuf, GetBeaconTofs(i));
		oapiWriteLine(fh, " ");
	}
	return;
}
