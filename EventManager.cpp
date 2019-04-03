#include "VesselBuilder1.h"
#include "DialogControl.h"

#include "EventManager.h"
#include "MeshManager.h"
#include "AnimationManager.h"
#include "MET.h"
#include "ConfigurationManager.h"
#include <Windows.h>
#pragma comment (lib, "winmm.lib")

Event::Event(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	Trigger.Type = TRIGGER::TRIGGERTYPE::NULL_TRIG;
	Consumed = false;
	Enabled = true;
	return;
}
Event::~Event() {}
Event::TRIGGER::TRIGGERTYPE Event::GetTriggerType() {
	return Trigger.Type;
}
void Event::SetTrigger(TRIGGER _trig) {
	Trigger = _trig;
	return;
}
void Event::SetConsumed(bool set) {
	Consumed = set;
	return;
}
bool Event::IsEventConsumed() {
	return Consumed;
}
void Event::ConsumeEvent() {
	return;
}
void Event::Trig() {
	if (Trigger.repeat_mode == TRIGGER::REPEAT_MODE::ONCE) {
		if (!IsEventConsumed()) {
			ConsumeEvent();
			SetConsumed(true);
		}
	}
	else {
		ConsumeEvent();
	}
	return;
}
void Event::EventPreStep(double simt, double simdt, double mjd) {
	switch (Trigger.Type)
	{
	case TRIGGER::ALTITUDE:
	{
		double altitude = VB1->GetAltitude();
		if (Trigger.condition == TRIGGER::ABOVE) {
			if (altitude > Trigger.TriggerValue) {
				Trig();
			}
		}
		else if (Trigger.condition == TRIGGER::BELOW) {
			if (altitude < Trigger.TriggerValue) {
				Trig();
			}
		}
		break;
	}
	case TRIGGER::VELOCITY:
	{
		double ref_speed;
		if (Trigger.vel_mode == TRIGGER::GROUNDSPEED) {
			ref_speed = VB1->GetGroundspeed();
		}
		else if (Trigger.vel_mode == TRIGGER::VEL_MODE::AIRSPEED) {
			ref_speed = VB1->GetAirspeed();
		}
		else if (Trigger.vel_mode == TRIGGER::VEL_MODE::ORBITALSPEED) {
			VECTOR3 r_vel;
			VB1->GetRelativeVel(VB1->GetSurfaceRef(), r_vel);
			ref_speed = length(r_vel);
		}
		else if (Trigger.vel_mode == TRIGGER::MACH) {
			ref_speed = VB1->GetMachNumber();
		}
		if (Trigger.condition == TRIGGER::ABOVE) {
			if (ref_speed > Trigger.TriggerValue) {
				Trig();
			}
		}
		else if (Trigger.condition == TRIGGER::BELOW) {
			if (ref_speed < Trigger.TriggerValue) {
				Trig();
			}
		}
		break;
	}
	case TRIGGER::MAINFUELTANK_LEVEL:
	{
		double tank_level = (VB1->GetPropellantMass(VB1->GetDefaultPropellantResource())) / (VB1->GetPropellantMaxMass(VB1->GetDefaultPropellantResource()));
		if (tank_level <= Trigger.TriggerValue) {
			Trig();
		}
		break;
	}
	case TRIGGER::DYNPRESSURE:
	{
		double ref_p = VB1->GetDynPressure();
		if (Trigger.condition == TRIGGER::ABOVE) {
			if (ref_p > Trigger.TriggerValue) {
				Trig();
			}
		}
		else if (Trigger.condition == TRIGGER::BELOW) {
			if (ref_p < Trigger.TriggerValue) {
				Trig();
			}
		}
		break;
	}
	case TRIGGER::TIME:
	{
		double ref_time;
		if (Trigger.time_mode == TRIGGER::MJD) {
			ref_time = oapiGetSimMJD();
		}
		else if (Trigger.time_mode == TRIGGER::MET) {
			ref_time = VB1->GetMET();
		}
		if (ref_time >= Trigger.TriggerValue) {
			Trig();
		}
		break;
	}
	case TRIGGER::SIMSTART:
	{
		if (oapiGetSimTime() >= 0) {
			Trig();
		}
		break;
	}
	case TRIGGER::OTHER_EVENT:
	{
		if (Trigger.Other_event_h) {
			if (Trigger.Other_event_h->IsEventConsumed()) {
				Trig();
			}
		}
		break;
	}
	}
	
	return;
}
void Event::ConsumeBufferedKey(DWORD key, bool down, char *kstate) {
	if (!down)return;
	if (VB1->Playback())return;
	if ((Trigger.KeyMods.Alt ? KEYMOD_ALT(kstate) : !KEYMOD_ALT(kstate)) && (Trigger.KeyMods.Ctrl ? KEYMOD_CONTROL(kstate) : !KEYMOD_CONTROL(kstate)) && (Trigger.KeyMods.Shift ? KEYMOD_SHIFT(kstate) : !KEYMOD_SHIFT(kstate)) && key == Trigger.Key) {
		Trig();
	}
		
	return;
}
void Event::EventDockEvent(int dock, OBJHANDLE mate) {
	if (dock == Trigger.DockingPort) {
		if (Trigger.WhenDocking && mate != NULL) {
			if (Trigger.AnyMate) {
				Trig();
			}
			else {
				char matenamebuf[256] = { '\0' };
				oapiGetObjectName(mate, matenamebuf, 256);
				string matename(matenamebuf);
				if (matename.compare(Trigger.MateName) == 0) {
					Trig();
				}
			}
		}
		else if (!Trigger.WhenDocking && mate == NULL) {
			Trig();
		}
	}
	
}
UINT Event::GetID() {
	return id;
}
void Event::SetName(string newname) {
	name = newname;
	return;
}
string Event::GetName() {
	return name;
}


NullEvent::NullEvent(VesselBuilder1 *VB1) : Event(VB1){}
NullEvent::~NullEvent(){}
void NullEvent::ConsumeEvent() {
	return;
}




Child_Spawn::Child_Spawn(VesselBuilder1 *VB1,string v_name,string v_class,VECTOR3 _ofs,VECTOR3 _vel, VECTOR3 _rot_vel, int _mesh_to_del):Event(VB1){
	spawned_vessel_name = v_name;
	spawned_vessel_class = v_class;
	ofs = _ofs;
	vel = _vel;
	rot_vel = _rot_vel;
	mesh_to_del = _mesh_to_del;
	return;	
}
Child_Spawn::~Child_Spawn(){}

void Child_Spawn::ConsumeEvent() {
	char buf[256] = { '\0' };
	sprintf(buf, "%s", spawned_vessel_name.c_str());
	OBJHANDLE h_ves = oapiGetVesselByName(buf);
	if (!oapiIsVessel(h_ves)) {
		VECTOR3 rofs;
		VESSELSTATUS2 vs;
		memset(&vs, 0, sizeof(vs));
		vs.version = 2;
		VB1->GetStatusEx(&vs);
		VB1->Local2Rel(ofs, vs.rpos);
		VB1->GlobalRot(vel, rofs);
		vs.rvel += rofs;
		vs.vrot += rot_vel;
		spawend_vessel_h = oapiCreateVesselEx(spawned_vessel_name.c_str(), spawned_vessel_class.c_str(), &vs);
		if (mesh_to_del >= 0) {
			VB1->DelMesh(mesh_to_del);
			VB1->MshMng->mesh_deleted.push_back(mesh_to_del);
		}
	}
	return;
}
void Child_Spawn::SetSpawnedVesselName(string newName) {
	spawned_vessel_name = newName;
	return;
}
void Child_Spawn::SetSpawnedVesselClass(string newClass) {
	spawned_vessel_class = newClass;
	return;
}
void Child_Spawn::SetOfs(VECTOR3 _ofs) {
	ofs = _ofs;
	return;
}
void Child_Spawn::SetVel(VECTOR3 _vel) {
	vel = _vel;
}
void Child_Spawn::SetRotVel(VECTOR3 _rot_Vel) {
	rot_vel = _rot_Vel;
	return;
}
string Child_Spawn::GetSpawnedVesselName() {
	return spawned_vessel_name;
}
string Child_Spawn::GetSpawnedVesselClass() {
	return spawned_vessel_class;
}
VECTOR3 Child_Spawn::GetOfs() {
	return ofs;
}
VECTOR3 Child_Spawn::GetVel() {
	return vel;
}
VECTOR3 Child_Spawn::GetRotVel() {
	return rot_vel;
}


Anim_Trigger::Anim_Trigger(VesselBuilder1* VB1, UINT _anim_idx, bool _forward):Event(VB1){
	anim_idx = _anim_idx;
	forward = _forward;
	return;
}
Anim_Trigger::~Anim_Trigger() {}

void Anim_Trigger::ConsumeEvent() {
	if (forward) {
		VB1->AnimMng->StartAnimationForward(anim_idx);
	}
	else {
		VB1->AnimMng->StartAnimationBackward(anim_idx);
	}
	return;
}
void Anim_Trigger::SetAnimIdx(UINT _anim_idx) {
	anim_idx = _anim_idx;
	return;
}
UINT Anim_Trigger::GetAnimIdx() {
	return anim_idx;
}
void Anim_Trigger::SetForward(bool set) {
	forward = set;
	return;
}
bool Anim_Trigger::GetForward() {
	return forward;
}


Thruster_Fire::Thruster_Fire(VesselBuilder1* VB1, THRUSTER_HANDLE _th, double _level):Event(VB1) {
	th = _th;
	level = _level;
	return;
}
Thruster_Fire::~Thruster_Fire() {}
void Thruster_Fire::ConsumeEvent() {
	VB1->SetThrusterLevel(th, level);
}
void Thruster_Fire::SetThrusterTH(THRUSTER_HANDLE _th) {
	th = _th;
	return;
}
THRUSTER_HANDLE Thruster_Fire::GetThrusterTH() {
	return th;
}
void Thruster_Fire::SetLevel(double _Level) {
	level = _Level;
	return;
}
double Thruster_Fire::Thruster_Fire::GetLevel() {
	return level;
}

ThrusterGroup_Fire::ThrusterGroup_Fire(VesselBuilder1* VB1, THGROUP_TYPE thgroup_type, double _level):Event(VB1) {
	thgroup = thgroup_type;
	level = _level;
	return;
}
ThrusterGroup_Fire::~ThrusterGroup_Fire() {}

void ThrusterGroup_Fire::ConsumeEvent() {
	VB1->SetThrusterGroupLevel(thgroup, level);
}
void ThrusterGroup_Fire::SetThGroup(THGROUP_TYPE thgrp_type) {
	thgroup = thgrp_type;
}
THGROUP_TYPE ThrusterGroup_Fire::GetThGroup() {
	return thgroup;
}
void ThrusterGroup_Fire::SetLevel(double _level) {
	level = _level;
}
double ThrusterGroup_Fire::GetLevel() {
	return level;
}

Payload_Jettison::Payload_Jettison(VesselBuilder1* VB1, bool _next, UINT _dock_idx) :Event(VB1) {
	next = _next;
	dock_idx = _dock_idx;
	return;
}
Payload_Jettison::~Payload_Jettison(){}

void Payload_Jettison::ConsumeEvent() {
	if (next) {
		VB1->JettisonNextDock();
	}
	else {
		VB1->JettisonDock(dock_idx);
	}
	return;
}
void Payload_Jettison::SetDockIdx(UINT _dock_idx) {
	dock_idx = _dock_idx;
	return;
}
UINT Payload_Jettison::GetDockIdx() {
	return dock_idx;
}
void Payload_Jettison::SetNext(bool set) {
	next = set;
	return;
}
bool Payload_Jettison::GetNext() {
	return next;
}


Reset_Met::Reset_Met(VesselBuilder1* VB1, bool _now, double _newmjd0):Event(VB1) {
	now = _now;
	newmjd0 = _newmjd0;
}
Reset_Met::~Reset_Met(){}

void Reset_Met::ConsumeEvent() {
	VB1->Met->SetMJD0(now ? oapiGetSimMJD() : newmjd0 );
	return;
}
void Reset_Met::SetNow(bool set) {
	now = set;
	return;
}
bool Reset_Met::GetNow() {
	return now;
}
void Reset_Met::SetNewMJD0(double _newmjd0) {
	newmjd0 = _newmjd0;
	return;
}
double Reset_Met::GetNewMJD0() {
	return newmjd0;
}

Reconfiguration::Reconfiguration(VesselBuilder1* VB1, UINT _newconfig) :Event(VB1) {
	newconfig = _newconfig;
	return;
}
Reconfiguration::~Reconfiguration(){}
void Reconfiguration::SetNewConfig(UINT _newconfig) {
	newconfig = _newconfig;
	return;
}
UINT Reconfiguration::GetNewConfig() {
	return newconfig;
}
void Reconfiguration::ConsumeEvent() {
	VB1->ConfigMng->ApplyConfiguration(newconfig);
	return;
}

ShiftCG::ShiftCG(VesselBuilder1* VB1, VECTOR3 _shift) : Event(VB1) {
	shift = _shift;
	return;
}
ShiftCG::~ShiftCG(){}
void ShiftCG::ConsumeEvent() {
	VB1->ShiftCG(shift);
	return;
}
void ShiftCG::SetShift(VECTOR3 _shift) {
	shift = _shift;
	return;
}
VECTOR3 ShiftCG::GetShift() {
	return shift;
}
TextureSwap::TextureSwap(VesselBuilder1* VB1, UINT _mesh, DWORD _texidx, string texture):Event(VB1) {
	mesh = _mesh;
	texidx = _texidx;
	texture_name = texture;
	return;
}
TextureSwap::~TextureSwap(){}

void TextureSwap::ConsumeEvent() {
	SURFHANDLE tex = oapiLoadTexture(texture_name.c_str());
	DEVMESHHANDLE devmesh = VB1->GetDevMesh(VB1->visual, mesh);
	oapiSetTexture(devmesh, texidx, tex);
	return;
}
void TextureSwap::SetMesh(UINT _mesh) {
	mesh = _mesh;
	return;
}
UINT TextureSwap::GetMesh() {
	return mesh;
}
void TextureSwap::SetTexIdx(DWORD _texidx) {
	texidx = _texidx;
	return;
}
DWORD TextureSwap::GetTexIdx() {
	return texidx;
}
void TextureSwap::SetTextureName(string _texture) {
	texture_name = _texture;
	return;
}
string TextureSwap::GetTextureName() {
	return texture_name;
}
Delete_Me::Delete_Me(VesselBuilder1* VB1) : Event(VB1) {}
Delete_Me::~Delete_Me(){}
void Delete_Me::ConsumeEvent() {
	oapiDeleteVessel(VB1->GetHandle());
	return;
}
PlaySoundEvent::PlaySoundEvent(VesselBuilder1* VB1, string _soundfile) :Event(VB1) {
	soundfile = _soundfile;
}
PlaySoundEvent::~PlaySoundEvent(){}

void PlaySoundEvent::ConsumeEvent() {
	PlaySound(soundfile.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}
void PlaySoundEvent::SetSoundFile(string filename) {
	soundfile = filename;
}
string PlaySoundEvent::GetSoundFile() {
	return soundfile;
}
EnableEvent::EnableEvent(VesselBuilder1* VB1, Event* _ev, bool _enable) :Event(VB1) {
	ev = _ev;
	enable = _enable;
	return;
}
EnableEvent::~EnableEvent(){}
void EnableEvent::ConsumeEvent() {
	ev->Enable(enable);
}


EventManager::EventManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	Events.clear();
	return;
}
EventManager::~EventManager() {
	VB1 = NULL;
}
Event* EventManager::CreateGeneralVBEvent(string name, Event::TYPE type, Event::TRIGGER _Trigger) {
	Event* ev = NULL;
	string empty;
	empty.clear();
	if (type == Event::NULL_EVENT) {
		ev = CreateNullEvent(name, _Trigger);
	}
	else if (type == Event::CHILD_SPAWN) {
		ev = CreateChildSpawnEvent(name, _Trigger, empty, empty);
	}
	else if (type == Event::ANIMATION_TRIGGER) {
		ev = CreateAnimTriggerEvent(name, _Trigger, 0, true);
	}
	else if (type == Event::THRUSTER_FIRING) {
		ev = CreateThrusterFireEvent(name, _Trigger, NULL);
	}
	else if (type == Event::THRUSTERGROUP_LEVEL) {
		ev = CreateThrusterGroupLevelEvent(name, _Trigger, THGROUP_MAIN);
	}
	else if (type == Event::PAYLOAD_JETTISON) {
		ev = CreatePayloadJettisonEvent(name, _Trigger);
	}
	else if (type == Event::RESET_MET) {
		ev = CreateResetMetEvent(name, _Trigger);
	}
	else if (type == Event::RECONFIG) {
		ev = CreateReconfigurationEvent(name, _Trigger, 0);
	}
	else if (type == Event::SHIFT_CG) {
		ev = CreateShiftCGEvent(name, _Trigger);
	}
	else if (type == Event::TEXTURE_SWAP) {
		ev = CreateTextureSwapEvent(name, _Trigger, 0, 0, empty);
	}
	else if (type == Event::DELETE_ME) {
		ev = CreateDeleteMeEvent(name, _Trigger);
	}
	else if (type == Event::PLAYSOUND) {
		ev = CreatePlaySoundEvent(name, _Trigger, empty);
	}
	else if (type == Event::ENABLE_EVENT) {
		ev = CreateEnableEvent(name, _Trigger, NULL, true);
	}
	/*if (ev) {
		ev->id = id_counter;
		id_counter++;
		ev->SetTrigger(_Trigger);
		//ev->SetName(name);
	}
	
	Events.push_back(ev);*/
	return ev;
}
Event* EventManager::CreateNullEvent(string name, Event::TRIGGER _Trigger) {
	Event* ev = new NullEvent(VB1);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Null Event %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateChildSpawnEvent(string name, Event::TRIGGER _Trigger, string v_name, string v_class, VECTOR3 ofs, VECTOR3 vel, VECTOR3 rot_vel, int mesh_to_del) {
	Event* ev = new Child_Spawn(VB1, v_name, v_class, ofs, vel, rot_vel, mesh_to_del);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Child Spawn %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
	
}

Event* EventManager::CreateAnimTriggerEvent(string name, Event::TRIGGER _Trigger, UINT _anim_idx, bool _forward) {
	Event* ev = new Anim_Trigger(VB1, _anim_idx, _forward);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Start Animation %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateThrusterFireEvent(string name, Event::TRIGGER _Trigger, THRUSTER_HANDLE th, double level) {
	Event* ev = new Thruster_Fire(VB1, th, level);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Thruster Fire %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreateThrusterGroupLevelEvent(string name, Event::TRIGGER _Trigger, THGROUP_TYPE thgroup_type, double level) {
	Event* ev = new ThrusterGroup_Fire(VB1, thgroup_type, level);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Thruster Group Level %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreatePayloadJettisonEvent(string name, Event::TRIGGER _Trigger, bool next, UINT dock_idx) {
	Event* ev = new Payload_Jettison(VB1, next, dock_idx);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Dock Jettison %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreateResetMetEvent(string name, Event::TRIGGER _Trigger, bool now , double newmjd0) {
	Event* ev = new Reset_Met(VB1, now, newmjd0);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Reset MET %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreateReconfigurationEvent(string name, Event::TRIGGER _Trigger, UINT newconfig) {
	Event* ev = new Reconfiguration(VB1, newconfig);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Reconfiguration %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateShiftCGEvent(string name, Event::TRIGGER _Trigger, VECTOR3 shift) {
	Event* ev = new ShiftCG(VB1, shift);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Shift CG %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateTextureSwapEvent(string name, Event::TRIGGER _Trigger, UINT mesh, DWORD texidx, string texture_name) {
	Event* ev = new TextureSwap(VB1, mesh,texidx,texture_name);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Texture Change %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateDeleteMeEvent(string name, Event::TRIGGER _Trigger) {
	Event* ev = new Delete_Me(VB1);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Delete Me %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreatePlaySoundEvent(string name, Event::TRIGGER _Trigger, string _soundfile) {
	Event* ev = new PlaySoundEvent(VB1, _soundfile);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "PlaySound %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateEnableEvent(string name, Event::TRIGGER _Trigger, Event* _ev, bool _enable) {
	Event* ev = new EnableEvent(VB1, _ev, _enable);
	ev->SetTrigger(_Trigger);
	string evname;
	if (name.size() <= 0) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "Enable %i", Events.size());
		evname.assign(nbuf);
	}
	else {
		evname = name;
	}
	ev->SetName(evname);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

void EventManager::DeleteEvent(Event* ev) {
	VBVector<Event*>::iterator it = find(Events.begin(), Events.end(), ev);
	if (it != Events.end()) {
		delete ev;
		Events.erase(it);
	}
	return;
}
void EventManager::PreStep(double simt, double simdt, double mjd) {
	for (UINT i = 0; i < Events.size(); i++) {
		if (!Events[i]->IsEnabled()) { continue; }
		Events[i]->EventPreStep(simt, simdt, mjd);
	}
	return;
}
void EventManager::ConsumeBufferedKey(DWORD key, bool down, char *kstate) {
	if (Events.size() <= 0) { return; }
	for (UINT i = 0; i < Events.size(); i++) {
		if (!Events[i]->IsEnabled()) { continue; }
		if (Events[i]->GetTriggerType()==Event::TRIGGER::TRIGGERTYPE::KEYPRESS){
			Events[i]->ConsumeBufferedKey(key, down, kstate);
		}
	}
	return;
}
void EventManager::DockEvent(int dock, OBJHANDLE mate) {
	if (Events.size() <= 0) { return; }
	for (UINT i = 0; i < Events.size(); i++) {
		if (!Events[i]->IsEnabled()) { continue; }
		if (Events[i]->GetTriggerType() == Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT) {
			Events[i]->EventDockEvent(dock, mate);
		}
	}
}
UINT EventManager::GetEventsCount() {
	return Events.size();
}
string EventManager::GetEventName(UINT idx) {
	return Events[idx]->GetName();
}
Event::TRIGGER::TRIGGERTYPE EventManager::GetEventTriggerType(UINT idx) {
	return Events[idx]->GetTriggerType();
}
Event::TRIGGER EventManager::GetEventTrigger(UINT idx) {
	return Events[idx]->GetTrigger();
}
void EventManager::SetEventName(UINT idx, string newname) {
	Events[idx]->SetName(newname);
	return;
}
Event* EventManager::GetEventH(UINT idx) {
	return Events[idx];
}
void EventManager::Clear() {
	/*aaVBVector<Event*>::iterator it = find(Events.begin(), Events.end(), ev);
	if (it != Events.cend()) {
		delete ev;
		Events.erase(it);
	}*/
	for (UINT i = 0; i < Events.size(); i++) {
		delete Events[i];
	}
	Events.clear();
	return;
}
Event::TYPE EventManager::GetEventType(UINT idx) {
	return Events[idx]->Type();
}
UINT EventManager::GetEventIdx(Event* ev) {
	//int idx;
	//Events.VBFind(ev, idx);
	for (UINT i = 0; i < Events.size(); i++) {
		if (Events[i] == ev) {
			return i;
		}
	}
	return 0;
}


void EventManager::SetSpawnedVesselName(UINT idx, string newName) {
	((Child_Spawn*)Events[idx])->SetSpawnedVesselName(newName);
	return;
}
void EventManager::SetSpawnedVesselClass(UINT idx, string newClass) {
	((Child_Spawn*)Events[idx])->SetSpawnedVesselClass(newClass);
	return;
}
void EventManager::SetOfs(UINT idx, VECTOR3 ofs) {
	((Child_Spawn*)Events[idx])->SetOfs(ofs);
	return;
}
void EventManager::SetVel(UINT idx, VECTOR3 vel) {
	((Child_Spawn*)Events[idx])->SetVel(vel);
	return;
}
void EventManager::SetRotVel(UINT idx, VECTOR3 rot_Vel) {
	((Child_Spawn*)Events[idx])->SetRotVel(rot_Vel);
	return;
}
string EventManager::GetSpawnedVesselName(UINT idx) {
	return ((Child_Spawn*)Events[idx])->GetSpawnedVesselName();
}
string EventManager::GetSpawnedVesselClass(UINT idx) {
	return ((Child_Spawn*)Events[idx])->GetSpawnedVesselClass();
}
VECTOR3 EventManager::GetOfs(UINT idx) {
	return ((Child_Spawn*)Events[idx])->GetOfs();
}
VECTOR3 EventManager::GetVel(UINT idx) {
	return ((Child_Spawn*)Events[idx])->GetVel();
}
VECTOR3 EventManager::GetRotVel(UINT idx) {
	return ((Child_Spawn*)Events[idx])->GetRotVel();
}
int EventManager::GetMeshToDel(UINT idx) {
	return ((Child_Spawn*)Events[idx])->GetMeshToDel();
}
void EventManager::SetMeshToDel(UINT idx, int msh) {
	((Child_Spawn*)Events[idx])->SetMeshToDel(msh);
	return;
}
void EventManager::SetAnimIdx(UINT idx, UINT _anim_idx) {
	((Anim_Trigger*)Events[idx])->SetAnimIdx(_anim_idx);
	return;
}
UINT EventManager::GetAnimIdx(UINT idx) {
	return ((Anim_Trigger*)Events[idx])->GetAnimIdx();
}
void EventManager::SetForward(UINT idx, bool set) {
	((Anim_Trigger*)Events[idx])->SetForward(set);
	return;
}
bool EventManager::GetForward(UINT idx) {
	return ((Anim_Trigger*)Events[idx])->GetForward();
}

void EventManager::SetThrusterTH(UINT idx, THRUSTER_HANDLE _th) {
	((Thruster_Fire*)Events[idx])->SetThrusterTH(_th);
	return;
}
THRUSTER_HANDLE EventManager::GetThrusterTH(UINT idx) {
	return ((Thruster_Fire*)Events[idx])->GetThrusterTH();
}
void EventManager::SetThLevel(UINT idx, double _Level) {
	((Thruster_Fire*)Events[idx])->SetLevel(_Level);
	return;
}
double EventManager::GetThLevel(UINT idx) {
	return ((Thruster_Fire*)Events[idx])->GetLevel();
}

void EventManager::SetThGroup(UINT idx, THGROUP_TYPE thgrp_type) {
	((ThrusterGroup_Fire*)Events[idx])->SetThGroup(thgrp_type);
	return;
}
THGROUP_TYPE EventManager::GetThGroup(UINT idx) {
	return ((ThrusterGroup_Fire*)Events[idx])->GetThGroup();
}
void EventManager::SetThGLevel(UINT idx, double _level) {
	((ThrusterGroup_Fire*)Events[idx])->SetLevel(_level);
	return;
}
double EventManager::GetThGLevel(UINT idx) {
	return ((ThrusterGroup_Fire*)Events[idx])->GetLevel();
}

void EventManager::SetDockIdx(UINT idx, UINT _dock_idx) {
	((Payload_Jettison*)Events[idx])->SetDockIdx(_dock_idx);
	return;
}
UINT EventManager::GetDockIdx(UINT idx) {
	return ((Payload_Jettison*)Events[idx])->GetDockIdx();
}
void EventManager::SetNext(UINT idx, bool set) {
	((Payload_Jettison*)Events[idx])->SetNext(set);
	return;
}
bool EventManager::GetNext(UINT idx) {
	return ((Payload_Jettison*)Events[idx])->GetNext();
}

void EventManager::SetNow(UINT idx, bool set) {
	((Reset_Met*)Events[idx])->SetNow(set);
	return;
}
bool EventManager::GetNow(UINT idx) {
	return ((Reset_Met*)Events[idx])->GetNow();
}
void EventManager::SetNewMJD0(UINT idx, double _newmjd0) {
	((Reset_Met*)Events[idx])->SetNewMJD0(_newmjd0);
	return;
}
double EventManager::GetNewMJD0(UINT idx) {
	return ((Reset_Met*)Events[idx])->GetNewMJD0();
}

void EventManager::SetNewConfig(UINT idx, UINT _newconfig) {
	((Reconfiguration*)Events[idx])->SetNewConfig(_newconfig);
	return;
}
UINT EventManager::GetNewConfig(UINT idx) {
	return ((Reconfiguration*)Events[idx])->GetNewConfig();
}

void EventManager::SetShift(UINT idx, VECTOR3 _shift) {
	((ShiftCG*)Events[idx])->SetShift(_shift);
	return;
}
VECTOR3 EventManager::GetShift(UINT idx) {
	return ((ShiftCG*)Events[idx])->GetShift();
}

void EventManager::SetMesh(UINT idx, UINT _mesh) {
	((TextureSwap*)Events[idx])->SetMesh(_mesh);
	return;
}
UINT EventManager::GetMesh(UINT idx) {
	return ((TextureSwap*)Events[idx])->GetMesh();
}
void EventManager::SetTexIdx(UINT idx, DWORD _texidx) {
	((TextureSwap*)Events[idx])->SetTexIdx(_texidx);
	return;
}
DWORD EventManager::GetTexIdx(UINT idx) {
	return ((TextureSwap*)Events[idx])->GetTexIdx();
}
void EventManager::SetTextureName(UINT idx, string _texture) {
	((TextureSwap*)Events[idx])->SetTextureName(_texture);
	return;
}
string EventManager::GetTextureName(UINT idx) {
	return ((TextureSwap*)Events[idx])->GetTextureName();
}
void EventManager::SetSoundFile(UINT idx, string filename) {
	((PlaySoundEvent*)Events[idx])->SetSoundFile(filename);
	return;
}
string EventManager::GetSoundFile(UINT idx) {
	return ((PlaySoundEvent*)Events[idx])->GetSoundFile();
}
void EventManager::SetToEnable(UINT idx, bool set) {
	((EnableEvent*)Events[idx])->SetEnable(set);
	return;
}
void EventManager::SetEventToEnable(UINT idx, Event* _ev) {
	((EnableEvent*)Events[idx])->SetEvent(_ev);
	return;
}
Event* EventManager::GetEventToEnable(UINT idx) {
	return ((EnableEvent*)Events[idx])->GetEvent();
}
bool EventManager::GetToEnable(UINT idx) {
	return ((EnableEvent*)Events[idx])->GetEnable();
}


void EventManager::SetEventTrigger(UINT idx, Event::TRIGGER _Trigger) {
	Events[idx]->SetTrigger(_Trigger);
	return;
}
bool EventManager::IsEventEnabled(UINT idx) {
	return Events[idx]->IsEnabled();
}
void EventManager::SetEnableEvent(UINT idx, bool set) {
	Events[idx]->Enable(set);
	return;
}
bool EventManager::GetEventDefaultEnabled(UINT idx) {
	return Events[idx]->GetDefaultEnabled();
}
void EventManager::SetEventDefaultEnabled(UINT idx, bool set) {
	Events[idx]->SetDefaultEnabled(set);
	return;
}

VBVector<UINT> EventManager::GetEventsConsumed() {
	VBVector<UINT> consumed;
	consumed.clear();
	for (UINT i = 0; i < Events.size(); i++) {
		if (Events[i]->IsEventConsumed()) {
			consumed.push_back(i);
		}
	}
	return consumed;
}
void EventManager::SetEventConsumed(UINT idx, bool set) {
	Events[idx]->SetConsumed(set);
	return;
}
VBVector<UINT> EventManager::GetEventsToReconsume() {
	VBVector<UINT> to_reconsume;
	to_reconsume.clear();
	for (UINT i = 0; i < Events.size(); i++) {
		if (Events[i]->IsEventConsumed()) {
			if ((Events[i]->Type() == Event::TYPE::TEXTURE_SWAP) || (Events[i]->Type() == Event::TYPE::ENABLE_EVENT)) {
				to_reconsume.push_back(i);
			}
		}
	}
	return to_reconsume;
}

void EventManager::ConsumeEvent(UINT idx) {
	Events[idx]->ConsumeEvent();
	return;
}