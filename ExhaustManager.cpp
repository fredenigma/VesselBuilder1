#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "ExhaustManager.h"
#include "ThrusterManager.h"
#include "ExTexManager.h"

ExhaustManager::ExhaustManager(VesselBuilder1* _VB1) {
	VB1 = _VB1;
	ex_def.clear();
	return;
}
ExhaustManager::~ExhaustManager() { VB1 = NULL; }


void ExhaustManager::AddUndefinedExDef() {
	EX_DEF ed = EX_DEF();
	UINT index = ex_def.size(); 
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Exhaust_%i", index);
	string name(cbuf);
	ed.name = name;
	ex_def.push_back(ed);
	return;
}
void ExhaustManager::DefineExDef(def_idx d_idx, UINT thr_index, double lsize, double wsize, bool customposdir, bool customtex, VECTOR3 pos, VECTOR3 dir, UINT extex) {
	ex_def[d_idx].es->th = VB1->ThrMng->GetThrTH(thr_index);
	ex_def[d_idx].es->level = NULL;
	ex_def[d_idx].es->lsize = lsize;
	ex_def[d_idx].es->wsize = wsize;
	if (!customposdir) {
		ex_def[d_idx].es->flags = NULL;
		ex_def[d_idx].es->lpos = NULL;
		ex_def[d_idx].es->ldir = NULL;
	}
	else {
		VECTOR3 ppos = pos;
		VECTOR3 ddir = dir;
		ex_def[d_idx].es->flags = EXHAUST_CONSTANTDIR | EXHAUST_CONSTANTPOS;
		ex_def[d_idx].es->lpos = &ppos;
		ex_def[d_idx].es->ldir = &ddir;
	}
	ex_def[d_idx].es->lofs = 0;
	ex_def[d_idx].es->modulate = 0;
	if (!customtex) {
		ex_def[d_idx].es->tex = NULL;
	}
	else {
		ex_def[d_idx].es->tex = VB1->ExTMng->GetExTexSurf(extex);
	}
	ex_def[d_idx].ID = VB1->AddExhaust(ex_def[d_idx].es);
	ex_def[d_idx].Defined = true;
	return;
}
UINT ExhaustManager::AddExDef(string name, UINT thr_index, double lsize, double wsize, bool customposdir, bool customtex, VECTOR3 pos, VECTOR3 dir, UINT extex) {
	EX_DEF ed = EX_DEF();
	ed.name = name;
	/*ed.es->th = VB1->ThrMng->GetThrTH(thr_index);
	ed.es->level = NULL;
	ed.es->lsize = lsize;
	ed.es->wsize = wsize;
	if (!customposdir) {
		ed.es->flags = NULL;
		ed.es->lpos = NULL;
		ed.es->ldir = NULL;
	}
	else {
		VECTOR3 ppos = pos;
		VECTOR3 ddir = dir;
		ed.es->flags = EXHAUST_CONSTANTDIR | EXHAUST_CONSTANTPOS;
		ed.es->lpos = &ppos;
		ed.es->ldir = &ddir;
	}
	ed.es->lofs = 0;
	ed.es->modulate = 0;
	if (!customtex) {
		ed.es->tex = NULL;
	}
	else {
		ed.es->tex = VB1->ExTMng->GetExTexSurf(extex);
	}
	UINT index = ex_def.size();
	ed.Created = true;
	ed.ID = VB1->AddExhaust(ed.es);*/
	int idx = ex_def.size();
	ex_def.push_back(ed);
	DefineExDef(idx, thr_index, lsize, wsize, customposdir, customtex, pos, dir, extex);
	return idx;
}
void ExhaustManager::UndefineExDef(def_idx d_idx) {
	ex_def[d_idx].Defined = false;
	VB1->DelExhaust(ex_def[d_idx].ID);
	return;
}
void ExhaustManager::DelExDef(def_idx d_idx) {
	if (ex_def[d_idx].Defined) {
		VB1->DelExhaust(ex_def[d_idx].ID);
	}	
	delete ex_def[d_idx].es;
	ex_def.erase(ex_def.begin() + d_idx);
	return;
}
void ExhaustManager::Clear() {
	for (UINT i = 0; i < ex_def.size(); i++) {
		if (ex_def[i].Defined) {
			VB1->DelExhaust(ex_def[i].ID);
			delete ex_def[i].es;
		}
	}
	ex_def.clear();
	return;
}

void ExhaustManager::GetExParams(def_idx d_idx, UINT &thr_index, double &lsize, double &wsize, VECTOR3 &pos, VECTOR3 &dir, UINT &extex, bool &customposdir, bool &customtex) {
	EXHAUSTSPEC es;
	for (UINT i = 0; i < VB1->GetExhaustCount(); i++) {
		VB1->GetExhaustSpec(i, &es);
		if (es.id == ex_def[d_idx].ID) {
			break;
		}
	}
	

	thr_index = VB1->ThrMng->GetThrIdx(es.th);
	lsize = es.lsize;
	wsize = es.wsize;
	if ((es.flags & EXHAUST_CONSTANTDIR) || (es.flags & EXHAUST_CONSTANTPOS)) {
		pos = *es.lpos;
		dir = *es.ldir;
		customposdir = true;
	}
	else {
		customposdir = false;
	}
	if (es.tex != NULL) {
		customtex = true;
		extex = VB1->ExTMng->GetExTexIdx(es.tex);
	}
	else {
		customtex = false;
	}
	return;
}

string ExhaustManager::GetExName(def_idx d_idx) {
	return ex_def[d_idx].name;
}
void ExhaustManager::SetExName(def_idx d_idx, string newname) {
	ex_def[d_idx].name = newname;
	return;
}
bool ExhaustManager::IsExDefined(def_idx d_idx) {
	return ex_def[d_idx].Defined;
}
UINT ExhaustManager::GetExDefCount() {
	return ex_def.size();
}
void ExhaustManager::ParseCfgFile(FILEHANDLE fh) {
	UINT ex_counter = 0;
	int id;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "EXHAUST_%i_ID", ex_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "EXHAUST_%i_NAME", ex_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		UINT thr_index, extex;
		double lsize, wsize;
		VECTOR3 pos, dir;
		bool customposdir, customtex;
		int thr;
		sprintf(cbuf, "EXHAUST_%i_TH", ex_counter);
		oapiReadItem_int(fh, cbuf, thr);
		thr_index = (UINT)thr;
		sprintf(cbuf, "EXHAUST_%i_LSIZE", ex_counter);
		oapiReadItem_float(fh, cbuf, lsize);
		sprintf(cbuf, "EXHAUST_%i_WSIZE", ex_counter);
		oapiReadItem_float(fh, cbuf, wsize);
		sprintf(cbuf, "EXHAUST_%i_CUSTOMPOSDIR", ex_counter);
		oapiReadItem_bool(fh, cbuf, customposdir);
		if (customposdir) {
			sprintf(cbuf, "EXHAUST_%i_POS", ex_counter);
			oapiReadItem_vec(fh, cbuf, pos);
			sprintf(cbuf, "EXHAUST_%i_DIR", ex_counter);
			oapiReadItem_vec(fh, cbuf, dir);
		}
		sprintf(cbuf, "EXHAUST_%i_CUSTOMTEX", ex_counter);
		oapiReadItem_bool(fh,cbuf, customtex);
		if (customtex) {
			int exi;
			sprintf(cbuf, "EXHAUST_%i_TEX", ex_counter);
			oapiReadItem_int(fh, cbuf, exi);
			extex = (UINT)exi;
		}
		AddExDef(name, thr_index, lsize, wsize, customposdir, customtex, pos, dir, extex);
		ex_counter++;
		sprintf(cbuf, "EXHAUST_%i_ID", ex_counter);
	}
		
	return;
}
void ExhaustManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------EXHAUST DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	UINT ex_counter = 0;
	for (UINT i = 0; i < GetExDefCount(); i++) {
		if (!IsExDefined(i)) { continue; }
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "EXHAUST_%i_ID", ex_counter);
		oapiWriteItem_int(fh, cbuf, i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetExName(i).c_str());
		sprintf(cbuf, "EXHAUST_%i_NAME", ex_counter);
		oapiWriteItem_string(fh, cbuf, namebuf);
		UINT thr_index;
		double lsize, wsize;
		VECTOR3 pos, dir;
		UINT extex;
		bool customposdir, customtex;
		GetExParams(i, thr_index, lsize, wsize, pos, dir, extex, customposdir, customtex);
		sprintf(cbuf, "EXHAUST_%i_TH", ex_counter);
		oapiWriteItem_int(fh, cbuf, thr_index);
		sprintf(cbuf, "EXHAUST_%i_LSIZE", ex_counter);
		oapiWriteItem_float(fh, cbuf, lsize);
		sprintf(cbuf, "EXHAUST_%i_WSIZE", ex_counter);
		oapiWriteItem_float(fh, cbuf, wsize);
		sprintf(cbuf, "EXHAUST_%i_CUSTOMPOSDIR", ex_counter);
		oapiWriteItem_bool(fh, cbuf, customposdir);
		if (customposdir) {
			sprintf(cbuf, "EXHAUST_%i_POS", ex_counter);
			oapiWriteItem_vec(fh, cbuf, pos);
			sprintf(cbuf, "EXHAUST_%i_DIR", ex_counter);
			oapiWriteItem_vec(fh, cbuf, dir);
		}
		sprintf(cbuf, "EXHAUST_%i_CUSTOMTEX", ex_counter);
		oapiWriteItem_bool(fh, cbuf, customtex);
		if (customtex) {
			sprintf(cbuf, "EXHAUST_%i_TEX", ex_counter);
			oapiWriteItem_int(fh, cbuf, extex);
		}
		oapiWriteLine(fh, " ");
		ex_counter++;
	}
	return;
}