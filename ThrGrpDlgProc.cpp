#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "ThrusterManager.h"

#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateThrGrpDialog(HWND hWnd) {
	THGROUP_TYPE thgt = (THGROUP_TYPE)CurrentSelection.idx;
	switch (thgt) {
	case THGROUP_MAIN:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "MAIN");
		break;
	}
	case THGROUP_RETRO:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "RETRO");
		break;
	}
	case THGROUP_HOVER:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "HOVER");
		break;
	}
	case THGROUP_ATT_PITCHUP:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "PITCH UP");
		break;
	}
	case THGROUP_ATT_PITCHDOWN:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "PITCH DOWN");
		break;
	}
	case THGROUP_ATT_YAWLEFT:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "YAW LEFT");
		break;
	}
	case THGROUP_ATT_YAWRIGHT:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "YAW RIGHT");
		break;
	}
	case THGROUP_ATT_BANKLEFT:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "BANK LEFT");
		break;
	}
	case THGROUP_ATT_BANKRIGHT:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "BANK RIGHT");
		break;
	}
	case THGROUP_ATT_UP:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "TRANSLATE UP");
		break;
	}
	case THGROUP_ATT_DOWN:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "TRANSLATE DOWN");
		break;
	}
	case THGROUP_ATT_RIGHT:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "TRANSLATE RIGHT");
		break;
	}
	case THGROUP_ATT_LEFT:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "TRANSLATE LEFT");
		break;
	}
	case THGROUP_ATT_FORWARD:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "TRANSLATE FORWARD");
		break;
	}
	case THGROUP_ATT_BACK:
	{
		SetDlgItemText(hWnd, IDC_EDIT_THGRP, "TRANSLATE BACK");
		break;
	}
	}
	VBVector<THRUSTER_HANDLE> thrusters = ThrGrpMng->GetThrusters(thgt);
	SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < ThrMng->GetThrCount(); i++) {
		SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_INSERTSTRING, i, (LPARAM)ThrMng->GetThrName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_SETITEMDATA, i, (LPARAM)ThrMng->GetThrTH(i));
		SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_SETSEL, false, i);
		for (UINT j = 0; j < thrusters.size(); j++) {
			if (ThrMng->GetThrTH(i) == thrusters[j]) {
				SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_SETSEL, true, i);
			}
		}
		
	}

	if (ThrGrpMng->IsGroupDefined(thgt)) {
		EnableWindow(GetDlgItem(hWnd, IDC_LIST_THGRP), false);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_THRGRPDEFUNDEF), "UNDEFINE GROUP");
	}
	else {
		EnableWindow(GetDlgItem(hWnd, IDC_LIST_THGRP), true);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_THRGRPDEFUNDEF), "DEFINE GROUP");
	}
	
	
	

	return;
}

BOOL DialogControl::ThrGrpDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_EDIT_THGRP, EM_SETREADONLY, true, 0);
		break;
	}
	case WM_COMMAND:
	{
		THGROUP_TYPE thgt = (THGROUP_TYPE)CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_THRGRPDEFUNDEF:
		{
			if (ThrGrpMng->IsGroupDefined(thgt)) {
				ThrGrpMng->UndefineGroup(thgt);
				UpdateTree(hDlg, THRUSTERGROUPS, CurrentSelection.config);
			}
			else {
				int nsel = SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_GETSELCOUNT, 0, 0);
				if (nsel <= 0) { break; }
				THRUSTER_HANDLE *sellist = new THRUSTER_HANDLE[nsel];
				SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_GETSELITEMS, nsel, (LPARAM)sellist);
				VBVector<THRUSTER_HANDLE>thrusters;
				for (UINT i = 0; i < nsel; i++) {
					THRUSTER_HANDLE th = (THRUSTER_HANDLE)SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_GETITEMDATA, (WPARAM)sellist[i], 0);
					thrusters.push_back(th);
				}
				ThrGrpMng->DefineGroup(thgt, thrusters);
				delete[] sellist;
			}
			UpdateTree(hDlg, THRUSTERGROUPS, CurrentSelection.config);
			break;
		}
		/*case IDC_CHECK_THGRPSHOW:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_THGRPSHOW, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					ShowingThrGrp = true;
					VB1->CreateThrGrpLaserExhausts(thgt);
				}
				else {
					ShowingThrGrp = false;
					VB1->DeleteThrGrpLaserExhausts();
				}
			}
			break;
		}*/
		/*case IDC_LIST_THGRP:
		{
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
			{
				if (ShowingThrGrp) {
					VB1->DeleteThrGrpLaserExhausts();
					int nsel = SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_GETSELCOUNT, 0, 0);
					if (nsel > 0) {
						THRUSTER_HANDLE *sellist = new THRUSTER_HANDLE[nsel];
						SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_GETSELITEMS, nsel, (LPARAM)sellist);
						for (UINT i = 0; i < nsel; i++) {
							THRUSTER_HANDLE th = (THRUSTER_HANDLE)SendDlgItemMessage(hWnd, IDC_LIST_THGRP, LB_GETITEMDATA,(WPARAM)sellist[i] , 0);
							VB1->CreateThrusterLaserExhaust(th);
						}
						delete[] sellist;
					}
				}
				break;
			}
			}
			break;
		}*/


		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}