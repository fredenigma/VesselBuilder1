#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "AirfoilsManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateAirfoilDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= AirfoilMng->GetAirfoilDefCount()) { return; }
	VECTOR3 ref;
	double c, S, A;
	AirfoilMng->GetAirfoilDefParams(idx, ref, c, S, A);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_AIRFOILREFX, IDC_EDIT_AIRFOILREFY, IDC_EDIT_AIRFOILREFZ, ref);
	SetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILC, c, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILS, S, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILA, A, 2);
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%s", AirfoilMng->GetAirfoilDefName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_AIRFOILNAME, (LPCSTR)cbuf);
	switch (AirfoilMng->GetAirfoilDefModel(idx)) {
	case AIRFOILS_DEFAULTS::NODEF:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETCURSEL, 0, 0);
		break;
	}
	case AIRFOILS_DEFAULTS::DELTAGLIDER:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETCURSEL, 1, 0);
		break;
	}
	case AIRFOILS_DEFAULTS::SHUTTLE:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETCURSEL, 2, 0);
		break;
	}
	case AIRFOILS_DEFAULTS::CAPSULE:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETCURSEL, 3, 0);
		break;
	}
	}
	UpdateAirfoilFuncList(hWnd);
	return;
}
void DialogControl::UpdateAirfoilFuncList(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	UINT PointsCount = AirfoilMng->GetAirfoilDefPointsCount(idx);
	SendDlgItemMessage(hWnd, IDC_LIST_AIRFOILFUNC, LB_RESETCONTENT, 0, 0);
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "    AOA         CL         CM");
	SendDlgItemMessage(hWnd, IDC_LIST_AIRFOILFUNC, LB_INSERTSTRING, 0, (LPARAM)cbuf);
	for (UINT i = 0; i < PointsCount; i++) {
		for (UINT k = 0; k < 256; k++) { cbuf[k] = '\0'; }
		double aoa, cl, cm;
		AirfoilMng->GetAirfoilDefPoint(idx, i, aoa, cl, cm);
		sprintf(cbuf, "%+06.1f    %+05.3f    %+05.3f", aoa*DEG, cl, cm);
		SendDlgItemMessage(hWnd, IDC_LIST_AIRFOILFUNC, LB_INSERTSTRING, i + 1, (LPARAM)cbuf);
	}
	return;
}
void DialogControl::ShowAirfoilFuncGraph(HWND hWnd, bool show) {
	RECT rect;
	GetWindowRect(hDlg, &rect);
	int currwheight = rect.bottom - rect.top;
	int currwwidth = rect.right - rect.left;
	
	if (show&&!ShowingAFGraph) {
		SetWindowPos(hDlg, HWND_TOP, rect.left, rect.top, currwwidth+500, currwheight, 0);
		ShowingAFGraph = true;
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_AIRFOILVIEWGRAPH), "Close Graph <<");
	}
	else if(!show&&ShowingAFGraph) {
		SetWindowPos(hDlg, HWND_TOP, rect.left, rect.top, currwwidth - 500, currwheight, 0);
		ShowingAFGraph = false;
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_AIRFOILVIEWGRAPH), "View Graph >>");
	}
	else {
		return;
	}
	return;
}
void DialogControl::UpdateAirfoilFuncGraph(HWND hWnd) {
	HWND hCtrl = GetDlgItem(hWnd, IDC_STATIC_AIRFOILGRAPH);
	HDC hDC = GetDC(hCtrl);
	RECT rect;
	GetClientRect(hCtrl, &rect);
	SelectObject(hDC, GetStockObject(WHITE_BRUSH));
	SelectObject(hDC, GetStockObject(BLACK_PEN));
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	int cntx, cnty;
	cntx = (rect.right + rect.left) / 2;
	cnty = (rect.bottom + rect.top) / 2;
	SelectObject(hDC, penblack);
	MoveToEx(hDC, cntx, rect.top, NULL); LineTo(hDC, cntx, rect.bottom);
	MoveToEx(hDC, rect.left, cnty, NULL); LineTo(hDC, rect.right, cnty);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	SelectObject(hDC, pengray);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(200, 200, 200));
	int k = 15;
	for (UINT i = 0; i < 13; i++) {
		MoveToEx(hDC, i*width / 12, rect.top, NULL); LineTo(hDC, i*width / 12, rect.bottom);
		char cbuf[64] = { '\0' };
		sprintf(cbuf, "%i", (30 * i) - 180);
		int delta = (k*i) / 12;
		TextOut(hDC, (i*width / 12)-delta, rect.top + 10, cbuf, 4);
	}
	



	UINT PointsCount = AirfoilMng->GetAirfoilDefPointsCount(CurrentSelection.idx);
	if (PointsCount <= 0) { ReleaseDC(hCtrl, hDC); return; }
	SelectObject(hDC, penblue_l);
	double aoa, cl, cm;
	AirfoilMng->GetAirfoilDefPoint(CurrentSelection.idx, 0, aoa, cl, cm);
	aoa *= DEG;
	MoveToEx(hDC, ((aoa + 180) / 360)*width, ((1-cl)/2)* height, NULL);
	for (UINT i = 1; i < PointsCount; i++) {
		AirfoilMng->GetAirfoilDefPoint(CurrentSelection.idx, i, aoa, cl, cm);
		aoa *= DEG;
		LineTo(hDC, ((aoa + 180) / 360)*width, ((1 - cl) / 2)* height);
	}



	ReleaseDC(hCtrl, hDC);
	
	return;
}


BOOL DialogControl::AirfoilsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_RESETCONTENT, 0, 0);
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_INSERTSTRING, 0, (LPARAM)"NONE");
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETITEMDATA, 0, AIRFOILS_DEFAULTS::NODEF);
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_INSERTSTRING, 1, (LPARAM)"DeltaGlider");
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETITEMDATA, 1, AIRFOILS_DEFAULTS::DELTAGLIDER);
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_INSERTSTRING, 2, (LPARAM)"Space Shuttle");
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETITEMDATA, 2, AIRFOILS_DEFAULTS::SHUTTLE);
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_INSERTSTRING, 3, (LPARAM)"Capsule");
		SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_SETITEMDATA, 3, AIRFOILS_DEFAULTS::CAPSULE);
		break;
	}
	case WM_PAINT:
	{
		if (ShowingAFGraph) {
			InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_AIRFOILGRAPH), NULL, FALSE);
			UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_AIRFOILGRAPH));
			UpdateAirfoilFuncGraph(hWnd);
		}
		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_AIRFOILNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_AIRFOILNAME, cbuf, 256);
			string newname(cbuf);
			AirfoilMng->SetAirfoilDefName(idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BUTTON_AIRFOILREFSET:
		{
			VECTOR3 newref = GetDlgItemsVector3(hWnd, IDC_EDIT_AIRFOILREFX, IDC_EDIT_AIRFOILREFY, IDC_EDIT_AIRFOILREFZ);
			AirfoilMng->SetAirfoilDefRef(idx, newref);
			break;
		}
		case IDC_BUTTON_AIRFOILCSET:
		{
			double newc = GetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILC);
			AirfoilMng->SetAirfoilDefC(idx, newc);
			break;
		}
		case IDC_BUTTON_AIRFOILSSET:
		{
			double newS = GetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILS);
			AirfoilMng->SetAirfoilDefS(idx, newS);
			break;
		}
		case IDC_BUTTON_AIRFOILASET:
		{
			double newA = GetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILA);
			AirfoilMng->SetairfoilDefA(idx, newA);
			break;
		}
		case IDC_BUTTON_AIRFOILADDPOINT:
		{
			double aoa, cl, cm;
			aoa = GetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILAOA);
			aoa *= RAD;
			cl = GetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILCL);
			cm = GetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILCM);
			if (AirfoilMng->AddPointAirfoiDef(idx, aoa, cl, cm)) {
				SetDlgItemText(hWnd, IDC_EDIT_AIRFOILAOA, "");
				SetDlgItemText(hWnd, IDC_EDIT_AIRFOILCL, "");
				SetDlgItemText(hWnd, IDC_EDIT_AIRFOILCM, "");
				UpdateAirfoilFuncList(hWnd);
			}
			
			break;
		}
		case IDC_BUTTON_AIRFOILREMPOINT:
		{
			int index = SendDlgItemMessage(hWnd, IDC_LIST_AIRFOILFUNC, LB_GETCURSEL, 0, 0);
			if (index > 0) {
				double aoa, cl, cm;
				AirfoilMng->GetAirfoilDefPoint(idx, index-1, aoa, cl, cm);
				SetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILAOA, aoa*DEG, 1);
				SetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILCL, cl, 3);
				SetDlgItemDouble(hWnd, IDC_EDIT_AIRFOILCM, cm, 3);
				AirfoilMng->RemovePointAirfoilDef(idx, index - 1);
				UpdateAirfoilFuncList(hWnd);
			}
			break;
		}
		case IDC_BUTTON_DELAIRFOIL:
		{
			AirfoilMng->DeleteAirfoilDef(idx);
			UpdateTree(hDlg, AIRFOILS, 0);
			break;
		}
		case IDC_COMBO_AIRFOILDEFAULTS:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_GETCURSEL, 0, 0);
				AIRFOILS_DEFAULTS adf = (AIRFOILS_DEFAULTS)SendDlgItemMessage(hWnd, IDC_COMBO_AIRFOILDEFAULTS, CB_GETITEMDATA, index, 0);
				AirfoilMng->SetAirfoilDefModel(idx, adf);
				UpdateAirfoilFuncList(hWnd);
				
			}
			break;
		}
		case IDC_BUTTON_AIRFOILVIEWGRAPH:
		{
			ShowAirfoilFuncGraph(hWnd, !ShowingAFGraph);
			UpdateAirfoilFuncGraph(hWnd);
			break;
		}


		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}