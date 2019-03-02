#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "CameraManager.h"

CameraManager::CameraManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	cam_defs.clear();
	CurrentCamera = 0;
	
	return;
}
CameraManager::~CameraManager() {
	VB1 = NULL;
	return;
}
UINT CameraManager::AddCamDef() {
	UINT index = cam_defs.size();
	char namebuf[256] = { '\0' };
	sprintf(namebuf, "Cam_%i", index);
	string name(namebuf);
	return AddCamDef(name, _V(0, 0, 0), _V(0, 0, 1), 0);
}
UINT CameraManager::AddCamDef(string name, VECTOR3 pos, VECTOR3 dir, double tilt) {
	CAM_DEF cd = CAM_DEF();
	cd.name = name;
	cd.pos = pos;
	cd.dir = dir;
	cd.tilt = tilt;
	UINT index = cam_defs.size();
	cam_defs.push_back(cd);
	return index;
}
void CameraManager::SetCamName(def_idx d_idx, string newname) {
	cam_defs[d_idx].name = newname;
	return;
}
string CameraManager::GetCamName(def_idx d_idx) {
	return cam_defs[d_idx].name;
}
void CameraManager::SetCamPos(def_idx d_idx, VECTOR3 newpos) {
	cam_defs[d_idx].pos = newpos;
	return;
}
VECTOR3 CameraManager::GetCamPos(def_idx d_idx) {
	return cam_defs[d_idx].pos;
}
void CameraManager::SetCamDir(def_idx d_idx, VECTOR3 newdir) {
	cam_defs[d_idx].dir = newdir;
	return;
}
VECTOR3 CameraManager::GetCamDir(def_idx d_idx) {
	return cam_defs[d_idx].dir;
}
void CameraManager::SetCamTilt(def_idx d_idx, double newtilt) {
	cam_defs[d_idx].tilt = newtilt;
	return;
}
double CameraManager::GetCamTilt(def_idx d_idx) {
	return cam_defs[d_idx].tilt;
}
void CameraManager::SetCamParams(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, double tilt) {
	SetCamPos(d_idx, pos);
	SetCamDir(d_idx, dir);
	SetCamTilt(d_idx, tilt);
	return;
}
void CameraManager::GetCamParams(def_idx d_idx, VECTOR3 &pos, VECTOR3 &dir, double &tilt) {
	pos = GetCamPos(d_idx);
	dir = GetCamDir(d_idx);
	tilt = GetCamTilt(d_idx);
	return;
}
void CameraManager::SetCurrentCamera(def_idx d_idx) {
	//if (!oapiCameraInternal()) { return; }
	if (d_idx >= cam_defs.size()) { return; }
	VB1->SetCameraOffset(GetCamPos(d_idx));
	VB1->SetCameraDefaultDirection(GetCamDir(d_idx),GetCamTilt(d_idx));
	oapiCameraSetCockpitDir(0, 0, false);
	VB1->SetCameraRotationRange(PI, PI, PI, PI);
	CurrentCamera = (int)d_idx;
	return;	
}
void CameraManager::SetNextCamera() {
	CurrentCamera++;
	if (CurrentCamera >= cam_defs.size()) {
		CurrentCamera = 0;
	}
	SetCurrentCamera(CurrentCamera);
	return;
}
void CameraManager::SetPreviousCamera() {
	CurrentCamera--;
	if (CurrentCamera < 0) {
		CurrentCamera = cam_defs.size() - 1;
	}
	SetCurrentCamera(CurrentCamera);
	return;
}
int CameraManager::GetCurrentCamera() {
	return CurrentCamera;
}
UINT CameraManager::GetCamCount() {
	return cam_defs.size();
}

void CameraManager::RemoveCamDef(def_idx d_idx) {
	cam_defs.erase(cam_defs.begin() + d_idx);
	return;
}
int CameraManager::ConsumeCameraBufferedKey(DWORD key, bool down, char *kstate) {
	if (!down)return 0;
	if (KEYMOD_CONTROL(kstate) || KEYMOD_SHIFT(kstate))
		return 0;
	if (key == OAPI_KEY_C) {
		//if (kstate[OAPI_KEY_UP] == 0x80) {
			SetNextCamera();
		//}
		//else if (kstate[OAPI_KEY_DOWN] == 0x80) {
		//	SetPreviousCamera();
		//}
	}
	return 1;
}

void CameraManager::ParseCfgFile(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	UINT cam_counter = 0;
	sprintf(cbuf, "CAM_%i_ID", cam_counter);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		VECTOR3 pos, dir;
		double tilt;
		sprintf(cbuf, "CAM_%i_NAME", cam_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "CAM_%i_POS", cam_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "CAM_%i_DIR", cam_counter);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "CAM_%i_TILT", cam_counter);
		oapiReadItem_float(fh, cbuf, tilt);
		tilt *= RAD;
		AddCamDef(name, pos, dir, tilt);
		cam_counter++;
		sprintf(cbuf, "CAM_%i_ID", cam_counter);
	}
	
	return;
}
void CameraManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------CAMERA DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < GetCamCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "CAM_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "CAM_%i_NAME", i);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", GetCamName(i).c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "CAM_%i_POS", i);
		oapiWriteItem_vec(fh, cbuf, GetCamPos(i));
		sprintf(cbuf, "CAM_%i_DIR", i);
		oapiWriteItem_vec(fh, cbuf, GetCamDir(i));
		sprintf(cbuf, "CAM_%i_TILT", i);
		oapiWriteItem_float(fh, cbuf, GetCamTilt(i)*DEG);
		oapiWriteLine(fh, " ");
	}
	return;
}

void CameraManager::Clear() {
	VB1->SetCameraDefaultDirection(_V(0, 0, 1),0);
	VB1->SetCameraOffset(_V(0, 0, 0));
	cam_defs.clear();
	return;
}