#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "ControlSurfacesManager.h"
#include "AnimationManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateCtrlSurfDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= CtrSurfMng->CtrlSurfDefCount()) { return; }

	string name = CtrSurfMng->GetCtrlSurfDefName(idx);
	char namebuf[256] = { '\0' };
	sprintf(namebuf, "%s", name.c_str());
	SetDlgItemText(hWnd, IDC_EDIT_CSNAME, namebuf);

	double area, dcl, delay;
	int axis;
	AIRCTRL_TYPE type;
	VECTOR3 ref;
	UINT anim;
	CtrSurfMng->GetCtrlSurfDefParams(idx, type, area, dcl, ref, axis, delay,anim);
	SetDlgItemDouble(hWnd, IDC_EDIT_CSAREA, area, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_CSDCL, dcl, 2);
	SetDlgItemDouble(hWnd, IDC_EDIT_CSDELAY, delay, 1);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_CSREFX, IDC_EDIT_CSREFY, IDC_EDIT_CSREFZ, ref);
	int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_CSTYPE), (void*)type);
	SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_SETCURSEL, index,0);
	SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_SETCURSEL, axis, 0);
	
	SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_INSERTSTRING, 0, (LPARAM)"NONE");
	SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_SETITEMDATA, 0, (UINT)-1);
	for (UINT i = 0; i < AnimMng->GetAnimDefsCount(); i++) {
		if (!AnimMng->IsAnimValid(i)) { continue; }
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "%s", AnimMng->GetAnimName(i).c_str());
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_ADDSTRING, 0, (LPARAM)cbuf);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_SETITEMDATA, index, i);
	}
	if (anim != (UINT)-1) {
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_CSANIM), (void*)anim);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_SETCURSEL, index, 0);
		//SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_SETCURSEL, anim+1, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_SETCURSEL, 0, 0);
	}
	int Edits[6] = { IDC_EDIT_CSAREA,IDC_EDIT_CSDCL,IDC_EDIT_CSREFX,IDC_EDIT_CSREFY,IDC_EDIT_CSREFZ,IDC_EDIT_CSDELAY };
	int Combos[3] = { IDC_COMBO_CSTYPE,IDC_COMBO_CSAXIS,IDC_COMBO_CSANIM };
	if (CtrSurfMng->IsCtrlSurfDefDefined(idx)) {
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_CSDEFUNDEF), "UNDEFINE");
		for (UINT i = 0; i < 6; i++) {
			SendDlgItemMessage(hWnd, Edits[i], EM_SETREADONLY, true, 0);
		}
		for (UINT j = 0; j < 3; j++) {
			EnableWindow(GetDlgItem(hWnd, Combos[j]), false);
		}
	}
	else {
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_CSDEFUNDEF), "DEFINE");
		for (UINT i = 0; i < 6; i++) {
			SendDlgItemMessage(hWnd, Edits[i], EM_SETREADONLY, false, 0);
		}
		for (UINT j = 0; j < 3; j++) {
			EnableWindow(GetDlgItem(hWnd, Combos[j]), true);
		}
	}
	return;
}
BOOL DialogControl::CtrlSurfDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_RESETCONTENT, 0, 0);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_INSERTSTRING, 0, (LPARAM)"ELEVATOR");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_SETITEMDATA, 0, AIRCTRL_TYPE::AIRCTRL_ELEVATOR);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_INSERTSTRING, 1, (LPARAM)"RUDDER");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_SETITEMDATA, 1, AIRCTRL_TYPE::AIRCTRL_RUDDER);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_INSERTSTRING, 2, (LPARAM)"AILERON");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_SETITEMDATA, 2, AIRCTRL_TYPE::AIRCTRL_AILERON);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_INSERTSTRING, 3, (LPARAM)"FLAP");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_SETITEMDATA, 3, AIRCTRL_TYPE::AIRCTRL_FLAP);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_INSERTSTRING, 4, (LPARAM)"ELEVATOR_TRIM");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_SETITEMDATA, 4, AIRCTRL_TYPE::AIRCTRL_ELEVATORTRIM);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_INSERTSTRING, 5, (LPARAM)"RUDDER_TRIM");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_SETITEMDATA, 5, AIRCTRL_TYPE::AIRCTRL_RUDDERTRIM);

		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_RESETCONTENT, 0, 0);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_INSERTSTRING, 0, (LPARAM)"AXIS AUTO");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_SETITEMDATA, 0, (LPARAM)AIRCTRL_AXIS_AUTO);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_INSERTSTRING, 1, (LPARAM)"AXIS YPOS");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_SETITEMDATA, 1, (LPARAM)AIRCTRL_AXIS_YPOS);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_INSERTSTRING, 2, (LPARAM)"AXIS YNEG");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_SETITEMDATA, 2, (LPARAM)AIRCTRL_AXIS_YNEG);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_INSERTSTRING, 3, (LPARAM)"AXIS XPOS");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_SETITEMDATA, 3, (LPARAM)AIRCTRL_AXIS_XPOS);
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_INSERTSTRING, 4, (LPARAM)"AXIS XNEG");
		SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_SETITEMDATA, 4, (LPARAM)AIRCTRL_AXIS_XNEG);

		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_CSAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_CSNAME, cbuf, 256);
			string newname(cbuf);
			CtrSurfMng->SetCtrlSurfDefName(idx, newname);

			char nbuf[256] = { '\0' };
			if (CtrSurfMng->IsCtrlSurfDefDefined(idx)) {
				sprintf(nbuf, cbuf);
			}
			else {
				sprintf(nbuf, "*%s", cbuf);
			}
			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)nbuf;
			tvi.cchTextMax = strlen(nbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BUTTON_CSPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_CSREFX, IDC_EDIT_CSREFY, IDC_EDIT_CSREFZ, VB1->vclip.pos);
			}
			break;
		}
		case IDC_BUTTON_CSDEFUNDEF:
		{
			if (!CtrSurfMng->IsCtrlSurfDefDefined(idx)) {
				double area = GetDlgItemDouble(hWnd, IDC_EDIT_CSAREA);
				double dcl = GetDlgItemDouble(hWnd, IDC_EDIT_CSDCL);
				double delay = GetDlgItemDouble(hWnd, IDC_EDIT_CSDELAY);
				VECTOR3 ref = GetDlgItemsVector3(hWnd, IDC_EDIT_CSREFX, IDC_EDIT_CSREFY, IDC_EDIT_CSREFZ);
				int axis = SendDlgItemMessage(hWnd, IDC_COMBO_CSAXIS, CB_GETCURSEL, 0, 0);
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_GETCURSEL, 0, 0);
				AIRCTRL_TYPE type = (AIRCTRL_TYPE)SendDlgItemMessage(hWnd, IDC_COMBO_CSTYPE, CB_GETITEMDATA, index, 0);
				index = SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_GETCURSEL, 0, 0);
				UINT anim = SendDlgItemMessage(hWnd, IDC_COMBO_CSANIM, CB_GETITEMDATA, index, 0);
				CtrSurfMng->SetCtrlSurfDefParams(idx, type, area, dcl, ref, axis, delay, anim);
				CtrSurfMng->DefineCtrlSurface(idx);
			}
			else {
				CtrSurfMng->UndefineCtrlSurface(idx);
			}
			
			UpdateCtrlSurfDialog(hWnd);

			string name = CtrSurfMng->GetCtrlSurfDefName(idx);
			char nbuf[256] = { '\0' };
			if (CtrSurfMng->IsCtrlSurfDefDefined(idx)) {
				sprintf(nbuf, "%s",name.c_str());
			}
			else {
				sprintf(nbuf, "*%s", name.c_str());
			}
			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)nbuf;
			tvi.cchTextMax = strlen(nbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BUTTON_CSDELETE:
		{
			CtrSurfMng->DeleteCtrlSurfDef(idx);
			UpdateTree(hDlg, CTRLSURFACES, CurrentSelection.config);
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}