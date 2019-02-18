
#define STRICT 
#define ORBITER_MODULE

#include <math.h>
#include <stdio.h>

#include "StationBuilder1.h"

#include "DialogControl.h"
#include "FollowMeDlg.h"
//#include "Common.h"
#include "resource.h"
#include "DlgCtrl.h"
#include "gcAPI.h"
#include "MeshManager.h"
#include "AnimDef.h"
#include "AnimCompDef.h"
#include "AnimationManager.h"

//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_DEPRECATE



//StationBuilder1 *SB1;
HINSTANCE hDLL;

//Creation
StationBuilder1::StationBuilder1(OBJHANDLE hObj,int fmodel):VESSEL4(hObj,fmodel){
	gcInitialize();

	OrbiterRoot.clear();
	char root[MAX_PATH] = { '\0' };
	GetCurrentDirectory(MAX_PATH, root);
	OrbiterRoot.assign(root);
	wD3D9 = false;
	GetOrbiterDirs();

	//mesh_definitions.clear();
	MshMng = new MeshManager(this);

	AnimMng = new AnimationManager(this);
	dock_definitions.clear();
	att_definitions.clear();
	
	cfgfilename.clear();
//	SB1 = this;
	//loading_msh_idx = 0;
	follow_me_pos = _V(0, 0, 0);
	follow_me_dir = _V(0, 0, 1);
	follow_me_rot = _V(0, 1, 0);
	follow_me_rm = _M(1, 0, 0, 0, 1, 0, 0, 0, 1);
	follow_me = false;
	h_follow_me = NULL;
	follow_me_rotation_speed = 60*RAD;
	follow_me_translation_speed = 6;
	ResetSBLog();
	follow_me_noteh = oapiCreateAnnotation(false, 1, _V(1, 1, 1));
	//Dlg = NULL;
	Dlg = new DialogControl(this);
	FMDlg = new FollowMeDlg(this);
	//HighLightColor = HIGHLIGHTCOLORRED;
	
	vclip = V_CLIPBOARD();
	colwhite = _V(1, 1, 1);
	colblue = _V(0, 0, 1);
	colgreen = _V(0, 1, 0);
	colred = _V(1, 0, 0);
	dockbcn = NULL;
	DockBeaconsActive = false;
	
	
	//animcomps_definanitions.clear();
	//animations_definitions.clear();
	SBLog("Class Initialize");
}


StationBuilder1::~StationBuilder1(){
	delete Dlg;
	Dlg = NULL;
	delete FMDlg;
	FMDlg = NULL;
	delete MshMng;
	MshMng = NULL;
	delete AnimMng;
	AnimMng = NULL;
	CloseSBLog();
	
	//ClearDelete(mgr);
	return;
}


void StationBuilder1::GetOrbiterDirs() {
	string cfgname("Orbiter.cfg");
	if (gcEnabled()) { 
		cfgname.assign("Orbiter_NG.cfg"); 
		wD3D9 = true;
//		oapiWriteLogV("wD3D9:%i", wD3D9);
	}
	meshdir.assign(".\\Meshes\\");
	configdir.assign(".\\Config\\");
	texturedir.assign(".\\Textures\\");
	texture2dir.assign(".\\Textures2\\");
	scenariodir.assign(".\\Scenarios\\");
	FILEHANDLE orbiter_cfg = oapiOpenFile(cfgname.c_str(), FILE_IN_ZEROONFAIL, ROOT);
	if (orbiter_cfg != NULL) {
		char line[MAX_PATH] = { '\0' };
		if (oapiReadItem_string(orbiter_cfg, "MeshDir", line)) {
			meshdir.assign(line);
		}
		if (oapiReadItem_string(orbiter_cfg, "ConfigDir", line)) {
			configdir.assign(line);
		}
		if (oapiReadItem_string(orbiter_cfg, "TextureDir", line)) {
			texturedir.assign(line);
		}
		if (oapiReadItem_string(orbiter_cfg, "HightexDir", line)) {
			texture2dir.assign(line);
		}
		if (oapiReadItem_string(orbiter_cfg, "ScenarioDir", line)) {
			scenariodir.assign(line);
		}

	}
	SBLog("cfg file:%s", cfgname.c_str());
	SBLog("Orbiter Folders: %s %s %s %s %s", meshdir.c_str(), configdir.c_str(), texturedir.c_str(), texture2dir.c_str(), scenariodir.c_str());
	oapiCloseFile(orbiter_cfg, FILE_IN_ZEROONFAIL);
	return;
}

void StationBuilder1::CreateDocks() {
	SBLog("Creating Docks...");
	for (UINT i = 0; i < dock_definitions.size(); i++) {
		dock_definitions[i].dh = CreateDock(dock_definitions[i].pos, dock_definitions[i].dir, dock_definitions[i].rot);
	}
	SBLog("Creation of %i Docking ports completed", dock_definitions.size());
	/*
	CreateDock(_V(0.010, -3.733, 4.419), _V(0.000, 0.000, 1.000), _V(0.000, 1.000, 0.000));
	CreateDock(_V(-0.018, -3.738, -32.247), _V(0.000, 0.000, -1.000), _V(-0.707, 0.707, 0.000));
	CreateDock(_V(-0.013, -9.028, -19.769), _V(0.000, -1.000, 0.000), _V(0.000, 0.000, -1.000));
	CreateDock(_V(-0.013, 1.531, -19.769), _V(0.000, 1.000, 0.000), _V(-0.000, 0.000, -1.000));
	CreateDock(_V(0.017, -8.861, 0.734), _V(0.000, -1.000, 0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(0.010, -5.192, 13.396), _V(0.000, 0.000, 1.000), _V(0.000, -1.000, 0.000));
	CreateDock(_V(20.370, 0.166, 4.980), _V(-0.000, -0.000, -1.000), _V(1.000, 0.000, -0.000));
	CreateDock(_V(-20.347, -0.016, 4.980), _V(-0.000, -0.000, -1.000), _V(-1.000, -0.029, 0.000));
	CreateDock(_V(0.000, -5.138, 20.998), _V(0.000, 0.000, 1.000), _V(-0.000, 1.000, 0.000));
	CreateDock(_V(0.010, -6.516, 15.808), _V(0.000, -1.000, 0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(-0.000, -2.436, 15.808), _V(-0.000, 1.000, 0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(-0.000, -0.137, 16.537), _V(-0.000, 1.000, 0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(-2.490, -4.566, 15.808), _V(1.000, 0.000, -0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(-10.750, -2.166, 15.808), _V(-0.000, 1.000, 0.000), _V(-1.000, -0.000, 0.000));
	CreateDock(_V(2.260, -4.416, 15.768), _V(1.000, 0.000, -0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(0.000, -5.184, 20.296), _V(0.000, -0.000, -1.000), _V(0.000, -1.000, 0.000));
	CreateDock(_V(-0.002, -8.793, 0.714), _V(0.000, -1.000, 0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(-2.060, -4.453, -0.011), _V(-1.000, 0.000, 0.000), _V(0.000, -1.000, 0.000));
	CreateDock(_V(0.010, -4.413, -1.791), _V(-1.000, 0.000, 0.000), _V(0.000, -1.000, 0.000));
	CreateDock(_V(0.001, -6.468, -0.010), _V(0.000, -1.000, 0.000), _V(0.000, 0.000, 1.000));
	CreateDock(_V(-2.260, -4.453, -0.011), _V(-1.000, 0.000, 0.000), _V(0.000, -1.000, 0.000));
	CreateDock(_V(-7.040, -6.557, -0.011), _V(0.000, 0.000, 1.000), _V(0.000, -1.000, 0.000));
	CreateDock(_V(-7.040, -4.403, -2.111), _V(0.000, 0.000, 1.000), _V(0.000, 1.000, 0.000));*/
	return;
}
void StationBuilder1::clbkSetClassCaps(FILEHANDLE cfg){
	SetEmptyMass(1000);
	SetSize(10);
	SBLog("ClassName:%s", GetClassName());
	
	
	ParseCfgFile(cfg);
	VehicleSetup();
	WriteBackupFile();
	
	

	

	return;
}
void StationBuilder1::clbkLoadStateEx(FILEHANDLE scn,void *vs)
{
	SBLog("Loading State...");
	bool wCfgFile = false;
	char *line;
	char cfgfile[256] = { '\0' };
	while (oapiReadScenario_nextline(scn, line)) {
	//	if(!strnicmp(line, "CONFIG_FILE", 11)) {
	//		wCfgFile = true;
	//		sscanf(line + 11, "%s", &cfgfile);
	//	}else{
			ParseScenarioLineEx(line, vs); 
	//	}
			
		
		
	}
	
	/*if (wCfgFile) {
		SBLog("Configuration File Found: %s", cfgfile);
		cfgfilename.assign(cfgfile);
		ParseCfgFile(cfgfilename);
		string cfgbackup = cfgfilename.substr(0,cfgfilename.size()-4);
		cfgbackup += "__BACKUP";
		cfgbackup += ".cfg";
		WriteCfgFile(cfgbackup);
	}*/

//	string fn("ISS_AtoZ_UV.cfg");

	
	//VehicleSetup();

	SBLog("Loading State Finished");
}
void StationBuilder1::clbkSaveState(FILEHANDLE scn)
{
	SBLog("Saving State...");

	char buff[256] = { '\0' };
	
	SaveDefaultState(scn);

	//sprintf(buff, cfgfilename.c_str());
	//oapiWriteScenario_string(scn, "CONFIG_FILE", buff);


	if (follow_me) {
		DeleteFollowMe();
	}
	string fn("\\Vessels\\");
	string cn(GetClassName());
	fn += cn;
	fn += ".cfg";
	WriteCfgFile(fn);
	
	SBLog("Save State Finished");
}
void StationBuilder1::clbkPostCreation() {
	
}



void StationBuilder1::MoveFollowMe(VECTOR3 axis) {
	follow_me_pos += axis*follow_me_translation_speed*oapiGetSimStep();
	return;
}
void StationBuilder1::RotateFollowMe(VECTOR3 axis) {
	double angle = follow_me_rotation_speed*oapiGetSimStep();
	MATRIX3 rot_m = rotm(axis, angle);
	MATRIX3 early_rm = follow_me_rm;
	follow_me_rm = mul(early_rm, rot_m);
	return;
}
int StationBuilder1::clbkConsumeDirectKey(char *kstate) {
	if (follow_me) {
		if (KEYDOWN(kstate, OAPI_KEY_UP)) {
			if (KEYMOD_SHIFT(kstate)) {
				double angle = follow_me_rotation_speed*oapiGetSimStep();
				VECTOR3 axis = _V(1, 0, 0);
				MATRIX3 rot_m = rotm(axis, angle);
				MATRIX3 early_rm = follow_me_rm;
				follow_me_rm = mul(early_rm, rot_m);
				RESETKEY(kstate, OAPI_KEY_UP);
			}
			else if (KEYMOD_ALT(kstate)) {
				follow_me_pos.y += follow_me_translation_speed*oapiGetSimStep();
				RESETKEY(kstate, OAPI_KEY_UP);
			}
			else {
				follow_me_pos.z += follow_me_translation_speed*oapiGetSimStep();
				RESETKEY(kstate, OAPI_KEY_UP);
			}
			return 1;
		}
		if (KEYDOWN(kstate, OAPI_KEY_RIGHT)) {
			if (KEYMOD_SHIFT(kstate)) {
				double angle = follow_me_rotation_speed*oapiGetSimStep();
				VECTOR3 axis = _V(0, 1, 0);
				MATRIX3 rot_m = rotm(axis, angle);
				MATRIX3 early_rm = follow_me_rm;
				follow_me_rm = mul(early_rm, rot_m);
				RESETKEY(kstate, OAPI_KEY_RIGHT);
			}
			else {
				follow_me_pos.x += follow_me_translation_speed*oapiGetSimStep();
				RESETKEY(kstate, OAPI_KEY_RIGHT);
			}
			
		}
		if (KEYDOWN(kstate, OAPI_KEY_LEFT)) {
			if (KEYMOD_SHIFT(kstate)) {
				double angle = -follow_me_rotation_speed*oapiGetSimStep();
				VECTOR3 axis = _V(0, 1, 0);
				MATRIX3 rot_m = rotm(axis, angle);
				MATRIX3 early_rm = follow_me_rm;
				follow_me_rm = mul(early_rm, rot_m);
				RESETKEY(kstate, OAPI_KEY_LEFT);
			}
			else {
				follow_me_pos.x -= follow_me_translation_speed*oapiGetSimStep();
				RESETKEY(kstate, OAPI_KEY_LEFT);
			}
		}
		if (KEYDOWN(kstate, OAPI_KEY_DOWN)) {
			if (KEYMOD_SHIFT(kstate)) {
				double angle = -follow_me_rotation_speed*oapiGetSimStep();
				VECTOR3 axis = _V(1, 0, 0);
				MATRIX3 rot_m = rotm(axis, angle);
				MATRIX3 early_rm = follow_me_rm;
				follow_me_rm = mul(early_rm, rot_m);
				RESETKEY(kstate, OAPI_KEY_DOWN);
			}
			else if (KEYMOD_ALT(kstate)) {
				follow_me_pos.y -= follow_me_translation_speed*oapiGetSimStep();
				RESETKEY(kstate, OAPI_KEY_DOWN);
			}
			else {
				follow_me_pos.z -= follow_me_translation_speed*oapiGetSimStep();
				RESETKEY(kstate, OAPI_KEY_DOWN);
			}
		}
		if (KEYDOWN(kstate, OAPI_KEY_NEXT)) {
			if (KEYMOD_SHIFT(kstate)) {
				double angle = -follow_me_rotation_speed*oapiGetSimStep();
				VECTOR3 axis = _V(0, 0, 1);
				MATRIX3 rot_m = rotm(axis, angle);
				MATRIX3 early_rm = follow_me_rm;
				follow_me_rm = mul(early_rm, rot_m);
				RESETKEY(kstate, OAPI_KEY_NEXT);
			}
			
		}
		if (KEYDOWN(kstate, OAPI_KEY_PRIOR)) {
			if (KEYMOD_SHIFT(kstate)) {
				double angle = follow_me_rotation_speed*oapiGetSimStep();
				VECTOR3 axis = _V(0, 0, 1);
				MATRIX3 rot_m = rotm(axis, angle);
				MATRIX3 early_rm = follow_me_rm;
				follow_me_rm = mul(early_rm, rot_m);
				RESETKEY(kstate, OAPI_KEY_PRIOR);
			}
		}

	}
	return 0;
}


int StationBuilder1::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if (!down) return 0; 
	if (Playback()) return 0; //
	//if (!AnimEditingMode) {
	AnimMng->ConsumeAnimKey(key, down, kstate);
	//}
	
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_SPACE) {
		if (Dlg->IsOpen()) {
			Dlg->Close();
		}
		else {
			Dlg->Open(hDLL);
		}
		/*if (hDlg) {
			oapiCloseDialog(hDlg);
			hDlg = NULL;
		}
		else {
			hDlg = oapiOpenDialogEx(hDLL, IDD_DIALOG1, DlgProc, DLG_CAPTIONCLOSE | DLG_CAPTIONHELP, this);
		}
		*/
		
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_SPACE) {
		if (follow_me) {
			DeleteFollowMe();
			
		}
		else {
			CreateFollowMe();
			
		}
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_K) {
		
	//	((MGROUP_ROTATE*)((ANIMATIONCOMP*)(anim_defs[0].animcomps[0].ach))->trans)->axis = _V(1, 0, 0);
		
	/*	anim_defs[0].animcomps[0].grps.clear();
		anim_defs[0].animcomps[0].grps.push_back(96);
		anim_defs[0].animcomps[0].grps.push_back(78);
		delete anim_defs[0].animcomps[0].pgrps;
		anim_defs[0].animcomps[0].pgrps = new UINT[2];
		anim_defs[0].animcomps[0].pgrps[0] = 96;
		anim_defs[0].animcomps[0].pgrps[1] = 78;
		((MGROUP_ROTATE*)((ANIMATIONCOMP*)(anim_defs[0].animcomps[0].ach))->trans)->grp = anim_defs[0].animcomps[0].pgrps;
		((MGROUP_ROTATE*)((ANIMATIONCOMP*)(anim_defs[0].animcomps[0].ach))->trans)->ngrp = 2;*/
	/*	if (anim_defs[0].running) {//to add the check also on backwards so it will stop on movimenti concordi and will reverse on movimenti discordi
			anim_defs[0].running = false;
			anim_defs[0].backward = false;
		}
		else {
			anim_defs[0].running = true;
			anim_defs[0].backward = false;
		}
		*/
		//MGROUP_TRANSFORM *mgt;
		//mgt = new MGROUP_TRANSFORM;
		//oapiWriteLogV("MGT: %i", mgt);
		//MGROUP_ROTATE *mgr = new MGROUP_ROTATE(0, NULL, 0, _V(0, 0, 0), _V(0, 1, 0), 90 * RAD);
		//delete mgt;
		//mgt = new MGROUP_ROTATE(0, NULL, 0, _V(0, 0, 0), _V(0, 1, 0), 90 * RAD);// dynamic_cast<MGROUP_ROTATE*>(mgt);

//		oapiWriteLogV("MGT: %i", mgt);
		
		//HighlightMeshGroup(0, 36, true);
		
		//MshMng->HighlightMeshGroup(0, 36, true);
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_K) {
		
		//HighlightMeshGroup(0, 36, false);
	//	SetAnimation(0, 0);
		/*if (anim_defs[0].running) {
			anim_defs[0].running = false;
			anim_defs[0].backward = true;
		}
		else {
			anim_defs[0].running = true;
			anim_defs[0].backward = true;
		}
		
		*/
	//	MshMng->HighlightMeshGroup(0, 36, false);
		
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_G) {
	/*	ANIM_COMPDEF acd;
		acd.state0 = 0;
		acd.state1 = 1;
		acd.msgt.nmesh = 0;
		acd.msgt.transform = MESHGROUP_TRANSFORM::ROTATE;
		acd.msgt.P.rotparam.ref = _V(0, 0, -10);
		acd.msgt.P.rotparam.axis = _V(0, 1, 0);
		acd.msgt.P.rotparam.angle = 135 * RAD;
		acd.grps.push_back(36);
		acd.msgt.nmesh = 0;
		
		//acd.msgt.transform = MESHGROUP_TRANSFORM::TRANSLATE;
		//acd.msgt.P.transparam.shift = _V(10, 10, 10);
		AddAnimCompDef(0, acd);*/
		
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_G) {
		SetAnimation(0, 1);
		return 1;
	}
	return 0;
}



void StationBuilder1::clbkPreStep(double simt, double simdt, double mjd) {
	AnimMng->AnimationPreStep(simt, simdt, mjd);
	if (follow_me) {
		UpdateFollowMe();
	}
	
	
	return;
}
void StationBuilder1::clbkPostStep(double simt, double simdt, double mjd) {
	
	return;
}
void StationBuilder1::clbkVisualCreated(VISHANDLE vis, int refcount) {
	visual = vis;
	MshMng->VisualCreated(vis, refcount);

	return;
}
void StationBuilder1::clbkVisualDestroyed(VISHANDLE vis, int refcount) {
	visual = NULL;
	MshMng->VisualDestroyed(vis, refcount);

	return;
}
bool StationBuilder1::clbkLoadVC(int id) {
	return true;
}

void StationBuilder1::VehicleSetup() {
	SBLog("Vehicle Setup Started...");
	ResetVehicle();
	MshMng->LoadMeshes();
	CreateDocks();
	SBLog("Vehicle Setup Finished...");
}
MATRIX3 StationBuilder1::FindRM(VECTOR3 dir2, VECTOR3 rot2) {
	return FindRM(_V(0, 0, 1), _V(0, 1, 0), dir2, rot2);
}
MATRIX3 StationBuilder1::FindRM(VECTOR3 dir1, VECTOR3 rot1, VECTOR3 dir2, VECTOR3 rot2) {
	VECTOR3 x_axis = _V(1, 0, 0);
	VECTOR3 y_axis = _V(0, 1, 0);

	double angle1 = acos(dotp(dir2, dir1));
	VECTOR3 cp1 = crossp(dir2, dir1);
	VECTOR3 axis1;
	if (length(cp1) > 0.0000001) {
		axis1 = cp1;
	}
	else {
		if ((abs(dir1.x) != x_axis.x) || (abs(dir2.x) != x_axis.x)) {
			axis1 = crossp(dir2, x_axis);
		}
		else {
			axis1 = crossp(dir2, y_axis);
		}
	}
	normalise(axis1);

	MATRIX3 m_rot1 = rotm(axis1, angle1);
	VECTOR3 rotated_rot = mul(m_rot1, rot2);
	double angle2 = acos(dotp(rotated_rot, rot1));
	VECTOR3 cp2 = crossp(rotated_rot, rot1);
	VECTOR3 axis2;
	if (length(cp2) > 0.0000001) {
		axis2 = cp2;
	}
	else {
		axis2 = -dir1;
	}
	normalise(axis2);

	MATRIX3 m_rot2 = rotm(axis2, angle2);
	MATRIX3 m_rot_def = mul(m_rot2, m_rot1);
	return m_rot_def;
}

void StationBuilder1::FindDirRot(MATRIX3 rm, VECTOR3 &dir, VECTOR3 &rot) {
	VECTOR3 zz = _V(0, 0, 1);
	VECTOR3 yy = _V(0, 1, 0);
	dir = tmul(rm, zz);
	rot = tmul(rm, yy);
	normalise(dir);
	normalise(rot);

	return;
}
void StationBuilder1::ResetVehicle() {
	SBLog("Resetting Vehicle...");
	ClearMeshes();
	ClearAttachments();
	ClearDockDefinitions();
	ClearLightEmitters();
	ClearBeacons();
	SBLog("Reset Complete");
}

void StationBuilder1::ParseCfgFile(FILEHANDLE fh) {
	SBLog("Parsing Configuration File");
	if (fh == NULL) { SBLog("WARNING: cfg file handle NULL!"); }
	//FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_IN_ZEROONFAIL, CONFIG);
	//if (fh == NULL) { 
	//	SBLog("ERROR: Failed To Open Config File!");
	//	return; }
	UINT mesh_counter = 0;
	char cbuf[256] = { '\0' };
	char item[256] = { '\0' };
	sprintf_s(cbuf, "MESH_%i_NAME", mesh_counter);
	while (oapiReadItem_string(fh, cbuf, item)) {
		string mn(item);
		VECTOR3 pos = _V(0, 0, 0);
		VECTOR3 dir = _V(0, 0, 1);
		VECTOR3 rot = _V(0, 1, 0);
		sprintf_s(cbuf, "MESH_%i_POS", mesh_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf_s(cbuf, "MESH_%i_DIR", mesh_counter);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf_s(cbuf, "MESH_%i_ROT", mesh_counter);
		oapiReadItem_vec(fh, cbuf, rot);
		MshMng->AddMeshDef(mn, pos, dir, rot);
		mesh_counter++;
		sprintf_s(cbuf, "MESH_%i_NAME", mesh_counter);
	}
	SBLog("Found %i Mesh Definitions", MshMng->GetMeshCount());
	
	UINT dock_counter = 0;
	VECTOR3 dpos = _V(0, 0, 0);
	sprintf_s(cbuf, "DOCK_%i_POS", dock_counter);
	while (oapiReadItem_vec(fh, cbuf, dpos)) {
		VECTOR3 ddir = _V(0, 0, 1);
		VECTOR3 drot = _V(0, 1, 0);
		sprintf_s(cbuf, "DOCK_%i_DIR", dock_counter);
		oapiReadItem_vec(fh, cbuf, ddir);
		sprintf_s(cbuf, "DOCK_%i_ROT", dock_counter);
		oapiReadItem_vec(fh, cbuf, drot);
		DCK_DEF dd = DCK_DEF();
		dd.pos = dpos;
		dd.dir = ddir;
		dd.rot = drot;
		dock_definitions.push_back(dd);
		dock_counter++;
		sprintf_s(cbuf, "DOCK_%i_POS", dock_counter);
	}
	SBLog("Found %i Dock Definitions", dock_definitions.size());

	AnimMng->ParseCfgFile(fh);
	

	SBLog("Parsing Completed");
	return;
}
void StationBuilder1::WriteCfgFile(string filename) {
	FILEHANDLE fh=oapiOpenFile(filename.c_str(), FILE_OUT, CONFIG);
	char cbuf[256] = { '\0' };
	oapiWriteLine(fh, "ClassName = StationBuilder1");
	oapiWriteLine(fh, "Module = StationBuilder1");
	oapiWriteLine(fh, " ");
	sprintf_s(cbuf, ";CONFIGURATION FILE FOR %s",GetName());
	oapiWriteLine(fh, cbuf);
	oapiWriteLine(fh, ";<-------------------------MESHES DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");

	
	MshMng->WriteCfg(fh);

	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------DOCKS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < dock_definitions.size(); i++) {
		sprintf_s(cbuf, "DOCK_%i_POS",i);
		oapiWriteItem_vec(fh, cbuf, dock_definitions[i].pos);
		sprintf_s(cbuf, "DOCK_%i_DIR",i);
		oapiWriteItem_vec(fh, cbuf, dock_definitions[i].dir);
		sprintf_s(cbuf, "DOCK_%i_ROT",i);
		oapiWriteItem_vec(fh, cbuf, dock_definitions[i].rot);
		oapiWriteLine(fh, " ");
	}
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------ANIMATIONS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	AnimMng->WriteCfg(fh);

	oapiCloseFile(fh, FILE_OUT);
	return;
}
void StationBuilder1::WriteBackupFile() {
	
	string fn = OrbiterRoot;
	fn += configdir;
	fn += "\\Vessels\\";
	fn += GetClassName();
	
	string fnbackup = fn;
	fnbackup += "__BACKUP.cfg";
	fn += ".cfg";
	ifstream cfgfile;
	ofstream cfgbackup;
	cfgfile.open(fn.c_str());
	cfgbackup.open(fnbackup.c_str());
	string line;
	while (getline(cfgfile, line)) {
		cfgbackup << line<<"\n";
	}
	cfgfile.close();
	cfgbackup.close();
}

void StationBuilder1::CreateFollowMe(VECTOR3 initpos) {
	follow_me_pos = initpos;
	return CreateFollowMe();
}
void StationBuilder1::CreateFollowMe() {
	VESSELSTATUS2 vs2;
	memset(&vs2, 0, sizeof(vs2));
	vs2.version = 2;
	GetStatusEx(&vs2);
	Local2Rel(follow_me_pos, vs2.rpos);
	h_follow_me = oapiCreateVesselEx("SB1_tgt", "CamTarget", &vs2);
	follow_me = true;
	FMDlg->Open(hDLL);
	oapiCameraAttach(h_follow_me, 1);
	
	return;
}
void StationBuilder1::DeleteFollowMe(bool reset) {
	oapiAnnotationSetText(follow_me_noteh, "");
	if (oapiIsVessel(h_follow_me)) {
		oapiDeleteVessel(h_follow_me, GetHandle());
		h_follow_me = NULL;
		follow_me = false;
		if (reset) {
			ResetFollowMe();
		}
	}
	FMDlg->Close();
	//sprintf(oapiDebugString(), "");
}
void StationBuilder1::UpdateFollowMe() {
	if (!oapiIsVessel(h_follow_me)) { return; }
	if (oapiCameraTarget() != h_follow_me) {
		oapiCameraAttach(h_follow_me, 1);
	}
	VESSELSTATUS2 vs2;
	memset(&vs2, 0, sizeof(vs2));
	vs2.version = 2;
	GetStatusEx(&vs2);
	Local2Rel(follow_me_pos, vs2.rpos);
	VESSEL *v = oapiGetVesselInterface(h_follow_me);
	MATRIX3 myrm;
	GetRotationMatrix(myrm);
	MATRIX3 finalrm = mul(myrm, follow_me_rm);
	vs2.arot.x = atan2(finalrm.m23, finalrm.m33);
	vs2.arot.y = -asin(finalrm.m13);
	vs2.arot.z = atan2(finalrm.m12, finalrm.m11);
	v->DefSetStateEx(&vs2);
	VECTOR3 zz = _V(0, 0, 1);
	VECTOR3 yy = _V(0, 1, 0);
	VECTOR3 dir = mul(follow_me_rm, zz);
	VECTOR3 rot = mul(follow_me_rm, yy);
	normalise(dir);
	normalise(rot);
	follow_me_dir = dir;
	follow_me_rot = rot;
	char notetxt[256] = { '\0' };
	sprintf_s(notetxt, "X: %.3f Y:%.3f Z:%.3f \nDIR:%.3f %.3f %.3f \nROT:%.3f %.3f %.3f", follow_me_pos.x, follow_me_pos.y, follow_me_pos.z, dir.x, dir.y, dir.z, rot.x, rot.y, rot.z);
	oapiAnnotationSetText(follow_me_noteh, notetxt);
	//sprintf(oapiDebugString(), "Current Coordinates: %.3f %.3f %.3f Dir: %.3f %.3f %.3f Rot: %.3f %.3f %.3f", follow_me_pos.x, follow_me_pos.y, follow_me_pos.z,dir.x,dir.y,dir.z,rot.x,rot.y,rot.z);
	if (FMDlg->IsOpen()) {
		FMDlg->UpdatePosDirRot();
	}
}
void StationBuilder1::ResetFollowMe() {
	follow_me_pos = _V(0, 0, 0);
	follow_me_rm = _M(1, 0, 0, 0, 1, 0, 0, 0, 1);
	return;
}







void StationBuilder1::AddDockDef() {
	DCK_DEF dd = DCK_DEF();
	
	dd.dh=CreateDock(dd.pos, dd.dir, dd.rot);
	dock_definitions.push_back(dd);
	return;
}
void StationBuilder1::AddDockDef(DCK_DEF dd) {
	dd.dh = CreateDock(dd.pos, dd.dir, dd.rot);
	dock_definitions.push_back(dd);
	return;
}
bool StationBuilder1::DeleteDockDef(int idx) {
	if ((dock_definitions.size() - 1 < idx) || (idx<0)) {
		SBLog("WARNING: Called a Delete Dock Definition with out of range index");
		return false;
	}
	DelDock(dock_definitions[idx].dh);
	dock_definitions.erase(dock_definitions.begin() + idx);
	return true;
}
void StationBuilder1::AddAttDef() {
	ATT_DEF ad = ATT_DEF();
	ad.ah = CreateAttachment(ad.toparent, ad.pos, ad.dir, ad.rot, ad.id.c_str(), ad.loose);
	att_definitions.push_back(ad);
	return;
}
void StationBuilder1::AddAttDef(ATT_DEF ad) {
	ad.ah = CreateAttachment(ad.toparent, ad.pos, ad.dir, ad.rot, ad.id.c_str(), ad.loose);
	att_definitions.push_back(ad);
	return;
}
bool StationBuilder1::DeleteAttDef(int idx) {
	if ((att_definitions.size() - 1 < idx) || (idx<0)) {
		SBLog("WARNING: Called a Delete Attachment Definition with out of range index");
		return false;
	}
	DelAttachment(att_definitions[idx].ah);
	att_definitions.erase(att_definitions.begin() + idx);
	return true;
}

void StationBuilder1::CreateDockBeacons() {
	UINT ndocks = dock_definitions.size();
	dockbcn = new BEACONSPOTS[ndocks * 5];
	for (UINT i = 0; i < dock_definitions.size(); i++) {
		
		dockbcn[i*5].pos = dock_definitions[i].pos;
		dockbcn[i*5].bls.pos = &dockbcn[i*5].pos;
		dockbcn[i*5].bls.shape = BEACONSHAPE_COMPACT;
		dockbcn[i*5].bls.col = &colwhite;
		dockbcn[i*5].bls.active = true;
		dockbcn[i*5].bls.size = 0.075;
		dockbcn[i*5].bls.period = 0;
		dockbcn[i*5].bls.falloff = 0.99;
		AddBeacon(&dockbcn[i*5].bls);

		dockbcn[5*i+1].pos = (dock_definitions[i].pos+dock_definitions[i].dir);
		dockbcn[5*i+1].bls.pos = &dockbcn[5*i + 1].pos;
		dockbcn[5*i+1].bls.shape = BEACONSHAPE_COMPACT;
		dockbcn[5*i+1].bls.col = &colgreen;
		dockbcn[5*i+1].bls.active = true;
		dockbcn[5*i+1].bls.size = 0.075;
		dockbcn[5*i+1].bls.period = 0;
		dockbcn[5*i+1].bls.falloff = 0.99;
		AddBeacon(&dockbcn[5*i+1].bls);

		dockbcn[5 * i + 2].pos = (dock_definitions[i].pos + (dock_definitions[i].dir*2));
		dockbcn[5 * i + 2].bls.pos = &dockbcn[5*i + 2].pos;
		dockbcn[5 * i + 2].bls.shape = BEACONSHAPE_COMPACT;
		dockbcn[5 * i + 2].bls.col = &colgreen;
		dockbcn[5 * i + 2].bls.active = true;
		dockbcn[5 * i + 2].bls.size = 0.075;
		dockbcn[5 * i + 2].bls.period = 0;
		dockbcn[5 * i + 2].bls.falloff = 0.99;
		AddBeacon(&dockbcn[5 * i + 2].bls);

		dockbcn[5 * i + 3].pos = (dock_definitions[i].pos + (dock_definitions[i].rot));
		dockbcn[5 * i + 3].bls.pos = &dockbcn[5 * i + 3].pos;
		dockbcn[5 * i + 3].bls.shape = BEACONSHAPE_COMPACT;
		dockbcn[5 * i + 3].bls.col = &colblue;
		dockbcn[5 * i + 3].bls.active = true;
		dockbcn[5 * i + 3].bls.size = 0.075;
		dockbcn[5 * i + 3].bls.period = 0;
		dockbcn[5 * i + 3].bls.falloff = 0.99;
		AddBeacon(&dockbcn[5 * i + 3].bls);

		dockbcn[5 * i + 4].pos = (dock_definitions[i].pos + (dock_definitions[i].rot*2));
		dockbcn[5 * i + 4].bls.pos = &dockbcn[5 * i + 4].pos;
		dockbcn[5 * i + 4].bls.shape = BEACONSHAPE_COMPACT;
		dockbcn[5 * i + 4].bls.col = &colblue;
		dockbcn[5 * i + 4].bls.active = true;
		dockbcn[5 * i + 4].bls.size = 0.075;
		dockbcn[5 * i + 4].bls.period = 0;
		dockbcn[5 * i + 4].bls.falloff = 0.99;
		AddBeacon(&dockbcn[5 * i + 4].bls);

		
	}
	DockBeaconsActive = true;
}
void StationBuilder1::DeleteDockBeacons() {
	DockBeaconsActive = false;
	ClearBeacons();
	if (dockbcn) {
		delete[] dockbcn;
	}
	return;
}

void StationBuilder1::UpdateDockBeaconsPos() {
	for (UINT i = 0; i < dock_definitions.size(); i++) {
		dockbcn[i * 5].pos = dock_definitions[i].pos;
		dockbcn[5 * i + 1].pos = (dock_definitions[i].pos + dock_definitions[i].dir);
		dockbcn[5 * i + 2].pos = (dock_definitions[i].pos + (dock_definitions[i].dir * 2));
		dockbcn[5 * i + 3].pos = (dock_definitions[i].pos + (dock_definitions[i].rot));
		dockbcn[5 * i + 4].pos = (dock_definitions[i].pos + (dock_definitions[i].rot * 2));
	}
	return;
}


bool StationBuilder1::UsingD3D9() {
//	oapiWriteLogV("SB1::UsingD3D9:%i", wD3D9);
	return wD3D9;
}
MATRIX3 StationBuilder1::Inverse(MATRIX3 m) {
	MATRIX3 r;
	MATRIX3 adjugate;
	adjugate.m11 = m.m22*m.m33 - m.m23 * m.m32;
	adjugate.m12 = -(m.m12*m.m33 - m.m13*m.m32);
	adjugate.m13 = m.m12*m.m23 - m.m13*m.m22;
	adjugate.m21 = -(m.m21*m.m33 - m.m23*m.m31);
	adjugate.m22 = m.m11*m.m33 - m.m13*m.m31;
	adjugate.m23 = -(m.m11*m.m23 - m.m13*m.m21);
	adjugate.m31 = m.m21*m.m32 - m.m22*m.m31;
	adjugate.m32 = -(m.m11*m.m32 - m.m12*m.m31);
	adjugate.m33 = m.m11*m.m22 - m.m12*m.m21;
	double detm = m.m11*m.m22*m.m33 + m.m12*m.m23*m.m31+m.m13*m.m21*m.m32-(m.m31*m.m22*m.m13+m.m32*m.m23*m.m11+m.m21*m.m12*m.m33);
	r = adjugate / detm;

	return r;
}




vector <string> StationBuilder1::ReadMeshTextures(string filename) {
	string folder("//Meshes//");
	string ext(".msh");
	//string fn = folder + filename;
	//FILEHANDLE mh_f = oapiOpenFile(fn.c_str(), FILE_IN_ZEROONFAIL, ROOT);
	vector<string> texture_names;
	texture_names.clear();

	char OR[1024] = { '\0' };
	GetCurrentDirectory(1024, OR);
	string OrbiterRoot(OR);
	string fn = OrbiterRoot + folder + filename + ext;
	string line;
	ifstream meshfile(fn);
	bool accept = false;
	if (meshfile) {
		while (getline(meshfile, line)) {
			if (accept) {
				texture_names.push_back(line);
			}
			string test = line.substr(0, 8);
			if (test.compare("TEXTURES") == 0) {
				accept = true;
			}
		}
		meshfile.close();
	}


	return texture_names;
	//oapiCloseFile(mh_f, FILE_IN_ZEROONFAIL);
}
/*void StationBuilder1::WriteUniqueMeshFile(string filename) {
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_OUT, ROOT);
	char line[1024] = { '\0' };
	oapiWriteLine(fh, "MSHX1");
	int total_groups = 0;
	int total_materials = 0;
	int total_textures = 0;
	for (UINT i = 0; i < mesh_definitions.size(); i++) {
		if (mesh_definitions[i].msh_h == NULL) { continue; }
		int k = oapiMeshGroupCount(mesh_definitions[i].msh_h);
		total_groups += k;

		k = oapiMeshMaterialCount(mesh_definitions[i].msh_h);
		total_materials += k;

		k = oapiMeshTextureCount(mesh_definitions[i].msh_h);
		total_textures += k;
	}


	sprintf(line, "GROUPS %i", total_groups);
	oapiWriteLine(fh, line);
	int material_offset = 0;
	int texture_offset = 0;
	vector<MATERIAL*> mats;
	mats.clear();
	vector<string> tex_names;
	tex_names.clear();

	for (UINT i = 0; i < mesh_definitions.size(); i++) {
		if (mesh_definitions[i].msh_h == NULL) { continue; }
		UINT groups_count = oapiMeshGroupCount(mesh_definitions[i].msh_h);
		if (groups_count <= 0) { continue; }
		for (UINT j = 0; j < groups_count; j++) {
			MESHGROUP *mg = oapiMeshGroup(mesh_definitions[i].msh_h, j);
			int nVtx = mg->nVtx;
			int nIdx = mg->nIdx;
			sprintf(line, "MATERIAL %i", mg->MtrlIdx + 1 + material_offset);
			oapiWriteLine(fh, line);
			sprintf(line, "TEXTURE %i", mg->TexIdx + 1 + texture_offset);
			oapiWriteLine(fh, line);
			sprintf(line, "GEOM %i %i", nVtx, nIdx / 3);
			oapiWriteLine(fh, line);
			for (UINT k = 0; k < nVtx; k++) {
				VECTOR3 vtx_p = _V(mg->Vtx[k].x, mg->Vtx[k].y, mg->Vtx[k].z);
				VECTOR3 vtx_n = _V(mg->Vtx[k].nx, mg->Vtx[k].ny, mg->Vtx[k].nz);
				vtx_p = mul(mesh_definitions[i].rm, vtx_p);
				vtx_n = mul(mesh_definitions[i].rm, vtx_n);


				sprintf(line, "%.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f", vtx_p.x + mesh_definitions[i].ofs.x, vtx_p.y + mesh_definitions[i].ofs.y, vtx_p.z + mesh_definitions[i].ofs.z, vtx_n.x, vtx_n.y, vtx_n.z, mg->Vtx[k].tu, mg->Vtx[k].tv);

				oapiWriteLine(fh, line);
			}
			for (UINT z = 0; z < nIdx - 2; z += 3) {
				sprintf(line, "%i %i %i", mg->Idx[z], mg->Idx[z + 1], mg->Idx[z + 2]);
				oapiWriteLine(fh, line);
			}





		}

		int matn = oapiMeshMaterialCount(mesh_definitions[i].msh_h);
		material_offset += matn;
		for (UINT q = 0; q < matn; q++) {
			MATERIAL* mat = oapiMeshMaterial(mesh_definitions[i].msh_h, q);
			mats.push_back(mat);
		}
		int texn = oapiMeshTextureCount(mesh_definitions[i].msh_h);
		texture_offset += texn;

		if (texn > 0) {
			vector<string> meshtexnames = ReadMeshTextures(mesh_definitions[i].meshname.c_str());
			if (meshtexnames.size() <= 0) {
				for (UINT t = 0; t < texn; t++) {
					char tname[256] = { '\0' };
					sprintf(tname, "NOTEX_%s_.dds", mesh_definitions[i].meshname.c_str());
					string tn(tname);
					meshtexnames.push_back(tn);
				}
			}
			for (UINT m = 0; m < texn; m++) {
				tex_names.push_back(meshtexnames[m]);
			}
		}

	}
	sprintf(line, "MATERIALS %i", total_materials);
	oapiWriteLine(fh, line);
	for (UINT p = 0; p < mats.size(); p++) {
		sprintf(line, "MAT_%i", p);
		oapiWriteLine(fh, line);
	}

	for (UINT r = 0; r < mats.size(); r++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "MAT_%i", r);
		sprintf(line, "MATERIAL %s", cbuf);
		oapiWriteLine(fh, line);
		sprintf(line, "%.6f %.6f %.6f %.6f", mats[r]->diffuse.r, mats[r]->diffuse.g, mats[r]->diffuse.b, mats[r]->diffuse.a);
		oapiWriteLine(fh, line);
		sprintf(line, "%.6f %.6f %.6f %.6f", mats[r]->ambient.r, mats[r]->ambient.g, mats[r]->ambient.b, mats[r]->ambient.a);
		oapiWriteLine(fh, line);
		sprintf(line, "%.6f %.6f %.6f %.6f %.3f", mats[r]->specular.r, mats[r]->specular.g, mats[r]->specular.b, mats[r]->specular.a, mats[r]->power);
		oapiWriteLine(fh, line);
		sprintf(line, "%.6f %.6f %.6f %.6f", mats[r]->emissive.r, mats[r]->emissive.g, mats[r]->emissive.b, mats[r]->emissive.a);
		oapiWriteLine(fh, line);
	}

	sprintf(line, "TEXTURES %i", total_textures);
	oapiWriteLine(fh, line);
	for (UINT n = 0; n < tex_names.size(); n++) {
		sprintf(line, "%s", tex_names[n].c_str());
		oapiWriteLine(fh, line);
	}




	oapiCloseFile(fh, FILE_OUT);
}*/
/*
void StationBuilder1::DefineAnimations() {
	for (UINT i = 0; i < animations_definitions.size(); i++) {
		animations_definitions[i].anim_idx = CreateAnimation(animations_definitions[i].defstate);
	}
	for (UINT i = 0; i < animcomps_definitions.size(); i++) {
		switch (animcomps_definitions[i].mgt.transform) {
		case MESHGROUP_TRANSFORM::ROTATE:
			{
			MGROUP_ROTATE* mgrr = new MGROUP_ROTATE(animcomps_definitions[i].mgt.nmesh, NULL, 0, animcomps_definitions[i].mgt.P.rotparam.ref, animcomps_definitions[i].mgt.P.rotparam.axis, animcomps_definitions[i].mgt.P.rotparam.angle);
			mgr.push_back(mgrr);
			UINT anim_idx = (UINT)-1;
			for (UINT j = 0; j < animations_definitions.size(); j++) {
				if (animations_definitions[j].seq_idx == animcomps_definitions[i].seq_idx) {
					anim_idx = animations_definitions[j].anim_idx;
				}
			}
			animcomps_definitions[i].ach = AddAnimationComponent(anim_idx, animcomps_definitions[i].state0, animcomps_definitions[i].state1, mgr.back());


				break;
			}
		}
	}
}

void StationBuilder1::ClearDelete(vector<MGROUP_ROTATE*>vmgr) {
	for (std::vector< MGROUP_ROTATE* >::iterator it = vmgr.begin(); it != vmgr.end(); ++it)
	{
		delete (*it);
	}
	vmgr.clear();
}
*/
//Creation





DLLCLBK void InitModule(HINSTANCE hModule) {
	hDLL = hModule;
	oapiRegisterCustomControls(hModule);
	
}
DLLCLBK void ExitModule(HINSTANCE hModule) {
	oapiUnregisterCustomControls(hModule);
	
	//g_Param.hDLL = NULL;
}
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){return new StationBuilder1(hvessel,flightmodel);}
DLLCLBK void ovcExit(VESSEL *vessel){if(vessel)delete(StationBuilder1*)vessel;}


