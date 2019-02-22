#include "StationBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "PropellantManager.h"
#include "ThrusterManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateThrDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= ThrMng->GetThrCount()) { return; }
	char cbuf[256] = { '\0' };
	sprintf(cbuf, ThrMng->GetThrName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_THNAME, cbuf);

	VECTOR3 pos = ThrMng->GetThrPos(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_THPOSX, IDC_EDIT_THPOSY, IDC_EDIT_THPOSZ, pos);
	VECTOR3 dir = ThrMng->GetThrDir(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, dir);

	double max0 = ThrMng->GetThrMax0(idx);
	sprintf(cbuf, "%.2f", max0);
	SetDlgItemText(hWnd, IDC_EDIT_THMAX, cbuf);
	double isp = ThrMng->GetThrIsp0(idx);
	sprintf(cbuf, "%.2f", isp);
	SetDlgItemText(hWnd, IDC_EDIT_THISP, cbuf);
	double ispref = ThrMng->GetThrIspRef(idx);
	sprintf(cbuf, "%.2f", ispref);
	SetDlgItemText(hWnd, IDC_EDIT_THISPREF, cbuf);
	double pref = ThrMng->GetThrPRef(idx);
	sprintf(cbuf, "%.2f", pref);
	SetDlgItemText(hWnd, IDC_EDIT_THPREF, cbuf);

	SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_RESETCONTENT, 0, 0);
	int index = SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_ADDSTRING, 0, (LPARAM)"NONE");
	SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETITEMDATA, index, (LPARAM)NULL);
	int topick = -1;
	for (UINT i = 0; i < PrpMng->GetPropCount(); i++) {
		index = SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_ADDSTRING, 0, (LPARAM)PrpMng->GetTankName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETITEMDATA, index, (LPARAM)PrpMng->GetTankPH(i));
		if (PrpMng->GetTankPH(i) == ThrMng->GetThrPh(idx)) {
			topick = index;
		}
	}
	if (topick != -1) {
		SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETCURSEL, topick, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETCURSEL, 0, 0);
	}

	return;
}
BOOL DialogControl::ThrDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_THPOSY, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_THPOSZ, UDM_SETRANGE32, -10000, 10000);
		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_THNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_THNAME, cbuf, 256);
			string newname(cbuf);
			ThrMng->SetThrName(idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);

			break;
		}
		case IDC_BUTTON_THPOSSET:
		{
			VECTOR3 newpos = GetDlgItemsVector3(hWnd, IDC_EDIT_THPOSX, IDC_EDIT_THPOSY, IDC_EDIT_THPOSZ);
			ThrMng->SetThrPos(idx, newpos);
			break;
		}
		case IDC_BUTTON_THDIRSET:
		{
			VECTOR3 newdir = GetDlgItemsVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ);
			ThrMng->SetThrDir(idx, newdir);
			break;
		}
		case IDC_BUTTON_THMAXSET:
		{
			double newmax = GetDlgItemDouble(hWnd, IDC_EDIT_THMAX);
			ThrMng->SetThrMax0(idx, newmax);
			break;
		}
		case IDC_BUTTON_THISPSET:
		{
			double newisp = GetDlgItemDouble(hWnd, IDC_EDIT_THISP);
			ThrMng->SetThrIsp(idx, newisp);
			break;
		}
		case IDC_BUTTON_THISPREFSET:
		{
			double newispref = GetDlgItemDouble(hWnd, IDC_EDIT_THISPREF);
			ThrMng->SetThrIspRef(idx, newispref);
			break;
		}
		case IDC_BUTTON_THPREFSET:
		{
			double newpref = GetDlgItemDouble(hWnd, IDC_EDIT_THPREF);
			ThrMng->SetThrPRef(idx, newpref);
			break;
		}
		case IDC_BUTTON_DELETETH:
		{
			ThrMng->DelThrDef(idx);
			UpdateTree(hDlg, THRUSTERS, 0);
			break;
		}
		case IDC_BTN_THPASTEV:
		{
			if (SB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_THPOSX, IDC_EDIT_THPOSY, IDC_EDIT_THPOSZ, SB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, SB1->vclip.dir);
			}
			break;
		}
		case IDC_BUTTON_THXP:
		{
			VECTOR3 xp = _V(1, 0, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, xp);
			break;
		}
		case IDC_BUTTON_THXM:
		{
			VECTOR3 xm = _V(-1, 0, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, xm);
			break;
		}
		case IDC_BUTTON_THYP:
		{
			VECTOR3 yp = _V(0, 1, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, yp);
			break;
		}
		case IDC_BUTTON_THYM:
		{
			VECTOR3 ym = _V(0, -1, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, ym);
			break;
		}
		case IDC_BUTTON_THZP:
		{
			VECTOR3 zp = _V(0, 0, 1);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, zp);
			break;
		}
		case IDC_BUTTON_THZM:
		{
			VECTOR3 zm = _V(0, 0, -1);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, zm);
			break;
		}
		case IDC_COMBO_THPH:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_GETCURSEL, 0, 0);
				PROPELLANT_HANDLE ph = (PROPELLANT_HANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_GETITEMDATA, index, 0);
				ThrMng->SetThrPH(idx, ph);
			}
			break;
		}
		case IDC_CHECK_THSHOW:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_THSHOW, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					SB1->CreateThExhausts();
				}
				else {
					SB1->DeleteThExhausts();
				}
			}
			break;
		}


		}
		break;
	}
	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SPIN_THPOSX:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.15;
				VECTOR3 pos = ThrMng->GetThrPos(CurrentSelection.idx);
				pos.x += spd*(double)lpd->iDelta;
				ThrMng->SetThrPos(CurrentSelection.idx, pos);
				UpdateThrDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_THPOSY:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.15;
				VECTOR3 pos = ThrMng->GetThrPos(CurrentSelection.idx);
				pos.y += spd*(double)lpd->iDelta;
				ThrMng->SetThrPos(CurrentSelection.idx, pos);
				UpdateThrDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_THPOSZ:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.15;
				VECTOR3 pos = ThrMng->GetThrPos(CurrentSelection.idx);
				pos.z += spd*(double)lpd->iDelta;
				ThrMng->SetThrPos(CurrentSelection.idx, pos);
				UpdateThrDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETPOS, 0, 0);
			}
			break;
		}

		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}