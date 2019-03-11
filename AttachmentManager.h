#pragma once
struct ATT_DEF {
	bool toparent;
	
	VECTOR3 pos;
	VECTOR3 dir;
	VECTOR3 rot;
	VECTOR3 antidir;
	VECTOR3 antirot;
	VECTOR3 *pos_ptr;
	VECTOR3 *dir_ptr;
	VECTOR3 *antidir_ptr;
	VECTOR3 *antirot_ptr;
	ATTACHMENTHANDLE ah;
	string id;
	bool loose;
	bool created;
	bool id_check;
	string id_check_string;
	double range;
	ATT_DEF() {
		pos = _V(0, 0, 0);
		pos_ptr = new VECTOR3;
		dir_ptr = new VECTOR3;
		antidir_ptr = new VECTOR3;
		antirot_ptr = new VECTOR3;
		dir = _V(0, 0, 1);
		antidir = _V(0, 0, -1);
		rot = _V(0, 1, 0);
		antirot = _V(0, -1, 0);
		ah = NULL;
		loose = false;
		toparent = false;
		id.clear();
		created = false;
		range = 10;
		id_check = false;
		id_check_string.clear();
	}
};

class AttachmentManager {
public:
	AttachmentManager(VesselBuilder1 *_VB1);
	~AttachmentManager();
	VesselBuilder1 *VB1;
	vector<ATT_DEF> att_defs;
	void CreateAttDef();
	void AddAttDefNoCreate();
	void CreateAttDef(ATT_DEF att_d);
	void CreateAttDef(bool toparent, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot, string id, double range, bool loose, bool id_check, string id_check_string);
	void DeleteAttDef(def_idx d_idx);
	void ModifyAttDef(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot);
	UINT GetAttCount();
	//UINT GetValidAttCount();
	bool AttIsCreated(def_idx d_idx);
	string GetAttDefId(def_idx d_idx);
	bool AttToParent(def_idx d_idx);
	void GetAttDefPosDirRot(def_idx d_idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot);
	void SetAttDefID(def_idx d_idx, string id);
	void SetAttDefCreated(def_idx d_idx);
	void SetAttDefToParent(def_idx d_idx, bool toparent);
	ATTACHMENTHANDLE GetAttDefAH(def_idx d_idx);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	def_idx IdxAtt2Def(UINT att_idx);
	void SetAttDefRange(def_idx d_idx, double newrange);
	double GetAttDefRange(def_idx d_idx);
	VECTOR3* GetAttDefPosPtr(def_idx d_idx);
	VECTOR3* GetAttDefDirPtr(def_idx d_idx);
	VECTOR3* GetAttDefAntiDirPtr(def_idx d_idx);
	VECTOR3* GetAttDefAntiRotPtr(def_idx d_idx);
	void SetIdCheck(def_idx d_idx, bool set);
	bool GetIdCheck(def_idx d_idx);
	void SetIdCheckString(def_idx d_idx, string _id_check_string);
	string GetIdCheckString(def_idx d_idx);
	void Clear();
};