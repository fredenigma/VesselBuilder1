#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "DockManager.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"
#include "ConfigurationManager.h"

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
void AnimationSection::ApplySection() {
	AnimMng->Clear();
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

	}
	return;
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