#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "PropellantManager.h"
#include "ParticleManager.h"
#include "ExTexManager.h"
#include "ThrusterManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

ThrusterManager::ThrusterManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	thr_defs.clear();
	return;
}
ThrusterManager::~ThrusterManager() { VB1 = NULL; }
void ThrusterManager::AddThrDef() {
	UINT n = thr_defs.size();
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Thruster_%i", n);
	string name(cbuf);
	AddThrDef(name, _V(0, 0, 0), _V(0, 0, 1), 1000, NULL, 1000, 0, 101400);
	return;
}
UINT ThrusterManager::AddThrDef(string name, VECTOR3 pos, VECTOR3 dir, double maxth, PROPELLANT_HANDLE ph, double isp0, double isp_ref, double p_ref) {
	LogV("Adding Thruster:%s", name.c_str());
	THR_DEF thr = THR_DEF();
	thr.name = name;
	thr.th = VB1->CreateThruster(pos, dir, maxth, ph, isp0, isp_ref, p_ref);
	thr.pos = pos;
	*thr.pos_ptr = pos;
	thr.dir = dir;
	*thr.dir_ptr = dir;
	thr_defs.push_back(thr);
	return thr_defs.size()-1;
}
void ThrusterManager::DelThrDef(def_idx d_idx) {
	LogV("Deleting Thruster:%i", d_idx);
	VB1->DelThruster(thr_defs[d_idx].th);
	delete thr_defs[d_idx].dir_ptr;
	delete thr_defs[d_idx].pos_ptr;
	thr_defs.erase(thr_defs.begin() + d_idx);
	LogV("Deleted");
	return;
}
string ThrusterManager::GetThrName(def_idx d_idx) {
	return thr_defs[d_idx].name;
}
VECTOR3 ThrusterManager::GetThrPos(def_idx d_idx) {
	VECTOR3 pos;
	VB1->GetThrusterRef(thr_defs[d_idx].th,pos);
	return pos;
}
VECTOR3 ThrusterManager::GetThrDir(def_idx d_idx) {
	VECTOR3 dir;
	VB1->GetThrusterDir(thr_defs[d_idx].th, dir);
	return dir;
}
double ThrusterManager::GetThrMax0(def_idx d_idx) {
	return VB1->GetThrusterMax0(thr_defs[d_idx].th);
}
PROPELLANT_HANDLE ThrusterManager::GetThrPh(def_idx d_idx) {
	return VB1->GetThrusterResource(thr_defs[d_idx].th);
}
double ThrusterManager::GetThrIsp0(def_idx d_idx) {
	return VB1->GetThrusterIsp0(thr_defs[d_idx].th);
}
double ThrusterManager::GetThrIspRef(def_idx d_idx) {
	return thr_defs[d_idx].isp_ref;
}
double ThrusterManager::GetThrPRef(def_idx d_idx) {
	return thr_defs[d_idx].p_ref;
}
void ThrusterManager::SetThrName(def_idx d_idx, string newname) {
	thr_defs[d_idx].name = newname;
	return;
}
void ThrusterManager::SetThrPos(def_idx d_idx, VECTOR3 newpos) {
	VB1->SetThrusterRef(thr_defs[d_idx].th, newpos);
	thr_defs[d_idx].pos = newpos;
	*thr_defs[d_idx].pos_ptr = newpos;
	return;
}
void ThrusterManager::SetThrDir(def_idx d_idx, VECTOR3 newdir) {
	VB1->SetThrusterDir(thr_defs[d_idx].th, newdir);
	thr_defs[d_idx].dir = newdir;
	*thr_defs[d_idx].dir_ptr = newdir;
	return;
}
void ThrusterManager::SetThrMax0(def_idx d_idx, double newMax0) {
	VB1->SetThrusterMax0(thr_defs[d_idx].th, newMax0);
	return;
}
void ThrusterManager::SetThrPH(def_idx d_idx, PROPELLANT_HANDLE newph) {
	VB1->SetThrusterResource(thr_defs[d_idx].th, newph);
	return;
}
void ThrusterManager::SetThrIsp(def_idx d_idx, double newisp0) {
	VB1->SetThrusterIsp(thr_defs[d_idx].th, newisp0, thr_defs[d_idx].isp_ref, thr_defs[d_idx].p_ref);
	return;
}
void ThrusterManager::SetThrIspRef(def_idx d_idx, double newispref) {
	thr_defs[d_idx].isp_ref = newispref;
	return;
}
void ThrusterManager::SetThrPRef(def_idx d_idx, double newPref) {
	thr_defs[d_idx].p_ref = newPref;
	return;
}
DWORD ThrusterManager::GetThrCount() {
	return VB1->GetThrusterCount();
}
THRUSTER_HANDLE ThrusterManager::GetThrTH(def_idx d_idx) {
	return thr_defs[d_idx].th;
}

bool ThrusterManager::HasThrExhausts(def_idx d_idx) {
	if (thr_defs[d_idx].Exhausts_def.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}
void ThrusterManager::AddThrExhaust(def_idx d_idx, double length, double width, SURFHANDLE tex, bool customposdir, VECTOR3 pos, VECTOR3 dir) {
	THEX_DEF ed = THEX_DEF();
	//oapiWriteLogV("l:%.3f w:%.3f csd:%i pos: %.3f %.3f %.3f dir: %.3f %.3f %.3f",)
	ed.es->lsize = length;
	ed.es->wsize = width;
	ed.es->modulate = 0;
	ed.es->tex = tex;
	ed.es->th = thr_defs[d_idx].th;
	ed.es->level = NULL;
	ed.es->lofs = 0;
	ed.customposdir = customposdir;
	if (customposdir) {
		ed.es->flags = EXHAUST_CONSTANTDIR | EXHAUST_CONSTANTPOS;
		ed.es->lpos = &pos;
		ed.es->ldir = &dir;
		ed.pos = pos;
		ed.dir = dir;
	}
	else {
		ed.es->lpos = NULL;
		ed.es->ldir = NULL;
	}
	ed.es->id=VB1->AddExhaust(ed.es);
	thr_defs[d_idx].Exhausts_def.push_back(ed);
	return;
}
void ThrusterManager::RemoveThrExhaust(def_idx d_idx, def_idx ex_idx) {
	VB1->DelExhaust(thr_defs[d_idx].Exhausts_def[ex_idx].es->id);
	delete thr_defs[d_idx].Exhausts_def[ex_idx].es;
	thr_defs[d_idx].Exhausts_def.erase(thr_defs[d_idx].Exhausts_def.begin() + ex_idx);
	return;
}
bool ThrusterManager::IsThrExhsCustomposdir(def_idx d_idx, def_idx ex_idx) {
	return thr_defs[d_idx].Exhausts_def[ex_idx].customposdir;
}
void ThrusterManager::GetThrExParams(def_idx d_idx, def_idx ex_idx, double &lsize, double &wsize, SURFHANDLE &tex, bool &customposdir, VECTOR3 &pos, VECTOR3 &dir) {
	lsize = thr_defs[d_idx].Exhausts_def[ex_idx].es->lsize;
	wsize = thr_defs[d_idx].Exhausts_def[ex_idx].es->wsize;
	tex = thr_defs[d_idx].Exhausts_def[ex_idx].es->tex;
	customposdir = thr_defs[d_idx].Exhausts_def[ex_idx].customposdir;
	if (customposdir) {
		//pos = *thr_defs[d_idx].Exhausts_def[ex_idx].es->lpos;
		//dir = *thr_defs[d_idx].Exhausts_def[ex_idx].es->ldir;
		pos = thr_defs[d_idx].Exhausts_def[ex_idx].pos;
		dir = thr_defs[d_idx].Exhausts_def[ex_idx].dir;
	}
	else {
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 1);
	}
	return;
}
UINT ThrusterManager::GetThrExCount(def_idx d_idx) {
	return thr_defs[d_idx].Exhausts_def.size(); 
}
bool ThrusterManager::HasThrParticles(def_idx d_idx) {
	if (thr_defs[d_idx].particles_def.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}
void ThrusterManager::AddThrParticle(def_idx d_idx, UINT pss_index, bool custompos, VECTOR3 pos) {
	PARTICLESTREAMSPEC pss = VB1->PartMng->GetPArticleDefSpecs(pss_index);
	THPART_DEF thd = THPART_DEF();
	thd.custompos = custompos;
	thd.pos = pos;
	thd.pss_idx = pss_index;
	if (custompos) {
		thd.psh = VB1->AddExhaustStream(thr_defs[d_idx].th, pos, &pss);
	}
	else {
		thd.psh = VB1->AddExhaustStream(thr_defs[d_idx].th, &pss);
	}
	thr_defs[d_idx].particles_def.push_back(thd);
	return;

}
void ThrusterManager::RemoveThrParticle(def_idx d_idx, def_idx part_idx) {
	VB1->DelExhaustStream(thr_defs[d_idx].particles_def[part_idx].psh);
	thr_defs[d_idx].particles_def.erase(thr_defs[d_idx].particles_def.begin() + part_idx);
	return;
}
void ThrusterManager::GetThrParticleParams(def_idx d_idx, def_idx part_idx, UINT &pss_index, bool &custompos, VECTOR3 &pos) {
	pss_index = thr_defs[d_idx].particles_def[part_idx].pss_idx;
	custompos = thr_defs[d_idx].particles_def[part_idx].custompos;
	pos = thr_defs[d_idx].particles_def[part_idx].pos;
	return;
}
UINT ThrusterManager::GetThrParticlesCount(def_idx d_idx) {
	return thr_defs[d_idx].particles_def.size();
}
VBVector<UINT> ThrusterManager::GetThrParticlesIDs(def_idx d_idx) {
	VBVector<UINT> p_ids;
	p_ids.clear();
	for (UINT i = 0; i < thr_defs[d_idx].particles_def.size(); i++) {
		p_ids.push_back(thr_defs[d_idx].particles_def[i].pss_idx);
	}
	return p_ids;
}

void ThrusterManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Thrusters Section");
	UINT thr_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "THR_%i_ID", thr_counter);
	int id;
	while(oapiReadItem_int(fh,cbuf,id)){
		VECTOR3 pos, dir;
		double Max0, isp, ispref, pref;
		int ph_idx;
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "THR_%i_NAME", thr_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "THR_%i_POS", thr_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "THR_%i_DIR", thr_counter);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "THR_%i_MAX0", thr_counter);
		oapiReadItem_float(fh, cbuf, Max0);
		sprintf(cbuf, "THR_%i_ISP0", thr_counter);
		oapiReadItem_float(fh, cbuf, isp);
		sprintf(cbuf, "THR_%i_ISPREF", thr_counter);
		oapiReadItem_float(fh, cbuf, ispref);
		sprintf(cbuf, "THR_%i_PREF", thr_counter);
		oapiReadItem_float(fh, cbuf, pref);
		sprintf(cbuf, "THR_%i_TANK", thr_counter);
		oapiReadItem_int(fh, cbuf, ph_idx);
		PROPELLANT_HANDLE ph = NULL;
		if (ph_idx != -1) {
			ph = VB1->PrpMng->GetTankPH(ph_idx);
		}
		UINT thidx = AddThrDef(name, pos, dir, Max0, ph, isp, ispref, pref);
		
		bool hasexh;
		sprintf(cbuf, "THR_%i_HASEXHAUST", thr_counter);
		if (!oapiReadItem_bool(fh, cbuf, hasexh)) { hasexh = false; }
		if (hasexh) {
			UINT ex_counter = 0;
			int ex_id;
			sprintf(cbuf, "THR_%i_EX_%i_ID", thr_counter, ex_counter);
			while(oapiReadItem_int(fh,cbuf,ex_id))
			{
				double lsize, wsize;
				bool customposdir;
				VECTOR3 pos, dir;
				int extex;
				sprintf(cbuf, "THR_%i_EX_%i_LSIZE", thr_counter, ex_counter);
				oapiReadItem_float(fh, cbuf, lsize);
				sprintf(cbuf, "THR_%i_EX_%i_WSIZE", thr_counter, ex_counter);
				oapiReadItem_float(fh, cbuf, wsize);
				sprintf(cbuf, "THR_%i_EX_%i_EXTEX", thr_counter, ex_counter);
				oapiReadItem_int(fh, cbuf, extex);
				sprintf(cbuf, "THR_%i_EX_%i_CUSTOMPOSDIR", thr_counter, ex_counter);
				oapiReadItem_bool(fh, cbuf, customposdir);
				if (customposdir) {
					sprintf(cbuf, "THR_%i_EX_%i_POS", thr_counter, ex_counter);
					oapiReadItem_vec(fh, cbuf, pos);
					sprintf(cbuf, "THR_%i_EX_%i_DIR", thr_counter, ex_counter);
					oapiReadItem_vec(fh, cbuf, dir);
				}
				SURFHANDLE tex = extex == -1 ? NULL:VB1->ExTMng->GetExTexSurf(extex);
				AddThrExhaust(thidx, lsize, wsize, tex, customposdir, pos, dir);
				ex_counter++;
				sprintf(cbuf, "THR_%i_EX_%i_ID", thr_counter, ex_counter);
			}
		}
		bool haspart;
		sprintf(cbuf, "THR_%i_HASPARTICLES", thr_counter);
		if (!oapiReadItem_bool(fh, cbuf, haspart)) { haspart = false; }
		if (haspart) {
			UINT part_counter = 0;
			int part_id;
			sprintf(cbuf, "THR_%i_PART_%i_ID", thr_counter, part_counter);
			while(oapiReadItem_int(fh,cbuf,part_id))
			{
				UINT pss_index;
				bool custompos;
				VECTOR3 pos;
				sprintf(cbuf, "THR_%i_PART_%i_PSSIDX", thr_counter, part_counter);
				int pss_int;
				oapiReadItem_int(fh, cbuf, pss_int);
				pss_index = (UINT)pss_int;
				sprintf(cbuf, "THR_%i_PART_%i_CUSTOMPOS", thr_counter, part_counter);
				oapiReadItem_bool(fh, cbuf, custompos);
				if (custompos) {
					sprintf(cbuf, "THR_%i_PART_%i_POS", thr_counter, part_counter);
					oapiReadItem_vec(fh, cbuf, pos);
				}
				AddThrParticle(thidx, pss_index, custompos, pos);
				part_counter++;
				sprintf(cbuf, "THR_%i_PART_%i_ID", thr_counter, part_counter);
			}
		}

		thr_counter++;
		sprintf(cbuf, "THR_%i_ID", thr_counter);

	}
	LogV("Parsing Thrusters Section Completed, found:%i Thrusters",thr_counter);
	return;
}
void ThrusterManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------THRUSTERS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (DWORD i = 0; i < GetThrCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "THR_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "THR_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s",GetThrName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "THR_%i_POS", i);
		oapiWriteItem_vec(fh, cbuf, GetThrPos(i));
		sprintf(cbuf, "THR_%i_DIR", i);
		oapiWriteItem_vec(fh, cbuf, GetThrDir(i));
		sprintf(cbuf, "THR_%i_MAX0", i);
		oapiWriteItem_float(fh, cbuf, GetThrMax0(i));
		sprintf(cbuf, "THR_%i_ISP0", i);
		oapiWriteItem_float(fh, cbuf, GetThrIsp0(i));
		sprintf(cbuf, "THR_%i_ISPREF", i);
		oapiWriteItem_float(fh, cbuf, GetThrIspRef(i));
		sprintf(cbuf, "THR_%i_PREF", i);
		oapiWriteItem_float(fh, cbuf, GetThrPRef(i));
		sprintf(cbuf, "THR_%i_TANK", i);
		oapiWriteItem_int(fh, cbuf, VB1->PrpMng->GetPrpIdx(GetThrPh(i)));
		sprintf(cbuf, "THR_%i_HASEXHAUST", i);
		bool hasexh = HasThrExhausts(i);
		oapiWriteItem_bool(fh, cbuf, hasexh);
		if (hasexh) {
			for (UINT j = 0; j < GetThrExCount(i); j++) {
				double lsize, wsize;
				SURFHANDLE tex;
				bool customposdir;
				VECTOR3 pos, dir;
				GetThrExParams(i, j, lsize, wsize, tex, customposdir, pos, dir);
				UINT extex_idx = VB1->ExTMng->GetExTexIdx(tex);
				sprintf(cbuf, "THR_%i_EX_%i_ID", i, j);
				oapiWriteItem_int(fh, cbuf, j);
				sprintf(cbuf, "THR_%i_EX_%i_LSIZE", i, j);
				oapiWriteItem_float(fh, cbuf, lsize);
				sprintf(cbuf, "THR_%i_EX_%i_WSIZE", i, j);
				oapiWriteItem_float(fh, cbuf, wsize);
				sprintf(cbuf, "THR_%i_EX_%i_EXTEX", i, j);
				oapiWriteItem_int(fh, cbuf, extex_idx);
				sprintf(cbuf, "THR_%i_EX_%i_CUSTOMPOSDIR", i, j);
				oapiWriteItem_bool(fh, cbuf, customposdir);
				if (customposdir) {
					sprintf(cbuf, "THR_%i_EX_%i_POS", i, j);
					oapiWriteItem_vec(fh, cbuf, pos);
					sprintf(cbuf, "THR_%i_EX_%i_DIR", i, j);
					oapiWriteItem_vec(fh, cbuf, dir);
				}
			}	
		}
		sprintf(cbuf, "THR_%i_HASPARTICLES", i);
		bool haspart = HasThrParticles(i);
		oapiWriteItem_bool(fh, cbuf, haspart);
		if (haspart) {
			for (UINT j = 0; j < GetThrParticlesCount(i); j++) {
				UINT pss_index;
				bool custompos;
				VECTOR3 pos;
				GetThrParticleParams(i, j, pss_index, custompos, pos);
				sprintf(cbuf, "THR_%i_PART_%i_ID", i, j);
				oapiWriteItem_int(fh, cbuf, j);
				sprintf(cbuf, "THR_%i_PART_%i_PSSIDX", i, j);
				oapiWriteItem_int(fh, cbuf, pss_index);
				sprintf(cbuf, "THR_%i_PART_%i_CUSTOMPOS", i, j);
				oapiWriteItem_bool(fh, cbuf, custompos);
				if (custompos) {
					sprintf(cbuf, "THR_%i_PART_%i_POS", i, j);
					oapiWriteItem_vec(fh, cbuf, pos);
				}
			}
		}

		oapiWriteLine(fh, " ");
	}
	return;
}

void ThrusterManager::ToggleThrusterTest(def_idx d_idx) {
	if (thr_defs[d_idx].testing) {
		thr_defs[d_idx].testing = false;
		VB1->SetThrusterLevel(thr_defs[d_idx].th, 0);
	}
	else {
		thr_defs[d_idx].testing = true;
		VB1->SetThrusterLevel(thr_defs[d_idx].th, 1);
	}
	return;
}
bool ThrusterManager::ThrIsTesting(def_idx d_idx) {
	return thr_defs[d_idx].testing;
}

UINT ThrusterManager::GetThrIdx(THRUSTER_HANDLE th) {
	for (UINT i = 0; i < thr_defs.size(); i++) {
		if (thr_defs[i].th == th) {
			return i;
		}
	}
	return (UINT)-1;
}
/*UINT ThrusterManager::GetThrParticlesCount(def_idx d_idx) {
	return thr_defs[d_idx].particles_id.size();
}
VBVector<UINT> ThrusterManager::GetThrParticlesIDs(def_idx d_idx) {
	return thr_defs[d_idx].particles_id;
}
VBVector<PSTREAM_HANDLE> ThrusterManager::GetThrParticlesH(def_idx d_idx) {
	return thr_defs[d_idx].particles_h;
}
void ThrusterManager::SetThrParticles(def_idx d_idx, VBVector<UINT> _ids) {
	thr_defs[d_idx].particles_id = _ids;
	for (UINT i = 0; i < _ids.size(); i++) {
		def_idx d = VB1->PartMng->IdxID2Def(_ids[i]);
		PSTREAM_HANDLE psh = VB1->AddExhaustStream(thr_defs[d_idx].th, &VB1->PartMng->GetPArticleDefSpecs(d));
		thr_defs[d_idx].particles_h.push_back(psh);
	}
	return;
}
void ThrusterManager::ClearThrParticles(def_idx d_idx) {
	for (UINT i = 0; i < thr_defs[d_idx].particles_h.size(); i++) {
		VB1->DelExhaustStream(thr_defs[d_idx].particles_h[i]);
	}
	thr_defs[d_idx].particles_h.clear();
	thr_defs[d_idx].particles_id.clear();
	return;
}
void ThrusterManager::AddThrParticles(def_idx d_idx, def_idx particle_idx) {
	UINT _id = VB1->PartMng->GetParticleDefID(particle_idx);
	thr_defs[d_idx].particles_id.push_back(_id);
	PSTREAM_HANDLE psh = VB1->AddExhaustStream(thr_defs[d_idx].th, &VB1->PartMng->GetPArticleDefSpecs(particle_idx));
	thr_defs[d_idx].particles_h.push_back(psh);
	
	return;
}*/
VECTOR3* ThrusterManager::GetThrPosPtr(def_idx d_idx) {
	return thr_defs[d_idx].pos_ptr;
}
VECTOR3* ThrusterManager::GetThrDirPtr(def_idx d_idx) {
	return thr_defs[d_idx].dir_ptr;
}
void ThrusterManager::Clear() {
	LogV("Clearing Thrusters Section");
	VB1->ClearThrusterDefinitions();
	thr_defs.clear();
	LogV("Clearing Thrusters Section Completed");
}





ThrusterGroupManager::ThrusterGroupManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	Defined[THGROUP_MAIN] = false;
	Defined[THGROUP_RETRO] = false;
	Defined[THGROUP_HOVER] = false;
	Defined[THGROUP_ATT_PITCHUP] = false;
	Defined[THGROUP_ATT_PITCHDOWN] = false;
	Defined[THGROUP_ATT_YAWLEFT] = false;
	Defined[THGROUP_ATT_YAWRIGHT] = false;
	Defined[THGROUP_ATT_BANKLEFT] = false;
	Defined[THGROUP_ATT_BANKRIGHT] = false;
	Defined[THGROUP_ATT_RIGHT] = false;
	Defined[THGROUP_ATT_LEFT] = false;
	Defined[THGROUP_ATT_UP] = false;
	Defined[THGROUP_ATT_DOWN] = false;
	Defined[THGROUP_ATT_FORWARD] = false;
	Defined[THGROUP_ATT_BACK] = false;
	return;
}
ThrusterGroupManager::~ThrusterGroupManager() {}
void ThrusterGroupManager::DefineGroup(THGROUP_TYPE thgt, VBVector<THRUSTER_HANDLE>thrusters) {
	Defined[thgt] = true;
	Thrusters[thgt] = thrusters;
	THRUSTER_HANDLE* th = new THRUSTER_HANDLE[thrusters.size()];
	for (UINT i = 0; i < thrusters.size(); i++) {
		th[i] = thrusters[i];
	}
	VB1->CreateThrusterGroup(th, thrusters.size(), thgt);
	return;
}
void ThrusterGroupManager::UndefineGroup(THGROUP_TYPE thgt) {
	VB1->DelThrusterGroup(thgt, false);
	Defined[thgt] = false;
	Thrusters[thgt].clear();//indeciso, serve? oppure è meglio lasciare quelli se magari ne devo aggiungere 1?
	return;
}

bool ThrusterGroupManager::IsGroupDefined(THGROUP_TYPE thgt) {
	return Defined[thgt];
}
VBVector<THRUSTER_HANDLE> ThrusterGroupManager::GetThrusters(THGROUP_TYPE thgt) {
	return Thrusters[thgt];
}
VBVector<THRUSTER_HANDLE> ThrusterGroupManager::GetThrustersfromIdx(VBVector<UINT> idx) {
	VBVector<THRUSTER_HANDLE> thv;
	thv.clear();
	for (UINT i = 0; i < idx.size(); i++) {
		THRUSTER_HANDLE th = VB1->ThrMng->GetThrTH(idx[i]);
		if (th == NULL) { continue; }
		thv.push_back(th);
	}
	return thv;
}
void ThrusterGroupManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Thruster Group Section");
	bool defined;
	if (!oapiReadItem_bool(fh, "THGROUP_MAIN", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_MAIN_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_MAIN, thv);
	}
	
	if (!oapiReadItem_bool(fh, "THGROUP_RETRO", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_RETRO_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_RETRO, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_HOVER", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_HOVER_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_HOVER, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_PITCHUP", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_PITCHUP_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_PITCHUP, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_PITCHDOWN", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_PITCHDOWN_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_PITCHDOWN, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_YAWLEFT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_YAWLEFT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_YAWLEFT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_YAWRIGHT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_YAWRIGHT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_YAWRIGHT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_BANKLEFT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_BANKLEFT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_BANKLEFT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_BANKRIGHT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_BANKRIGHT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_BANKRIGHT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_RIGHT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_RIGHT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_RIGHT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_LEFT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_LEFT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_LEFT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_UP", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_UP_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_UP, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_DOWN", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_DOWN_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_DOWN, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_FORWARD", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_FORWARD_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_FORWARD, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_BACK", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_BACK_THRUSTERS", cbuf);
		string idx_s(cbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		VBVector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_BACK, thv);
	}

	LogV("Parsing Thruster Group Section Completed");
	return;
}
void ThrusterGroupManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, ";<-------------------------THRUSTER GROUPS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	char cbuf[256] = { '\0' };
	VBVector<THRUSTER_HANDLE>thrusters;
	VBVector<UINT>indexes;
	thrusters.clear();
	indexes.clear();
	string thr_s;
	thr_s.clear();

	if (Defined[THGROUP_MAIN]) {
		oapiWriteItem_bool(fh, "THGROUP_MAIN", Defined[THGROUP_MAIN]);
		indexes.clear();
		thrusters = GetThrusters(THGROUP_MAIN);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_MAIN_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
	}
	
	if (Defined[THGROUP_RETRO]) {
		oapiWriteItem_bool(fh, "THGROUP_RETRO", Defined[THGROUP_RETRO]);
		thrusters = GetThrusters(THGROUP_RETRO);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_RETRO_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();

	}
	
	if (Defined[THGROUP_HOVER]) {
		oapiWriteItem_bool(fh, "THGROUP_HOVER", Defined[THGROUP_HOVER]);
		thrusters = GetThrusters(THGROUP_HOVER);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_HOVER_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();

	}
	
	if (Defined[THGROUP_ATT_PITCHUP]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_PITCHUP", Defined[THGROUP_ATT_PITCHUP]);
		thrusters = GetThrusters(THGROUP_ATT_PITCHUP);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_PITCHUP_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_PITCHDOWN]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_PITCHDOWN", Defined[THGROUP_ATT_PITCHDOWN]);
		thrusters = GetThrusters(THGROUP_ATT_PITCHDOWN);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_PITCHDOWN_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_YAWLEFT]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_YAWLEFT", Defined[THGROUP_ATT_YAWLEFT]);
		thrusters = GetThrusters(THGROUP_ATT_YAWLEFT);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_YAWLEFT_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_YAWRIGHT]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_YAWRIGHT", Defined[THGROUP_ATT_YAWRIGHT]);
		thrusters = GetThrusters(THGROUP_ATT_YAWRIGHT);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_YAWRIGHT_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_BANKLEFT]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_BANKLEFT", Defined[THGROUP_ATT_BANKLEFT]);
		thrusters = GetThrusters(THGROUP_ATT_BANKLEFT);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_BANKLEFT_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_BANKRIGHT]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_BANKRIGHT", Defined[THGROUP_ATT_BANKRIGHT]);
		thrusters = GetThrusters(THGROUP_ATT_BANKRIGHT);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_BANKRIGHT_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_RIGHT]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_RIGHT", Defined[THGROUP_ATT_RIGHT]);
		thrusters = GetThrusters(THGROUP_ATT_RIGHT);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_RIGHT_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_LEFT]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_LEFT", Defined[THGROUP_ATT_LEFT]);
		thrusters = GetThrusters(THGROUP_ATT_LEFT);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_LEFT_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_UP]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_UP", Defined[THGROUP_ATT_UP]);
		thrusters = GetThrusters(THGROUP_ATT_UP);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_UP_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_DOWN]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_DOWN", Defined[THGROUP_ATT_DOWN]);
		thrusters = GetThrusters(THGROUP_ATT_DOWN);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_DOWN_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_FORWARD]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_FORWARD", Defined[THGROUP_ATT_FORWARD]);
		thrusters = GetThrusters(THGROUP_ATT_FORWARD);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_FORWARD_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	
	if (Defined[THGROUP_ATT_BACK]) {
		oapiWriteItem_bool(fh, "THGROUP_ATT_BACK", Defined[THGROUP_ATT_BACK]);
		thrusters = GetThrusters(THGROUP_ATT_BACK);
		for (UINT i = 0; i < thrusters.size(); i++) {
			UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
			if (idx != (UINT)-1) {
				indexes.push_back(idx);
			}
		}
		thr_s = VB1->WriteVectorUINT(indexes);
		sprintf(cbuf, "%s", thr_s.c_str());
		oapiWriteItem_string(fh, "THGROUP_ATT_BACK_THRUSTERS", cbuf);
		indexes.clear();
		thrusters.clear();
		thr_s.clear();
	}
	

	return;
}

void ThrusterGroupManager::ResetDefine() {
	Defined[THGROUP_MAIN] = false;
	Defined[THGROUP_RETRO] = false;
	Defined[THGROUP_HOVER] = false;
	Defined[THGROUP_ATT_PITCHUP] = false;
	Defined[THGROUP_ATT_PITCHDOWN] = false;
	Defined[THGROUP_ATT_YAWLEFT] = false;
	Defined[THGROUP_ATT_YAWRIGHT] = false;
	Defined[THGROUP_ATT_BANKLEFT] = false;
	Defined[THGROUP_ATT_BANKRIGHT] = false;
	Defined[THGROUP_ATT_RIGHT] = false;
	Defined[THGROUP_ATT_LEFT] = false;
	Defined[THGROUP_ATT_UP] = false;
	Defined[THGROUP_ATT_DOWN] = false;
	Defined[THGROUP_ATT_FORWARD] = false;
	Defined[THGROUP_ATT_BACK] = false;
	return;
}