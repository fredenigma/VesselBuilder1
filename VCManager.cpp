#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "VCManager.h"
#include "MeshManager.h"

VCManager::VCManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	vc_positions.clear();
	id_counter = 1000;
	wHUD = false;
	vc_hud = new VCHUDSPEC;
	vc_hud->nmesh = 0;
	vc_hud->ngroup = 0;
	vc_hud->hudcnt = _V(0, 0, 0);
	vc_hud->size = 0;
	VCPosition = 0;

	/*VC_POS vc_pos = VC_POS();
	vc_pos.ofs= _V(0, 1.467, 6.782);
	vc_pos.dir = _V(0, 0, 1);
	vc_positions.push_back(vc_pos);
	vc_pos.ofs = _V(-0.7, 1.15, 5.55);
	vc_positions.push_back(vc_pos);
	vc_pos.ofs = _V(0.7, 1.15, 5.55);
	vc_positions.push_back(vc_pos);
	vc_pos.ofs = _V(-0.8, 1.2, 4.4);
	vc_positions.push_back(vc_pos);
	vc_pos.ofs = _V(0.8, 1.2, 4.4);
	vc_positions.push_back(vc_pos);
	vc_hud.nmesh = 1;
	vc_hud.ngroup = 129;
	vc_hud.hudcnt = _V(0, 1.462, 7.09);
	vc_hud.size = 0.15;
	EnableHUD(true);
	vc_mfds.clear();
	VC_MFD mfd = VC_MFD();
	mfd.mfd_spec.nmesh = 1;
	mfd.mfd_spec.ngroup = 88;
	vc_mfds.push_back(mfd);
	SetMFDHasPwrBtns(0, true);
	SetMFDHasColumnBtns(0, true);
	DefineMFDPwrButtons(0, _V(-0.19571, 1.06634, 7.22216), _V(-0.15394, 1.06634, 7.22216));
	DefineMFDColumnButtons(0, 1, _V(-0.25898, 1.20934, 7.27019), _V(-0.25898, 1.10680, 7.23137));
	DefineMFDColumnButtons(0, 2, _V(-0.07043, 1.20934, 7.27019), _V(-0.07043, 1.10680, 7.23137));
	mfd.mfd_spec.ngroup = 89;
	vc_mfds.push_back(mfd);
	SetMFDHasPwrBtns(1, true);
	SetMFDHasColumnBtns(1, true);
	DefineMFDPwrButtons(1, _V(0.13629,1.06644,7.22203), _V(0.17616, 1.06644, 7.22203));
	DefineMFDColumnButtons(1, 1, _V(0.07094,1.20652,7.27201), _V(0.07094,1.1048,7.23374));
	DefineMFDColumnButtons(1, 2, _V(0.25985,1.20652,7.27201), _V(0.25985,1.10405,7.23434));
	*/
	//vc_mfds.push_back(mfd);
	//mfd.mfd_spec.ngroup = 89;
	//mfd.pwr_btn_ctr = _V(0.13434, 1.06596, 7.22188);
	//vc_mfds.push_back(mfd);
	/*VCMFDSPEC mfd;
	mfd.nmesh = 1;
	mfd.ngroup = 88;
	vc_mfds.push_back(mfd);
	mfd.ngroup = 89;
	vc_mfds.push_back(mfd);*/
	return;
}
VCManager::~VCManager() {
	VB1 = NULL;
	delete vc_hud;
	vc_hud = NULL;
	return;
}
bool VCManager::LoadVC(int id) {
	VCPosition = id;
	if (id >= vc_positions.size()) { return false; }
	id_counter = 1000;
	
	if (wHUD) {
		oapiVCRegisterHUD(vc_hud);
	}
	
	
	
	for (UINT i = 0; i < vc_mfds.size(); i++) {
		oapiVCRegisterMFD(MFD_LEFT +i,&vc_mfds[i].mfd_spec);
		vc_mfds[i].mfd = MFD_LEFT + i;
		if (vc_mfds[i].wPwrBtns) {
			for (UINT j = 0; j < 3; j++) {
				oapiVCRegisterArea(id_counter, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBUP | PANEL_MOUSE_ONREPLAY);
				vc_mfds[i].pwr_btns_area_id[j] = id_counter;
				oapiVCSetAreaClickmode_Spherical(id_counter, vc_mfds[i].pwr_btns[j], 0.02);
				id_counter++;
			}
		}
		if (vc_mfds[i].wColsBtns) {
			for (UINT j = 0; j < 12; j++) {
				oapiVCRegisterArea(id_counter, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED | PANEL_MOUSE_ONREPLAY);
				vc_mfds[i].cols_btn_area_id[j] = id_counter;
				oapiVCSetAreaClickmode_Spherical(id_counter, vc_mfds[i].col_btns[j], 0.02);
				id_counter++;
			}
		}
	}



	VB1->SetCameraOffset(vc_positions[id].ofs);
	VB1->SetCameraDefaultDirection(vc_positions[id].dir);
	VB1->SetCameraMovement(_V(0, 0, 0.5), 0, 0, _V(-0.5, 0, 0), 75 * RAD, 30 * RAD, _V(0.5, 0, 0), -75 * RAD, -30 * RAD);
	if (vc_positions.size() > 1) {
		if (id == 0) {
			oapiVCSetNeighbours(-1, -1, -1, 1);
		}
		else if (id == vc_positions.size() - 1) {
			oapiVCSetNeighbours(-1, -1, id-1, - 1);
		}
		else {
			oapiVCSetNeighbours(-1, -1, id-1, id + 1);
		}
	}
	
	return true;
}
bool VCManager::VCMouseEvent(int id, int event, VECTOR3 &p) {
	
	for (UINT i = 0; i < vc_mfds.size(); i++) {
		for (UINT j = 0; j < 3; j++) {
			if (id == vc_mfds[i].pwr_btns_area_id[j]) {
				if (j == 0) {
					oapiSendMFDKey(vc_mfds[i].mfd, OAPI_KEY_ESCAPE);
					return true;
				}
				else if (j == 1) {
					oapiSendMFDKey(vc_mfds[i].mfd, OAPI_KEY_F1);
					return true;
				}
				else if (j == 2) {
					oapiSendMFDKey(vc_mfds[i].mfd, OAPI_KEY_GRAVE);
					return true;
				}
			}
		}
		for (UINT k = 0; k < 12; k++) {
			if (id == vc_mfds[i].cols_btn_area_id[k]) {
				//sprintf(oapiDebugString(), "id:%i k:%i mfd:%i", id,k,vc_mfds[i].mfd);
				oapiProcessMFDButton(vc_mfds[i].mfd, k, event);
				return true;
			}
		}
	}
	
	return false;
}
void VCManager::DefineMFDColumnButtons(def_idx d_idx, int col, VECTOR3 first, VECTOR3 last) {
	vc_mfds[d_idx].col_btns[6*(col-1)] = first;
	vc_mfds[d_idx].col_btns[6 * (col - 1)+5] = last;
	VECTOR3 delta = (last - first) / 5;
	for (UINT i = 1; i < 5; i++) {
		vc_mfds[d_idx].col_btns[6*(col-1)+i] = first + (delta*i);
	}
	return;
}
void VCManager::DefineMFDPwrButtons(def_idx d_idx, VECTOR3 first, VECTOR3 last) {
	vc_mfds[d_idx].pwr_btns[0] = first;
	vc_mfds[d_idx].pwr_btns[2] = last;
	vc_mfds[d_idx].pwr_btns[1] = (first+last)*0.5;
	return;
}
void VCManager::SetMFDHasPwrBtns(def_idx d_idx, bool set) {
	vc_mfds[d_idx].wPwrBtns = set;
	return;
}
void VCManager::SetMFDHasColumnBtns(def_idx d_idx, bool set) {
	vc_mfds[d_idx].wColsBtns = set;
	return;
}


UINT VCManager::AddPosition() {
	UINT index = vc_positions.size();
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "vc_position_%i", index);
	string name(cbuf);
	return AddPosition(name, _V(0, 0, 0), _V(0, 0, 1));
}
UINT VCManager::AddPosition(string name, VECTOR3 ofs, VECTOR3 dir) {
	VC_POS vc_pos = VC_POS();
	vc_pos.name = name;
	vc_pos.dir = dir;
	vc_pos.ofs = ofs;
	UINT index = vc_positions.size();
	vc_positions.push_back(vc_pos);
	return index;
}
void VCManager::SetPositionName(def_idx d_idx, string newname) {
	vc_positions[d_idx].name = newname;
	return;
}
string VCManager::GetPositionName(def_idx d_idx) {
	return vc_positions[d_idx].name;
}
void VCManager::SetPositionPos(def_idx d_idx, VECTOR3 newPos) {
	vc_positions[d_idx].ofs = newPos;
	return;
}
void VCManager::SetPositionDir(def_idx d_idx, VECTOR3 newDir) {
	vc_positions[d_idx].dir = newDir;
	return;
}
VECTOR3 VCManager::GetPositionPos(def_idx d_idx) {
	return vc_positions[d_idx].ofs;
}
VECTOR3 VCManager::GetPositionDir(def_idx d_idx) {
	return vc_positions[d_idx].dir;
}
void VCManager::GetPositionParams(def_idx d_idx, VECTOR3 &ofs, VECTOR3 &dir) {
	ofs = GetPositionPos(d_idx);
	dir = GetPositionDir(d_idx);
	return;
}
void VCManager::DeletePosition(def_idx d_idx) {
	vc_positions.erase(vc_positions.begin() + d_idx);
	return;
}
UINT VCManager::GetPositionCount() {
	return vc_positions.size();
}
int VCManager::GetVCPosition() {
	return VCPosition;
}
UINT VCManager::AddMFD() {
	if (GetMFDCount() >= MAXMFD) { return (UINT)-1; }
	return AddMFD(0, 0, false, _V(0, 0, 0), _V(0, 0, 0), false, _V(0, 0, 0), _V(0, 0, 0), _V(0, 0, 0), _V(0, 0, 0));
}
UINT VCManager::AddMFD(int mesh, int group, bool wPwrBtns, VECTOR3 pwr_btn, VECTOR3 mnu_btn, bool wColsBtns, VECTOR3 TopLeftBtn, VECTOR3 BottomLeftBtn, VECTOR3 TopRightBtn, VECTOR3 BottomRightBtn) {
	if (GetMFDCount() >= MAXMFD) { return (UINT)-1; }
	VC_MFD mfd = VC_MFD();
	mfd.mfd_spec.nmesh = mesh;
	mfd.mfd_spec.ngroup = group;
	UINT index = vc_mfds.size();
	vc_mfds.push_back(mfd);
	if (wPwrBtns) {
		SetMFDHasPwrBtns(index, true);
		DefineMFDPwrButtons(index, pwr_btn, mnu_btn);
	}
	if (wColsBtns) {
		SetMFDHasColumnBtns(index, true);
		DefineMFDColumnButtons(index, 1, TopLeftBtn, BottomLeftBtn);
		DefineMFDColumnButtons(index, 2, TopRightBtn, BottomRightBtn);
	}
	
	return index;
}
void VCManager::DeleteMFD(def_idx d_idx) {
	vc_mfds.erase(vc_mfds.begin() + d_idx);
	return;
}
void VCManager::SetMFDMesh(def_idx d_idx, int newmesh) {
	vc_mfds[d_idx].mfd_spec.nmesh = newmesh;
	return;
}
void VCManager::SetMFDGroup(def_idx d_idx, int newgroup) {
	vc_mfds[d_idx].mfd_spec.ngroup = newgroup;
	return;
}
bool VCManager::GetMFDHasPwrBtns(def_idx d_idx) {
	return vc_mfds[d_idx].wPwrBtns;
}
bool VCManager::GetMFDHasColBtns(def_idx d_idx) {
	return vc_mfds[d_idx].wColsBtns;
}
DWORD VCManager::GetMFDMesh(def_idx d_idx) {
	return vc_mfds[d_idx].mfd_spec.nmesh;
}
DWORD VCManager::GetMFDGroup(def_idx d_idx) {
	return vc_mfds[d_idx].mfd_spec.ngroup;
}
void VCManager::GetMFDPwrVectors(def_idx d_idx, VECTOR3 &pwr_btn, VECTOR3 &mnu_btn) {
	pwr_btn = vc_mfds[d_idx].pwr_btns[0];
	mnu_btn = vc_mfds[d_idx].pwr_btns[2];
	return;
}
void VCManager::GetMFDColVectors(def_idx d_idx, VECTOR3 &top_left, VECTOR3 &bottom_left, VECTOR3 &top_right, VECTOR3 &bottom_right) {
	top_left = vc_mfds[d_idx].col_btns[0];
	bottom_left = vc_mfds[d_idx].col_btns[5];
	top_right = vc_mfds[d_idx].col_btns[6];
	bottom_right = vc_mfds[d_idx].col_btns[11];
	return;
}
UINT VCManager::GetMFDCount() {
	return vc_mfds.size();
}
void VCManager::EnableHUD(bool enable) {
	wHUD = enable;
	return;
}
bool VCManager::IsHUDEnabled() {
	return wHUD;
}
void VCManager::SetHUDMesh(DWORD nmesh) {
	vc_hud->nmesh = nmesh;
	return;
}
void VCManager::SetHUDGroup(DWORD ngroup) {
	vc_hud->ngroup = ngroup;
	return;
}
void VCManager::SetHUDCenter(VECTOR3 hudcnt) {
	vc_hud->hudcnt = hudcnt;
	return;
}
void VCManager::SetHUDSize(double size) {
	vc_hud->size = size;
	return;
}
void VCManager::GetHUDParams(DWORD &nmesh, DWORD &ngroup, VECTOR3 &hudcnt, double &size) {
	nmesh = vc_hud->nmesh;
	ngroup = vc_hud->ngroup;
	hudcnt = vc_hud->hudcnt;
	size = vc_hud->size;
	return;
}
void VCManager::SetHUDParams(int mesh, int group, VECTOR3 hudcnt, double size) {
	SetHUDMesh(mesh);
	SetHUDGroup(group);
	SetHUDCenter(hudcnt);
	SetHUDSize(size);
	return;
}


void VCManager::ParseCfgFile(FILEHANDLE fh) {
	UINT position_counter = 0;
	char cbuf[256] = { '\0' };
	int id;
	sprintf(cbuf, "VC_POS_%i_ID", position_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		sprintf(cbuf, "VC_POS_%i_NAME", position_counter);
		char namebuf[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		VECTOR3 ofs, dir;
		sprintf(cbuf, "VC_POS_%i_OFS", position_counter);
		oapiReadItem_vec(fh, cbuf, ofs);
		sprintf(cbuf, "VC_POS_%i_DIR", position_counter);
		oapiReadItem_vec(fh, cbuf, dir);
		AddPosition(name, ofs, dir);
		position_counter++;
		sprintf(cbuf, "VC_POS_%i_ID", position_counter);
	}
	UINT MFD_counter = 0;
	int mfd_id;
	sprintf(cbuf, "VC_MFD_%i_ID", MFD_counter);
	while (oapiReadItem_int(fh, cbuf, mfd_id)) {
		int mesh, group;
		bool wPwrBtns, wColsBtns;
		VECTOR3 pwr_btn, mnu_btn, TopLeftBtn, BottomLeftBtn, TopRightBtn, BottomRightBtn;

		sprintf(cbuf, "VC_MFD_%i_MESH", MFD_counter);
		oapiReadItem_int(fh, cbuf, mesh);
		sprintf(cbuf, "VC_MFD_%i_GROUP", MFD_counter);
		oapiReadItem_int(fh, cbuf, group);
		sprintf(cbuf, "VC_MFD_%i_WPWR", MFD_counter);
		if (!oapiReadItem_bool(fh, cbuf, wPwrBtns)) { wPwrBtns = false; }
		if (wPwrBtns) {
			sprintf(cbuf, "VC_MFD_%i_PWR0", MFD_counter);
			oapiReadItem_vec(fh, cbuf, pwr_btn);
			sprintf(cbuf, "VC_MFD_%i_PWR2", MFD_counter);
			oapiReadItem_vec(fh, cbuf, mnu_btn);
		}
		sprintf(cbuf, "VC_MFD_%i_WCOL", MFD_counter);
		if (!oapiReadItem_bool(fh, cbuf, wColsBtns)) { wColsBtns = false; }
		if (wColsBtns) {
			sprintf(cbuf, "VC_MFD_%i_COLTL", MFD_counter);
			oapiReadItem_vec(fh, cbuf, TopLeftBtn);
			sprintf(cbuf, "VC_MFD_%i_COLBL", MFD_counter);
			oapiReadItem_vec(fh, cbuf, BottomLeftBtn);
			sprintf(cbuf, "VC_MFD_%i_COLTR", MFD_counter);
			oapiReadItem_vec(fh, cbuf, TopRightBtn);
			sprintf(cbuf, "VC_MFD_%i_COLBR", MFD_counter);
			oapiReadItem_vec(fh, cbuf, BottomRightBtn);

		}
		UINT index = AddMFD(mesh, group, wPwrBtns, pwr_btn, mnu_btn, wColsBtns, TopLeftBtn, BottomLeftBtn, TopRightBtn, BottomRightBtn);
		MFD_counter++;
		sprintf(cbuf, "VC_MFD_%i_ID", MFD_counter);
	}
	bool Hud;
	int mesh, group;
	VECTOR3 hudcnt;
	double size;
	if (!oapiReadItem_bool(fh, "VC_HUD", Hud)) { Hud = false; }
	if (Hud) {
		EnableHUD(true);
		oapiReadItem_int(fh, "VC_HUD_MESH", mesh);
		oapiReadItem_int(fh, "VC_HUD_GROUP", group);
		oapiReadItem_vec(fh, "VC_HUD_CNT", hudcnt);
		oapiReadItem_float(fh, "VC_HUD_SIZE", size);
		SetHUDParams(mesh, group, hudcnt, size);
	}
	return;
}
void VCManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------VIRTUAL COCKPIT DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < GetPositionCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "VC_POS_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "VC_POS_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetPositionName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "VC_POS_%i_OFS", i);
		oapiWriteItem_vec(fh, cbuf, GetPositionPos(i));
		sprintf(cbuf, "VC_POS_%i_DIR", i);
		oapiWriteItem_vec(fh, cbuf, GetPositionDir(i));
		oapiWriteLine(fh, " ");
	}
	for (UINT i = 0; i < GetMFDCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "VC_MFD_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "VC_MFD_%i_MESH", i);
		oapiWriteItem_int(fh, cbuf, GetMFDMesh(i));
		sprintf(cbuf, "VC_MFD_%i_GROUP", i);
		oapiWriteItem_int(fh, cbuf, GetMFDGroup(i));
		sprintf(cbuf, "VC_MFD_%i_WPWR", i);
		bool trans = GetMFDHasPwrBtns(i);
		oapiWriteItem_bool(fh, cbuf, GetMFDHasPwrBtns(i));
		if (trans) {
			VECTOR3 pwr0, pwr2;
			GetMFDPwrVectors(i, pwr0, pwr2);
			sprintf(cbuf, "VC_MFD_%i_PWR0", i);
			oapiWriteItem_vec(fh, cbuf, pwr0);
			sprintf(cbuf, "VC_MFD_%i_PWR2", i);
			oapiWriteItem_vec(fh, cbuf, pwr2);
		}
		trans = GetMFDHasColBtns(i);
		sprintf(cbuf, "VC_MFD_%i_WCOL", i);
		oapiWriteItem_bool(fh, cbuf, trans);
		if (trans) {
			VECTOR3 TopLeft, BottomLeft, TopRight, BottomRight;
			GetMFDColVectors(i, TopLeft, BottomLeft, TopRight, BottomRight);
			sprintf(cbuf, "VC_MFD_%i_COLTL", i);
			oapiWriteItem_vec(fh, cbuf, TopLeft);
			sprintf(cbuf, "VC_MFD_%i_COLBL", i);
			oapiWriteItem_vec(fh, cbuf, BottomLeft);
			sprintf(cbuf, "VC_MFD_%i_COLTR", i);
			oapiWriteItem_vec(fh, cbuf, TopRight);
			sprintf(cbuf, "VC_MFD_%i_COLBR", i);
			oapiWriteItem_vec(fh, cbuf, BottomRight);
		}
		oapiWriteLine(fh, " ");
	}
	oapiWriteItem_bool(fh, "VC_HUD", IsHUDEnabled());
	if (IsHUDEnabled()) {
		DWORD mesh, group;
		VECTOR3 hudcnt;
		double size;
		GetHUDParams(mesh, group, hudcnt, size);
		oapiWriteItem_int(fh, "VC_HUD_MESH", mesh);
		oapiWriteItem_int(fh, "VC_HUD_GROUP", group);
		oapiWriteItem_vec(fh, "VC_HUD_CNT", hudcnt);
		oapiWriteItem_float(fh, "VC_HUD_SIZE", size);
		oapiWriteLine(fh, " ");
	}
	return;
}
void VCManager::Clear() {
	vc_positions.clear();
	vc_mfds.clear();
	vc_hud->hudcnt = _V(0, 0, 0);
	vc_hud->ngroup = 0;
	vc_hud->nmesh = 0;
	vc_hud->size = 0;
	return;
}