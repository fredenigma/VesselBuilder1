#include "VesselBuilder1.h"
#include "SoftDock.h"
#include "DockManager.h"
#include "AnimationManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

SoftDock::SoftDock(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	DckMng = VB1->DckMng;
	hvessel = NULL;
	h_dock = NULL;
	my_dh = NULL;
	SDSystemEnabled = false;
	Active = false;
	note_sd = oapiCreateAnnotation(true, 0.5, _V(0.6, 0.6, 0.6));
}
SoftDock::~SoftDock() {
	VB1 = NULL;
	DckMng = NULL;
}
void SoftDock::SetSoftDockParams(def_idx d_idx) {
	bool enable;
	DckMng->GetSoftDockParams(d_idx, enable, soft_dock_distance, anim_idx);
	my_dh = DckMng->GetDH(d_idx);
	//anim_idx = _anim_idx;
	//my_dh = _my_dh;
	VB1->GetDockParams(my_dh, pos, dir, rot);
	//soft_dock_distance = _soft_dock_distance;
	return;

}
bool SoftDock::IsOBJinVector(OBJHANDLE h_obj, VBVector<OBJHANDLE>vh) {
	for (UINT i = 0; i < vh.size(); i++) {
		if (h_obj == vh[i]) {
			return true;
		}
	}
	return false;
}
bool SoftDock::ChooseDock(def_idx &d_idx,OBJHANDLE &hvessel) {
	//DOCKHANDLE dh_candidate = NULL;
	int idx = -1;
	OBJHANDLE h_ves_candidate = NULL;
	VBVector<OBJHANDLE>linked;
	linked.clear();
	UINT dck_count = VB1->DockCount();
	DWORD att_count = VB1->AttachmentCount(false);
	for (UINT d = 0; d < dck_count; d++) {
		DOCKHANDLE dh = VB1->GetDockHandle(d);
		OBJHANDLE hd = VB1->GetDockStatus(dh);
		if (hd != NULL) {
			linked.push_back(hd);
		}
	}
	for (DWORD a = 0; a < att_count; a++) {
		ATTACHMENTHANDLE ah = VB1->GetAttachmentHandle(false, a);
		OBJHANDLE ha = VB1->GetAttachmentStatus(ah);
		if (ha != NULL) {
			linked.push_back(ha);
		}
	}
	for (UINT i = 0; i < DckMng->GetDockCount(); i++) {
		bool SD;
		double sd_distance;
		UINT anim;
		DckMng->GetSoftDockParams(i, SD, sd_distance, anim);
		
		if (!SD) { continue; }
		VECTOR3 dpos, ddir, drot;
		DckMng->GetDockParams(i, dpos, ddir, drot);
		VECTOR3 dlpos, dgpos;
		dlpos = dpos + (ddir*sd_distance);
		VB1->Local2Global(dlpos, dgpos); //global position of soft dock point
		double v_dist = 1 * AU;
		for (DWORD j = 0; j < oapiGetVesselCount(); j++) {
			OBJHANDLE h_ves = oapiGetVesselByIndex(j);
			if (h_ves == VB1->GetHandle()) { continue; }
			if (IsOBJinVector(h_ves, linked)) { continue; }
			VECTOR3 grpos;
			VB1->GetRelativePos(h_ves, grpos);
			double vessel_distance = length(grpos);
			if ((vessel_distance < (20 * oapiGetSize(h_ves))) && ((vessel_distance < v_dist))) {
				v_dist = vessel_distance;
				
				//dh_candidate = DckMng->GetDH(i);
				idx =(int)i;
				
				h_ves_candidate = h_ves;
			}
		}
	}
	if (idx != -1) {
		//dh = dh_candidate;
		d_idx = (def_idx)idx;
		hvessel = h_ves_candidate;
		return true;
	}
	else {
		return false;
	}
}
bool SoftDock::SoftDockCheck(OBJHANDLE &hves,DOCKHANDLE &hdock) {

	hves = NULL;
	hdock = NULL;
	/*VECTOR3 dlpos, dgpos;
	dlpos = pos + (dir*soft_dock_distance);
	VB1->Local2Global(dlpos, dgpos); //global position of soft dock point
	for (DWORD i = 0; i < oapiGetVesselCount(); i++) {
		OBJHANDLE h_ves = oapiGetVesselByIndex(i);
		if (h_ves == VB1->GetHandle()) { continue; }
		VECTOR3 grpos;
		VB1->GetRelativePos(h_ves, grpos);
		double vessel_distance = length(grpos);
		if (vessel_distance < 2 * oapiGetSize(h_ves)) {*/
	def_idx idx;
	OBJHANDLE h_ves;
	if(ChooseDock(idx,h_ves)){
		SetSoftDockParams(idx);
		VECTOR3 dlpos, dgpos;
		dlpos = pos + (dir*soft_dock_distance);
		VB1->Local2Global(dlpos, dgpos); //global position of soft dock point
		VESSEL *v = oapiGetVesselInterface(h_ves);
		UINT his_dock_count = v->DockCount();
		for (UINT j = 0; j < his_dock_count; j++) {
			DOCKHANDLE dh = v->GetDockHandle(j);
			VECTOR3 his_dpos, his_ddir, his_drot;
			v->GetDockParams(dh, his_dpos, his_ddir, his_drot);
			VECTOR3 his_dgpos;
			v->Local2Global(his_dpos, his_dgpos);
			double docks_distance = dist(dgpos, his_dgpos);
			double his_dock_ves_distance = length(his_dpos);
			VECTOR3 his_gpos;
			v->GetGlobalPos(his_gpos);
			double my_dock_his_ves_distance = dist(dgpos, his_gpos);
			if ((docks_distance < (0.75*soft_dock_distance))&&((my_dock_his_ves_distance)<=his_dock_ves_distance)) { //we are there, now check alignment
				MATRIX3 his_rm, my_rm;
				v->GetRotationMatrix(his_rm);
				VB1->GetRotationMatrix(my_rm);
				//rot part (easier)
				VECTOR3 his_g_rot, my_g_rot;
				his_g_rot = mul(his_rm, his_drot);
				my_g_rot = mul(my_rm, rot);
				double angle_rot = acos(dotp(his_g_rot, my_g_rot));
				if (angle_rot > 5 * RAD) {
					return false;
				}
				VECTOR3 his_ddir_rev,his_g_dir, my_g_dir;
				his_ddir_rev = his_ddir*(-1);
				his_g_dir = mul(his_rm, his_ddir_rev);
				my_g_dir = mul(my_rm, dir);
				double angle_dir = acos(dotp(his_g_dir, my_g_dir));
				if (angle_dir > 5 * RAD) {
					return false;
				}
				else {
					hves = h_ves;
					hdock = dh;
					return true;
				}
			}
		}
	}
	return false;
}
void SoftDock::EnableSoftDock(bool enable) {
	Active = enable;
	if (enable == false) {
		//sprintf(oapiDebugString(), "");
		oapiAnnotationSetText(note_sd, "");
	}
}
void SoftDock::PreStep() {
	if (!IsSDSysEnabled()) { //sprintf(oapiDebugString(), ""); 
		oapiAnnotationSetText(note_sd, ""); 
		return; 
	}
	//sprintf(oapiDebugString(), "SoftDock System Active");
	oapiAnnotationSetText(note_sd, "SoftDock System Active");
	if (!SoftDockActive()) {
		if (SoftDockCheck(hvessel, h_dock)) {
			EnableSoftDock(true);
		}
	}
	else {
		StationKeeping();
	}

	return;
}
void SoftDock::StationKeeping() {
	if (VB1->GetDockStatus(my_dh) != NULL) { //sprintf(oapiDebugString(), "HARD DOCK"); 
	oapiAnnotationSetText(note_sd, "HARD DOCK"); 
	return; 
	}
	VESSEL* v = oapiGetVesselInterface(hvessel);
	VESSELSTATUS2 vs2;
	memset(&vs2, 0, sizeof(vs2));
	vs2.version = 2;
	v->GetStatusEx(&vs2);
	VECTOR3 relative_pos;
//	VECTOR3 my_gpos, his_gpos;
	VECTOR3 dock_iss_pos, dock_iss_dir, dock_iss_rot;
	v->GetDockParams(h_dock, dock_iss_pos, dock_iss_dir, dock_iss_rot);
	VECTOR3 dock_my_pos, dock_my_dir, dock_my_rot;
	VB1->GetDockParams(my_dh, dock_my_pos, dock_my_dir, dock_my_rot);
	double distance = soft_dock_distance*VB1->AnimMng->GetAnimationState(anim_idx);
	//VB1->Local2Global(dock_my_pos+((dock_my_dir)*distance),my_gpos);
	//v->Local2Global(dock_iss_pos, his_gpos);
	//relative_pos = my_gpos - his_gpos;
	//VECTOR3 rloc_pos;
	//v->Global2Local(relative_pos,rloc_pos);
	VECTOR3 my_dock_final_pos = ((dock_my_pos + ((dock_my_dir)*distance))*(-1));
	MATRIX3 iss_rm;
	v->GetRotationMatrix(iss_rm);
	MATRIX3 my_rm;
	VB1->GetRotationMatrix(my_rm);
	MATRIX3 iss_rm_inverse = VB1->Inverse(iss_rm);
	relative_pos = mul(iss_rm_inverse,mul(my_rm,my_dock_final_pos)) + dock_iss_pos;
	v->Local2Rel(relative_pos, vs2.rpos);
	VECTOR3 my_dir_reversed = dock_my_dir*(-1);
	MATRIX3 rm = VB1->FindRM(dock_iss_dir, dock_iss_rot, my_dir_reversed, dock_my_rot);
	
	MATRIX3 finalrm = mul(iss_rm, rm);
	vs2.arot.x = atan2(finalrm.m23, finalrm.m33);
	vs2.arot.y = -asin(finalrm.m13);
	vs2.arot.z = atan2(finalrm.m12, finalrm.m11);

	VB1->DefSetStateEx(&vs2);
	//sprintf(oapiDebugString(), "SOFT DOCK");
	oapiAnnotationSetText(note_sd, "SOFT DOCK");
}
void SoftDock::ConsumeSoftDockKey(DWORD key, bool down, char *kstate) {
	//if (!IsSDSysEnabled()) { return; }
	if (KEYMOD_ALT(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_CONTROL(kstate) && key == OAPI_KEY_D) {
		SetSDSystemEnabled(!IsSDSysEnabled());
		if (SoftDockActive()) {
			EnableSoftDock(false);
		}
	}
}
