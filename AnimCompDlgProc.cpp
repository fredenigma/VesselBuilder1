#include "StationBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "AnimCompDef.h"
#include "AnimDef.h"
#include "AnimationManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateAnimCompGroupLists(HWND hWnd, def_idx meshdef_idx) {
	UINT idx = CurrentSelection.idx;
	SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM, LB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < SB1->MshMng->GetMeshDefNGrps(meshdef_idx); i++) {
		
		if (IsUintInVector(i, AnimMng->GetAnimCompDefGroups(idx))) {
			char cbuf[128] = { '\0' };
			sprintf(cbuf, "%i", i);
			int index = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM, LB_ADDSTRING, 0, (LPARAM)cbuf);
			SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM, LB_SETITEMDATA, index, (LPARAM)i);
		}
		else {
			char cbuf[128] = { '\0' };
			sprintf(cbuf, "%i", i);
			int index = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_ADDSTRING, 0, (LPARAM)cbuf);
			SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_SETITEMDATA, index, (LPARAM)i);
		}
			
	
	
	}
	return;
}

void DialogControl::UpdateAnimCompDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	char cbuf[256] = { '\0' };
	SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPNAME, (LPCSTR)AnimMng->GetAnimCompDefName(idx).c_str());
	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < SB1->MshMng->GetMeshCount(); i++) {
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_INSERTSTRING, i, (LPARAM)SB1->MshMng->GetMeshDefName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_SETITEMDATA, i, (LPARAM)i);
	}
	UINT mdindex = SB1->MshMng->IdxMsh2Def(AnimMng->GetAnimCompDefMesh(idx));
	if (mdindex != (UINT)-1) {
		int comboindex = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_ANIMCOMPMESH), (void*)mdindex);
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_SETCURSEL, comboindex, 0);
		UpdateAnimCompGroupLists(hWnd, mdindex);
	}
	sprintf(cbuf, "%.3f", AnimMng->GetAnimCompDefState0(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPS0, (LPCSTR)cbuf);
	sprintf(cbuf, "%.3f", AnimMng->GetAnimCompDefState1(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPS1, (LPCSTR)cbuf);
	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < AnimMng->GetAnimCompDefCount(); i++) {
		if (i != CurrentSelection.idx) {
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_ADDSTRING, 0, (LPARAM)AnimMng->GetAnimCompDefFullName(idx).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_SETITEMDATA, index, (LPARAM)AnimMng->GetAnimComp(idx));
		}
	}
	if (AnimMng->GetAnimCompDefType(idx)== MGROUP_TRANSFORM::ROTATE) {
		SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPTYPE, (LPCSTR)"ROTATE");
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISX), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISY), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISZ), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPANGLE), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPAXISSET), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPANGLESET), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_ACANGLE), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_ACAX), SW_SHOW);
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_ACREF), (LPCSTR)"Reference Point - XYZ");
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_ACAX), (LPCSTR)"Rotation Axis - XYZ");
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_ACANGLE), (LPCSTR)"Angle [DEG]");
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ, AnimMng->GetAnimCompDefRef(idx));
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ, AnimMng->GetAnimCompDefAxis(idx));
		sprintf(cbuf, "%.1f", AnimMng->GetAnimCompDefAngle(idx)*DEG);
		SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPANGLE, (LPCSTR)cbuf);
	}
	else if (AnimMng->GetAnimCompDefType(idx) == MGROUP_TRANSFORM::TRANSLATE) {

		SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPTYPE, (LPCSTR)"TRANSLATE");
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPANGLE), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPAXISSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPANGLESET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_ACANGLE), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_ACAX), SW_HIDE);
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_ACREF), (LPCSTR)"Shift Vector - XYZ");
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ, AnimMng->GetAnimCompDefShift(idx));
	}
	else if (AnimMng->GetAnimCompDefType(idx) == MGROUP_TRANSFORM::SCALE) {
		SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPTYPE, (LPCSTR)"SCALE");
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISX), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISY), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPAXISZ), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPANGLE), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPAXISSET), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPANGLESET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_ACANGLE), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_ACAX), SW_SHOW);
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_ACREF), (LPCSTR)"Reference Point - XYZ");
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_ACAX), (LPCSTR)"Scale Factor - XYZ");
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ, AnimMng->GetAnimCompDefRef(idx));
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ, AnimMng->GetAnimCompDefScale(idx));
	}
	return;
}

BOOL DialogControl::AnimCompDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		InitCommonControls();
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANIMCOMPTYPE), false);
		
		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {

		case IDC_BUTTON_ANIMCOMPANGLESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPANGLE, cbuf, 256);
			double newangle = atof(cbuf);
			newangle *= RAD;
			AnimMng->SetAnimCompDefAngle(idx, newangle);
			sprintf(cbuf, "%.1f", newangle*DEG);
			SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPANGLE, (LPCSTR)cbuf);
		}
		case IDC_BUTTON_ANIMCOMPAXISSET:
		{
			if (AnimMng->GetAnimCompDefType(idx) == MGROUP_TRANSFORM::ROTATE) {
				VECTOR3 newAxis = GetDlgItemsVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ);
				normalise(newAxis);
				AnimMng->SetAnimCompDefAxis(idx, newAxis);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ, newAxis);
			}
			else if (AnimMng->GetAnimCompDefType(idx) == MGROUP_TRANSFORM::SCALE) {
				VECTOR3 newScale = GetDlgItemsVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ);
				AnimMng->SetAnimCompDefScale(idx, newScale);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ, newScale);
			}
			break;
		}
		case IDC_BUTTON_ANIMCOMPREFSET:
		{
			if (AnimMng->GetAnimCompDefType(idx) == MGROUP_TRANSFORM::ROTATE) {
				VECTOR3 newRef = GetDlgItemsVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ);
				AnimMng->SetAnimCompDefRef(idx, newRef);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ,newRef);
			}
			else if (AnimMng->GetAnimCompDefType(idx) == MGROUP_TRANSFORM::SCALE) {
				VECTOR3 newRef = GetDlgItemsVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ);
				AnimMng->SetAnimCompDefRef(idx, newRef);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ,newRef);
			}
			else if (AnimMng->GetAnimCompDefType(idx) == MGROUP_TRANSFORM::TRANSLATE) {
				VECTOR3 newShift = GetDlgItemsVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ);
				AnimMng->SetAnimCompDefShift(idx, newShift);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ, newShift);
			}
			break;
		}
		case IDC_BUTTON_ANIMCOMPSETGRPS:
		{
			int nsel = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETSELCOUNT, 0, 0);
			UINT *sellist = new UINT[nsel];
			SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETSELITEMS, nsel, (LPARAM)sellist);
			vector<UINT>grpstopass;
			for (UINT i = 0; i < nsel; i++) {
				int groupnumber = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETITEMDATA, sellist[i], 0);
				grpstopass.push_back(groupnumber);
			}
			AnimMng->AnimCompDefAddGroups(idx, grpstopass);
			delete[] sellist;
			
			UpdateAnimCompGroupLists(hWnd, SB1->MshMng->IdxMsh2Def(AnimMng->GetAnimCompDefMesh(idx)));
			//((ANIMATIONCOMP*)SB1->animcomp_defs[CurrentSelection.idx].ach)->trans->grp = SB1->animcomp_defs[CurrentSelection.idx].pgrps;
			//((ANIMATIONCOMP*)SB1->animcomp_defs[CurrentSelection.idx].ach)->trans->ngrp = SB1->animcomp_defs[CurrentSelection.idx].grps.size();
			break;
		}
		
		case IDC_LIST_ANIMCOMPGRPSTOPICK:
		//case IDC_LIST_ANIMCOMPGRPSINANIM:
		{
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
			{
				//UINT mdindex = SB1->MshMng->IdxMsh2Def(SB1->animcomp_defs[CurrentSelection.idx]->GetMesh());
				SB1->MshMng->ResetMeshGroupHighlights(AnimMng->GetAnimCompDefMesh(idx));
				int nsel = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETSELCOUNT, 0, 0);
				UINT *sellist = new UINT[nsel];
				SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETSELITEMS, nsel, (LPARAM)sellist);
				
				for (UINT i = 0; i < nsel; i++) {
					int groupnumber = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETITEMDATA, sellist[i], 0);
					if (!SB1->MshMng->MeshGroupIsHighlighted(AnimMng->GetAnimCompDefMesh(idx), groupnumber)) {
						SB1->MshMng->HighlightMeshGroup(AnimMng->GetAnimCompDefMesh(idx), groupnumber, true);
					}
				}
				delete[] sellist;

			
				
					
				break;
			}
			}

			break;
		}
		case IDC_BUTTON_ANIMCOMPPASTEV:
		{
			if (SB1->vclip.valid) {
				VECTOR3 msh_pos=SB1->MshMng->GetMeshDefPos(SB1->MshMng->IdxMsh2Def(AnimMng->GetAnimCompDefMesh(idx)));
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ, SB1->vclip.pos-msh_pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ, SB1->vclip.dir);
				
			}
			break;
		}



		}
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}