#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "AttachmentManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

AttachmentManager::AttachmentManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	att_defs.clear();
	return;
}
AttachmentManager::~AttachmentManager() {}

void AttachmentManager::CreateAttDef() {
	ATT_DEF attd = ATT_DEF();
	return CreateAttDef(attd);
}

void AttachmentManager::CreateAttDef(bool toparent, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot, string id,double range, bool loose,bool id_check,string id_check_string) {
	LogV("Creating Attachment: To Parent:%i pos:%.3f %.3f %.3f dir:%.3f %.3f %.3f rot:%.3f %.3f %.3f id:%s range:%.3f loose:%i", toparent, pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, rot.x, rot.y, rot.z, id.c_str(), range, loose);
	ATT_DEF attd = ATT_DEF();
	attd.toparent = toparent;
	attd.pos = pos;
	attd.dir = dir;
	attd.antidir = dir*(-1);
	attd.rot = rot;
	attd.antirot = rot*(-1);
	attd.id = id;
	attd.loose = loose;
	attd.range = range;
	attd.id_check = id_check;
	attd.id_check_string = id_check_string;
	return CreateAttDef(attd);
}

void AttachmentManager::CreateAttDef(ATT_DEF att_d) {
	ATT_DEF att = att_d;
	*att.pos_ptr = att.pos;
	*att.dir_ptr = att.dir;
	*att.antidir_ptr = att.dir*(-1);
	*att.antirot_ptr = att.rot*(-1);
	att.ah = VB1->CreateAttachment(att.toparent, att.pos, att.dir, att.rot, att.id.c_str(), att.loose);
	//oapiWriteLogV("Attachment count:%i %i", SB1->AttachmentCount(true), SB1->AttachmentCount(false));
	att.created = true;
	att_defs.push_back(att);
	return;
}
void AttachmentManager::DeleteAttDef(def_idx d_idx) {
	LogV("Deleting Attachment definition: %i", d_idx);
	VB1->DelAttachment(att_defs[d_idx].ah);
	delete att_defs[d_idx].pos_ptr;
	delete att_defs[d_idx].dir_ptr;
	delete att_defs[d_idx].antidir_ptr;
	delete att_defs[d_idx].antirot_ptr;
	att_defs.erase(att_defs.begin() + d_idx);
	LogV("Deleted");
	return;
}
void AttachmentManager::ModifyAttDef(def_idx d_idx, VECTOR3 pos, VECTOR3 dir, VECTOR3 rot) {
	VB1->SetAttachmentParams(att_defs[d_idx].ah, pos, dir, rot);
	att_defs[d_idx].pos = pos;
	*att_defs[d_idx].pos_ptr = pos;
	*att_defs[d_idx].dir_ptr = dir;
	*att_defs[d_idx].antidir_ptr = dir*(-1);
	*att_defs[d_idx].antirot_ptr = rot*(-1);
	att_defs[d_idx].dir = dir;
	att_defs[d_idx].antidir = dir*(-1);
	att_defs[d_idx].rot = rot;
	att_defs[d_idx].antirot = rot*(-1);
	
	return;
}
UINT AttachmentManager::GetAttCount() {
	return att_defs.size();
}
void AttachmentManager::AddAttDefNoCreate() {
	ATT_DEF attd = ATT_DEF();
	att_defs.push_back(attd);
}

bool AttachmentManager::AttIsCreated(def_idx d_idx) {
	return att_defs[d_idx].created;
}
string AttachmentManager::GetAttDefId(def_idx d_idx) {
	return att_defs[d_idx].id;
}
bool AttachmentManager::AttToParent(def_idx d_idx) {
	return att_defs[d_idx].toparent;
}
void AttachmentManager::GetAttDefPosDirRot(def_idx d_idx, VECTOR3 &pos, VECTOR3 &dir, VECTOR3 &rot) {
	pos = att_defs[d_idx].pos;
	dir = att_defs[d_idx].dir;
	rot = att_defs[d_idx].rot;
	return;
}
void AttachmentManager::SetAttDefID(def_idx d_idx, string id) {
	att_defs[d_idx].id = id;
	return;
}
void AttachmentManager::SetAttDefCreated(def_idx d_idx) {
	att_defs[d_idx].ah = VB1->CreateAttachment(att_defs[d_idx].toparent, att_defs[d_idx].pos, att_defs[d_idx].dir, att_defs[d_idx].rot, att_defs[d_idx].id.c_str(), att_defs[d_idx].loose);
	*att_defs[d_idx].pos_ptr = att_defs[d_idx].pos;
	*att_defs[d_idx].dir_ptr = att_defs[d_idx].dir;
	*att_defs[d_idx].antidir_ptr = att_defs[d_idx].dir*(-1);
	*att_defs[d_idx].antirot_ptr = att_defs[d_idx].rot*(-1);
	att_defs[d_idx].created = true;
	return;
}
void AttachmentManager::SetAttDefToParent(def_idx d_idx, bool toparent) {
	att_defs[d_idx].toparent = toparent;
	return;
}
VECTOR3* AttachmentManager::GetAttDefPosPtr(def_idx d_idx) {
	return att_defs[d_idx].pos_ptr;
}
VECTOR3* AttachmentManager::GetAttDefDirPtr(def_idx d_idx) {
	return att_defs[d_idx].dir_ptr;
}
VECTOR3* AttachmentManager::GetAttDefAntiDirPtr(def_idx d_idx) {
	return att_defs[d_idx].antidir_ptr;
}
VECTOR3* AttachmentManager::GetAttDefAntiRotPtr(def_idx d_idx) {
	return att_defs[d_idx].antirot_ptr;
}
void AttachmentManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Attachment Section");
	UINT att_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "ATT_%i_IDX", att_counter);
	int idx;
	while (oapiReadItem_int(fh, cbuf, idx)) {
		VECTOR3 pos, dir, rot;
		char idbuf[256] = { '\0' };
		bool toparent = false;
		sprintf(cbuf, "ATT_%i_POS",att_counter);
		oapiReadItem_vec(fh, cbuf, pos);
		sprintf(cbuf, "ATT_%i_DIR", att_counter);
		oapiReadItem_vec(fh, cbuf, dir);
		sprintf(cbuf, "ATT_%i_ROT", att_counter);
		oapiReadItem_vec(fh, cbuf, rot);
		double range = 10;
		sprintf(cbuf, "ATT_%i_RANGE", att_counter);
		oapiReadItem_float(fh, cbuf, range);
		sprintf(cbuf, "ATT_%i_ID", att_counter);
		oapiReadItem_string(fh, cbuf, idbuf);
		string id(idbuf);
		sprintf(cbuf, "ATT_%i_TOPARENT", att_counter);
		oapiReadItem_bool(fh, cbuf, toparent);

		bool id_check;
		string id_check_string;
		id_check_string.clear();
		sprintf(cbuf, "ATT_%i_IDCHECK", att_counter);
		if (!oapiReadItem_bool(fh, cbuf, id_check)) { id_check = false; }
		if (id_check) {
			sprintf(cbuf, "ATT_%i_IDCHECKSTRING", att_counter);
			char checkbuff[256] = { '\0' };
			oapiReadItem_string(fh, cbuf, checkbuff);
			id_check_string.assign(checkbuff);
		}

		CreateAttDef(toparent, pos, dir, rot, id,range, false,id_check,id_check_string);

		


		att_counter++;
		sprintf(cbuf, "ATT_%i_IDX", att_counter);
	}
	LogV("Parsing Attachment Section Completed");
	LogV("Found %i Attachment Definitions", att_counter);
	return;
}
void AttachmentManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------ATTACHMENTS DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");

	UINT counter = 0;
	
	for (UINT i = 0; i < GetAttCount(); i++) {
		if (!AttIsCreated(i)) { continue; }
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "ATT_%i_IDX", counter);
		oapiWriteItem_int(fh, cbuf, counter);
		sprintf(cbuf, "ATT_%i_POS", counter);
		oapiWriteItem_vec(fh, cbuf, att_defs[i].pos);
		sprintf(cbuf, "ATT_%i_DIR", counter);
		oapiWriteItem_vec(fh, cbuf, att_defs[i].dir);
		sprintf(cbuf, "ATT_%i_ROT", counter);
		oapiWriteItem_vec(fh, cbuf, att_defs[i].rot);
		sprintf(cbuf, "ATT_%i_RANGE", counter);
		oapiWriteItem_float(fh, cbuf, att_defs[i].range);
		sprintf(cbuf, "ATT_%i_ID", counter);
		char cbuf2[256] = { '\0' };
		sprintf(cbuf2, att_defs[i].id.c_str());
		oapiWriteItem_string(fh, cbuf, cbuf2);
		sprintf(cbuf, "ATT_%i_TOPARENT", counter);
		oapiWriteItem_bool(fh, cbuf, att_defs[i].toparent);
		sprintf(cbuf, "ATT_%i_IDCHECK", counter);
		oapiWriteItem_bool(fh, cbuf, att_defs[i].id_check);
		if (att_defs[i].id_check)
		{
			sprintf(cbuf, "ATT_%i_IDCHECKSTRING", counter);
			char checkbuff[256] = { '\0' };
			sprintf(checkbuff, "%s", att_defs[i].id_check_string.c_str());
			oapiWriteItem_string(fh, cbuf, checkbuff);
		}
		oapiWriteLine(fh, " ");
		counter++;
		
	}
	return;
}

ATTACHMENTHANDLE AttachmentManager::GetAttDefAH(def_idx d_idx) {
	return att_defs[d_idx].ah;
}
def_idx AttachmentManager::IdxAtt2Def(UINT att_idx) {
	ATTACHMENTHANDLE ah = VB1->GetAttachmentHandle(false, att_idx);
	for (UINT i = 0; i < GetAttCount(); i++) {
		if (att_defs[i].ah == ah) {
			return i;
		}
	}
	return 0;
}

void AttachmentManager::SetAttDefRange(def_idx d_idx, double newrange) {
	att_defs[d_idx].range = newrange;
}
double AttachmentManager::GetAttDefRange(def_idx d_idx) {
	return att_defs[d_idx].range;
}
void AttachmentManager::Clear() {
	LogV("Clearing Attachments");
	VB1->ClearAttachments();
	for (UINT i = 0; i < att_defs.size(); i++) {
		delete att_defs[i].pos_ptr;
		delete att_defs[i].dir_ptr;
		delete att_defs[i].antidir_ptr;
		delete att_defs[i].antirot_ptr;
	}
	att_defs.clear();
	LogV("Clearing Attachments Completed");
	return;
}

void AttachmentManager::SetIdCheck(def_idx d_idx, bool set) {
	att_defs[d_idx].id_check = set;
	return;
}
bool AttachmentManager::GetIdCheck(def_idx d_idx) {
	return att_defs[d_idx].id_check;
}
void AttachmentManager::SetIdCheckString(def_idx d_idx, string _id_check_string) {
	att_defs[d_idx].id_check_string = _id_check_string;
	return;
}
string AttachmentManager::GetIdCheckString(def_idx d_idx) {
	return att_defs[d_idx].id_check_string;
}