#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "DockManager.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"
#include "ConfigurationManager.h"
#include "PropellantManager.h"
#include "ExTexManager.h"
#include "ParticleManager.h"
#include "ThrusterManager.h"
#include "TouchdownPointsManager.h"

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
void Section::ManagerClear(){}

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
void MeshSection::ManagerClear() {
	MshMng->Clear();
}
void MeshSection::ApplySection() {
	for (UINT i = 0; i < Defs.size(); i++) {
		MshMng->AddMeshDef(Defs[i].meshname, Defs[i].pos, Defs[i].dir, Defs[i].rot, Defs[i].visibility);
	}
	MshMng->PreLoadMeshes();
	MshMng->AddMeshes();
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
void DockSection::ManagerClear() {
	DckMng->Clear();
}
void DockSection::ApplySection() {
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
void AttachmentSection::ManagerClear() {
	AttMng->Clear();
}
void AttachmentSection::ApplySection() {
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

AnimationSection::AnimationSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	AnimDefs.clear();
	AnimCompDefs.clear();
	AnimMng = VB1->AnimMng;
	ParseSection(cfg);
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
		vector<UINT>grps;
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
		vector<UINT> grps = AnimCompDefs[i].animcomp_grps;
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
	}
	return;
}


PropellantSection::PropellantSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	PrpMng = VB1->PrpMng;
	ParseSection(cfg);
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


ThrusterSection::ThrusterSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Defs.clear();
	ThrMng = VB1->ThrMng;
	ParseSection(cfg);
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
				oapiReadItem_bool(fh, cbuf, customposdir);
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
				oapiReadItem_bool(fh, cbuf, custompos);
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
				d.Exhausts.push_back(ed);
			}	
		}
		d.hasparticle = ThrMng->HasThrParticles(i);
		if (d.hasparticle) {
			for (UINT j = 0; j < ThrMng->GetThrParticlesCount(i); j++) {
				Definitions::ParticleDefs pd;
				UINT pss_idx = (UINT)pd.pss_int;
				ThrMng->GetThrParticleParams(i, j, pss_idx, pd.custompos, pd.pos);
				d.Particles.push_back(pd);
			}
		}
		Defs.push_back(d);
	}
}
void ThrusterSection::ManagerClear() {
	ThrMng->Clear();
}




ThrusterGroupSection::ThrusterGroupSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	vector<UINT>nonum;
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
	ParseSection(cfg);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
		vector<UINT> idx = VB1->readVectorUINT(idx_s);
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
	map<THGROUP_TYPE, bool>::iterator it;
	//UINT counter = 0;
	for (it = Defined.begin(); it != Defined.end(); it++) {
		//LogV("Counter:%i", counter);
		if (it->second) {
			//LogV("true:%i",counter);
			vector<THRUSTER_HANDLE>thv = ThrGrpMng->GetThrustersfromIdx(Thrusters[it->first]);
			ThrGrpMng->DefineGroup(it->first, thv);
		}
		//counter++;
	}
	return;
}
void ThrusterGroupSection::UpdateSection() {
	Defined = ThrGrpMng->Defined;
	map<THGROUP_TYPE, bool>::iterator it;
	vector<UINT>indexes;
	indexes.clear();
	for (it = Defined.begin(); it != Defined.end(); it++) {
		if (it->second) {
			indexes.clear();
			vector<THRUSTER_HANDLE>thrusters;
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

TouchDownPointSection::TouchDownPointSection(VesselBuilder1* VB1, UINT config, FILEHANDLE cfg) :Section(VB1, config, cfg) {
	Set1.clear();
	Set2.clear();
	SecondSetEnabled = false;
	changeoveranim = -1;
	TdpMng = VB1->TdpMng;
	ParseSection(cfg);
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




Configuration::Configuration(VesselBuilder1 *_VB1, map<ItemType, bool> _Sections, UINT _config, FILEHANDLE _cfg) {
	VB1 = _VB1;
	Configuration_Sections = _Sections;
	LogV("Adding Configuration n:%i", _config);
	if (Configuration_Sections[MESH]) {
		LogV("Mesh Section included");
		Sections.push_back(new MeshSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[DOCK]) {
		LogV("Dock Section included");
		Sections.push_back(new DockSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[ATTACHMENT]) {
		LogV("Attachments Section included");
		Sections.push_back(new AttachmentSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[ANIMATIONS]) {
		LogV("Animations Section included");
		Sections.push_back(new AnimationSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[PROPELLANT]) {
		LogV("Propellants Section included");
		Sections.push_back(new PropellantSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[THRUSTERS]) {
		LogV("Thrusters Section included");
		Sections.push_back(new ThrusterSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[THRUSTERGROUPS]) {
		LogV("ThrusterGroup Section included");
		Sections.push_back(new ThrusterGroupSection(_VB1, _config, _cfg));
	}
	if (Configuration_Sections[TOUCHDOWNPOINTS]) {
		LogV("Touchdown Points Section included");
		Sections.push_back(new TouchDownPointSection(_VB1, _config, _cfg));
	}
	Config_idx = _config;
	return;
	
}



Configuration::~Configuration(){
	for (UINT i = 0; i < Sections.size(); i++) {
		delete Sections[i];
	}
	Sections.clear();
	
}
void Configuration::Apply(){
	for (UINT i = 0; i < Sections.size(); i++) {
		UINT p = Sections.size() -1- i;
		Sections[p]->ManagerClear();
	}
	for (UINT i = 0; i < Sections.size(); i++) {
		Sections[i]->ApplySection();
	}

}
void Configuration::Write(FILEHANDLE cfg) {
	for (UINT i = 0; i < Sections.size(); i++) {
		Sections[i]->WriteSection(cfg);
	}
	

}
void Configuration::Update() {
	for (UINT i = 0; i < Sections.size(); i++) {
		Sections[i]->UpdateSection();
	}
	
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