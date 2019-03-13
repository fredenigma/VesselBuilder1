#pragma once

class Event {
public:
	Event(VesselBuilder1 *_VB1);
	~Event();
	VesselBuilder1 *VB1;
	string name;
	UINT id;
	enum TYPE{NULL_EVENT, CHILD_SPAWN,PAYLOAD_JETTISON,ANIMATION_TRIGGER,THRUSTER_FIRING,THRUSTERGROUP_LEVEL,RESET_MET,RECONFIG,TEXTURE_SWAP};
	virtual TYPE Type() const { return NULL_EVENT; }
	
	struct TRIGGER {
		enum TRIGGERTYPE { NULL_TRIG, SIMSTART, KEYPRESS, ALTITUDE, MAINFUELTANK_LEVEL, VELOCITY, TIME, DYNPRESSURE }Type;
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
	//TRIGGER GetTrigger();
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

class Anim_Trigger :public Event {
public:
	Anim_Trigger(VesselBuilder1* VB1, UINT anim_idx, bool forward=true);
	~Anim_Trigger();
	TYPE Type() const { return ANIMATION_TRIGGER; }
	UINT anim_idx;
	bool forward;
	void ConsumeEvent();
	void SetAnimIdx(UINT _anim_idx);
	UINT GetAnimIdx();
	void SetForward(bool set);
	bool GetForward();
};

class Thruster_Fire :public Event {
public:
	Thruster_Fire(VesselBuilder1* VB1, THRUSTER_HANDLE th, double level = 1);
	~Thruster_Fire();
	TYPE Type() const { return THRUSTER_FIRING; }
	THRUSTER_HANDLE th;
	double level;
	void ConsumeEvent();
	void SetThrusterTH(THRUSTER_HANDLE _th);
	THRUSTER_HANDLE GetThrusterTH();
	void SetLevel(double _Level);
	double GetLevel();
};

class ThrusterGroup_Fire :public Event {
public:
	ThrusterGroup_Fire(VesselBuilder1* VB1, THGROUP_TYPE thgroup_type, double level = 1);
	~ThrusterGroup_Fire();
	TYPE Type() const { return THRUSTERGROUP_LEVEL; }
	THGROUP_TYPE thgroup;
	double level;
	void ConsumeEvent();
	void SetThGroup(THGROUP_TYPE thgrp_type);
	THGROUP_TYPE GetThGroup();
	void SetLevel(double _level);
	double GetLevel();
};

class Payload_Jettison :public Event {
public:
	Payload_Jettison(VesselBuilder1* VB1, bool next = true, UINT dock_idx = (UINT)-1);
	~Payload_Jettison();
	TYPE Type() const { return PAYLOAD_JETTISON; }
	bool next;
	UINT dock_idx;
	void ConsumeEvent();
	void SetDockIdx(UINT _dock_idx);
	UINT GetDockIdx();
	void SetNext(bool set);
	bool GetNext();
};

class Reset_Met :public Event {
public:
	Reset_Met(VesselBuilder1* VB1, bool now = true, double newmjd0 = 0);
	~Reset_Met();
	TYPE Type() const { return RESET_MET; }
	bool now;
	double newmjd0;
	void ConsumeEvent();
	void SetNow(bool set);
	bool GetNow();
	void SetNewMJD0(double _newmjd0);
	double GetNewMJD0();
};

class Reconfiguration :public Event {
public:
	Reconfiguration(VesselBuilder1* VB1, bitset<N_SECTIONS>Sections, string newfilename);
	~Reconfiguration();
	TYPE Type() const { return RECONFIG; }
	bitset<N_SECTIONS>Sections;
	string newfilename;
	void ConsumeEvent();
	void SetSections(bitset<N_SECTIONS>newSections);
	bitset<N_SECTIONS> GetSections();
	void SetNewFileName(string _newfilename);
	string GetNewFileName();
};
class EventManager {
public:
	EventManager(VesselBuilder1 *_VB1);
	~EventManager();
	VesselBuilder1 *VB1;
	vector<Event*>Events;
	Event* CreateGeneralVBEvent(string name,Event::TYPE type,Event::TRIGGER _Trigger);
	Event* CreateChildSpawnEvent(string name, Event::TRIGGER _Trigger, string v_name, string v_class, VECTOR3 ofs = _V(0, 0, 0), VECTOR3 vel = _V(0, 0, 0), VECTOR3 rot_vel = _V(0, 0, 0), int mesh_to_del = -1);
	Event* CreateAnimTriggerEvent(string name, Event::TRIGGER _Trigger, UINT _anim_idx, bool _forward);
	Event* CreateThrusterFireEvent(string name, Event::TRIGGER _Trigger, THRUSTER_HANDLE th, double level = 1);
	Event* CreateThrusterGroupLevelEvent(string name, Event::TRIGGER _Trigger, THGROUP_TYPE thgroup_type, double level = 1);
	Event* CreatePayloadJettisonEvent(string name, Event::TRIGGER _Trigger, bool next = true, UINT dock_idx = (UINT)-1);
	Event* CreateResetMetEvent(string name, Event::TRIGGER _Trigger, bool now = true, double newmjd0 = 0);
	Event* CreateReconfigurationEvent(string name, Event::TRIGGER _Trigger, bitset<N_SECTIONS>_Sections, string _newfilename);
	void DeleteEvent(Event* ev);
	void PreStep(double simt, double simdt, double mjd);
	void ConsumeBufferedKey(DWORD key, bool down, char *kstate);
	UINT id_counter;

};