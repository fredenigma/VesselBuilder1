#pragma once
struct EX_DEF {
	string name;
	UINT ID;
	EXHAUSTSPEC *es;
	//bool customposdir;
	//bool customtex;
	bool Defined;
	//UINT th_index;
	EX_DEF() {
		name.clear();
		ID = (UINT)-1;
		//customposdir = false;
		//customtex = false;
		Defined = false;
		es = new EXHAUSTSPEC;
		//th_index = (UINT)-1;
	}
};
class ExhaustManager {
public:
	ExhaustManager(VesselBuilder1* _VB1);
	~ExhaustManager();
	VesselBuilder1* VB1;
	vector<EX_DEF>ex_def;
	void AddUndefinedExDef();
	string GetExName(def_idx d_idx);
	void SetExName(def_idx d_idx, string newname);
	UINT AddExDef(string name, UINT thr_index,double lsize,double wsize,bool customposdir, bool customtex,VECTOR3 pos,VECTOR3 dir,UINT extex);
	void DefineExDef(def_idx d_idx, UINT thr_index, double lsize, double wsize, bool customposdir, bool customtex, VECTOR3 pos, VECTOR3 dir, UINT extex);
	void DelExDef(def_idx d_idx);
	void UndefineExDef(def_idx d_idx);
	bool IsExDefined(def_idx d_idx);
	void GetExParams(def_idx d_idx, UINT &thr_index, double &lsize, double &wsize, VECTOR3 &pos, VECTOR3 &dir, UINT &extex,bool &customposdir,bool &customtex);
	UINT GetExDefCount();
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void Clear();
};