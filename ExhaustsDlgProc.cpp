#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "ExhaustManager.h"
#include "ThrusterManager.h"
#include "ExTexManager.h"

void DialogControl::UpdateExDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= ExMng->GetExDefCount()) { return; }
	char namebuf[256] = { '\0' };
	sprintf(namebuf, "%s", ExMng->GetExName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_EXNAME, namebuf);

	SendDlgItemMessage(hWnd, IDC_COMBO_EXTH, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < VB1->ThrMng->GetThrCount(); i++) {
		char nbuf[256] = { '\0' };
		sprintf(nbuf, "%s", VB1->ThrMng->GetThrName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_EXTH, CB_INSERTSTRING, i, (LPARAM)nbuf);
		SendDlgItemMessage(hWnd, IDC_COMBO_EXTH, CB_SETITEMDATA, i, (LPARAM)VB1->ThrMng->GetThrTH(i));
	}
	SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_RESETCONTENT, 0, 0);
	
	for (UINT i = 0; i < VB1->ExTMng->GetExTexCount(); i++) {
		char ebuf[256] = { '\0' };
		sprintf(ebuf, "%s", ExTMng->GetExTexName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_INSERTSTRING, i, (LPARAM)ebuf);
		SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_SETITEMDATA, i, (LPARAM)VB1->ExTMng->GetExTexSurf(i));
	}
	SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_INSERTSTRING, 0, (LPARAM)"DEFAULT");
	SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_SETITEMDATA, 0, (LPARAM)NULL);
	SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_SETCURSEL, 0, 0);
	int Edits[8] = {IDC_EDIT_EXPOSX,IDC_EDIT_EXPOSY,IDC_EDIT_EXPOSZ, IDC_EDIT_EXDIRX,IDC_EDIT_EXDIRY,IDC_EDIT_EXDIRZ, IDC_EDIT_EXLSIZE,IDC_EDIT_EXWSIZE, };
	int Cmds[4] = {IDC_COMBO_EXTH,IDC_COMBO_EXTEXT,IDC_CHECK_EXCUSTOMPOSDIR,IDC_BUTTON_EXPASTEV };
	if (ExMng->IsExDefined(idx)) {
		for (UINT i = 0; i < 8; i++) {
			SendDlgItemMessage(hWnd, Edits[i], EM_SETREADONLY, true, 0);
			if (i < 4) {
				EnableWindow(GetDlgItem(hWnd, Cmds[i]), false);
			}
		}
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_EXDEFUNDEF), "UNDEFINE EXHAUST");

		UINT thr_index, extex;
		double lsize, wsize;
		VECTOR3 pos, dir;
		bool customposdir, customex;
		ExMng->GetExParams(idx, thr_index, lsize, wsize, pos, dir, extex, customposdir, customex);
		if (customposdir) {
			SendDlgItemMessage(hWnd, IDC_CHECK_EXCUSTOMPOSDIR, BM_SETCHECK, BST_CHECKED, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_EXPOSX, IDC_EDIT_EXPOSY, IDC_EDIT_EXPOSZ, pos);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_EXDIRX, IDC_EDIT_EXDIRY, IDC_EDIT_EXDIRZ, dir);
		}
		else {
			SendDlgItemMessage(hWnd, IDC_CHECK_EXCUSTOMPOSDIR, BM_SETCHECK, BST_UNCHECKED, 0);
			int cdedits[6] = { IDC_EDIT_EXPOSX, IDC_EDIT_EXPOSY, IDC_EDIT_EXPOSZ, IDC_EDIT_EXDIRX, IDC_EDIT_EXDIRY, IDC_EDIT_EXDIRZ };
			for (UINT i = 0; i < 6; i++) {
				SetDlgItemText(hWnd, cdedits[i], "");
			}
		}
		SetDlgItemDouble(hWnd, IDC_EDIT_EXLSIZE, lsize, 1);
		SetDlgItemDouble(hWnd, IDC_EDIT_EXWSIZE, wsize, 1);


		SendDlgItemMessage(hWnd, IDC_COMBO_EXTH, CB_SETCURSEL, thr_index, 0);
		SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_SETCURSEL, extex + 1, 0);

	}
	else {
		for (UINT i = 0; i < 3; i++) {
			EnableWindow(GetDlgItem(hWnd, Cmds[i]), true);
		}
		SendDlgItemMessage(hWnd, IDC_CHECK_EXCUSTOMPOSDIR, BM_SETCHECK, BST_UNCHECKED, 0);
		for (UINT i = 0; i < 6; i++) {
			SendDlgItemMessage(hWnd, Edits[i], EM_SETREADONLY, true, 0);
		}
		SendDlgItemMessage(hWnd, IDC_EDIT_EXLSIZE, EM_SETREADONLY, false,0);
		SendDlgItemMessage(hWnd, IDC_EDIT_EXWSIZE, EM_SETREADONLY, false,0);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXPASTEV), false);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_EXDEFUNDEF), "DEFINE EXHAUST");
		for (UINT i = 0; i < 8; i++) {
			SetDlgItemText(hWnd, Edits[i], "");
		}
	}

	return;
}
BOOL DialogControl::ExhaustDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_EXNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_EXNAME, cbuf, 256);
			string newname(cbuf);
			ExMng->SetExName(idx, newname);

			char nbuf[256] = { '\0' };
			if (ExMng->IsExDefined(idx)) {
				sprintf(nbuf, "%s", newname.c_str());
			}
			else {
				sprintf(nbuf, "*%s", newname.c_str());
			}
			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			
			tvi.pszText = (LPSTR)nbuf;
			tvi.cchTextMax = strlen(nbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BUTTON_EXPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_EXPOSX, IDC_EDIT_EXPOSY, IDC_EDIT_EXPOSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_EXDIRX, IDC_EDIT_EXDIRY, IDC_EDIT_EXDIRZ, VB1->vclip.dir*(-1));
			}
			break;
		}
		case IDC_BUTTON_EXDELETE:
		{
			ExMng->DelExDef(idx);
			UpdateTree(hDlg, EXHAUSTS, 0);
			break;
		}
		case IDC_BUTTON_EXDEFUNDEF:
		{
			UINT thr_index, extex = 0;
			double lsize, wsize;
			VECTOR3 pos = _V(0, 0, 0);
			VECTOR3 dir = _V(0, 0, 1);
			bool customposdir, customtex;

			if (ExMng->IsExDefined(idx)) {
				ExMng->GetExParams(idx, thr_index, lsize, wsize, pos, dir, extex, customposdir, customtex);
				ExMng->UndefineExDef(idx);
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_EXDEFUNDEF), "DEFINE EXHAUST");
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "*%s", ExMng->GetExName(idx).c_str());
				TVITEM tvi;
				tvi.mask = TVIF_TEXT;

				tvi.pszText = (LPSTR)cbuf;
				tvi.cchTextMax = strlen(cbuf);
				tvi.hItem = CurrentSelection.hitem;
				SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_EXTH), true);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_EXTEXT), true);
				EnableWindow(GetDlgItem(hWnd, IDC_CHECK_EXCUSTOMPOSDIR), true);
				SendDlgItemMessage(hWnd, IDC_EDIT_EXLSIZE, EM_SETREADONLY, false, 0);
				SendDlgItemMessage(hWnd, IDC_EDIT_EXWSIZE, EM_SETREADONLY, false, 0);
				if (customposdir) {
					int edits[6] = { IDC_EDIT_EXPOSX,IDC_EDIT_EXPOSY, IDC_EDIT_EXPOSZ,IDC_EDIT_EXDIRX,IDC_EDIT_EXDIRY,IDC_EDIT_EXDIRZ };
					for (UINT i = 0; i < 6; i++) {
						SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, false, 0);
					}
					EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXPASTEV), true);
				}
			}
			else {
				
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "%s", ExMng->GetExName(idx).c_str());
				TVITEM tvi;
				tvi.mask = TVIF_TEXT;

				tvi.pszText = (LPSTR)cbuf;
				tvi.cchTextMax = strlen(cbuf);
				tvi.hItem = CurrentSelection.hitem;
				SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);

				
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EXTH, CB_GETCURSEL, 0, 0);
				if (index < 0) { break; }
				thr_index = (UINT)index;
				lsize = GetDlgItemDouble(hWnd, IDC_EDIT_EXLSIZE);
				wsize = GetDlgItemDouble(hWnd, IDC_EDIT_EXWSIZE);
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EXTEXT, CB_GETCURSEL, 0, 0);
				if (index > 0) {
					customtex = true;
					extex = (UINT)index-1;
				}
				customposdir = IsCheckBoxChecked(hWnd, IDC_CHECK_EXCUSTOMPOSDIR);
				if (customposdir) {
					pos = GetDlgItemsVector3(hWnd, IDC_EDIT_EXPOSX, IDC_EDIT_EXPOSY, IDC_EDIT_EXPOSZ);
					dir = GetDlgItemsVector3(hWnd, IDC_EDIT_EXDIRX, IDC_EDIT_EXDIRY, IDC_EDIT_EXDIRZ);
					normalise(dir);
				}
				ExMng->DefineExDef(idx, thr_index, lsize, wsize, customposdir, customtex, pos, dir, extex);
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_EXDEFUNDEF), "UNDEFINE EXHAUST");
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_EXTH), false);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_EXTEXT), false);
				EnableWindow(GetDlgItem(hWnd, IDC_CHECK_EXCUSTOMPOSDIR), false);
				SendDlgItemMessage(hWnd, IDC_EDIT_EXLSIZE, EM_SETREADONLY, true, 0);
				SendDlgItemMessage(hWnd, IDC_EDIT_EXWSIZE, EM_SETREADONLY, true, 0);
				if (customposdir) {
					int edits[6] = { IDC_EDIT_EXPOSX,IDC_EDIT_EXPOSY, IDC_EDIT_EXPOSZ,IDC_EDIT_EXDIRX,IDC_EDIT_EXDIRY,IDC_EDIT_EXDIRZ };
					for (UINT i = 0; i < 6; i++) {
						SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, true, 0);
					}
					EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXPASTEV), false);
				}
			}
			
			break;
		}
		case IDC_CHECK_EXCUSTOMPOSDIR:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				int edits[6] = { IDC_EDIT_EXPOSX,IDC_EDIT_EXPOSY, IDC_EDIT_EXPOSZ,IDC_EDIT_EXDIRX,IDC_EDIT_EXDIRY,IDC_EDIT_EXDIRZ };
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_EXCUSTOMPOSDIR);
				if (check) {
					for (UINT i = 0; i < 6; i++) {
						SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, false, 0);
					}
					EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXPASTEV), true);
				}
				else {
					for (UINT i = 0; i < 6; i++) {
						SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, true, 0);
					}
					EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXPASTEV), false);
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