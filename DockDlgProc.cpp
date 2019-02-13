#include "StationBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateDockDialog(HWND hWnd) {
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "DOCK %i", CurrentSelection.idx);
	SetDlgItemText(hWnd, IDC_EDIT_DOCKNAME, (LPCSTR)cbuf);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKPOSX, IDC_EDIT_DOCKPOSY, IDC_EDIT_DOCKPOSZ, SB1->dock_definitions[CurrentSelection.idx].pos);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKDIRX, IDC_EDIT_DOCKDIRY, IDC_EDIT_DOCKDIRZ, SB1->dock_definitions[CurrentSelection.idx].dir);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKROTX, IDC_EDIT_DOCKROTY, IDC_EDIT_DOCKROTZ, SB1->dock_definitions[CurrentSelection.idx].rot);
	if (SB1->DockBeaconsActive) {
		SB1->UpdateDockBeaconsPos();
	}
	return;
}

void DialogControl::DockNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	
	switch (LOWORD(wParam))
	{
	case IDC_SPIN_DOCKROLL:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double spd = 1 * RAD;
			VECTOR3 axis = _V(0, 0, 1);
			double angle = spd*(double)lpd->iDelta;
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			MATRIX3 oldrm = SB1->FindRM(dir, rot);
			MATRIX3 rotrm = rotm(axis, angle);
			MATRIX3 newrm = mul(rotrm,oldrm);
			SB1->FindDirRot(newrm, dir, rot);
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh,pos, dir, rot);
			SB1->dock_definitions[CurrentSelection.idx].dir = dir;
			SB1->dock_definitions[CurrentSelection.idx].rot = rot;
			UpdateDockDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_DOCKROLL, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_DOCKYAW:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double spd = 1 * RAD;
			VECTOR3 axis = _V(0, -1, 0);
			double angle = spd*(double)lpd->iDelta;
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			MATRIX3 oldrm = SB1->FindRM(dir, rot);
			MATRIX3 rotrm = rotm(axis, angle);
			MATRIX3 newrm = mul(rotrm, oldrm);
			SB1->FindDirRot(newrm, dir, rot);
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			SB1->dock_definitions[CurrentSelection.idx].dir = dir;
			SB1->dock_definitions[CurrentSelection.idx].rot = rot;
			UpdateDockDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_DOCKYAW, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_DOCKPITCH:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double spd = 1 * RAD;
			VECTOR3 axis = _V(1, 0, 0);
			double angle = spd*(double)lpd->iDelta;
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			MATRIX3 oldrm = SB1->FindRM(dir, rot);
			MATRIX3 rotrm = rotm(axis, angle);
			MATRIX3 newrm = mul(rotrm, oldrm);
			SB1->FindDirRot(newrm, dir, rot);
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			SB1->dock_definitions[CurrentSelection.idx].dir = dir;
			SB1->dock_definitions[CurrentSelection.idx].rot = rot;
			UpdateDockDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPITCH, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_DOCKPOSX:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double spd = 0.25;
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			pos.x += spd*(double)lpd->iDelta;
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			SB1->dock_definitions[CurrentSelection.idx].pos = pos;
			
			UpdateDockDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPITCH, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_DOCKPOSY:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double spd = 0.25;
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			pos.y += spd*(double)lpd->iDelta;
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			SB1->dock_definitions[CurrentSelection.idx].pos = pos;

			UpdateDockDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPOSY, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_DOCKPOSZ:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double spd = 0.25;
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			pos.z += spd*(double)lpd->iDelta;
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			SB1->dock_definitions[CurrentSelection.idx].pos = pos;

			UpdateDockDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPOSZ, UDM_SETPOS, 0, 0);
		}
		break;
	}





	}

	return;
}

BOOL DialogControl::DockDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_DOCKNAME), false);
		if (SB1->DockBeaconsActive) {
			SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_CHECKED, 0);
		}
		SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPITCH, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_DOCKROLL, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_DOCKYAW, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPOSX, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPOSY, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPOSZ, UDM_SETRANGE32, -10000, 10000);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_DELETEDOCKDEF:
		{
			SB1->DeleteDockDef(CurrentSelection.idx);
			if (SB1->DockBeaconsActive) { SB1->DeleteDockBeacons();
			SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
			}
			UpdateTree(hDlg, DOCK,hrootDocks);
			break;
		}
		case IDC_BTN_DOCKPASTEV:
		{
			if (SB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKPOSX, IDC_EDIT_DOCKPOSY, IDC_EDIT_DOCKPOSZ, SB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKDIRX, IDC_EDIT_DOCKDIRY, IDC_EDIT_DOCKDIRZ, SB1->vclip.dir);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKROTX, IDC_EDIT_DOCKROTY, IDC_EDIT_DOCKROTZ, SB1->vclip.rot);

			}
			break;
		}
		case IDC_BUTTON_DOCKPOS_SET:
		{
		//	if (SB1->DockBeaconsActive) {
		//		SB1->DeleteDockBeacons();
		//		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
		//	}
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			pos = GetDlgItemsVector3(hWnd, IDC_EDIT_DOCKPOSX, IDC_EDIT_DOCKPOSY, IDC_EDIT_DOCKPOSZ);
			SB1->dock_definitions[CurrentSelection.idx].pos = pos;
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			
			UpdateDockDialog(hWnd);
			break;
		}
		case IDC_BUTTON_DOCKDIR_SET:
		{
		//	if (SB1->DockBeaconsActive) {
		//		SB1->DeleteDockBeacons();
		//		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
		//	}
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			dir = GetDlgItemsVector3(hWnd, IDC_EDIT_DOCKDIRX, IDC_EDIT_DOCKDIRY, IDC_EDIT_DOCKDIRZ);
			normalise(dir);
			SB1->dock_definitions[CurrentSelection.idx].dir = dir;
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			UpdateDockDialog(hWnd);
			break;
		}
		case IDC_BUTTON_DOCKROT_SET:
		{
		//	if (SB1->DockBeaconsActive) {
		//		SB1->DeleteDockBeacons();
		//		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
		//	}
			VECTOR3 pos, dir, rot;
			SB1->GetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			rot = GetDlgItemsVector3(hWnd, IDC_EDIT_DOCKROTX, IDC_EDIT_DOCKROTY, IDC_EDIT_DOCKROTZ);
			normalise(rot);
			SB1->dock_definitions[CurrentSelection.idx].rot = rot;
			SB1->SetDockParams(SB1->dock_definitions[CurrentSelection.idx].dh, pos, dir, rot);
			UpdateDockDialog(hWnd);
			break;
		}
		case IDC_CHECK_HIGHLIGHT_DOCK:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					SB1->CreateDockBeacons();
				}
				else {
					SB1->DeleteDockBeacons();
				}
			}
			break;
		}
		}
		break;
	}
	case WM_NOTIFY:
	{
		DockNotify(hWnd, wParam, lParam);
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}