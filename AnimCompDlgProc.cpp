#include "StationBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "AnimCompDef.h"
#include "AnimDef.h"
#include "AttachmentManager.h"
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
void DialogControl::ShowAnimCompArmTip(HWND hWnd, bool showtip) {
	if (!showtip) {
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_ANIMCOMPMESH), true);
		ShowWindow(GetDlgItem(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPBACKGRPS), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPSETGRPS), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPTIPSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_LIST_ANIMCOMPATTS), SW_HIDE);
		
	}
	else {
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_ANIMCOMPMESH), false);
		ShowWindow(GetDlgItem(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPBACKGRPS), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPSETGRPS), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMCOMPTIPSET), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_LIST_ANIMCOMPATTS), SW_SHOW);
	
	}
	return;
}
void DialogControl::UpdateAnimCompDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	char cbuf[256] = { '\0' };
	SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPNAME, (LPCSTR)AnimMng->GetAnimCompDefName(idx).c_str());

	if (!AnimMng->IsAnimCompDefArmTip(idx)) {
		ShowAnimCompArmTip(hWnd, false);
		SendDlgItemMessage(hWnd, IDC_CHECK_ANIMCOMPARMTIP, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	else {
		ShowAnimCompArmTip(hWnd, true);
		SendDlgItemMessage(hWnd, IDC_CHECK_ANIMCOMPARMTIP, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_RESETCONTENT, 0, 0);
		//UINT counter = 0;
		int selindex = -1;
		for (UINT i = 0; i < AttMng->GetAttCount(); i++) {
			if (!AttMng->AttIsCreated(i)) { continue; }
			char cbuf[256] = { '\0' };
			if (AttMng->GetAttDefId(i).size() <= 0) {
				sprintf(cbuf, "Attachment_%i", i);
			}
			else {
				sprintf(cbuf, AttMng->GetAttDefId(i).c_str());
			}
			int index = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_ADDSTRING, 0, (LPARAM)cbuf);
			if (i == AnimMng->GetAnimCompDefAttTip(idx)) {
				selindex = index;
			}
			SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_SETITEMDATA, index, (LPARAM)i);
		}
		SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_SETCURSEL, selindex, 0);
	}

	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < SB1->MshMng->GetMeshCount(); i++) {
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_INSERTSTRING, i, (LPARAM)SB1->MshMng->GetMeshDefName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_SETITEMDATA, i, (LPARAM)SB1->MshMng->GetMeshIdx(i));
	}
	UINT mdindex = SB1->MshMng->IdxMsh2Def(AnimMng->GetAnimCompDefMesh(idx));
	if (mdindex != (UINT)-1) {
		int comboindex = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_ANIMCOMPMESH), (void*)mdindex);
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_SETCURSEL, comboindex, 0);
		UpdateAnimCompGroupLists(hWnd, mdindex);
	}




	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < idx;i++){// AnimMng->GetAnimCompDefCount(); i++) {
		if (i == idx) { continue; }
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_ADDSTRING, 0, (LPARAM)AnimMng->GetAnimCompDefName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_SETITEMDATA, index, (LPARAM)AnimMng->GetAnimComp(i));
	}
	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_INSERTSTRING, 0, (LPARAM)"NONE");
	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_SETITEMDATA, 0, (LPARAM)NULL);

	AnimCompDef* acd = AnimMng->GetAnimCompDefParent(idx);
	if (acd != NULL) {
		int comboindex = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_ANIMCOMPPARENT), (void*)acd);
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_SETCURSEL, comboindex, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_SETCURSEL, 0, 0);
	}

	sprintf(cbuf, "%.3f", AnimMng->GetAnimCompDefState0(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPS0, (LPCSTR)cbuf);
	sprintf(cbuf, "%.3f", AnimMng->GetAnimCompDefState1(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPS1, (LPCSTR)cbuf);

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
		case IDC_BUTTON_ANIMCOMPS0SET:
		{
			AnimMng->SetAnimCompDefState0(idx, GetDlgItemDouble(hWnd, IDC_EDIT_ANIMCOMPS0));
			break;
		}
		case IDC_BUTTON_ANIMCOMPS1SET:
		{
			AnimMng->SetAnimCompDefState1(idx, GetDlgItemDouble(hWnd, IDC_EDIT_ANIMCOMPS1));
			break;
		}
		case IDC_BUTTON_ANIMCOMPNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPNAME, cbuf, 256);
			string newname(cbuf);
			AnimMng->SetAnimCompDefName(idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			sprintf(cbuf, "%s", AnimMng->GetAnimCompDefName(idx).c_str());
			tvi.pszText = (LPSTR)cbuf;
			tvi.hItem = CurrentSelection.hitem;
			TreeView_SetItem(GetDlgItem(hDlg, IDC_TREE1), &tvi);
			break;
		}
		case IDC_BUTTON_ANIMCOMPANGLESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPANGLE, cbuf, 256);
			double newangle = atof(cbuf);
			newangle *= RAD;
			AnimMng->SetAnimCompDefAngle(idx, newangle);
			sprintf(cbuf, "%.1f", newangle*DEG);
			SetDlgItemText(hWnd, IDC_EDIT_ANIMCOMPANGLE, (LPCSTR)cbuf);
			break;
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
			if (nsel <= 0) { break; }
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
			AnimMng->SetIsAnimCompDefArmTip(idx, false);
			//((ANIMATIONCOMP*)SB1->animcomp_defs[CurrentSelection.idx].ach)->trans->grp = SB1->animcomp_defs[CurrentSelection.idx].pgrps;
			//((ANIMATIONCOMP*)SB1->animcomp_defs[CurrentSelection.idx].ach)->trans->ngrp = SB1->animcomp_defs[CurrentSelection.idx].grps.size();
			break;
		}
		case IDC_BUTTON_ANIMCOMPBACKGRPS:
		{
			int nsel = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM, LB_GETSELCOUNT, 0, 0);
			if (nsel <= 0) { break; }
			UINT *sellist = new UINT[nsel];
			SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM, LB_GETSELITEMS, nsel, (LPARAM)sellist);
			vector<UINT>grpstopass;
			for (UINT i = 0; i < nsel; i++) {
				int groupnumber = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSINANIM, LB_GETITEMDATA, sellist[i], 0);
				grpstopass.push_back(groupnumber);
			}
			AnimMng->AnimCompDefRemoveGroups(idx, grpstopass);
			////////////////////AnimMng->AnimCompDefAddGroups(idx, grpstopass);
			delete[] sellist;

			UpdateAnimCompGroupLists(hWnd, SB1->MshMng->IdxMsh2Def(AnimMng->GetAnimCompDefMesh(idx)));
			break;
		}
		
		case IDC_LIST_ANIMCOMPGRPSTOPICK:
		//case IDC_LIST_ANIMCOMPGRPSINANIM:
		{
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
			{
				SB1->MshMng->ResetMeshGroupHighlights(AnimMng->GetAnimCompDefMesh(idx));
				int nsel = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETSELCOUNT, 0, 0);
				if (nsel > 0) {
					UINT *sellist = new UINT[nsel];
				SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETSELITEMS, nsel, (LPARAM)sellist);
				for (UINT i = 0; i < nsel; i++) {
					int groupnumber = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPGRPSTOPICK, LB_GETITEMDATA, sellist[i], 0);
					msh_idx ms_idx = AnimMng->GetAnimCompDefMesh(idx);

					if (!SB1->MshMng->MeshGroupIsHighlighted(AnimMng->GetAnimCompDefMesh(idx), groupnumber)) {
						SB1->MshMng->HighlightMeshGroup(AnimMng->GetAnimCompDefMesh(idx), groupnumber, true);
					}
				}
				delete[] sellist;
				}

			
				
					
				break;
			}
			}

			break;
		}
		case IDC_BUTTON_ANIMCOMPPASTEV:
		{
			if (SB1->vclip.valid) {
				VECTOR3 msh_pos = _V(0, 0, 0);
				if (!AnimMng->IsAnimCompDefArmTip(idx))
				{
					msh_pos = SB1->MshMng->GetMeshDefPos(SB1->MshMng->IdxMsh2Def(AnimMng->GetAnimCompDefMesh(idx)));
				}
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPREFX, IDC_EDIT_ANIMCOMPREFY, IDC_EDIT_ANIMCOMPREFZ, SB1->vclip.pos-msh_pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ANIMCOMPAXISX, IDC_EDIT_ANIMCOMPAXISY, IDC_EDIT_ANIMCOMPAXISZ, SB1->vclip.dir);
				
			}
			break;
		}
		case IDC_COMBO_ANIMCOMPMESH:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_GETCURSEL, 0, 0);
				msh_idx m_idx = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPMESH, CB_GETITEMDATA, index, 0);
				AnimMng->SetAnimCompDefMesh(idx, m_idx);
				AnimMng->AnimCompDefResetGroups(idx);
				UpdateAnimCompGroupLists(hWnd, SB1->MshMng->IdxMsh2Def(AnimMng->GetAnimCompDefMesh(idx)));
				AnimMng->SetIsAnimCompDefArmTip(idx, false);
				//UpdateAnimCompDialog(hWnd);
				//DWORD Key = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_GETITEMDATA, index, 0);
				//AnimMng->SetAnimKey(idx, Key);
			}
			break;
		}
		case IDC_COMBO_ANIMCOMPPARENT:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_GETCURSEL, 0, 0);
				//ANIMATIONCOMPONENT_HANDLE parent_ach =(ANIMATIONCOMPONENT_HANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_GETITEMDATA, index, 0);
				AnimCompDef* parent_acd = (AnimCompDef*)SendDlgItemMessage(hWnd, IDC_COMBO_ANIMCOMPPARENT, CB_GETITEMDATA, index, 0);
				
				AnimMng->SetAnimCompDefParent(idx, parent_acd);
			}


			break;
		}
		case IDC_BUTTON_ANIMCOMPDEL:
		{
			AnimMng->DeleteAnimCompDef(idx);
			UpdateTree(hDlg, ANIMATIONS, 0);
			break;
		}
		case IDC_CHECK_ANIMCOMPARMTIP:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_ANIMCOMPARMTIP, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					ShowAnimCompArmTip(hWnd, true);
					SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_RESETCONTENT, 0, 0);
					//UINT counter = 0;
					for (UINT i = 0; i < AttMng->GetAttCount(); i++) {
						if (!AttMng->AttIsCreated(i)) { continue; }
						char cbuf[256] = { '\0' };
						if (AttMng->GetAttDefId(i).size() <= 0) {
							sprintf(cbuf, "Attachment_%i", i);
						}
						else {
							sprintf(cbuf, AttMng->GetAttDefId(i).c_str());
						}
						int index = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_ADDSTRING, 0, (LPARAM)cbuf);
						SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_SETITEMDATA, index, (LPARAM)i);
					}
					

				}
				else {
					ShowAnimCompArmTip(hWnd, false);
				}
			}
			break;
		}
		case IDC_BUTTON_ANIMCOMPTIPSET:
		{
			int check = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_GETCURSEL, 0, 0);
			if (check == LB_ERR) { break; }
		//	AnimMng->SetAnimCompDefMesh(idx, LOCALVERTEXLIST);
		//	AnimMng->AnimCompDefResetGroups(idx);
		//	AnimMng->SetAnimCompDefArmTip(idx, true);
			UINT att_idx = SendDlgItemMessage(hWnd, IDC_LIST_ANIMCOMPATTS, LB_GETITEMDATA, check, 0);
			AnimMng->SetAnimCompDefArmTip(idx, att_idx);
		//	VECTOR3 pos, dir, rot;
		//	AttMng->GetAttDefPosDirRot(att_idx, pos, dir, rot);
		//	SB1->arm_tip[0] = pos;
		//	SB1->arm_tip[1] = dir;
		//	SB1->arm_tip[2] = rot;
			
		//	AnimMng->SetAnimCompDefTips(idx, pos, dir+pos, rot+pos);
		//	AnimMng->SetAnimCompDefAttTip(idx, att_idx);
			
		//	((ANIMATIONCOMP*)AnimMng->animcomp_defs[idx]->ach)->trans->grp = MAKEGROUPARRAY(AnimMng->animcomp_defs[idx]->Tip);
		//	((ANIMATIONCOMP*)AnimMng->animcomp_defs[idx]->ach)->trans->ngrp = 3;
			


			break;
		}



		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}