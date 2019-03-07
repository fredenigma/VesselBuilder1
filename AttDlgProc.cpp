#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "AttachmentManager.h"
#include "LaserManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateAttDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= AttMng->GetAttCount()) { return; }
	if (AttMng->AttIsCreated(idx)) {
		EnableWindow(GetDlgItem(hWnd, IDC_CHECK_ATTHIGHLIGHT), true);
		EnableWindow(GetDlgItem(hWnd, IDC_CHECK_ATTPARENT), false);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTID, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTPOSX, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTPOSY, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTPOSZ, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTDIRX, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTDIRY, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTDIRZ, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTROTX, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTROTY, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTROTZ, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTRANGE, EM_SETREADONLY, false, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPITCH), true);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTROLL), true);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTYAW), true);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPOSX), true);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPOSY), true);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPOSZ), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTDIR_SET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTROT_SET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTPOS_SET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_ATTPASTEV), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_CRDELATTDEF), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTRANGESET), true);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_CRDELATTDEF), (LPCSTR)TEXT("DELETE THIS ATTACHMENT"));

	}
	else {
		EnableWindow(GetDlgItem(hWnd, IDC_CHECK_ATTHIGHLIGHT), false);
		EnableWindow(GetDlgItem(hWnd, IDC_CHECK_ATTPARENT), true);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTID, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTPOSX, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTPOSY, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTPOSZ, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTDIRX, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTDIRY, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTDIRZ, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTROTX, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTROTY, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTROTZ, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_ATTRANGE, EM_SETREADONLY, true, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPITCH), false);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTROLL), false);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTYAW), false);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPOSX), false);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPOSY), false);
		EnableWindow(GetDlgItem(hWnd, IDC_SPIN_ATTPOSZ), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTDIR_SET), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTROT_SET), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTPOS_SET), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_ATTPASTEV), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_CRDELATTDEF), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTRANGESET), false);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_CRDELATTDEF), (LPCSTR)TEXT("CREATE THE ATTACHMENT"));

	}
	
	SetDlgItemText(hWnd, IDC_EDIT_ATTID, AttMng->GetAttDefId(idx).c_str());
	if (AttMng->AttToParent(idx)) {
		SendDlgItemMessage(hWnd, IDC_CHECK_ATTPARENT, BM_SETCHECK, BST_CHECKED, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_ATTPARENT, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	VECTOR3 pos, dir, rot;
	AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_ATTPOSX, IDC_EDIT_ATTPOSY, IDC_EDIT_ATTPOSZ, pos);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_ATTDIRX, IDC_EDIT_ATTDIRY, IDC_EDIT_ATTDIRZ, dir);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_ATTROTX, IDC_EDIT_ATTROTY, IDC_EDIT_ATTROTZ, rot);

	if (AttLaserMap[idx]!=NULL) {
		SendDlgItemMessage(hWnd, IDC_CHECK_ATTHIGHLIGHT, BM_SETCHECK, BST_CHECKED, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_ATTHIGHLIGHT, BM_SETCHECK, BST_UNCHECKED, 0);
	}

	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%.2f", AttMng->GetAttDefRange(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ATTRANGE, cbuf);

	return;
}

BOOL DialogControl::AttDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		
		SendDlgItemMessage(hWnd, IDC_SPIN_ATTPITCH, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_ATTROLL, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_ATTYAW, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_ATTPOSX, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_ATTPOSY, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_ATTPOSZ, UDM_SETRANGE32, -10000, 10000);
		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_CRDELATTDEF:
		{
			if (AttMng->AttIsCreated(idx)) {
				if (AttLaserMap[idx] != NULL) {
					VB1->Laser->DeleteLaser(AttLaserMap[idx]);
				}
				AttMng->DeleteAttDef(idx);
				UpdateTree(hDlg, ATTACHMENT,0);
			}
			else {
				char cbuf[256] = { '\0' };
				GetDlgItemText(hWnd, IDC_EDIT_ATTID, cbuf, 256);
				string newid(cbuf);
				AttMng->SetAttDefID(idx, newid);
				bool toparent = false;
				LRESULT check = SendDlgItemMessage(hWnd, IDC_CHECK_ATTPARENT, BM_GETCHECK, 0, 0);
				if (check == BST_CHECKED) {
					toparent = true;
				}
				AttMng->SetAttDefToParent(idx, toparent);
				AttMng->SetAttDefCreated(idx);
				TVITEM tvi;
				tvi.mask = TVIF_TEXT;
				sprintf(cbuf, "Attachment_%i", idx);
				tvi.pszText = (LPSTR)cbuf;
				tvi.cchTextMax = 13;
				tvi.hItem = CurrentSelection.hitem;
				SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
				UpdateAttDialog(hWnd);
			}
			break;
		}
		case IDC_BTN_ATTPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ATTPOSX, IDC_EDIT_ATTPOSY, IDC_EDIT_ATTPOSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ATTDIRX, IDC_EDIT_ATTDIRY, IDC_EDIT_ATTDIRZ, VB1->vclip.dir);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_ATTROTX, IDC_EDIT_ATTROTY, IDC_EDIT_ATTROTZ, VB1->vclip.rot);
			}
			break;
		}
		case IDC_BUTTON_ATTPOS_SET:
		{
			VECTOR3 pos, dir, rot;
			AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
			VECTOR3 newpos = GetDlgItemsVector3(hWnd, IDC_EDIT_ATTPOSX, IDC_EDIT_ATTPOSY, IDC_EDIT_ATTPOSZ);
			AttMng->ModifyAttDef(idx, newpos, dir, rot);
			UpdateAttDialog(hWnd);
			break;
		}
		case IDC_BUTTON_ATTDIR_SET:
		{
			VECTOR3 pos, dir, rot;
			AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
			VECTOR3 newdir = GetDlgItemsVector3(hWnd, IDC_EDIT_ATTDIRX, IDC_EDIT_ATTDIRY, IDC_EDIT_ATTDIRZ);
			normalise(newdir);
			AttMng->ModifyAttDef(idx, pos, newdir, rot);
			UpdateAttDialog(hWnd);
			break;
		}
		case IDC_BUTTON_ATTROT_SET:
		{
			VECTOR3 pos, dir, rot;
			AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
			VECTOR3 newrot = GetDlgItemsVector3(hWnd, IDC_EDIT_ATTROTX, IDC_EDIT_ATTROTY, IDC_EDIT_ATTROTZ);
			normalise(newrot);
			AttMng->ModifyAttDef(idx, pos, dir, newrot);
			UpdateAttDialog(hWnd);
			break;
		}
		case IDC_CHECK_ATTHIGHLIGHT:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_ATTHIGHLIGHT, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					AttLaserMap[idx] = VB1->Laser->CreateLaserL(AttMng->GetAttDefPosPtr(idx), AttMng->GetAttDefAntiDirPtr(idx), AttMng->GetAttDefAntiRotPtr(idx), LASER_GREEN_TEX, LASER_BLUE_TEX);
				}
				else {
					VB1->Laser->DeleteLaser(AttLaserMap[idx]);
				}
			}
			break;
		}
		case IDC_BUTTON_ATTRANGESET:
		{
			double newrange = GetDlgItemDouble(hWnd, IDC_EDIT_ATTRANGE);
			AttMng->SetAttDefRange(idx, newrange);
			break;
		}

		}
		break;
	}
	case WM_NOTIFY:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_SPIN_ATTPITCH:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 1 * RAD;
				VECTOR3 axis = _V(1, 0, 0);
				double angle = spd*(double)lpd->iDelta;
				VECTOR3 pos, dir, rot;
				AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
				MATRIX3 oldrm = VB1->FindRM(dir, rot);
				MATRIX3 rotrm = rotm(axis, angle);
				MATRIX3 newrm = mul(rotrm, oldrm);
				VB1->FindDirRot(newrm, dir, rot);
				AttMng->ModifyAttDef(idx, pos, dir, rot);
				UpdateAttDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_ATTPITCH, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_ATTROLL:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 1 * RAD;
				VECTOR3 axis = _V(0, 0, 1);
				double angle = spd*(double)lpd->iDelta;
				VECTOR3 pos, dir, rot;
				AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
				MATRIX3 oldrm = VB1->FindRM(dir, rot);
				MATRIX3 rotrm = rotm(axis, angle);
				MATRIX3 newrm = mul(rotrm, oldrm);
				VB1->FindDirRot(newrm, dir, rot);
				AttMng->ModifyAttDef(idx, pos, dir, rot);
				UpdateAttDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_ATTPITCH, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_ATTYAW:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 1 * RAD;
				VECTOR3 axis = _V(0, 1, 0);
				double angle = spd*(double)lpd->iDelta;
				VECTOR3 pos, dir, rot;
				AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
				MATRIX3 oldrm = VB1->FindRM(dir, rot);
				MATRIX3 rotrm = rotm(axis, angle);
				MATRIX3 newrm = mul(rotrm, oldrm);
				VB1->FindDirRot(newrm, dir, rot);
				AttMng->ModifyAttDef(idx, pos, dir, rot);
				UpdateAttDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_ATTPITCH, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_ATTPOSX:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.25;
				VECTOR3 pos, dir, rot;
				AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
				pos.x += spd*(double)lpd->iDelta;
				AttMng->ModifyAttDef(idx, pos, dir, rot);
				UpdateAttDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_ATTPOSX, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_ATTPOSY:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.25;
				VECTOR3 pos, dir, rot;
				AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
				pos.y += spd*(double)lpd->iDelta;
				AttMng->ModifyAttDef(idx, pos, dir, rot);
				UpdateAttDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_ATTPOSY, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_ATTPOSZ:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.25;
				VECTOR3 pos, dir, rot;
				AttMng->GetAttDefPosDirRot(idx, pos, dir, rot);
				pos.z += spd*(double)lpd->iDelta;
				AttMng->ModifyAttDef(idx, pos, dir, rot);
				UpdateAttDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_ATTPOSZ, UDM_SETPOS, 0, 0);
			}
			break;
		}




		}
		break;
	}

	////
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}