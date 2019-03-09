#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "VariableDragManager.h"
#include "AnimationManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

VariableDragManager::VariableDragManager(VesselBuilder1* _VB1) {
	VB1 = _VB1;
	vard_def.clear();
	return;
}
VariableDragManager::~VariableDragManager() {
	VB1 = NULL;
	return;
}
void VariableDragManager::AddUndefinedVardDef() {
	VARD_DEF vd = VARD_DEF();
	vd.defined = false;
	UINT index = vard_def.size();
	char nbuf[256] = { '\0' };
	sprintf(nbuf, "VariableDrag_%i", index);
	vd.name.assign(nbuf);
	vard_def.push_back(vd);
	return;
}
UINT VariableDragManager::AddVardDef(string name, UINT anim_idx, double factor, VECTOR3 ref) {
	VARD_DEF vd = VARD_DEF();
	vd.name = name;
	vd.ref = ref;
	vd.factor = factor;
	vd.anim_idx = anim_idx;
	vd.defined = true;
	UINT index = vard_def.size();
	vard_def.push_back(vd);
	DefineVarDef(index);
	return index;
}
void VariableDragManager::DefineVarDef(def_idx d_idx) {
	double *state_ptr = VB1->AnimMng->GetAnimStatePtr(vard_def[d_idx].anim_idx);
	VB1->CreateVariableDragElement(state_ptr, vard_def[d_idx].factor, vard_def[d_idx].ref);
	vard_def[d_idx].defined = true;
	return;
}
void VariableDragManager::DefineAll() {
	for (UINT i = 0; i < GetVardDefCount(); i++) {
		if (IsVardDefDefined(i)) {
			DefineVarDef(i);
		}
	}
	return;
}
void VariableDragManager::UnDefineVardDef(def_idx d_idx) {
	vard_def[d_idx].defined = false;
	VB1->ClearVariableDragElements();
	DefineAll();
	return;
}
void VariableDragManager::DeleteVarDef(def_idx d_idx) {
	vard_def.erase(vard_def.begin() + d_idx);
	VB1->ClearVariableDragElements();
	DefineAll();
	return;
}
string VariableDragManager::GetVardName(def_idx d_idx) {
	return vard_def[d_idx].name;
}
void VariableDragManager::SetVardName(def_idx d_idx, string newname) {
	vard_def[d_idx].name = newname;
	return;
}
UINT VariableDragManager::GetVardDefCount() {
	return vard_def.size();
}
void VariableDragManager::Clear() {
	vard_def.clear();
	VB1->ClearVariableDragElements();
	return;
}
void VariableDragManager::GetVardParams(def_idx d_idx, double &factor, VECTOR3 &ref, UINT &anim_idx) {
	factor = vard_def[d_idx].factor;
	ref = vard_def[d_idx].ref;
	anim_idx = vard_def[d_idx].anim_idx;
	return;
}
void VariableDragManager::SetVardParams(def_idx d_idx, double factor, VECTOR3 ref, UINT anim_index) {
	vard_def[d_idx].factor = factor;
	vard_def[d_idx].ref = ref;
	vard_def[d_idx].anim_idx = anim_index;
	return;
}
bool VariableDragManager::IsVardDefDefined(def_idx d_idx) {
	return vard_def[d_idx].defined;
}
void VariableDragManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Variable Drag Section");
	UINT vard_counter = 0;
	char cbuf[256] = { '\0' };
	int id;
	sprintf(cbuf, "VARIABLEDRAG_%i_ID", vard_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "VARIABLEDRAG_%i_NAME", vard_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		double factor;
		int anim_idx;
		VECTOR3 ref;
		sprintf(cbuf, "VARIABLEDRAG_%i_FACTOR", vard_counter);
		oapiReadItem_float(fh, cbuf, factor);
		sprintf(cbuf, "VARIABLEDRAG_%i_REF", vard_counter);
		oapiReadItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "VARIABLEDRAG_%i_ANIM", vard_counter);
		oapiReadItem_int(fh, cbuf, anim_idx);
		AddVardDef(name, anim_idx, factor, ref);
		vard_counter++;
		sprintf(cbuf, "VARIABLEDRAG_%i_ID", vard_counter);
	}
	LogV("Parsing Variable Drag Section Completed, found %i definitions",vard_counter);
	return;
}
void VariableDragManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------VARIABLE DRAG ITEMS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	UINT vard_counter = 0;
	for (UINT i = 0; i < GetVardDefCount(); i++) {
		if (!IsVardDefDefined(i)) { continue; }
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "VARIABLEDRAG_%i_ID", vard_counter);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "VARIABLEDRAG_%i_NAME", vard_counter);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetVardName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		double factor;
		UINT anim_idx;
		VECTOR3 ref;
		GetVardParams(i, factor, ref, anim_idx);
		sprintf(cbuf, "VARIABLEDRAG_%i_FACTOR", vard_counter);
		oapiWriteItem_float(fh, cbuf, factor);
		sprintf(cbuf, "VARIABLEDRAG_%i_REF", vard_counter);
		oapiWriteItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "VARIABLEDRAG_%i_ANIM", vard_counter);
		oapiWriteItem_int(fh, cbuf, anim_idx);
		oapiWriteLine(fh, " ");
		vard_counter++;		
	}
	return;
}