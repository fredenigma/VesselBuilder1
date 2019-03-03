#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "ExTexManager.h"

ExTexManager::ExTexManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	extex_defs.clear();
	return;
}
ExTexManager::~ExTexManager() {
	VB1 = NULL;
	return;
}


void ExTexManager::AddExTexDef() {
	EXTEX_DEF ex = EXTEX_DEF();
	extex_defs.push_back(ex);
	return;
}
void ExTexManager::AddExTexDef(string texname) {
	EXTEX_DEF ex = EXTEX_DEF();
	ex.TexName = texname;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%s", texname.c_str());
	ex.tex = oapiRegisterExhaustTexture(cbuf);
	ex.created = true;
	extex_defs.push_back(ex);
	return;
}
bool ExTexManager::StoreExTexDef(string texname, def_idx d_idx) {
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
void ExTexManager::DelExTedDef(def_idx d_idx) {
	extex_defs.erase(extex_defs.begin() + d_idx);
	return;
}
SURFHANDLE ExTexManager::GetExTexSurf(def_idx d_idx) {
	return extex_defs[d_idx].tex;
}
string ExTexManager::GetExTexName(def_idx d_idx) {
	return extex_defs[d_idx].TexName;
}
UINT ExTexManager::GetExTexCount() {
	return extex_defs.size();
}
bool ExTexManager::IsExTexCreated(def_idx d_idx) {
	return extex_defs[d_idx].created;
}
int ExTexManager::GetExTexIdx(SURFHANDLE tex) {
	if (tex == NULL) { return -1; }
	for (int i = 0; i < extex_defs.size(); i++) {
		if (extex_defs[i].tex == tex) {
			return i;
		}
	}
	return -1;
}

void ExTexManager::ParseCfgFile(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
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
	return;
}
void ExTexManager::WriteCfg(FILEHANDLE fh) {
	char cbuf[256] = { '\0' };
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
	return;
}
void ExTexManager::Clear() {
	extex_defs.clear();
	return;
}