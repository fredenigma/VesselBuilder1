#include "StationBuilder1.h"
#include "DialogControl.h"
#include "PropellantManager.h"
#include "ThrusterManager.h"

ThrusterManager::ThrusterManager(StationBuilder1 *_SB1) {
	SB1 = _SB1;
	thr_defs.clear();
	return;
}
ThrusterManager::~ThrusterManager() {}
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
	thr.th = SB1->CreateThruster(pos, dir, maxth, ph, isp0, isp_ref, p_ref);
	thr.pos = pos;
	thr.antidir = dir*(-1);
	thr_defs.push_back(thr);
	return thr_defs.size()-1;
}
void ThrusterManager::DelThrDef(def_idx d_idx) {
	SB1->DelThruster(thr_defs[d_idx].th);
	thr_defs.erase(thr_defs.begin() + d_idx);
	return;
}
string ThrusterManager::GetThrName(def_idx d_idx) {
	return thr_defs[d_idx].name;
}
VECTOR3 ThrusterManager::GetThrPos(def_idx d_idx) {
	VECTOR3 pos;
	SB1->GetThrusterRef(thr_defs[d_idx].th,pos);
	return pos;
}
VECTOR3 ThrusterManager::GetThrDir(def_idx d_idx) {
	VECTOR3 dir;
	SB1->GetThrusterDir(thr_defs[d_idx].th, dir);
	return dir;
}
double ThrusterManager::GetThrMax0(def_idx d_idx) {
	return SB1->GetThrusterMax0(thr_defs[d_idx].th);
}
PROPELLANT_HANDLE ThrusterManager::GetThrPh(def_idx d_idx) {
	return SB1->GetThrusterResource(thr_defs[d_idx].th);
}
double ThrusterManager::GetThrIsp0(def_idx d_idx) {
	return SB1->GetThrusterIsp0(thr_defs[d_idx].th);
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
	SB1->SetThrusterRef(thr_defs[d_idx].th, newpos);
	thr_defs[d_idx].pos = newpos;
	return;
}
void ThrusterManager::SetThrDir(def_idx d_idx, VECTOR3 newdir) {
	SB1->SetThrusterDir(thr_defs[d_idx].th, newdir);
	thr_defs[d_idx].antidir = newdir*(-1);
	return;
}
void ThrusterManager::SetThrMax0(def_idx d_idx, double newMax0) {
	SB1->SetThrusterMax0(thr_defs[d_idx].th, newMax0);
	return;
}
void ThrusterManager::SetThrPH(def_idx d_idx, PROPELLANT_HANDLE newph) {
	SB1->SetThrusterResource(thr_defs[d_idx].th, newph);
	return;
}
void ThrusterManager::SetThrIsp(def_idx d_idx, double newisp0) {
	SB1->SetThrusterIsp(thr_defs[d_idx].th, newisp0, thr_defs[d_idx].isp_ref, thr_defs[d_idx].p_ref);
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
	return SB1->GetThrusterCount();
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
			ph = SB1->PrpMng->GetTankPH(ph_idx);
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
				SetThrExhaust(thidx, ex_l, ex_w, SB1->GetExTexSurf(ex_idx));
			}
			else {
				SetThrExhaust(thidx, ex_l, ex_w, NULL);
			}
			
		}
		


		thr_counter++;
		sprintf(cbuf, "THR_%i_ID", thr_counter);

	}
	return;
}
void ThrusterManager::WriteCfg(FILEHANDLE fh) {
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
		oapiWriteItem_int(fh, cbuf, SB1->PrpMng->GetPrpIdx(GetThrPh(i)));
		sprintf(cbuf, "THR_%i_HASEXHAUST", i);
		oapiWriteItem_bool(fh, cbuf, ThrHasExhaust(i));
		sprintf(cbuf, "THR_%i_EX_L", i);
		oapiWriteItem_float(fh, cbuf, GetThrExhaustLength(i));
		sprintf(cbuf, "THR_%i_EX_W", i);
		oapiWriteItem_float(fh, cbuf, GetThrExhaustWidth(i));
		sprintf(cbuf, "THR_%i_EX_TEX", i);
		oapiWriteItem_int(fh, cbuf, SB1->GetExTexIdx(GetThrExhaustTex(i)));
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
	thr_defs[d_idx].ExhaustID = SB1->AddExhaust(&es);
	thr_defs[d_idx].HasExhaust = true;

	return;
}
void ThrusterManager::SetThrHasExhaust(def_idx d_idx, bool set) {
	if (set) {
		if (thr_defs[d_idx].ExhaustID != (UINT)-1) {
			SB1->DelExhaust(thr_defs[d_idx].ExhaustID);
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
		thr_defs[d_idx].ExhaustID=SB1->AddExhaust(&es );
		thr_defs[d_idx].HasExhaust = true;
	}
	else {
		if (thr_defs[d_idx].ExhaustID != (UINT)-1) {
			SB1->DelExhaust(thr_defs[d_idx].ExhaustID);
		}
		thr_defs[d_idx].ExhaustID = (UINT)-1;
		thr_defs[d_idx].HasExhaust = false;
	}
}
void ThrusterManager::SetThrExhaustLength(def_idx d_idx, double newlength) {
	EXHAUSTSPEC es;
	SB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
	es.lsize = newlength;
	SB1->DelExhaust(thr_defs[d_idx].ExhaustID);
	thr_defs[d_idx].ExhaustID = SB1->AddExhaust(&es);
	return;
}
void ThrusterManager::SetThrExhaustWidth(def_idx d_idx, double newwidth) {
	EXHAUSTSPEC es;
	SB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
	es.wsize = newwidth;
	SB1->DelExhaust(thr_defs[d_idx].ExhaustID);
	thr_defs[d_idx].ExhaustID = SB1->AddExhaust(&es);
	return;
}
double ThrusterManager::GetThrExhaustLength(def_idx d_idx) {
	if (ThrHasExhaust(d_idx)) {
		EXHAUSTSPEC es;
		SB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
		return es.lsize;
	}
	else {
		return 0;
	}
}
double ThrusterManager::GetThrExhaustWidth(def_idx d_idx) {
	if (ThrHasExhaust(d_idx)) {
		EXHAUSTSPEC es;
		SB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
		return es.wsize;
	}
	else {
		return 0;
	}
}
SURFHANDLE ThrusterManager::GetThrExhaustTex(def_idx d_idx) {
	if (ThrHasExhaust(d_idx)) {
		EXHAUSTSPEC es;
		SB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
		return es.tex;
	}
	else {
		return NULL;
	}
}
void ThrusterManager::SetThrExhaustTex(def_idx d_idx, SURFHANDLE newtex) {
	EXHAUSTSPEC es;
	SB1->GetExhaustSpec(thr_defs[d_idx].ExhaustID, &es);
	es.tex = newtex;
	SB1->DelExhaust(thr_defs[d_idx].ExhaustID);
	thr_defs[d_idx].ExhaustID = SB1->AddExhaust(&es);
	return;
}

void ThrusterManager::ToggleThrusterTest(def_idx d_idx) {
	if (thr_defs[d_idx].testing) {
		thr_defs[d_idx].testing = false;
		SB1->SetThrusterLevel(thr_defs[d_idx].th, 0);
	}
	else {
		thr_defs[d_idx].testing = true;
		SB1->SetThrusterLevel(thr_defs[d_idx].th, 1);
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

ThrusterGroupManager::ThrusterGroupManager(StationBuilder1 *_SB1) {
	SB1 = _SB1;
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
	SB1->CreateThrusterGroup(th, thrusters.size(), thgt);
	return;
}
void ThrusterGroupManager::UndefineGroup(THGROUP_TYPE thgt) {
	SB1->DelThrusterGroup(thgt, false);
	Defined[thgt] = false;
	Thrusters[thgt].clear();//indeciso, serve? oppure è meglio lasciare quelli se magari ne devo aggiungere 1?
	return;
}