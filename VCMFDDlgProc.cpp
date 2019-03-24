#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "VCManager.h"
#include "MeshManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateVCMFDDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= VCMng->GetMFDCount()) { return; }
	char cbuf[256] = { '\0' };
	if (idx == 0) {
		sprintf(cbuf, "MFD_LEFT");
	}
	else if (idx == 1) {
		sprintf(cbuf, "MFD_RIGHT");
	}
	else {
		sprintf(cbuf, "MFD_USER_%i", idx - 1);
	}
	SetDlgItemText(hWnd, IDC_EDIT_VCMFDNAME, cbuf);

	int DlgEditPwr[6] = { IDC_EDIT_VCMFDPWRX,IDC_EDIT_VCMFDPWRY,IDC_EDIT_VCMFDPWRZ,IDC_EDIT_VCMFDMNUX,IDC_EDIT_VCMFDMNUY,IDC_EDIT_VCMFDMNUZ };
	if (VCMng->GetMFDHasPwrBtns(idx)) {
		SendDlgItemMessage(hWnd, IDC_CHECK_VCMFDPWRSET, BM_SETCHECK, BST_CHECKED, 0);
		for (UINT i = 0; i < 6; i++) {
			SendDlgItemMessage(hWnd, DlgEditPwr[i], EM_SETREADONLY, false, 0);
		}
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDPWRPASTEV), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDMNUPASTEV), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDPWRSET), true);
		VECTOR3 pwr0, pwr2;
		VCMng->GetMFDPwrVectors(idx, pwr0, pwr2);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDPWRX, IDC_EDIT_VCMFDPWRY, IDC_EDIT_VCMFDPWRZ, pwr0, 5);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDMNUX, IDC_EDIT_VCMFDMNUY, IDC_EDIT_VCMFDMNUZ, pwr2, 5);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_VCMFDPWRSET, BM_SETCHECK, BST_UNCHECKED, 0);
		for (UINT i = 0; i < 6; i++) {
			SendDlgItemMessage(hWnd, DlgEditPwr[i], EM_SETREADONLY, true, 0);
			SetDlgItemText(hWnd, DlgEditPwr[i], "");
		}
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDPWRPASTEV), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDMNUPASTEV), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDPWRSET), false);
	}

	int DlgEditCols[12] = { IDC_EDIT_VCMFDTLX,IDC_EDIT_VCMFDTLY,IDC_EDIT_VCMFDTLZ,IDC_EDIT_VCMFDBLX,IDC_EDIT_VCMFDBLY,IDC_EDIT_VCMFDBLZ,IDC_EDIT_VCMFDTRX,IDC_EDIT_VCMFDTRY,IDC_EDIT_VCMFDTRZ,IDC_EDIT_VCMFDBRX,IDC_EDIT_VCMFDBRY,IDC_EDIT_VCMFDBRZ };
	if (VCMng->GetMFDHasColBtns(idx)) {
		SendDlgItemMessage(hWnd, IDC_CHECK_VCMFDBC, BM_SETCHECK, BST_CHECKED, 0);
		for (UINT i = 0; i < 12; i++) {
			SendDlgItemMessage(hWnd, DlgEditCols[i], EM_SETREADONLY, false, 0);
		}
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDCOLSET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDTLPASTEV), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDTRPASTEV), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDBLPASTEV), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDBRPASTEV), true);
		VECTOR3 tl, bl, tr, br;
		VCMng->GetMFDColVectors(idx, tl, bl, tr, br);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDTLX, IDC_EDIT_VCMFDTLY, IDC_EDIT_VCMFDTLZ, tl, 5);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDBLX, IDC_EDIT_VCMFDBLY, IDC_EDIT_VCMFDBLZ, bl, 5);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDTRX, IDC_EDIT_VCMFDTRY, IDC_EDIT_VCMFDTRZ, tr, 5);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDBRX, IDC_EDIT_VCMFDBRY, IDC_EDIT_VCMFDBRZ, br, 5);

	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_VCMFDBC, BM_SETCHECK, BST_UNCHECKED, 0);
		for (UINT i = 0; i < 12; i++) {
			SendDlgItemMessage(hWnd, DlgEditCols[i], EM_SETREADONLY, true, 0);
			SetDlgItemText(hWnd, DlgEditCols[i], "");
		}
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDCOLSET), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDTLPASTEV), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDTRPASTEV), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDBLPASTEV), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VCMFDBRPASTEV), false);
	}
	DWORD mesh = VCMng->GetMFDMesh(idx);
	DWORD group = VCMng->GetMFDGroup(idx);

	SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_RESETCONTENT, 0, 0);
	int sel_index = -1;
	for (UINT i = 0; i < MshMng->GetMeshCount(); i++) {
		WORD vis = MshMng->GetMeshVisibility(i);
		UINT msh_counter = 0;
		if (vis & MESHVIS_VC) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, MshMng->GetMeshName(i).c_str());
			msh_idx mesh_index = MshMng->IdxDef2Msh(i);
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_ADDSTRING, 0, (LPARAM)cbuf);
			SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_SETITEMDATA, index, mesh_index);
			if (mesh_index == mesh) {
				sel_index = msh_counter;
			}
			msh_counter++;
		}
	}
	if (sel_index != -1) {
		SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_SETCURSEL, sel_index, 0);
	}

	SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_RESETCONTENT, 0, 0);
	int msh_cur_index = SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_GETCURSEL, 0, 0);
	if (msh_cur_index != CB_ERR) {
		msh_idx mesh_idx = SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_GETITEMDATA, msh_cur_index, 0);
		if (mesh_idx >= 0) {
			UINT ngroups = MshMng->GetMeshDefNGrps(mesh_idx);
			int selgrp_index = -1;
			for (UINT i = 0; i < ngroups; i++) {
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "%i", i);
				SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_INSERTSTRING, i, (LPARAM)cbuf);
				SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_SETITEMDATA, i, i);
				if (group == i) {
					selgrp_index = i;
				}
			}
			if (selgrp_index != -1) {
				SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_SETCURSEL, selgrp_index, 0);
			}

		}
	}
	

	return;
}

BOOL DialogControl::VCMFDDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_EDIT_VCMFDNAME, EM_SETREADONLY, true, 0);
		break;
	}
	case WM_COMMAND:
	{	
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_CHECK_VCMFDPWRSET:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_VCMFDPWRSET);
				VCMng->SetMFDHasPwrBtns(idx, check);
				UpdateVCMFDDialog(hWnd);
			}
			break;
		}
		case IDC_CHECK_VCMFDBC:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_VCMFDBC);
				VCMng->SetMFDHasColumnBtns(idx, check);
				UpdateVCMFDDialog(hWnd);
			}
			break;
		}
		case IDC_BUTTON_VCMFDPWRPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDPWRX, IDC_EDIT_VCMFDPWRY, IDC_EDIT_VCMFDPWRZ, VB1->vclip.pos, 5);
			}
			break;
		}
		case IDC_BUTTON_VCMFDMNUPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDMNUX, IDC_EDIT_VCMFDMNUY, IDC_EDIT_VCMFDMNUZ, VB1->vclip.pos, 5);
			}
			break;
		}
		case IDC_BUTTON_VCMFDTLPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDTLX, IDC_EDIT_VCMFDTLY, IDC_EDIT_VCMFDTLZ, VB1->vclip.pos, 5);
			}
			break;
		}
		case IDC_BUTTON_VCMFDBLPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDBLX, IDC_EDIT_VCMFDBLY, IDC_EDIT_VCMFDBLZ, VB1->vclip.pos, 5);
			}
			break;
		}
		case IDC_BUTTON_VCMFDTRPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDTRX, IDC_EDIT_VCMFDTRY, IDC_EDIT_VCMFDTRZ, VB1->vclip.pos, 5);
			}
			break;
		}
		case IDC_BUTTON_VCMFDBRPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCMFDBRX, IDC_EDIT_VCMFDBRY, IDC_EDIT_VCMFDBRZ, VB1->vclip.pos, 5);
			}
			break;
		}
		case IDC_BUTTON_VCMFDPWRSET:
		{
			VECTOR3 pwr0, pwr2;
			pwr0 = GetDlgItemsVector3(hWnd, IDC_EDIT_VCMFDPWRX, IDC_EDIT_VCMFDPWRY, IDC_EDIT_VCMFDPWRZ);
			pwr2 = GetDlgItemsVector3(hWnd, IDC_EDIT_VCMFDMNUX, IDC_EDIT_VCMFDMNUY, IDC_EDIT_VCMFDMNUZ);
			VCMng->DefineMFDPwrButtons(idx, pwr0, pwr2);
			break;
		}
		case IDC_BUTTON_VCMFDCOLSET:
		{
			VECTOR3 tl, bl, tr, br;
			tl = GetDlgItemsVector3(hWnd, IDC_EDIT_VCMFDTLX, IDC_EDIT_VCMFDTLY, IDC_EDIT_VCMFDTLZ);
			bl = GetDlgItemsVector3(hWnd, IDC_EDIT_VCMFDBLX, IDC_EDIT_VCMFDBLY, IDC_EDIT_VCMFDBLZ);
			tr = GetDlgItemsVector3(hWnd, IDC_EDIT_VCMFDTRX, IDC_EDIT_VCMFDTRY, IDC_EDIT_VCMFDTRZ);
			br = GetDlgItemsVector3(hWnd, IDC_EDIT_VCMFDBRX, IDC_EDIT_VCMFDBRY, IDC_EDIT_VCMFDBRZ);
			VCMng->DefineMFDColumnButtons(idx, 1, tl, bl);
			VCMng->DefineMFDColumnButtons(idx, 2, tr, br);
			break;
		}
		case IDC_COMBO_VCMFDGRP:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_GETCURSEL, 0, 0);
				if (index != CB_ERR) {
					VCMng->SetMFDGroup(idx, index);
				}
			}
			break;
		}
		case IDC_COMBO_VCMFDMESH:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_GETCURSEL, 0, 0);
				if (index != CB_ERR) {
					msh_idx mesh_index = SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDMESH, CB_GETITEMDATA, index, 0);
					if (mesh_index >= 0) {
						VCMng->SetMFDMesh(idx, mesh_index);
						SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_RESETCONTENT, 0, 0);
						UINT ngroups = MshMng->GetMeshDefNGrps(mesh_index);
						for (UINT i = 0; i < ngroups; i++) {
							char cbuf[256] = { '\0' };
							sprintf(cbuf, "%i", i);
							SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_INSERTSTRING, i, (LPARAM)cbuf);
							SendDlgItemMessage(hWnd, IDC_COMBO_VCMFDGRP, CB_SETITEMDATA, i, i);
						}

					}
				}
			}
			break;
		}
		case IDC_BUTTON_VCMFDCHECK:
		{
			int pos = VCMng->GetVCPosition();
			VB1->clbkLoadVC(pos);
			break;
		}
		case IDC_BUTTON_VCMFDDELETE:
		{
			VCMng->DeleteMFD(idx);
			UpdateTree(hDlg, VCMFD, CurrentSelection.config);
			break;
		}

		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}