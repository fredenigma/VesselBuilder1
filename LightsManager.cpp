#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "AttachmentManager.h"
#include "LightsManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

LightsManager::LightsManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	beacons_def.clear();
	AllBcnsActive = false;
	lights_def.clear();
	CurrentLight = 0;
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
void LightsManager::ActivateBeacon(def_idx d_idx, bool activate) {
	beacons_def[d_idx].bs->active = activate;
	return;
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


UINT LightsManager::AddLightDef(LightEmitter::TYPE type) {
	UINT index = lights_def.size();
	char cbuf[256] = { '\0' };
	if (type == LightEmitter::TYPE::LT_POINT) {
		sprintf(cbuf, "PointLight_%i", index);
	}
	else if (type == LightEmitter::TYPE::LT_SPOT) {
		sprintf(cbuf, "SpotLight_%i", index);
	}
	
	string name(cbuf);
	COLOUR4 col_d = { 0.9,0.8,1,0 };
	COLOUR4 col_s = { 0.9,0.8,1,0 };
	COLOUR4 col_a = { 0,0,0,0 };
	return AddLightDef(type, name, _V(0, 0, 0), LightEmitter::VISIBILITY::VIS_EXTERNAL,_V(0,0,1), _V(1e-3, 0, 2e-3), 200,45*RAD,90*RAD, col_d, col_s, col_a);
}
UINT LightsManager::AddLightDef(LightEmitter::TYPE type, COLOUR4 diffuse, COLOUR4 specular, COLOUR4 ambient) {
	UINT index = lights_def.size();
	char cbuf[256] = { '\0' };
	if (type == LightEmitter::TYPE::LT_POINT) {
		sprintf(cbuf, "PointLight_%i", index);
	}
	else if (type == LightEmitter::TYPE::LT_SPOT) {
		sprintf(cbuf, "SpotLight_%i", index);
	}
	string name(cbuf);
	return AddLightDef(type, name, _V(0, 0, 0),LightEmitter::VISIBILITY::VIS_EXTERNAL, _V(0,0,1),_V(1e-3, 0, 2e-3), 200,45*RAD,90*RAD, diffuse, specular, ambient);
}
UINT LightsManager::AddLightDef(LightEmitter::TYPE type, string name, VECTOR3 position, LightEmitter::VISIBILITY vis, VECTOR3 direction, VECTOR3 attenuation, double range, double umbra, double penumbra, COLOUR4 diffuse, COLOUR4 specular, COLOUR4 ambient) {
	LIGHTS_DEF ld = LIGHTS_DEF();
	ld.name = name;
	*ld.pos_ptr = position;
	*ld.dir_ptr = direction;
	if (type == LightEmitter::TYPE::LT_POINT) {
		ld.LE = VB1->AddPointLight(position, range, attenuation.x, attenuation.y, attenuation.z, diffuse, specular, ambient);
		ld.LE->SetPositionRef(ld.pos_ptr);
	}
	else if (type == LightEmitter::TYPE::LT_SPOT) {
		ld.LE = VB1->AddSpotLight(position, direction, range, attenuation.x, attenuation.y, attenuation.z, umbra, penumbra, diffuse, specular, ambient);
		ld.LE->SetPositionRef(ld.pos_ptr);
		ld.LE->SetDirectionRef(ld.dir_ptr);
	}
	ld.LE->Activate(false);
	
	
	UINT index = lights_def.size();
	lights_def.push_back(ld);
	return index;
}
void LightsManager::DeleteLight(def_idx d_idx) {
	VB1->DelLightEmitter(lights_def[d_idx].LE);
	delete lights_def[d_idx].pos_ptr;
	delete lights_def[d_idx].dir_ptr;
	lights_def.erase(lights_def.begin() + d_idx);
	return;
}
void LightsManager::SetLightName(def_idx d_idx, string newname) {
	lights_def[d_idx].name = newname;
	return;
}
string LightsManager::GetLightName(def_idx d_idx) {
	return lights_def[d_idx].name;
}
LightEmitter::TYPE LightsManager::GetLightType(def_idx d_idx) {
	return lights_def[d_idx].LE->GetType();
}
double LightsManager::GetLightRange(def_idx d_idx) {
	if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_POINT) {
		return ((PointLight*)lights_def[d_idx].LE)->GetRange();
	}
	else if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_SPOT) {
		return ((SpotLight*)lights_def[d_idx].LE)->GetRange();
	}
	return 0;
}
LightEmitter::VISIBILITY LightsManager::GetLightVisibility(def_idx d_idx) {
	return lights_def[d_idx].LE->GetVisibility();
}
void LightsManager::SetLightVisibility(def_idx d_idx, LightEmitter::VISIBILITY vis) {
	lights_def[d_idx].LE->SetVisibility(vis);
	return;
}
void LightsManager::SetLightRange(def_idx d_idx, double newRange) {
	if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_POINT) {
		((PointLight*)lights_def[d_idx].LE)->SetRange(newRange);
	}
	else if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_SPOT) {
		((SpotLight*)lights_def[d_idx].LE)->SetRange(newRange);
	}
	
	return;
}
VECTOR3 LightsManager::GetLightAttenuation(def_idx d_idx) {
	VECTOR3 att;
	if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_POINT) {
		const double *att_d = ((PointLight*)lights_def[d_idx].LE)->GetAttenuation();
		att.x = att_d[0];
		att.y = att_d[1];
		att.z = att_d[2];
	}
	else if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_SPOT) {
		const double *att_d = ((SpotLight*)lights_def[d_idx].LE)->GetAttenuation();
		att.x = att_d[0];
		att.y = att_d[1];
		att.z = att_d[2];
	}
	return att;
}
void LightsManager::SetLightAttenuation(def_idx d_idx, VECTOR3 attenuation) {
	if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_POINT) {
		((PointLight*)lights_def[d_idx].LE)->SetAttenuation(attenuation.x, attenuation.y, attenuation.z);
	}
	else if (lights_def[d_idx].LE->GetType() == LightEmitter::TYPE::LT_SPOT) {
		((SpotLight*)lights_def[d_idx].LE)->SetAttenuation(attenuation.x, attenuation.y, attenuation.z);
	}
	
	return;
}
VECTOR3 LightsManager::GetLightPosition(def_idx d_idx) {
	return lights_def[d_idx].LE->GetPosition();
}
void LightsManager::SetLightPosition(def_idx d_idx, VECTOR3 newPos) {
	*lights_def[d_idx].pos_ptr = newPos;
	//lights_def[d_idx].LE->SetPosition(newPos);
	return;
}
COLOUR4 LightsManager::GetLightDiffuseColour(def_idx d_idx) {
	return lights_def[d_idx].LE->GetDiffuseColour();
}
COLOUR4 LightsManager::GetLightAmbientColour(def_idx d_idx) {
	return lights_def[d_idx].LE->GetAmbientColour();
}
COLOUR4 LightsManager::GetLightSpecularColour(def_idx d_idx) {
	return lights_def[d_idx].LE->GetSpecularColour();
}
void LightsManager::GetLightColours(def_idx d_idx, COLOUR4 &diffuse, COLOUR4 &ambient, COLOUR4 &specular) {
	diffuse = GetLightDiffuseColour(d_idx);
	ambient = GetLightAmbientColour(d_idx);
	specular = GetLightSpecularColour(d_idx);
	return;
}
void LightsManager::SetLightDirection(def_idx d_idx, VECTOR3 newDirection) {
	*lights_def[d_idx].dir_ptr = newDirection;
	//lights_def[d_idx].LE->SetDirection(newDirection);
	return;
}
VECTOR3 LightsManager::GetLightDirection(def_idx d_idx) {
	return lights_def[d_idx].LE->GetDirection();
}
void LightsManager::SetLightAperture(def_idx d_idx, double umbra, double penumbra) {
	if (GetLightType(d_idx) == LightEmitter::TYPE::LT_SPOT) {
		((SpotLight*)lights_def[d_idx].LE)->SetAperture(umbra, penumbra);
	}
	return;
}
void LightsManager::GetLightAperture(def_idx d_idx, double &umbra, double &penumbra) {
	if (GetLightType(d_idx) == LightEmitter::TYPE::LT_SPOT) {
		umbra = ((SpotLight*)lights_def[d_idx].LE)->GetUmbra();
		penumbra = ((SpotLight*)lights_def[d_idx].LE)->GetPenumbra();
	}
	else {
		umbra = PI;
		penumbra = PI;
	}
	return;
}
void LightsManager::SetLightToAttachment(def_idx d_idx, bool set, def_idx att_idx) {
	lights_def[d_idx].AttachedtoAttachment = set;
	if (set) {
		lights_def[d_idx].Att_idx = att_idx;
		lights_def[d_idx].LE->SetPositionRef(VB1->AttMng->GetAttDefPosPtr(att_idx));
		lights_def[d_idx].LE->SetDirectionRef(VB1->AttMng->GetAttDefDirPtr(att_idx));
	}
	else {
		lights_def[d_idx].Att_idx = (UINT)-1;
		lights_def[d_idx].LE->SetPositionRef(lights_def[d_idx].pos_ptr);
		lights_def[d_idx].LE->SetDirectionRef(lights_def[d_idx].dir_ptr);
	}
	return;
}
bool LightsManager::IsLightAttachedToAttachment(def_idx d_idx) {
	return lights_def[d_idx].AttachedtoAttachment;
}
def_idx LightsManager::GetLigthAttachment(def_idx d_idx) {
	return lights_def[d_idx].Att_idx;
}
bool LightsManager::IsLightActive(def_idx d_idx) {
	return lights_def[d_idx].LE->IsActive();
}
void LightsManager::ActivateLight(def_idx d_idx, bool active) {
	lights_def[d_idx].LE->Activate(active);
	return;
}
void LightsManager::ToggleLight(def_idx d_idx) {
	bool active = IsLightActive(d_idx);
	ActivateLight(d_idx, !active);
	return;
}
UINT LightsManager::GetLightsCount() {
	return lights_def.size();
}



void LightsManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Lights Section");
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
	
	UINT lights_counter = 0;
	for (UINT i = 0; i < 256; i++) { cbuf[i] = '\0'; }
	sprintf(cbuf, "LIGHT_%i_ID", lights_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		sprintf(cbuf, "LIGHT_%i_NAME", lights_counter);
		char namebuf[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		int type_int;
		LightEmitter::TYPE type;
		VECTOR3 col_d, col_a, col_s,pos;
		COLOUR4 diff, amb, spec;
		double range;
		int vis_int;
		LightEmitter::VISIBILITY vis;
		VECTOR3 attenuation;
		bool hasatt;
		int attachment;
		VECTOR3 direction = _V(0,0,0);
		VECTOR3 aperture = _V(0, 0, 0);
		sprintf(cbuf, "LIGHT_%i_TYPE", lights_counter);
		oapiReadItem_int(fh, cbuf, type_int);
		type = (LightEmitter::TYPE)type_int;
		sprintf(cbuf, "LIGHT_%i_DCOL", lights_counter);
		oapiReadItem_vec(fh, cbuf, col_d);
		diff.r = col_d.x;
		diff.g = col_d.y;
		diff.b = col_d.z;
		diff.a = 0;
		sprintf(cbuf, "LIGHT_%i_ACOL", lights_counter);
		oapiReadItem_vec(fh, cbuf, col_a);
		amb.r = col_a.x;
		amb.g = col_a.y;
		amb.b = col_a.z;
		amb.a = 0;
		sprintf(cbuf, "LIGHT_%i_SCOL", lights_counter);
		oapiReadItem_vec(fh, cbuf, col_s);
		spec.r = col_s.x;
		spec.g = col_s.y;
		spec.b = col_s.z;
		spec.a = 0;
		sprintf(cbuf, "LIGHT_%i_POSITION", lights_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "LIGHT_%i_RANGE", lights_counter);
		oapiReadItem_float(fh, cbuf, range);
		sprintf(cbuf, "LIGHT_%i_VISIBILITY", lights_counter);
		oapiReadItem_int(fh, cbuf, vis_int);
		vis = (LightEmitter::VISIBILITY)vis_int;
		sprintf(cbuf, "LIGHT_%i_ATTENUATION", lights_counter);
		oapiReadItem_vec(fh, cbuf, attenuation);
		if (type == LightEmitter::TYPE::LT_SPOT) {
			sprintf(cbuf, "LIGHT_%i_DIRECTION", lights_counter);
			oapiReadItem_vec(fh, cbuf, direction);
			sprintf(cbuf, "LIGHT_%i_APERTURE", lights_counter);
			oapiReadItem_vec(fh, cbuf, aperture);
			aperture *= RAD;

		}
		sprintf(cbuf, "LIGHT_%i_HASATTACHMENT", lights_counter);
		if (!oapiReadItem_bool(fh, cbuf, hasatt)) {
			hasatt = false;
		}
		if (hasatt) {
			sprintf(cbuf, "LIGHT_%i_ATTACHMENT", lights_counter);
			oapiReadItem_int(fh, cbuf, attachment);
		}
		UINT index = AddLightDef(type, name, pos, vis, direction, attenuation, range, aperture.x, aperture.y, diff, spec, amb);
		if (hasatt) {
			SetLightToAttachment(index, true, attachment);
		}
		lights_counter++;
		sprintf(cbuf, "LIGHT_%i_ID", lights_counter);
	}
	
	LogV("Parsing Lights Section Completed, found %i beacons and %i light emitters",beacon_counter,lights_counter);

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

	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------LIGHTS: LIGHT EMITTERS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < GetLightsCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "LIGHT_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "LIGHT_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetLightName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "LIGHT_%i_TYPE", i);
		int type = (int)GetLightType(i);
		oapiWriteItem_int(fh, cbuf, type);
		sprintf(cbuf, "LIGHT_%i_DCOL", i);
		COLOUR4 col_d, col_a, col_s;
		GetLightColours(i, col_d, col_a, col_s);
		VECTOR3 col_dv, col_av, col_sv;
		col_dv.x = col_d.r;
		col_dv.y = col_d.g;
		col_dv.z = col_d.b;
		col_av.x = col_a.r;
		col_av.y = col_a.g;
		col_av.z = col_a.b;
		col_sv.x = col_s.r;
		col_sv.y = col_s.g;
		col_sv.z = col_s.b;
		oapiWriteItem_vec(fh, cbuf, col_dv);
		sprintf(cbuf, "LIGHT_%i_ACOL", i);
		oapiWriteItem_vec(fh, cbuf, col_av);
		sprintf(cbuf, "LIGHT_%i_SCOL", i);
		oapiWriteItem_vec(fh, cbuf, col_sv);
		sprintf(cbuf, "LIGHT_%i_POSITION", i);
		oapiWriteItem_vec(fh, cbuf, GetLightPosition(i));
		sprintf(cbuf, "LIGHT_%i_RANGE", i);
		oapiWriteItem_float(fh, cbuf, GetLightRange(i));
		sprintf(cbuf, "LIGHT_%i_VISIBILITY", i);
		oapiWriteItem_int(fh, cbuf, (int)GetLightVisibility(i));
		sprintf(cbuf, "LIGHT_%i_ATTENUATION", i);
		oapiWriteItem_vec(fh, cbuf, GetLightAttenuation(i));
		sprintf(cbuf, "LIGHT_%i_HASATTACHMENT", i);
		bool hasatt = IsLightAttachedToAttachment(i);
		oapiWriteItem_bool(fh, cbuf, hasatt);
		if (hasatt) {
			sprintf(cbuf, "LIGHT_%i_ATTACHMENT", i);
			oapiWriteItem_int(fh, cbuf, GetLigthAttachment(i));
		}
		if (type == 2) {
			sprintf(cbuf, "LIGHT_%i_DIRECTION", i);
			oapiWriteItem_vec(fh, cbuf, GetLightDirection(i));
			double umbra, penumbra;
			GetLightAperture(i, umbra, penumbra);
			VECTOR3 aperture = _V(umbra, penumbra, 0);
			aperture *= DEG;
			sprintf(cbuf, "LIGHT_%i_APERTURE", i);
			oapiWriteItem_vec(fh, cbuf, aperture);
		}
	}
	return;
}
int LightsManager::ConsumeLightsBufferedKey(DWORD key, bool down, char *kstate) {
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_L) {
		if (CurrentLight < GetLightsCount()) {
			ToggleLight(CurrentLight);
			CurrentLight++;
			if (CurrentLight == GetLightsCount()) {
				CurrentLight = 0;
			}
		}
		
		
		return 1;
	}
	return 0;
}
vector<UINT> LightsManager::GetLightsOn() {
	vector<UINT>on_count;
	on_count.clear();
	for (UINT i = 0; i < GetLightsCount(); i++) {
		if (IsLightActive(i)) {
			on_count.push_back(i);
		}
	}
	return on_count;
}
vector<UINT> LightsManager::GetBeaconsOn() {
	vector<UINT> on_count;
	on_count.clear();
	for (UINT i = 0; i < GetBeaconCount(); i++) {
		if (IsBeaconActive(i)) {
			on_count.push_back(i);
		}
	}
	return on_count;
}

void LightsManager::Clear() {
	ClearBeaconsDefs();
	VB1->ClearLightEmitters();
	for (UINT i = 0; i < GetLightsCount(); i++) {
		delete lights_def[i].pos_ptr;
		delete lights_def[i].dir_ptr;
	}
	lights_def.clear();
	
}