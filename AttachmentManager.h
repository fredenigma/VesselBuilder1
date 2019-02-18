#pragma once
struct ATT_DEF {
	bool toparent;
	VECTOR3 pos;
	VECTOR3 dir;
	VECTOR3 rot;
	VECTOR3 antidir;
	VECTOR3 antirot;
	ATTACHMENTHANDLE ah;
	string id;
	bool loose;
	bool created;
	ATT_DEF() {
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 1);
		antidir = _V(0, 0, -1);
		rot = _V(0, 1, 0);
		antirot = _V(0, -1, 0);
		ah = NULL;
		loose = false;
		toparent = false;
		id.clear();
		created = false;
	}
};

class AttachmentManager {
public:
	AttachmentManager(StationBuilder1 *_SB1);
	~AttachmentManager();
	StationBuilder1 *SB1;
	vector<ATT_DEF> att_defs;
	void CreateAttDef();
	void AddAttDefNoCreate();
	void CreateAttDef(ATT_DEF att_d);
	void CreateAttDef(bool toparent, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot, string id, bool loose);
	void DeleteAttDef(def_idx d_idx);
	void ModifyAttDef(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot);
	UINT GetAttCount();
	bool AttIsCreated(def_idx d_idx);
	string GetAttDefId(def_idx d_idx);
	bool AttToParent(def_idx d_idx);
	void GetAttDefPosDirRot(def_idx d_idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot);
	void SetAttDefID(def_idx d_idx, string id);
	void SetAttDefCreated(def_idx d_idx);
	void SetAttDefToParent(def_idx d_idx, bool toparent);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
};