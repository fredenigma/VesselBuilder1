#define ORBITER_MODULE


#include <chrono>
#include <ctime> 
#include <math.h>
#include <stdio.h>
#include "VesselBuilder1.h"
#include "Log.h"
#include "LaserManager.h"
#include "DialogControl.h"
#include "FollowMeDlg.h"
#include "TextReaderDlg.h"
#include "resource.h"
#include "DlgCtrl.h"
#include "gcAPI.h"
#include "GeneralSettingsManager.h"
#include "MeshManager.h"
#include "DockManager.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"
#include "PropellantManager.h"
#include "ThrusterManager.h"
#include "ParticleManager.h"
#include "TouchdownPointsManager.h"
#include "AirfoilsManager.h"
#include "ControlSurfacesManager.h"
#include "CameraManager.h"
#include "ExTexManager.h"
#include "VCManager.h"
#include "LightsManager.h"
#include "VariableDragManager.h"
#include "MET.h"
#include "EventManager.h"
#include "ConfigurationManager.h"

#define LogV(x,...) Log->Log(x,##__VA_ARGS__)

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

HINSTANCE hDLL;
bool IamFirst = true;
//Creation
VesselBuilder1::VesselBuilder1(OBJHANDLE hObj,int fmodel):VESSEL4(hObj,fmodel){
	gcInitialize();
	Log = new Logger(this);
	if (IamFirst) {
		IamFirst = false;
		Log->InitLog();
		//oapiWriteLogV("First : %s", GetName());
	}
	
	
	OrbiterRoot.clear();
	char root[MAX_PATH] = { '\0' };
	GetCurrentDirectory(MAX_PATH, root);
	OrbiterRoot.assign(root);
	wD3D9 = false;
	GetOrbiterDirs();
	
	SetMng = new GeneralSettingsManager(this);
	MshMng = new MeshManager(this);
	DckMng = new DockManager(this);
	AttMng = new AttachmentManager(this);
	AnimMng = new AnimationManager(this);
	PrpMng = new PropellantManager(this);
	ThrMng = new ThrusterManager(this);
	ThrGrpMng = new ThrusterGroupManager(this);
	PartMng = new ParticleManager(this);
	TdpMng = new TouchdownPointsManager(this);
	AirfoilMng = new AirfoilsManager(this);
	CtrSurfMng = new ControlSurfacesManager(this);
	CamMng = new CameraManager(this);
	ExTMng = new ExTexManager(this);
	VCMng = new VCManager(this);
	LightsMng = new LightsManager(this);
	Laser = new LaserManager(this);
	VardMng = new VariableDragManager(this);
	EvMng = new EventManager(this);
	Met = new MET();
	ConfigMng = new ConfigurationManager(this);

	cfgfilename.clear();
	follow_me_pos = _V(0, 0, 0);
	follow_me_dir = _V(0, 0, 1);
	follow_me_rot = _V(0, 1, 0);
	follow_me_rm = _M(1, 0, 0, 0, 1, 0, 0, 0, 1);
	follow_me = false;
	h_follow_me = NULL;
	follow_me_rotation_speed = 60*RAD;
	follow_me_translation_speed = 6;
	FollowMeSuperPrecision = false;
//	ResetSBLog();
	follow_me_noteh = oapiCreateAnnotation(false, 1, _V(1, 1, 1));
	Dlg = new DialogControl(this);
	FMDlg = new FollowMeDlg(this);
	TextDlg = new TextReaderDlg();

	vclip = V_CLIPBOARD();
	
	
	GrabMode = false;
	currentGrabAtt = 0;
	NoEditor = false;
	DlgOpened = false;
	level1 = 1.0;
	redL = oapiRegisterExhaustTexture("red_L");
	greenL = oapiRegisterExhaustTexture("green_L");
	blueL = oapiRegisterExhaustTexture("blue_L");
	LogV("Class Initialized");
//	SBLog("Class Initialize");
	return;
}

VesselBuilder1::~VesselBuilder1(){
	LogV("Destructor Started");
	delete Dlg;
	Dlg = NULL;
	delete FMDlg;
	FMDlg = NULL;
	delete TextDlg;
	TextDlg = NULL;
	delete SetMng;
	SetMng = NULL;
	delete MshMng;
	MshMng = NULL;
	delete DckMng;
	DckMng = NULL;
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
	delete CamMng;
	CamMng = NULL;
	delete ExTMng;
	ExTMng = NULL;
	delete VCMng;
	VCMng = NULL;
	delete LightsMng;
	LightsMng = NULL;
	delete Laser;
	Laser = NULL;
	delete VardMng;
	VardMng = NULL;
	delete EvMng;
	EvMng = NULL;
	delete Met;
	Met = NULL;
	delete ConfigMng;
	ConfigMng = NULL;
	LogV("Destructor Completed");
	Log->CloseLog();
	delete Log;
	Log = NULL;
//	CloseSBLog();
	
	//ClearDelete(mgr);
	return;
}


void VesselBuilder1::GetOrbiterDirs() {
	string cfgname("Orbiter.cfg");
	if (gcEnabled()) { 
		cfgname.assign("Orbiter_NG.cfg"); 
		wD3D9 = true;
		LogV("D3D9 Active");
	}
	else {
		LogV("Using Inline Graphic Client");
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
	
	
	char absconfigdir_c[MAX_PATH];
	char currentdir1[MAX_PATH];
	GetCurrentDirectory(sizeof(currentdir1) / sizeof(char), currentdir1);
	SetCurrentDirectory(configdir.c_str());
	GetCurrentDirectory(sizeof(absconfigdir_c) / sizeof(char), absconfigdir_c);
	SetCurrentDirectory(currentdir1);
	configdir.assign(absconfigdir_c);
	char absmeshdir_c[MAX_PATH];
	GetCurrentDirectory(sizeof(currentdir1) / sizeof(char), currentdir1);
	SetCurrentDirectory(meshdir.c_str());
	GetCurrentDirectory(sizeof(absmeshdir_c) / sizeof(char), absmeshdir_c);
	SetCurrentDirectory(currentdir1);
	meshdir.assign(absmeshdir_c);
	char abstexdir_c[MAX_PATH];
	GetCurrentDirectory(sizeof(currentdir1) / sizeof(char), currentdir1);
	SetCurrentDirectory(texturedir.c_str());
	GetCurrentDirectory(sizeof(abstexdir_c) / sizeof(char), abstexdir_c);
	SetCurrentDirectory(currentdir1);
	texturedir.assign(abstexdir_c);
	char abstex2dir_c[MAX_PATH];
	GetCurrentDirectory(sizeof(currentdir1) / sizeof(char), currentdir1);
	SetCurrentDirectory(texture2dir.c_str());
	GetCurrentDirectory(sizeof(abstex2dir_c) / sizeof(char), abstex2dir_c);
	SetCurrentDirectory(currentdir1);
	texture2dir.assign(abstex2dir_c);
	char absscndir_c[MAX_PATH];
	GetCurrentDirectory(sizeof(currentdir1) / sizeof(char), currentdir1);
	SetCurrentDirectory(scenariodir.c_str());
	GetCurrentDirectory(sizeof(absscndir_c) / sizeof(char), absscndir_c);
	SetCurrentDirectory(currentdir1);
	scenariodir.assign(absscndir_c);
	
	LogV("Orbiter dirs: %s %s %s %s %s", configdir.c_str(), meshdir.c_str(), texturedir.c_str(), texture2dir.c_str(), scenariodir.c_str());
	
	//SBLog("cfg file:%s", cfgname.c_str());
//	SBLog("Orbiter Folders: %s %s %s %s %s", meshdir.c_str(), configdir.c_str(), texturedir.c_str(), texture2dir.c_str(), scenariodir.c_str());
	oapiCloseFile(orbiter_cfg, FILE_IN_ZEROONFAIL);
	return;
}


void VesselBuilder1::clbkSetClassCaps(FILEHANDLE cfg){
	LogV("Set Class Caps Started");
	SetEmptyMass(1000);
	SetSize(10);
	
	ExTMng->ParseCfgFile(cfg);
	PartMng->ParseCfgFile(cfg);
	
	ConfigMng->ParseCfgFile(cfg);
	
	if (!oapiReadItem_bool(cfg, "NOEDITOR", NoEditor)) { NoEditor = false; }
	if (NoEditor) {
		LogV("No Editor option active");
	}
	
	SetNosewheelSteering(true);
	SetMaxWheelbrakeForce(2e5);
	LogV("Set Class Caps Completed");
	return;
}
void VesselBuilder1::clbkLoadStateEx(FILEHANDLE scn,void *vs)
{
	LogV("Load State Started");
//	SBLog("Loading State...");
	char *line;
	while (oapiReadScenario_nextline(scn, line)) {
		if (!_strnicmp(line, "ANIM_", 5)) {
			UINT seq = 0;
			sscanf(line + 5, "%i", &seq);
			if (seq >= AnimMng->GetAnimDefsCount()) { continue; }
			UINT c = 1;
			if ((seq > 9)&&(seq<100)) { c = 2; }
			else if ((seq > 99) && (seq < 1000)) { c = 3; }
			else if ((seq > 999) && (seq < 10000)) { c = 4; }
			double state;
			int status;
			sscanf(line + 5 + 1 + c, "%lf %d", &state, &status);
			AnimMng->SetAnimationState(seq, state);
			if (status == 1) {
				AnimMng->StartAnimation(seq);
				AnimMng->SetAnimationBackward(seq, false);
			}
			else if (status == -1) {
				AnimMng->StartAnimation(seq);
				AnimMng->SetAnimationBackward(seq, true);
			}
		}
		else if (!_strnicmp(line, "DOCKS_JETTISONED", 16)) {
			char docks_gone[256] = { '\0' };
			sscanf(line + 16, "%s", docks_gone);
			string dcg(docks_gone);
			DckMng->docks_jettisoned = readVectorUINT(dcg);
			for (UINT i = 0; i < DckMng->docks_jettisoned.size(); i++) {
				DckMng->docks_to_del.push_back(DckMng->GetDH(DckMng->docks_jettisoned[i]));
			}
		}
		else if (!_strnicmp(line, "CURRENT_CAM",11)) {
			UINT cam = 0;
			sscanf(line + 11, "%i", &cam);
			CamMng->SetCurrentCamera(cam);
		}
		else if (!_strnicmp(line, "BEACONS", 7)) {
			char bcn_ons[256] = { '\0' };
			sscanf(line + 7, "%s", bcn_ons);
			string bnc(bcn_ons);
			vector<UINT> bc = readVectorUINT(bnc);
			for (UINT i = 0; i < bc.size(); i++) {
				LightsMng->ActivateBeacon(bc[i], true);
			}
			/*int active = 0;
			sscanf(line + 7, "%i", &active);
			if (active > 0) {
				LightsMng->ActivateAllBeacons(true);
			}
			else {
				LightsMng->ActivateAllBeacons(false);
			}*/
		}
		else if (!_strnicmp(line, "LIGHTS", 6)) {
			char lgt_on[256] = { '\0' };
			sscanf(line + 6, "%s", lgt_on);
			string lnc(lgt_on);
			vector<UINT>lc = readVectorUINT(lnc);
			for (UINT i = 0; i < lc.size(); i++) {
				LightsMng->ActivateLight(lc[i], true);
			}
		}
		else{ 
			ParseScenarioLineEx(line, vs); 
		}	
	}

	LogV("Load State Completed");
//	SBLog("Loading State Finished");
}
void VesselBuilder1::clbkSaveState(FILEHANDLE scn)
{
	LogV("Save State Started");
	//SBLog("Saving State...");

	
	
	SaveDefaultState(scn);

	for (UINT i = 0; i < AnimMng->GetAnimDefsCount(); i++) {
		if (!AnimMng->IsAnimValid(i)) { continue; }
		if (AnimMng->GetAnimCycle(i) == AnimCycleType::AUTOMATIC) { continue; }
		char buff[256] = { '\0' };
		char buff2[256] = { '\0' };
		sprintf(buff, "ANIM_%i",i);
		sprintf(buff2,"%.5f %i", AnimMng->GetAnimationState(i), AnimMng->AnimationRunStatus(i));
		oapiWriteScenario_string(scn, buff, buff2);
		//oapiWriteScenario_float(scn, buff, AnimMng->GetAnimationState(i));
	}
	if (CamMng->GetCamCount() > 0) {
		char buf[256] = { '\0' };
		sprintf(buf, "CURRENT_CAM");
		oapiWriteScenario_int(scn, buf, CamMng->GetCurrentCamera());
	}
	if (DckMng->docks_jettisoned.size() > 0) {
		string docks_gone = WriteVectorUINT(DckMng->docks_jettisoned, false);
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "DOCKS_JETTISONED");
		char d_buff[256] = { '\0' };
		sprintf(d_buff, "%s", docks_gone.c_str());
		oapiWriteScenario_string(scn, cbuf, d_buff);
	}
	if (LightsMng->GetBeaconCount() > 0) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "BEACONS");
		vector<UINT> v_bon = LightsMng->GetBeaconsOn();
		if (v_bon.size() > 0) {
			string bcn_on = WriteVectorUINT(v_bon, false);
			char lbuf[256] = { '\0' };
			sprintf(lbuf, "%s", bcn_on.c_str());
			oapiWriteScenario_string(scn, cbuf, lbuf);
		}
		
		//oapiWriteScenario_int(scn, cbuf,(int)LightsMng->AreAllBeaconsActive());
	}
	if (LightsMng->GetLightsCount() > 0) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "LIGHTS");
		vector<UINT> v_lon = LightsMng->GetLightsOn();
		if (v_lon.size() > 0) {
			string lights_on = WriteVectorUINT(v_lon, false);
			char lbuf[256] = { '\0' };
			sprintf(lbuf, "%s", lights_on.c_str());
			oapiWriteScenario_string(scn, cbuf, lbuf);
		}
	}

	if (follow_me) {
		DeleteFollowMe();
	}
	if (DlgOpened) {
		string fn("\\Vessels\\");
		string cn(GetClassName());
		fn += cn;
		fn += ".cfg";
		WriteCfgFile(fn);
	}
	
	LogV("Save State Completed");
//	SBLog("Save State Finished");
}
void VesselBuilder1::clbkPostCreation() {
	
}

bool VesselBuilder1::ToggleGrapple() {
	def_idx AttIdx = AttMng->IdxAtt2Def(currentGrabAtt);
	
	
	ATTACHMENTHANDLE ah = GetAttachmentHandle(false, currentGrabAtt);
	OBJHANDLE h_attached = GetAttachmentStatus(ah);
	if (h_attached == NULL) {
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
				bool id_check = AttMng->GetIdCheck(AttIdx);
				if (id_check) {
					string id_check_string = AttMng->GetIdCheckString(AttIdx);
					const char* otherId = v->GetAttachmentId(hAtt);
					string otherId_s(otherId);
					UINT length = id_check_string.length();
					if (id_check_string.compare(0, length, otherId_s) != 0) {
						continue;
					}
				}
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
		DetachChild(ah, 0.05);
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
	double translation_speed = FollowMeSuperPrecision ? 0.06 : 6;
	double rotation_speed = FollowMeSuperPrecision ? 0.6*RAD : 60 * RAD;
	if (KEYDOWN(kstate, OAPI_KEY_UP)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = rotation_speed*oapiGetSimStep();
			VECTOR3 axis = _V(1, 0, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_UP);
		}
		else if (KEYMOD_ALT(kstate)) {
			follow_me_pos.y += translation_speed*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_UP);
		}
		else {
			follow_me_pos.z += translation_speed*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_UP);
		}
	}
	if (KEYDOWN(kstate, OAPI_KEY_RIGHT)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = rotation_speed*oapiGetSimStep();
			VECTOR3 axis = _V(0, 1, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_RIGHT);
		}
		else {
			follow_me_pos.x += translation_speed*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_RIGHT);
		}

	}
	if (KEYDOWN(kstate, OAPI_KEY_LEFT)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = -rotation_speed*oapiGetSimStep();
			VECTOR3 axis = _V(0, 1, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_LEFT);
		}
		else {
			follow_me_pos.x -= translation_speed*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_LEFT);
		}
	}
	if (KEYDOWN(kstate, OAPI_KEY_DOWN)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = -rotation_speed*oapiGetSimStep();
			VECTOR3 axis = _V(1, 0, 0);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_DOWN);
		}
		else if (KEYMOD_ALT(kstate)) {
			follow_me_pos.y -= translation_speed*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_DOWN);
		}
		else {
			follow_me_pos.z -= translation_speed*oapiGetSimStep();
			RESETKEY(kstate, OAPI_KEY_DOWN);
		}
	}
	if (KEYDOWN(kstate, OAPI_KEY_NEXT)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = -rotation_speed*oapiGetSimStep();
			VECTOR3 axis = _V(0, 0, 1);
			MATRIX3 rot_m = rotm(axis, angle);
			MATRIX3 early_rm = follow_me_rm;
			follow_me_rm = mul(early_rm, rot_m);
			RESETKEY(kstate, OAPI_KEY_NEXT);
		}

	}
	if (KEYDOWN(kstate, OAPI_KEY_PRIOR)) {
		if (KEYMOD_SHIFT(kstate)) {
			double angle = rotation_speed*oapiGetSimStep();
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
	//if (!Dlg->IsOpen()) {
		AnimMng->ConsumeAnimDirectKey(kstate);
	//}
	
	return 0;
}


int VesselBuilder1::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if (!down) return 0; 
	if (Playback()) return 0; //
	//if (!AnimEditingMode) {
	//if (!Dlg->IsOpen()) {
	AnimMng->ConsumeAnimBufferedKey(key, down, kstate);
	//}
	CamMng->ConsumeCameraBufferedKey(key, down, kstate);
	LightsMng->ConsumeLightsBufferedKey(key, down, kstate);
	EvMng->ConsumeBufferedKey(key, down, kstate);
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
	if (KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_B) {
		LightsMng->ToggleAllBeaconsActive();
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_J) {
		if (JettisonNextDock()) { return 1; }
		else { return 0; }
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_K) {
	
	//	ConfigMng->ApplyConfiguration(1);
		//Met->SetMJD0(oapiGetSimMJD() + 0.01);
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
	/*	LightEmitter *LE = new LightEmitter();
		COLOUR4 col_d, col_s, col_a;
		col_d = LE->GetDiffuseColour();
		col_s = LE->GetSpecularColour();
		col_a = LE->GetAmbientColour();
		LightEmitter::VISIBILITY vis = LE->GetVisibility();
		VECTOR3 pos = LE->GetPosition();
		VECTOR3 dir = LE->GetDirection();
		LightEmitter::TYPE type = LE->GetType();

		oapiWriteLogV("LIGHT EMITTER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

		oapiWriteLogV("Diffuse : %.3f %.3f %.3f %.3f", col_d.r, col_d.g, col_d.b, col_d.a);
		oapiWriteLogV("Specular : %.3f %.3f %.3f %.3f", col_s.r, col_s.g, col_s.b, col_s.a);
		oapiWriteLogV("Ambient : %.3f %.3f %.3f %.3f", col_a.r, col_a.g, col_a.b, col_a.a);
		oapiWriteLogV("Visibility:%i", (int)vis);
		oapiWriteLogV("Position: %.3f %.3f %.3f", pos.x, pos.y, pos.z);
		oapiWriteLogV("Direction: %.3f %.3f %.3f", dir.x, dir.y, dir.z);
		oapiWriteLogV("Type: %i", (int)type);
		oapiWriteLogV("END LIGHT EMITTER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");*/
		return 1;
	}
	if (!KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_CONTROL(kstate) && key == OAPI_KEY_K) {
		/*Event::TRIGGER Trig = Event::TRIGGER();
		Trig.Type = Event::TRIGGER::TIME;
		Trig.time_mode = Event::TRIGGER::TIME_MODE::MET;
		Trig.TriggerValue = 10;*/
		//Trig.Type = Event::TRIGGER::TRIGGERTYPE::KEYPRESS;
		//Trig.Key = OAPI_KEY_M;
		//Trig.KeyMods.Alt = true;
		//Trig.repeat_mode = Event::TRIGGER::REPEAT_MODE::ALWAYS;
		//EvMng->CreateChildSpawnEvent("TestSpawn", Trig, "SLS\\core", "stage");
		//EvMng->CreateAnimTriggerEvent("TESTANIM", Trig, 5, true);
		//THRUSTER_HANDLE th = ThrMng->GetThrTH(3);
		//EvMng->CreateThrusterFireEvent("TEST", Trig, th);
	/*	string fn("\\Vessels\\VesselBuilder1\\Cupola_VB.cfg");
		bitset<N_SECTIONS>Sects;
		for (UINT i = 0; i < N_SECTIONS; i++) {
			Sects.set(i);
		}
		EvMng->CreateReconfigurationEvent("test_reconfig", Trig, Sects, fn);*/
		/*EvMng->CreateThrusterGroupLevelEvent("TEST", Trig, THGROUP_MAIN);
		Trig.TriggerValue = 15;
		EvMng->CreateThrusterGroupLevelEvent("TEST", Trig, THGROUP_MAIN,0);*/
		//EvMng->CreateThrusterFireEvent("TEST", Trig, th,0);
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
	Met->PreStep(mjd);
	if (GrabMode) {
		def_idx attidx = AttMng->IdxAtt2Def(currentGrabAtt);
		sprintf(oapiDebugString(), "Attachment ready for Grab: %i %s", currentGrabAtt, AttMng->GetAttDefId(attidx).c_str());
	}
	AnimMng->AnimationPreStep(simt, simdt, mjd);
	TdpMng->TouchDownPointsPreStep(simt, simdt, mjd);
	if (follow_me) {
		UpdateFollowMe();
	}
	DckMng->DockPreStep(simt, simdt, mjd);
	EvMng->PreStep(simt, simdt, mjd);
	/*
	int sign, hrs, mins, secs;
	Met->GetHMS(sign, hrs, mins, secs);

	sprintf(oapiDebugString(), "%i %02d:%02d:%02d %.8f %i", sign, hrs, mins, secs,Met->d_secs,Met->MyRound(Met->d_secs));*/
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
	
	sprintf(oapiDebugString(), "Current Configuration:%i", ConfigMng->GetCurrentConfiguration());
	
	
	return;
}
void VesselBuilder1::clbkVisualCreated(VISHANDLE vis, int refcount) {
	LogV("VisualCreated");
	visual = vis;
	MshMng->VisualCreated(vis, refcount);

	return;
}
void VesselBuilder1::clbkVisualDestroyed(VISHANDLE vis, int refcount) {
	LogV("Visual Destroyed");
	visual = NULL;
	MshMng->VisualDestroyed(vis, refcount);
	LogV("Visual Destroyed Finished");
	return;
}
bool VesselBuilder1::clbkLoadVC(int id) {
	LogV("LoadVC");
	VCMng->LoadVC(id);
	return true;
}
bool VesselBuilder1::clbkVCMouseEvent(int id, int event, VECTOR3 &p) {
	LogV("VCMouseEvent");
	return VCMng->VCMouseEvent(id, event, p);	
}
void VesselBuilder1::clbkDockEvent(int dock, OBJHANDLE mate) {
	LogV("Dock Event");
	DckMng->DockEvent(dock, mate);
	LogV("Dock Event Finished");
	return;
}

bool VesselBuilder1::JettisonNextDock() {
	for (UINT i = 0; i < DckMng->GetDockCount(); i++) {
		if (DckMng->IsDockJettisonable(i) && (DckMng->GetDH(i) != NULL)) {
			if (GetDockStatus(DckMng->GetDH(i)) != NULL) {
				UINT orb_dock_idx = DckMng->GetOrbiterDockIdx(DckMng->GetDH(i));
				if (orb_dock_idx != (UINT)-1) {
					Undock(orb_dock_idx);
					return true;
				}

			}
		}
	}
	return false;
}
bool VesselBuilder1::JettisonDock(UINT idx) {
	if (!DckMng->IsDockJettisonable(idx)) { return false; }
	if (DckMng->GetDH(idx) == NULL) { return false; }
	if (GetDockStatus(DckMng->GetDH(idx)) == NULL) { return false; }
	UINT orb_dock_idx = DckMng->GetOrbiterDockIdx(DckMng->GetDH(idx));
	if (orb_dock_idx == (UINT)-1) { return false; }
	Undock(orb_dock_idx);
	return true;
}
void VesselBuilder1::VehicleSetup() {
	//SBLog("Vehicle Setup Started...");
//	ResetVehicle();
	//MshMng->LoadMeshes();
	//SBLog("Vehicle Setup Finished...");
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
	//SBLog("Resetting Vehicle...");
	LogV("Resetting entirely the Vehicle");
	VardMng->Clear();
	VCMng->Clear();
	CamMng->Clear();
	LightsMng->Clear();
	CtrSurfMng->Clear();
	AirfoilMng->Clear();
	TdpMng->Clear();
	ThrMng->Clear();
	ThrGrpMng->ResetDefine();
	PartMng->Clear();
	ExTMng->Clear();
	PrpMng->Clear();
	AnimMng->Clear();
	AttMng->Clear();
	DckMng->Clear();
	MshMng->Clear();

	//SETTINGS????

	/*ClearMeshes();
	ClearAttachments();
	ClearDockDefinitions();
	ClearLightEmitters();
	ClearBeacons();
	ClearAirfoilDefinitions();
	ClearControlSurfaceDefinitions();
	ClearThrusterDefinitions();
	ClearLightEmitters();
	ClearPropellantResources();
	ClearVariableDragElements();*/
	LogV("Reset Complete");
	//SBLog("Reset Complete");
}

void VesselBuilder1::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing of Cfg File Started");
//	if (fh == NULL) { SBLog("WARNING: cfg file handle NULL!"); }
	char cbuf[256] = { '\0' };

	
//	MshMng->ParseCfgFile(fh);
//	MshMng->PreLoadMeshes();
//	MshMng->AddMeshes();
//	DckMng->ParseCfgFile(fh);
//	AttMng->ParseCfgFile(fh);
//	AnimMng->ParseCfgFile(fh);
//	PrpMng->ParseCfgFile(fh);
//	ExTMng->ParseCfgFile(fh);
//	PartMng->ParseCfgFile(fh);
//	ThrMng->ParseCfgFile(fh);
//	ThrGrpMng->ParseCfgFile(fh);
//	TdpMng->ParseCfgFile(fh);
//	AirfoilMng->ParseCfgFile(fh);
//	CtrSurfMng->ParseCfgFile(fh);
//	CamMng->ParseCfgFile(fh);
//	VCMng->ParseCfgFile(fh);
//	LightsMng->ParseCfgFile(fh);
//	VardMng->ParseCfgFile(fh);


	LogV("Parsing of Cfg File Completed");
	//SBLog("Parsing Completed");
	return;
}
void VesselBuilder1::WriteCfgFile(string filename) {
	LogV("Writing CFG file...");
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
	/*oapiWriteItem_float(fh, "Mass", GetEmptyMass());
	oapiWriteItem_float(fh, "Size", GetSize());
	VECTOR3 pmi;
	GetPMI(pmi);
	oapiWriteItem_vec(fh, "Inertia", pmi);
	VECTOR3 cs;
	GetCrossSections(cs);
	oapiWriteItem_vec(fh, "CrossSections", cs);
	oapiWriteItem_float(fh, "GravityGradientDamping", GetGravityGradientDamping());
	VECTOR3 rd;
	GetRotDrag(rd);
	oapiWriteItem_vec(fh, "RotResistance", rd);
	oapiWriteLine(fh, " ");
	*/
//	MshMng->WriteCfg(fh);
	ConfigMng->UpdateConfiguration(ConfigMng->GetCurrentConfiguration());
	ConfigMng->WriteCfg(fh);
	//ExTMng->WriteCfg(fh);
	//PartMng->WriteCfg(fh);
	//DckMng->WriteCfg(fh);
	//AttMng->WriteCfg(fh);
	//AnimMng->WriteCfg(fh);
	//PrpMng->WriteCfg(fh);
	
	//ThrMng->WriteCfg(fh);
//	ThrGrpMng->WriteCfg(fh);
//	TdpMng->WriteCfg(fh);
//	AirfoilMng->WriteCfg(fh);
//	CtrSurfMng->WriteCfg(fh);
//	CamMng->WriteCfg(fh);
//	VCMng->WriteCfg(fh);
//	LightsMng->WriteCfg(fh);
//	VardMng->WriteCfg(fh);

	oapiCloseFile(fh, FILE_OUT);
	LogV("Writing CFG file Completed!");
	return;
}
void VesselBuilder1::WriteBackupFile() {
	LogV("Writing Backup...");
	string fn = configdir;
	fn += "\\Vessels\\";
	fn += GetClassName();
	
	string fnbackup = fn;
	fnbackup += "__BACKUPS.txt";
	fn += ".cfg";
	ifstream cfgfile;
	ofstream cfgbackup;
	cfgfile.open(fn.c_str());
	cfgbackup.open(fnbackup.c_str(),fstream::app);
	string line;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "==========================================================================================================");
	cfgbackup << cbuf << "\n";

	auto time = std::chrono::system_clock::now();
	std::time_t bup_time = std::chrono::system_clock::to_time_t(time);
	sprintf(cbuf, "BACKUP DATE:");// , oapiGetSysMJD());
	cfgbackup << cbuf << std::ctime(&bup_time) <<"\n";
	sprintf(cbuf, "BEGIN:");
	cfgbackup << cbuf << "\n";
	while (getline(cfgfile, line)) {
		cfgbackup << line<<"\n";
	}
	sprintf(cbuf, "END");
	cfgbackup << cbuf << "\n";
	sprintf(cbuf, "==========================================================================================================");
	cfgbackup << cbuf << "\n";
	cfgfile.close();
	cfgbackup.close();
	LogV("Writing Backup completed!");
	return;
}

void VesselBuilder1::CreateFollowMe(VECTOR3 initpos) {
	follow_me_pos = initpos;
	return CreateFollowMe();
}
void VesselBuilder1::CreateFollowMe() {
	LogV("Creating Follow Me Tool");
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
	LogV("Deleting Follow Me Tool");
	oapiAnnotationSetText(follow_me_noteh, "");
	if (oapiIsVessel(h_follow_me)) {
		oapiDeleteVessel(h_follow_me, GetHandle());
		h_follow_me = NULL;
		follow_me = false;
		FollowMeSuperPrecision = false;
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
void VesselBuilder1::ToggleFollowMeSuprePrecision() {
	if (oapiIsVessel(h_follow_me)) {
		VESSEL3 *v = (VESSEL3*)oapiGetVesselInterface(h_follow_me);
		v->clbkGeneric(VMSG_USER, 0);
	}
	if (FollowMeSuperPrecision) {
		FollowMeSuperPrecision = false;
	}
	else {
		FollowMeSuperPrecision = true;
	}

	return;
}
void VesselBuilder1::AddDefaultRCS() {
	LogV("Adding Default RCS System");
	double target_ang_acc = 3*RAD;
	double target_lin_acc = 0.15;
	VECTOR3 PMI;
	GetPMI(PMI);
	double mass = GetMass();
	double arm = 10;
	double massive_isp = 1e7;
	VECTOR3 force_needed;
	VECTOR3 lin_force_needed;
	force_needed.x = (PMI.x*mass*target_ang_acc) / arm;
	force_needed.y = (PMI.y*mass*target_ang_acc) / arm;
	force_needed.z = (PMI.z*mass*target_ang_acc) / arm;
	
	lin_force_needed.x = mass*target_lin_acc;
	lin_force_needed.y = mass*target_lin_acc;
	lin_force_needed.z = mass*target_lin_acc;

	LogV("Vessel Mass: %.1f Target Ang Acc:3 deg/s2 Target Linear Acc:0.15 m/s2 Torque Needed: %.3f %.3f %.3f Force Needed:%.3f", mass, force_needed.x, force_needed.y, force_needed.z, lin_force_needed.x);

	vector<UINT>th_idx;
	vector<THRUSTER_HANDLE>th_h;


	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_PITCHUP)) {
		UINT idx_pup1 = ThrMng->AddThrDef("Pitch_up_1", _V(0, 0, arm), _V(0, 1, 0), force_needed.x*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		UINT idx_pup2 = ThrMng->AddThrDef("Pitch_up_2", _V(0, 0, -arm), _V(0, -1, 0), force_needed.x*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_pup1);
		th_idx.push_back(idx_pup2);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_PITCHUP, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_PITCHDOWN)) {
		UINT idx_pdwn1 = ThrMng->AddThrDef("Pitch_dwn_1", _V(0, 0, arm), _V(0, -1, 0), force_needed.x*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		UINT idx_pdwn2 = ThrMng->AddThrDef("Pitch_dwn_2", _V(0, 0, -arm), _V(0, 1, 0), force_needed.x*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_pdwn1);
		th_idx.push_back(idx_pdwn2);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_PITCHDOWN, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_YAWLEFT)) {
		UINT idx_yawleft1 = ThrMng->AddThrDef("Yaw_left_1", _V(0, 0, arm), _V(-1, 0, 0), force_needed.y*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		UINT idx_yawleft2 = ThrMng->AddThrDef("Yaw_left_2", _V(0, 0, -arm), _V(1, 0, 0), force_needed.y*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_yawleft1);
		th_idx.push_back(idx_yawleft2);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_YAWLEFT, th_h);

	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_YAWRIGHT)) {
		UINT idx_yawright1 = ThrMng->AddThrDef("Yaw_right_1", _V(0, 0, arm), _V(1, 0, 0), force_needed.y*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		UINT idx_yawright2 = ThrMng->AddThrDef("Yaw_right_2", _V(0, 0, -arm), _V(-1, 0, 0), force_needed.y*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_yawright1);
		th_idx.push_back(idx_yawright2);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_YAWRIGHT, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_BANKLEFT)) {
		UINT idx_bankleft1 = ThrMng->AddThrDef("Bank_left_1", _V(arm, 0, 0), _V(0, 1, 0), force_needed.z*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		UINT idx_bankleft2 = ThrMng->AddThrDef("Bank_left_2", _V(arm, 0, 0), _V(0, -1, 0), force_needed.z*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_bankleft1);
		th_idx.push_back(idx_bankleft1);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_BANKLEFT, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_BANKRIGHT)) {
		UINT idx_bankright1 = ThrMng->AddThrDef("Bank_right_1", _V(arm, 0, 0), _V(0, -1, 0), force_needed.z*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		UINT idx_bankright2 = ThrMng->AddThrDef("Bank_right_2", _V(arm, 0, 0), _V(0, 1, 0), force_needed.z*0.5, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_bankright1);
		th_idx.push_back(idx_bankright1);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_BANKRIGHT, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_UP)) {
		UINT idx_up = ThrMng->AddThrDef("Translate_up", _V(0, 0, 0), _V(0, 1, 0), lin_force_needed.y, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_up);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_UP, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_DOWN)) {
		UINT idx_dwn = ThrMng->AddThrDef("Translate_dwn", _V(0, 0, 0), _V(0, -1, 0), lin_force_needed.y, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_dwn);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_DOWN, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_LEFT)) {
		UINT idx_left = ThrMng->AddThrDef("Translate_left", _V(0, 0, 0), _V(-1, 0, 0), lin_force_needed.x, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_left);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_LEFT, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_RIGHT)) {
		UINT idx_right = ThrMng->AddThrDef("Translate_right", _V(0, 0, 0), _V(1, 0, 0), lin_force_needed.x, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_right);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_RIGHT, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_FORWARD)) {
		UINT idx_forward = ThrMng->AddThrDef("Translate_forward", _V(0, 0, 0), _V(0, 0, 1), lin_force_needed.z, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_forward);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_FORWARD, th_h);
	}
	if (!ThrGrpMng->IsGroupDefined(THGROUP_ATT_BACK)) {
		UINT idx_back = ThrMng->AddThrDef("Translate_backward", _V(0, 0, 0), _V(0, 0, -1), lin_force_needed.z, PrpMng->GetMainTankPH(), massive_isp, 0, 101400);
		th_h.clear();
		th_idx.clear();
		th_idx.push_back(idx_back);
		th_h = ThrGrpMng->GetThrustersfromIdx(th_idx);
		ThrGrpMng->DefineGroup(THGROUP_ATT_BACK, th_h);
	}
	
	
	if (Dlg->IsOpen()) {
		Dlg->UpdateTree(Dlg->hDlg, THRUSTERS, 0);
		Dlg->UpdateTree(Dlg->hDlg, THRUSTERGROUPS, 0);
	}
	LogV("Default RCS System Added");
	return;
}

double VesselBuilder1::GetMET() {
	return Met->GetDeltaSeconds();
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
string VesselBuilder1::WriteVectorUINT(vector<UINT> v,bool spaces) {
	string line;
	line.clear();
	if (v.size() > 0) {
		for (UINT i = 0; i < v.size(); i++) {
			char add[128] = { '\0' };
			if (i == v.size() - 1) {
				sprintf(add, "%i", v[i]);
			}
			else {
				if (spaces) {
					sprintf(add, "%i, ", v[i]);
				}
				else {
					sprintf(add, "%i,", v[i]);
				}
				
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
bool VesselBuilder1::IsUintInVector(UINT u, vector<UINT>v) {
	for (UINT i = 0; i < v.size(); i++) {
		if (u == v[i]) {
			return true;
		}
	}
	return false;
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


