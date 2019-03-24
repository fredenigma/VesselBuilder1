#include "VesselBuilder1.h"
#include "DialogControl.h"

#include "EventManager.h"
#include "AnimationManager.h"
#include "MET.h"
#include "ConfigurationManager.h"

Event::Event(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	Trigger.Type = TRIGGER::TRIGGERTYPE::NULL_TRIG;
	Consumed = false;
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
		if (Trigger.vel_mode == TRIGGER::MS) {
			ref_speed = VB1->GetGroundspeed();
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
	if (type == Event::CHILD_SPAWN) {
		ev = new Child_Spawn(VB1,"","");
	}
	else if (type == Event::ANIMATION_TRIGGER) {
		ev = new Anim_Trigger(VB1, 0);
	}
	else if (type == Event::THRUSTER_FIRING) {
		ev = new Thruster_Fire(VB1, NULL);
	}
	else if (type == Event::THRUSTERGROUP_LEVEL) {
		ev = new ThrusterGroup_Fire(VB1, THGROUP_MAIN);
	}
	else if (type == Event::PAYLOAD_JETTISON) {
		ev = new Payload_Jettison(VB1);
	}
	else if (type == Event::RESET_MET) {
		ev = new Reset_Met(VB1);
	}
	else if (type == Event::RECONFIG) {
		ev = new Reconfiguration(VB1, 0);
	}
	else if (type == Event::SHIFT_CG) {
		ev = new ShiftCG(VB1, _V(0, 0, 0));
	}
	else if (type == Event::TEXTURE_SWAP) {
		ev = new TextureSwap(VB1,0,0,NULL);
	}
	if (ev) {
		ev->id = id_counter;
		id_counter++;
		ev->SetTrigger(_Trigger);
		ev->SetName(name);
	}
	
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateChildSpawnEvent(string name, Event::TRIGGER _Trigger, string v_name, string v_class, VECTOR3 ofs, VECTOR3 vel, VECTOR3 rot_vel, int mesh_to_del) {
	Event* ev = new Child_Spawn(VB1, v_name, v_class, ofs, vel, rot_vel, mesh_to_del);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
	
}

Event* EventManager::CreateAnimTriggerEvent(string name, Event::TRIGGER _Trigger, UINT _anim_idx, bool _forward) {
	Event* ev = new Anim_Trigger(VB1, _anim_idx, _forward);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateThrusterFireEvent(string name, Event::TRIGGER _Trigger, THRUSTER_HANDLE th, double level) {
	Event* ev = new Thruster_Fire(VB1, th, level);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreateThrusterGroupLevelEvent(string name, Event::TRIGGER _Trigger, THGROUP_TYPE thgroup_type, double level) {
	Event* ev = new ThrusterGroup_Fire(VB1, thgroup_type, level);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreatePayloadJettisonEvent(string name, Event::TRIGGER _Trigger, bool next, UINT dock_idx) {
	Event* ev = new Payload_Jettison(VB1, next, dock_idx);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreateResetMetEvent(string name, Event::TRIGGER _Trigger, bool now , double newmjd0) {
	Event* ev = new Reset_Met(VB1, now, newmjd0);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

Event* EventManager::CreateReconfigurationEvent(string name, Event::TRIGGER _Trigger, UINT newconfig) {
	Event* ev = new Reconfiguration(VB1, newconfig);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateShiftCGEvent(string name, Event::TRIGGER _Trigger, VECTOR3 shift) {
	Event* ev = new ShiftCG(VB1, shift);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}
Event* EventManager::CreateTextureSwapEvent(string name, Event::TRIGGER _Trigger, UINT mesh, DWORD texidx, string texture_name) {
	Event* ev = new TextureSwap(VB1, mesh,texidx,texture_name);
	ev->SetTrigger(_Trigger);
	ev->SetName(name);
	ev->id = id_counter;
	id_counter++;
	Events.push_back(ev);
	return ev;
}

void EventManager::DeleteEvent(Event* ev) {
	vector<Event*>::iterator it = find(Events.begin(), Events.end(), ev);
	if (it != Events.cend()) {
		delete ev;
		Events.erase(it);
	}
	return;
}
void EventManager::PreStep(double simt, double simdt, double mjd) {
	for (UINT i = 0; i < Events.size(); i++) {
		Events[i]->EventPreStep(simt, simdt, mjd);
	}
	return;
}
void EventManager::ConsumeBufferedKey(DWORD key, bool down, char *kstate) {
	if (Events.size() <= 0) { return; }
	for (UINT i = 0; i < Events.size(); i++) {
		if (Events[i]->GetTriggerType()==Event::TRIGGER::TRIGGERTYPE::KEYPRESS){
			Events[i]->ConsumeBufferedKey(key, down, kstate);
		}
	}
	return;
}
