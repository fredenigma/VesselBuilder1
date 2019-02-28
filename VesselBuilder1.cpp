#define ORBITER_MODULE



#include <math.h>
#include <stdio.h>
#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "FollowMeDlg.h"
#include "resource.h"
#include "DlgCtrl.h"
#include "gcAPI.h"
#include "MeshManager.h"
#include "AnimDef.h"
#include "AnimCompDef.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"
#include "PropellantManager.h"
#include "ThrusterManager.h"
#include "ParticleManager.h"
#include "TouchdownPointsManager.h"
#include "AirfoilsManager.h"
#include "ControlSurfacesManager.h"
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

HINSTANCE hDLL;
//Creation
VesselBuilder1::VesselBuilder1(OBJHANDLE hObj,int fmodel):VESSEL4(hObj,fmodel){
	gcInitialize();
	OrbiterRoot.clear();
	char root[MAX_PATH] = { '\0' };
	GetCurrentDirectory(MAX_PATH, root);
	OrbiterRoot.assign(root);
	wD3D9 = false;
	GetOrbiterDirs();
	
	MshMng = new MeshManager(this);
	AttMng = new AttachmentManager(this);
	AnimMng = new AnimationManager(this);
	PrpMng = new PropellantManager(this);
	ThrMng = new ThrusterManager(this);
	ThrGrpMng = new ThrusterGroupManager(this);
	PartMng = new ParticleManager(this);
	TdpMng = new TouchdownPointsManager(this);
	AirfoilMng = new AirfoilsManager(this);
	CtrSurfMng = new ControlSurfacesManager(this);

	dock_definitions.clear();
	extex_defs.clear();
	
	cfgfilename.clear();
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
	Dlg = new DialogControl(this);
	FMDlg = new FollowMeDlg(this);
	
	vclip = V_CLIPBOARD();
	DockExhaustsActive = false;
	DockExhaustsID.clear();
	AttExhaustsActive = false;
	AttExhaustsID.clear();
	ThGrpExhaustsID.clear();
	thExhaustsActive = false;
	ThExaustsID.clear();
	
	TDPCurExhaustsID.clear();
	TDPSetExhaustsID.clear();
	TdpCurExhaustsActive = false;
	TdpSetExhaustsActive = false;

	GrabMode = false;
	currentGrabAtt = 0;
	NoEditor = false;
	level1 = 1.0;
	SBLog("Class Initialize");
	return;
}

VesselBuilder1::~VesselBuilder1(){
	delete Dlg;
	Dlg = NULL;
	delete FMDlg;
	FMDlg = NULL;
	delete MshMng;
	MshMng = NULL;
	delete AttMng;
	AttMng = NULL;
	delete AnimMng;
	AnimMng = NULL;
	delete PrpMng;
	PrpMng = NULL;
	delete ThrMng;
	ThrMng = NULL;
	delete ThrGrpMng;
	ThrGrpMng = NULL;
	delete PartMng;
	PartMng = NULL;
	delete TdpMng;
	TdpMng = NULL;
	delete AirfoilMng;
	AirfoilMng = NULL;
	delete CtrSurfMng;
	CtrSurfMng = NULL;

	CloseSBLog();
	
	//ClearDelete(mgr);
	return;
}


void VesselBuilder1::GetOrbiterDirs() {
	string cfgname("Orbiter.cfg");
	if (gcEnabled()) { 
		cfgname.assign("Orbiter_NG.cfg"); 
		wD3D9 = true;

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

void VesselBuilder1::CreateDocks() {
	SBLog("Creating Docks...");
	for (UINT i = 0; i < dock_definitions.size(); i++) {
		dock_definitions[i].dh = CreateDock(dock_definitions[i].pos, dock_definitions[i].dir, dock_definitions[i].rot);
	}
	SBLog("Creation of %i Docking ports completed", dock_definitions.size());

	return;
}
void VesselBuilder1::clbkSetClassCaps(FILEHANDLE cfg){
	SetEmptyMass(1000);
	SetSize(10);
	
	redL = oapiRegisterExhaustTexture("red_L");
	greenL = oapiRegisterExhaustTexture("green_L");
	blueL = oapiRegisterExhaustTexture("blue_L");

	SBLog("ClassName:%s", GetClassName());
	
	ResetVehicle();
	ParseCfgFile(cfg);
	VehicleSetup();
	if (!NoEditor) {
		WriteBackupFile();
	}
	SetNosewheelSteering(true);
	SetMaxWheelbrakeForce(2e5);
	//SetCW(0.09, 0.09, 2, 1.4);
	SetRotDrag(_V(0.10, 0.13, 0.04));
	return;
}
void VesselBuilder1::clbkLoadStateEx(FILEHANDLE scn,void *vs)
{
	SBLog("Loading State...");
	char *line;
	while (oapiReadScenario_nextline(scn, line)) {
		if (!_strnicmp(line, "ANIM_", 5)) {
			UINT seq = 0;
			sscanf(line + 5, "%i", &seq);
			if (seq >= AnimMng->GetAnimDefCount()) { continue; }
			UINT c = 1;
			if ((seq > 9)&&(seq<100)) { c = 2; }
			else if ((seq > 99) && (seq < 1000)) { c = 3; }
			else if ((seq > 999) && (seq < 10000)) { c = 4; }
			double state;
			int status;
			sscanf(line + 5 + 1 + c, "%lf %d", &state, &status);
			AnimMng->SetAnimationState(seq, state);
			if (status == 1) {
				AnimMng->StartAnim(seq);
				AnimMng->SetAnimBackward(seq, false);
			}
			else if (status == -1) {
				AnimMng->StartAnim(seq);
				AnimMng->SetAnimBackward(seq, true);
			}
		}
		
		else{ 
			ParseScenarioLineEx(line, vs); 
		}	
	}


	SBLog("Loading State Finished");
}
void VesselBuilder1::clbkSaveState(FILEHANDLE scn)
{
	SBLog("Saving State...");

	
	
	SaveDefaultState(scn);

	for (UINT i = 0; i < AnimMng->GetAnimDefCount(); i++) {
		char buff[256] = { '\0' };
		char buff2[256] = { '\0' };
		sprintf(buff, "ANIM_%i",i);
		sprintf(buff2,"%.5f %i", AnimMng->GetAnimationState(i), AnimMng->AnimationRunStatus(i));
		oapiWriteScenario_string(scn, buff, buff2);
		//oapiWriteScenario_float(scn, buff, AnimMng->GetAnimationState(i));
	}
	

	if (follow_me) {
		DeleteFollowMe();
	}
	string fn("\\Vessels\\");
	string cn(GetClassName());
	fn += cn;
	fn += ".cfg";
	if (!NoEditor) {
		WriteCfgFile(fn);
	}
	
	
	SBLog("Save State Finished");
}
void VesselBuilder1::clbkPostCreation() {
	
}

bool VesselBuilder1::ToggleGrapple() {
	def_idx AttIdx = AttMng->IdxAtt2Def(currentGrabAtt);
	
	
	ATTACHMENTHANDLE ah = GetAttachmentHandle(false, currentGrabAtt);
	OBJHANDLE h_attached = GetAttachmentStatus(ah);
	if (h_attached != NULL) {
		VECTOR3 gpos, grms, pos, dir, rot;
		GetAttachmentParams(ah, pos, dir, rot);
		Local2Global(pos, grms);  // global position of RMS tip
		double distbar = AttMng->GetAttDefRange(AttIdx);
		OBJHANDLE h_candidate = NULL;
		int index_candidate = -1;
		ATTACHMENTHANDLE att_h_candidate = NULL;
		for (DWORD i = 0; i < oapiGetVesselCount(); i++) {
			OBJHANDLE hV = oapiGetVesselByIndex(i);
			if (hV == GetHandle()) continue; // we don't want to grapple ourselves ...
			oapiGetGlobalPos(hV, &gpos);
			VESSEL *v = oapiGetVesselInterface(hV);
			DWORD nAttach = v->AttachmentCount(true);
			for (DWORD j = 0; j < nAttach; j++) { // now scan all attachment points of the candidate
				ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle(true, j);
				v->GetAttachmentParams(hAtt, pos, dir, rot);
				v->Local2Global(pos, gpos);
				if (dist(gpos, grms) < distbar) {
					distbar = dist(gpos, grms);
					h_candidate = hV;
					index_candidate = j;
					att_h_candidate = hAtt;
				}
			}
		}
		if ((index_candidate == -1) || (h_candidate == NULL)) {
			return false;
		}
		AttachChild(h_candidate, ah, att_h_candidate);
		return true;
	}
	else {
		DetachChild(ah, 0.01);
	}
	
	
}

void VesselBuilder1::MoveFollowMe(VECTOR3 axis) {
	follow_me_pos += axis*follow_me_translation_speed*oapiGetSimStep();
	return;
}
void VesselBuilder1::RotateFollowMe(VECTOR3 axis) {
	double angle = follow_me_rotation_speed*oapiGetSimStep();
	MATRIX3 rot_m = rotm(axis, angle);
	MATRIX3 early_rm = follow_me_rm;
	follow_me_rm = mul(early_rm, rot_m);
	return;
}
void VesselBuilder1::ConsumeFollowMeKey(char *kstate) {
	if (KEYDOWN(kstate, OAPI_KEY_UP)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = 60*RAD*oapiGetSimStep();
			VECTOR3 axis = _V(1, 0, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_UP);
		}
		else if (KEYMOD_ALT(kstate)) {
			follow_me_pos.y += 6*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_UP);
		}
		else {
			follow_me_pos.z += 6*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_UP);
		}
	}
	if (KEYDOWN(kstate, OAPI_KEY_RIGHT)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = 60 * RAD*oapiGetSimStep();
			VECTOR3 axis = _V(0, 1, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_RIGHT);
		}
		else {
			follow_me_pos.x += 6*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_RIGHT);
		}

	}
	if (KEYDOWN(kstate, OAPI_KEY_LEFT)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = -60 * RAD*oapiGetSimStep();
			VECTOR3 axis = _V(0, 1, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_LEFT);
		}
		else {
			follow_me_pos.x -= 6*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_LEFT);
		}
	}
	if (KEYDOWN(kstate, OAPI_KEY_DOWN)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = -60 * RAD*oapiGetSimStep();
			VECTOR3 axis = _V(1, 0, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_DOWN);
		}
		else if (KEYMOD_ALT(kstate)) {
			follow_me_pos.y -= 6*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_DOWN);
		}
		else {
			follow_me_pos.z -= 6*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_DOWN);
		}
	}
	if (KEYDOWN(kstate, OAPI_KEY_NEXT)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = -60 * RAD*oapiGetSimStep();
			VECTOR3 axis = _V(0, 0, 1);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_NEXT);
		}

	}
	if (KEYDOWN(kstate, OAPI_KEY_PRIOR)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = 60 * RAD*oapiGetSimStep();
			VECTOR3 axis = _V(0, 0, 1);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_PRIOR);
		}
	}

	return;
}
int VesselBuilder1::clbkConsumeDirectKey(char *kstate) {
	if (follow_me) {
		ConsumeFollowMeKey(kstate);
	}
	if (!Dlg->IsOpen()) {
		AnimMng->ConsumeAnimDirectKey(kstate);
	}
	
	return 0;
}


int VesselBuilder1::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if (!down) return 0; 
	if (Playback()) return 0; //
	//if (!AnimEditingMode) {
	if (!Dlg->IsOpen()) {
		AnimMng->ConsumeAnimBufferedKey(key, down, kstate);
	}
	
	//}
	
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_SPACE) {
		if (!NoEditor) {
			if (Dlg->IsOpen()) {
				Dlg->Close();
			}
			else {
				Dlg->Open(hDLL);
			}
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
	if (!KEYMOD_ALT(kstate) && KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_SPACE) {
		if (follow_me) {
			DeleteFollowMe();
			
		}
		else {
			CreateFollowMe();
			
		}
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_K) {
	/*	DWORD td_count = this->GetTouchdownPointCount();
		for (UINT i = 0; i < td_count; i++) {
			oapiWriteLogV("td:%i", i);
			TOUCHDOWNVTX tdvtx;
			this->GetTouchdownPoint(tdvtx, i);
			oapiWriteLogV("pos:%.3f %.3f %.3f", tdvtx.pos.x, tdvtx.pos.y, tdvtx.pos.z);
			oapiWriteLogV("stiffness: %.6f damping:%.6f", tdvtx.stiffness, tdvtx.damping);
			oapiWriteLogV("Mu:%.3f Mu_lng:%.3f", tdvtx.mu, tdvtx.mu_lng);
		}
		*/




	
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_K) {
	/*	ResetVehicle();
		MshMng->msh_defs.clear();
		AttMng->att_defs.clear();
		PrpMng->tanks.clear();
		AirfoilMng->airfoil_defs.clear();
		AnimMng->animcomp_defs.clear();
		AnimMng->anim_defs.clear();
		PartMng->particle_defs.clear();
		FILEHANDLE newcfg = oapiOpenFile("Vessels//VesselBuilder1//HST.cfg", FILE_IN, CONFIG);
		clbkSetClassCaps(newcfg);
		oapiCloseFile(newcfg, FILE_IN);*/
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_G) {
		if (GrabMode) {
			ToggleGrapple();
		}

		
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_G) {
		if (!GrabMode) {
			if (AttachmentCount(false) > 0) {
				GrabMode = true;
			}
		}
		else {
			GrabMode = false;
			sprintf(oapiDebugString(), "");
		}

		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_RIGHT) {
		if (GrabMode) {
			if (currentGrabAtt < (AttachmentCount(false) - 1)) {
				currentGrabAtt += 1;
			}
			return 1;
		}
		
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_LEFT) {
		if (GrabMode) {
			if (currentGrabAtt > 0) {
				currentGrabAtt -= 1;
			}
			return 1;
		}

	}

	return 0;
}



void VesselBuilder1::clbkPreStep(double simt, double simdt, double mjd) {
	if (GrabMode) {
		def_idx attidx = AttMng->IdxAtt2Def(currentGrabAtt);
		sprintf(oapiDebugString(), "Attachment ready for Grab: %i %s", currentGrabAtt, AttMng->GetAttDefId(attidx).c_str());
	}
	AnimMng->AnimationPreStep(simt, simdt, mjd);
	if (follow_me) {
		UpdateFollowMe();
	}
	
	
	return;
}
void VesselBuilder1::clbkPostStep(double simt, double simdt, double mjd) {
	/*VECTOR3 CS;
	GetCrossSections(CS);
	double cw_zp, cw_zm, cw_x, cw_y;
	GetCW(cw_zp, cw_zm, cw_x, cw_y);
	VECTOR3 rd;
	GetRotDrag(rd);
	sprintf(oapiDebugString(), "GGD:%.3f CS:%.3f %.3f %.3f RD:%.3f %.3f %.3f cw:%.3f %.3f %.3f %.3f", GetGravityGradientDamping(), CS.x, CS.y, CS.z, rd.x, rd.y, rd.z, cw_zp, cw_zm, cw_x, cw_y);*/
	return;
}
void VesselBuilder1::clbkVisualCreated(VISHANDLE vis, int refcount) {
	visual = vis;
	MshMng->VisualCreated(vis, refcount);

	return;
}
void VesselBuilder1::clbkVisualDestroyed(VISHANDLE vis, int refcount) {
	visual = NULL;
	MshMng->VisualDestroyed(vis, refcount);

	return;
}
bool VesselBuilder1::clbkLoadVC(int id) {
	return true;
}

void VesselBuilder1::VehicleSetup() {
	SBLog("Vehicle Setup Started...");
//	ResetVehicle();
	MshMng->LoadMeshes();
	CreateDocks();
	SBLog("Vehicle Setup Finished...");
}
MATRIX3 VesselBuilder1::FindRM(VECTOR3 dir2, VECTOR3 rot2) {
	return FindRM(_V(0, 0, 1), _V(0, 1, 0), dir2, rot2);
}
MATRIX3 VesselBuilder1::FindRM(VECTOR3 dir1, VECTOR3 rot1, VECTOR3 dir2, VECTOR3 rot2) {
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

void VesselBuilder1::FindDirRot(MATRIX3 rm, VECTOR3 &dir, VECTOR3 &rot) {
	VECTOR3 zz = _V(0, 0, 1);
	VECTOR3 yy = _V(0, 1, 0);
	dir = tmul(rm, zz);
	rot = tmul(rm, yy);
	normalise(dir);
	normalise(rot);

	return;
}
void VesselBuilder1::ResetVehicle() {
	SBLog("Resetting Vehicle...");
	ClearMeshes();
	ClearAttachments();
	ClearDockDefinitions();
	ClearLightEmitters();
	ClearBeacons();
	ClearAirfoilDefinitions();
	ClearControlSurfaceDefinitions();
	ClearThrusterDefinitions();
	ClearLightEmitters();
	ClearPropellantResources();
	ClearVariableDragElements();
	SBLog("Reset Complete");
}

void VesselBuilder1::ParseCfgFile(FILEHANDLE fh) {
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
		dd.antidir = ddir*(-1);
		dd.antirot = drot*(-1);
		dock_definitions.push_back(dd);
		dock_counter++;
		sprintf_s(cbuf, "DOCK_%i_POS", dock_counter);
	}
	SBLog("Found %i Dock Definitions", dock_definitions.size());
	AttMng->ParseCfgFile(fh);
	AnimMng->ParseCfgFile(fh);
	PrpMng->ParseCfgFile(fh);

	UINT extex_counter = 0;
	int id;
	sprintf(cbuf, "EXTEX_%i_ID", extex_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "EXTEX_%i_TEXNAME", extex_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		AddExTexDef(name);
		extex_counter++;
		sprintf(cbuf, "EXTEX_%i_ID", extex_counter);
	}

	

	PartMng->ParseCfgFile(fh);
	ThrMng->ParseCfgFile(fh);
	ThrGrpMng->ParseCfgFile(fh);
	TdpMng->ParseCfgFile(fh);
	AirfoilMng->ParseCfgFile(fh);
	CtrSurfMng->ParseCfgFile(fh);

	if (!oapiReadItem_bool(fh, "NOEDITOR", NoEditor)) { NoEditor = false; }
	SBLog("Parsing Completed");
	return;
}
void VesselBuilder1::WriteCfgFile(string filename) {
	FILEHANDLE fh=oapiOpenFile(filename.c_str(), FILE_OUT, CONFIG);
	char cbuf[256] = { '\0' };
	oapiWriteLine(fh, "ClassName = VesselBuilder1");
	oapiWriteLine(fh, "Module = VesselBuilder1");
	oapiWriteLine(fh, " ");
	sprintf_s(cbuf, ";CONFIGURATION FILE FOR %s",GetName());
	oapiWriteLine(fh, cbuf);
	oapiWriteLine(fh, " ");
	oapiWriteItem_bool(fh, "NOEDITOR", NoEditor);
	oapiWriteLine(fh, " ");
	oapiWriteItem_float(fh, "Mass", GetEmptyMass());
	oapiWriteItem_float(fh, "Size", GetSize());
	VECTOR3 pmi;
	GetPMI(pmi);
	oapiWriteItem_vec(fh, "Inertia", pmi);
	VECTOR3 cs;
	GetCrossSections(cs);
	oapiWriteItem_vec(fh, "CrossSections", cs);
	oapiWriteItem_float(fh, "GravityGradientDamping", GetGravityGradientDamping());
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
	AttMng->WriteCfg(fh);
	AnimMng->WriteCfg(fh);
	PrpMng->WriteCfg(fh);
	
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------EXHAUST TEXTURES DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	UINT extex_counter = 0;
	for (UINT i = 0; i < GetExTexCount(); i++) {
		if (IsExTexCreated(i)) {
			sprintf(cbuf, "EXTEX_%i_ID", extex_counter);
			oapiWriteItem_int(fh, cbuf, extex_counter);
			sprintf(cbuf, "EXTEX_%i_TEXNAME", extex_counter);
			char namebuf[256] = { '\0' };
			sprintf(namebuf, "%s", GetExTexName(i).c_str());
			oapiWriteItem_string(fh, cbuf, namebuf);
			extex_counter++;
			oapiWriteLine(fh, " ");
		}
	}
	PartMng->WriteCfg(fh);
	ThrMng->WriteCfg(fh);
	ThrGrpMng->WriteCfg(fh);
	TdpMng->WriteCfg(fh);
	AirfoilMng->WriteCfg(fh);
	CtrSurfMng->WriteCfg(fh);
	oapiCloseFile(fh, FILE_OUT);
	return;
}
void VesselBuilder1::WriteBackupFile() {
	
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

void VesselBuilder1::CreateFollowMe(VECTOR3 initpos) {
	follow_me_pos = initpos;
	return CreateFollowMe();
}
void VesselBuilder1::CreateFollowMe() {
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
void VesselBuilder1::DeleteFollowMe(bool reset) {
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
void VesselBuilder1::UpdateFollowMe() {
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
void VesselBuilder1::ResetFollowMe() {
	follow_me_pos = _V(0, 0, 0);
	follow_me_rm = _M(1, 0, 0, 0, 1, 0, 0, 0, 1);
	return;
}







void VesselBuilder1::AddDockDef() {
	DCK_DEF dd = DCK_DEF();
	
	dd.dh=CreateDock(dd.pos, dd.dir, dd.rot);
	dock_definitions.push_back(dd);
	return;
}
void VesselBuilder1::AddDockDef(DCK_DEF dd) {
	dd.dh = CreateDock(dd.pos, dd.dir, dd.rot);
	dock_definitions.push_back(dd);
	return;
}
bool VesselBuilder1::DeleteDockDef(int idx) {
	if ((dock_definitions.size() - 1 < idx) || (idx<0)) {
		SBLog("WARNING: Called a Delete Dock Definition with out of range index");
		return false;
	}
	DelDock(dock_definitions[idx].dh);
	dock_definitions.erase(dock_definitions.begin() + idx);
	return true;
}
void VesselBuilder1::AddExTexDef() {
	EXTEX_DEF ex = EXTEX_DEF();
	extex_defs.push_back(ex);
	return;
}
void VesselBuilder1::AddExTexDef(string texname) {
	EXTEX_DEF ex = EXTEX_DEF();
	ex.TexName = texname;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%s", texname.c_str());
	ex.tex = oapiRegisterExhaustTexture(cbuf);
	ex.created = true;
	extex_defs.push_back(ex);
	return;
}
bool VesselBuilder1::StoreExTexDef(string texname,def_idx d_idx) {
	extex_defs[d_idx].TexName = texname;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%s", texname.c_str());
	extex_defs[d_idx].tex = oapiRegisterExhaustTexture(cbuf);
	if (extex_defs[d_idx].tex != NULL) {
		extex_defs[d_idx].created = true;
		return true;
	}
	else {
		return false;
	}
}
void VesselBuilder1::DelExTedDef(def_idx d_idx) {
	extex_defs.erase(extex_defs.begin() + d_idx);
	return;
}
SURFHANDLE VesselBuilder1::GetExTexSurf(def_idx d_idx) {
	return extex_defs[d_idx].tex;
}
string VesselBuilder1::GetExTexName(def_idx d_idx) {
	return extex_defs[d_idx].TexName;
}
UINT VesselBuilder1::GetExTexCount() {
	return extex_defs.size();
}
bool VesselBuilder1::IsExTexCreated(def_idx d_idx) {
	return extex_defs[d_idx].created;
}
int VesselBuilder1::GetExTexIdx(SURFHANDLE tex) {
	if(tex==NULL){return -1; }
	for (int i = 0; i < extex_defs.size(); i++) {
		if (extex_defs[i].tex == tex) {
			return i;
		}
	}
	return -1;
}



void VesselBuilder1::CreateDockExhausts() {
	EXHAUSTSPEC es;
	double level = 1;
	for (UINT i = 0; i < dock_definitions.size(); i++) {
		es.flags = EXHAUST_CONSTANTLEVEL;
		es.th = NULL;
		es.tex = greenL;
		es.lsize = 100;
		es.wsize = 0.002;
		es.modulate = 0;
		es.lofs = 0;
		es.level = &level;
		es.lpos = &dock_definitions[i].pos;
		es.ldir = &dock_definitions[i].antidir;
		UINT idx = AddExhaust(&es);
		DockExhaustsID.push_back(idx);
		es.tex = blueL;
		es.ldir = &dock_definitions[i].antirot;
		idx = AddExhaust(&es);
		DockExhaustsID.push_back(idx);
	}
	DockExhaustsActive = true;
	return;
}
void VesselBuilder1::DeleteDockExhausts() {
	for (UINT i = 0; i < DockExhaustsID.size(); i++) {
		DelExhaust(DockExhaustsID[i]);
	}
	DockExhaustsID.clear();
	DockExhaustsActive = false;
	return;
}
void VesselBuilder1::CreateAttExhausts() {
	EXHAUSTSPEC es;
	double level = 1;
	for (UINT i = 0; i < AttMng->GetAttCount(); i++) {
		es.flags = EXHAUST_CONSTANTLEVEL;
		es.th = NULL;
		es.tex = greenL;
		es.lsize = 100;
		es.wsize = 0.002;
		es.modulate = 0;
		es.lofs = 0;
		es.level = &level;
		es.lpos = &AttMng->att_defs[i].pos;
		es.ldir = &AttMng->att_defs[i].antidir;
		UINT idx = AddExhaust(&es);
		AttExhaustsID.push_back(idx);
		es.tex = blueL;
		es.ldir = &AttMng->att_defs[i].antirot;
		idx = AddExhaust(&es);
		AttExhaustsID.push_back(idx);
	}
	AttExhaustsActive = true;
	return;
}
void VesselBuilder1::DeleteAttExhausts() {
	for (UINT i = 0; i < AttExhaustsID.size(); i++) {
		DelExhaust(AttExhaustsID[i]);
	}
	AttExhaustsID.clear();
	AttExhaustsActive = false;
	return;
}


void VesselBuilder1::CreateThExhausts() {
	EXHAUSTSPEC es;
	double level = 1;
	for (UINT i = 0; i < ThrMng->GetThrCount(); i++) {
		es.flags = EXHAUST_CONSTANTLEVEL;
		es.th = ThrMng->GetThrTH(i);//NULL;
		es.tex = greenL;
		es.lsize = 100;
		es.wsize = 0.005;
		es.modulate = 0;
		es.lofs = 0;
		es.level = &level;
		es.lpos = NULL;//&ThrMng->thr_defs[i].pos;
		es.ldir = &ThrMng->thr_defs[i].antidir;//NULL;//&ThrMng->thr_defs[i].antidir;
		UINT idx = AddExhaust(&es);
		ThExaustsID.push_back(idx);	
	}
	thExhaustsActive = true;
}
void VesselBuilder1::DeleteThExhausts() {
	for (UINT i = 0; i < ThExaustsID.size(); i++) {
		DelExhaust(ThExaustsID[i]);
	}
	ThExaustsID.clear();
	thExhaustsActive = false;
	return;
}
void VesselBuilder1::CreateThrGrpLaserExhausts(THGROUP_TYPE thgt) {
	EXHAUSTSPEC es;
	double level = 1;
	es.flags = EXHAUST_CONSTANTLEVEL;
	es.tex = greenL;
	es.lsize = 100;
	es.wsize = 0.005;
	es.modulate = 0;
	es.lofs = 0;
	es.level = &level;
	es.lpos = NULL;//&ThrMng->thr_defs[i].pos;
	 //NULL;//&ThrMng->t
	vector<THRUSTER_HANDLE>thrusters = ThrGrpMng->GetThrusters(thgt);
	for (UINT i = 0; i < thrusters.size(); i++) {
		es.th = thrusters[i];
		def_idx th_idx = ThrMng->GetThrIdx(thrusters[i]);
		es.ldir = &ThrMng->thr_defs[th_idx].antidir;
		UINT idx = AddExhaust(&es);
		ThGrpExhaustsID.push_back(idx);
	}
	return;
}
void VesselBuilder1::CreateThrusterLaserExhaust(THRUSTER_HANDLE th) {
	EXHAUSTSPEC es;
	double level = 1;
	es.flags = EXHAUST_CONSTANTLEVEL;
	es.tex = greenL;
	es.lsize = 100;
	es.wsize = 0.005;
	es.modulate = 0;
	es.lofs = 0;
	es.level = &level;
	es.lpos = NULL;//&ThrMng->thr_defs[i].pos;
	def_idx th_idx = ThrMng->GetThrIdx(th);
	es.ldir = &ThrMng->thr_defs[th_idx].antidir;
	//es.ldir = NULL;//&ThrMng->t
	es.th = th;
	UINT idx = AddExhaust(&es);
	ThGrpExhaustsID.push_back(idx);
	
	return;
}
void VesselBuilder1::DeleteThrGrpLaserExhausts() {
	for (UINT i = 0; i < ThGrpExhaustsID.size(); i++) {
		DelExhaust(ThGrpExhaustsID[i]);
	}
	ThGrpExhaustsID.clear();
	return;
}

void VesselBuilder1::CreateTDPExhausts(bool Current, vector<TOUCHDOWNVTX> &tdvtx) {
	EXHAUSTSPEC es;
	double level = 1;
	VECTOR3 zplus = _V(0, 0, 1);
	VECTOR3 zminus = _V(0, 0, -1);
	VECTOR3 xplus = _V(1, 0, 0);
	VECTOR3 xminus = _V(-1, 0, 0);
	VECTOR3 yplus = _V(0, 1, 0);
	VECTOR3 yminus = _V(0, -1, 0);
	es.flags = EXHAUST_CONSTANTLEVEL|EXHAUST_CONSTANTPOS|EXHAUST_CONSTANTDIR;
	es.lsize = 1.5;
	es.wsize = 0.005;
	es.modulate = 0;
	es.lofs = 0;
	es.level = &level;
	es.th = NULL;
	if (Current) {
		es.tex = greenL;
		DWORD tdpcount = GetTouchdownPointCount();
		tdvtx.clear();
		for (DWORD i = 0; i < tdpcount; i++) {
			TOUCHDOWNVTX td;
			GetTouchdownPoint(td, i);
			tdvtx.push_back(td);
		}
	}
	else {
		es.tex = redL;
	}
	for (UINT i = 0; i < tdvtx.size(); i++) {
		es.lpos = &tdvtx[i].pos;
		es.ldir = &zplus;
		UINT idx = AddExhaust(&es);
		if (Current) {
			TDPCurExhaustsID.push_back(idx);
		}
		else {
			TDPSetExhaustsID.push_back(idx);
		}
		
		es.ldir = &zminus;
		 idx = AddExhaust(&es);
		 if (Current) {
			 TDPCurExhaustsID.push_back(idx);
		 }
		 else {
			 TDPSetExhaustsID.push_back(idx);
		 }
		es.ldir = &xplus;
		 idx = AddExhaust(&es);
		 if (Current) {
			 TDPCurExhaustsID.push_back(idx);
		 }
		 else {
			 TDPSetExhaustsID.push_back(idx);
		 }
		es.ldir = &xminus;
		 idx = AddExhaust(&es);
		 if (Current) {
			 TDPCurExhaustsID.push_back(idx);
		 }
		 else {
			 TDPSetExhaustsID.push_back(idx);
		 }
		es.ldir = &yplus;
		 idx = AddExhaust(&es);
		 if (Current) {
			 TDPCurExhaustsID.push_back(idx);
		 }
		 else {
			 TDPSetExhaustsID.push_back(idx);
		 }
		es.ldir = &yminus;
		 idx = AddExhaust(&es);
		 if (Current) {
			 TDPCurExhaustsID.push_back(idx);
		 }
		 else {
			 TDPSetExhaustsID.push_back(idx);
		 }





	}
	if (Current) {
		TdpCurExhaustsActive = true;
	}
	else {
		TdpSetExhaustsActive = true;
	}
	
	return;
}
void VesselBuilder1::DeleteTDPExhausts(bool Current) {
	vector<UINT> IDs;
	IDs.clear();
	if (Current) {
		IDs = TDPCurExhaustsID;
		TDPCurExhaustsID.clear();
		TdpCurExhaustsActive = false;
	}
	else {
		IDs = TDPSetExhaustsID;
		TDPSetExhaustsID.clear();
		TdpSetExhaustsActive = false;
	}
	for (UINT i = 0; i < IDs.size(); i++) {
		DelExhaust(IDs[i]);
	}
	
	return;
}






bool VesselBuilder1::UsingD3D9() {
//	oapiWriteLogV("SB1::UsingD3D9:%i", wD3D9);
	return wD3D9;
}
MATRIX3 VesselBuilder1::Inverse(MATRIX3 m) {
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
bool VesselBuilder1::AreVector3Equal(VECTOR3 v1, VECTOR3 v2) {
	if ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z)) {
		return true;
	}
	else {
		return false;
	}

}
string VesselBuilder1::WriteVectorUINT(vector<UINT> v) {
	string line;
	line.clear();
	if (v.size() > 0) {
		for (UINT i = 0; i < v.size(); i++) {
			char add[128] = { '\0' };
			if (i == v.size() - 1) {
				sprintf(add, "%i", v[i]);
			}
			else {
				sprintf(add, "%i, ", v[i]);
			}
			string add_s(add);
			line += add_s;
		}
	}
	else {
		char add[128] = { '\0' };
		sprintf(add, "%i", -1);
		line.assign(add);
	}
	
	return line;
}
vector<UINT> VesselBuilder1::readVectorUINT(string s) {
	vector<UINT>r;
	r.clear();
	if (s.size() <= 0) { return r; }
	string delimiter = ",";
	size_t pos0 = 0;
	size_t pos1 = 0;
	while (pos1 != string::npos) {
		pos1 = s.find(delimiter, pos0);
		string token = s.substr(pos0, pos1 - pos0);
		r.push_back(stoi(token));
		pos0 = pos1 + delimiter.length();
	}
	return r;
}

/*if (ngrps > 0) {
			char grpsbuf[512] = { '\0' };
			sprintf(cbuf, "ANIMCOMP_%i_GRPS", animcomp_counter);
			oapiReadItem_string(fh, cbuf, grpsbuf);
			string grpsbuf_s(grpsbuf);
			
			string delimiter = ",";
			size_t pos0 = 0;
			size_t pos1 = 0;
			while (pos1 != string::npos) {
				pos1 = grpsbuf_s.find(delimiter, pos0);
				string token = grpsbuf_s.substr(pos0, pos1 - pos0);
				grps.push_back(stoi(token));
				pos0 = pos1 + delimiter.length();
			}
		}*/

vector <string> VesselBuilder1::ReadMeshTextures(string filename) {
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
	return;
}
DLLCLBK void ExitModule(HINSTANCE hModule) {
	oapiUnregisterCustomControls(hModule);
	return;
}
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){return new VesselBuilder1(hvessel,flightmodel);}
DLLCLBK void ovcExit(VESSEL *vessel){if(vessel)delete(VesselBuilder1*)vessel;}


