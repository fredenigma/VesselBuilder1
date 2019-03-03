#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "VCManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateVCPosDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= VCMng->GetPositionCount()) { return; }
	char namebuf[256] = { '\0' };
	sprintf(namebuf, "%s", VCMng->GetPositionName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_VCPOSNAME, namebuf);
	VECTOR3 ofs, dir;
	VCMng->GetPositionParams(idx, ofs, dir);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCPOSOFSX, IDC_EDIT_VCPOSOFSY, IDC_EDIT_VCPOSOFSZ, ofs);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCPOSDIRX, IDC_EDIT_VCPOSDIRY, IDC_EDIT_VCPOSDIRZ, dir);
	return;
}
BOOL DialogControl::VCPosDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_VCPOSNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_VCPOSNAME, cbuf, 256);
			string name(cbuf);
			VCMng->SetPositionName(idx, name);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BUTTON_VCPOSOFSSET:
		{
			VECTOR3 ofs = GetDlgItemsVector3(hWnd, IDC_EDIT_VCPOSOFSX, IDC_EDIT_VCPOSOFSY, IDC_EDIT_VCPOSOFSZ);
			VCMng->SetPositionPos(idx, ofs);
			break;
		}
		case IDC_BUTTON_VCPOSDIRSET:
		{
			VECTOR3 dir = GetDlgItemsVector3(hWnd, IDC_EDIT_VCPOSDIRX, IDC_EDIT_VCPOSDIRY, IDC_EDIT_VCPOSDIRZ);
			VCMng->SetPositionDir(idx, dir);
			break;
		}
		case IDC_BUTTON_VCPOSPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCPOSOFSX, IDC_EDIT_VCPOSOFSY, IDC_EDIT_VCPOSOFSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_VCPOSDIRX, IDC_EDIT_VCPOSDIRY, IDC_EDIT_VCPOSDIRZ, VB1->vclip.dir);
			}
			break;
		}
		case IDC_BUTTON_VCPOSCHECK:
		{
			VB1->clbkLoadVC(idx);
			break;
		}
		case IDC_BUTTON_VCPOSDELETE:
		{
			VCMng->DeletePosition(idx);
			UpdateTree(hDlg, VCPOS, 0);
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}