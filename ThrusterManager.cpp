#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "PropellantManager.h"
#include "ParticleManager.h"
#include "ExTexManager.h"
#include "ThrusterManager.h"

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
	THR_DEF thr = THR_DEF();
	thr.name = name;
	thr.th = VB1->CreateThruster(pos, dir, maxth, ph, isp0, isp_ref, p_ref);
	thr.pos = pos;
	*thr.pos_ptr = pos;
	thr.antidir = dir*(-1);
	*thr.antidir_ptr = dir*(-1);
	thr_defs.push_back(thr);
	return thr_defs.size()-1;
}
void ThrusterManager::DelThrDef(def_idx d_idx) {
	VB1->DelThruster(thr_defs[d_idx].th);
	delete thr_defs[d_idx].antidir_ptr;
	delete thr_defs[d_idx].pos_ptr;
	thr_defs.erase(thr_defs.begin() + d_idx);
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
	thr_defs[d_idx].antidir = newdir*(-1);
	*thr_defs[d_idx].antidir_ptr = newdir*(-1);
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
void ThrusterManager::ParseCfgFile(FILEHANDLE fh) {
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

		bool hasexhaust;
		double ex_l, ex_w;
		int ex_idx;

		sprintf(cbuf, "THR_%i_HASEXHAUST", thr_counter);
		oapiReadItem_bool(fh, cbuf, hasexhaust);
		if (hasexhaust) {
			sprintf(cbuf, "THR_%i_EX_L", thr_counter);
			oapiReadItem_float(fh, cbuf, ex_l);
			sprintf(cbuf, "THR_%i_EX_W", thr_counter);
			oapiReadItem_float(fh, cbuf, ex_w);
			sprintf(cbuf, "THR_%i_EX_TEX", thr_counter);
			oapiReadItem_int(fh, cbuf, ex_idx);
			if (ex_idx > -1) {
				SetThrExhaust(thidx, ex_l, ex_w, VB1->ExTMng->GetExTexSurf(ex_idx));
			}
			else {
				SetThrExhaust(thidx, ex_l, ex_w, NULL);
			}
			
		}
		
		sprintf(cbuf, "THR_%i_PARTICLES", thr_counter);
		char partsbuf[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, partsbuf);
		string parts(partsbuf);
		vector<UINT>parts_defidx = VB1->readVectorUINT(parts);
		for (UINT z = 0; z < parts_defidx.size(); z++) {
			if (parts_defidx[z] == -1) {
				continue;
			}
			AddThrParticles(thidx, parts_defidx[z]);
		}


		thr_counter++;
		sprintf(cbuf, "THR_%i_ID", thr_counter);

	}
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
		oapiWriteItem_bool(fh, cbuf, ThrHasExhaust(i));
		sprintf(cbuf, "THR_%i_EX_L", i);
		oapiWriteItem_float(fh, cbuf, GetThrExhaustLength(i));
		sprintf(cbuf, "THR_%i_EX_W", i);
		oapiWriteItem_float(fh, cbuf, GetThrExhaustWidth(i));
		sprintf(cbuf, "THR_%i_EX_TEX", i);
		oapiWriteItem_int(fh, cbuf, VB1->ExTMng->GetExTexIdx(GetThrExhaustTex(i)));
		sprintf(cbuf, "THR_%i_PARTICLES", i);
		vector<def_idx> part_defidxs;
		part_defidxs.clear();
		vector<UINT>ids = GetThrParticlesIDs(i);
		for (UINT j = 0; j < ids.size(); j++) {
			UINT idd = VB1->PartMng->IdxID2Def(ids[j]);
			if (idd == (UINT)-1) { continue; }
			part_defidxs.push_back(idd);
		}
		string vals = VB1->WriteVectorUINT(part_defidxs);
		char val[256] = { '\0' };
		sprintf(val, "%s", vals.c_str());
		oapiWriteItem_string(fh, cbuf, val);
		oapiWriteLine(fh, " ");
	}
	return;
}

bool ThrusterManager::ThrHasExhaust(def_idx d_idx) {
	return thr_defs[d_idx].HasExhaust;
}
void ThrusterManager::SetThrExhaust(def_idx d_idx, double length,double width,SURFHANDLE tex) {
	EXHAUSTSPEC es;
	es.th = thr_defs[d_idx].th;
	es.ldir = NULL;
	es.lpos = NULL;
	es.level = NULL;
	es.lofs = 0;
	es.modulate = 0.05;
	es.tex = tex;
	es.lsize = length;
	es.wsize = width;
	thr_defs[d_idx].ExhaustID = VB1->AddExhaust(&es);
	thr_defs[d_idx].HasExhaust = true;

	return;
}
void ThrusterManager::SetThrHasExhaust(def_idx d_idx, bool set) {
	if (set) {
		if (thr_defs[d_idx].ExhaustID != (UINT)-1) {
			VB1->DelExhaust(thr_defs[d_idx].ExhaustID);
			thr_defs[d_idx].ExhaustID = (UINT)-1;
		}
		EXHAUSTSPEC es;
		es.th = thr_defs[d_idx].th;
		es.ldir = NULL;
		es.lpos = NULL;
		es.level = NULL;
		es.lofs = 0;
		es.modulate = 0.05;
		es.tex = NULL;
		es.lsize = 6;
		es.wsize = 1;
		thr_defs[d_idx].ExhaustID=VB1->AddExhaust(&es );
		thr_defs[d_idx].HasExhaust = true;
	}
	else {
		if (thr_defs[d_idx].ExhaustID != (UINT)-1) {
			VB1->DelExhaust(thr_defs[d_idx].ExhaustID);
		}
		thr_defs[d_idx].ExhaustID = (UINT)-1;
		thr_defs[d_idx].HasExhaust = false;
	}
}
void ThrusterManager::SetThrExhaustLength(def_idx d_idx, double newlength) {
	EXHAUSTSPEC es;
	VB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
	es.lsize = newlength;
	VB1->DelExhaust(thr_defs[d_idx].ExhaustID);
	thr_defs[d_idx].ExhaustID = VB1->AddExhaust(&es);
	return;
}
void ThrusterManager::SetThrExhaustWidth(def_idx d_idx, double newwidth) {
	EXHAUSTSPEC es;
	VB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
	es.wsize = newwidth;
	VB1->DelExhaust(thr_defs[d_idx].ExhaustID);
	thr_defs[d_idx].ExhaustID = VB1->AddExhaust(&es);
	return;
}
double ThrusterManager::GetThrExhaustLength(def_idx d_idx) {
	if (ThrHasExhaust(d_idx)) {
		EXHAUSTSPEC es;
		VB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
		return es.lsize;
	}
	else {
		return 0;
	}
}
double ThrusterManager::GetThrExhaustWidth(def_idx d_idx) {
	if (ThrHasExhaust(d_idx)) {
		EXHAUSTSPEC es;
		VB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
		return es.wsize;
	}
	else {
		return 0;
	}
}
SURFHANDLE ThrusterManager::GetThrExhaustTex(def_idx d_idx) {
	if (ThrHasExhaust(d_idx)) {
		EXHAUSTSPEC es;
		VB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
		return es.tex;
	}
	else {
		return NULL;
	}
}
void ThrusterManager::SetThrExhaustTex(def_idx d_idx, SURFHANDLE newtex) {
	EXHAUSTSPEC es;
	VB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
	es.tex = newtex;
	VB1->DelExhaust(thr_defs[d_idx].ExhaustID);
	thr_defs[d_idx].ExhaustID = VB1->AddExhaust(&es);
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
UINT ThrusterManager::GetThrParticlesCount(def_idx d_idx) {
	return thr_defs[d_idx].particles_id.size();
}
vector<UINT> ThrusterManager::GetThrParticlesIDs(def_idx d_idx) {
	return thr_defs[d_idx].particles_id;
}
vector<PSTREAM_HANDLE> ThrusterManager::GetThrParticlesH(def_idx d_idx) {
	return thr_defs[d_idx].particles_h;
}
void ThrusterManager::SetThrParticles(def_idx d_idx, vector<UINT> _ids) {
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
}
VECTOR3* ThrusterManager::GetThrPosPtr(def_idx d_idx) {
	return thr_defs[d_idx].pos_ptr;
}
VECTOR3* ThrusterManager::GetThrAntiDirPtr(def_idx d_idx) {
	return thr_defs[d_idx].antidir_ptr;
}
void ThrusterManager::Clear() {
	VB1->ClearThrusterDefinitions();
	thr_defs.clear();
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
void ThrusterGroupManager::DefineGroup(THGROUP_TYPE thgt, vector<THRUSTER_HANDLE>thrusters) {
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
vector<THRUSTER_HANDLE> ThrusterGroupManager::GetThrusters(THGROUP_TYPE thgt) {
	return Thrusters[thgt];
}
vector<THRUSTER_HANDLE> ThrusterGroupManager::GetThrustersfromIdx(vector<UINT> idx) {
	vector<THRUSTER_HANDLE> thv;
	thv.clear();
	for (UINT i = 0; i < idx.size(); i++) {
		THRUSTER_HANDLE th = VB1->ThrMng->GetThrTH(idx[i]);
		if (th == NULL) { continue; }
		thv.push_back(th);
	}
	return thv;
}
void ThrusterGroupManager::ParseCfgFile(FILEHANDLE fh) {
	bool defined;
	if (!oapiReadItem_bool(fh, "THGROUP_MAIN", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_MAIN_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_MAIN, thv);
	}
	
	if (!oapiReadItem_bool(fh, "THGROUP_RETRO", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_RETRO_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_RETRO, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_HOVER", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_HOVER_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_HOVER, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_PITCHUP", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_PITCHUP_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_PITCHUP, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_PITCHDOWN", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_PITCHDOWN_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_PITCHDOWN, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_YAWLEFT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_YAWLEFT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_YAWLEFT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_YAWRIGHT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_YAWRIGHT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_YAWRIGHT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_BANKLEFT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_BANKLEFT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_BANKLEFT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_BANKRIGHT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_BANKRIGHT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_BANKRIGHT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_RIGHT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_RIGHT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_RIGHT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_LEFT", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_LEFT_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_LEFT, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_UP", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_UP_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_UP, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_DOWN", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_DOWN_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_DOWN, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_FORWARD", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_FORWARD_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_FORWARD, thv);
	}

	if (!oapiReadItem_bool(fh, "THGROUP_ATT_BACK", defined)) {
		defined = false;
	}
	if (defined) {
		char cbuf[256] = { '\0' };
		oapiReadItem_string(fh, "THGROUP_ATT_BACK_THRUSTERS", cbuf);
		string idx_s(cbuf);
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
		vector<THRUSTER_HANDLE> thv = GetThrustersfromIdx(idx);
		DefineGroup(THGROUP_ATT_BACK, thv);
	}


	return;
}
void ThrusterGroupManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, ";<-------------------------THRUSTER GROUPS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	char cbuf[256] = { '\0' };
	oapiWriteItem_bool(fh, "THGROUP_MAIN", Defined[THGROUP_MAIN]);
	vector<THRUSTER_HANDLE>thrusters; 
	vector<UINT>indexes;
	indexes.clear();
	thrusters = GetThrusters(THGROUP_MAIN);
	for (UINT i = 0; i < thrusters.size(); i++) {
		UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
		if (idx != (UINT)-1) {
			indexes.push_back(idx);
		}
	}
	string thr_s = VB1->WriteVectorUINT(indexes);
	sprintf(cbuf, "%s", thr_s.c_str());
	oapiWriteItem_string(fh, "THGROUP_MAIN_THRUSTERS", cbuf);
	indexes.clear();
	thrusters.clear();
	
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