#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "ExTexManager.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	_id_counter = 0;
	DefaultContrail.name = "DefaultContrail";
	DefaultExhaust.name = "DefaultExhaust";
	PARTICLESTREAMSPEC pss;
	pss.flags = 0;
	pss.srcsize = 8;
	pss.srcrate = 5;
	pss.v0 = 150;
	pss.srcspread = 0.3;
	pss.lifetime = 8;
	pss.growthrate = 4;
	pss.atmslowdown = 3;
	pss.ltype = PARTICLESTREAMSPEC::EMISSIVE;
	pss.levelmap = PARTICLESTREAMSPEC::LVL_PSQRT;
	pss.lmin = 0;
	pss.lmax = 0.5;
	pss.atmsmap = PARTICLESTREAMSPEC::ATM_PLOG;
	pss.amin = 1e-6;
	pss.amax = 0.1;
	pss.tex = oapiRegisterParticleTexture("Contrail1");
	DefaultContrail.pss = pss;

	pss.srcsize = 4;
	pss.srcrate = 20;
	pss.v0 = 150;
	pss.srcspread = 0.1;
	pss.lifetime = 0.3;
	pss.growthrate = 12;
	pss.atmslowdown = 2;
	pss.tex = oapiRegisterParticleTexture("Contrail3");
	DefaultExhaust.pss = pss;

	particle_defs.clear();

	AddParticleDef(DefaultContrail);
	AddParticleDef(DefaultExhaust);

	return;
}
ParticleManager::~ParticleManager() {
	VB1 = NULL;
}


void ParticleManager::AddParticleDef() {
	UINT index = particle_defs.size();
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Particle_%i", index);
	string name(cbuf);
	PARTICLESTREAMSPEC pss = DefaultContrail.pss;
	return AddParticleDef(name, pss);
}
void ParticleManager::AddParticleDef(string name, PARTICLESTREAMSPEC pss) {
	PARTICLE_DEF pdef = PARTICLE_DEF();
	pdef.name = name;
	pdef.pss = pss;
	pdef._id = _id_counter;
	_id_counter++;
	particle_defs.push_back(pdef);
	return;
}
void ParticleManager::AddParticleDef(PARTICLE_DEF pdef) {
	pdef._id = _id_counter;
	_id_counter++;
	particle_defs.push_back(pdef);
	return;
}
void ParticleManager::DeleteParticleDef(def_idx d_idx) {
	particle_defs.erase(particle_defs.begin() + d_idx);
	return;
}
void ParticleManager::SetParticleDefName(def_idx d_idx, string newname) {
	particle_defs[d_idx].name = newname;
	return;
}
string ParticleManager::GetParticleDefName(def_idx d_idx) {
	return particle_defs[d_idx].name;
}
void ParticleManager::SetParticleDefSpecs(def_idx d_idx, PARTICLESTREAMSPEC pss) {
	particle_defs[d_idx].pss = pss;
	return;
}
PARTICLESTREAMSPEC ParticleManager::GetPArticleDefSpecs(def_idx d_idx) {
	return particle_defs[d_idx].pss;
}

void ParticleManager::ParseCfgFile(FILEHANDLE fh) {
	UINT part_counter = 2;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "PARTICLE_%i_ID", part_counter);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		PARTICLESTREAMSPEC pss = { 0 };
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "PARTICLE_%i_NAME", part_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "PARTICLE_%i_SIZE", part_counter);
		oapiReadItem_float(fh, cbuf, pss.srcsize);
		sprintf(cbuf, "PARTICLE_%i_RATE", part_counter);
		oapiReadItem_float(fh, cbuf, pss.srcrate);
		sprintf(cbuf, "PARTICLE_%i_V0", part_counter);
		oapiReadItem_float(fh, cbuf, pss.v0);
		sprintf(cbuf, "PARTICLE_%i_SPREAD", part_counter);
		oapiReadItem_float(fh, cbuf, pss.srcspread);
		sprintf(cbuf, "PARTICLE_%i_LIFETIME", part_counter);
		oapiReadItem_float(fh, cbuf, pss.lifetime);
		sprintf(cbuf, "PARTICLE_%i_GROWTHRATE", part_counter);
		oapiReadItem_float(fh, cbuf, pss.growthrate);
		sprintf(cbuf, "PARTICLE_%i_ATMSLOWDOWN", part_counter);
		oapiReadItem_float(fh, cbuf, pss.atmslowdown);
		sprintf(cbuf, "PARTICLE_%i_LTYPE", part_counter);
		int ltype = -1;
		oapiReadItem_int(fh, cbuf, ltype);
		pss.ltype = (PARTICLESTREAMSPEC::LTYPE)ltype;
		sprintf(cbuf, "PARTICLE_%i_LEVELMAP", part_counter);
		int levelmap = -1;
		oapiReadItem_int(fh, cbuf, levelmap);
		pss.levelmap = (PARTICLESTREAMSPEC::LEVELMAP)levelmap;
		sprintf(cbuf, "PARTICLE_%i_LMIN", part_counter);
		oapiReadItem_float(fh, cbuf, pss.lmin);
		sprintf(cbuf, "PARTICLE_%i_LMAX", part_counter);
		oapiReadItem_float(fh, cbuf, pss.lmax);
		sprintf(cbuf, "PARTICLE_%i_ATMSMAP", part_counter);
		int atmsmap = -1;
		oapiReadItem_int(fh, cbuf, atmsmap);
		pss.atmsmap = (PARTICLESTREAMSPEC::ATMSMAP)atmsmap;
		sprintf(cbuf, "PARTICLE_%i_AMIN", part_counter);
		oapiReadItem_float(fh, cbuf, pss.amin);
		sprintf(cbuf, "PARTICLE_%i_AMAX", part_counter);
		oapiReadItem_float(fh, cbuf, pss.amax);
		sprintf(cbuf, "PARTICLE_%i_TEX", part_counter);
		int tex_int = -1;
		oapiReadItem_int(fh, cbuf, tex_int);
		if (tex_int == -1) {
			pss.tex = NULL;
		}
		else {
			pss.tex = VB1->ExTMng->GetExTexSurf(tex_int);
		}
		AddParticleDef(name, pss);

		part_counter++;
		sprintf(cbuf, "PARTICLE_%i_ID", part_counter);
	}
	
	
	return;
}
void ParticleManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------PARTICLES DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 2; i < particle_defs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "PARTICLE_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "PARTICLE_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", particle_defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "PARTICLE_%i_SIZE", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.srcsize);
		sprintf(cbuf, "PARTICLE_%i_RATE", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.srcrate);
		sprintf(cbuf, "PARTICLE_%i_V0", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.v0);
		sprintf(cbuf, "PARTICLE_%i_SPREAD", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.srcspread);
		sprintf(cbuf, "PARTICLE_%i_LIFETIME", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.lifetime);
		sprintf(cbuf, "PARTICLE_%i_GROWTHRATE", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.growthrate);
		sprintf(cbuf, "PARTICLE_%i_ATMSLOWDOWN", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.atmslowdown);
		sprintf(cbuf, "PARTICLE_%i_LTYPE", i);
		oapiWriteItem_int(fh, cbuf, (int)particle_defs[i].pss.ltype);
		sprintf(cbuf, "PARTICLE_%i_LEVELMAP", i);
		oapiWriteItem_int(fh, cbuf, (int)particle_defs[i].pss.levelmap);
		sprintf(cbuf, "PARTICLE_%i_LMIN", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.lmin);
		sprintf(cbuf, "PARTICLE_%i_LMAX", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.lmax);
		sprintf(cbuf, "PARTICLE_%i_ATMSMAP", i);
		oapiWriteItem_int(fh, cbuf, (int)particle_defs[i].pss.atmsmap);
		sprintf(cbuf, "PARTICLE_%i_AMIN", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.amin);
		sprintf(cbuf, "PARTICLE_%i_AMAX", i);
		oapiWriteItem_float(fh, cbuf, particle_defs[i].pss.amax);
		sprintf(cbuf, "PARTICLE_%i_TEX", i);
		int extex_idx = VB1->ExTMng->GetExTexIdx(particle_defs[i].pss.tex);
		oapiWriteItem_int(fh, cbuf, extex_idx);
		oapiWriteLine(fh, " ");
	}
	return;
}

UINT ParticleManager::GetParticleDefCount() {
	return particle_defs.size();
}
void ParticleManager::ToggleParticleTesting(def_idx d_idx) {
	if (particle_defs[d_idx].testing) {
		particle_defs[d_idx].testing = false;
		VB1->DelExhaustStream(particle_defs[d_idx].test_PSH);
	}
	else {
		particle_defs[d_idx].testing = true;
		particle_defs[d_idx].test_PSH = VB1->AddParticleStream(&particle_defs[d_idx].pss, _V(0, 0, -20), _V(0, 0, -1), &VB1->level1);
	}
	return;
}
bool ParticleManager::IsParticleTesting(def_idx d_idx) {
	return particle_defs[d_idx].testing;
}
UINT ParticleManager::GetParticleDefID(def_idx d_idx) {
	return particle_defs[d_idx]._id;
}
def_idx ParticleManager::IdxID2Def(UINT _id) {
	for (UINT i = 0; i < particle_defs.size(); i++) {
		if (particle_defs[i]._id == _id) {
			return i;
		}
	}
	return (UINT)-1;
}

void ParticleManager::Clear() {
	particle_defs.clear();
	AddParticleDef(DefaultContrail);
	AddParticleDef(DefaultExhaust);
	return;
}