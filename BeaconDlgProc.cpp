#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "LightsManager.h"

void DialogControl::UpdateBeaconsDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= LightsMng->GetBeaconCount()) { return; }
	char namebuf[256] = { '\0' };
	sprintf(namebuf, LightsMng->GetBeaconName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_BCNNAME, namebuf);
	DWORD shape;
	VECTOR3 pos, col;
	double size, falloff, period, duration, tofs;
	LightsMng->GetBeaconParams(idx, shape, pos, col, size, falloff, period, duration, tofs);
	int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_BCNSHAPE), shape);
	if (index != -1) { SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_SETCURSEL, index, 0); }
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_BCNPOSX, IDC_EDIT_BCNPOSY, IDC_EDIT_BCNPOSZ, pos);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_BCNCOLR, IDC_EDIT_BCNCOLG, IDC_EDIT_BCNCOLB, col);
	SetDlgItemDouble(hWnd, IDC_EDIT_BCNSIZE, size, 2);
	SetDlgItemDouble(hWnd, IDC_EDIT_BCNFALLOFF, falloff, 2);
	SetDlgItemDouble(hWnd, IDC_EDIT_BCNPERIOD, period, 2);
	SetDlgItemDouble(hWnd, IDC_EDIT_BCNDURATION, duration, 2);
	SetDlgItemDouble(hWnd, IDC_EDIT_BCNTOFS, tofs, 2);
	return;
}

BOOL DialogControl::BeaconsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_RESETCONTENT, 0, 0);
		SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_INSERTSTRING, 0, (LPARAM)"COMPACT");
		SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_SETITEMDATA, 0, (LPARAM)BEACONSHAPE_COMPACT);
		SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_INSERTSTRING, 1, (LPARAM)"DIFFUSE");
		SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_SETITEMDATA, 1, (LPARAM)BEACONSHAPE_DIFFUSE);
		SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_INSERTSTRING, 2, (LPARAM)"STAR");
		SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_SETITEMDATA, 2, (LPARAM)BEACONSHAPE_STAR);
		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_BCNNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_BCNNAME, cbuf, 256);
			string name(cbuf);
			LightsMng->SetBeaconName(idx, name);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_COMBO_BCNSHAPE:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_GETCURSEL, 0, 0);
				DWORD shape = (DWORD)SendDlgItemMessage(hWnd, IDC_COMBO_BCNSHAPE, CB_GETITEMDATA, index, 0);
				LightsMng->SetBeaconShape(idx, shape);
			}
			break;
		}
		case IDC_BUTTON_BCNPOSSET:
		{
			VECTOR3 pos = GetDlgItemsVector3(hWnd, IDC_EDIT_BCNPOSX, IDC_EDIT_BCNPOSY, IDC_EDIT_BCNPOSZ);
			LightsMng->SetBeaconPos(idx, pos);
			break;
		}
		case IDC_BUTTON_BCNPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_BCNPOSX, IDC_EDIT_BCNPOSY, IDC_EDIT_BCNPOSZ, VB1->vclip.pos);
			}
			break;
		}
		case IDC_BUTTON_BCNCOLSET:
		{
			VECTOR3 col = GetDlgItemsVector3(hWnd, IDC_EDIT_BCNCOLR, IDC_EDIT_BCNCOLG, IDC_EDIT_BCNCOLB);
			LightsMng->SetBeaconCol(idx, col);
			break;
		}
		case IDC_BUTTON_BCNSIZESET:
		{
			double size = GetDlgItemDouble(hWnd, IDC_EDIT_BCNSIZE);
			LightsMng->SetBeaconSize(idx, size);
			break;
		}
		case IDC_BUTTON_BCNFALLOFFSET:
		{
			double falloff = GetDlgItemDouble(hWnd, IDC_EDIT_BCNFALLOFF);
			LightsMng->SetBeaconFalloff(idx, falloff);
			break;
		}
		case IDC_BUTTON_BCNPERIODSET:
		{
			double period = GetDlgItemDouble(hWnd, IDC_EDIT_BCNPERIOD);
			LightsMng->SetBeaconPeriod(idx, period);
			break;
		}
		case IDC_BUTTON_BCNDURATIONSET:
		{
			double duration = GetDlgItemDouble(hWnd, IDC_EDIT_BCNDURATION);
			LightsMng->SetBeaconDuration(idx, duration);
			break;
		}
		case IDC_BUTTON_BCNTOFSSET:
		{
			double tofs = GetDlgItemDouble(hWnd, IDC_EDIT_BCNTOFS);
			LightsMng->SetBeaconTofs(idx, tofs);
			break;
		}
		case IDC_BUTTON_BCNTOGGLE:
		{
			LightsMng->ToggleBeaconActive(idx);			
			break;
		}
		case IDC_BUTTON_BCNDEL:
		{
			LightsMng->DeleteBeaconDef(idx);
			UpdateTree(hDlg, BEACONS, 0);
			break;
		}






		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}