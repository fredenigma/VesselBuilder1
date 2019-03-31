#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "GeneralSettingsManager.h"
#include "MeshManager.h"
#include "DockManager.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"
#include "PropellantManager.h"
#include "ExTexManager.h"
#include "ParticleManager.h"
#include "ThrusterManager.h"
#include "TouchdownPointsManager.h"
#include "AirfoilsManager.h"
#include "ControlSurfacesManager.h"
#include "CameraManager.h"
#include "VCManager.h"
#include "LightsManager.h"
#include "VariableDragManager.h"
#include "ConfigurationManager.h"
#include "EventManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

Section::Section(VesselBuilder1* _VB1, UINT _config, FILEHANDLE cfg) {
	VB1 = _VB1;
	Config_idx = _config;
	Active = false;
	//ParseSection(cfg);
	return;
}

Section::~Section() { VB1 = NULL; }
void Section::WriteSection(FILEHANDLE fh){}
void Section::ParseSection(FILEHANDLE fh) {}
void Section::ApplySection() {
	Active = true;
}
void Section::UpdateSection(){}
void Section::ManagerClear(){}

void Section::ConfigCheck(char* cbuf, UINT config) {
	char abuf[256] = { '\0' };
	if (config != 0) {
		strcpy(abuf, cbuf);
		sprintf(cbuf, "%i_%s", config, abuf);
	}
	return;
}
void Section::SetConfigIdx(UINT config_idx) {
	Config_idx = config_idx;
	return;
}
UINT Section::GetConfigIdx() {
	return Config_idx;
}

SettingSection::SettingSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) : Section(VB1, config, cfg) {
	Def = Definitions();
	SetMng = VB1->SetMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	
	return;
}
SettingSection::~SettingSection() {
	SetMng = NULL;
}
void SettingSection::ParseSection(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "EMPTY_MASS");
	ConfigCheck(cbuf, Config_idx);
	oapiReadItem_float(fh, cbuf, Def.EmptyMass);
	sprintf(cbuf, "VSIZE");
	ConfigCheck(cbuf, Config_idx);
	oapiReadItem_float(fh, cbuf, Def.Size);
	sprintf(cbuf, "PMI");
	ConfigCheck(cbuf, Config_idx);
	oapiReadItem_vec(fh, cbuf, Def.PMI);
	sprintf(cbuf, "CSECTIONS");
	ConfigCheck(cbuf, Config_idx);
	oapiReadItem_vec(fh, cbuf, Def.CrossSections);
	sprintf(cbuf, "GRAVITYGDAMP");
	ConfigCheck(cbuf, Config_idx);
	oapiReadItem_float(fh, cbuf, Def.GravityGradientDamping);
	sprintf(cbuf, "ROTDRAG");
	ConfigCheck(cbuf, Config_idx);
	oapiReadItem_vec(fh, cbuf, Def.RotDrag);
	sprintf(cbuf, "MET_ENABLED");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, Def.met_enabled)) { Def.met_enabled = false; }
	if (Def.met_enabled) {
		sprintf(cbuf, "MET_IN_HUD");
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, Def.show_met_in_hud)) { Def.show_met_in_hud= false; }
	}
}
void SettingSection::WriteSection(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	oapiWriteLine(fh, " ");
	sprintf(cbuf, ";<-------------------------GENERAL SETTINGS - Configuration: %i------------------------->", Config_idx);
	oapiWriteLine(fh, cbuf);
	oapiWriteLine(fh, " ");
	sprintf(cbuf, "EMPTY_MASS");
	ConfigCheck(cbuf, Config_idx);
	oapiWriteItem_float(fh, cbuf, Def.EmptyMass);	
	sprintf(cbuf, "VSIZE");
	ConfigCheck(cbuf, Config_idx);
	oapiWriteItem_float(fh, cbuf, Def.Size);
	sprintf(cbuf, "PMI");
	ConfigCheck(cbuf, Config_idx);
	oapiWriteItem_vec(fh, cbuf, Def.PMI);
	sprintf(cbuf, "CSECTIONS");
	ConfigCheck(cbuf, Config_idx);
	oapiWriteItem_vec(fh, cbuf, Def.CrossSections);
	sprintf(cbuf, "GRAVITYGDAMP");
	ConfigCheck(cbuf, Config_idx);
	oapiWriteItem_float(fh, cbuf, Def.GravityGradientDamping);
	sprintf(cbuf, "ROTDRAG");
	ConfigCheck(cbuf, Config_idx);
	oapiWriteItem_vec(fh, cbuf, Def.RotDrag);
	if (Def.met_enabled) {
		sprintf(cbuf, "MET_ENABLED");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Def.met_enabled);
		if (Def.show_met_in_hud) {
			sprintf(cbuf, "MET_IN_HUD");
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Def.show_met_in_hud);
		}
	}
	
		
	oapiWriteLine(fh, " ");
	
}
void SettingSection::ApplySection() {
	Section::ApplySection();
	SetMng->SetEmptyMass(Def.EmptyMass);
	SetMng->SetSize(Def.Size);
	SetMng->SetPMI(Def.PMI);
	SetMng->SetCrossSections(Def.CrossSections);
	SetMng->SetGravityGradientDamping(Def.GravityGradientDamping);
	SetMng->SetRotDrag(Def.RotDrag);
	SetMng->EnableMET(Def.met_enabled);
	SetMng->SetShowMet(Def.show_met_in_hud);
}
void SettingSection::UpdateSection() {
	Def.EmptyMass = SetMng->GetEmptyMass();
	Def.Size = SetMng->GetSize();
	Def.PMI = SetMng->GetPMI();
	Def.CrossSections = SetMng->GetCrossSections();
	Def.GravityGradientDamping = SetMng->GetGravityGradientDamping();
	Def.RotDrag = SetMng->GetRotDrag();
	Def.met_enabled = SetMng->IsMETEnabled();
	Def.show_met_in_hud = SetMng->GetShowMet();
}
void SettingSection::ManagerClear() {
	SetMng->Clear();
}

SettingSection::Definitions SettingSection::GetSection() {
	return Def;
}

/*
double SettingSection::GetEmptyMass() {
	return Def.EmptyMass;
}
double SettingSection::GetSize() {
	return Def.Size;
}
VECTOR3 SettingSection::GetPMI() {
	return Def.PMI;
}
VECTOR3 SettingSection::GetCrossSections() {
	return Def.CrossSections;
}
double SettingSection::GetGravityGradientDamping() {
	return Def.GravityGradientDamping;
}
VECTOR3 SettingSection::GetRotDrag() {
	return Def.RotDrag;
}*/


MeshSection::MeshSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1,config, cfg){
	Defs.clear();
	MshMng = VB1->MshMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
MeshSection::~MeshSection() { MshMng = NULL; }

void MeshSection::ParseSection(FILEHANDLE fh) {
	UINT mesh_counter = 0;
	char cbuf[256] = { '\0' };
	char item[256] = { '\0' };
	UINT config = Config_idx;
	sprintf_s(cbuf, "MESH_%i_NAME", mesh_counter);
	ConfigCheck(cbuf, config);
	
	
	while (oapiReadItem_string(fh, cbuf, item)) {
		//LogV("Mesh:%i name:%s", mesh_counter, item);
		string mn(item);
		VECTOR3 pos = _V(0, 0, 0);
		VECTOR3 dir = _V(0, 0, 1);
		VECTOR3 rot = _V(0, 1, 0);
		WORD visibility;
		
		sprintf_s(cbuf, "MESH_%i_POS", mesh_counter);
		ConfigCheck(cbuf, config);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf_s(cbuf, "MESH_%i_DIR", mesh_counter);
		ConfigCheck(cbuf, config);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf_s(cbuf, "MESH_%i_ROT", mesh_counter);
		ConfigCheck(cbuf, config);
		oapiReadItem_vec(fh, cbuf, rot);
		sprintf(cbuf, "MESH_%i_VIS", mesh_counter);
		ConfigCheck(cbuf, config);
		int vis;
		if (!oapiReadItem_int(fh, cbuf, vis)) { vis = 1; }
		visibility = (WORD)vis;
		Definitions d = Definitions();
		d.meshname = mn;
		d.pos = pos;
		d.dir = dir;
		d.rot = rot;
		d.visibility = visibility;
		Defs.push_back(d);
		mesh_counter++;
		sprintf_s(cbuf, "MESH_%i_NAME", mesh_counter);
		ConfigCheck(cbuf, config);
	}
	for (UINT i = 0; i < Defs.size(); i++) {
		MshMng->PreLoadMesh(Defs[i].meshname, Defs[i].dir, Defs[i].rot);
	}
}

void MeshSection::WriteSection(FILEHANDLE fh) {
	//FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_APP, CONFIG);
	oapiWriteLine(fh, ";<-------------------------MESHES DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	char cbuf[256] = { '\0' };
	char abuf[256] = { '\0' };
	char cbuf2[256] = { '\0' };
	UINT config = Config_idx;
	for (UINT i = 0; i < Defs.size(); i++) {
		sprintf(cbuf, "MESH_%i_NAME", i);
		ConfigCheck(cbuf, config);
		sprintf(cbuf2, "%s", Defs[i].meshname.c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "MESH_%i_POS", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_vec(fh, cbuf, Defs[i].pos);
		sprintf(cbuf, "MESH_%i_DIR", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_vec(fh, cbuf, Defs[i].dir);
		sprintf(cbuf, "MESH_%i_ROT", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_vec(fh, cbuf, Defs[i].rot);
		sprintf(cbuf, "MESH_%i_VIS", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_int(fh, cbuf, Defs[i].visibility);
		oapiWriteLine(fh, " ");
	}
	//oapiCloseFile(fh, FILE_APP);
	return;
}
void MeshSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < MshMng->GetMeshCount(); i++) {
		Definitions d = Definitions();
		d.meshname = MshMng->GetMeshName(i);
		d.pos = MshMng->GetMeshDefPos(i);
		d.dir = MshMng->GetMeshDefDir(i);
		d.rot = MshMng->GetMEshDefRot(i);
		d.visibility = MshMng->GetMeshVisibility(i);
		Defs.push_back(d);
	}
}
void MeshSection::ManagerClear() {
	MshMng->Clear();
}
void MeshSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		MshMng->AddMeshDef(Defs[i].meshname, Defs[i].pos, Defs[i].dir, Defs[i].rot, Defs[i].visibility);
	}
	MshMng->PreLoadMeshes();
	MshMng->AddMeshes();
}

VBVector<MeshSection::Definitions> MeshSection::GetSection() {
	return Defs;
}
/*int MeshSection::GetMeshDefCount() {
	return Defs.size();
}
string MeshSection::GetMeshName(UINT idx) {
	return Defs[idx].meshname;
}
VECTOR3 MeshSection::GetMeshPos(UINT idx) {
	return Defs[idx].pos;
}
VECTOR3 MeshSection::GetMeshDir(UINT idx) {
	return Defs[idx].dir;
}
VECTOR3 MeshSection::GetMeshRot(UINT idx) {
	return Defs[idx].rot;
}
WORD MeshSection::GetMeshVisibility(UINT idx) {
	return (WORD)Defs[idx].visibility;
}*/




DockSection::DockSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	DckMng = VB1->DckMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
DockSection::~DockSection() { DckMng = NULL; }
void DockSection::ParseSection(FILEHANDLE fh) {
	UINT dock_counter = 0;
	char cbuf[256] = { '\0' };
	UINT config = Config_idx;
	sprintf(cbuf, "DOCK_%i_ID", dock_counter);
	ConfigCheck(cbuf, config);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		VECTOR3 pos, dir, rot;
		bool jett;
		bool sd;
		double sd_dist = 0;
		int sd_anim = -1;
		sprintf(cbuf, "DOCK_%i_NAME", dock_counter);
		ConfigCheck(cbuf, config);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "DOCK_%i_POS", dock_counter);
		ConfigCheck(cbuf, config);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "DOCK_%i_DIR", dock_counter);
		ConfigCheck(cbuf, config);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "DOCK_%i_ROT", dock_counter);
		ConfigCheck(cbuf, config);
		oapiReadItem_vec(fh, cbuf, rot);
		sprintf(cbuf, "DOCK_%i_JETT", dock_counter);
		ConfigCheck(cbuf, config);
		sprintf(cbuf, "DOCK_%i_SOFTDOCK", dock_counter);
		ConfigCheck(cbuf, config);
		if (!oapiReadItem_bool(fh, cbuf, sd)) { sd = false; }
		if (sd) {
			sprintf(cbuf, "DOCK_%i_SOFTDOCKDISTANCE", dock_counter);
			ConfigCheck(cbuf, config);
			oapiReadItem_float(fh, cbuf, sd_dist);
			sprintf(cbuf, "DOCK_%i_SOFTDOCKANIM", dock_counter);
			ConfigCheck(cbuf, config);
			oapiReadItem_int(fh, cbuf, sd_anim);
		}
		
		if (!oapiReadItem_bool(fh, cbuf, jett)) { jett = false; }
		Definitions d = Definitions();
		d.name = name;
		d.pos = pos;
		d.dir = dir;
		d.rot = rot;
		d.jettisonable = jett;
		d.softdock = sd;
		d.sd_distance = sd_dist;
		d.sd_anim = sd_anim;
		Defs.push_back(d);
		dock_counter++;
		sprintf(cbuf, "DOCK_%i_ID", dock_counter);
		ConfigCheck(cbuf, config);
	}
	return;
}
void DockSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------DOCKS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	UINT config = Config_idx;
	for (UINT i = 0; i <Defs.size(); i++) {
		char cbuf[256] = { '\0' };
//		VECTOR3 pos, dir, rot;
		sprintf(cbuf, "DOCK_%i_ID", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "DOCK_%i_NAME", i);
		ConfigCheck(cbuf, config);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "DOCK_%i_POS", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_vec(fh, cbuf, Defs[i].pos);
		sprintf(cbuf, "DOCK_%i_DIR", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_vec(fh, cbuf, Defs[i].dir);
		sprintf(cbuf, "DOCK_%i_ROT", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_vec(fh, cbuf, Defs[i].rot);
		sprintf(cbuf, "DOCK_%i_JETT", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_bool(fh, cbuf, Defs[i].jettisonable);
		sprintf(cbuf, "DOCK_%i_SOFTDOCK", i);
		ConfigCheck(cbuf, config);
		oapiWriteItem_bool(fh, cbuf, Defs[i].softdock);
		if (Defs[i].softdock) {
			sprintf(cbuf, "DOCK_%i_SOFTDOCKDISTANCE", i);
			ConfigCheck(cbuf, config);
			oapiWriteItem_float(fh, cbuf, Defs[i].sd_distance);
			sprintf(cbuf, "DOCK_%i_SOFTDOCKANIM", i);
			ConfigCheck(cbuf, config);
			oapiWriteItem_int(fh, cbuf, Defs[i].sd_anim);
		}
		oapiWriteLine(fh, " ");
	}
}

void DockSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < DckMng->GetDockCount(); i++) {
		Definitions d = Definitions();
		VECTOR3 pos, dir, rot;
		DckMng->GetDockParams(i, pos, dir, rot);
		d.pos = pos;
		d.dir = dir;
		d.rot = rot;
		d.name = DckMng->GetDockName(i);
		bool sd;
		double sd_dist;
		def_idx sd_anim;
		d.jettisonable = DckMng->IsDockJettisonable(i);
		DckMng->GetSoftDockParams(i, sd, sd_dist, sd_anim);
		d.softdock = sd;
		d.sd_distance = sd_dist;
		d.sd_anim = sd_anim;
		Defs.push_back(d);
	}
}
void DockSection::ManagerClear() {
	DckMng->Clear();
}
void DockSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		if (Defs[i].softdock) {
			DckMng->AddDockDef(Defs[i].name, Defs[i].pos, Defs[i].dir, Defs[i].rot, Defs[i].jettisonable,Defs[i].softdock,Defs[i].sd_distance,Defs[i].sd_anim);
		}
		else {
			DckMng->AddDockDef(Defs[i].name, Defs[i].pos, Defs[i].dir, Defs[i].rot, Defs[i].jettisonable);
		}
		
	}
}

VBVector<DockSection::Definitions> DockSection::GetSection() {
	return Defs;
}
/*UINT DockSection::GetDockCount() {
	return Defs.size();
}
string DockSection::GetDockName(UINT idx) {
	return Defs[idx].name;
}
void DockSection::GetDockParams(UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot) {
	pos = Defs[idx].pos;
	dir = Defs[idx].dir;
	rot = Defs[idx].rot;
	return;
}
bool DockSection::IsDockJettisonable(UINT idx) {
	return Defs[idx].jettisonable;
}
*/



AttachmentSection::AttachmentSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	AttMng = VB1->AttMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
}
AttachmentSection::~AttachmentSection() { AttMng = NULL; }
void AttachmentSection::ParseSection(FILEHANDLE fh) {
	UINT att_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "ATT_%i_IDX", att_counter);
	ConfigCheck(cbuf, Config_idx);
	int idx;
	while (oapiReadItem_int(fh, cbuf, idx)) {
		VECTOR3 pos, dir, rot;
		char idbuf[256] = { '\0' };
		bool toparent = false;
		sprintf(cbuf, "ATT_%i_POS", att_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "ATT_%i_DIR", att_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "ATT_%i_ROT", att_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, rot);
		double range = 10;
		sprintf(cbuf, "ATT_%i_RANGE", att_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, range);
		sprintf(cbuf, "ATT_%i_ID", att_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, idbuf);
		string id(idbuf);
		sprintf(cbuf, "ATT_%i_TOPARENT", att_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_bool(fh, cbuf, toparent);

		bool id_check;
		string id_check_string;
		id_check_string.clear();
		sprintf(cbuf, "ATT_%i_IDCHECK", att_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, id_check)) { id_check = false; }
		if (id_check) {
			sprintf(cbuf, "ATT_%i_IDCHECKSTRING", att_counter);
			ConfigCheck(cbuf, Config_idx);
			char checkbuff[256] = { '\0' };
			oapiReadItem_string(fh, cbuf, checkbuff);
			id_check_string.assign(checkbuff);
		}
		Definitions d = Definitions();
		d.toparent = toparent;
		d.pos = pos;
		d.dir = dir;
		d.rot = rot;
		d.id = id;
		d.range = range;
		d.idcheck = id_check;
		d.idcheck_string = id_check_string;
		Defs.push_back(d);

		att_counter++;
		sprintf(cbuf, "ATT_%i_IDX", att_counter);
		ConfigCheck(cbuf, Config_idx);
	}
}
void AttachmentSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------ATTACHMENTS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");

	
	for (UINT i = 0; i < Defs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "ATT_%i_IDX", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "ATT_%i_POS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].pos);
		sprintf(cbuf, "ATT_%i_DIR", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].dir);
		sprintf(cbuf, "ATT_%i_ROT", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].rot);
		sprintf(cbuf, "ATT_%i_RANGE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].range);
		sprintf(cbuf, "ATT_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		char cbuf2[256] = { '\0' };
		sprintf(cbuf2, Defs[i].id.c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ATT_%i_TOPARENT", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defs[i].toparent);
		sprintf(cbuf, "ATT_%i_IDCHECK", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defs[i].idcheck);
		if (Defs[i].idcheck)
		{
			sprintf(cbuf, "ATT_%i_IDCHECKSTRING", i);
			ConfigCheck(cbuf, Config_idx);
			char checkbuff[256] = { '\0' };
			sprintf(checkbuff, "%s", Defs[i].idcheck_string.c_str());
			oapiWriteItem_string(fh, cbuf, checkbuff);
		}
		oapiWriteLine(fh, " ");

	}
	return;
}
void AttachmentSection::ManagerClear() {
	AttMng->Clear();
}
void AttachmentSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		AttMng->CreateAttDef(Defs[i].toparent, Defs[i].pos, Defs[i].dir, Defs[i].rot, Defs[i].id, Defs[i].range, false, Defs[i].idcheck, Defs[i].idcheck_string);
	}
}
void AttachmentSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < AttMng->GetAttCount(); i++) {
		if (!AttMng->AttIsCreated(i)) { continue; }
		Definitions d = Definitions();
		d.pos = AttMng->att_defs[i].pos;
		d.dir = AttMng->att_defs[i].dir;
		d.rot = AttMng->att_defs[i].rot;
		d.range = AttMng->att_defs[i].range;
		d.id = AttMng->att_defs[i].id;
		d.toparent = AttMng->att_defs[i].toparent;
		d.idcheck = AttMng->att_defs[i].id_check;
		d.idcheck_string = AttMng->att_defs[i].id_check_string;
		Defs.push_back(d);
	}
}
VBVector<AttachmentSection::Definitions> AttachmentSection::GetSection() {
	return Defs;
}

/*UINT AttachmentSection::GetAttCount() {
	return Defs.size();
}
bool AttachmentSection::GetIdCheck(UINT idx) {
	return Defs[idx].idcheck;
}
string AttachmentSection::GetIdCheckString(UINT idx) {
	return Defs[idx].idcheck_string;
}
string AttachmentSection::GetAttID(UINT idx) {
	return Defs[idx].id;
}
bool AttachmentSection::AttToParent(UINT idx) {
	return Defs[idx].toparent;
}
void AttachmentSection::GetAttPosDirRot(UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot) {
	pos = Defs[idx].pos;
	dir = Defs[idx].dir;
	rot = Defs[idx].rot;
	return;
}
double AttachmentSection::GetAttRange(UINT idx) {
	return Defs[idx].range;
}*/



AnimationSection::AnimationSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	AnimDefs.clear();
	AnimCompDefs.clear();
	AnimMng = VB1->AnimMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
AnimationSection::~AnimationSection() { AnimMng = NULL; }
void AnimationSection::ParseSection(FILEHANDLE fh) {
	UINT anim_counter = 0;
	char cbuf[256] = { '\0' };
	int id;
	sprintf(cbuf, "ANIM_%i_ID", anim_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		sprintf(cbuf, "ANIM_%i_NAME", anim_counter);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		double defstate, duration;
		DWORD key;
		AnimCycleType Cycle;
		sprintf(cbuf, "ANIM_%i_DEFSTATE", anim_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, defstate);
		sprintf(cbuf, "ANIM_%i_DURATION", anim_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, duration);
		sprintf(cbuf, "ANIM_%i_KEY", anim_counter);
		ConfigCheck(cbuf, Config_idx);
		int kk;
		oapiReadItem_int(fh, cbuf, kk);
		key = (DWORD)kk;
		int Cyc;
		sprintf(cbuf, "ANIM_%i_CYCLE", anim_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, Cyc);
		Cycle = (AnimCycleType)Cyc;
		AnimDefinitions ad = AnimDefinitions();
		ad.anim_name = name;
		ad.anim_duration = duration;
		ad.Cycle = Cycle;
		ad.anim_key = key;
		ad.anim_defstate = defstate;
		AnimDefs.push_back(ad);
		//AddAnimDef(name, duration, Cycle, key, defstate);

		anim_counter++;
		sprintf(cbuf, "ANIM_%i_ID", anim_counter);
		ConfigCheck(cbuf, Config_idx);
	}

	for (UINT i = 0; i < 256; i++) {
		cbuf[i] = '\0';
	}
	UINT animcompdef_counter = 0;
	sprintf(cbuf, "ANIMCOMP_%i_ID", animcompdef_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		int seq, parent_idx, armatt, mesh, type_int, ngrps;
		bool arm_tip;
		double state0, state1, angle;
		VECTOR3 ref, axis, scale, shift;
		VBVector<UINT>grps;
		MGROUP_TRANSFORM::TYPE type;
		sprintf(cbuf, "ANIMCOMP_%i_NAME", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "ANIMCOMP_%i_SEQ", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, seq);
		sprintf(cbuf, "ANIMCOMP_%i_STATE0", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, state0);
		sprintf(cbuf, "ANIMCOMP_%i_STATE1", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, state1);
		sprintf(cbuf, "ANIMCOMP_%i_MESH", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, mesh);
		sprintf(cbuf, "ANIMCOMP_%i_NGRPS", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, ngrps);

		grps.clear();
		if (ngrps > 0) {
			sprintf(cbuf, "ANIMCOMP_%i_GRPS", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			char grpsbuf[256] = { '\0' };
			oapiReadItem_string(fh, cbuf, grpsbuf);
			string grpsbuf_s(grpsbuf);
			grps = VB1->readVectorUINT(grpsbuf_s);
			//LogV("GRPS SIZE:%i", grps.size());
			for (UINT zz = 0; zz < grps.size(); zz++) {
				//LogV("animcomp:%i grps[%i]=%i", animcompdef_counter, zz, grps[zz]);
			}
			if ((grps.size() == 1) && (grps[0] == -1)) {
				mesh = LOCALVERTEXLIST;
			}
		}
		sprintf(cbuf, "ANIMCOMP_%i_TYPE", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, type_int);
		type = (MGROUP_TRANSFORM::TYPE)type_int;
		if (type_int == 1) { //ROTATE
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, ref);
			sprintf(cbuf, "ANIMCOMP_%i_AXIS", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, axis);
			sprintf(cbuf, "ANIMCOMP_%i_ANGLE", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, angle);
			angle *= RAD;
		}
		else if (type_int == 2) { //TRANSLATE
			sprintf(cbuf, "ANIMCOMP_%i_SHIFT", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, shift);
		}
		else if (type_int == 3) { //SCALE
			sprintf(cbuf, "ANIMCOMP_%i_REF", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, ref);
			sprintf(cbuf, "ANIMCOMP_%i_SCALE", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, scale);
		}
		sprintf(cbuf, "ANIMCOMP_%i_ARMTIP", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, arm_tip)) { arm_tip = false; }
		if (arm_tip) {
			sprintf(cbuf, "ANIMCOMP_%i_ARMATT", animcompdef_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, armatt);
		}

		sprintf(cbuf, "ANIMCOMP_%i_PARENT", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, parent_idx);
		AnimCompDefinitions acd = AnimCompDefinitions();
		acd.animcomp_seq = seq;
		acd.animcomp_name = name;
		acd.animcomp_state0 = state0;
		acd.animcomp_state1 = state1;
		acd.animcomp_mesh = mesh;
		acd.animcomp_ngrps = ngrps;
		acd.animcomp_grps = grps;
		acd.parent_idx = parent_idx;
		acd.type = type_int;
		acd.ref = ref;
		acd.axis = axis;
		acd.scale = scale;
		acd.shift = shift;
		acd.angle = angle;
		acd.arm_tip = arm_tip;
		acd.arm_att = armatt;
		AnimCompDefs.push_back(acd);
	//	UINT index = AddAnimCompDef(seq, name, state0, state1, mesh, ngrps, grps, parent_idx, type_int, ref, axis, scale, shift, angle);
	//	if (arm_tip) {
		//	SetAnimCompDefArmTip(index, armatt);
	//	}


		animcompdef_counter++;
		sprintf(cbuf, "ANIMCOMP_%i_ID", animcompdef_counter);
		ConfigCheck(cbuf, Config_idx);
	}

	return;
}

void AnimationSection::WriteSection(FILEHANDLE fh) {

	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------ANIMATIONS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");

	char cbuf[256] = { '\0' };
	char cbuf2[256] = { '\0' };
	
	for (UINT i = 0; i < AnimDefs.size(); i++) {
		sprintf(cbuf, "ANIM_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "ANIM_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		sprintf(cbuf2, "%s", AnimDefs[i].anim_name.c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIM_%i_DEFSTATE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, AnimDefs[i].anim_defstate);
		sprintf(cbuf, "ANIM_%i_DURATION", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, AnimDefs[i].anim_duration);
		sprintf(cbuf, "ANIM_%i_KEY", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, AnimDefs[i].anim_key);
		sprintf(cbuf, "ANIM_%i_CYCLE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, (int)AnimDefs[i].Cycle);
		oapiWriteLine(fh, " ");

	}

	for (UINT i = 0; i < AnimCompDefs.size(); i++) {
		sprintf(cbuf, "ANIMCOMP_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "ANIMCOMP_%i_SEQ", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, AnimCompDefs[i].animcomp_seq);
		sprintf(cbuf, "ANIMCOMP_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		sprintf(cbuf2, "%s", AnimCompDefs[i].animcomp_name.c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIMCOMP_%i_STATE0", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, AnimCompDefs[i].animcomp_state0);
		sprintf(cbuf, "ANIMCOMP_%i_STATE1", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, AnimCompDefs[i].animcomp_state1);
		bool isArmTip = AnimCompDefs[i].arm_tip;
		sprintf(cbuf, "ANIMCOMP_%i_ARMTIP", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, isArmTip);
		if (isArmTip) {
			sprintf(cbuf, "ANIMCOMP_%i_ARMATT", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, AnimCompDefs[i].arm_att);
		}
		sprintf(cbuf, "ANIMCOMP_%i_MESH", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, AnimCompDefs[i].animcomp_mesh);
		sprintf(cbuf, "ANIMCOMP_%i_TYPE", i);
		ConfigCheck(cbuf, Config_idx);
		int type = AnimCompDefs[i].type;
		oapiWriteItem_int(fh, cbuf, type);
		if (type == 1) { //ROTATE
			sprintf(cbuf, "ANIMCOMP_%i_REF", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, AnimCompDefs[i].ref);
			sprintf(cbuf, "ANIMCOMP_%i_AXIS", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, AnimCompDefs[i].axis);
			sprintf(cbuf, "ANIMCOMP_%i_ANGLE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, AnimCompDefs[i].angle*DEG);
		}
		else if (type == 2) { //TRANSLATE
			sprintf(cbuf, "ANIMCOMP_%i_SHIFT", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, AnimCompDefs[i].shift);
		}
		else if (type == 3) { //SCALE
			sprintf(cbuf, "ANIMCOMP_%i_REF", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, AnimCompDefs[i].ref);
			sprintf(cbuf, "ANIMCOMP_%i_SCALE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, AnimCompDefs[i].scale);
		}
		sprintf(cbuf, "ANIMCOMP_%i_NGRPS", i);
		ConfigCheck(cbuf, Config_idx);
		int ngrps = AnimCompDefs[i].animcomp_ngrps;
		oapiWriteItem_int(fh, cbuf, ngrps);
		VBVector<UINT> grps = AnimCompDefs[i].animcomp_grps;
		string line = VB1->WriteVectorUINT(grps);
		sprintf(cbuf2, "%s", line.c_str());
		sprintf(cbuf, "ANIMCOMP_%i_GRPS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ANIMCOMP_%i_PARENT", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, AnimCompDefs[i].parent_idx);
		oapiWriteLine(fh, " ");
	}

	return;
}

void AnimationSection::ManagerClear() {
	AnimMng->Clear();
}
void AnimationSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < AnimDefs.size(); i++) {
		AnimMng->AddAnimDef(AnimDefs[i].anim_name, AnimDefs[i].anim_duration, AnimDefs[i].Cycle, AnimDefs[i].anim_key, AnimDefs[i].anim_defstate);
	}
	for (UINT i = 0; i < AnimCompDefs.size(); i++) {
		UINT index = AnimMng->AddAnimCompDef(AnimCompDefs[i].animcomp_seq, AnimCompDefs[i].animcomp_name, AnimCompDefs[i].animcomp_state0, AnimCompDefs[i].animcomp_state1, AnimCompDefs[i].animcomp_mesh, AnimCompDefs[i].animcomp_ngrps, AnimCompDefs[i].animcomp_grps, AnimCompDefs[i].parent_idx, AnimCompDefs[i].type, AnimCompDefs[i].ref, AnimCompDefs[i].axis, AnimCompDefs[i].scale, AnimCompDefs[i].shift, AnimCompDefs[i].angle);
		if (AnimCompDefs[i].arm_tip) {
			AnimMng->SetAnimCompDefArmTip(index, AnimCompDefs[i].arm_att);
		}
	}
	return;
}
void AnimationSection::UpdateSection() {
	AnimDefs.clear();
	for(UINT i=0;i<AnimMng->GetAnimDefsCount();i++){
		if (!AnimMng->IsAnimValid(i)) { continue; }
		AnimDefinitions ad = AnimDefinitions();
		ad.anim_name = AnimMng->GetAnimName(i);
		ad.anim_defstate = AnimMng->GetAnimDefState(i);
		ad.anim_duration = AnimMng->GetAnimDuration(i);
		ad.anim_key = AnimMng->GetAnimKey(i);
		ad.Cycle = AnimMng->GetAnimCycle(i);
		AnimDefs.push_back(ad);
	}
	AnimCompDefs.clear();
	for (UINT i = 0; i < AnimMng->GetAnimCompDefsCount(); i++) {
		if (!AnimMng->IsAnimCompDefValid(i)) { continue; }
		AnimCompDefinitions acd = AnimCompDefinitions();
		acd.animcomp_name = AnimMng->GetAnimCompDefName(i);
		acd.animcomp_seq = AnimMng->GetAnimCompDefSeqIdx(i);
		acd.animcomp_state0 = AnimMng->GetAnimCompDefState0(i);
		acd.animcomp_state1 = AnimMng->GetAnimCompDefState1(i);
		acd.arm_tip = AnimMng->IsAnimCompArmTip(i);
		if (acd.arm_tip) {
			acd.arm_att = AnimMng->GetAnimCompDefAttTip(i);
		}
		else {
			acd.arm_att = -1;
		}
		acd.animcomp_mesh = AnimMng->GetAnimCompDefMesh(i);
		acd.type = (int)AnimMng->GetAnimCompDefType(i);
		if (acd.type == 1) {
			acd.ref = AnimMng->GetAnimCompDefRef(i);
			acd.axis = AnimMng->GetAnimCompDefAxis(i);
			acd.angle = AnimMng->GetAnimCompDefAngle(i);
		}
		else if (acd.type == 2) {
			acd.shift = AnimMng->GetAnimCompDefShift(i);
		}
		else if (acd.type == 3) {
			acd.ref = AnimMng->GetAnimCompDefRef(i);
			acd.scale = AnimMng->GetAnimCompDefScale(i);
		}
		acd.animcomp_ngrps = AnimMng->GetAnimCompDefNGroups(i);
		acd.animcomp_grps = AnimMng->GetAnimCompDefGroups(i);
		acd.parent_idx = AnimMng->GetParentCompDefIdx(AnimMng->GetAnimCompDefParent(i));
		AnimCompDefs.push_back(acd);
	}
	return;
}

void AnimationSection::GetSection(VBVector<AnimationSection::AnimDefinitions> &ad, VBVector<AnimationSection::AnimCompDefinitions> &acd) {
	ad = AnimDefs;
	acd = AnimCompDefs;
	return;
}

/*UINT AnimationSection::GetAnimCount() {
	return AnimDefs.size();
}
double AnimationSection::GetAnimDefState(UINT idx) {
	return AnimDefs[idx].anim_defstate;
}
string AnimationSection::GetAnimName(UINT idx) {
	return AnimDefs[idx].anim_name;
}
UINT AnimationSection::GetAnimNComps(UINT idx) {
	return 0;
}
DWORD AnimationSection::GetAnimKey(UINT idx) {
	return AnimDefs[idx].anim_key;
}
double AnimationSection::GetAnimDuration(UINT idx) {
	return AnimDefs[idx].anim_duration;
}
AnimCycleType AnimationSection::GetAnimCycle(UINT idx) {
	return AnimDefs[idx].Cycle;
}*/




PropellantSection::PropellantSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	PrpMng = VB1->PrpMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
PropellantSection::~PropellantSection() { PrpMng = NULL; }
void PropellantSection::ParseSection(FILEHANDLE fh) {
	UINT prp_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "PRP_%i_ID", prp_counter);
	ConfigCheck(cbuf, Config_idx);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		double maxmass = 0;
		double efficiency = 1;
		bool primary = false;
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "PRP_%i_MAXMASS", prp_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, maxmass);
		sprintf(cbuf, "PRP_%i_EFFICIENCY", prp_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, efficiency);
		sprintf(cbuf, "PRP_%i_NAME", prp_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "PRP_%i_PRIMARY", prp_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, primary)) { primary = false; }
		sprintf(cbuf, "PRP_%i_RETAINFL", prp_counter);
		ConfigCheck(cbuf, Config_idx);
		bool retain;
		if (!oapiReadItem_bool(fh, cbuf, retain)) { retain = false; }

		Definitions d = Definitions();
		d.name = name;
		d.maxmass = maxmass;
		d.efficiency = efficiency;
		d.primary = primary;
		d.retainfl = retain;
		Defs.push_back(d);
		
		
		prp_counter++;
		sprintf(cbuf, "PRP_%i_ID", prp_counter);
		ConfigCheck(cbuf, Config_idx);
	}
}
void PropellantSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, ";<-------------------------PROPELLANT DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (DWORD i = 0; i < Defs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "PRP_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "PRP_%i_MAXMASS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].maxmass);
		sprintf(cbuf, "PRP_%i_EFFICIENCY", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].efficiency);
		sprintf(cbuf, "PRP_%i_PRIMARY", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defs[i].primary);
		if (Defs[i].retainfl) {
			sprintf(cbuf, "PRP_%i_RETAINFL", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].retainfl);
		}
		sprintf(cbuf, "PRP_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char name[256] = { '\0' };
		sprintf(name, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, name);
		oapiWriteLine(fh, " ");
	}

}
void PropellantSection::ManagerClear() {
	for (UINT i = 0; i < Defs.size(); i++) {
		if (Defs[i].retainfl) {
			if (i >= PrpMng->GetPropCount()) { continue; }
			double curmass = PrpMng->GetTankCurrentMass(i);
			double maxmass = PrpMng->GetTankMaxMass(i);
			double level = curmass / maxmass;
			Defs[i].currentmass = Defs[i].maxmass*level;
		}
		else {
			Defs[i].currentmass = -1;
		}
	}
	PrpMng->Clear();
}
void PropellantSection::ApplySection() {
	Section::ApplySection();
		
	for (UINT i = 0; i < Defs.size(); i++) {
		PrpMng->AddTankDef(Defs[i].name, Defs[i].maxmass, Defs[i].efficiency,Defs[i].currentmass);
		if (Defs[i].primary) {
			PrpMng->MakePrimary(i);
		}
		PrpMng->SetPrpRetainFuel(i, Defs[i].retainfl);
	}
	return;
}
void PropellantSection::UpdateSection() {
	Defs.clear();
	for (DWORD i = 0; i < PrpMng->GetPropCount(); i++) {
		Definitions d = Definitions();
		d.name = PrpMng->GetTankName(i);
		d.maxmass = PrpMng->GetTankMaxMass(i);
		d.efficiency = PrpMng->GetTankEfficiency(i);
		d.primary = PrpMng->IsPrimary(i);
		d.retainfl = PrpMng->GetPrpRetainFuel(i);
		Defs.push_back(d);
	}
	return;
}
VBVector<PropellantSection::Definitions> PropellantSection::GetSection() {
	return Defs;
}



ThrusterSection::ThrusterSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	ThrMng = VB1->ThrMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
ThrusterSection::~ThrusterSection() { ThrMng = NULL; }
void ThrusterSection::ParseSection(FILEHANDLE fh) {
	UINT thr_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "THR_%i_ID", thr_counter);
	ConfigCheck(cbuf, Config_idx);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		VECTOR3 pos, dir;
		double Max0, isp, ispref, pref;
		int ph_idx;
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "THR_%i_NAME", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "THR_%i_POS", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "THR_%i_DIR", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "THR_%i_MAX0", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, Max0);
		sprintf(cbuf, "THR_%i_ISP0", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, isp);
		sprintf(cbuf, "THR_%i_ISPREF", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, ispref);
		sprintf(cbuf, "THR_%i_PREF", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, pref);
		sprintf(cbuf, "THR_%i_TANK", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, ph_idx);
	//	PROPELLANT_HANDLE ph = NULL;
	//	if (ph_idx != -1) {
	//		ph = VB1->PrpMng->GetTankPH(ph_idx);
	//	}
		//UINT thidx = AddThrDef(name, pos, dir, Max0, ph, isp, ispref, pref);
		Definitions d = Definitions();
		d.name = name;
		d.pos = pos;
		d.dir = dir;
		d.max0 = Max0;
		d.tank = ph_idx;
		d.isp0 = isp;
		d.ispref = ispref;
		d.pref = pref;
		bool hasexh;
		sprintf(cbuf, "THR_%i_HASEXHAUST", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, hasexh)) { hasexh = false; }
		d.hasexhaust = hasexh;

		if (hasexh) {
			UINT ex_counter = 0;
			int ex_id;
			sprintf(cbuf, "THR_%i_EX_%i_ID", thr_counter, ex_counter);
			ConfigCheck(cbuf, Config_idx);
			while (oapiReadItem_int(fh, cbuf, ex_id))
			{
				double lsize, wsize;
				bool customposdir;
				VECTOR3 pos, dir;
				int extex;
				sprintf(cbuf, "THR_%i_EX_%i_LSIZE", thr_counter, ex_counter);
				ConfigCheck(cbuf, Config_idx);
				oapiReadItem_float(fh, cbuf, lsize);
				sprintf(cbuf, "THR_%i_EX_%i_WSIZE", thr_counter, ex_counter);
				ConfigCheck(cbuf, Config_idx);
				oapiReadItem_float(fh, cbuf, wsize);
				sprintf(cbuf, "THR_%i_EX_%i_EXTEX", thr_counter, ex_counter);
				ConfigCheck(cbuf, Config_idx);
				oapiReadItem_int(fh, cbuf, extex);
				sprintf(cbuf, "THR_%i_EX_%i_CUSTOMPOSDIR", thr_counter, ex_counter);
				ConfigCheck(cbuf, Config_idx);
				if (!oapiReadItem_bool(fh, cbuf, customposdir)) { customposdir = false; }
				if (customposdir) {
					sprintf(cbuf, "THR_%i_EX_%i_POS", thr_counter, ex_counter);
					ConfigCheck(cbuf, Config_idx);
					oapiReadItem_vec(fh, cbuf, pos);
					sprintf(cbuf, "THR_%i_EX_%i_DIR", thr_counter, ex_counter);
					ConfigCheck(cbuf, Config_idx);
					oapiReadItem_vec(fh, cbuf, dir);
				}
		//		SURFHANDLE tex = extex == -1 ? NULL : VB1->ExTMng->GetExTexSurf(extex);
		//		AddThrExhaust(thidx, lsize, wsize, tex, customposdir, pos, dir);
				Definitions::ExhaustDefs ed;
				ed.lsize = lsize;
				ed.wsize = wsize;
				ed.extex = extex;
				ed.customposdir = customposdir;
				ed.pos = pos;
				ed.dir = dir;
				d.Exhausts.push_back(ed);
				ex_counter++;
				sprintf(cbuf, "THR_%i_EX_%i_ID", thr_counter, ex_counter);
				ConfigCheck(cbuf, Config_idx);
			}
		}
		bool haspart;
		sprintf(cbuf, "THR_%i_HASPARTICLES", thr_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, haspart)) { haspart = false; }
		d.hasparticle = haspart;
		if (haspart) {
			UINT part_counter = 0;
			int part_id;
			sprintf(cbuf, "THR_%i_PART_%i_ID", thr_counter, part_counter);
			ConfigCheck(cbuf, Config_idx);
			while (oapiReadItem_int(fh, cbuf, part_id))
			{
				UINT pss_index;
				bool custompos;
				VECTOR3 pos;
				sprintf(cbuf, "THR_%i_PART_%i_PSSIDX", thr_counter, part_counter);
				ConfigCheck(cbuf, Config_idx);
				int pss_int;
				oapiReadItem_int(fh, cbuf, pss_int);
				pss_index = (UINT)pss_int;
				sprintf(cbuf, "THR_%i_PART_%i_CUSTOMPOS", thr_counter, part_counter);
				ConfigCheck(cbuf, Config_idx);
				if (!oapiReadItem_bool(fh, cbuf, custompos)) { custompos = false; }
				if (custompos) {
					sprintf(cbuf, "THR_%i_PART_%i_POS", thr_counter, part_counter);
					ConfigCheck(cbuf, Config_idx);
					oapiReadItem_vec(fh, cbuf, pos);
				}
				//AddThrParticle(thidx, pss_index, custompos, pos);
				Definitions::ParticleDefs pd;
				pd.pss_int = pss_int;
				pd.custompos = custompos;
				pd.pos = pos;
				d.Particles.push_back(pd);
				part_counter++;
				sprintf(cbuf, "THR_%i_PART_%i_ID", thr_counter, part_counter);
				ConfigCheck(cbuf, Config_idx);
			}
		}
		Defs.push_back(d);
		thr_counter++;
		sprintf(cbuf, "THR_%i_ID", thr_counter);
		ConfigCheck(cbuf, Config_idx);

	}
}
void ThrusterSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------THRUSTERS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < Defs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "THR_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "THR_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "THR_%i_POS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].pos);
		sprintf(cbuf, "THR_%i_DIR", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].dir);
		sprintf(cbuf, "THR_%i_MAX0", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].max0);
		sprintf(cbuf, "THR_%i_ISP0", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].isp0);
		sprintf(cbuf, "THR_%i_ISPREF", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].ispref);
		sprintf(cbuf, "THR_%i_PREF", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].pref);
		sprintf(cbuf, "THR_%i_TANK", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].tank);
		sprintf(cbuf, "THR_%i_HASEXHAUST", i);
		ConfigCheck(cbuf, Config_idx);
		bool hasexh = Defs[i].hasexhaust;
		oapiWriteItem_bool(fh, cbuf, hasexh);
		if (hasexh) {
			for (UINT j = 0; j < Defs[i].Exhausts.size(); j++) {
				sprintf(cbuf, "THR_%i_EX_%i_ID", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_int(fh, cbuf, j);
				sprintf(cbuf, "THR_%i_EX_%i_LSIZE", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_float(fh, cbuf, Defs[i].Exhausts[j].lsize);
				sprintf(cbuf, "THR_%i_EX_%i_WSIZE", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_float(fh, cbuf, Defs[i].Exhausts[j].wsize);
				sprintf(cbuf, "THR_%i_EX_%i_EXTEX", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_int(fh, cbuf, Defs[i].Exhausts[j].extex);
				sprintf(cbuf, "THR_%i_EX_%i_CUSTOMPOSDIR", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_bool(fh, cbuf, Defs[i].Exhausts[j].customposdir);
				if (Defs[i].Exhausts[j].customposdir) {
					sprintf(cbuf, "THR_%i_EX_%i_POS", i, j);
					ConfigCheck(cbuf, Config_idx);
					oapiWriteItem_vec(fh, cbuf, Defs[i].Exhausts[j].pos);
					sprintf(cbuf, "THR_%i_EX_%i_DIR", i, j);
					ConfigCheck(cbuf, Config_idx);
					oapiWriteItem_vec(fh, cbuf, Defs[i].Exhausts[j].dir);
				}
			}
		}
		sprintf(cbuf, "THR_%i_HASPARTICLES", i);
		ConfigCheck(cbuf, Config_idx);
		bool haspart = Defs[i].hasparticle;
		oapiWriteItem_bool(fh, cbuf, haspart);
		if (haspart) {
			for (UINT j = 0; j < Defs[i].Particles.size(); j++) {
				sprintf(cbuf, "THR_%i_PART_%i_ID", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_int(fh, cbuf, j);
				sprintf(cbuf, "THR_%i_PART_%i_PSSIDX", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_int(fh, cbuf, Defs[i].Particles[j].pss_int);
				sprintf(cbuf, "THR_%i_PART_%i_CUSTOMPOS", i, j);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_bool(fh, cbuf, Defs[i].Particles[j].custompos);
				if (Defs[i].Particles[j].custompos) {
					sprintf(cbuf, "THR_%i_PART_%i_POS", i, j);
					ConfigCheck(cbuf, Config_idx);
					oapiWriteItem_vec(fh, cbuf, Defs[i].Particles[j].pos);
				}
			}
		}

		oapiWriteLine(fh, " ");
	}

}
void ThrusterSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		PROPELLANT_HANDLE ph = NULL;
		if (Defs[i].tank != -1) {
			ph = VB1->PrpMng->GetTankPH(Defs[i].tank);
		}
		UINT thidx = ThrMng->AddThrDef(Defs[i].name, Defs[i].pos, Defs[i].dir, Defs[i].max0, ph, Defs[i].isp0, Defs[i].ispref, Defs[i].pref);
		for (UINT j = 0; j < Defs[i].Exhausts.size(); j++) {
			SURFHANDLE tex = Defs[i].Exhausts[j].extex == -1 ? NULL : VB1->ExTMng->GetExTexSurf(Defs[i].Exhausts[j].extex);
			ThrMng->AddThrExhaust(thidx, Defs[i].Exhausts[j].lsize, Defs[i].Exhausts[j].wsize, tex, Defs[i].Exhausts[j].customposdir, Defs[i].Exhausts[j].pos, Defs[i].Exhausts[j].dir);
		}
		for (UINT j = 0; j < Defs[i].Particles.size(); j++) {
			ThrMng->AddThrParticle(thidx, Defs[i].Particles[j].pss_int, Defs[i].Particles[j].custompos, Defs[i].Particles[j].pos);
		}
	}
}
void ThrusterSection::UpdateSection() {
	Defs.clear();
	for (DWORD i = 0; i < ThrMng->GetThrCount(); i++) {
		Definitions d = Definitions();
		d.name = ThrMng->GetThrName(i);
		d.pos = ThrMng->GetThrPos(i);
		d.dir = ThrMng->GetThrDir(i);
		d.max0 = ThrMng->GetThrMax0(i);
		d.isp0 = ThrMng->GetThrIsp0(i);
		d.ispref = ThrMng->GetThrIspRef(i);
		d.pref = ThrMng->GetThrPRef(i);
		d.tank = VB1->PrpMng->GetPrpIdx(ThrMng->GetThrPh(i));
		d.hasexhaust = ThrMng->HasThrExhausts(i);
		if (d.hasexhaust) {
			for (UINT j = 0; j < ThrMng->GetThrExCount(i); j++) {
				SURFHANDLE tex;
				Definitions::ExhaustDefs ed;
				ThrMng->GetThrExParams(i, j, ed.lsize, ed.wsize, tex, ed.customposdir, ed.pos, ed.dir);
				ed.extex = VB1->ExTMng->GetExTexIdx(tex);
				d.Exhausts.push_back(ed);
			}	
		}
		d.hasparticle = ThrMng->HasThrParticles(i);
		if (d.hasparticle) {
			for (UINT j = 0; j < ThrMng->GetThrParticlesCount(i); j++) {
				Definitions::ParticleDefs pd;
				UINT pss_idx;// = (UINT)pd.pss_int;
				ThrMng->GetThrParticleParams(i, j, pss_idx, pd.custompos, pd.pos);
				pd.pss_int = (int)pss_idx;
				d.Particles.push_back(pd);
			}
		}
		Defs.push_back(d);
	}
}
void ThrusterSection::ManagerClear() {
	ThrMng->Clear();
}
VBVector<ThrusterSection::Definitions> ThrusterSection::GetSection() {
	return Defs;
}



ThrusterGroupSection::ThrusterGroupSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	VBVector<UINT>nonum;
	nonum.clear();
	Defined[THGROUP_MAIN] = false;
	Thrusters[THGROUP_MAIN] = nonum;
	Defined[THGROUP_RETRO] = false;
	Thrusters[THGROUP_RETRO] = nonum;
	Defined[THGROUP_HOVER] = false;
	Thrusters[THGROUP_HOVER] = nonum;
	Defined[THGROUP_ATT_PITCHUP] = false;
	Thrusters[THGROUP_ATT_PITCHUP] = nonum;
	Defined[THGROUP_ATT_PITCHDOWN] = false;
	Thrusters[THGROUP_ATT_PITCHDOWN] = nonum;
	Defined[THGROUP_ATT_YAWLEFT] = false;
	Thrusters[THGROUP_ATT_YAWLEFT] = nonum;
	Defined[THGROUP_ATT_YAWRIGHT] = false;
	Thrusters[THGROUP_ATT_YAWRIGHT] = nonum;
	Defined[THGROUP_ATT_BANKLEFT] = false;
	Thrusters[THGROUP_ATT_BANKLEFT] = nonum;
	Defined[THGROUP_ATT_BANKRIGHT] = false;
	Thrusters[THGROUP_ATT_BANKRIGHT] = nonum;
	Defined[THGROUP_ATT_RIGHT] = false;
	Thrusters[THGROUP_ATT_RIGHT] = nonum;
	Defined[THGROUP_ATT_LEFT] = false;
	Thrusters[THGROUP_ATT_LEFT] = nonum;
	Defined[THGROUP_ATT_UP] = false;
	Thrusters[THGROUP_ATT_UP] = nonum;
	Defined[THGROUP_ATT_DOWN] = false;
	Thrusters[THGROUP_ATT_DOWN] = nonum;
	Defined[THGROUP_ATT_FORWARD] = false;
	Thrusters[THGROUP_ATT_FORWARD] = nonum;
	Defined[THGROUP_ATT_BACK] = false;
	Thrusters[THGROUP_ATT_BACK] = nonum;

	ThrGrpMng = VB1->ThrGrpMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
}
ThrusterGroupSection::~ThrusterGroupSection() {
	ThrGrpMng = NULL;
}
void ThrusterGroupSection::ParseSection(FILEHANDLE fh) {
	bool defined;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "THGROUP_MAIN");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_MAIN] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_MAIN_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_MAIN] = idx;
	}

	sprintf(cbuf, "THGROUP_RETRO");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_RETRO] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_RETRO_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_RETRO] = idx;
	}

	sprintf(cbuf, "THGROUP_HOVER");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_HOVER] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_HOVER_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_HOVER] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_PITCHUP");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_PITCHUP] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_PITCHUP_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_PITCHUP] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_PITCHDOWN");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_PITCHDOWN] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_PITCHDOWN_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_PITCHDOWN] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_YAWLEFT");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_YAWLEFT] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_YAWLEFT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_YAWLEFT] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_YAWRIGHT");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_YAWRIGHT] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_YAWRIGHT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_YAWRIGHT] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_BANKLEFT");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_BANKLEFT] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_BANKLEFT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_BANKLEFT] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_BANKRIGHT");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_BANKRIGHT] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_BANKRIGHT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_BANKRIGHT] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_RIGHT");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_RIGHT] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_RIGHT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_RIGHT] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_LEFT");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_LEFT] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_LEFT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_LEFT] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_UP");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_UP] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_UP_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_UP] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_DOWN");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_DOWN] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_DOWN_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_DOWN] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_FORWARD");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_FORWARD] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_FORWARD_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_FORWARD] = idx;
	}

	sprintf(cbuf, "THGROUP_ATT_BACK");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, defined)) {
		defined = false;
	}
	Defined[THGROUP_ATT_BACK] = defined;
	if (defined) {
		char ccbuf[256] = { '\0' };
		sprintf(cbuf, "THGROUP_ATT_BACK_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, ccbuf);
		string idx_s(ccbuf);
		VBVector<UINT> idx = VB1->readVectorUINT(idx_s);
		Thrusters[THGROUP_ATT_BACK] = idx;
	}

}
void ThrusterGroupSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, ";<-------------------------THRUSTER GROUPS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	char ccbuf[256] = { '\0' };
	string thr_s;
	thr_s.clear();
	char cbuf[256] = { '\0' };

	if (Defined[THGROUP_MAIN]) {
		sprintf(cbuf, "THGROUP_MAIN");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_MAIN]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_MAIN]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_MAIN_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_RETRO]) {
		sprintf(cbuf, "THGROUP_RETRO");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_RETRO]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_RETRO]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_RETRO_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_HOVER]) {
		sprintf(cbuf, "THGROUP_HOVER");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_HOVER]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_HOVER]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_HOVER_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_PITCHUP]) {
		sprintf(cbuf, "THGROUP_ATT_PITCHUP");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_PITCHUP]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_PITCHUP]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_PITCHUP_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_PITCHDOWN]) {
		sprintf(cbuf, "THGROUP_ATT_PITCHDOWN");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_PITCHDOWN]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_PITCHDOWN]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_PITCHDOWN_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_YAWLEFT]) {
		sprintf(cbuf, "THGROUP_ATT_YAWLEFT");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_YAWLEFT]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_YAWLEFT]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_YAWLEFT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_YAWRIGHT]) {
		sprintf(cbuf, "THGROUP_ATT_YAWRIGHT");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_YAWRIGHT]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_YAWRIGHT]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_YAWRIGHT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_BANKLEFT]) {
		sprintf(cbuf, "THGROUP_ATT_BANKLEFT");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_BANKLEFT]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_BANKLEFT]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_BANKLEFT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_BANKRIGHT]) {
		sprintf(cbuf, "THGROUP_ATT_BANKRIGHT");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_BANKRIGHT]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_BANKRIGHT]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_BANKRIGHT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_RIGHT]) {
		sprintf(cbuf, "THGROUP_ATT_RIGHT");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_RIGHT]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_RIGHT]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_RIGHT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_LEFT]) {
		sprintf(cbuf, "THGROUP_ATT_LEFT");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_LEFT]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_LEFT]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_LEFT_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_UP]) {
		sprintf(cbuf, "THGROUP_ATT_UP");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_UP]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_UP]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_UP_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_DOWN]) {
		sprintf(cbuf, "THGROUP_ATT_DOWN");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_DOWN]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_DOWN]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_DOWN_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_FORWARD]) {
		sprintf(cbuf, "THGROUP_ATT_FORWARD");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_FORWARD]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_FORWARD]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_FORWARD_THRUSTERS");
		oapiWriteItem_string(fh, cbuf, ccbuf);
		ConfigCheck(cbuf, Config_idx);
		thr_s.clear();
	}

	if (Defined[THGROUP_ATT_BACK]) {
		sprintf(cbuf, "THGROUP_ATT_BACK");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, Defined[THGROUP_ATT_BACK]);
		thr_s = VB1->WriteVectorUINT(Thrusters[THGROUP_ATT_BACK]);
		sprintf(ccbuf, "%s", thr_s.c_str());
		sprintf(cbuf, "THGROUP_ATT_BACK_THRUSTERS");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, ccbuf);
		thr_s.clear();
	}
}
void ThrusterGroupSection::ApplySection() {
	Section::ApplySection();
	map<THGROUP_TYPE, bool>::iterator it;
	//UINT counter = 0;
	for (it = Defined.begin(); it != Defined.end(); it++) {
		//LogV("Counter:%i", counter);
		if (it->second) {
			//LogV("true:%i",counter);
			VBVector<THRUSTER_HANDLE>thv = ThrGrpMng->GetThrustersfromIdx(Thrusters[it->first]);
			ThrGrpMng->DefineGroup(it->first, thv);
		}
		//counter++;
	}
	return;
}
void ThrusterGroupSection::UpdateSection() {
	Defined = ThrGrpMng->Defined;
	map<THGROUP_TYPE, bool>::iterator it;
	VBVector<UINT>indexes;
	indexes.clear();
	for (it = Defined.begin(); it != Defined.end(); it++) {
		if (it->second) {
			indexes.clear();
			VBVector<THRUSTER_HANDLE>thrusters;
			thrusters = ThrGrpMng->GetThrusters(it->first);
			for (UINT i = 0; i < thrusters.size(); i++) {
				UINT idx = VB1->ThrMng->GetThrIdx(thrusters[i]);
				if (idx != (UINT)-1) {
					indexes.push_back(idx);
				}
			}
			
		}
		else {
			indexes.clear();
		}
		Thrusters[it->first] = indexes;
	}

}
void ThrusterGroupSection::ManagerClear() {
	ThrGrpMng->ResetDefine();
}
void ThrusterGroupSection::GetSection(map<THGROUP_TYPE, bool> &Def, map<THGROUP_TYPE, VBVector<UINT> > &thrusters) {
	Def = Defined;
	thrusters = Thrusters;
	return;
}



TouchDownPointSection::TouchDownPointSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Set1.clear();
	Set2.clear();
	SecondSetEnabled = false;
	changeoveranim = -1;
	TdpMng = VB1->TdpMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
TouchDownPointSection::~TouchDownPointSection() { TdpMng = NULL; }

void TouchDownPointSection::ParseSection(FILEHANDLE fh) {
	for (UINT set = 1; set < 3; set++) {
		char cbuf[256] = { '\0' };
		UINT point_counter = 0;
		VECTOR3 pos;
		sprintf(cbuf, "TDP_%i_%i_POS", set, point_counter);
		ConfigCheck(cbuf, Config_idx);
		while (oapiReadItem_vec(fh, cbuf, pos)) {
			double damping, stiffness, mu, mu_lng;
			sprintf(cbuf, "TDP_%i_%i_STIFFNESS", set, point_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, stiffness);
			sprintf(cbuf, "TDP_%i_%i_DAMPING", set, point_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, damping);
			sprintf(cbuf, "TDP_%i_%i_MU", set, point_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, mu);
			sprintf(cbuf, "TDP_%i_%i_MULNG", set, point_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, mu_lng);
			Definitions d = Definitions();
			d.pos = pos;
			d.stiffness = stiffness;
			d.damping = damping;
			d.mu = mu;
			d.mu_lng = mu_lng;
			if (set == 1) {
				Set1.push_back(d);
			}
			else if (set == 2) {
				Set2.push_back(d);
			}

			point_counter++;
			sprintf(cbuf, "TDP_%i_%i_POS", set, point_counter);
			ConfigCheck(cbuf, Config_idx);
		}

	}
	//bool SecondSetEnabled;
	//int changeoveranim;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "TDP_SECONDSET_ENABLED");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh, cbuf, SecondSetEnabled)) { SecondSetEnabled = false; }
	if (SecondSetEnabled) {
		sprintf(cbuf, "TDP_CHANGEOVER_ANIM");
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_int(fh, cbuf, changeoveranim)) { changeoveranim = -1; };
	}
	else {
		changeoveranim = -1;
	}

	
}
void TouchDownPointSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------TOUCHDOWNPOINTS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT set = 1; set < 3; set++) {
		if (set == 2) {
			char cbuf[256] = { '\0' };
			//bool set2Enabled = IsSecondSetEnabled();
			if (SecondSetEnabled) {
				sprintf(cbuf, "TDP_SECONDSET_ENABLED");
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_bool(fh, cbuf, SecondSetEnabled);
				sprintf(cbuf, "TDP_CHANGEOVER_ANIM");
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_int(fh, cbuf, changeoveranim);
			}

		}
		UINT npoints = set == 1 ? Set1.size() : Set2.size();
		for (UINT i = 0; i < npoints; i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "TDP_%i_%i_POS", set, i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, set == 1 ? Set1[i].pos : Set2[i].pos);
			sprintf(cbuf, "TDP_%i_%i_STIFFNESS", set, i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, set == 1 ? Set1[i].stiffness : Set2[i].stiffness);
			sprintf(cbuf, "TDP_%i_%i_DAMPING", set, i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, set == 1 ? Set1[i].damping : Set2[i].damping);
			sprintf(cbuf, "TDP_%i_%i_MU", set, i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, set == 1 ? Set1[i].mu : Set2[i].mu);
			sprintf(cbuf, "TDP_%i_%i_MULNG", set, i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, set == 1 ? Set1[i].mu_lng : Set2[i].mu_lng);
			oapiWriteLine(fh, " ");
		}


	}
}
void TouchDownPointSection::ApplySection() {
	Section::ApplySection();
	
	for (UINT i = 0; i < Set1.size(); i++) {
		TOUCHDOWNVTX tdvtx;
		tdvtx.pos = Set1[i].pos;
		tdvtx.stiffness = Set1[i].stiffness;
		tdvtx.damping = Set1[i].damping;
		tdvtx.mu = Set1[i].mu;
		tdvtx.mu_lng = Set1[i].mu_lng;
		TdpMng->AddPoint(1, tdvtx);
	}
	for (UINT i = 0; i < Set2.size(); i++) {
		TOUCHDOWNVTX tdvtx;
		tdvtx.pos = Set2[i].pos;
		tdvtx.stiffness = Set2[i].stiffness;
		tdvtx.damping = Set2[i].damping;
		tdvtx.mu = Set2[i].mu;
		tdvtx.mu_lng = Set2[i].mu_lng;
		TdpMng->AddPoint(2, tdvtx);
	}

	if (Set1.size() > 3) {
		TdpMng->SetCurrentSet(1);
	}
	TdpMng->EnableSecondSet(SecondSetEnabled);
	if (changeoveranim > 0) {
		TdpMng->SetChangeOverAnimation(changeoveranim);
	}
	return;
}
void TouchDownPointSection::UpdateSection() {
	Set1.clear();
	Set2.clear();
	changeoveranim = TdpMng->GetChangeOverAnimation();
	SecondSetEnabled = TdpMng->IsSecondSetEnabled();
	for (UINT set = 1; set < 3; set++) {
		for (UINT i = 0; i < TdpMng->GetPointsCount(set); i++) {
			Definitions d = Definitions();
			d.pos = TdpMng->GetPointPos(set, i);
			d.stiffness = TdpMng->GetPointStiffnes(set, i);
			d.damping = TdpMng->GetPointDamping(set, i);
			d.mu = TdpMng->GetPointMu(set, i);
			d.mu_lng = TdpMng->GetPointMuLng(set, i);
			set == 1 ? Set1.push_back(d) : Set2.push_back(d);
		}
		
	}
}
void TouchDownPointSection::ManagerClear() {
	TdpMng->Clear();
}
void TouchDownPointSection::GetSection(VBVector<TouchDownPointSection::Definitions> &set1, VBVector<TouchDownPointSection::Definitions>&set2) {
	set1 = Set1;
	set2 = Set2;
}


AirfoilSection::AirfoilSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) : Section(VB1, config, cfg) {
	Defs.clear();
	AirfoilMng = VB1->AirfoilMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
AirfoilSection::~AirfoilSection() { AirfoilMng = NULL; }
void AirfoilSection::ParseSection(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	char namebuf[256] = { '\0' };
	int id;
	UINT airfoil_counter = 0;
	sprintf(cbuf, "AIRFOIL_%i_ID", airfoil_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		VECTOR3 ref;
		double c, S, A;
		sprintf(cbuf, "AIRFOIL_%i_NAME", airfoil_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "AIRFOIL_%i_ORIENTATION", airfoil_counter);
		ConfigCheck(cbuf, Config_idx);
		int al;
		oapiReadItem_int(fh, cbuf, al);
		AIRFOIL_ORIENTATION align = (AIRFOIL_ORIENTATION)al;
		sprintf(cbuf, "AIRFOIL_%i_REF", airfoil_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "AIRFOIL_%i_C", airfoil_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, c);
		sprintf(cbuf, "AIRFOIL_%i_S", airfoil_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, S);
		sprintf(cbuf, "AIRFOIL_%i_A", airfoil_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, A);
		Definitions d = Definitions();
		d.name = name;
		d.align = al;
		d.ref = ref;
		d.c = c;
		d.S = S;
		d.A = A;

		//UINT index = CreateAirfoilDef(name, align, ref, c, S, A);
		UINT point_counter = 0;
		sprintf(cbuf, "AIRFOIL_%i_POINT_%i_AOA", airfoil_counter, point_counter);
		ConfigCheck(cbuf, Config_idx);
		double aoa, cl, cm;
		while (oapiReadItem_float(fh, cbuf, aoa)) {
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CL", airfoil_counter, point_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, cl);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CM", airfoil_counter, point_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, cm);
			aoa *= RAD;
			//AddPointAirfoiDef(index, aoa, cl, cm);
			Definitions::LC_Point P;
			P.aoa = aoa;
			P.cl = cl;
			P.cm = cm;
			d.Points.push_back(P);
			point_counter++;
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_AOA", airfoil_counter, point_counter);
			ConfigCheck(cbuf, Config_idx);
		}
		Defs.push_back(d);
		airfoil_counter++;
		sprintf(cbuf, "AIRFOIL_%i_ID", airfoil_counter);
		ConfigCheck(cbuf, Config_idx);
	}
}
void AirfoilSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------AIRFOILS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < Defs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "AIRFOIL_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "AIRFOIL_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
//		VECTOR3 ref;
	//	double c, S, A;
		//AIRFOIL_ORIENTATION align = GetAirfoilDefOrientation(i);
		//GetAirfoilDefParams(i, ref, c, S, A);
		sprintf(cbuf, "AIRFOIL_%i_ORIENTATION", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].align);
		sprintf(cbuf, "AIRFOIL_%i_REF", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].ref);
		sprintf(cbuf, "AIRFOIL_%i_C", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].c);
		sprintf(cbuf, "AIRFOIL_%i_S", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].S);
		sprintf(cbuf, "AIRFOIL_%i_A", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].A);
		for (UINT j = 0; j < Defs[i].Points.size(); j++) {
			//double aoa, cl, cm;
			//GetAirfoilDefPoint(i, j, aoa, cl, cm);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_AOA", i, j);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, Defs[i].Points[j].aoa*DEG);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CL", i, j);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, Defs[i].Points[j].cl);
			sprintf(cbuf, "AIRFOIL_%i_POINT_%i_CM", i, j);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, Defs[i].Points[j].cm);
		}
		oapiWriteLine(fh, " ");
	}

}
void AirfoilSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		AIRFOIL_ORIENTATION align = (AIRFOIL_ORIENTATION)Defs[i].align;
		UINT index = AirfoilMng->CreateAirfoilDef(Defs[i].name, align, Defs[i].ref, Defs[i].c, Defs[i].S, Defs[i].A);
		for (UINT j = 0; j < Defs[i].Points.size(); j++) {
			AirfoilMng->AddPointAirfoiDef(index, Defs[i].Points[j].aoa, Defs[i].Points[j].cl, Defs[i].Points[j].cm);
		}
	}
}
void AirfoilSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < AirfoilMng->GetAirfoilDefCount(); i++) {
		Definitions d = Definitions();
		d.name = AirfoilMng->GetAirfoilDefName(i);
		d.align = (int)AirfoilMng->GetAirfoilDefOrientation(i);
		double c, S, A;
		VECTOR3 ref;
		AirfoilMng->GetAirfoilDefParams(i, ref, c, S, A);
		d.ref = ref;
		d.c = c;
		d.S = S;
		d.A = A;
		for (UINT j = 0; j < AirfoilMng->GetAirfoilDefPointsCount(i); j++) {
			double aoa, cl, cm;
			AirfoilMng->GetAirfoilDefPoint(i, j, aoa, cl, cm);
			Definitions::LC_Point P;
			P.aoa = aoa;
			P.cl = cl;
			P.cm = cm;
			d.Points.push_back(P);
		}
		Defs.push_back(d);
	}
}
void AirfoilSection::ManagerClear() {
	AirfoilMng->Clear();
}
VBVector<AirfoilSection::Definitions> AirfoilSection::GetSection() {
	return Defs;
}



CtrlSurfaceSection::CtrlSurfaceSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) : Section(VB1,config,cfg) {
	Defs.clear();
	CtrSurfMng = VB1->CtrSurfMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
CtrlSurfaceSection::~CtrlSurfaceSection() { CtrSurfMng = NULL; }

void CtrlSurfaceSection::ParseSection(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	UINT ctr_counter = 0;
	int id;
	sprintf(cbuf, "CTRL_SURFACES_%i_ID", ctr_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		AIRCTRL_TYPE type;
		double area, dcl, delay;
		int axis;
		//UINT anim;
		VECTOR3 ref;
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "CTRL_SURFACES_%i_NAME", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "CTRL_SURFACES_%i_TYPE", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		int tp;
		oapiReadItem_int(fh, cbuf, tp);
		type = (AIRCTRL_TYPE)tp;
		sprintf(cbuf, "CTRL_SURFACES_%i_AREA", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, area);
		sprintf(cbuf, "CTRL_SURFACES_%i_DCL", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, dcl);
		sprintf(cbuf, "CTRL_SURFACES_%i_REF", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "CTRL_SURFACES_%i_AXIS", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, axis);
		sprintf(cbuf, "CTRL_SURFACES_%i_DELAY", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, delay);
		sprintf(cbuf, "CTRL_SURFACES_%i_ANIM", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
		int anim;
		oapiReadItem_int(fh, cbuf, anim);
		//anim = (UINT)an;
		Definitions d = Definitions();
		d.name = name;
		d.type = tp;
		d.area = area;
		d.dcl = dcl;
		d.ref = ref;
		d.axis = axis;
		d.delay = delay;
		d.anim = anim;
		Defs.push_back(d);
		//UINT index = CreateUndefinedCtrlSurfDef(name, type, area, dcl, ref, axis, delay, anim);
		//DefineCtrlSurface(index);

		ctr_counter++;
		sprintf(cbuf, "CTRL_SURFACES_%i_ID", ctr_counter);
		ConfigCheck(cbuf, Config_idx);
	}
	return;
}
void CtrlSurfaceSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------CONTROL SURFACES DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	
	for (UINT i = 0; i < Defs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "CTRL_SURFACES_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "CTRL_SURFACES_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "CTRL_SURFACES_%i_TYPE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].type);
		sprintf(cbuf, "CTRL_SURFACES_%i_AREA", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].area);
		sprintf(cbuf, "CTRL_SURFACES_%i_DCL", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].dcl);
		sprintf(cbuf, "CTRL_SURFACES_%i_REF", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].ref);
		sprintf(cbuf, "CTRL_SURFACES_%i_AXIS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].axis);
		sprintf(cbuf, "CTRL_SURFACES_%i_DELAY", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].delay);
		sprintf(cbuf, "CTRL_SURFACES_%i_ANIM", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].anim);
		oapiWriteLine(fh, " ");
	}

	return;
}
void CtrlSurfaceSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		AIRCTRL_TYPE tp = (AIRCTRL_TYPE)Defs[i].type;
		UINT index = CtrSurfMng->CreateUndefinedCtrlSurfDef(Defs[i].name, tp, Defs[i].area, Defs[i].dcl, Defs[i].ref, Defs[i].axis, Defs[i].delay, Defs[i].anim);
		CtrSurfMng->DefineCtrlSurface(index);
	}
}
void CtrlSurfaceSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < CtrSurfMng->CtrlSurfDefCount(); i++) {
		if (!CtrSurfMng->IsCtrlSurfDefDefined(i)) { continue; }
		AIRCTRL_TYPE type;
		double area, dcl, delay;
		int axis;
		UINT anim;
		VECTOR3 ref;
		string name;
		name = CtrSurfMng->GetCtrlSurfDefName(i);
		CtrSurfMng->GetCtrlSurfDefParams(i, type, area, dcl, ref, axis, delay, anim);
		Definitions d = Definitions();
		d.name = name;
		d.ref = ref;
		d.area = area;
		d.dcl = dcl;
		d.axis = axis;
		d.delay = delay;
		d.anim = anim;
		d.type = (int)type;
		Defs.push_back(d);
	}
	
}
void CtrlSurfaceSection::ManagerClear() {
	CtrSurfMng->Clear();
}
VBVector<CtrlSurfaceSection::Definitions> CtrlSurfaceSection::GetSection() {
	return Defs;
}

CameraSection::CameraSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg) : Section(VB1, config, cfg) {
	Defs.clear();
	CamMng = VB1->CamMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
CameraSection::~CameraSection() { CamMng = NULL; }
void CameraSection::ParseSection(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	UINT cam_counter = 0;
	sprintf(cbuf, "CAM_%i_ID", cam_counter);
	ConfigCheck(cbuf, Config_idx);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		VECTOR3 pos, dir;
		double tilt;
		sprintf(cbuf, "CAM_%i_NAME", cam_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "CAM_%i_POS", cam_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "CAM_%i_DIR", cam_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "CAM_%i_TILT", cam_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, tilt);
		tilt *= RAD;
		//AddCamDef(name, pos, dir, tilt);
		Definitions d = Definitions();
		d.name = name;
		d.pos = pos;
		d.dir = dir;
		d.tilt = tilt;
		Defs.push_back(d);
		cam_counter++;
		sprintf(cbuf, "CAM_%i_ID", cam_counter);
		ConfigCheck(cbuf, Config_idx);
	}
}
void CameraSection::WriteSection(FILEHANDLE fh){
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------CAMERA DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < Defs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "CAM_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "CAM_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "CAM_%i_POS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].pos);
		sprintf(cbuf, "CAM_%i_DIR", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].dir);
		sprintf(cbuf, "CAM_%i_TILT", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].tilt*DEG);
		oapiWriteLine(fh, " ");
	}
}
void CameraSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		CamMng->AddCamDef(Defs[i].name, Defs[i].pos, Defs[i].dir, Defs[i].tilt);
	}
}
void CameraSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < CamMng->GetCamCount(); i++) {
		Definitions d = Definitions();
		d.name = CamMng->GetCamName(i);
		d.pos = CamMng->GetCamPos(i);
		d.dir = CamMng->GetCamDir(i);
		d.tilt = CamMng->GetCamTilt(i);
		Defs.push_back(d);
	}
}
void CameraSection::ManagerClear() {
	CamMng->Clear();
}
VBVector<CameraSection::Definitions> CameraSection::GetSection() {
	return Defs;
}


VCSection::VCSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	PosDefs.clear();
	MFDDefs.clear();
	HUDDef = HUDDefinition();
	VCMng = VB1->VCMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
VCSection::~VCSection() { VCMng = NULL; }
void VCSection::ParseSection(FILEHANDLE fh){
	UINT position_counter = 0;
	char cbuf[256] = { '\0' };
	int id;
	sprintf(cbuf, "VC_POS_%i_ID", position_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		sprintf(cbuf, "VC_POS_%i_NAME", position_counter);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		VECTOR3 ofs, dir;
		sprintf(cbuf, "VC_POS_%i_OFS", position_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, ofs);
		sprintf(cbuf, "VC_POS_%i_DIR", position_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, dir);
		PosDefinitions pd = PosDefinitions();
		pd.name = name;
		pd.pos = ofs;
		pd.dir = dir;
		PosDefs.push_back(pd);
	//	AddPosition(name, ofs, dir);
		position_counter++;
		sprintf(cbuf, "VC_POS_%i_ID", position_counter);
		ConfigCheck(cbuf, Config_idx);
	}
	UINT MFD_counter = 0;
	int mfd_id;
	sprintf(cbuf, "VC_MFD_%i_ID", MFD_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, mfd_id)) {
		int mesh, group;
		bool wPwrBtns, wColsBtns;
		VECTOR3 pwr_btn, mnu_btn, TopLeftBtn, BottomLeftBtn, TopRightBtn, BottomRightBtn;

		sprintf(cbuf, "VC_MFD_%i_MESH", MFD_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, mesh);
		sprintf(cbuf, "VC_MFD_%i_GROUP", MFD_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, group);
		sprintf(cbuf, "VC_MFD_%i_WPWR", MFD_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, wPwrBtns)) { wPwrBtns = false; }
		if (wPwrBtns) {
			sprintf(cbuf, "VC_MFD_%i_PWR0", MFD_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, pwr_btn);
			sprintf(cbuf, "VC_MFD_%i_PWR2", MFD_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, mnu_btn);
		}
		sprintf(cbuf, "VC_MFD_%i_WCOL", MFD_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, wColsBtns)) { wColsBtns = false; }
		if (wColsBtns) {
			sprintf(cbuf, "VC_MFD_%i_COLTL", MFD_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, TopLeftBtn);
			sprintf(cbuf, "VC_MFD_%i_COLBL", MFD_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, BottomLeftBtn);
			sprintf(cbuf, "VC_MFD_%i_COLTR", MFD_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, TopRightBtn);
			sprintf(cbuf, "VC_MFD_%i_COLBR", MFD_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, BottomRightBtn);

		}
	//	UINT index = AddMFD(mesh, group, wPwrBtns, pwr_btn, mnu_btn, wColsBtns, TopLeftBtn, BottomLeftBtn, TopRightBtn, BottomRightBtn);
		MFDDefinitions md = MFDDefinitions();
		md.mesh = mesh;
		md.group = group;
		md.wpwr = wPwrBtns;
		md.pwr0 = pwr_btn;
		md.pwr2 = mnu_btn;
		md.wcol = wColsBtns;
		md.coltl = TopLeftBtn;
		md.colbl = BottomLeftBtn;
		md.coltr = TopRightBtn;
		md.colbr = BottomRightBtn;
		MFDDefs.push_back(md);
		MFD_counter++;
		sprintf(cbuf, "VC_MFD_%i_ID", MFD_counter);
		ConfigCheck(cbuf, Config_idx);
	}
	bool Hud;
	int mesh, group;
	VECTOR3 hudcnt;
	double size;
	sprintf(cbuf, "VC_HUD");
	ConfigCheck(cbuf, Config_idx);
	if (!oapiReadItem_bool(fh,cbuf , Hud)) { Hud = false; }
	HUDDef.wHud = Hud;
	if (Hud) {
		//EnableHUD(true);
		sprintf(cbuf, "VC_HUD_MESH");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, mesh);
		sprintf(cbuf, "VC_HUD_GROUP");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, group);
		sprintf(cbuf, "VC_HUD_CNT");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, hudcnt);
		sprintf(cbuf, "VC_HUD_SIZE");
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, size);
		//SetHUDParams(mesh, group, hudcnt, size);
		HUDDef.mesh = mesh;
		HUDDef.group = group;
		HUDDef.hudcnt = hudcnt;
		HUDDef.size = size;
	}
	return;
}
void VCSection::WriteSection(FILEHANDLE fh){
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------VIRTUAL COCKPIT DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < PosDefs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "VC_POS_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "VC_POS_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", PosDefs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "VC_POS_%i_OFS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, PosDefs[i].pos);
		sprintf(cbuf, "VC_POS_%i_DIR", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, PosDefs[i].dir);
		oapiWriteLine(fh, " ");
	}
	for (UINT i = 0; i < MFDDefs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "VC_MFD_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "VC_MFD_%i_MESH", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, MFDDefs[i].mesh);
		sprintf(cbuf, "VC_MFD_%i_GROUP", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, MFDDefs[i].group);
		sprintf(cbuf, "VC_MFD_%i_WPWR", i);
		ConfigCheck(cbuf, Config_idx);
		bool trans = MFDDefs[i].wpwr;
		oapiWriteItem_bool(fh, cbuf, MFDDefs[i].wpwr);
		if (trans) {
			VECTOR3 pwr0, pwr2;
			pwr0 = MFDDefs[i].pwr0;
			pwr2 = MFDDefs[i].pwr2;
			sprintf(cbuf, "VC_MFD_%i_PWR0", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, pwr0);
			sprintf(cbuf, "VC_MFD_%i_PWR2", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, pwr2);
		}
		trans = MFDDefs[i].wcol;
		sprintf(cbuf, "VC_MFD_%i_WCOL", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, trans);
		if (trans) {
			VECTOR3 TopLeft, BottomLeft, TopRight, BottomRight;
			TopLeft = MFDDefs[i].coltl;
			BottomLeft = MFDDefs[i].colbl;
			TopRight = MFDDefs[i].coltr;
			BottomRight = MFDDefs[i].colbr;
			sprintf(cbuf, "VC_MFD_%i_COLTL", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, TopLeft);
			sprintf(cbuf, "VC_MFD_%i_COLBL", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, BottomLeft);
			sprintf(cbuf, "VC_MFD_%i_COLTR", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, TopRight);
			sprintf(cbuf, "VC_MFD_%i_COLBR", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, BottomRight);
		}
		oapiWriteLine(fh, " ");
	}
	bool wHud = HUDDef.wHud;
	if (wHud) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "VC_HUD");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_bool(fh, cbuf, wHud);
		int mesh, group;
		VECTOR3 hudcnt;
		double size;
		mesh = HUDDef.mesh;
		group = HUDDef.group;
		hudcnt = HUDDef.hudcnt;
		size = HUDDef.size;
		//GetHUDParams(mesh, group, hudcnt, size);
		sprintf(cbuf, "VC_HUD_MESH");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, mesh);
		sprintf(cbuf, "VC_HUD_GROUP");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, group);
		sprintf(cbuf, "VC_HUD_CNT");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, hudcnt);
		sprintf(cbuf, "VC_HUD_SIZE");
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, size);
		oapiWriteLine(fh, " ");
	}
	return;

}
void VCSection::ApplySection(){
	Section::ApplySection();
	for (UINT i = 0; i < PosDefs.size(); i++) {
		VCMng->AddPosition(PosDefs[i].name, PosDefs[i].pos, PosDefs[i].dir);
	}
	for (UINT i = 0; i < MFDDefs.size(); i++) {
		VCMng->AddMFD(MFDDefs[i].mesh, MFDDefs[i].group, MFDDefs[i].wpwr, MFDDefs[i].pwr0, MFDDefs[i].pwr2, MFDDefs[i].wcol, MFDDefs[i].coltl, MFDDefs[i].colbl, MFDDefs[i].coltr, MFDDefs[i].colbr);
	}
	VCMng->EnableHUD(HUDDef.wHud);
	if (HUDDef.wHud) {
		VCMng->SetHUDParams(HUDDef.mesh, HUDDef.group, HUDDef.hudcnt, HUDDef.size);
	}
	return;
}
void VCSection::UpdateSection(){
	PosDefs.clear();
	MFDDefs.clear();
	HUDDef = HUDDefinition();
	for (UINT i = 0; i < VCMng->GetPositionCount(); i++) {
		PosDefinitions pd = PosDefinitions();
		pd.name = VCMng->GetPositionName(i);
		pd.pos = VCMng->GetPositionPos(i);
		pd.dir = VCMng->GetPositionDir(i);
		PosDefs.push_back(pd);
	}
	for (UINT i = 0; i < VCMng->GetMFDCount(); i++) {
		MFDDefinitions md = MFDDefinitions();
		md.mesh = VCMng->GetMFDMesh(i);
		md.group = VCMng->GetMFDGroup(i);
		md.wpwr = VCMng->GetMFDHasPwrBtns(i);
		if (md.wpwr) {
			VCMng->GetMFDPwrVectors(i, md.pwr0, md.pwr2);
		}
		md.wcol = VCMng->GetMFDHasColBtns(i);
		if (md.wcol) {
			VCMng->GetMFDColVectors(i, md.coltl, md.colbl, md.coltr, md.colbr);
		}
		MFDDefs.push_back(md);
	}
	HUDDef.wHud = VCMng->IsHUDEnabled();
	if (HUDDef.wHud) {
		DWORD mesh, group;
		VCMng->GetHUDParams(mesh, group, HUDDef.hudcnt, HUDDef.size);
		HUDDef.mesh = (int)mesh;
		HUDDef.group = (int)group;
	}
}
void VCSection::ManagerClear() {
	VCMng->Clear();
}
void VCSection::GetSection(VBVector<VCSection::PosDefinitions> &pd, VBVector<VCSection::MFDDefinitions>&md, VCSection::HUDDefinition &hd) {
	hd = HUDDef;
	pd = PosDefs;
	md = MFDDefs;
	return;
}



LightSection::LightSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg):Section(VB1,config,cfg) {
	BcnDefs.clear();
	LgtDefs.clear();
	LightsMng = VB1->LightsMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
}
LightSection::~LightSection() {
	LightsMng = NULL;
}
void LightSection::ParseSection(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	UINT beacon_counter = 0;
	int id;
	sprintf(cbuf, "BCN_%i_ID", beacon_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "BCN_%i_NAME", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		DWORD shape;
		VECTOR3 pos, col;
		double size, falloff, period, duration, tofs;
		sprintf(cbuf, "BCN_%i_SHAPE", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		int sh;
		oapiReadItem_int(fh, cbuf, sh);
		shape = (DWORD)sh;
		sprintf(cbuf, "BCN_%i_POS", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "BCN_%i_COL", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, col);
		sprintf(cbuf, "BCN_%i_SIZE", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, size);
		sprintf(cbuf, "BCN_%i_FALLOFF", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, falloff);
		sprintf(cbuf, "BCN_%i_PERIOD", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, period);
		sprintf(cbuf, "BCN_%i_DURATION", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, duration);
		sprintf(cbuf, "BCN_%i_TOFS", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, tofs);
		BcnDefinitions bd = BcnDefinitions();
		bd.name = name;
		bd.shape = sh;
		bd.pos = pos;
		bd.col = col;
		bd.size = size;
		bd.falloff = falloff;
		bd.period = period;
		bd.duration = duration;
		bd.tofs = tofs;
		BcnDefs.push_back(bd);
		//AddBeaconDef(name, shape, pos, col, size, falloff, period, duration, tofs);
		beacon_counter++;
		sprintf(cbuf, "BCN_%i_ID", beacon_counter);
		ConfigCheck(cbuf, Config_idx);
	}

	UINT lights_counter = 0;
	for (UINT i = 0; i < 256; i++) { cbuf[i] = '\0'; }
	sprintf(cbuf, "LIGHT_%i_ID", lights_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		sprintf(cbuf, "LIGHT_%i_NAME", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		int type_int;
		LightEmitter::TYPE type;
		VECTOR3 col_d, col_a, col_s, pos;
		COLOUR4 diff, amb, spec;
		double range;
		int vis_int;
		LightEmitter::VISIBILITY vis;
		VECTOR3 attenuation;
		bool hasatt;
		int attachment=-1;
		VECTOR3 direction = _V(0, 0, 0);
		VECTOR3 aperture = _V(0, 0, 0);
		sprintf(cbuf, "LIGHT_%i_TYPE", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, type_int);
		type = (LightEmitter::TYPE)type_int;
		sprintf(cbuf, "LIGHT_%i_DCOL", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, col_d);
		diff.r = col_d.x;
		diff.g = col_d.y;
		diff.b = col_d.z;
		diff.a = 0;
		sprintf(cbuf, "LIGHT_%i_ACOL", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, col_a);
		amb.r = col_a.x;
		amb.g = col_a.y;
		amb.b = col_a.z;
		amb.a = 0;
		sprintf(cbuf, "LIGHT_%i_SCOL", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, col_s);
		spec.r = col_s.x;
		spec.g = col_s.y;
		spec.b = col_s.z;
		spec.a = 0;
		sprintf(cbuf, "LIGHT_%i_POSITION", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "LIGHT_%i_RANGE", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, range);
		sprintf(cbuf, "LIGHT_%i_VISIBILITY", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, vis_int);
		vis = (LightEmitter::VISIBILITY)vis_int;
		sprintf(cbuf, "LIGHT_%i_ATTENUATION", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, attenuation);
		if (type == LightEmitter::TYPE::LT_SPOT) {
			sprintf(cbuf, "LIGHT_%i_DIRECTION", lights_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, direction);
			sprintf(cbuf, "LIGHT_%i_APERTURE", lights_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, aperture);
			aperture *= RAD;

		}
		sprintf(cbuf, "LIGHT_%i_HASATTACHMENT", lights_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, hasatt)) {
			hasatt = false;
		}
		if (hasatt) {
			sprintf(cbuf, "LIGHT_%i_ATTACHMENT", lights_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, attachment);
		}
		LightDefinitions ld = LightDefinitions();
		ld.type = type_int;
		ld.name = name;
		ld.pos = pos;
		ld.visibility = vis_int;
		ld.dir = direction;
		ld.attenuation = attenuation;
		ld.range = range;
		ld.aperture = aperture;
		ld.dcol = col_d;
		ld.scol = col_s;
		ld.acol = col_a;
		ld.hasattachment = hasatt;
		ld.attachment = attachment;
		LgtDefs.push_back(ld);
		//UINT index = AddLightDef(type, name, pos, vis, direction, attenuation, range, aperture.x, aperture.y, diff, spec, amb);
		//if (hasatt) {
		//	SetLightToAttachment(index, true, attachment);
		//}
		lights_counter++;
		sprintf(cbuf, "LIGHT_%i_ID", lights_counter);
		ConfigCheck(cbuf, Config_idx);
	}
}
void LightSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------LIGHTS: BEACONS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < BcnDefs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "BCN_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "BCN_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", BcnDefs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "BCN_%i_SHAPE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, BcnDefs[i].shape);
		sprintf(cbuf, "BCN_%i_POS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, BcnDefs[i].pos);
		sprintf(cbuf, "BCN_%i_COL", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, BcnDefs[i].col);
		sprintf(cbuf, "BCN_%i_SIZE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, BcnDefs[i].size);
		sprintf(cbuf, "BCN_%i_FALLOFF", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, BcnDefs[i].falloff);
		sprintf(cbuf, "BCN_%i_PERIOD", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, BcnDefs[i].period);
		sprintf(cbuf, "BCN_%i_DURATION", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, BcnDefs[i].duration);
		sprintf(cbuf, "BCN_%i_TOFS", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, BcnDefs[i].tofs);
		oapiWriteLine(fh, " ");
	}

	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------LIGHTS: LIGHT EMITTERS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	LogV("lightsdefs size:%i", LgtDefs.size());
	for (UINT i = 0; i < LgtDefs.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "LIGHT_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "LIGHT_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", LgtDefs[i].name.c_str());
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "LIGHT_%i_TYPE", i);
		ConfigCheck(cbuf, Config_idx);
		//int type = (int)GetLightType(i);
		oapiWriteItem_int(fh, cbuf, LgtDefs[i].type);
		sprintf(cbuf, "LIGHT_%i_DCOL", i);
		ConfigCheck(cbuf, Config_idx);
		/*COLOUR4 col_d, col_a, col_s;
		GetLightColours(i, col_d, col_a, col_s);
		VECTOR3 col_dv, col_av, col_sv;
		col_dv.x = col_d.r;
		col_dv.y = col_d.g;
		col_dv.z = col_d.b;
		col_av.x = col_a.r;
		col_av.y = col_a.g;
		col_av.z = col_a.b;
		col_sv.x = col_s.r;
		col_sv.y = col_s.g;
		col_sv.z = col_s.b;*/
		oapiWriteItem_vec(fh, cbuf, LgtDefs[i].dcol);
		sprintf(cbuf, "LIGHT_%i_ACOL", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, LgtDefs[i].acol);
		sprintf(cbuf, "LIGHT_%i_SCOL", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, LgtDefs[i].scol);
		sprintf(cbuf, "LIGHT_%i_POSITION", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, LgtDefs[i].pos);
		sprintf(cbuf, "LIGHT_%i_RANGE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, LgtDefs[i].range);
		sprintf(cbuf, "LIGHT_%i_VISIBILITY", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, LgtDefs[i].visibility);
		sprintf(cbuf, "LIGHT_%i_ATTENUATION", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, LgtDefs[i].attenuation);
		sprintf(cbuf, "LIGHT_%i_HASATTACHMENT", i);
		ConfigCheck(cbuf, Config_idx);
		bool hasatt = LgtDefs[i].hasattachment;
		oapiWriteItem_bool(fh, cbuf, hasatt);
		if (hasatt) {
			sprintf(cbuf, "LIGHT_%i_ATTACHMENT", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, LgtDefs[i].attachment);
		}
		if (LgtDefs[i].type == 2) {
			sprintf(cbuf, "LIGHT_%i_DIRECTION", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, LgtDefs[i].dir);
			//double umbra, penumbra;
			//GetLightAperture(i, umbra, penumbra);
			VECTOR3 aperture = LgtDefs[i].aperture;// _V(umbra, penumbra, 0);
			aperture *= DEG;
			sprintf(cbuf, "LIGHT_%i_APERTURE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, aperture);
		}
		oapiWriteLine(fh, " ");
	}
}
void LightSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < BcnDefs.size(); i++) {
		DWORD shape = (DWORD)BcnDefs[i].shape;
		LightsMng->AddBeaconDef(BcnDefs[i].name, shape, BcnDefs[i].pos, BcnDefs[i].col, BcnDefs[i].size, BcnDefs[i].falloff, BcnDefs[i].period, BcnDefs[i].duration, BcnDefs[i].tofs);
	}
	for (UINT i = 0; i < LgtDefs.size(); i++) {
		LightEmitter::TYPE ltype;
		COLOUR4 diff, amb, spec;
		LightEmitter::VISIBILITY vis;
		ltype = (LightEmitter::TYPE)LgtDefs[i].type;
		vis = (LightEmitter::VISIBILITY)LgtDefs[i].visibility;
		diff.r = LgtDefs[i].dcol.x;
		diff.g = LgtDefs[i].dcol.y;
		diff.b = LgtDefs[i].dcol.z;
		diff.a = 0;
		amb.r = LgtDefs[i].acol.x;
		amb.g = LgtDefs[i].acol.y;
		amb.b = LgtDefs[i].acol.z;
		amb.a = 0;
		spec.r = LgtDefs[i].scol.x;
		spec.g = LgtDefs[i].scol.y;
		spec.b = LgtDefs[i].scol.z;
		spec.a = 0;
		UINT index = LightsMng->AddLightDef(ltype, LgtDefs[i].name, LgtDefs[i].pos, vis, LgtDefs[i].dir, LgtDefs[i].attenuation, LgtDefs[i].range, LgtDefs[i].aperture.x, LgtDefs[i].aperture.y, diff, spec, amb);
		if (LgtDefs[i].hasattachment) {
			LightsMng->SetLightToAttachment(index, true, LgtDefs[i].attachment);
		}
	}
}
void LightSection::UpdateSection() {
	BcnDefs.clear();
	LgtDefs.clear();
	for (UINT i = 0; i < LightsMng->GetBeaconCount(); i++) {
		BcnDefinitions bd = BcnDefinitions();
		bd.name = LightsMng->GetBeaconName(i);
		bd.shape = (int)LightsMng->GetBeaconShape(i);
		bd.pos = LightsMng->GetBeaconPos(i);
		bd.col = LightsMng->GetBeaconCol(i);
		bd.size = LightsMng->GetBeaconSize(i);
		bd.falloff = LightsMng->GetBeaconFalloff(i);
		bd.period = LightsMng->GetBeaconPeriod(i);
		bd.duration = LightsMng->GetBeaconDuration(i);
		bd.tofs = LightsMng->GetBeaconTofs(i);
		BcnDefs.push_back(bd);
	}
	for (UINT i = 0; i < LightsMng->GetLightsCount(); i++) {
		LightDefinitions ld = LightDefinitions();
		ld.name = LightsMng->GetLightName(i);
		ld.type = (int)LightsMng->GetLightType(i);
		COLOUR4 col_d, col_a, col_s;
		LightsMng->GetLightColours(i, col_d, col_a, col_s);
		ld.dcol.x = col_d.r;
		ld.dcol.y = col_d.g;
		ld.dcol.z = col_d.b;
		ld.acol.x = col_a.r;
		ld.acol.y = col_a.g;
		ld.acol.z = col_a.b;
		ld.scol.x = col_s.r;
		ld.scol.y = col_s.g;
		ld.scol.z = col_s.b;
		ld.pos = LightsMng->GetLightPosition(i);
		ld.range = LightsMng->GetLightRange(i);
		ld.visibility = (int)LightsMng->GetLightVisibility(i);
		ld.attenuation = LightsMng->GetLightAttenuation(i);
		ld.hasattachment = LightsMng->IsLightAttachedToAttachment(i);
		if (ld.hasattachment) {
			ld.attachment = LightsMng->GetLigthAttachment(i);
		}
		if (ld.type == 2) {
			ld.dir = LightsMng->GetLightDirection(i);
			double umbra, penumbra;
			LightsMng->GetLightAperture(i, umbra, penumbra);
			VECTOR3 aperture = _V(umbra, penumbra, 0);
			ld.aperture = aperture;
		}
		LgtDefs.push_back(ld);
	}
}
void LightSection::ManagerClear() {
	LightsMng->Clear();
}
void LightSection::GetSection(VBVector<LightSection::BcnDefinitions> &bd, VBVector<LightSection::LightDefinitions>&ld) {
	bd = BcnDefs;
	ld = LgtDefs;
	return;
}




VardSection::VardSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	VardMng = VB1->VardMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
VardSection::~VardSection() {
	VardMng = NULL;
}
void VardSection::ParseSection(FILEHANDLE fh) {
	UINT vard_counter = 0;
	char cbuf[256] = { '\0' };
	int id;
	sprintf(cbuf, "VARIABLEDRAG_%i_ID", vard_counter);
	ConfigCheck(cbuf, Config_idx);
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "VARIABLEDRAG_%i_NAME", vard_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		double factor;
		int anim_idx;
		VECTOR3 ref;
		sprintf(cbuf, "VARIABLEDRAG_%i_FACTOR", vard_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_float(fh, cbuf, factor);
		sprintf(cbuf, "VARIABLEDRAG_%i_REF", vard_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "VARIABLEDRAG_%i_ANIM", vard_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, anim_idx);
		Definitions d = Definitions();
		d.name = name;
		d.factor = factor;
		d.anim = anim_idx;
		d.ref = ref;
		Defs.push_back(d);
	//	AddVardDef(name, anim_idx, factor, ref);
		vard_counter++;
		sprintf(cbuf, "VARIABLEDRAG_%i_ID", vard_counter);
		ConfigCheck(cbuf, Config_idx);
	}
}
void VardSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------VARIABLE DRAG ITEMS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (UINT i = 0; i < Defs.size(); i++) {
		//if (!IsVardDefDefined(i)) { continue; }
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "VARIABLEDRAG_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "VARIABLEDRAG_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "VARIABLEDRAG_%i_FACTOR", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_float(fh, cbuf, Defs[i].factor);
		sprintf(cbuf, "VARIABLEDRAG_%i_REF", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_vec(fh, cbuf, Defs[i].ref);
		sprintf(cbuf, "VARIABLEDRAG_%i_ANIM", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].anim);
		oapiWriteLine(fh, " ");
	}

}
void VardSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		VardMng->AddVardDef(Defs[i].name, Defs[i].anim, Defs[i].factor, Defs[i].ref);
	}
}
void VardSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < VardMng->GetVardDefCount(); i++) {
		if (!VardMng->IsVardDefDefined(i)) { continue; }
		Definitions d = Definitions();
		d.name = VardMng->GetVardName(i);
		UINT anim;
		VardMng->GetVardParams(i, d.factor, d.ref, anim);
		d.anim = (int)anim;
		Defs.push_back(d);
	}
}
void VardSection::ManagerClear() {
	VardMng->Clear();
}
VBVector<VardSection::Definitions> VardSection::GetSection() {
	return Defs;
}



EventSection::EventSection(VesselBuilder1 *VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	EvMng = VB1->EvMng;
	if (cfg != NULL) {
		ParseSection(cfg);
	}
	else {
		UpdateSection();
	}
	return;
}
EventSection::~EventSection() {
	EvMng = NULL;
}
void EventSection::ParseSection(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
	UINT event_counter = 0;
	sprintf(cbuf, "EVENT_%i_ID", event_counter);
	ConfigCheck(cbuf, Config_idx);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		char namebuf[256] = { '\0' };
		int type;
		int trigger_type;
		int repeat_mode;
		int key;
		int keymod; //1 shift, 2 ctrl, 4 alt, 3 shift + ctrl, 5 shift + alt, 6 ctrl + alt, 7 shift + ctrl + alt
		int docking_port;
		string mate_name;
		bool any_mate;
		bool when_docking;
		int condition;
		double trigger_value;
		int other_event;
		int vel_mode;
		int time_mode;
		string spawned_vessel_name;
		string spawned_vessel_class;
		VECTOR3 ofs;
		VECTOR3 vel;
		VECTOR3 rot_vel;
		//bool delmesh;
		int mesh_to_del;
		int anim_idx;
		bool forward;
		int thruster;
		double thlevel;
		THGROUP_TYPE group;
		double thglevel;
		int dock_idx;
		bool next;
		bool now;
		double newmjd0;
		int newconfig;
		VECTOR3 shiftcg;
		int mesh;
		int texidx;
		string texture_name;
		string sound_file;
		bool disabled;
		int other_event_to_enable;
		bool other_event_enable;

		sprintf(cbuf, "EVENT_%i_NAME", event_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "EVENT_%i_TYPE", event_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, type);
		sprintf(cbuf, "EVENT_%i_DISABLED", event_counter);
		ConfigCheck(cbuf, Config_idx);
		if (!oapiReadItem_bool(fh, cbuf, disabled)) { disabled = false; }
		sprintf(cbuf, "EVENT_%i_TRIGGERTYPE", event_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, trigger_type);
		sprintf(cbuf, "EVENT_%i_REPEAT", event_counter);
		ConfigCheck(cbuf, Config_idx);
		oapiReadItem_int(fh, cbuf, repeat_mode);
		if (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::KEYPRESS) {
			sprintf(cbuf, "EVENT_%i_KEY", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, key);
			sprintf(cbuf, "EVENT_%i_KEYMOD", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, keymod);
		}
		if ((trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::ALTITUDE) || (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::VELOCITY) || (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE)) {
			sprintf(cbuf, "EVENT_%i_CONDITION", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, condition);
		}
		if ((trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::ALTITUDE) || (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::MAINFUELTANK_LEVEL) || (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::VELOCITY) || (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::TIME) || (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE)) {
			sprintf(cbuf, "EVENT_%i_TRIGGERVALUE", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, trigger_value);
		}
		if (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::OTHER_EVENT) {
			sprintf(cbuf, "EVENT_%i_OTHEREVENT", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, other_event);
		}
		if (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::VELOCITY) {
			sprintf(cbuf, "EVENT_%i_VELMODE", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, vel_mode);
		}
		if (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::TIME) {
			sprintf(cbuf, "EVENT_%i_TIMEMODE", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, time_mode);
		}
		if (trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT) {
			sprintf(cbuf, "EVENT_%i_DOCKPORT", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, docking_port);
			sprintf(cbuf, "EVENT_%i_WHENDOCKING", event_counter);
			ConfigCheck(cbuf, Config_idx);
			if (!oapiReadItem_bool(fh, cbuf, when_docking)) { when_docking = false; }
			sprintf(cbuf, "EVENT_%i_ANYMATE", event_counter);
			ConfigCheck(cbuf, Config_idx);
			if (!oapiReadItem_bool(fh, cbuf, any_mate)) { any_mate = false; }
			if (!any_mate) {
				sprintf(cbuf, "EVENT_%i_MATENAME", event_counter);
				ConfigCheck(cbuf, Config_idx);
				char matebuff[256] = { '\0' };
				oapiReadItem_string(fh, cbuf, matebuff);
				mate_name.assign(matebuff);
			}
			
		}
		if (type == (int)Event::TYPE::CHILD_SPAWN) {
			char ccbuf[256] = { '\0' };
			sprintf(cbuf, "EVENT_%i_SPAWNEDCLASS", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_string(fh, cbuf, ccbuf);
			spawned_vessel_class.assign(ccbuf);
			sprintf(cbuf, "EVENT_%i_SPAWNEDNAME", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_string(fh, cbuf, ccbuf);
			spawned_vessel_name.assign(ccbuf);
			sprintf(cbuf, "EVENT_%i_SPAWNEDOFS", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, ofs);
			sprintf(cbuf, "EVENT_%i_SPAWNEDVEL", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, vel);
			sprintf(cbuf, "EVENT_%i_SPAWNEDROTVEL", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, rot_vel);
			sprintf(cbuf, "EVENT_%i_SPAWNDELMESH", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, mesh_to_del);
		}
		if (type == (int)Event::TYPE::ANIMATION_TRIGGER) {
			sprintf(cbuf, "EVENT_%i_ANIM", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, anim_idx);
			sprintf(cbuf, "EVENT_%i_ANIMFWD", event_counter);
			ConfigCheck(cbuf, Config_idx);
			if (!oapiReadItem_bool(fh, cbuf, forward)) { forward = false; }
		}
		if (type == (int)Event::TYPE::THRUSTER_FIRING) {
			sprintf(cbuf, "EVENT_%i_THR", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, thruster);
			sprintf(cbuf, "EVENT_%i_THRLVL", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, thlevel);
		}
		if (type == (int)Event::TYPE::THRUSTERGROUP_LEVEL) {
			sprintf(cbuf, "EVENT_%i_THG", event_counter);
			ConfigCheck(cbuf, Config_idx);
			int thg;
			oapiReadItem_int(fh, cbuf, thg);
			group = (THGROUP_TYPE)thg;
			sprintf(cbuf, "EVENT_%i_THGLVL", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_float(fh, cbuf, thglevel);
		}
		if (type == (int)Event::TYPE::PAYLOAD_JETTISON) {
			sprintf(cbuf, "EVENT_%i_JETTNEXT", event_counter);
			ConfigCheck(cbuf, Config_idx);
			if (!oapiReadItem_bool(fh, cbuf, next)) { next = false; }
			if (!next) {
				sprintf(cbuf, "EVENT_%i_JETTIDX", event_counter);
				ConfigCheck(cbuf, Config_idx);
				oapiReadItem_int(fh, cbuf, dock_idx);
			}
		}
		if (type == (int)Event::TYPE::RESET_MET) {
			sprintf(cbuf, "EVENT_%i_TIMENOW", event_counter);
			ConfigCheck(cbuf, Config_idx);
			if (!oapiReadItem_bool(fh, cbuf, now)) { now = false; }
			if (!now) {
				sprintf(cbuf, "EVENT_%i_TIMEMJD0", event_counter);
				ConfigCheck(cbuf, Config_idx);
				oapiReadItem_float(fh, cbuf, newmjd0);
			}
		}
		if (type == (int)Event::TYPE::RECONFIG) {
			sprintf(cbuf, "EVENT_%i_RECONFIG", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, newconfig);
		}
		if (type == (int)Event::TYPE::SHIFT_CG) {
			sprintf(cbuf, "EVENT_%i_SHIFTCG", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_vec(fh, cbuf, shiftcg);
		}
		if (type == (int)Event::TYPE::TEXTURE_SWAP) {
			sprintf(cbuf, "EVENT_%i_TXSWMESH", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, mesh);
			sprintf(cbuf, "EVENT_%i_TXSWTEXIDX", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, texidx);
			char ccbuf[256] = { '\0' };
			sprintf(cbuf, "EVENT_%i_TXSWTEXNAME", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_string(fh, cbuf, ccbuf);
			texture_name.assign(ccbuf);
		}
		if (type == (int)Event::TYPE::PLAYSOUND) {
			char sndbf[256] = { '\0' };
			sprintf(cbuf, "EVENT_%i_SNDFILE", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_string(fh, cbuf, sndbf);
			sound_file.assign(sndbf);
		}
		if (type == (int)Event::TYPE::ENABLE_EVENT) {
			sprintf(cbuf, "EVENT_%i_OTHERTOENABLE", event_counter);
			ConfigCheck(cbuf, Config_idx);
			oapiReadItem_int(fh, cbuf, other_event_to_enable);
			sprintf(cbuf, "EVENT_%i_OTHERENABLE", event_counter);
			ConfigCheck(cbuf, Config_idx);
			if (!oapiReadItem_bool(fh, cbuf, other_event_enable)) { other_event_enable = false; }
		}
		Definitions d;
		d.name = name;
		d.type = type;
		d.trigger_type = trigger_type;
		d.repeat_mode = repeat_mode;
		d.key = key;
		d.keymod = keymod;
		d.docking_port = docking_port;
		d.mate_name = mate_name;
		d.any_mate = any_mate;
		d.when_docking = when_docking;
		d.condition = condition;
		d.trigger_value = trigger_value;
		d.other_event = other_event;
		d.vel_mode = vel_mode;
		d.time_mode = time_mode;
		d.spawned_vessel_name = spawned_vessel_name;
		d.spawned_vessel_class = spawned_vessel_class;
		d.ofs = ofs;
		d.vel = vel;
		d.rot_vel = rot_vel;
		d.mesh_to_del = mesh_to_del;
		d.anim_idx = anim_idx;
		d.forward = forward;
		d.thruster = thruster;
		d.thlevel = thlevel;
		d.group = group;
		d.thglevel = thglevel;
		d.dock_idx = dock_idx;
		d.next = next;
		d.now = now;
		d.newmjd0 = newmjd0;
		d.newconfig = newconfig;
		d.shiftcg = shiftcg;
		d.mesh = mesh;
		d.texidx = texidx;
		d.texture_name = texture_name;
		d.sound_file = sound_file;
		d.disabled = disabled;
		d.other_event_to_enable = other_event_to_enable;
		d.other_event_enable = other_event_enable;
		Defs.push_back(d);
		event_counter++;
		sprintf(cbuf, "EVENT_%i_ID", event_counter);
		ConfigCheck(cbuf, Config_idx);
		
	}
}
void EventSection::WriteSection(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------EVENTS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");

	for (UINT i = 0; i < EvMng->GetEventsCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "EVENT_%i_ID", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "EVENT_%i_NAME", i);
		ConfigCheck(cbuf, Config_idx);
		char namebuf[256] = { '\0' };
		sprintf(namebuf, "%s", Defs[i].name.c_str());
		oapiWriteItem_string(fh, cbuf, namebuf);
		sprintf(cbuf, "EVENT_%i_TYPE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].type);
		if (Defs[i].disabled) {
			sprintf(cbuf, "EVENT_%i_DISABLED", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].disabled);
		}
		sprintf(cbuf, "EVENT_%i_TRIGGERTYPE", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].trigger_type);
		sprintf(cbuf, "EVENT_%i_REPEAT", i);
		ConfigCheck(cbuf, Config_idx);
		oapiWriteItem_int(fh, cbuf, Defs[i].repeat_mode);
		if (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::KEYPRESS) {
			sprintf(cbuf, "EVENT_%i_KEY", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].key);
			sprintf(cbuf, "EVENT_%i_KEYMOD", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].keymod);
		}
		if ((Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::ALTITUDE) || (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::VELOCITY) || (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE)) {
			sprintf(cbuf, "EVENT_%i_CONDITION", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].condition);
		}
		if ((Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::ALTITUDE) || (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::MAINFUELTANK_LEVEL) || (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::VELOCITY) || (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::TIME) || (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE)) {
			sprintf(cbuf, "EVENT_%i_TRIGGERVALUE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, Defs[i].trigger_value);
		}
		if (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::OTHER_EVENT) {
			sprintf(cbuf, "EVENT_%i_OTHEREVENT", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].other_event);
		}
		if (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::VELOCITY) {
			sprintf(cbuf, "EVENT_%i_VELMODE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].vel_mode);
		}
		if (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::TIME) {
			sprintf(cbuf, "EVENT_%i_TIMEMODE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].time_mode);
		}
		if (Defs[i].trigger_type == (int)Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT) {
			sprintf(cbuf, "EVENT_%i_DOCKPORT", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].docking_port);
			sprintf(cbuf, "EVENT_%i_WHENDOCKING", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].when_docking);
			sprintf(cbuf, "EVENT_%i_ANYMATE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].any_mate);
			if (!Defs[i].any_mate) {
				sprintf(cbuf, "EVENT_%i_MATENAME", i);
				ConfigCheck(cbuf, Config_idx);
				char matebuff[256] = { '\0' };
				sprintf(matebuff, "%s", Defs[i].mate_name.c_str());
				oapiWriteItem_string(fh, cbuf, matebuff);
			}
			
		}
		if (Defs[i].type == (int)Event::TYPE::CHILD_SPAWN) {
			char ccbuf[256] = { '\0' };
			sprintf(cbuf, "EVENT_%i_SPAWNEDCLASS", i);
			ConfigCheck(cbuf, Config_idx);
			sprintf(ccbuf, "%s", Defs[i].spawned_vessel_class.c_str());
			oapiWriteItem_string(fh, cbuf, ccbuf);
			sprintf(cbuf, "EVENT_%i_SPAWNEDNAME", i);
			ConfigCheck(cbuf, Config_idx);
			sprintf(ccbuf, "%s", Defs[i].spawned_vessel_name.c_str());
			oapiWriteItem_string(fh, cbuf, ccbuf);
			sprintf(cbuf, "EVENT_%i_SPAWNEDOFS", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, Defs[i].ofs);
			sprintf(cbuf, "EVENT_%i_SPAWNEDVEL", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, Defs[i].vel);
			sprintf(cbuf, "EVENT_%i_SPAWNEDROTVEL", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, Defs[i].rot_vel);
			sprintf(cbuf, "EVENT_%i_SPAWNDELMESH", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].mesh_to_del);
		}
		if (Defs[i].type == (int)Event::TYPE::ANIMATION_TRIGGER) {
			sprintf(cbuf, "EVENT_%i_ANIM", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].anim_idx);
			sprintf(cbuf, "EVENT_%i_ANIMFWD", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].forward);
		}
		if (Defs[i].type == (int)Event::TYPE::THRUSTER_FIRING) {
			sprintf(cbuf, "EVENT_%i_THR", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].thruster);
			sprintf(cbuf, "EVENT_%i_THRLVL", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, Defs[i].thlevel);
		}
		if (Defs[i].type == (int)Event::TYPE::THRUSTERGROUP_LEVEL) {
			sprintf(cbuf, "EVENT_%i_THG", i);
			ConfigCheck(cbuf, Config_idx);
			int thg = (int)Defs[i].group;
			oapiWriteItem_int(fh, cbuf, thg);
			sprintf(cbuf, "EVENT_%i_THGLVL", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_float(fh, cbuf, Defs[i].thglevel);
		}
		if (Defs[i].type == (int)Event::TYPE::PAYLOAD_JETTISON) {
			sprintf(cbuf, "EVENT_%i_JETTNEXT", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].next);
			if (!Defs[i].next) {
				sprintf(cbuf, "EVENT_%i_JETTIDX", i);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_int(fh, cbuf, Defs[i].dock_idx);
			}
		}
		if (Defs[i].type == (int)Event::TYPE::RESET_MET) {
			sprintf(cbuf, "EVENT_%i_TIMENOW", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].now);
			if (!Defs[i].now) {
				sprintf(cbuf, "EVENT_%i_TIMEMJD0", i);
				ConfigCheck(cbuf, Config_idx);
				oapiWriteItem_float(fh, cbuf, Defs[i].newmjd0);
			}
		}
		if (Defs[i].type == (int)Event::TYPE::RECONFIG) {
			sprintf(cbuf, "EVENT_%i_RECONFIG", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].newconfig);
		}
		if (Defs[i].type == (int)Event::TYPE::SHIFT_CG) {
			sprintf(cbuf, "EVENT_%i_SHIFTCG", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_vec(fh, cbuf, Defs[i].shiftcg);
		}
		if (Defs[i].type == (int)Event::TYPE::TEXTURE_SWAP) {
			sprintf(cbuf, "EVENT_%i_TXSWMESH", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].mesh);
			sprintf(cbuf, "EVENT_%i_TXSWTEXIDX", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].texidx);
			char ccbuf[256] = { '\0' };
			sprintf(cbuf, "EVENT_%i_TXSWTEXNAME", i);
			ConfigCheck(cbuf, Config_idx);
			sprintf(ccbuf, "%s", Defs[i].texture_name.c_str());
			oapiWriteItem_string(fh, cbuf, ccbuf);
		}
		if (Defs[i].type == (int)Event::TYPE::PLAYSOUND) {
			char sndbf[256] = { '\0' };
			sprintf(cbuf, "EVENT_%i_SNDFILE", i);
			ConfigCheck(cbuf, Config_idx);
			sprintf(sndbf, "%s", Defs[i].sound_file.c_str());
			oapiWriteItem_string(fh, cbuf, sndbf);
		}
		if (Defs[i].type == (int)Event::TYPE::ENABLE_EVENT) {
			sprintf(cbuf, "EVENT_%i_OTHERTOENABLE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_int(fh, cbuf, Defs[i].other_event_to_enable);
			sprintf(cbuf, "EVENT_%i_OTHERENABLE", i);
			ConfigCheck(cbuf, Config_idx);
			oapiWriteItem_bool(fh, cbuf, Defs[i].other_event_enable);
		}
		oapiWriteLine(fh, " ");
	}
	return;
}
void EventSection::ApplySection() {
	Section::ApplySection();
	for (UINT i = 0; i < Defs.size(); i++) {
		Event::TRIGGER trig = Event::TRIGGER();
		trig.Type = (Event::TRIGGER::TRIGGERTYPE)Defs[i].trigger_type;
		trig.repeat_mode = (Event::TRIGGER::REPEAT_MODE)Defs[i].repeat_mode;
		if (trig.Type == Event::TRIGGER::TRIGGERTYPE::KEYPRESS) {
			trig.Key = (DWORD)Defs[i].key;

			trig.KeyMods.Alt = false;
			trig.KeyMods.Shift = false;
			trig.KeyMods.Ctrl = false;
			if ((Defs[i].keymod == 1 )||(Defs[i].keymod == 3) ||(Defs[i].keymod == 5) || (Defs[i].keymod == 7)) {
				trig.KeyMods.Shift = true;
			}
			if ((Defs[i].keymod == 2 )||(Defs[i].keymod == 3) ||(Defs[i].keymod == 6) || (Defs[i].keymod == 7)) {
				trig.KeyMods.Ctrl = true;
			}
			if ((Defs[i].keymod == 4 )||(Defs[i].keymod == 5) ||(Defs[i].keymod == 6) ||(Defs[i].keymod == 7)) {
				trig.KeyMods.Alt = true;
			}
			/*switch (Defs[i].keymod) {
			case 0:
			{
				trig.KeyMods.Alt = false;
				trig.KeyMods.Shift = false;
				trig.KeyMods.Ctrl = false;
				break;
			}
			case 1:
			{
				trig.KeyMods.Alt = false;
				trig.KeyMods.Shift = true;
				trig.KeyMods.Ctrl = false;
				break;
			}
			case 2:
			{
				trig.KeyMods.Alt = false;
				trig.KeyMods.Shift = false;
				trig.KeyMods.Ctrl = true;
				break;
			}
			case 3:
			{
				trig.KeyMods.Alt = false;
				trig.KeyMods.Shift = true;
				trig.KeyMods.Ctrl = true;
				break;
			}
			case 4:
			{
				trig.KeyMods.Alt = true;
				trig.KeyMods.Shift = false;
				trig.KeyMods.Ctrl = false;
				break;
			}
			case 5:
			{
				trig.KeyMods.Alt = true;
				trig.KeyMods.Shift = true;
				trig.KeyMods.Ctrl = false;
				break;
			}
			case 6:
			{
				trig.KeyMods.Alt = true;
				trig.KeyMods.Shift = false;
				trig.KeyMods.Ctrl = true;
				break;
			}
			case 7:
			{
				trig.KeyMods.Alt = true;
				trig.KeyMods.Shift = true;
				trig.KeyMods.Ctrl = true;
				break;
			}
			}*/
		}
		if ((trig.Type == Event::TRIGGER::TRIGGERTYPE::ALTITUDE)||(trig.Type == Event::TRIGGER::TRIGGERTYPE::VELOCITY) ||(trig.Type == Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE)) {
			trig.condition = (Event::TRIGGER::CONDITION)Defs[i].condition;
		}
		if ((trig.Type == Event::TRIGGER::TRIGGERTYPE::ALTITUDE) || (trig.Type == Event::TRIGGER::TRIGGERTYPE::MAINFUELTANK_LEVEL) || (trig.Type == Event::TRIGGER::TRIGGERTYPE::VELOCITY) || (trig.Type == Event::TRIGGER::TRIGGERTYPE::TIME) || (trig.Type == Event::TRIGGER::TRIGGERTYPE::TIME || Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE)) {
			trig.TriggerValue = Defs[i].trigger_value;
		}
		if (trig.Type == Event::TRIGGER::TRIGGERTYPE::OTHER_EVENT) {
			trig.Other_event_h = EvMng->GetEventH(Defs[i].other_event);
		}
		if (trig.Type == Event::TRIGGER::TRIGGERTYPE::VELOCITY) {
			trig.vel_mode = (Event::TRIGGER::VEL_MODE)Defs[i].vel_mode;
		}
		if (trig.Type == Event::TRIGGER::TRIGGERTYPE::TIME) {
			trig.time_mode = (Event::TRIGGER::TIME_MODE)Defs[i].time_mode;
		}
		if (trig.Type == Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT) {
			trig.DockingPort = Defs[i].docking_port;
			trig.MateName = Defs[i].mate_name;
			trig.AnyMate = Defs[i].any_mate;
			trig.WhenDocking = Defs[i].when_docking;
		}

		Event::TYPE etp = (Event::TYPE)Defs[i].type;
		Event* ev;
		switch (etp) {
		case Event::TYPE::CHILD_SPAWN:
		{
			ev = EvMng->CreateChildSpawnEvent(Defs[i].name, trig, Defs[i].spawned_vessel_name, Defs[i].spawned_vessel_class, Defs[i].ofs, Defs[i].vel, Defs[i].rot_vel, Defs[i].mesh_to_del);
			break;
		}
		case Event::TYPE::ANIMATION_TRIGGER:
		{
			ev = EvMng->CreateAnimTriggerEvent(Defs[i].name, trig, Defs[i].anim_idx, Defs[i].forward);
			break;
		}
		case Event::TYPE::THRUSTER_FIRING:
		{
			THRUSTER_HANDLE th = VB1->ThrMng->GetThrTH(Defs[i].thruster);
			ev = EvMng->CreateThrusterFireEvent(Defs[i].name, trig, th, Defs[i].thlevel);
			break;
		}
		case Event::TYPE::THRUSTERGROUP_LEVEL:
		{
			ev = EvMng->CreateThrusterGroupLevelEvent(Defs[i].name, trig, Defs[i].group, Defs[i].thglevel);
			break;
		}
		case Event::TYPE::PAYLOAD_JETTISON:
		{
			ev = EvMng->CreatePayloadJettisonEvent(Defs[i].name, trig, Defs[i].next, Defs[i].dock_idx);
			break;
		}
		case Event::TYPE::RESET_MET:
		{
			ev = EvMng->CreateResetMetEvent(Defs[i].name, trig, Defs[i].now, Defs[i].newmjd0);
			break;
		}
		case Event::TYPE::RECONFIG:
		{
			ev = EvMng->CreateReconfigurationEvent(Defs[i].name, trig, Defs[i].newconfig);
			break;
		}
		case Event::TYPE::SHIFT_CG:
		{
			ev = EvMng->CreateShiftCGEvent(Defs[i].name, trig, Defs[i].shiftcg);
			break;
		}
		case Event::TYPE::TEXTURE_SWAP:
		{
			ev = EvMng->CreateTextureSwapEvent(Defs[i].name, trig, Defs[i].mesh, Defs[i].texidx, Defs[i].texture_name);
			break;
		}
		case Event::TYPE::DELETE_ME:
		{
			ev = EvMng->CreateDeleteMeEvent(Defs[i].name, trig);
			break;
		}
		case Event::TYPE::PLAYSOUND:
		{
			ev = EvMng->CreatePlaySoundEvent(Defs[i].name, trig, Defs[i].sound_file);
			break;
		}
		case Event::TYPE::ENABLE_EVENT:
		{
			Event* ev_to_enable = EvMng->GetEventH(Defs[i].other_event_enable);
			ev = EvMng->CreateEnableEvent(Defs[i].name, trig, ev_to_enable, Defs[i].other_event_enable);
			break;
		}
		ev->Enable(!Defs[i].disabled);
		}

	}
	return;
}
void EventSection::UpdateSection() {
	Defs.clear();
	for (UINT i = 0; i < EvMng->GetEventsCount(); i++) {
		Definitions d;
		d.name = EvMng->GetEventName(i);
		Event::TYPE tp = EvMng->GetEventType(i);
		d.type = (int)tp;
		d.disabled = !EvMng->IsEventEnabled(i);
		Event::TRIGGER trig = EvMng->GetEventTrigger(i);
		d.trigger_type = (int)trig.Type;
		d.repeat_mode = (int)trig.repeat_mode;
		d.key = trig.Key;
		d.keymod = trig.KeyMods.Shift ? 1 : 0 + trig.KeyMods.Ctrl ? 2 : 0 + trig.KeyMods.Alt ? 4 : 0;
		d.condition = (int)trig.condition;
		d.trigger_value = trig.TriggerValue;
		d.other_event = EvMng->GetEventIdx(trig.Other_event_h);
		d.vel_mode = (int)trig.vel_mode;
		d.time_mode = (int)trig.time_mode;
		d.docking_port = trig.DockingPort;
		d.mate_name = trig.MateName;
		d.any_mate = trig.AnyMate;
		d.when_docking = trig.WhenDocking;
		if (tp == Event::TYPE::CHILD_SPAWN) {
			d.spawned_vessel_class = EvMng->GetSpawnedVesselClass(i);
			d.spawned_vessel_name = EvMng->GetSpawnedVesselName(i);
			d.ofs = EvMng->GetOfs(i);
			d.vel = EvMng->GetVel(i);
			d.rot_vel = EvMng->GetRotVel(i);
			d.mesh_to_del = EvMng->GetMeshToDel(i);
		}
		else if (tp == Event::TYPE::ANIMATION_TRIGGER) {
			d.anim_idx = EvMng->GetAnimIdx(i);
			d.forward = EvMng->GetForward(i);
		}
		else if (tp == Event::TYPE::THRUSTER_FIRING) {
			d.thruster = VB1->ThrMng->GetThrIdx(EvMng->GetThrusterTH(i));
			d.thlevel = EvMng->GetThLevel(i);
		}
		else if (tp == Event::TYPE::THRUSTERGROUP_LEVEL) {
			d.group = EvMng->GetThGroup(i);
			d.thglevel = EvMng->GetThGLevel(i);
		}
		else if (tp == Event::TYPE::PAYLOAD_JETTISON) {
			d.dock_idx = EvMng->GetDockIdx(i);
			d.next = EvMng->GetNext(i);
		}
		else if (tp == Event::TYPE::RESET_MET) {
			d.now = EvMng->GetNow(i);
			d.newmjd0 = EvMng->GetNewMJD0(i);
		}
		else if (tp == Event::TYPE::RECONFIG) {
			d.newconfig = EvMng->GetNewConfig(i);
		}
		else if (tp == Event::TYPE::SHIFT_CG) {
			d.shiftcg = EvMng->GetShift(i);
		}
		else if (tp == Event::TYPE::TEXTURE_SWAP) {
			d.mesh = EvMng->GetMesh(i);
			d.texidx = EvMng->GetTexIdx(i);
			d.texture_name = EvMng->GetTextureName(i);
		}
		else if (tp == Event::TYPE::PLAYSOUND) {
			d.sound_file = EvMng->GetSoundFile(i);
		}
		else if (tp == Event::TYPE::ENABLE_EVENT) {
			d.other_event_enable = EvMng->GetToEnable(i);
			d.other_event_to_enable = EvMng->GetEventIdx(EvMng->GetEventToEnable(i));
		}
		Defs.push_back(d);
	}
	return;
}
void EventSection::ManagerClear() {
	EvMng->Clear();
	return;
}









Configuration::Configuration(VesselBuilder1 *_VB1, map<ItemType, bool> _Sections, UINT _config, FILEHANDLE _cfg) {
	VB1 = _VB1;
	Configuration_Sections = _Sections;
	LogV("Adding Configuration n:%i", _config);
	Sections.clear();
	Sections.push_back(new SettingSection(_VB1, _config, _cfg)); //0
	Sections.push_back(new MeshSection(_VB1, _config, _cfg)); //1
	Sections.push_back(new DockSection(_VB1, _config, _cfg)); //2
	Sections.push_back(new AttachmentSection(_VB1, _config, _cfg)); //3
	Sections.push_back(new AnimationSection(_VB1, _config, _cfg)); //4
	Sections.push_back(new PropellantSection(_VB1, _config, _cfg)); //5
	Sections.push_back(new ThrusterSection(_VB1, _config, _cfg)); //6
	Sections.push_back(new ThrusterGroupSection(_VB1, _config, _cfg)); //7
	Sections.push_back(new TouchDownPointSection(_VB1, _config, _cfg)); //8
	Sections.push_back(new AirfoilSection(_VB1, _config, _cfg)); //9
	Sections.push_back(new CtrlSurfaceSection(_VB1, _config, _cfg));  //10
	Sections.push_back(new CameraSection(_VB1, _config, _cfg));  //11
	Sections.push_back(new VCSection(_VB1, _config, _cfg)); //12
	Sections.push_back(new LightSection(_VB1, _config, _cfg)); //13
	Sections.push_back(new VardSection(_VB1, _config, _cfg)); //14
	Sections.push_back(new EventSection(_VB1, _config, _cfg)); //15
	UpdateValids();

	
	Config_idx = _config;
	return;
	
}





Configuration::~Configuration(){
	for (UINT i = 0; i < Sections.size(); i++) {
		if (Sections[i]) {
			delete Sections[i];
		}
	}
	Sections.clear();
	
}
void Configuration::Delete() {
	for (UINT i = 0; i < Sections.size(); i++) {
		if (Sections[i]) {
			delete Sections[i];
		}
	}
	Sections.clear();

}
void Configuration::Apply(){
	for (UINT i = 0; i < Sections.size(); i++) {
		UINT p = Sections.size() -1- i;
		if (Sections[p]->IsValid()) {
			Sections[p]->ManagerClear();
		}
	}
	for (UINT i = 0; i < Sections.size(); i++) {
		if (Sections[i]->IsValid()) {
			Sections[i]->ApplySection();
		}
	}
}
void Configuration::Write(FILEHANDLE cfg) {
	for (UINT i = 0; i < Sections.size(); i++) {
		if (Sections[i]->IsValid()) {
			Sections[i]->WriteSection(cfg);
		}
	}
	

}
void Configuration::Update() {
	for (UINT i = 0; i < Sections.size(); i++) {
		if (Sections[i]->IsValid()) {
			Sections[i]->UpdateSection();
		}
	}
}
bool Configuration::IsSectionValid(ItemType Type) {
	if (Configuration_Sections[Type]) {
		return true;
	}
	else {
		return false;
	}

}
void Configuration::SetSectionValid(ItemType Type, bool set) {
	Configuration_Sections[Type] = set;
	UpdateValids();
}

bool Configuration::IsSectionActive(ItemType Type) {
	int sect = -1;
	switch (Type) {
	case SETTINGS:
	{
		sect = SETTINGS_SECTION;
		break;
	}
	case MESH:
	{
		sect = MESH_SECTION;
		break;
	}
	case DOCK:
	{
		sect = DOCK_SECTION;
		break;
	}
	case ATTACHMENT:
	{
		sect = ATT_SECTION;
		break;
	}
	case ANIMATIONS:
	{
		sect = ANIM_SECTION;
		break;
	}
	case PROPELLANT:
	{
		sect = PROP_SECTION;
		break;
	}
	case THRUSTERS:
	{
		sect = THRUST_SECTION;
		break;
	}
	case THRUSTERGROUPS:
	{
		sect = THGROUP_SECTION;
		break;
	}
	case TOUCHDOWNPOINTS:
	{
		sect = TDP_SECTION;
		break;
	}
	case AIRFOILS:
	{
		sect = AIRFOIL_SECTION;
		break;
	}
	case CTRLSURFACES:
	{
		sect = CTRLSURF_SECTION;
		break;
	}
	case CAMERA:
	{
		sect = CAMERAS_SECTION;
		break;
	}
	case VC:
	{
		sect = VC_SECTION;
		break;
	}
	case LIGHTS:
	{
		sect = LIGHTS_SECTION;
		break;
	}
	case VARIABLEDRAG:
	{
		sect = VARDRAG_SECTION;
		break;
	}
	case EVENTS:
	{
		sect = EVENTS_SECTION;
		break;
	}
	}

	if (sect == -1) {
		return false;
	}
	return Sections[sect]->IsActive();
	
}
void Configuration::SetSectionActive(ItemType Type, bool set) {
	int sect = -1;
	switch (Type) {
	case SETTINGS:
	{
		sect = SETTINGS_SECTION;
		break;
	}
	case MESH:
	{
		sect = MESH_SECTION;
		break;
	}
	case DOCK:
	{
		sect = DOCK_SECTION;
		break;
	}
	case ATTACHMENT:
	{
		sect = ATT_SECTION;
		break;
	}
	case ANIMATIONS:
	{
		sect = ANIM_SECTION;
		break;
	}
	case PROPELLANT:
	{
		sect = PROP_SECTION;
		break;
	}
	case THRUSTERS:
	{
		sect = THRUST_SECTION;
		break;
	}
	case THRUSTERGROUPS:
	{
		sect = THGROUP_SECTION;
		break;
	}
	case TOUCHDOWNPOINTS:
	{
		sect = TDP_SECTION;
		break;
	}
	case AIRFOILS:
	{
		sect = AIRFOIL_SECTION;
		break;
	}
	case CTRLSURFACES:
	{
		sect = CTRLSURF_SECTION;
		break;
	}
	case CAMERA:
	{
		sect = CAMERAS_SECTION;
		break;
	}
	case VC:
	{
		sect = VC_SECTION;
		break;
	}
	case LIGHTS:
	{
		sect = LIGHTS_SECTION;
		break;
	}
	case VARIABLEDRAG:
	{
		sect = VARDRAG_SECTION;
		break;
	}
	case EVENTS:
	{
		sect = EVENTS_SECTION;
		break;
	}
	}

	if (sect == -1) {
		return;
	}
	Sections[sect]->SetActive(set);

	return;
}

void Configuration::UpdateValids() {

	for (UINT i = 0; i < 16; i++) {
		Sections[i]->SetValid(false);
	}

	if (Configuration_Sections[SETTINGS]) {
		LogV("General Settings Section included");
		Sections[0]->SetValid(true);
	}

	if (Configuration_Sections[MESH]) {
		LogV("Mesh Section included");
		Sections[1]->SetValid(true);
	}
	if (Configuration_Sections[DOCK]) {
		LogV("Dock Section included");
		Sections[2]->SetValid(true);
	}
	if (Configuration_Sections[ATTACHMENT]) {
		LogV("Attachments Section included");
		Sections[3]->SetValid(true);
	}
	if (Configuration_Sections[ANIMATIONS]) {
		LogV("Animations Section included");
		Sections[4]->SetValid(true);
	}
	if (Configuration_Sections[PROPELLANT]) {
		LogV("Propellants Section included");
		Sections[5]->SetValid(true);
	}
	if (Configuration_Sections[THRUSTERS]) {
		LogV("Thrusters Section included");
		Sections[6]->SetValid(true);
	}
	if (Configuration_Sections[THRUSTERGROUPS]) {
		LogV("ThrusterGroup Section included");
		Sections[7]->SetValid(true);
	}
	if (Configuration_Sections[TOUCHDOWNPOINTS]) {
		LogV("Touchdown Points Section included");
		Sections[8]->SetValid(true);
	}
	if (Configuration_Sections[AIRFOILS]) {
		LogV("Airfoils Section included");
		Sections[9]->SetValid(true);
	}
	if (Configuration_Sections[CTRLSURFACES]) {
		LogV("Control Surfaces Section included");
		Sections[10]->SetValid(true);
	}
	if (Configuration_Sections[CAMERA]) {
		LogV("Camera Section included");
		Sections[11]->SetValid(true);
	}
	if (Configuration_Sections[VC]) {
		LogV("Virtual Cockpit Section included");
		Sections[12]->SetValid(true);
	}
	if (Configuration_Sections[LIGHTS]) {
		LogV("Lights Section included");
		Sections[13]->SetValid(true);
	}
	if (Configuration_Sections[VARIABLEDRAG]) {
		LogV("Variable Drag Section included");
		Sections[14]->SetValid(true);
	}
	if (Configuration_Sections[EVENTS]) {
		LogV("Events Section included");
		Sections[15]->SetValid(true);
	}
}
void Configuration::SetIndex(UINT config_idx) {
	Config_idx = config_idx;
	for (UINT i = 0; i < Sections.size(); i++) {
		Sections[i]->SetConfigIdx(config_idx);
	}
	return;
}
UINT Configuration::GetIndex() {
	return Config_idx;
}
bool Configuration::GetSettingSection(SettingSection::Definitions &d){
	if (IsSectionValid(SETTINGS)) {
		d = ((SettingSection*)Sections[SETTINGS_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetSettingsSection(SettingSection::Definitions d) {
	if (IsSectionValid(SETTINGS)) {
		((SettingSection*)Sections[SETTINGS_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetMeshSection(VBVector<MeshSection::Definitions> &d) {
	if (IsSectionValid(MESH)) {
		d = ((MeshSection*)Sections[MESH_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetMeshSection(VBVector<MeshSection::Definitions>d) {
	if (IsSectionValid(MESH)) {
		((MeshSection*)Sections[MESH_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetDockSection(VBVector<DockSection::Definitions> &d) {
	if (IsSectionValid(DOCK)) {
		d = ((DockSection*)Sections[DOCK_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetDockSection(VBVector<DockSection::Definitions>d) {
	if (IsSectionValid(DOCK)) {
		((DockSection*)Sections[DOCK_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetAttachmentSection(VBVector<AttachmentSection::Definitions> &d) {
	if (IsSectionValid(ATTACHMENT)) {
		d = ((AttachmentSection*)Sections[ATT_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetAttachmentSection(VBVector<AttachmentSection::Definitions> d) {
	if (IsSectionValid(ATTACHMENT)) {
		((AttachmentSection*)Sections[ATT_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetAnimationSection(VBVector<AnimationSection::AnimDefinitions>&ad, VBVector<AnimationSection::AnimCompDefinitions>&acd) {
	if (IsSectionValid(ANIMATIONS)) {
		((AnimationSection*)Sections[ANIM_SECTION])->GetSection(ad, acd);
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetAnimationSection(VBVector<AnimationSection::AnimDefinitions> ad, VBVector<AnimationSection::AnimCompDefinitions> acd) {
	if (IsSectionValid(ANIMATIONS)) {
		((AnimationSection*)Sections[ANIM_SECTION])->SetSection(ad, acd);
	}
	return;
}
bool Configuration::GetPropellantSection(VBVector<PropellantSection::Definitions>&d) {
	if (IsSectionValid(PROPELLANT)) {
		d = ((PropellantSection*)Sections[PROP_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetPropellantSection(VBVector<PropellantSection::Definitions>d) {
	if (IsSectionValid(PROPELLANT)) {
		((PropellantSection*)Sections[PROP_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetThrusterSection(VBVector<ThrusterSection::Definitions>&d) {
	if (IsSectionValid(THRUSTERS)) {
		d = ((ThrusterSection*)Sections[THRUST_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetThrusterSection(VBVector<ThrusterSection::Definitions>d) {
	if (IsSectionValid(THRUSTERS)) {
		((ThrusterSection*)Sections[THRUST_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetThrusterGroupSection(map<THGROUP_TYPE, bool> &Def, map<THGROUP_TYPE, VBVector<UINT> > &thrusters) {
	if (IsSectionValid(THRUSTERGROUPS)) {
		((ThrusterGroupSection*)Sections[THGROUP_SECTION])->GetSection(Def, thrusters);
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetThrusterGroupSection(map<THGROUP_TYPE, bool> Def, map<THGROUP_TYPE, VBVector<UINT> > thrusters) {
	if (IsSectionValid(THRUSTERGROUPS)) {
		((ThrusterGroupSection*)Sections[THGROUP_SECTION])->SetSection(Def, thrusters);
	}
	return;
}
bool Configuration::GetTouchdownPointSection(VBVector<TouchDownPointSection::Definitions> &set1, VBVector<TouchDownPointSection::Definitions>&set2) {
	if (IsSectionValid(TOUCHDOWNPOINTS)) {
		((TouchDownPointSection*)Sections[TDP_SECTION])->GetSection(set1, set2);
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetTouchdownPointSection(VBVector<TouchDownPointSection::Definitions> set1, VBVector<TouchDownPointSection::Definitions> set2) {
	if (IsSectionValid(TOUCHDOWNPOINTS)) {
		((TouchDownPointSection*)Sections[TDP_SECTION])->SetSection(set1, set2);
	}
	return;
}
bool Configuration::GetAirfoilSection(VBVector<AirfoilSection::Definitions>&d) {
	if (IsSectionValid(AIRFOILS)) {
		d = ((AirfoilSection*)Sections[AIRFOIL_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetAirfoilSection(VBVector<AirfoilSection::Definitions>d) {
	if (IsSectionValid(AIRFOILS)) {
		((AirfoilSection*)Sections[AIRFOIL_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetCtrlSurfSection(VBVector<CtrlSurfaceSection::Definitions>&d) {
	if (IsSectionValid(CTRLSURFACES)) {
		d = ((CtrlSurfaceSection*)Sections[CTRLSURF_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetCtrlSurfSection(VBVector<CtrlSurfaceSection::Definitions>d) {
	if (IsSectionValid(CTRLSURFACES)) {
		((CtrlSurfaceSection*)Sections[CTRLSURF_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetCameraSection(VBVector<CameraSection::Definitions>&d) {
	if (IsSectionValid(CAMERA)) {
		d = ((CameraSection*)Sections[CAMERAS_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetCameraSection(VBVector<CameraSection::Definitions>d) {
	if (IsSectionValid(CAMERA)) {
		((CameraSection*)Sections[CAMERAS_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetVCSection(VBVector<VCSection::PosDefinitions>&pd, VBVector<VCSection::MFDDefinitions>&md, VCSection::HUDDefinition &hd) {
	if (IsSectionValid(VC)) {
		((VCSection*)Sections[VC_SECTION])->GetSection(pd, md, hd);
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetVCSection(VBVector<VCSection::PosDefinitions>pd, VBVector<VCSection::MFDDefinitions>md, VCSection::HUDDefinition hd) {
	if (IsSectionValid(VC)) {
		((VCSection*)Sections[VC_SECTION])->SetSection(pd, md, hd);
	}
	return;
}
bool Configuration::GetLightSection(VBVector<LightSection::BcnDefinitions>&bd, VBVector<LightSection::LightDefinitions>&ld) {
	if (IsSectionValid(LIGHTS)) {
		((LightSection*)Sections[LIGHTS_SECTION])->GetSection(bd, ld);
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetLightSection(VBVector<LightSection::BcnDefinitions> bd, VBVector<LightSection::LightDefinitions> ld) {
	if (IsSectionValid(LIGHTS)) {
		((LightSection*)Sections[LIGHTS_SECTION])->SetSection(bd, ld);
	}
	return;
}
bool Configuration::GetVardSection(VBVector<VardSection::Definitions>&d) {
	if (IsSectionValid(VARIABLEDRAG)) {
		d = ((VardSection*)Sections[VARDRAG_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetVardSection(VBVector<VardSection::Definitions>d) {
	if (IsSectionValid(VARIABLEDRAG)) {
		((VardSection*)Sections[VARDRAG_SECTION])->SetSection(d);
	}
	return;
}
bool Configuration::GetEventSection(VBVector<EventSection::Definitions>&d) {
	if (IsSectionValid(EVENTS)) {
		d = ((EventSection*)Sections[EVENTS_SECTION])->GetSection();
		return true;
	}
	else {
		return false;
	}
}
void Configuration::SetEventSection(VBVector<EventSection::Definitions>d) {
	if (IsSectionValid(EVENTS)) {
		((EventSection*)Sections[EVENTS_SECTION])->SetSection(d);
	}
	return;
}



ConfigurationManager::ConfigurationManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	Configurations.clear();
	config_counter = 0;
	CurrentConfiguration = 0;
	return;
}
ConfigurationManager::~ConfigurationManager() {
	for (UINT i = 0; i < Configurations.size(); i++) {
		delete Configurations[i];
		Configurations[i] = NULL;
	}
	Configurations.clear();
	VB1 = NULL;
	return;
}

UINT ConfigurationManager::AddConfiguration(VesselBuilder1* VB1, map<ItemType, bool>_Sections, FILEHANDLE cfg) {
	UINT index = Configurations.size();
	//Configuration *Config = new Configuration(VB1, _Sections, config_counter, cfg);
	Configuration *Config = new Configuration(VB1, _Sections, index, cfg);
	//config_counter++;
	Configurations.push_back(Config);
	//return config_counter-1;
	return index;
}



void ConfigurationManager::ApplyConfiguration(UINT config, bool firstload) {
	if (config >= Configurations.size()) {
		return;
	}
	if ((!firstload)&&(VB1->DlgOpened)) { // Update non va fatto anche se non apro la dialog
		UINT old_config = CurrentConfiguration;
		Configurations[old_config]->Update();
		map<ItemType, bool>newSects = Configurations[config]->GetSections();
		map<ItemType, bool>::iterator it;
		for (it = newSects.begin(); it != newSects.end(); it++) {
			if (it->second == true) {
				Configurations[old_config]->SetSectionActive(it->first, false);
			}
		}
	}
	Configurations[config]->Apply();
	CurrentConfiguration = config;
	return;
}
void ConfigurationManager::ApplyDefaultConfiguration(bool firstload) {
	if (Configurations.size() > 0) {
		ApplyConfiguration(0, firstload);
	}
		/*if (!firstload) {
			UINT old_config = CurrentConfiguration;
			Configurations[old_config]->Update();
		}
		Configurations[0]->Apply();
	}
	CurrentConfiguration = 0;*/
	return;
}

UINT ConfigurationManager::GetCurrentConfiguration() {
	return CurrentConfiguration;
}
UINT ConfigurationManager::GetConfigurationsCount() {
	return Configurations.size();
}
void ConfigurationManager::WriteConfiguration(UINT config_idx, FILEHANDLE cfg) {
	Configurations[config_idx]->Write(cfg);
}
bool ConfigurationManager::IsSectionValid(UINT config, ItemType Type) {
	return Configurations[config]->IsSectionValid(Type);
}
bool ConfigurationManager::IsSectionActive(UINT config, ItemType Type) {
	return Configurations[config]->IsSectionActive(Type);
}

void ConfigurationManager::ParseCfgFile(FILEHANDLE fh) {
	int Configs;
	if (!oapiReadItem_int(fh, "CONFIGURATIONS", Configs)) { Configs = 1; }

	map<ItemType, bool>Sects;
	Sects[SETTINGS] = true;
	Sects[MESH] = true;
	Sects[DOCK] = true;
	Sects[ATTACHMENT] = true;
	Sects[ANIMATIONS] = true;
	Sects[PROPELLANT] = true;
	Sects[THRUSTERS] = true;
	Sects[THRUSTERGROUPS] = true;
	Sects[TOUCHDOWNPOINTS] = true;
	Sects[AIRFOILS] = true;
	Sects[CTRLSURFACES] = true;
	Sects[CAMERA] = true;
	Sects[VC] = true;
	Sects[LIGHTS] = true;
	Sects[VARIABLEDRAG] = true;
	Sects[EVENTS] = true;
	for (UINT i = 0; i < Configs; i++) {
		if (i > 0) {
			char cbuf[256] = { '\0' };
			char Sections_c[256] = { '\0' };
			sprintf(cbuf, "CONFIGURATION_%i_SECTIONS", i);
			oapiReadItem_string(fh, cbuf, Sections_c);
			string Sections_s(Sections_c);
			VBVector<UINT>Sections_n = VB1->readVectorUINT(Sections_s);
			VB1->IsUintInVector(SETTINGS_SECTION, Sections_n) ? Sects[SETTINGS] = true : Sects[SETTINGS] = false;
			VB1->IsUintInVector(MESH_SECTION, Sections_n) ? Sects[MESH] = true : Sects[MESH] = false;
			VB1->IsUintInVector(DOCK_SECTION, Sections_n) ? Sects[DOCK] = true : Sects[DOCK] = false;
			VB1->IsUintInVector(ATT_SECTION, Sections_n) ? Sects[ATTACHMENT] = true : Sects[ATTACHMENT] = false;
			VB1->IsUintInVector(ANIM_SECTION, Sections_n) ? Sects[ANIMATIONS] = true : Sects[ANIMATIONS] = false;
			VB1->IsUintInVector(PROP_SECTION, Sections_n) ? Sects[PROPELLANT] = true : Sects[PROPELLANT] = false;
			VB1->IsUintInVector(THRUST_SECTION, Sections_n) ? Sects[THRUSTERS] = true : Sects[THRUSTERS] = false;
			VB1->IsUintInVector(THGROUP_SECTION, Sections_n) ? Sects[THRUSTERGROUPS] = true : Sects[THRUSTERGROUPS] = false;
			VB1->IsUintInVector(TDP_SECTION, Sections_n) ? Sects[TOUCHDOWNPOINTS] = true : Sects[TOUCHDOWNPOINTS] = false;
			VB1->IsUintInVector(AIRFOIL_SECTION, Sections_n) ? Sects[AIRFOILS] = true : Sects[AIRFOILS] = false;
			VB1->IsUintInVector(CTRLSURF_SECTION, Sections_n) ? Sects[CTRLSURFACES] = true : Sects[CTRLSURFACES] = false;
			VB1->IsUintInVector(CAMERAS_SECTION, Sections_n) ? Sects[CAMERA] = true : Sects[CAMERA] = false;
			VB1->IsUintInVector(VC_SECTION, Sections_n) ? Sects[VC] = true : Sects[VC] = false;
			VB1->IsUintInVector(LIGHTS_SECTION, Sections_n) ? Sects[LIGHTS] = true : Sects[LIGHTS] = false;
			VB1->IsUintInVector(VARDRAG_SECTION, Sections_n) ? Sects[VARIABLEDRAG] = true : Sects[VARIABLEDRAG] = false;
			VB1->IsUintInVector(EVENTS_SECTION, Sections_n) ? Sects[EVENTS] = true : Sects[EVENTS] = false;
			
		}
		AddConfiguration(VB1, Sects,fh);
	}
	ApplyDefaultConfiguration(true);
}

void ConfigurationManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteItem_int(fh, "CONFIGURATIONS", GetConfigurationsCount());
	for (UINT i = 0; i < GetConfigurationsCount(); i++) {
		if (i > 0) {
			char cbuf[256] = { '\0' };
			char Sections_c[256] = { '\0' };
			sprintf(cbuf, "CONFIGURATION_%i_SECTIONS", i);
			VBVector<UINT> Sections_n;
			Sections_n.clear();
			if (IsSectionValid(i, SETTINGS)) { Sections_n.push_back(SETTINGS_SECTION); }
			if (IsSectionValid(i, MESH)) { Sections_n.push_back(MESH_SECTION); }
			if (IsSectionValid(i, DOCK)) { Sections_n.push_back(DOCK_SECTION); }
			if (IsSectionValid(i, ATTACHMENT)) { Sections_n.push_back(ATT_SECTION); }
			if (IsSectionValid(i, ANIMATIONS)) { Sections_n.push_back(ANIM_SECTION); }
			if (IsSectionValid(i, PROPELLANT)) { Sections_n.push_back(PROP_SECTION); }
			if (IsSectionValid(i, THRUSTERS)) { Sections_n.push_back(THRUST_SECTION); }
			if (IsSectionValid(i, THRUSTERGROUPS)) { Sections_n.push_back(THGROUP_SECTION); }
			if (IsSectionValid(i, TOUCHDOWNPOINTS)) { Sections_n.push_back(TDP_SECTION); }
			if (IsSectionValid(i, AIRFOILS)) { Sections_n.push_back(AIRFOIL_SECTION); }
			if (IsSectionValid(i, CTRLSURFACES)) { Sections_n.push_back(CTRLSURF_SECTION); }
			if (IsSectionValid(i, CAMERA)) { Sections_n.push_back(CAMERAS_SECTION); }
			if (IsSectionValid(i, VC)) { Sections_n.push_back(VC_SECTION); }
			if (IsSectionValid(i, LIGHTS)) { Sections_n.push_back(LIGHTS_SECTION); }
			if (IsSectionValid(i, VARIABLEDRAG)) { Sections_n.push_back(VARDRAG_SECTION); }
			if (IsSectionValid(i, EVENTS)) { Sections_n.push_back(EVENTS_SECTION); }
			string Section_s = VB1->WriteVectorUINT(Sections_n, false);
			sprintf(Sections_c, "%s", Section_s.c_str());
			oapiWriteItem_string(fh, cbuf, Sections_c);
		}
		WriteConfiguration(i, fh);
		if (i == 0) {
			VB1->ExTMng->WriteCfg(fh);
			VB1->PartMng->WriteCfg(fh);
		}
	}
	return;
}

void ConfigurationManager::UpdateConfiguration(UINT config) {
	Configurations[config]->Update();
	return;
}

UINT ConfigurationManager::GetSectionActiveConfig(ItemType Type) {
	for (UINT i = 0; i < GetConfigurationsCount(); i++) {
		if (Configurations[i]->IsSectionActive(Type)) {
			return i;
		}
	}
	return 0;
}

map<ItemType, bool> ConfigurationManager::GetConfigurationSections(UINT config) {
	return Configurations[config]->GetSections();
}

void ConfigurationManager::DeleteConfiguration(UINT config) {
	Configurations[config]->Delete();
	delete Configurations[config];
	Configurations[config] = NULL;
	Configurations.erase(Configurations.begin() + config);
	UpdateConfigIndexes();
	return;
}

void ConfigurationManager::SetSectionValid(UINT config, ItemType Type, bool set) {
	return Configurations[config]->SetSectionValid(Type, set);
}


void ConfigurationManager::UpdateConfigIndexes() {
	for (UINT i = 0; i < Configurations.size(); i++) {
		Configurations[i]->SetIndex(i);
	}
}

void ConfigurationManager::CopyConfigurationSection(UINT config_src, UINT config_dest, ItemType type) {
	switch (type) {
	case SETTINGS: 
	{
		SettingSection::Definitions d;
		if (Configurations[config_src]->GetSettingSection(d)) {
			Configurations[config_dest]->SetSettingsSection(d);
		}
		break;
	}
	case MESH:
	{
		VBVector<MeshSection::Definitions> d;
		if (Configurations[config_src]->GetMeshSection(d)) {
			Configurations[config_dest]->SetMeshSection(d);
		}
		break;
	}
	case DOCK:
	{
		VBVector<DockSection::Definitions>d;
		if (Configurations[config_src]->GetDockSection(d)) {
			Configurations[config_dest]->SetDockSection(d);
		}
		break;
	}
	case ATTACHMENT:
	{
		VBVector<AttachmentSection::Definitions>d;
		if (Configurations[config_src]->GetAttachmentSection(d)) {
			Configurations[config_dest]->SetAttachmentSection(d);
		}
		break;
	}
	case ANIMATIONS:
	{
		VBVector<AnimationSection::AnimDefinitions> ad;
		VBVector<AnimationSection::AnimCompDefinitions> acd;
		if (Configurations[config_src]->GetAnimationSection(ad, acd)) {
			Configurations[config_dest]->SetAnimationSection(ad, acd);
		}
		break;
	}
	case PROPELLANT:
	{
		VBVector<PropellantSection::Definitions> d;
		if (Configurations[config_src]->GetPropellantSection(d)) {
			Configurations[config_dest]->SetPropellantSection(d);
		}
		break;
	}
	case TOUCHDOWNPOINTS:
	{
		VBVector<TouchDownPointSection::Definitions> set1;
		VBVector<TouchDownPointSection::Definitions> set2;
		if (Configurations[config_src]->GetTouchdownPointSection(set1, set2)) {
			Configurations[config_dest]->SetTouchdownPointSection(set1, set2);
		}
		break;
	}
	case AIRFOILS:
	{
		VBVector<AirfoilSection::Definitions>d;
		if (Configurations[config_src]->GetAirfoilSection(d)) {
			Configurations[config_dest]->SetAirfoilSection(d);
		}
		break;
	}
	case CAMERA:
	{
		VBVector<CameraSection::Definitions>d;
		if (Configurations[config_src]->GetCameraSection(d)) {
			Configurations[config_dest]->SetCameraSection(d);
		}
		break;
	}
	case VC:
	{
		VBVector<VCSection::PosDefinitions>pd;
		VBVector<VCSection::MFDDefinitions>md;
		VCSection::HUDDefinition hd;
		if (Configurations[config_src]->GetVCSection(pd, md, hd)) {
			Configurations[config_dest]->SetVCSection(pd, md, hd);
		}
		break;
	}
	case LIGHTS:
	{
		VBVector<LightSection::BcnDefinitions>bd;
		VBVector<LightSection::LightDefinitions>ld;
		if (Configurations[config_src]->GetLightSection(bd, ld)) {
			Configurations[config_dest]->SetLightSection(bd, ld);
		}
		break;
	}
	case VARIABLEDRAG:
	{
		VBVector<VardSection::Definitions>d;
		if (Configurations[config_src]->GetVardSection(d)) {
			Configurations[config_dest]->SetVardSection(d);
		}
		break;
	}
	case EVENTS:
	{
		VBVector<EventSection::Definitions>d;
		if (Configurations[config_src]->GetEventSection(d)) {
			Configurations[config_dest]->SetEventSection(d);
		}
		break;
	}


	}
}


/*
double ConfigurationManager::GetEmptyMass(UINT config) {
	if (config == GetCurrentConfiguration()) {
		return VB1->SetMng->GetEmptyMass();
	}
	else {
		return ((SettingSection*)Configurations[config]->Sections[SETTINGS_SECTION])->GetEmptyMass();
	}	
}
double ConfigurationManager::GetSize(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->SetMng->GetSize() : ((SettingSection*)Configurations[config]->Sections[SETTINGS_SECTION])->GetSize();
}
VECTOR3 ConfigurationManager::GetPMI(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->SetMng->GetPMI() : ((SettingSection*)Configurations[config]->Sections[SETTINGS_SECTION])->GetPMI();
}
VECTOR3 ConfigurationManager::GetCrossSections(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->SetMng->GetCrossSections() : ((SettingSection*)Configurations[config]->Sections[SETTINGS_SECTION])->GetCrossSections();
}
double ConfigurationManager::GetGravityGradientDamping(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->SetMng->GetGravityGradientDamping() : ((SettingSection*)Configurations[config]->Sections[SETTINGS_SECTION])->GetGravityGradientDamping();
}
VECTOR3 ConfigurationManager::GetRotDrag(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->SetMng->GetRotDrag() : ((SettingSection*)Configurations[config]->Sections[SETTINGS_SECTION])->GetRotDrag();
}

int ConfigurationManager::GetMeshDefCount(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->MshMng->GetMeshCount() : ((MeshSection*)Configurations[config]->Sections[MESH_SECTION])->GetMeshDefCount();
}
string ConfigurationManager::GetMeshName(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->MshMng->GetMeshDefName(idx) : ((MeshSection*)Configurations[config]->Sections[MESH_SECTION])->GetMeshName(idx);
}
VECTOR3 ConfigurationManager::GetMeshPos(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->MshMng->GetMeshDefPos(idx) : ((MeshSection*)Configurations[config]->Sections[MESH_SECTION])->GetMeshPos(idx);
}
VECTOR3 ConfigurationManager::GetMeshDir(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->MshMng->GetMeshDefDir(idx) : ((MeshSection*)Configurations[config]->Sections[MESH_SECTION])->GetMeshDir(idx);
}
VECTOR3 ConfigurationManager::GetMeshRot(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->MshMng->GetMEshDefRot(idx) : ((MeshSection*)Configurations[config]->Sections[MESH_SECTION])->GetMeshRot(idx);
}
WORD ConfigurationManager::GetMeshVisibility(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->MshMng->GetMeshVisibility(idx) : ((MeshSection*)Configurations[config]->Sections[MESH_SECTION])->GetMeshVisibility(idx);
}
UINT ConfigurationManager::GetDockCount(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->DckMng->GetDockCount() : ((DockSection*)Configurations[config]->Sections[DOCK_SECTION])->GetDockCount();
}
string ConfigurationManager::GetDockName(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->DckMng->GetDockName(idx) : ((DockSection*)Configurations[config]->Sections[DOCK_SECTION])->GetDockName(idx);
}
void ConfigurationManager::GetDockParams(UINT config, UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot) {
	return config == GetCurrentConfiguration() ?  VB1->DckMng->GetDockParams(idx, pos, dir, rot) : ((DockSection*)Configurations[config]->Sections[DOCK_SECTION])->GetDockParams(idx, pos, dir, rot);
}
bool ConfigurationManager::IsDockJettisonable(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->DckMng->IsDockJettisonable(idx) : ((DockSection*)Configurations[config]->Sections[DOCK_SECTION])->IsDockJettisonable(idx);
}
UINT ConfigurationManager::GetAttCount(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->AttMng->GetAttCount() : ((AttachmentSection*)Configurations[config]->Sections[ATT_SECTION])->GetAttCount();
}
bool ConfigurationManager::GetIdCheck(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AttMng->AttToParent(idx) : ((AttachmentSection*)Configurations[config]->Sections[ATT_SECTION])->AttToParent(idx);
}
string ConfigurationManager::GetIdCheckString(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AttMng->GetIdCheckString(idx) : ((AttachmentSection*)Configurations[config]->Sections[ATT_SECTION])->GetIdCheckString(idx);
}
string ConfigurationManager::GetAttID(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AttMng->GetAttDefId(idx) : ((AttachmentSection*)Configurations[config]->Sections[ATT_SECTION])->GetAttID(idx);
}
bool ConfigurationManager::AttToParent(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AttMng->AttToParent(idx) : ((AttachmentSection*)Configurations[config]->Sections[ATT_SECTION])->AttToParent(idx);
}
void ConfigurationManager::GetAttPosDirRot(UINT config, UINT idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot) {
	return config == GetCurrentConfiguration() ? VB1->AttMng->GetAttDefPosDirRot(idx, pos, dir, rot) : ((AttachmentSection*)Configurations[config]->Sections[ATT_SECTION])->GetAttPosDirRot(idx, pos, dir, rot);
}
double ConfigurationManager::GetAttRange(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AttMng->GetAttDefRange(idx) : ((AttachmentSection*)Configurations[config]->Sections[ATT_SECTION])->GetAttRange(idx);
}

UINT ConfigurationManager::GetAnimCount(UINT config) {
	return config == GetCurrentConfiguration() ? VB1->AnimMng->GetAnimDefsCount() : ((AnimationSection*)Configurations[config]->Sections[ANIM_SECTION])->GetAnimCount();
}
double ConfigurationManager::GetAnimDefState(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AnimMng->GetAnimDefState(idx) : ((AnimationSection*)Configurations[config]->Sections[ANIM_SECTION])->GetAnimDefState(idx);
}
string ConfigurationManager::GetAnimName(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AnimMng->GetAnimName(idx) : ((AnimationSection*)Configurations[config]->Sections[ANIM_SECTION])->GetAnimName(idx);
}
UINT ConfigurationManager::GetAnimNComps(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AnimMng->GetAnimNComps(idx) : ((AnimationSection*)Configurations[config]->Sections[ANIM_SECTION])->GetAnimNComps(idx);
}
DWORD ConfigurationManager::GetAnimKey(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AnimMng->GetAnimKey(idx) : ((AnimationSection*)Configurations[config]->Sections[ANIM_SECTION])->GetAnimKey(idx);
}
double ConfigurationManager::GetAnimDuration(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AnimMng->GetAnimDuration(idx) : ((AnimationSection*)Configurations[config]->Sections[ANIM_SECTION])->GetAnimDuration(idx);
}
AnimCycleType ConfigurationManager::GetAnimCycle(UINT config, UINT idx) {
	return config == GetCurrentConfiguration() ? VB1->AnimMng->GetAnimCycle(idx) : ((AnimationSection*)Configurations[config]->Sections[ANIM_SECTION])->GetAnimCycle(idx);
}*/