#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "PropellantManager.h"
#pragma comment(lib, "comctl32.lib")


void DialogControl::UpdatePrpDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= PrpMng->GetPropCount()) { return; }


	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%s", PrpMng->GetTankName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_TANKNAME, cbuf);
	sprintf(cbuf, "%.2f", PrpMng->GetTankMaxMass(idx));
	SetDlgItemText(hWnd, IDC_EDIT_TANKMAXMASS, cbuf);
	sprintf(cbuf, "%.2f", PrpMng->GetTankEfficiency(idx));
	SetDlgItemText(hWnd, IDC_EDIT_TANKEFFIC, cbuf);
	sprintf(cbuf, "%.2f", PrpMng->GetTankCurrentMass(idx));
	SetDlgItemText(hWnd, IDC_EDIT_TANKCURRMASS, cbuf);
	SetCheckBox(hWnd, IDC_CHECK_PRPKEEPFUEL, PrpMng->GetPrpRetainFuel(idx));
	if (PrpMng->IsPrimary(idx)) {
		SetDlgItemText(hWnd, IDC_EDIT_TANKISPRIMARY, "TRUE");
	}
	else {
		SetDlgItemText(hWnd, IDC_EDIT_TANKISPRIMARY, "FALSE");
	}

	return;
}

BOOL DialogControl::PrpDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_EDIT_TANKISPRIMARY, EM_SETREADONLY, true, 0);

		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_TANKNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_TANKNAME, cbuf, 256);
			string newname(cbuf);
			PrpMng->SetTankName(idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);

			break;
		}
		case IDC_BUTTON_TANKMAXMASSSET:
		{
			double newmaxmass = GetDlgItemDouble(hWnd, IDC_EDIT_TANKMAXMASS);
			PrpMng->SetTankMaxMass(idx, newmaxmass);
			break;
		}
		case IDC_BUTTON_TANKCURRMASSSET:
		{
			double newmass = GetDlgItemDouble(hWnd, IDC_EDIT_TANKCURRMASS);
			PrpMng->SetTankCurrentMass(idx, newmass);
			break;
		}
		case IDC_BUTTON_TANKEFFICSET:
		{
			double neweffic = GetDlgItemDouble(hWnd, IDC_EDIT_TANKEFFIC);
			PrpMng->SetTankEfficiency(idx, neweffic);
			break;
		}
		case IDC_BUTTON_DELETETANK:
		{
			PrpMng->DelTankDef(idx);
			UpdateTree(hDlg, PROPELLANT, CurrentSelection.config);
			break;
		}
		case IDC_BUTTON_TANKPRIMARY:
		{
			PrpMng->MakePrimary(idx);
			UpdatePrpDialog(hWnd);
			break;
		}
		case IDC_CHECK_PRPKEEPFUEL:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_PRPKEEPFUEL);
				PrpMng->SetPrpRetainFuel(idx, check);
			}
			break;
		}

		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}