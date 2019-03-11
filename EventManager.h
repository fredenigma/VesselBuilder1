#pragma once

class Event {
public:
	Event(VesselBuilder1 *_VB1);
	~Event();
	VesselBuilder1 *VB1;
	string name;
	UINT id;
	enum TYPE{NULL_EVENT, CHILD_SPAWN,PAYLOAD_JETTISON,ANIMATION_TRIGGER,THRUSTER_FIRING,THRUSTERGROUP_LEVEL,PARTIAL_RECONFIG,TOTAL_RECONFIG,TEXTURE_SWAP};
	virtual TYPE Type() const { return NULL_EVENT; }
	
	struct TRIGGER {
		enum TRIGGERTYPE { NULL_TRIG, KEYPRESS, ALTITUDE, FUELTANK_LEVEL, VELOCITY, TIME, DYNPRESSURE }Type;
		enum REPEAT_MODE{ONCE,ALWAYS}repeat_mode;
		DWORD Key;
		struct KEYMOD { bool Shift; bool Ctrl; bool Alt; }KeyMods;
		enum FROM{BELOW,ABOVE}from;
		double TriggerValue;
		enum VEL_MODE{MS,MACH}vel_mode;
		enum TIME_MODE{MET,MJD}time_mode;
		TRIGGER() {
			Type = NULL_TRIG;
			repeat_mode = ONCE;
			Key = 0;
			KeyMods.Shift = false;
			KeyMods.Ctrl = false;
			KeyMods.Alt = false;
			from = BELOW;
			TriggerValue = 0;
			vel_mode = MS;
			time_mode = MJD;
		}
	}Trigger;
	void SetTrigger(TRIGGER _trig);
	TRIGGER GetTrigger();
	TRIGGER::TRIGGERTYPE GetTriggerType();
	virtual void EventPreStep(double simt, double simdt, double mjd);
	virtual void ConsumeBufferedKey(DWORD key, bool down, char *kstate);
	virtual void Trig();
	virtual void ConsumeEvent();
	bool Consumed;
	void SetConsumed(bool set);
	bool IsEventConsumed();
	UINT GetID();
	void SetName(string newname);
	string GetName();
};

class Child_Spawn :public Event {
public:
	Child_Spawn(VesselBuilder1 *VB1, string v_name, string v_class, VECTOR3 ofs=_V(0,0,0), VECTOR3 vel=_V(0,0,0), VECTOR3 rot_vel=_V(0,0,0),int mesh_to_del=-1);
	~Child_Spawn();
	TYPE Type() const { return CHILD_SPAWN; }
	string spawned_vessel_name;
	string spawned_vessel_class;
	OBJHANDLE spawend_vessel_h;
	VECTOR3 ofs;
	VECTOR3 vel;
	VECTOR3 rot_vel;
	int mesh_to_del;
	void ConsumeEvent();
	void SetSpawnedVesselName(string newName);
	void SetSpawnedVesselClass(string newClass);
	void SetOfs(VECTOR3 ofs);
	void SetVel(VECTOR3 vel);
	void SetRotVel(VECTOR3 rot_Vel);
	string GetSpawnedVesselName();
	string GetSpawnedVesselClass();
	VECTOR3 GetOfs();
	VECTOR3 GetVel();
	VECTOR3 GetRotVel();

};

class EventManager {
public:
	EventManager(VesselBuilder1 *_VB1);
	~EventManager();
	VesselBuilder1 *VB1;
	vector<Event*>Events;
	Event* CreateGeneralVBEvent(string name,Event::TYPE type,Event::TRIGGER _Trigger);
	Event* CreateChildSpawnEvent(string name, Event::TRIGGER _Trigger, string v_name, string v_class, VECTOR3 ofs = _V(0, 0, 0), VECTOR3 vel = _V(0, 0, 0), VECTOR3 rot_vel = _V(0, 0, 0), int mesh_to_del = -1);
	void DeleteEvent(Event* ev);
	void PreStep(double simt, double simdt, double mjd);
	void ConsumeBufferedKey(DWORD key, bool down, char *kstate);
	UINT id_counter;

};