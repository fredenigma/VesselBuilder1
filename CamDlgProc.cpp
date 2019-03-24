#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "CameraManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateCamDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= CamMng->GetCamCount()) { return; }
	string name = CamMng->GetCamName(idx);
	char namebuf[256] = { '\0' };
	sprintf(namebuf, "%s", name.c_str());
	SetDlgItemText(hWnd, IDC_EDIT_CAMNAME,namebuf);
	VECTOR3 pos = CamMng->GetCamPos(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMPOSX, IDC_EDIT_CAMPOSY, IDC_EDIT_CAMPOSZ, pos);
	VECTOR3 dir = CamMng->GetCamDir(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, dir);
	double tilt = CamMng->GetCamTilt(idx);
	tilt *= DEG;
	SetDlgItemDouble(hWnd, IDC_EDIT_CAMTILT, tilt, 1);

	return;
}

BOOL DialogControl::CamDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_CAMPOSSET:
		{
			VECTOR3 pos = GetDlgItemsVector3(hWnd, IDC_EDIT_CAMPOSX, IDC_EDIT_CAMPOSY, IDC_EDIT_CAMPOSZ);
			CamMng->SetCamPos(idx, pos);
			break;
		}
		case IDC_BUTTON_CAMDIRSET:
		{
			VECTOR3 dir = GetDlgItemsVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ);
			normalise(dir);
			CamMng->SetCamDir(idx, dir);
			break;
		}
		case IDC_BUTTON_CAMTILTSET:
		{
			double tilt = GetDlgItemDouble(hWnd, IDC_EDIT_CAMTILT);
			CamMng->SetCamTilt(idx, tilt);
			break;
		}
		case IDC_BUTTON_CAMNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_CAMNAME, cbuf, 256);
			string name(cbuf);
			CamMng->SetCamName(idx, name);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BTN_CAMPASTEV:
		{
			if (!VB1->vclip.valid) {break;}
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMPOSX, IDC_EDIT_CAMPOSY, IDC_EDIT_CAMPOSZ, VB1->vclip.pos);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, VB1->vclip.dir);
			
			break;
		}
		case IDC_BUTTON_CAMXP:
		{
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, _V(1,0,0));
			break;
		}
		case IDC_BUTTON_CAMXM:
		{
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, _V(-1, 0, 0));
			break;
		}
		case IDC_BUTTON_CAMYP:
		{
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, _V(0, 1, 0));
			break;
		}
		case IDC_BUTTON_CAMYM:
		{
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, _V(0, -1, 0));
			break;
		}
		case IDC_BUTTON_CAMZP:
		{
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, _V(0, 0, 1));
			break;
		}
		case IDC_BUTTON_CAMZM:
		{
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CAMDIRX, IDC_EDIT_CAMDIRY, IDC_EDIT_CAMDIRZ, _V(0, 0, -1));
			break;
		}
		case IDC_BUTTON_CAMTEST:
		{
			CamMng->SetCurrentCamera(idx);
			break;
		}
		case IDC_BUTTON_DELETECAM:
		{
			CamMng->RemoveCamDef(idx);
			UpdateTree(hDlg, CAMERA, CurrentSelection.config);
			break;
		}

		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}