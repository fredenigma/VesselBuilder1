#pragma once

struct THEX_DEF {
	EXHAUSTSPEC* es;
	bool customposdir;
	VECTOR3 pos, dir;
	THEX_DEF() {
		es = new EXHAUSTSPEC;
		customposdir = false;
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 1);
	}
};
struct THPART_DEF {
	PSTREAM_HANDLE psh;
	bool custompos;
	VECTOR3 pos;
	UINT pss_idx;
	THPART_DEF() {
		psh = NULL;
		pos = _V(0, 0, 0);
		custompos = false;
		pss_idx = (UINT)-1;
	}
};

struct THR_DEF {
	THRUSTER_HANDLE th;
	string name;
	double p_ref;
	double isp_ref;
	VECTOR3 pos;
	VECTOR3 antidir;
	//UINT ExhaustID;
	//bool HasExhaust;
	vector<THEX_DEF>Exhausts_def;
	bool testing;
	//vector<UINT>particles_id;
	//vector<PSTREAM_HANDLE>particles_h;
	vector<THPART_DEF>particles_def;
	VECTOR3* pos_ptr;
	VECTOR3* antidir_ptr;
	THR_DEF() {
		th = NULL;
		name.clear();
		p_ref = 101400;
		isp_ref = 0;
		pos = _V(0, 0, 0);
		antidir = _V(0, 0, -1);
		//ExhaustID = (UINT)-1;
		//HasExhaust = false;
		Exhausts_def.clear();
		testing = false;
		//particles_id.clear();
		//particles_h.clear();
		pos_ptr = new VECTOR3;
		antidir_ptr = new VECTOR3;
	}
};


class ThrusterManager {
public:
	ThrusterManager(VesselBuilder1 *_VB1);
	~ThrusterManager();
	VesselBuilder1 *VB1;
	vector<THR_DEF> thr_defs;
	void AddThrDef();
	UINT AddThrDef(string name, VECTOR3 pos, VECTOR3 dir, double maxth, PROPELLANT_HANDLE ph, double isp0, double isp_ref, double p_ref);
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

	//void EnableThrExhausts(def_idx d_idx, bool enable);
	bool HasThrExhausts(def_idx d_idx);
	void AddThrExhaust(def_idx d_idx, double length, double width, SURFHANDLE tex, bool customposdir,VECTOR3 pos, VECTOR3 dir);
	void RemoveThrExhaust(def_idx d_idx, def_idx ex_idx);
	bool IsThrExhsCustomposdir(def_idx d_idx, def_idx ex_idx);
	void GetThrExParams(def_idx d_idx, def_idx ex_idx, double &lsize, double &wsize, SURFHANDLE &tex, bool &customposdir, VECTOR3 &pos, VECTOR3 &dir);
	UINT GetThrExCount(def_idx d_idx);

	bool HasThrParticles(def_idx d_idx);
	void AddThrParticle(def_idx d_idx, UINT pss_index, bool custompos=false, VECTOR3 pos=_V(0,0,0));
	void RemoveThrParticle(def_idx d_idx, def_idx part_idx);
	void GetThrParticleParams(def_idx d_idx, def_idx part_idx, UINT &pss_index, bool &custompos, VECTOR3 &pos);
	UINT GetThrParticlesCount(def_idx d_idx);
	vector<UINT> GetThrParticlesIDs(def_idx d_idx);
	//void SetThrExhsCustomposdir(def_idx d_idx, UINT exId);


//	bool ThrHasExhaust(def_idx d_idx);
//	void SetThrExhaust(def_idx d_idx, double length, double width, SURFHANDLE tex);
//	void SetThrHasExhaust(def_idx d_idx, bool set);
//	void SetThrExhaustLength(def_idx d_idx, double newlength);
//	void SetThrExhaustWidth(def_idx d_idx, double newwidth);
//	double GetThrExhaustLength(def_idx d_idx);
//	double GetThrExhaustWidth(def_idx d_idx);
//	SURFHANDLE GetThrExhaustTex(def_idx d_idx);
//	void SetThrExhaustTex(def_idx d_idx, SURFHANDLE newtex);
	void ToggleThrusterTest(def_idx d_idx);
	bool ThrIsTesting(def_idx d_idx);
	UINT GetThrIdx(THRUSTER_HANDLE th);
	//UINT GetThrParticlesCount(def_idx d_idx);
	//vector<UINT> GetThrParticlesIDs(def_idx d_idx);
	//vector<PSTREAM_HANDLE> GetThrParticlesH(def_idx d_idx);
	//void SetThrParticles(def_idx d_idx, vector<UINT> _ids);
	//void AddThrParticles(def_idx d_idx, def_idx particle_idx);
	//void ClearThrParticles(def_idx d_idx);
	VECTOR3* GetThrPosPtr(def_idx d_idx);
	VECTOR3* GetThrAntiDirPtr(def_idx d_idx);
	void Clear();
};

class ThrusterGroupManager {
public:
	ThrusterGroupManager(VesselBuilder1 *_VB1);
	~ThrusterGroupManager();
	VesselBuilder1 *VB1;
	map<THGROUP_TYPE , bool> Defined;
	map<THGROUP_TYPE , vector<THRUSTER_HANDLE> > Thrusters;
	void DefineGroup(THGROUP_TYPE thgt, vector<THRUSTER_HANDLE>thrusters);
	void UndefineGroup(THGROUP_TYPE thgt);
	bool IsGroupDefined(THGROUP_TYPE thgt);
	vector<THRUSTER_HANDLE> GetThrusters(THGROUP_TYPE thgt);
	vector<THRUSTER_HANDLE> GetThrustersfromIdx(vector<UINT> idx);
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	void ResetDefine();
};