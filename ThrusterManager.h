#pragma once
struct THR_DEF {
	THRUSTER_HANDLE th;
	string name;
	double p_ref;
	double isp_ref;
	VECTOR3 pos;
	VECTOR3 antidir;
	THR_DEF() {
		th = NULL;
		name.clear();
		p_ref = 101400;
		isp_ref = 0;
		pos = _V(0, 0, 0);
		antidir = _V(0, 0, -1);
	}
};

class ThrusterManager {
public:
	ThrusterManager(StationBuilder1 *_SB1);
	~ThrusterManager();
	StationBuilder1 *SB1;
	vector<THR_DEF> thr_defs;
	void AddThrDef();
	void AddThrDef(string name, VECTOR3 pos, VECTOR3 dir, double maxth, PROPELLANT_HANDLE ph, double isp0, double isp_ref, double p_ref);
	void DelThrDef(def_idx d_idx);
	string GetThrName(def_idx d_idx);
	VECTOR3 GetThrPos(def_idx d_idx);
	VECTOR3 GetThrDir(def_idx d_idx);
	double GetThrMax0(def_idx d_idx);
	PROPELLANT_HANDLE GetThrPh(def_idx d_idx);
	double GetThrIsp0(def_idx d_idx);
	double GetThrIspRef(def_idx d_idx);
	double GetThrPRef(def_idx d_idx);
	void SetThrName(def_idx d_idx, string newname);
	void SetThrPos(def_idx d_idx, VECTOR3 newpos);
	void SetThrDir(def_idx d_idx, VECTOR3 newdir);
	void SetThrMax0(def_idx d_idx, double newMax0);
	void SetThrPH(def_idx d_idx, PROPELLANT_HANDLE newph);
	void SetThrIsp(def_idx d_idx, double newisp0);
	void SetThrIspRef(def_idx d_idx, double newispref);
	void SetThrPRef(def_idx d_idx, double newPref);
	DWORD GetThrCount();
	THRUSTER_HANDLE GetThrTH(def_idx d_idx);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
};