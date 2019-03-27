#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "DockManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

DockManager::DockManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	dock_defs.clear();
	docks_to_del.clear();
	docks_jettisoned.clear();
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
UINT DockManager::AddDockDef(string name, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot, bool dockjett, bool SoftDock, double SoftDockDistance , def_idx SoftDockAnim) {
	LogV("Adding Dock:%s pos:%.3f %.3f %.3f dir:%.3f %.3f %.3f rot:%.3f %.3f %.3f Jettisonable?:%i", name.c_str(), pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, rot.x, rot.y, rot.z, dockjett);
	DCK_DEF dd = DCK_DEF();
	dd.name = name;
	dd.IsDockJett = dockjett;
	*dd.pos_ptr = pos;
	*dd.antidir_ptr = dir*(-1);
	*dd.antirot_ptr = rot*(-1);
	dd.pos = pos;
	dd.dir = dir;
	dd.rot = rot;
	dd.dh = VB1->CreateDock(pos, dir, rot);
	dd.SoftDock = SoftDock;
	dd.SoftDockDistance = SoftDockDistance;
	dd.SoftDockAnim = SoftDockAnim;

	UINT index = dock_defs.size();
	dock_defs.push_back(dd);
	return index;
}

bool DockManager::DeleteDockDef(def_idx d_idx) {
	if (d_idx >= dock_defs.size()) { LogV("WARNING: Dock impossible to delete, wrong index called"); return false; }
	LogV("Deleting Dock n.%i", d_idx);
	VB1->DelDock(dock_defs[d_idx].dh);
	delete dock_defs[d_idx].pos_ptr;
	delete dock_defs[d_idx].antidir_ptr;
	delete dock_defs[d_idx].antirot_ptr;
	dock_defs.erase(dock_defs.begin() + d_idx);
	LogV("Dock Deleted");
	return true;
}
void DockManager::GetDockParams(def_idx d_idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot) {
	//if (dock_defs[d_idx].dh == NULL) { return; }
	//VB1->GetDockParams(dock_defs[d_idx].dh, pos, dir, rot);
	pos = dock_defs[d_idx].pos;
	dir = dock_defs[d_idx].dir;
	rot = dock_defs[d_idx].rot;
	return;
}
void DockManager::SetDockParams(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot) {
	if (dock_defs[d_idx].dh == NULL) { return; }
	*dock_defs[d_idx].pos_ptr = pos;
	*dock_defs[d_idx].antidir_ptr = dir*(-1);
	*dock_defs[d_idx].antirot_ptr = rot*(-1);
	dock_defs[d_idx].pos = pos;
	dock_defs[d_idx].dir = dir;
	dock_defs[d_idx].rot = rot;
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
void DockManager::DockPreStep(double simt, double simdt, double mjd) {
	if (docks_to_del.size() > 0) {
		VB1->DelDock(docks_to_del[0]); //needed because other wise crash on scenario close
	}
	return;
}

void DockManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Docks Section Started");
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
	LogV("Parsing Docks Section Completed");
	LogV("Found %i Dock Definitions", dock_counter);
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
	LogV("Clearing Docks Section");
	VB1->ClearDockDefinitions();
	dock_defs.clear();
	docks_to_del.clear();
	docks_jettisoned.clear();
	LogV("Clearing Docks Section Completed");
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

VECTOR3* DockManager::GetDockPosPtr(def_idx d_idx) {
	return dock_defs[d_idx].pos_ptr;
}
VECTOR3* DockManager::GetDockAntiDirPtr(def_idx d_idx) {
	return dock_defs[d_idx].antidir_ptr;
}
VECTOR3 *DockManager::GetDockAntiRotPtr(def_idx d_idx) {
	return dock_defs[d_idx].antirot_ptr;
}

void DockManager::DockEvent(int dock, OBJHANDLE mate) {
	if (mate != NULL) { 
		return; }
	DOCKHANDLE dh = VB1->GetDockHandle(dock);
	def_idx d_idx = GetDockIdx(dh);
	if (IsDockJettisonable(d_idx)) {
		docks_to_del.push_back(dock_defs[d_idx].dh);
		docks_jettisoned.push_back(d_idx);
	//	VB1->DelDock(dock_defs[d_idx].dh);
		dock_defs[d_idx].dh = NULL;
	}
	return;
}

DOCKHANDLE DockManager::GetDH(def_idx d_idx) {
	return dock_defs[d_idx].dh;
}
UINT DockManager::GetOrbiterDockIdx(DOCKHANDLE dh) {
	for (UINT i = 0; i < VB1->DockCount(); i++) {
		if (VB1->GetDockHandle(i) == dh) {
			return i;
		}
	}
	return (UINT)-1;
}
void DockManager::SetSoftDockParams(def_idx d_idx, bool enable, double distance, def_idx anim) {
	dock_defs[d_idx].SoftDock = enable;
	dock_defs[d_idx].SoftDockDistance = distance;
	dock_defs[d_idx].SoftDockAnim = anim;
	return;
}
void DockManager::GetSoftDockParams(def_idx d_idx, bool &enable, double &distance, def_idx &anim) {
	enable = dock_defs[d_idx].SoftDock;
	distance = dock_defs[d_idx].SoftDockDistance;
	anim = dock_defs[d_idx].SoftDockAnim;
}