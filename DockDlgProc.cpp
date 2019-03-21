#include "VesselBuilder1.h"
#include "resource.h"
#include "LaserManager.h"
#include "DialogControl.h"
#include "DockManager.h"
#pragma comment(lib, "comctl32.lib")

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)


void DialogControl::UpdateDockDialog(HWND hWnd) {
	if (CurrentSelection.idx >= DckMng->GetDockCount()) { return; }
	
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%s", DckMng->GetDockName(CurrentSelection.idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_DOCKNAME, (LPCSTR)cbuf);
	VECTOR3 pos, dir, rot;
	DckMng->GetDockParams(CurrentSelection.idx, pos, dir, rot);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKPOSX, IDC_EDIT_DOCKPOSY, IDC_EDIT_DOCKPOSZ, pos);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKDIRX, IDC_EDIT_DOCKDIRY, IDC_EDIT_DOCKDIRZ, dir);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKROTX, IDC_EDIT_DOCKROTY, IDC_EDIT_DOCKROTZ, rot);
	bool jett = DckMng->IsDockJettisonable(CurrentSelection.idx);
	if (jett) {
		SendDlgItemMessage(hWnd, IDC_CHECK_DOCKJETT, BM_SETCHECK, BST_CHECKED, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_DOCKJETT, BM_SETCHECK, BST_UNCHECKED, 0);
	}

	if (DockLaserMap[CurrentSelection.idx] != NULL) {
		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_CHECKED, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	//if (SB1->DockBeaconsActive) {
	//	SB1->UpdateDockBeaconsPos();
	//}
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
			DckMng->RotateDock(CurrentSelection.idx, axis, angle);
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
			DckMng->RotateDock(CurrentSelection.idx, axis, angle);
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
			DckMng->RotateDock(CurrentSelection.idx, axis, angle);
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
			VECTOR3 shift = _V(spd*(double)lpd->iDelta, 0, 0);
			DckMng->MoveDock(CurrentSelection.idx, shift);
			UpdateDockDialog(hWnd);
			SendDlgItemMessage(hWnd, IDC_SPIN_DOCKPOSX, UDM_SETPOS, 0, 0);
		}
		break;
	}
	case IDC_SPIN_DOCKPOSY:
	{
		if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
		{
			LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
			double spd = 0.25;
			VECTOR3 shift = _V(0,spd*(double)lpd->iDelta, 0);
			DckMng->MoveDock(CurrentSelection.idx, shift);
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
			VECTOR3 shift = _V(0, 0, spd*(double)lpd->iDelta);
			DckMng->MoveDock(CurrentSelection.idx, shift);
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
		//EnableWindow(GetDlgItem(hWnd, IDC_EDIT_DOCKNAME), false);
		
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
			
			if (DockLaserMap[CurrentSelection.idx] != NULL) {
				VB1->Laser->DeleteLaser(DockLaserMap[CurrentSelection.idx]);
			}
			DckMng->DeleteDockDef(CurrentSelection.idx);
			UpdateTree(hDlg, DOCK,0);
			break;
		}
		case IDC_BTN_DOCKPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKPOSX, IDC_EDIT_DOCKPOSY, IDC_EDIT_DOCKPOSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKDIRX, IDC_EDIT_DOCKDIRY, IDC_EDIT_DOCKDIRZ, VB1->vclip.dir);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_DOCKROTX, IDC_EDIT_DOCKROTY, IDC_EDIT_DOCKROTZ, VB1->vclip.rot);
			}
			break;
		}
		case IDC_BUTTON_DOCKPOS_SET:
		{
		//	if (SB1->DockBeaconsActive) {
		//		SB1->DeleteDockBeacons();
		//		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
		//	}
			
			VECTOR3 pos = GetDlgItemsVector3(hWnd, IDC_EDIT_DOCKPOSX, IDC_EDIT_DOCKPOSY, IDC_EDIT_DOCKPOSZ);
			DckMng->SetDockPos(CurrentSelection.idx, pos);
			UpdateDockDialog(hWnd);
			break;
		}
		case IDC_BUTTON_DOCKDIR_SET:
		{
		//	if (SB1->DockBeaconsActive) {
		//		SB1->DeleteDockBeacons();
		//		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
		//	}
			
			VECTOR3 dir = GetDlgItemsVector3(hWnd, IDC_EDIT_DOCKDIRX, IDC_EDIT_DOCKDIRY, IDC_EDIT_DOCKDIRZ);
			normalise(dir);
			DckMng->SetDockDir(CurrentSelection.idx, dir);
			UpdateDockDialog(hWnd);
			break;
		}
		case IDC_BUTTON_DOCKROT_SET:
		{
		//	if (SB1->DockBeaconsActive) {
		//		SB1->DeleteDockBeacons();
		//		SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
		//	}
			
			VECTOR3 rot = GetDlgItemsVector3(hWnd, IDC_EDIT_DOCKROTX, IDC_EDIT_DOCKROTY, IDC_EDIT_DOCKROTZ);
			normalise(rot);
			DckMng->SetDockRot(CurrentSelection.idx, rot);
			UpdateDockDialog(hWnd);
			break;
		}
		case IDC_CHECK_HIGHLIGHT_DOCK:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_HIGHLIGHT_DOCK, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					DockLaserMap[CurrentSelection.idx] = VB1->Laser->CreateLaserL(DckMng->GetDockPosPtr(CurrentSelection.idx), DckMng->GetDockAntiDirPtr(CurrentSelection.idx), DckMng->GetDockAntiRotPtr(CurrentSelection.idx), LASER_GREEN_TEX, LASER_BLUE_TEX);
				}
				else {
					VB1->Laser->DeleteLaser(DockLaserMap[CurrentSelection.idx]);
				}
			
			}
			break;
		}
		case IDC_CHECK_DOCKJETT:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_DOCKJETT, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					DckMng->SetIsDockJettisonable(CurrentSelection.idx, true);
				}
				else {
					DckMng->SetIsDockJettisonable(CurrentSelection.idx, false);
				}
			}
			break;
		}
		case IDC_BUTTON_DOCKNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_DOCKNAME, cbuf, 256);
			string newname(cbuf);
			DckMng->SetDockName(CurrentSelection.idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
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