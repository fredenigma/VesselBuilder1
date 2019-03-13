#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "ConfigurationManager.h"
#include "MeshManager.h"
#include "DockManager.h"
#include "AttachmentManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

Section::Section(VesselBuilder1* _VB1, UINT _config, FILEHANDLE cfg) {
	VB1 = _VB1;
	Config_idx = _config;
	//ParseSection(cfg);
	return;
}

Section::~Section() { VB1 = NULL; }
void Section::WriteSection(FILEHANDLE fh){}
void Section::ParseSection(FILEHANDLE fh) {}
void Section::ApplySection() {}
void Section::UpdateSection(){}

void Section::ConfigCheck(char* cbuf, UINT config) {
	char abuf[256] = { '\0' };
	if (config != 0) {
		strcpy(abuf, cbuf);
		sprintf(cbuf, "%i_%s", config, abuf);
	}
	return;
}
MeshSection::MeshSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1,config, cfg){
	Defs.clear();
	MshMng = VB1->MshMng;
	ParseSection(cfg);
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
void MeshSection::ApplySection() {
	MshMng->Clear();
	for (UINT i = 0; i < Defs.size(); i++) {
		MshMng->AddMeshDef(Defs[i].meshname, Defs[i].pos, Defs[i].dir, Defs[i].rot, Defs[i].visibility);
		MshMng->PreLoadMeshes();
		MshMng->AddMeshes();
	}
}

DockSection::DockSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	DckMng = VB1->DckMng;
	ParseSection(cfg);
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
		if (!oapiReadItem_bool(fh, cbuf, jett)) { jett = false; }
		Definitions d = Definitions();
		d.name = name;
		d.pos = pos;
		d.dir = dir;
		d.rot = rot;
		d.jettisonable = jett;
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
		VECTOR3 pos, dir, rot;
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
		d.jettisonable = DckMng->IsDockJettisonable(i);
		Defs.push_back(d);
	}
}

void DockSection::ApplySection() {
	DckMng->Clear();
	for (UINT i = 0; i < Defs.size(); i++) {
		DckMng->AddDockDef(Defs[i].name, Defs[i].pos, Defs[i].dir, Defs[i].rot, Defs[i].jettisonable);
	}
}

AttachmentSection::AttachmentSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	AttMng = VB1->AttMng;
	ParseSection(cfg);
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

void AttachmentSection::ApplySection() {
	AttMng->Clear();
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
Configuration::Configuration(VesselBuilder1 *_VB1, map<ItemType, bool> _Sections, UINT _config, FILEHANDLE _cfg) {
	VB1 = _VB1;
	Configuration_Sections = _Sections;
	/*Msh_Sect = NULL;
	Dck_Sect = NULL;
	Att_Sect = NULL;*/
	if (Configuration_Sections[MESH]) {
		//Msh_Sect = new MeshSection(_VB1,_config, _cfg);
		//Sections.push_back(Msh_Sect);
		Sections.push_back(new MeshSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[DOCK]) {
		//Dck_Sect = new DockSection(_VB1, _config, _cfg);
		//Sections.push_back(Dck_Sect);
		Sections.push_back(new DockSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[ATTACHMENT]) {
		//Att_Sect = new AttachmentSection(_VB1, _config, _cfg);
		//Sections.push_back(Att_Sect);
		Sections.push_back(new AttachmentSection(_VB1, _config, _cfg));
	}
	Config_idx = _config;
	return;
	//CommonInit();
}



Configuration::~Configuration(){
	for (UINT i = 0; i < Sections.size(); i++) {
		delete Sections[i];
	}
	Sections.clear();
	/*delete Msh_Sect;
	Msh_Sect = NULL;
	delete Dck_Sect;
	Dck_Sect = NULL;
	delete Att_Sect;
	Att_Sect = NULL;*/
}
void Configuration::Apply(){
	for (UINT i = 0; i < Sections.size(); i++) {
		Sections[i]->ApplySection();
	}
/*	if (Configuration_Sections[MESH]) {
		Msh_Sect->ApplySection();
	}
	if (Configuration_Sections[DOCK]) {
		Dck_Sect->ApplySection();
	}
	if (Configuration_Sections[ATTACHMENT]) {
		Att_Sect->ApplySection();
	}*/
}
void Configuration::Write(FILEHANDLE cfg) {
	for (UINT i = 0; i < Sections.size(); i++) {
		Sections[i]->WriteSection(cfg);
	}
	/*if (Configuration_Sections[MESH]) {
		Msh_Sect->WriteSection(cfg);
	}
	if (Configuration_Sections[DOCK]) {
		Dck_Sect->WriteSection(cfg);
	}
	if (Configuration_Sections[ATTACHMENT]) {
		Att_Sect->WriteSection(cfg);
	}*/

}
void Configuration::Update() {
	for (UINT i = 0; i < Sections.size(); i++) {
		Sections[i]->UpdateSection();
	}
	/*if (Configuration_Sections[MESH]) {
		Msh_Sect->UpdateSection();
	}
	if (Configuration_Sections[DOCK]) {
		Dck_Sect->UpdateSection();
	}
	if (Configuration_Sections[ATTACHMENT]) {
		Att_Sect->UpdateSection();
	}*/
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

void ConfigurationManager::AddConfiguration(VesselBuilder1* VB1, map<ItemType, bool>_Sections, FILEHANDLE cfg) {
	Configuration *Config = new Configuration(VB1, _Sections, config_counter, cfg);
	config_counter++;
	Configurations.push_back(Config);
	return;
}



void ConfigurationManager::ApplyConfiguration(UINT config, bool firstload) {
	if (config >= Configurations.size()) {
		return;
	}
	if (!firstload) {
		UINT old_config = CurrentConfiguration;
		Configurations[old_config]->Update();
	}
	Configurations[config]->Apply();
	CurrentConfiguration = config;
	return;
}
void ConfigurationManager::ApplyDefaultConfiguration(bool firstload) {
	if (Configurations.size() > 0) {
		if (!firstload) {
			UINT old_config = CurrentConfiguration;
			Configurations[old_config]->Update();
		}
		Configurations[0]->Apply();
	}
	CurrentConfiguration = 0;
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