#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "TouchdownPointsManager.h"
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
		sprintf(cbuf, "Point:%i Pos:%.2f %.2f %.2f Damp:%.1e Stiff:%.1e Mu:%.1f Mu lng:%.1f", i,curset[i].pos.x, curset[i].pos.y, curset[i].pos.z, curset[i].damping, curset[i].stiffness, curset[i].mu, curset[i].mu_lng);
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
				vector<TOUCHDOWNVTX> tdvtx;
				VB1->CreateTDPExhausts(true, tdvtx);
			}
			else {
				VB1->DeleteTDPExhausts(true);
			}
			break;
		}
		case IDC_CHECK_TDPSHOWSET:
		{
			LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_TDPSHOWSET, BM_GETCHECK, 0, 0);
			if (getcheck == BST_CHECKED) {
				vector<TOUCHDOWNVTX> tdvtx = TdpMng->GetSet(set);
				VB1->CreateTDPExhausts(false, tdvtx);
			}
			else {
				VB1->DeleteTDPExhausts(false);
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
			double Mass = VB1->GetMass();
			double ro = 15;
			double HeightFromGround = 0.5;
			TOUCHDOWNVTX td[4];

			double x_target = -0.5;
			double stiffness = (-1)*(Mass*9.80655) / (3 * x_target);
			double damping = 0.9*(2 * sqrt(Mass*stiffness));
			
			for (int i = 0; i<4; i++)
			{

				td[i].damping = damping;
				td[i].mu = 3;
				td[i].mu_lng = 3;
				td[i].stiffness = stiffness;
			}
			td[0].pos.x = -cos(30 * RAD)*ro;
			td[0].pos.y = -HeightFromGround;
			td[0].pos.z = -sin(30 * RAD)*ro;
			td[1].pos.x = 0;
			td[1].pos.y = -HeightFromGround;
			td[1].pos.z = 1 * ro;
			td[2].pos.x = cos(30 * RAD)*ro;
			td[2].pos.y = -HeightFromGround;
			td[2].pos.z = -sin(30 * RAD)*ro;
			td[3].pos.x = 0;
			td[3].pos.y = 1 * ro; 
			td[3].pos.z = 0;

			for (int i = 0; i < 4; i++) {
				TdpMng->AddPoint(1, td[i]);
			}
			UpdateTdpDialog(hWnd);
			
			break;
		}
		case IDC_BUTTON_TDPAPPLY:
		{
			vector<TOUCHDOWNVTX>set = TdpMng->GetSet(1);
			DWORD ntdvtx = set.size();
			if (ntdvtx < 3) { break; }
			TOUCHDOWNVTX *td = new TOUCHDOWNVTX[ntdvtx];
			for (UINT i = 0; i < ntdvtx; i++) {
				td[i] = set[i];
			}
			VB1->SetTouchdownPoints(td, ntdvtx);
			delete[] td;

			break;
		}
		}
		break;
	}


	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}