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
	void ConfigCheck(char* cbuf, UINT config);
	UINT Config_idx;
	VesselBuilder1* VB1;
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
	vector<Definitions> Defs;
	MeshManager* MshMng;
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
		Definitions() {
			name.clear();
			pos = _V(0, 0, 0);
			dir = _V(0, 0, 1);
			rot = _V(0, 1, 0);
			jettisonable = false;
		}
	};
	vector<Definitions> Defs;
	DockManager *DckMng;
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
	vector<Definitions> Defs;
	AttachmentManager *AttMng;
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
		vector<UINT> animcomp_grps;
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
	vector<AnimDefinitions>AnimDefs;
	vector<AnimCompDefinitions>AnimCompDefs;
	AnimationManager *AnimMng;
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
	vector<Definitions> Defs;
	PropellantManager *PrpMng;
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
		vector<ExhaustDefs>Exhausts;
		bool hasparticle;
		struct ParticleDefs {
			int pss_int;
			bool custompos;
			VECTOR3 pos;
		};
		vector<ParticleDefs>Particles;
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
	vector<Definitions> Defs;
	ThrusterManager *ThrMng;
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
	/*MeshSection* Msh_Sect;
	DockSection* Dck_Sect;
	AttachmentSection *Att_Sect;*/
	vector<Section*>Sections;
	UINT Config_idx;
};

class ConfigurationManager {
public:
	ConfigurationManager(VesselBuilder1 *_VB1);
	~ConfigurationManager();
	VesselBuilder1* VB1;
	vector<Configuration*>Configurations;
	UINT config_counter;
	void AddConfiguration(VesselBuilder1* VB1, map<ItemType,bool>_Sections, FILEHANDLE cfg);
	void ApplyConfiguration(UINT config, bool firstload = false);
	void ApplyDefaultConfiguration(bool firstload = false);
	UINT CurrentConfiguration;
	UINT GetCurrentConfiguration();
	UINT GetConfigurationsCount();
	void WriteConfiguration(UINT config_idx,FILEHANDLE cfg);

};