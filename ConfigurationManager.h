#pragma once

class Section {
public:
	Section(VesselBuilder1* _VB1, UINT config, FILEHANDLE cfg);
	virtual ~Section();
	virtual void WriteSection(FILEHANDLE fh);
	virtual void ParseSection(FILEHANDLE fh);
	virtual void ApplySection();
	virtual void UpdateSection();
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