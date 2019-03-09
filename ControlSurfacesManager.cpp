#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "ControlSurfacesManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

ControlSurfacesManager::ControlSurfacesManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	ctrlsurf_defs.clear();
	return;
}
ControlSurfacesManager::~ControlSurfacesManager() {
	VB1 = NULL;
	return;
}
UINT ControlSurfacesManager::CreateUndefinedCtrlSurfDef() {
	UINT index = ctrlsurf_defs.size();
	char nbuf[256] = { '\0' };
	sprintf(nbuf, "Control_Surf_%i", index);
	string name(nbuf);
	return CreateUndefinedCtrlSurfDef(name, AIRCTRL_AILERON, 1, 1.5, _V(0, 0, 0), AIRCTRL_AXIS_AUTO, 1, (UINT)-1);
}
UINT ControlSurfacesManager::CreateUndefinedCtrlSurfDef(string name, AIRCTRL_TYPE type, double area, double dCl, VECTOR3 ref, int axis, double delay, UINT anim) {
	CTRLSURF_DEF ctrsd = CTRLSURF_DEF();
	ctrsd.name = name;
	ctrsd.type = type;
	ctrsd.area = area;
	ctrsd.dCl = dCl;
	ctrsd.ref = ref;
	ctrsd.axis = axis;
	ctrsd.delay = delay;
	ctrsd.anim = anim;
	UINT index = ctrlsurf_defs.size();
	ctrlsurf_defs.push_back(ctrsd);
	return index;
}
void ControlSurfacesManager::SetCtrlSurfDefParams(def_idx d_idx, AIRCTRL_TYPE type, double area, double dCl, VECTOR3 ref, int axis, double delay, UINT anim) {
	ctrlsurf_defs[d_idx].type = type;
	ctrlsurf_defs[d_idx].area = area;
	ctrlsurf_defs[d_idx].dCl = dCl;
	ctrlsurf_defs[d_idx].ref = ref;
	ctrlsurf_defs[d_idx].axis = axis;
	ctrlsurf_defs[d_idx].delay = delay;
	ctrlsurf_defs[d_idx].anim = anim;
	return;
}
void ControlSurfacesManager::DefineCtrlSurface(def_idx d_idx) {
	LogV("Defining CtrlSurface:%i", d_idx);
	ctrlsurf_defs[d_idx].csfh = VB1->CreateControlSurface3(ctrlsurf_defs[d_idx].type, ctrlsurf_defs[d_idx].area, ctrlsurf_defs[d_idx].dCl, ctrlsurf_defs[d_idx].ref, ctrlsurf_defs[d_idx].axis, ctrlsurf_defs[d_idx].delay, ctrlsurf_defs[d_idx].anim);
	ctrlsurf_defs[d_idx].defined = true;
	return;
}
void ControlSurfacesManager::UndefineCtrlSurface(def_idx d_idx) {
	-VB1->DelControlSurface(ctrlsurf_defs[d_idx].csfh);
	ctrlsurf_defs[d_idx].csfh = NULL;
	ctrlsurf_defs[d_idx].defined = false;
	return;
}
void ControlSurfacesManager::DeleteCtrlSurfDef(def_idx d_idx) {
	if (IsCtrlSurfDefDefined(d_idx)) {
		UndefineCtrlSurface(d_idx);
	}
	ctrlsurf_defs.erase(ctrlsurf_defs.begin() + d_idx);
	return;
}
bool ControlSurfacesManager::IsCtrlSurfDefDefined(def_idx d_idx) {
	return ctrlsurf_defs[d_idx].defined;
}
UINT ControlSurfacesManager::CtrlSurfDefCount() {
	return ctrlsurf_defs.size();
}
string ControlSurfacesManager::GetCtrlSurfDefName(def_idx d_idx) {
	return ctrlsurf_defs[d_idx].name;
}
void ControlSurfacesManager::SetCtrlSurfDefName(def_idx d_idx, string newname) {
	ctrlsurf_defs[d_idx].name = newname;
	return;
}
void ControlSurfacesManager::GetCtrlSurfDefParams(def_idx d_idx, AIRCTRL_TYPE &type, double &area, double &dCl, VECTOR3 &ref, int &axis, double &delay, UINT &anim) {
	type = ctrlsurf_defs[d_idx].type;
	area = ctrlsurf_defs[d_idx].area;
	dCl = ctrlsurf_defs[d_idx].dCl;
	ref = ctrlsurf_defs[d_idx].ref;
	axis = ctrlsurf_defs[d_idx].axis;
	delay = ctrlsurf_defs[d_idx].delay;
	anim = ctrlsurf_defs[d_idx].anim;
	return;
}
void ControlSurfacesManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Control Surfaces Section");
	char cbuf[256] = { '\0' };
	UINT ctr_counter = 0;
	int id;
	sprintf(cbuf, "CTRL_SURFACES_%i_ID", ctr_counter);
	while (oapiReadItem_int(fh, cbuf, id)) {
		AIRCTRL_TYPE type;
		double area, dcl, delay;
		int axis;
		UINT anim;
		VECTOR3 ref;
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "CTRL_SURFACES_%i_NAME", ctr_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		sprintf(cbuf, "CTRL_SURFACES_%i_TYPE", ctr_counter);
		int tp;
		oapiReadItem_int(fh, cbuf, tp);
		type = (AIRCTRL_TYPE)tp;
		sprintf(cbuf, "CTRL_SURFACES_%i_AREA", ctr_counter);
		oapiReadItem_float(fh, cbuf, area);
		sprintf(cbuf, "CTRL_SURFACES_%i_DCL", ctr_counter);
		oapiReadItem_float(fh, cbuf, dcl);
		sprintf(cbuf, "CTRL_SURFACES_%i_REF", ctr_counter);
		oapiReadItem_vec(fh, cbuf, ref);
		sprintf(cbuf, "CTRL_SURFACES_%i_AXIS", ctr_counter);
		oapiReadItem_int(fh, cbuf, axis);
		sprintf(cbuf, "CTRL_SURFACES_%i_DELAY", ctr_counter);
		oapiReadItem_float(fh, cbuf, delay);
		sprintf(cbuf, "CTRL_SURFACES_%i_ANIM", ctr_counter);
		int an;
		oapiReadItem_int(fh, cbuf, an);
		anim = (UINT)an;

		UINT index = CreateUndefinedCtrlSurfDef(name, type, area, dcl, ref, axis, delay, anim);
		DefineCtrlSurface(index);

		ctr_counter++;
		sprintf(cbuf, "CTRL_SURFACES_%i_ID", ctr_counter);
	}
	
	
	LogV("Parsing Control Surfaces Section Completed, found %i definitions",ctr_counter);
	
	
	return;
}
void ControlSurfacesManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, " ");
	oapiWriteLine(fh, ";<-------------------------CONTROL SURFACES DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	UINT ctr_counter = 0;
	for (UINT i = 0; i < CtrlSurfDefCount(); i++) {
		if (IsCtrlSurfDefDefined(i)) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "CTRL_SURFACES_%i_ID", ctr_counter);
			oapiWriteItem_int(fh, cbuf, ctr_counter);
			sprintf(cbuf, "CTRL_SURFACES_%i_NAME", ctr_counter);
			char namebuf[256] = { '\0' };
			sprintf(namebuf, "%s", GetCtrlSurfDefName(i).c_str());
			oapiWriteItem_string(fh, cbuf, namebuf);
			AIRCTRL_TYPE type;
			double area, dcl, delay;
			int axis;
			UINT anim;
			VECTOR3 ref;
			GetCtrlSurfDefParams(i, type, area, dcl, ref, axis, delay, anim);
			sprintf(cbuf, "CTRL_SURFACES_%i_TYPE", ctr_counter);
			oapiWriteItem_int(fh, cbuf, (int)type);
			sprintf(cbuf, "CTRL_SURFACES_%i_AREA", ctr_counter);
			oapiWriteItem_float(fh, cbuf, area);
			sprintf(cbuf, "CTRL_SURFACES_%i_DCL", ctr_counter);
			oapiWriteItem_float(fh, cbuf, dcl);
			sprintf(cbuf, "CTRL_SURFACES_%i_REF", ctr_counter);
			oapiWriteItem_vec(fh, cbuf, ref);
			sprintf(cbuf, "CTRL_SURFACES_%i_AXIS", ctr_counter);
			oapiWriteItem_int(fh, cbuf, axis);
			sprintf(cbuf, "CTRL_SURFACES_%i_DELAY", ctr_counter);
			oapiWriteItem_float(fh, cbuf, delay);
			sprintf(cbuf, "CTRL_SURFACES_%i_ANIM", ctr_counter);
			oapiWriteItem_int(fh, cbuf, anim);
			oapiWriteLine(fh, " ");
			ctr_counter++;
		}
	}
	return;
}
void ControlSurfacesManager::Clear() {
	LogV("Clearing CtrlSurfaces");
	VB1->ClearControlSurfaceDefinitions();
	ctrlsurf_defs.clear();
	LogV("Clearing CtrlSurfaces Completed");
	return;
}