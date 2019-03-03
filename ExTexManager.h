#pragma once
struct EXTEX_DEF {
	string TexName;
	SURFHANDLE tex;
	bool created;
	EXTEX_DEF() {
		TexName.clear();
		tex = NULL;
		created = false;
	}
};

class ExTexManager {
public:
	ExTexManager(VesselBuilder1 *_VB1);
	~ExTexManager();
	VesselBuilder1 *VB1;
	vector <EXTEX_DEF> extex_defs;
	void AddExTexDef();
	void AddExTexDef(string texname);
	bool StoreExTexDef(string texname, def_idx d_idx);
	void DelExTedDef(def_idx d_idx);
	SURFHANDLE GetExTexSurf(def_idx d_idx);
	string GetExTexName(def_idx d_idx);
	UINT GetExTexCount();
	bool IsExTexCreated(def_idx d_idx);
	int GetExTexIdx(SURFHANDLE tex);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void Clear();
};