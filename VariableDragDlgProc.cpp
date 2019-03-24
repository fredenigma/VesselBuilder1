#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "VariableDragManager.h"
#include "AnimationManager.h"

void DialogControl::UpdateVarDragDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= VardMng->GetVardDefCount()) { return; }

	char namebuf[256] = { '\0' };
	sprintf(namebuf, "%s", VardMng->GetVardName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_VDNAME, namebuf);

	int edits[4] = { IDC_EDIT_VBX,IDC_EDIT_VBY,IDC_EDIT_VBZ,IDC_EDIT_VBFACTOR };
	bool defined = VardMng->IsVardDefDefined(idx);
	for (UINT i = 0; i < 4; i++) {
		SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, defined ? true : false, 0);
	}
	EnableWindow(GetDlgItem(hWnd, IDC_COMBO_VDANIM), defined ? false : true);
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_VDPASTEV), defined ? false : true);
	SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_VBDEFUNDEF), defined ? "UNDEFINE THIS VARIABLE DRAG ELEMENT" : "DEFINE THIS VARIABLE DRAG ELEMENT");

	SendDlgItemMessage(hWnd, IDC_COMBO_VDANIM, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < AnimMng->GetAnimDefsCount(); i++) {
		if (!AnimMng->IsAnimValid(i)) { continue; }
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "%s", AnimMng->GetAnimName(i).c_str());
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_VDANIM, CB_ADDSTRING, 0, (LPARAM)cbuf);
		SendDlgItemMessage(hWnd, IDC_COMBO_VDANIM, CB_SETITEMDATA, index, i);
	}
	double factor;
	VECTOR3 ref;
	UINT an_idx;
	VardMng->GetVardParams(idx, factor, ref, an_idx);
	if (an_idx != (UINT)-1) {
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_VDANIM), (void*)an_idx);
		SendDlgItemMessage(hWnd, IDC_COMBO_VDANIM, CB_SETCURSEL, index, 0);
	}
	SetDlgItemDouble(hWnd, IDC_EDIT_VBFACTOR, factor, 2);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_VBX, IDC_EDIT_VBY, IDC_EDIT_VBZ, ref);	

	return;
}
BOOL DialogControl::VarDragDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_VDPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VBX, IDC_EDIT_VBY, IDC_EDIT_VBZ, VB1->vclip.pos);
			}
			break;
		}
		case IDC_BUTTON_VBDEFUNDEF:
		{
			
			

			bool defined = VardMng->IsVardDefDefined(idx);
			if (defined) {
				VardMng->UnDefineVardDef(idx);
			}
			else {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_VDANIM, CB_GETCURSEL, 0, 0);
				UINT anim_index = (UINT)SendDlgItemMessage(hWnd, IDC_COMBO_VDANIM, CB_GETITEMDATA, index, 0);
				if (anim_index==(UINT)-1 || anim_index>AnimMng->GetAnimDefsCount()) { break; }
				double factor = GetDlgItemDouble(hWnd, IDC_EDIT_VBFACTOR);
				VECTOR3 ref = GetDlgItemsVector3(hWnd, IDC_EDIT_VBX, IDC_EDIT_VBY, IDC_EDIT_VBZ);
				VardMng->SetVardParams(idx, factor, ref, anim_index);
				VardMng->DefineVarDef(idx);
			}
			UpdateVarDragDialog(hWnd);

			char nbuf[256] = { '\0' };
			if (!defined) {
				sprintf(nbuf, "%s", VardMng->GetVardName(idx).c_str());
			}
			else {
				sprintf(nbuf, "*%s", VardMng->GetVardName(idx).c_str());
			}

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)nbuf;
			tvi.cchTextMax = strlen(nbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BUTTON_VBDELETE:
		{
			VardMng->DeleteVarDef(idx);
			UpdateTree(hDlg, VARIABLEDRAG, CurrentSelection.config);
			break;
		}
		case IDC_BUTTON_VDNAMESET:
		{
			char namebuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_VDNAME, namebuf, 256);
			string name(namebuf);
			VardMng->SetVardName(idx, name);

			char nbuf[256] = { '\0' };
			if (VardMng->IsVardDefDefined(idx)) {
				sprintf(nbuf, "%s", namebuf);
			}
			else {
				sprintf(nbuf, "*%s", namebuf);
			}
			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)nbuf;
			tvi.cchTextMax = strlen(nbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}