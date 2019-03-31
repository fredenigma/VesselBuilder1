#pragma once

class Section {
public:
	Section(VesselBuilder1* _VB1, UINT config, FILEHANDLE cfg);
	virtual ~Section();
	virtual void WriteSection(FILEHANDLE fh);
	virtual void ParseSection(FILEHANDLE fh);
	virtual void ApplySection();
	virtual void UpdateSection();
	virtual void ManagerClear();
	bool IsActive() { return Active; }
	bool IsValid() { return Valid; }
	void SetValid(bool set) { Valid = set; }
	void SetActive(bool set) { Active = set; }
	bool Valid;
	bool Active;
	void ConfigCheck(char* cbuf, UINT config);
	UINT Config_idx;
	void SetConfigIdx(UINT config_idx);
	UINT GetConfigIdx();
	VesselBuilder1* VB1;
};

class SettingSection : public Section {
public:
	SettingSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~SettingSection();
	void WriteSection(FILEHANDLE fh);
	void ParseSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		double EmptyMass;
		double Size;
		VECTOR3 PMI;
		VECTOR3 CrossSections;
		double GravityGradientDamping;
		VECTOR3 RotDrag;
		bool met_enabled;
		bool show_met_in_hud;
		Definitions() {
			EmptyMass = 100;
			Size = 10;
			PMI = _V(1, 1, 1);
			CrossSections = _V(10, 10, 10);
			GravityGradientDamping = 20;
			RotDrag = _V(1, 1, 1);
			met_enabled = false;
			show_met_in_hud = false;
		}
	}Def;

	Definitions GetSection();
	void SetSection(Definitions d) { Def = d; }
	/*double GetEmptyMass();
	double GetSize();
	VECTOR3 GetPMI();
	VECTOR3 GetCrossSections();
	double GetGravityGradientDamping();
	VECTOR3 GetRotDrag();*/
	GeneralSettingsManager* SetMng;
};

class MeshSection :public Section {
public:
	MeshSection(VesselBuilder1* VB1, UINT config,FILEHANDLE cfg);
	~MeshSection();
	void WriteSection(FILEHANDLE fh);
	void ParseSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string meshname;
		VECTOR3 pos;
		VECTOR3 dir;
		VECTOR3 rot;
		WORD visibility;
		Definitions() {
			meshname.clear();
			pos = _V(0, 0, 0);
			dir = _V(0, 0, 1);
			rot = _V(0, 1, 0);
		}
	};
	VBVector<Definitions> Defs;
	MeshManager* MshMng;

	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
/*	int GetMeshDefCount();
	string GetMeshName(UINT idx);
	VECTOR3 GetMeshPos(UINT idx);
	VECTOR3 GetMeshDir(UINT idx);
	VECTOR3 GetMeshRot(UINT idx);
	WORD GetMeshVisibility(UINT idx);*/
};

class DockSection : public Section {
public:
	DockSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~DockSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string name;
		VECTOR3 pos;
		VECTOR3 dir;
		VECTOR3 rot;
		bool jettisonable;
		bool softdock;
		double sd_distance;
		UINT sd_anim;
		Definitions() {
			name.clear();
			pos = _V(0, 0, 0);
			dir = _V(0, 0, 1);
			rot = _V(0, 1, 0);
			jettisonable = false;
			softdock = false;
			sd_distance = 0;
			sd_anim = 0;
		}
	};
	VBVector<Definitions> Defs;
	
	DockManager *DckMng;


	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
/*	UINT GetDockCount();
	string GetDockName(UINT idx);
	void GetDockParams(UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot);
	bool IsDockJettisonable(UINT idx);*/
};

class AttachmentSection : public Section {
public:
	AttachmentSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~AttachmentSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		VECTOR3 pos;
		VECTOR3 dir;
		VECTOR3 rot;
		double range;
		string id;
		bool toparent;
		bool idcheck;
		string idcheck_string;
		Definitions() {
			id.clear();
			pos = _V(0, 0, 0);
			dir = _V(0, 0, 1);
			rot = _V(0, 1, 0);
			toparent = false;
			idcheck = false;
			idcheck_string.clear();
		}
	};
	VBVector<Definitions> Defs;
	AttachmentManager *AttMng;

	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
/*	UINT GetAttCount();
	bool GetIdCheck(UINT idx);
	string GetIdCheckString(UINT idx);
	string GetAttID(UINT idx);
	bool AttToParent(UINT idx);
	void GetAttPosDirRot(UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot);
	double GetAttRange(UINT idx);*/
};

class AnimationSection: public Section {
public:
	AnimationSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~AnimationSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct AnimDefinitions {
		string anim_name;
		double anim_defstate;
		double anim_duration;
		DWORD anim_key;
		AnimCycleType Cycle;
		AnimDefinitions() {
			anim_name.clear();
			anim_defstate = 0;
			anim_duration = 10;
			anim_key = OAPI_KEY_K;
			Cycle = AnimCycleType::GOANDSTOP;
		}
	};
	struct AnimCompDefinitions{	
	string animcomp_name;
		int animcomp_seq;
		double animcomp_state0;
		double animcomp_state1;
		int animcomp_mesh;
		int animcomp_ngrps;
		VBVector<UINT> animcomp_grps;
		int type;
		VECTOR3 ref;
		VECTOR3 axis;
		double angle;
		VECTOR3 shift;
		VECTOR3 scale;
		bool arm_tip;
		int arm_att;
		int parent_idx;
		AnimCompDefinitions() {
			animcomp_name.clear();
			animcomp_seq = -1;
			animcomp_state0 = 0;
			animcomp_state1 = 1;
			animcomp_mesh = -1;
			animcomp_ngrps = 0;
			animcomp_grps.clear();
			type = 1;
			ref = _V(0, 0, 0);
			axis = _V(0, 0, 1);
			shift = _V(0, 0, 0);
			scale = _V(1, 1, 1);
			arm_tip = false;
			arm_att = -1;
			parent_idx = -1;
		}
	};
	VBVector<AnimDefinitions>AnimDefs;
	VBVector<AnimCompDefinitions>AnimCompDefs;
	AnimationManager *AnimMng;

	void GetSection(VBVector<AnimDefinitions> &ad, VBVector<AnimCompDefinitions> &acd);
	void SetSection(VBVector<AnimDefinitions> ad, VBVector<AnimCompDefinitions> acd) { AnimDefs = ad; AnimCompDefs = acd; }
/*	UINT GetAnimCount();
	double GetAnimDefState(UINT idx);
	string GetAnimName(UINT idx);
	UINT GetAnimNComps(UINT idx);
	DWORD GetAnimKey(UINT idx);
	double GetAnimDuration(UINT idx);
	AnimCycleType GetAnimCycle(UINT idx);*/
};

class PropellantSection : public Section {
public:
	PropellantSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~PropellantSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		double maxmass;
		double efficiency;
		bool primary;
		bool retainfl;
		string name;
		double currentmass;
		Definitions() {
			maxmass = 100;
			efficiency = 1;
			primary = false;
			retainfl = false;
			name.clear();
			currentmass = -1;
		}
	};
	VBVector<Definitions> Defs;
	PropellantManager *PrpMng;

	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
};

class ThrusterSection : public Section {
public:
	ThrusterSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~ThrusterSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string name;
		VECTOR3 pos;
		VECTOR3 dir;
		double max0;
		double isp0;
		double ispref;
		double pref;
		int tank;
		bool hasexhaust;
		struct ExhaustDefs {
			double lsize;
			double wsize;
			int extex;
			bool customposdir;
			VECTOR3 pos;
			VECTOR3 dir;
		};
		VBVector<ExhaustDefs>Exhausts;
		bool hasparticle;
		struct ParticleDefs {
			int pss_int;
			bool custompos;
			VECTOR3 pos;
		};
		VBVector<ParticleDefs>Particles;
		Definitions() {
			name.clear();
			pos = _V(0, 0, 0);
			dir = _V(0, 0, 1);
			max0 = 100;
			isp0 = 1000;
			ispref = 1000;
			pref = 101400;
			tank = -1;
			hasexhaust = false;
			Exhausts.clear();
			hasparticle = false;
			Particles.clear();
		}
	};
	VBVector<Definitions> Defs;
	ThrusterManager *ThrMng;

	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
};

class ThrusterGroupSection : public Section {
public:
	ThrusterGroupSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~ThrusterGroupSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	map<THGROUP_TYPE, bool> Defined;
	map<THGROUP_TYPE, VBVector<UINT> >Thrusters;
	
	ThrusterGroupManager *ThrGrpMng;

	void GetSection(map<THGROUP_TYPE, bool> &Def, map<THGROUP_TYPE, VBVector<UINT> > &thrusters);
	void SetSection(map<THGROUP_TYPE, bool> Def, map<THGROUP_TYPE, VBVector<UINT> > thrusters) { Defined = Def; Thrusters = thrusters; }
};

class TouchDownPointSection : public Section {
public:
	TouchDownPointSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~TouchDownPointSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		VECTOR3 pos;
		double stiffness;
		double damping;
		double mu;
		double mu_lng;
		Definitions() {
			pos = _V(0, 0, 0);
			stiffness = 1e5;
			damping = 1e7;
			mu = 3;
			mu_lng = 1.5;
		}
	};
	VBVector<Definitions>Set1;
	VBVector<Definitions>Set2;
	bool SecondSetEnabled;
	int changeoveranim;

	TouchdownPointsManager *TdpMng;

	void GetSection(VBVector<Definitions> &set1, VBVector<Definitions>&set2);
	void SetSection(VBVector<Definitions> set1, VBVector<Definitions> set2) { Set1 = set1; Set2 = set2; }

	
};

class AirfoilSection : public Section {
public:
	AirfoilSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~AirfoilSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string name;
		int align;
		VECTOR3 ref;
		double c;
		double S;
		double A;
		struct LC_Point {
			double aoa;
			double cl;
			double cm;
		};
		VBVector<LC_Point>Points;
		Definitions() {
			name.clear();
			align = 0;
			ref = _V(0, 0, 0);
			c = 0;
			S = 0;
			A = 0;
			Points.clear();
		}
	};

	VBVector<Definitions> Defs;
	AirfoilsManager *AirfoilMng;

	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
};

class CtrlSurfaceSection : public Section {
public:
	CtrlSurfaceSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg);
	~CtrlSurfaceSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string name;
		int type;
		double area, dcl, delay;
		int anim;
		int axis;
		VECTOR3 ref;
		Definitions() {
			name.clear();
			type = 0;
			area = 0;
			dcl = 0;
			axis = 0;
			delay = 1;
			anim = -1;
			ref = _V(0, 0, 0);
		}
	};
	VBVector<Definitions> Defs;
	ControlSurfacesManager *CtrSurfMng;
	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
};

class CameraSection : public Section {
public:
	CameraSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg);
	~CameraSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string name;
		VECTOR3 pos;
		VECTOR3 dir;
		double tilt;
		Definitions() {
			name.clear();
			pos = _V(0, 0, 0);
			dir = _V(0, 0, 1);
			tilt = 0;
		}
	};
	VBVector<Definitions> Defs;
	CameraManager *CamMng;
	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
};

class VCSection : public Section {
public:
	VCSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg);
	~VCSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct PosDefinitions {
		string name;
		VECTOR3 pos;
		VECTOR3 dir;
		PosDefinitions() {
			name.clear();
			pos = _V(0, 0, 0);
			dir = _V(0, 0, 1);
		}
	};
	struct MFDDefinitions{
		int mesh;
		int group;
		bool wpwr;
		VECTOR3 pwr0;
		VECTOR3 pwr2;
		bool wcol;
		VECTOR3 coltl;
		VECTOR3 colbl;
		VECTOR3 coltr;
		VECTOR3 colbr;
		MFDDefinitions() {
			mesh = 0;
			group = 0;
			wpwr = false;
			pwr0 = _V(0, 0, 0);
			pwr2 = _V(0, 0, 0);
			wcol = false;
			coltl = colbl = coltr = colbr = _V(0, 0, 0);
		}
	};
	struct HUDDefinition {
		bool wHud;
		int mesh;
		int group;
		VECTOR3 hudcnt;
		double size;
		HUDDefinition() {
			wHud = false;
			mesh = 0;
			group = 0;
			hudcnt = _V(0, 0, 0);
			size = 0;
		}
	}HUDDef;

	VBVector<PosDefinitions> PosDefs;
	VBVector<MFDDefinitions> MFDDefs;
	VCManager *VCMng;

	void GetSection(VBVector<PosDefinitions> &pd, VBVector<MFDDefinitions>&md, HUDDefinition &hd);
	void SetSection(VBVector<PosDefinitions> pd, VBVector<MFDDefinitions>md, HUDDefinition hd) { PosDefs = pd; MFDDefs = md; HUDDef = hd; }
};

class LightSection : public Section {
public:
	LightSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg);
	~LightSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct BcnDefinitions {
		string name;
		int shape;
		VECTOR3 pos;
		VECTOR3 col;
		double size;
		double falloff;
		double period;
		double duration;
		double tofs;
		BcnDefinitions() {
			name.clear();
			shape = 0;
			pos = _V(0, 0, 0);
			col = _V(0, 0, 0);
			size = falloff = period = duration = tofs = 0;
		}
	};
	struct LightDefinitions {
		string name;
		int type;
		VECTOR3 dcol, acol, scol;
		VECTOR3 pos;
		double range;
		int visibility;
		VECTOR3 attenuation;
		VECTOR3 dir;
		VECTOR3 aperture;
		bool hasattachment;
		int attachment;
		LightDefinitions() {
			name.clear();
			type = 0;
			dcol = acol = scol = _V(0, 0, 0);
			pos = _V(0, 0, 0);
			range = 100;
			visibility = 0;
			attenuation = _V(0, 0, 0);
			dir = _V(0, 0, 1);
			aperture = _V(0, 45 * RAD, 90 * RAD);
			hasattachment = false;
			attachment = 0;
		}
	};
	VBVector<BcnDefinitions> BcnDefs;
	VBVector<LightDefinitions> LgtDefs;
	LightsManager *LightsMng;

	void GetSection(VBVector<BcnDefinitions> &bd, VBVector<LightDefinitions>&ld);
	void SetSection(VBVector<BcnDefinitions> bd, VBVector<LightDefinitions> ld) { BcnDefs = bd; LgtDefs = ld; }
};

class VardSection :public Section {
public:
	VardSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg);
	~VardSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string name;
		double factor;
		VECTOR3 ref;
		int anim;
		Definitions() {
			name.clear();
			factor = 0;
			ref = _V(0, 0, 0);
			anim = 0;
		}
	};
	VBVector<Definitions> Defs;
	VariableDragManager *VardMng;

	VBVector<Definitions> GetSection();
	void SetSection(VBVector<Definitions>d) { Defs = d; }
};

class EventSection : public Section {
public:
	EventSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg);
	~EventSection();
	void ParseSection(FILEHANDLE fh);
	void WriteSection(FILEHANDLE fh);
	void ApplySection();
	void UpdateSection();
	void ManagerClear();
	struct Definitions {
		string name;
		int type;
		int trigger_type;
		int repeat_mode;
		DWORD key;
		int keymod; //1 shift, 2 ctrl, 4 alt, 3 shift + ctrl, 5 shift + alt, 6 ctrl + alt, 7 shift + ctrl + alt
		int docking_port;
		string mate_name;
		bool any_mate;
		bool when_docking;
		int condition;
		double trigger_value;
		int other_event;
		int vel_mode;
		int time_mode;
		string spawned_vessel_name;
		string spawned_vessel_class;
		VECTOR3 ofs;
		VECTOR3 vel;
		VECTOR3 rot_vel;
		bool delmesh;
		int mesh_to_del;
		UINT anim_idx;
		bool forward;
		int thruster;
		double thlevel;
		THGROUP_TYPE group;
		double thglevel;
		UINT dock_idx;
		bool next;
		bool now;
		double newmjd0;
		UINT newconfig;
		VECTOR3 shiftcg;
		int mesh;
		DWORD texidx;
		string texture_name;
		string sound_file;
		bool disabled;
		int other_event_to_enable;
		bool other_event_enable;
		Definitions() {
			name.clear();
			type = 0;
			trigger_type = 0;
			repeat_mode = 0;
			key = 0;
			keymod = 0;
			docking_port = 0;
			mate_name.clear();
			any_mate = true;
			when_docking = false;
			condition = 0;
			trigger_value = 10000;
			other_event = 0;
			vel_mode = 0;
			time_mode = 0;
			spawned_vessel_class.clear();
			spawned_vessel_name.clear();
			ofs = vel = rot_vel = shiftcg = _V(0, 0, 0);
			delmesh = false;
			mesh_to_del = 0;
			anim_idx = 0;
			forward = true;
			thruster = 0;
			thlevel = 1;
			group = THGROUP_MAIN;
			thglevel = 1;
			dock_idx = 0;
			next = true;
			now = true;
			newmjd0 = 0;
			newconfig = 0;
			mesh = 0;
			texidx = 0;
			texture_name.clear();
			sound_file.clear();
			disabled = false;
			other_event_to_enable = 0;
			other_event_enable = true;
		}
	};
	VBVector<Definitions> Defs;
	EventManager *EvMng;
	VBVector<Definitions> GetSection() { return Defs; }
	void SetSection(VBVector<Definitions>d) { Defs = d; }
};


class Configuration {
public:
	Configuration(VesselBuilder1 *_VB1, map<ItemType,bool> _Sections, UINT config, FILEHANDLE _cfg);
	~Configuration();
	void Apply();
	void Write(FILEHANDLE cfg);
	void Update();
	map<ItemType, bool> Configuration_Sections;
	VesselBuilder1 *VB1;
	string filename;
	map<ItemType, bool> GetSections() { return Configuration_Sections; }
	bool IsSectionValid(ItemType Type);
	void SetSectionValid(ItemType Type, bool set);
	bool IsSectionActive(ItemType Type);
	void SetSectionActive(ItemType Type, bool set);
	void UpdateValids();
	VBVector<Section*>Sections;
	void SetIndex(UINT confing_idx);
	UINT GetIndex();
	UINT Config_idx;
	void Delete();
	bool GetSettingSection(SettingSection::Definitions &d);
	void SetSettingsSection(SettingSection::Definitions d);
	bool GetMeshSection(VBVector<MeshSection::Definitions> &d);
	void SetMeshSection(VBVector<MeshSection::Definitions>d);
	bool GetDockSection(VBVector<DockSection::Definitions> &d);
	void SetDockSection(VBVector<DockSection::Definitions>d);
	bool GetAttachmentSection(VBVector<AttachmentSection::Definitions> &d);
	void SetAttachmentSection(VBVector<AttachmentSection::Definitions> d);
	bool GetAnimationSection(VBVector<AnimationSection::AnimDefinitions>&ad, VBVector<AnimationSection::AnimCompDefinitions>&acd);
	void SetAnimationSection(VBVector<AnimationSection::AnimDefinitions> ad, VBVector<AnimationSection::AnimCompDefinitions> acd);
	bool GetPropellantSection(VBVector<PropellantSection::Definitions>&d);
	void SetPropellantSection(VBVector<PropellantSection::Definitions>d);
	bool GetThrusterSection(VBVector<ThrusterSection::Definitions>&d);
	void SetThrusterSection(VBVector<ThrusterSection::Definitions>d);
	bool GetThrusterGroupSection(map<THGROUP_TYPE, bool> &Def, map<THGROUP_TYPE, VBVector<UINT> > &thrusters);
	void SetThrusterGroupSection(map<THGROUP_TYPE, bool> Def, map<THGROUP_TYPE, VBVector<UINT> > thrusters);
	bool GetTouchdownPointSection(VBVector<TouchDownPointSection::Definitions> &set1, VBVector<TouchDownPointSection::Definitions>&set2);
	void SetTouchdownPointSection(VBVector<TouchDownPointSection::Definitions> set1, VBVector<TouchDownPointSection::Definitions> set2);
	bool GetAirfoilSection(VBVector<AirfoilSection::Definitions>&d);
	void SetAirfoilSection(VBVector<AirfoilSection::Definitions>d);
	bool GetCtrlSurfSection(VBVector<CtrlSurfaceSection::Definitions>&d);
	void SetCtrlSurfSection(VBVector<CtrlSurfaceSection::Definitions>d);
	bool GetCameraSection(VBVector<CameraSection::Definitions>&d);
	void SetCameraSection(VBVector<CameraSection::Definitions>d);
	bool GetVCSection(VBVector<VCSection::PosDefinitions>&pd, VBVector<VCSection::MFDDefinitions>&md, VCSection::HUDDefinition &hd);
	void SetVCSection(VBVector<VCSection::PosDefinitions>pd, VBVector<VCSection::MFDDefinitions>md, VCSection::HUDDefinition hd);
	bool GetLightSection(VBVector<LightSection::BcnDefinitions>&bd, VBVector<LightSection::LightDefinitions>&ld);
	void SetLightSection(VBVector<LightSection::BcnDefinitions> bd, VBVector<LightSection::LightDefinitions> ld);
	bool GetVardSection(VBVector<VardSection::Definitions>&d);
	void SetVardSection(VBVector<VardSection::Definitions>d);
	bool GetEventSection(VBVector<EventSection::Definitions>&d);
	void SetEventSection(VBVector<EventSection::Definitions>d);

};

class ConfigurationManager {
public:
	ConfigurationManager(VesselBuilder1 *_VB1);
	~ConfigurationManager();
	VesselBuilder1* VB1;
	VBVector<Configuration*>Configurations;
	UINT config_counter;
	UINT AddConfiguration(VesselBuilder1* VB1, map<ItemType,bool>_Sections, FILEHANDLE cfg);
	void ApplyConfiguration(UINT config, bool firstload = false);
	void ApplyDefaultConfiguration(bool firstload = false);
	bool IsSectionValid(UINT config, ItemType Type);
	void SetSectionValid(UINT config, ItemType Type, bool set);
	bool IsSectionActive(UINT config, ItemType Type);
	void UpdateConfiguration(UINT config);

	UINT GetCurrentConfiguration();
	UINT GetConfigurationsCount();
	void WriteConfiguration(UINT config_idx,FILEHANDLE cfg);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);

	UINT GetSectionActiveConfig(ItemType Type);
	map<ItemType, bool> GetConfigurationSections(UINT config);

	void DeleteConfiguration(UINT config);

	void UpdateConfigIndexes();

	void CopyConfigurationSection(UINT config_src, UINT config_dest, ItemType type);
	/*
	double GetEmptyMass(UINT config);
	double GetSize(UINT config);
	VECTOR3 GetPMI(UINT config);
	VECTOR3 GetCrossSections(UINT config);
	double GetGravityGradientDamping(UINT config);
	VECTOR3 GetRotDrag(UINT config);

	int GetMeshDefCount(UINT config);
	string GetMeshName(UINT config, UINT idx);
	VECTOR3 GetMeshPos(UINT config, UINT idx);
	VECTOR3 GetMeshDir(UINT config, UINT idx);
	VECTOR3 GetMeshRot(UINT config, UINT idx);
	WORD GetMeshVisibility(UINT config, UINT idx);

	UINT GetDockCount(UINT config);
	string GetDockName(UINT config, UINT idx);
	void GetDockParams(UINT config, UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot);
	bool IsDockJettisonable(UINT config, UINT idx);

	UINT GetAttCount(UINT config);
	bool GetIdCheck(UINT config, UINT idx);
	string GetIdCheckString(UINT config, UINT idx);
	string GetAttID(UINT config, UINT idx);
	bool AttToParent(UINT config, UINT idx);
	void GetAttPosDirRot(UINT config, UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot);
	double GetAttRange(UINT config, UINT idx);

	UINT GetAnimCount(UINT config);
	double GetAnimDefState(UINT config, UINT idx);
	string GetAnimName(UINT config, UINT idx);
	UINT GetAnimNComps(UINT config, UINT idx);
	DWORD GetAnimKey(UINT config, UINT idx);
	double GetAnimDuration(UINT config, UINT idx);
	AnimCycleType GetAnimCycle(UINT config, UINT idx);
	*/







	UINT CurrentConfiguration;
};