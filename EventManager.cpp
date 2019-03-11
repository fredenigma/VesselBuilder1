#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "EventManager.h"

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
	if (ev) {
		ev->id = id_counter;
		id_counter++;
		ev->SetTrigger(_Trigger);
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

}
void EventManager::ConsumeBufferedKey(DWORD key, bool down, char *kstate) {
	if (Events.size() <= 0) { return; }
	for (UINT i = 0; i < Events.size(); i++) {
		if (Events[i]->GetTriggerType()==Event::TRIGGER::TRIGGERTYPE::KEYPRESS){
			Events[i]->ConsumeBufferedKey(key, down, kstate);
		}
	}
}
