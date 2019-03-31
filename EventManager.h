#pragma once

class Event {
public:
	Event(VesselBuilder1 *_VB1);
	~Event();
	VesselBuilder1 *VB1;
	string name;
	UINT id;
	enum TYPE{NULL_EVENT, CHILD_SPAWN,PAYLOAD_JETTISON,ANIMATION_TRIGGER,THRUSTER_FIRING,THRUSTERGROUP_LEVEL,RESET_MET,RECONFIG,TEXTURE_SWAP,SHIFT_CG,DELETE_ME,PLAYSOUND,ENABLE_EVENT};
	virtual TYPE Type() const { return NULL_EVENT; }
	
	struct TRIGGER {
		enum TRIGGERTYPE { NULL_TRIG, SIMSTART, KEYPRESS, ALTITUDE, MAINFUELTANK_LEVEL, VELOCITY, TIME, DYNPRESSURE,OTHER_EVENT,DOCK_EVENT }Type;
		enum REPEAT_MODE{ONCE,ALWAYS}repeat_mode;
		DWORD Key;
		struct KEYMOD { bool Shift; bool Ctrl; bool Alt; }KeyMods;
		int DockingPort;
		string MateName;
		bool AnyMate;
		bool WhenDocking;
		enum CONDITION{BELOW,ABOVE}condition;
		double TriggerValue;
		Event* Other_event_h;
		enum VEL_MODE{GROUNDSPEED,AIRSPEED,MACH,ORBITALSPEED}vel_mode;
		enum TIME_MODE{MET,MJD}time_mode;
		TRIGGER() {
			Type = NULL_TRIG;
			repeat_mode = ONCE;
			Key = 0;
			KeyMods.Shift = false;
			KeyMods.Ctrl = false;
			KeyMods.Alt = false;
			condition = BELOW;
			TriggerValue = 0;
			vel_mode = GROUNDSPEED;
			time_mode = MJD;
			Other_event_h = NULL;
			DockingPort = 0;
			MateName.clear();
			AnyMate = true;
			WhenDocking = true;
		}
	}Trigger;
	void SetTrigger(TRIGGER _trig);
	//TRIGGER GetTrigger();
	TRIGGER::TRIGGERTYPE GetTriggerType();
	virtual void EventPreStep(double simt, double simdt, double mjd);
	virtual void ConsumeBufferedKey(DWORD key, bool down, char *kstate);
	virtual void EventDockEvent(int dock, OBJHANDLE mate);
	virtual void Trig();
	virtual void ConsumeEvent();
	bool Consumed;
	void SetConsumed(bool set);
	bool IsEventConsumed();
	UINT GetID();
	void SetName(string newname);
	string GetName();
	TRIGGER GetTrigger() { return Trigger; }
	bool Enabled;
	void Enable(bool set) { Enabled = set; }
	bool IsEnabled() { return Enabled; }
};

class NullEvent : public Event {
public:
	NullEvent(VesselBuilder1 *VB1);
	~NullEvent();
	TYPE Type() const { return NULL_EVENT; }
	void ConsumeEvent();
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
	int GetMeshToDel() { return mesh_to_del; }
	void SetMeshToDel(int msh) { mesh_to_del = msh; }
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
	Reconfiguration(VesselBuilder1* VB1, UINT _newconfig = 0);
	~Reconfiguration();
	TYPE Type() const { return RECONFIG; }
	UINT newconfig;
	void ConsumeEvent();
	void SetNewConfig(UINT _newconfig);
	UINT GetNewConfig();
};

class ShiftCG : public Event {
public:
	ShiftCG(VesselBuilder1* VB1, VECTOR3 shift =_V(0,0,0));
	~ShiftCG();
	TYPE Type() const { return SHIFT_CG; }
	VECTOR3 shift;
	void ConsumeEvent();
	void SetShift(VECTOR3 _shift);
	VECTOR3 GetShift();
};

class TextureSwap : public Event {
public:
	TextureSwap(VesselBuilder1* VB1, UINT mesh,DWORD texidx,string texture);
	~TextureSwap();
	TYPE Type() const { return TEXTURE_SWAP; }
	void ConsumeEvent();
	void SetMesh(UINT _mesh);
	UINT GetMesh();
	void SetTexIdx(DWORD _texidx);
	DWORD GetTexIdx();
	void SetTextureName(string _texture);
	string GetTextureName();
	string texture_name;
	UINT mesh;
	DWORD texidx;

	
};

class Delete_Me : public Event {
public:
	Delete_Me(VesselBuilder1* VB1);
	~Delete_Me();
	TYPE Type() const { return DELETE_ME; }
	void ConsumeEvent();
};

class PlaySoundEvent : public Event {
public:
	PlaySoundEvent(VesselBuilder1* VB1, string _soundfile);
	~PlaySoundEvent();
	TYPE Type() const { return PLAYSOUND; }
	void ConsumeEvent();
	string soundfile;
	void SetSoundFile(string filename);
	string GetSoundFile();
};

class EnableEvent : public Event {
public:
	EnableEvent(VesselBuilder1* VB1, Event* _ev,bool _enable);
	~EnableEvent();
	TYPE Type() const { return ENABLE_EVENT; }
	void ConsumeEvent();
	bool enable;
	Event* ev;
	void SetEnable(bool set) { enable = set; }
	void SetEvent(Event* _ev) { ev = _ev; }
	Event* GetEvent() { return ev; }
	bool GetEnable() { return enable; }
};

class EventManager {
public:
	EventManager(VesselBuilder1 *_VB1);
	~EventManager();
	VesselBuilder1 *VB1;
	VBVector<Event*>Events;
	Event* CreateGeneralVBEvent(string name,Event::TYPE type,Event::TRIGGER _Trigger);
	Event* CreateNullEvent(string name, Event::TRIGGER _Trigger);
	Event* CreateChildSpawnEvent(string name, Event::TRIGGER _Trigger, string v_name, string v_class, VECTOR3 ofs = _V(0, 0, 0), VECTOR3 vel = _V(0, 0, 0), VECTOR3 rot_vel = _V(0, 0, 0), int mesh_to_del = -1);
	Event* CreateAnimTriggerEvent(string name, Event::TRIGGER _Trigger, UINT _anim_idx, bool _forward);
	Event* CreateThrusterFireEvent(string name, Event::TRIGGER _Trigger, THRUSTER_HANDLE th, double level = 1);
	Event* CreateThrusterGroupLevelEvent(string name, Event::TRIGGER _Trigger, THGROUP_TYPE thgroup_type, double level = 1);
	Event* CreatePayloadJettisonEvent(string name, Event::TRIGGER _Trigger, bool next = true, UINT dock_idx = (UINT)-1);
	Event* CreateResetMetEvent(string name, Event::TRIGGER _Trigger, bool now = true, double newmjd0 = 0);
	Event* CreateReconfigurationEvent(string name, Event::TRIGGER _Trigger,UINT newconfig = 0);
	Event* CreateShiftCGEvent(string name, Event::TRIGGER _Trigger, VECTOR3 shift = _V(0,0,0));
	Event* CreateTextureSwapEvent(string name, Event::TRIGGER _Trigger, UINT mesh, DWORD texidx, string texture_name);
	Event* CreateDeleteMeEvent(string name, Event::TRIGGER _Trigger);
	Event* CreatePlaySoundEvent(string name, Event::TRIGGER _Trigger, string soundfile);
	Event* CreateEnableEvent(string name, Event::TRIGGER _Trigger, Event* _ev, bool _enable);
	void SetEventTrigger(UINT idx, Event::TRIGGER _Trigger);
	void DeleteEvent(Event* ev);
	void PreStep(double simt, double simdt, double mjd);
	void ConsumeBufferedKey(DWORD key, bool down, char *kstate);
	void DockEvent(int dock, OBJHANDLE mate);
	UINT id_counter;
	UINT GetEventsCount();
	string GetEventName(UINT idx);
	void SetEventName(UINT idx, string newname);
	Event::TRIGGER::TRIGGERTYPE GetEventTriggerType(UINT idx);
	Event::TRIGGER GetEventTrigger(UINT idx);
	Event* GetEventH(UINT idx);
	UINT GetEventIdx(Event* ev);
	Event::TYPE GetEventType(UINT idx);
	bool IsEventEnabled(UINT idx);
	void SetEnableEvent(UINT idx, bool set);

	void SetSpawnedVesselName(UINT idx, string newName);
	void SetSpawnedVesselClass(UINT idx, string newClass);
	void SetOfs(UINT idx, VECTOR3 ofs);
	void SetVel(UINT idx, VECTOR3 vel);
	void SetRotVel(UINT idx, VECTOR3 rot_Vel);
	string GetSpawnedVesselName(UINT idx);
	string GetSpawnedVesselClass(UINT idx);
	VECTOR3 GetOfs(UINT idx);
	VECTOR3 GetVel(UINT idx);
	VECTOR3 GetRotVel(UINT idx);
	int GetMeshToDel(UINT idx);
	void SetMeshToDel(UINT idx, int msh);

	void SetAnimIdx(UINT idx, UINT _anim_idx);
	UINT GetAnimIdx(UINT idx);
	void SetForward(UINT idx, bool set);
	bool GetForward(UINT idx);

	void SetThrusterTH(UINT idx, THRUSTER_HANDLE _th);
	THRUSTER_HANDLE GetThrusterTH(UINT idx);
	void SetThLevel(UINT idx, double _Level);
	double GetThLevel(UINT idx);

	void SetThGroup(UINT idx, THGROUP_TYPE thgrp_type);
	THGROUP_TYPE GetThGroup(UINT idx);
	void SetThGLevel(UINT idx, double _level);
	double GetThGLevel(UINT idx);

	void SetDockIdx(UINT idx, UINT _dock_idx);
	UINT GetDockIdx(UINT idx);
	void SetNext(UINT idx, bool set);
	bool GetNext(UINT idx);

	void SetNow(UINT idx, bool set);
	bool GetNow(UINT idx);
	void SetNewMJD0(UINT idx, double _newmjd0);
	double GetNewMJD0(UINT idx);

	void SetNewConfig(UINT idx, UINT _newconfig);
	UINT GetNewConfig(UINT idx);

	void SetShift(UINT idx, VECTOR3 _shift);
	VECTOR3 GetShift(UINT idx);

	void SetMesh(UINT idx, UINT _mesh);
	UINT GetMesh(UINT idx);
	void SetTexIdx(UINT idx, DWORD _texidx);
	DWORD GetTexIdx(UINT idx);
	void SetTextureName(UINT idx, string _texture);
	string GetTextureName(UINT idx);

	void SetSoundFile(UINT idx, string filename);
	string GetSoundFile(UINT idx);

	void SetToEnable(UINT idx, bool set);
	void SetEventToEnable(UINT idx, Event* _ev);
	Event* GetEventToEnable(UINT idx);
	bool GetToEnable(UINT idx);


	void Clear();
};

