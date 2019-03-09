#pragma once
struct PARTICLE_DEF {
	UINT _id;
	string name;
	PARTICLESTREAMSPEC pss;
	bool testing;
	PSTREAM_HANDLE test_PSH;
	PARTICLE_DEF() {
		_id = (UINT)-1;
		name.clear();
		pss = { 0 };
		testing = false;
		test_PSH = NULL;
	}
};



class ParticleManager {
public:
	ParticleManager(VesselBuilder1 *_VB1);
	~ParticleManager();
	VesselBuilder1 *VB1;
	vector<PARTICLE_DEF> particle_defs;
	void AddParticleDef();
	void AddParticleDef(string name, PARTICLESTREAMSPEC pss);
	void AddParticleDef(PARTICLE_DEF pdef);
	void DeleteParticleDef(def_idx d_idx);
	void SetParticleDefName(def_idx d_idx, string newname);
	string GetParticleDefName(def_idx d_idx);
	void SetParticleDefSpecs(def_idx d_idx, PARTICLESTREAMSPEC pss);
	PARTICLESTREAMSPEC GetPArticleDefSpecs(def_idx d_idx);
	PARTICLE_DEF DefaultContrail;
	PARTICLE_DEF DefaultExhaust;
	void ParseCfgFile(FILEHANDLE fh);
	void WriteCfg(FILEHANDLE fh);
	UINT GetParticleDefCount();
	void ToggleParticleTesting(def_idx d_idx);
	bool IsParticleTesting(def_idx d_idx);
	UINT _id_counter;
	UINT GetParticleDefID(def_idx d_idx);
	def_idx IdxID2Def(UINT _id);
	void Clear();

};