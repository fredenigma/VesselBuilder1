#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "LightsManager.h"
#include "AttachmentManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateAttachmentListBox(HWND hWnd, int id, UINT CurrAtt) {
	SendDlgItemMessage(hWnd, id, LB_RESETCONTENT, 0, 0);
	int selindex = -1;
	for (UINT i = 0; i < AttMng->GetAttCount(); i++) {
		if (!AttMng->AttIsCreated(i)) { continue; }
		char cbuf[256] = { '\0' };
		if (AttMng->GetAttDefId(i).size() <= 0) {
			sprintf(cbuf, "Attachment_%i", i);
		}
		else {
			sprintf(cbuf, AttMng->GetAttDefId(i).c_str());
		}
		int index = SendDlgItemMessage(hWnd, id, LB_ADDSTRING, 0, (LPARAM)cbuf);
		if (i == CurrAtt) {
			selindex = index;
		}
		SendDlgItemMessage(hWnd, id, LB_SETITEMDATA, index, (LPARAM)i);
	}
	if (selindex != -1) {
		SendDlgItemMessage(hWnd, id, LB_SETCURSEL, selindex, 0);
	}
	return;
}
void DialogControl::UpdateLightsDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= LightsMng->GetLightsCount()) { return; }
	char namebuf[256] = { '\0' };
	sprintf(namebuf, "%s", LightsMng->GetLightName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_LGTNAME, namebuf);
	LightEmitter::TYPE type = LightsMng->GetLightType(idx);
	int SpotWindows[14] = { IDC_EDIT_LGTDIRX,IDC_EDIT_LGTDIRY, IDC_EDIT_LGTDIRZ,IDC_EDIT_LGTUMB,IDC_EDIT_LGTPEN,IDC_BUTTON_LGTAPSET,IDC_BUTTON_LGTDIRSET,IDC_STATIC_LGTAPGRP,IDC_STATIC_LGTDIRGRP,IDC_STATIC_LGTDIRX,IDC_STATIC_LGTDIRY,IDC_STATIC_LGTDIRZ,IDC_STATIC_LGTUMB,IDC_STATIC_LGTPEN };
	if (type == LightEmitter::TYPE::LT_POINT) {
		SetDlgItemText(hWnd, IDC_EDIT_LGTTYPE, "Point Light");
		for (UINT i = 0; i < 14; i++) {
			ShowWindow(GetDlgItem(hWnd, SpotWindows[i]), SW_HIDE);
		}
		

	}
	else if (type == LightEmitter::TYPE::LT_SPOT) {
		SetDlgItemText(hWnd, IDC_EDIT_LGTTYPE, "Spot Light");
		for (UINT i = 0; i < 14; i++) {
			ShowWindow(GetDlgItem(hWnd, SpotWindows[i]), SW_SHOW);
		}
		VECTOR3 dir = LightsMng->GetLightDirection(idx);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_LGTDIRX, IDC_EDIT_LGTDIRY, IDC_EDIT_LGTDIRZ, dir);
		double umbra, penumbra;
		LightsMng->GetLightAperture(idx, umbra, penumbra);
		SetDlgItemDouble(hWnd, IDC_EDIT_LGTUMB, umbra*DEG, 1);
		SetDlgItemDouble(hWnd, IDC_EDIT_LGTPEN, penumbra*DEG, 1);
	}
	LightEmitter::VISIBILITY vis = LightsMng->GetLightVisibility(idx);
	if (vis == LightEmitter::VISIBILITY::VIS_EXTERNAL) {
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_SETCURSEL, 0, 0);
	}
	else if (vis == LightEmitter::VISIBILITY::VIS_COCKPIT) {
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_SETCURSEL, 1, 0);
	}
	else if (vis == LightEmitter::VISIBILITY::VIS_ALWAYS) {
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_SETCURSEL, 2, 0);
	}
	VECTOR3 pos = LightsMng->GetLightPosition(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_LGTPOSX, IDC_EDIT_LGTPOSY, IDC_EDIT_LGTPOSZ, pos);
	VECTOR3 att = LightsMng->GetLightAttenuation(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_LGTATT0, IDC_EDIT_LGTATT1, IDC_EDIT_LGTATT2, att,4);
	double range = LightsMng->GetLightRange(idx);
	SetDlgItemDouble(hWnd, IDC_EDIT_LGTRANGE, range, 1);
	
	

	int posdir_edits[6] = { IDC_EDIT_LGTDIRX,IDC_EDIT_LGTDIRY, IDC_EDIT_LGTDIRZ,IDC_EDIT_LGTPOSX,IDC_EDIT_LGTPOSY,IDC_EDIT_LGTPOSZ };
	if (LightsMng->IsLightAttachedToAttachment(idx)) {
		SendDlgItemMessage(hWnd, IDC_CHECK_LGTUSEATT, BM_SETCHECK, BST_CHECKED, 0);
		for (UINT i = 0; i < 6; i++) {
			SendDlgItemMessage(hWnd, posdir_edits[i], EM_SETREADONLY, true, 0);
			SetDlgItemText(hWnd, posdir_edits[i], "");
		}
		EnableWindow(GetDlgItem(hWnd, IDC_LIST_LGTATTLIST), true);
		UpdateAttachmentListBox(hWnd, IDC_LIST_LGTATTLIST, LightsMng->GetLigthAttachment(idx));
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_LGTATTACHSET), true);
		
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_LGTUSEATT, BM_SETCHECK, BST_UNCHECKED, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_LIST_LGTATTLIST), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_LGTATTACHSET), false);
		SendDlgItemMessage(hWnd, IDC_LIST_LGTATTLIST, LB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < 6; i++) {
			SendDlgItemMessage(hWnd, posdir_edits[i], EM_SETREADONLY, false, 0);
		}
	}
	bool active = LightsMng->IsLightActive(idx);
	if (active) {
		SetDlgItemText(hWnd, IDC_EDIT_LGTNOW, "ON");
	}
	else {
		SetDlgItemText(hWnd, IDC_EDIT_LGTNOW, "OFF");
	}

	InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_LGTAMB), NULL, FALSE);
	UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_LGTAMB));
	COLOUR4 col_a = LightsMng->GetLightAmbientColour(idx);
	VECTOR3 col_av;
	col_av.x = col_a.r;
	col_av.y = col_a.g;
	col_av.z = col_a.b;
	UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_LGTAMB), col_av);

	InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_LGTDIFF), NULL, FALSE);
	UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_LGTDIFF));
	COLOUR4 col_d = LightsMng->GetLightDiffuseColour(idx);
	VECTOR3 col_dv;
	col_dv.x = col_d.r;
	col_dv.y = col_d.g;
	col_dv.z = col_d.b;
	UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_LGTDIFF), col_dv);

	InvalidateRect(GetDlgItem(hWnd, IDC_STATIC_LGTSPEC), NULL, FALSE);
	UpdateWindow(GetDlgItem(hWnd, IDC_STATIC_LGTSPEC));
	COLOUR4 col_s = LightsMng->GetLightSpecularColour(idx);
	VECTOR3 col_sv;
	col_sv.x = col_s.r;
	col_sv.y = col_s.g;
	col_sv.z = col_s.b;
	UpdateColorExamples(GetDlgItem(hWnd, IDC_STATIC_LGTSPEC), col_sv);
	return;
}
BOOL DialogControl::LightsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_RESETCONTENT, 0, 0);
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_INSERTSTRING, 0, (LPARAM)"EXTERNAL");
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_SETITEMDATA, 0, (LPARAM)LightEmitter::VISIBILITY::VIS_EXTERNAL);
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_INSERTSTRING, 1, (LPARAM)"COCKPIT");
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_SETITEMDATA, 1, (LPARAM)LightEmitter::VISIBILITY::VIS_COCKPIT);
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_INSERTSTRING, 2, (LPARAM)"ALWAYS");
		SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_SETITEMDATA, 2, (LPARAM)LightEmitter::VISIBILITY::VIS_ALWAYS);

		SendDlgItemMessage(hWnd, IDC_EDIT_LGTTYPE, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_LGTNOW, EM_SETREADONLY, true, 0);

		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_LGTNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_LGTNAME, cbuf, 256);
			string newname(cbuf);
			LightsMng->SetLightName(idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_COMBO_LGTVIS:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_LGTVIS, CB_GETCURSEL, 0, 0);
				if (index == 0) {
					LightsMng->SetLightVisibility(idx, LightEmitter::VISIBILITY::VIS_EXTERNAL);
				}
				else if (index == 1) {
					LightsMng->SetLightVisibility(idx, LightEmitter::VISIBILITY::VIS_COCKPIT);
				}
				else if (index == 2) {
					LightsMng->SetLightVisibility(idx, LightEmitter::VISIBILITY::VIS_ALWAYS);
				}
			
			}
			break;
		}
		case IDC_BUTTON_LGTPOSSET:
		{
			VECTOR3 pos = GetDlgItemsVector3(hWnd, IDC_EDIT_LGTPOSX, IDC_EDIT_LGTPOSY, IDC_EDIT_LGTPOSZ);
			LightsMng->SetLightPosition(idx, pos);
			break;
		}
		case IDC_BUTTON_LGTATTSET:
		{
			VECTOR3 att = GetDlgItemsVector3(hWnd, IDC_EDIT_LGTATT0, IDC_EDIT_LGTATT1, IDC_EDIT_LGTATT2);
			LightsMng->SetLightAttenuation(idx, att);
			break;
		}
		case IDC_BUTTON_LGTRANGESET:
		{
			double range = GetDlgItemDouble(hWnd, IDC_EDIT_LGTRANGE);
			LightsMng->SetLightRange(idx, range);
			break;
		}
		case IDC_BUTTON_LGTDIRSET:
		{
			VECTOR3 dir = GetDlgItemsVector3(hWnd, IDC_EDIT_LGTDIRX, IDC_EDIT_LGTDIRY, IDC_EDIT_LGTDIRZ);
			LightsMng->SetLightDirection(idx, dir);
			break;
		}
		case IDC_BUTTON_LGTAPSET:
		{
			double umbra, penumbra;
			umbra = GetDlgItemDouble(hWnd, IDC_EDIT_LGTUMB);
			penumbra = GetDlgItemDouble(hWnd, IDC_EDIT_LGTPEN);
			umbra *= RAD;
			penumbra *= RAD;
			LightsMng->SetLightAperture(idx, umbra, penumbra);
			break;
		}
		case IDC_BUTTON_LGTTOGGLE:
		{
			LightsMng->ToggleLight(idx);
			UpdateLightsDialog(hWnd);
			break;
		}
		case IDC_BUTTON_LGTDEL:
		{
			LightsMng->DeleteLight(idx);
			UpdateTree(hDlg, LIGHTS, CurrentSelection.config);
			break;
		}
		case IDC_BUTTON_LGTPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_LGTPOSX, IDC_EDIT_LGTPOSY, IDC_EDIT_LGTPOSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_LGTDIRX, IDC_EDIT_LGTDIRY, IDC_EDIT_LGTDIRZ, VB1->vclip.dir);
			}
			break;
		}
		case IDC_CHECK_LGTUSEATT:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_LGTUSEATT);
				int posdir_edits[6] = { IDC_EDIT_LGTDIRX,IDC_EDIT_LGTDIRY, IDC_EDIT_LGTDIRZ,IDC_EDIT_LGTPOSX,IDC_EDIT_LGTPOSY,IDC_EDIT_LGTPOSZ };
				
				if (check) {
					EnableWindow(GetDlgItem(hWnd, IDC_LIST_LGTATTLIST), true);
					EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_LGTATTACHSET), true);
					UpdateAttachmentListBox(hWnd, IDC_LIST_LGTATTLIST, LightsMng->GetLigthAttachment(idx));
					for (UINT i = 0; i < 6; i++) {
						SendDlgItemMessage(hWnd, posdir_edits[i], EM_SETREADONLY, true, 0);
						SetDlgItemText(hWnd, posdir_edits[i], "");
					}
				}
				else {
					EnableWindow(GetDlgItem(hWnd, IDC_LIST_LGTATTLIST), false);
					EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_LGTATTACHSET), false);
					LightsMng->SetLightToAttachment(idx, false, (UINT)-1);
					for (UINT i = 0; i < 6; i++) {
						SendDlgItemMessage(hWnd, posdir_edits[i], EM_SETREADONLY, false, 0);
					}
					UpdateLightsDialog(hWnd);
				}
			}
			break;
		}
		case IDC_BUTTON_LGTATTACHSET:
		{
			int index = SendDlgItemMessage(hWnd, IDC_LIST_LGTATTLIST, LB_GETCURSEL, 0, 0);
			UINT att = SendDlgItemMessage(hWnd, IDC_LIST_LGTATTLIST, LB_GETITEMDATA, index, 0);
			LightsMng->SetLightToAttachment(idx, true, att);
			break;
		}


		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}

