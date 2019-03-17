#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "AnimationManager.h"
#include "TouchdownPointsManager.h"
#include "LaserManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateTdpDialog(HWND hWnd) {
	UINT set = CurrentSelection.idx;
	if ((set < 1) || (set > 2)) { return; }
	vector<TOUCHDOWNVTX> curset;
	if (set == 1) {
		curset = TdpMng->GetSet(1);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_TDPCHANGEOVERANIM), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_TDPENABLESET2), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPCHANGE), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPJ3P), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_TDPHFG), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_TDPRAD), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_TDPTAILSITTER), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPDEFGRP), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPDEFHFG), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPDEFRAD), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPAPPLY), SW_SHOW);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_TDPAPPLY), "APPLY");
		SetDlgItemText(hWnd, IDC_EDIT_TDPCURSET, "SET 1");

		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPPASTEV), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPADD), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPREMOVE), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPCLEARSET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPAPPLY), true);
		EnableWindow(GetDlgItem(hWnd, IDC_CHECK_TDPSHOWCUR), true);
		EnableWindow(GetDlgItem(hWnd, IDC_CHECK_TDPSHOWSET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_LIST_TDP), true);
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSX, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSY, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSZ, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPDAMP, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPSTIFF, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPMU, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPMULNG, EM_SETREADONLY, false, 0);
	
	}
	else {
		curset = TdpMng->GetSet(2);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_TDPCHANGEOVERANIM), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_TDPENABLESET2), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPCHANGE), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPJ3P), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_TDPHFG), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_TDPRAD), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_TDPTAILSITTER), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPDEFGRP), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPDEFHFG), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TDPDEFRAD), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPAPPLY), SW_HIDE);
		SetDlgItemText(hWnd, IDC_EDIT_TDPCURSET, "SET 2");
		if (TdpMng->IsSecondSetEnabled()) {
			SendDlgItemMessage(hWnd, IDC_CHECK_TDPENABLESET2, BM_SETCHECK, BST_CHECKED, 0);
			EnableWindow(GetDlgItem(hWnd, IDC_COMBO_TDPCHANGEOVERANIM), true);

			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPPASTEV), true);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPADD), true);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPREMOVE), true);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPCLEARSET), true);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPAPPLY), true);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECK_TDPSHOWCUR), true);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECK_TDPSHOWSET), true);
			EnableWindow(GetDlgItem(hWnd, IDC_LIST_TDP), true);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSX, EM_SETREADONLY, false, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSY, EM_SETREADONLY, false, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSZ, EM_SETREADONLY, false, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPDAMP, EM_SETREADONLY, false, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPSTIFF, EM_SETREADONLY, false, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPMU, EM_SETREADONLY, false, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPMULNG, EM_SETREADONLY, false, 0);
			SendDlgItemMessage(hWnd, IDC_COMBO_TDPCHANGEOVERANIM, CB_RESETCONTENT, 0, 0);
			for (UINT i = 0; i < AnimMng->GetAnimDefsCount(); i++) {
				if (!AnimMng->IsAnimValid(i)) { continue; }
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "%s", AnimMng->GetAnimName(i).c_str());
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_TDPCHANGEOVERANIM, CB_ADDSTRING, 0, (LPARAM)cbuf);
				SendDlgItemMessage(hWnd, IDC_COMBO_TDPCHANGEOVERANIM, CB_SETITEMDATA, index, i);
			}
			UINT changeoveranim = TdpMng->GetChangeOverAnimation();
			if (changeoveranim != (UINT)-1) {
				SendDlgItemMessage(hWnd, IDC_COMBO_TDPCHANGEOVERANIM, CB_SETCURSEL, changeoveranim, 0);
			}
		}
		else {
			SendDlgItemMessage(hWnd, IDC_CHECK_TDPENABLESET2, BM_SETCHECK, BST_UNCHECKED, 0);
			EnableWindow(GetDlgItem(hWnd, IDC_COMBO_TDPCHANGEOVERANIM), false);

			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPPASTEV), false);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPADD), false);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPREMOVE), false);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPCLEARSET), false);
			EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TDPAPPLY), false);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECK_TDPSHOWCUR), false);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECK_TDPSHOWSET), false);
			EnableWindow(GetDlgItem(hWnd, IDC_LIST_TDP), false);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSX, EM_SETREADONLY, true, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSY, EM_SETREADONLY, true, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPPOSZ, EM_SETREADONLY, true, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPDAMP, EM_SETREADONLY, true, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPSTIFF, EM_SETREADONLY, true, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPMU, EM_SETREADONLY, true, 0);
			SendDlgItemMessage(hWnd, IDC_EDIT_TDPMULNG, EM_SETREADONLY, true, 0);
		}
	}

	SetDlgItemText(hWnd, IDC_EDIT_TDPPOSX, "");
	SetDlgItemText(hWnd, IDC_EDIT_TDPPOSY, "");
	SetDlgItemText(hWnd, IDC_EDIT_TDPPOSZ, "");
	SetDlgItemText(hWnd, IDC_EDIT_TDPDAMP, "");
	SetDlgItemText(hWnd, IDC_EDIT_TDPSTIFF, "");
	SetDlgItemText(hWnd, IDC_EDIT_TDPMU, "");
	SetDlgItemText(hWnd, IDC_EDIT_TDPMULNG, "");
	
	SendDlgItemMessage(hWnd, IDC_LIST_TDP, LB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < curset.size(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "Point:%i Pos:%.2f %.2f %.2f Stiff:%.1e Damp:%.1e Mu:%.1f Mu lng:%.1f", i,curset[i].pos.x, curset[i].pos.y, curset[i].pos.z, curset[i].stiffness, curset[i].damping, curset[i].mu, curset[i].mu_lng);
		SendDlgItemMessage(hWnd, IDC_LIST_TDP, LB_INSERTSTRING, i, (LPARAM)cbuf);
		SendDlgItemMessage(hWnd, IDC_LIST_TDP, LB_SETITEMDATA, i, (LPARAM)i);
	}
	return;
}
BOOL DialogControl::TdpDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_EDIT_TDPCURSET, EM_SETREADONLY, true, 0);
		break;
	}
	case WM_COMMAND:
	{
		UINT set = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_CHECK_TDPSHOWCUR:
		{
			LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_TDPSHOWCUR, BM_GETCHECK, 0, 0);
			if (getcheck == BST_CHECKED) {
				UINT curset = TdpMng->GetCurrentSet();
				UINT set_ind = curset*10000;
				UINT points_count = TdpMng->GetPointsCount(curset);
				TdpCurOn[0] = curset;
				TdpCurOn[1] = points_count;
				for (UINT i = 0; i < points_count; i++) {
					VECTOR3 pos = TdpMng->GetPointPos(curset, i);
					set_ind += i;
					TdpLaserMap[set_ind]=VB1->Laser->CreateFixedLaserStar(pos, LASER_GREEN_TEX, 1.5);
				}
			}
			else {
				UINT set_ind = TdpCurOn[0] * 10000;
				UINT points_count = TdpCurOn[1];
				for (UINT i = 0; i < points_count; i++) {
					set_ind += i;
					VB1->Laser->DeleteLaser(TdpLaserMap[set_ind]);
				}
			}
			break;
		}
		case IDC_CHECK_TDPSHOWSET:
		{
			LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_TDPSHOWSET, BM_GETCHECK, 0, 0);
			if (getcheck == BST_CHECKED) {
				UINT set_ind = set * 1000;
				UINT points_count = TdpMng->GetPointsCount(set);
				TdpSetOn[0] = set;
				TdpSetOn[1] = points_count;
				for (UINT i = 0; i < points_count; i++) {
					VECTOR3 pos = TdpMng->GetPointPos(set, i);
					set_ind += i;
					TdpLaserMap[set_ind] = VB1->Laser->CreateFixedLaserStar(pos, LASER_RED_TEX, 1.5);
				}
			}
			else {
				UINT set_ind = TdpSetOn[0] * 1000;
				UINT points_count = TdpSetOn[1];
				for (UINT i = 0; i < points_count; i++) {
					set_ind += i;
					VB1->Laser->DeleteLaser(TdpLaserMap[set_ind]);
				}
			}
			break;
		}
		case IDC_BUTTON_TDPADD:
		{
			TOUCHDOWNVTX tdvtx;
			tdvtx.pos = GetDlgItemsVector3(hWnd, IDC_EDIT_TDPPOSX, IDC_EDIT_TDPPOSY, IDC_EDIT_TDPPOSZ);
			tdvtx.damping = GetDlgItemDouble(hWnd, IDC_EDIT_TDPDAMP);
			tdvtx.stiffness = GetDlgItemDouble(hWnd, IDC_EDIT_TDPSTIFF);
			tdvtx.mu = GetDlgItemDouble(hWnd, IDC_EDIT_TDPMU);
			tdvtx.mu_lng = GetDlgItemDouble(hWnd, IDC_EDIT_TDPMULNG);
			TdpMng->AddPoint(set, tdvtx);
			UpdateTdpDialog(hWnd);
			break;
		}
		case IDC_BUTTON_TDPCLEARSET:
		{
			TdpMng->ClearSet(set);
			UpdateTdpDialog(hWnd);
			break;
		}
		case IDC_BUTTON_TDPREMOVE:
		{
			int index = SendDlgItemMessage(hWnd, IDC_LIST_TDP, LB_GETCURSEL, 0, 0);
			if (index == LB_ERR) { break; }
			UINT idx = SendDlgItemMessage(hWnd, IDC_LIST_TDP, LB_GETITEMDATA, index, 0);
			TdpMng->RemovePoint(set, idx);
			UpdateTdpDialog(hWnd);
			break;
		}
		case IDC_BUTTON_TDPPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_TDPPOSX, IDC_EDIT_TDPPOSY, IDC_EDIT_TDPPOSZ, VB1->vclip.pos);
			}
			break;
		}
		case IDC_BUTTON_TDPJ3P:
		{
			TdpMng->ClearSet(1);
			double Mass = VB1->GetMass();
			double ro = GetDlgItemDouble(hWnd, IDC_EDIT_TDPRAD);
			double HeightFromGround = GetDlgItemDouble(hWnd,IDC_EDIT_TDPHFG);
			bool tailsitter = IsCheckBoxChecked(hWnd, IDC_CHECK_TDPTAILSITTER);
			TOUCHDOWNVTX td[4];

			double x_target = -0.5;
			double stiffness = (-1)*(Mass*9.81) / (3 * x_target);
			double damping = 0.9*(2 * sqrt(Mass*stiffness));
			
			for (int i = 0; i<4; i++)
			{

				td[i].damping = damping;
				td[i].mu = 3;
				td[i].mu_lng = 0.2;
				td[i].stiffness = stiffness;
			}
			
			if (tailsitter) {
				//to RECHECK!
				td[1].pos.x = -cos(30 * RAD)*ro;
				td[1].pos.y = -sin(30 * RAD)*ro; 
				td[1].pos.z = -HeightFromGround;
				td[2].pos.x = 0;
				td[2].pos.y = 1 * ro;
				td[2].pos.z = -HeightFromGround;
				td[0].pos.x = cos(30 * RAD)*ro;
				td[0].pos.y = -sin(30 * RAD)*ro;
				td[0].pos.z = -HeightFromGround;
				td[3].pos.x = 0;
				td[3].pos.z = 1 * ro;
				td[3].pos.y = 0;
			}
			else {

				td[0].pos.x = 0;
				td[0].pos.y = -HeightFromGround;
				td[0].pos.z = 1.1 * ro;
				td[0].mu = 1.5;
				td[0].mu = 0.1;

				td[1].pos.x = -cos(30 * RAD)*ro;
				td[1].pos.y = -HeightFromGround;
				td[1].pos.z = -sin(30 * RAD)*ro;

				td[2].pos.x = cos(30 * RAD)*ro;
				td[2].pos.y = -HeightFromGround;
				td[2].pos.z = -sin(30 * RAD)*ro;
				
				td[3].pos.x = 0;
				td[3].pos.y = 1 * ro;
				td[3].pos.z = 0;
			}
			
			for (int i = 0; i < 4; i++) {
				TdpMng->AddPoint(1, td[i]);
			}
			UpdateTdpDialog(hWnd);
			
			break;
		}
		case IDC_BUTTON_TDPAPPLY:
		{
			TdpMng->SetCurrentSet(1);
			break;
		}
		case IDC_CHECK_TDPENABLESET2:
		{
			if (IsCheckBoxChecked(hWnd, IDC_CHECK_TDPENABLESET2)) {
				TdpMng->EnableSecondSet(true);
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "SET 2");
				TVITEM tvi;
				tvi.mask = TVIF_TEXT;
				tvi.pszText = (LPSTR)cbuf;
				tvi.cchTextMax = strlen(cbuf);
				tvi.hItem = CurrentSelection.hitem;
				SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
				UpdateTdpDialog(hWnd);
			}
			else {
				TdpMng->EnableSecondSet(false);
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "*SET 2");
				TVITEM tvi;
				tvi.mask = TVIF_TEXT;
				tvi.pszText = (LPSTR)cbuf;
				tvi.cchTextMax = strlen(cbuf);
				tvi.hItem = CurrentSelection.hitem;
				SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
				UpdateTdpDialog(hWnd);
			}
			break;
		}
		case IDC_COMBO_TDPCHANGEOVERANIM:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_TDPCHANGEOVERANIM, CB_GETCURSEL, 0, 0);
				UINT anim_dindx = (UINT)SendDlgItemMessage(hWnd, IDC_COMBO_TDPCHANGEOVERANIM, CB_GETITEMDATA, index, 0);
				if (anim_dindx >= 0) {
					TdpMng->SetChangeOverAnimation(anim_dindx);
				}
			}
			break;
		}
		}
		break;
	}


	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}