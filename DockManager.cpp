#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "DockManager.h"

DockManager::DockManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	dock_defs.clear();
	return;
}
DockManager::~DockManager() {
	VB1 = NULL;
	return;
}
UINT DockManager::AddDockDef() {
	UINT index = dock_defs.size();
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Dock_%i", index);
	string name(cbuf);
	return AddDockDef(name, _V(0, 0, 0), _V(0, 0, 1), _V(0, 1, 0), false);
}
UINT DockManager::AddDockDef(string name, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot, bool dockjett) {
	DCK_DEF dd = DCK_DEF();
	dd.name = name;
	dd.IsDockJett = dockjett;
	dd.dh = VB1->CreateDock(pos, dir, rot);
	UINT index = dock_defs.size();
	dock_defs.push_back(dd);
	return index;
}

bool DockManager::DeleteDockDef(def_idx d_idx) {
	if (d_idx >= dock_defs.size()) { return false; }
	VB1->DelDock(dock_defs[d_idx].dh);
	dock_defs.erase(dock_defs.begin() + d_idx);
	return true;
}
void DockManager::GetDockParams(def_idx d_idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot) {
	if (dock_defs[d_idx].dh == NULL) { return; }
	VB1->GetDockParams(dock_defs[d_idx].dh, pos, dir, rot);
	return;
}
void DockManager::SetDockParams(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot) {
	if (dock_defs[d_idx].dh == NULL) { return; }
	VB1->SetDockParams(dock_defs[d_idx].dh, pos, dir, rot);
	return;
}
string DockManager::GetDockName(def_idx d_idx) {
	return dock_defs[d_idx].name;
}
void DockManager::SetDockName(def_idx d_idx, string name) {
	dock_defs[d_idx].name = name;
	return;
}
bool DockManager::IsDockJettisonable(def_idx d_idx) {
	return dock_defs[d_idx].IsDockJett;
}
void DockManager::SetIsDockJettisonable(def_idx d_idx, bool set) {
	dock_defs[d_idx].IsDockJett = set;
	return;
}
void DockManager::RotateDock(def_idx d_idx, VECTOR3 axis, double angle) {
	VECTOR3 pos, dir, rot;
	GetDockParams(d_idx, pos, dir, rot);
	MATRIX3 oldrm = VB1->FindRM(dir, rot);
	MATRIX3 rotrm = rotm(axis, angle);
	MATRIX3 newrm = mul(rotrm, oldrm);
	VB1->FindDirRot(newrm, dir, rot);
	SetDockParams(d_idx, pos, dir, rot);
	return;
}
void DockManager::MoveDock(def_idx d_idx, VECTOR3 shift) {
	VECTOR3 pos, dir, rot;
	GetDockParams(d_idx, pos, dir, rot);
	pos += shift;
	SetDockParams(d_idx, pos, dir, rot);
	return;
}
void DockManager::SetDockPos(def_idx d_idx, VECTOR3 pos) {
	VECTOR3 old_pos, dir, rot;
	GetDockParams(d_idx, old_pos, dir, rot);
	SetDockParams(d_idx, pos, dir, rot);
	return;
}
void DockManager::SetDockDir(def_idx d_idx, VECTOR3 dir) {
	VECTOR3 old_dir, pos, rot;
	GetDockParams(d_idx, pos, old_dir, rot);
	SetDockParams(d_idx, pos, dir, rot);
	return;
}
void DockManager::SetDockRot(def_idx d_idx, VECTOR3 rot) {
	VECTOR3 old_rot, pos, dir;
	GetDockParams(d_idx, pos, dir, old_rot);
	SetDockParams(d_idx, pos, dir, rot);
	return;
}


void DockManager::ParseCfgFile(FILEHANDLE fh) {
	UINT dock_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "DOCK_%i_ID", dock_counter);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		VECTOR3 pos, dir, rot;
		bool jett;
		sprintf(cbuf, "DOCK_%i_NAME", dock_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "DOCK_%i_POS", dock_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "DOCK_%i_DIR", dock_counter);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "DOCK_%i_ROT", dock_counter);
		oapiReadItem_vec(fh, cbuf, rot);
		sprintf(cbuf, "DOCK_%i_JETT", dock_counter);
		if (!oapiReadItem_bool(fh, cbuf, jett)) { jett = false; }
		AddDockDef(name, pos, dir, rot, jett);
		dock_counter++;
		sprintf(cbuf, "DOCK_%i_ID", dock_counter);
	}

	return;
}
void DockManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------DOCKS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i <GetDockCount(); i++) {
		char cbuf[256] = { '\0' };
		VECTOR3 pos, dir, rot;
		sprintf(cbuf, "DOCK_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "DOCK_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetDockName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		GetDockParams(i, pos, dir, rot);
		sprintf(cbuf, "DOCK_%i_POS", i);
		oapiWriteItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "DOCK_%i_DIR", i);
		oapiWriteItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "DOCK_%i_ROT", i);
		oapiWriteItem_vec(fh, cbuf, rot);
		sprintf(cbuf, "DOCK_%i_JETT", i);
		oapiWriteItem_bool(fh, cbuf, IsDockJettisonable(i));
		oapiWriteLine(fh, " ");
	}
	return;
}

void DockManager::Clear() {
	VB1->ClearDockDefinitions();
	dock_defs.clear();
	return;
}
UINT DockManager::GetDockCount() {
	return dock_defs.size();
}
def_idx DockManager::GetDockIdx(DOCKHANDLE dh) {
	for (UINT i = 0; i < GetDockCount(); i++) {
		if (dock_defs[i].dh == dh) {
			return i;
		}
	}
	return (UINT)-1;
}
void DockManager::DockEvent(int dock, OBJHANDLE mate) {
	if (mate != NULL) { return; }
	DOCKHANDLE dh = VB1->GetDockHandle(dock);
	def_idx d_idx = GetDockIdx(dh);
	if (IsDockJettisonable(d_idx)) {
		VB1->DelDock(dock_defs[d_idx].dh);
		dock_defs[d_idx].dh = NULL;
	}
	return;
}