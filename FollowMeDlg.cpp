#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "FollowMeDlg.h"
#pragma comment(lib, "comctl32.lib")

extern HINSTANCE hDLL;

BOOL CALLBACK FollowMeDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	FollowMeDlg *FollowMeDLG = (uMsg == WM_INITDIALOG ? (FollowMeDlg*)lParam : (FollowMeDlg*)oapiGetDialogContext(hWnd));
	return FollowMeDLG->DlgProc(hWnd, uMsg, wParam, lParam);
}

FollowMeDlg::FollowMeDlg(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	open = false;
	hDlg = NULL;
}
FollowMeDlg::~FollowMeDlg() {
	VB1 = NULL;
	hDlg = NULL;
	open = false;
}
void FollowMeDlg::Open(HINSTANCE hDLL) {
	hDlg = oapiOpenDialogEx(hDLL, IDD_DIALOG_FOLLOWME, FollowMeDlgProcHook, DLG_CAPTIONHELP | DLG_CAPTIONCLOSE, this);
	open = true;
}
void FollowMeDlg::Close() {
	open = false;
	oapiCloseDialog(hDlg);
	hDlg = NULL;
}
bool FollowMeDlg::IsOpen() {
	return open;
}

void FollowMeDlg::UpdatePosDirRot() {
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%.5f %.5f %.5f", VB1->follow_me_pos.x, VB1->follow_me_pos.y, VB1->follow_me_pos.z);
	SetDlgItemText(hDlg, IDC_EDIT_FMPOS, (LPCSTR)cbuf);
	sprintf(cbuf, "%.3f %.3f %.3f", VB1->follow_me_dir.x, VB1->follow_me_dir.y, VB1->follow_me_dir.z);
	SetDlgItemText(hDlg, IDC_EDIT_FMDIR, (LPCSTR)cbuf);
	sprintf(cbuf, "%.3f %.3f %.3f", VB1->follow_me_rot.x, VB1->follow_me_rot.y, VB1->follow_me_rot.z);
	SetDlgItemText(hDlg, IDC_EDIT_FMROT, (LPCSTR)cbuf);
	return;
}

BOOL FollowMeDlg::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		InitCommonControls();
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FMPOS), false);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FMDIR), false);
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FMROT), false);
		SendDlgItemMessage(hWnd, IDC_SPIN_FMX, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_FMY, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_FMZ, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_FMPITCH, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_FMROLL, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_FMYAW, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SLIDER_FMSPEED, TBM_SETRANGE, (WPARAM)false, MAKELPARAM(1, 100));
		UpdatePosDirRot();
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case ID_EXIT:
		{
			VB1->DeleteFollowMe();
			break;
		}
		case IDC_FMRESET:
		{
			VB1->ResetFollowMe();
			break;
		}
		case ID_COPYVS:
		{
			VB1->vclip.pos = VB1->follow_me_pos;
			VB1->vclip.dir = VB1->follow_me_dir;
			VB1->vclip.rot = VB1->follow_me_rot;
			VB1->vclip.valid = true;
			break;
		}
		case IDC_CHECK_FOLLWOMESUPERPREC:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				VB1->ToggleFollowMeSuprePrecision();
			}
			break;
		}
		
		}

		break;
	}
	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SPIN_FMX:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				if (lpd->iDelta > 0) {
					VB1->MoveFollowMe(_V(1, 0, 0));
				}
				else if (lpd->iDelta < 0) {
					VB1->MoveFollowMe(_V(-1, 0, 0));
				}

				UpdatePosDirRot();
				SendDlgItemMessage(hWnd, IDC_SPIN_FMX, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_FMY:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				if (lpd->iDelta > 0) {
					VB1->MoveFollowMe(_V(0, 1, 0));
				}
				else if (lpd->iDelta < 0) {
					VB1->MoveFollowMe(_V(0, -1, 0));
				}

				UpdatePosDirRot();
				SendDlgItemMessage(hWnd, IDC_SPIN_FMY, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_FMZ:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				if (lpd->iDelta > 0) {
					VB1->MoveFollowMe(_V(0, 0, 1));
				}
				else if (lpd->iDelta < 0) {
					VB1->MoveFollowMe(_V(0, 0, -1));
				}

				UpdatePosDirRot();
				SendDlgItemMessage(hWnd, IDC_SPIN_FMZ, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_FMPITCH:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				if (lpd->iDelta > 0) {
					VB1->RotateFollowMe(_V(-1, 0, 0));
				}
				else if (lpd->iDelta < 0) {
					VB1->RotateFollowMe(_V(1, 0, 0));
				}

				UpdatePosDirRot();
				SendDlgItemMessage(hWnd, IDC_SPIN_FMPITCH, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_FMYAW:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				if (lpd->iDelta > 0) {
					VB1->RotateFollowMe(_V(0, 1, 0));
				}
				else if (lpd->iDelta < 0) {
					VB1->RotateFollowMe(_V(0, -1, 0));
				}

				UpdatePosDirRot();
				SendDlgItemMessage(hWnd, IDC_SPIN_FMYAW, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_FMROLL:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				if (lpd->iDelta > 0) {
					VB1->RotateFollowMe(_V(0, 0, -1));
				}
				else if (lpd->iDelta < 0) {
					VB1->RotateFollowMe(_V(0, 0, 1));
				}

				UpdatePosDirRot();
				SendDlgItemMessage(hWnd, IDC_SPIN_FMROLL, UDM_SETPOS, 0, 0);
			}
			break;
		}
		//case IDC_SLIDER_FMSPEED:
		//{
		//	LRESULT result = SendDlgItemMessage(hWnd, IDC_SLIDER_FMSPEED, TBM_GETPOS, 0, 0);
		//	sprintf(oapiDebugString(), "%i", result);
		//	break;
		//}






		break;
		}
	}

	case WM_HSCROLL:
	{
		if (lParam == (LPARAM)GetDlgItem(hWnd, IDC_SLIDER_FMSPEED)) {
			LRESULT result = SendDlgItemMessage(hWnd, IDC_SLIDER_FMSPEED, TBM_GETPOS, 0, 0);
			VB1->follow_me_translation_speed = (double)result*50;
			VB1->follow_me_rotation_speed = (double)result*RAD*50;
			break;
		}
	}

	

	}
	
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}