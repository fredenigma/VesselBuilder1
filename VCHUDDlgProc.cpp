#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "VCManager.h"
#include "MeshManager.h"
#pragma comment(lib, "comctl32.lib")


void DialogControl::EnableVCHudWindows(HWND hWnd, bool enable) {
	int DialogCmds[6] = { IDC_COMBO_VCHUDMESH,IDC_COMBO_VCHUDGRP,IDC_BUTTON_VCHUDCSET,IDC_BUTTON_VCHUDSIZESET,IDC_BUTTON_VCHUDCHECK,IDC_BUTTON_VCHUDPASTEV };
	int DialogEdits[4] = { IDC_EDIT_VCHUDCX,IDC_EDIT_VCHUDCY,IDC_EDIT_VCHUDCZ,IDC_EDIT_VCHUDSIZE };
	if (!enable) {
		for (UINT i = 0; i < 6; i++) {
			EnableWindow(GetDlgItem(hWnd, DialogCmds[i]), false);
		}
		for (UINT i = 0; i < 4; i++) {
			SendDlgItemMessage(hWnd, DialogEdits[i], EM_SETREADONLY, true, 0);
		}
	}
	else {
		for (UINT i = 0; i < 6; i++) {
			EnableWindow(GetDlgItem(hWnd, DialogCmds[i]), true);
		}
		for (UINT i = 0; i < 4; i++) {
			SendDlgItemMessage(hWnd, DialogEdits[i], EM_SETREADONLY, false, 0);
		}
	}
	return;
}

void DialogControl::UpdateVCHUDDialog(HWND hWnd) {
	if (VCMng->IsHUDEnabled()) {
		EnableVCHudWindows(hWnd, true);
		SendDlgItemMessage(hWnd, IDC_CHECK_VCHUDENABLE, BM_SETCHECK, BST_CHECKED, 0);
		DWORD mesh, group;
		VECTOR3 hudcnt;
		double size;
		VCMng->GetHUDParams(mesh, group, hudcnt, size);
		SetDlgItemDouble(hWnd, IDC_EDIT_VCHUDSIZE, size, 3);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCHUDCX, IDC_EDIT_VCHUDCY, IDC_EDIT_VCHUDCZ, hudcnt, 4);

		SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_RESETCONTENT, 0, 0);
		int sel_index = -1;
		for (UINT i = 0; i < MshMng->GetMeshCount(); i++) {
			WORD vis = MshMng->GetMeshVisibility(i);
			UINT msh_counter = 0;
			if (vis >= MESHVIS_VC)  {
				char cbuf[256] = { '\0' };
				sprintf(cbuf, MshMng->GetMeshName(i).c_str());
				msh_idx mesh_index = MshMng->IdxDef2Msh(i);
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_ADDSTRING, 0, (LPARAM)cbuf);
				SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_SETITEMDATA, index, mesh_index);
				if (mesh_index == mesh) {
					sel_index = msh_counter;
				}
				msh_counter++;
			}
		}
		if (sel_index != -1) {
			SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_SETCURSEL, sel_index, 0);
		}

		SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_RESETCONTENT, 0, 0);
		int msh_cur_index = SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_GETCURSEL, 0, 0);
		if (msh_cur_index != CB_ERR) {
			msh_idx mesh_idx = SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_GETITEMDATA, msh_cur_index, 0);
			if (mesh_idx >= 0) {
				UINT ngroups = MshMng->GetMeshDefNGrps(mesh_idx);
				int selgrp_index = -1;
				for (UINT i = 0; i < ngroups; i++) {
					char cbuf[256] = { '\0' };
					sprintf(cbuf, "%i", i);
					SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_INSERTSTRING, i, (LPARAM)cbuf);
					SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_SETITEMDATA, i, i);
					if (group == i) {
						selgrp_index = i;
					}
				}
				if (selgrp_index != -1) {
					SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_SETCURSEL, selgrp_index, 0);
				}

			}
		}

	}
	else {
		EnableVCHudWindows(hWnd, false);
		SendDlgItemMessage(hWnd, IDC_CHECK_VCHUDENABLE, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	return;
}
BOOL DialogControl::VCHUDDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_VCHUDCSET:
		{
			VECTOR3 hudcnt = GetDlgItemsVector3(hWnd, IDC_EDIT_VCHUDCX, IDC_EDIT_VCHUDCY, IDC_EDIT_VCHUDCZ);
			VCMng->SetHUDCenter(hudcnt);
			break;
		}
		case IDC_BUTTON_VCHUDSIZESET:
		{
			double size = GetDlgItemDouble(hWnd, IDC_EDIT_VCHUDSIZE);
			VCMng->SetHUDSize(size);
			break;
		}
		case IDC_BUTTON_VCHUDPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCHUDCX, IDC_EDIT_VCHUDCY, IDC_EDIT_VCHUDCZ, VB1->vclip.pos, 4);
			}
			break;
		}
		case IDC_CHECK_VCHUDENABLE:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_VCHUDENABLE);
				VCMng->EnableHUD(check);
				UpdateVCHUDDialog(hWnd);
			}

			break;
		}
		case IDC_COMBO_VCHUDGRP:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_GETCURSEL, 0, 0);
				if (index != CB_ERR) {
					VCMng->SetHUDGroup((DWORD)index);
				}
			}
			break;
		}
		case IDC_COMBO_VCHUDMESH:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_GETCURSEL, 0, 0);
				if (index != CB_ERR) {
					msh_idx mesh_index = SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDMESH, CB_GETITEMDATA, index, 0);
					if (mesh_index >= 0) {
						VCMng->SetHUDMesh(mesh_index);
						SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_RESETCONTENT, 0, 0);
						UINT ngroups = MshMng->GetMeshDefNGrps(mesh_index);
						for (UINT i = 0; i < ngroups; i++) {
							char cbuf[256] = { '\0' };
							sprintf(cbuf, "%i", i);
							SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_INSERTSTRING, i, (LPARAM)cbuf);
							SendDlgItemMessage(hWnd, IDC_COMBO_VCHUDGRP, CB_SETITEMDATA, i, i);
						}

					}
				}
				
			}
			break;
		}
		case IDC_BUTTON_VCHUDCHECK:
		{
			int pos = VCMng->GetVCPosition();
			VB1->clbkLoadVC(pos);
			break;
		}

		}
	
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}