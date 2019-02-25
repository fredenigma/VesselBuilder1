#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateExTexDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= VB1->GetExTexCount()) { return; }
	if (VB1->IsExTexCreated(idx)) {
		SetDlgItemText(hWnd, IDC_EDIT_EXTEXNAME, VB1->GetExTexName(idx).c_str());
		SendDlgItemMessage(hWnd, IDC_EDIT_EXTEXNAME, EM_SETREADONLY, true, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXTEXLOAD), false);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_DELETEEXTEX), "DELETE THIS EXHAUST TEXTURE");
	}
	else {
		SetDlgItemText(hWnd, IDC_EDIT_EXTEXNAME, VB1->GetExTexName(idx).c_str());
		SendDlgItemMessage(hWnd, IDC_EDIT_EXTEXNAME, EM_SETREADONLY, false, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_EXTEXLOAD), true);
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_DELETEEXTEX), "SAVE THIS EXHAUST TEXTURE");
	}
	return;
}

BOOL DialogControl::ExTexDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_DELETEEXTEX:
		{
			if (VB1->IsExTexCreated(idx)) {
				VB1->DelExTedDef(idx);
				UpdateTree(hDlg, EXTEX, 0);
			}
			else {
				char cbuf[256] = { '\0' };
				GetDlgItemText(hWnd, IDC_EDIT_EXTEXNAME, cbuf, 256);
				string name(cbuf);
				VB1->StoreExTexDef(name, idx);
				UpdateTree(hDlg, EXTEX, 0);
			}
			break;
		}
		case IDC_BUTTON_EXTEXLOAD:
		{
			//string texdir = SB1->OrbiterRoot;
			string texdir = VB1->texturedir;
			
			char abstexdir_c[MAX_PATH];
			char currentdir1[MAX_PATH];
			GetCurrentDirectory(sizeof(currentdir1) / sizeof(char), currentdir1);
			SetCurrentDirectory(texdir.c_str());
			GetCurrentDirectory(sizeof(abstexdir_c) / sizeof(char), abstexdir_c);
			string abstexdir(abstexdir_c);
			SetCurrentDirectory(currentdir1);

			//sprintf(oapiDebugString(), "AbsTexDir:%s", abstexdir.c_str());
			OPENFILENAME ofn;       // common dialog box structure
			char szFile[260] = { '\0' };       // buffer for file name
											   //HWND hwnd;              // owner window
											   //			HANDLE hf;              // file handle

											   // Initialize OPENFILENAME
			char fTitle[MAX_PATH] = { '\0' };
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Texture Files\0*.dds\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = fTitle;
			ofn.nMaxFileTitle = sizeof(fTitle);
			ofn.lpstrInitialDir = texdir.c_str();// meshdir.c_str();
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			char currentdir[MAX_PATH];

			GetCurrentDirectory(sizeof(currentdir) / sizeof(char), currentdir);

			if (GetOpenFileName(&ofn) == TRUE)
			{

				SetCurrentDirectory(currentdir);

				string result(szFile);
				string filename = result.substr(abstexdir.size()+1, ofn.nFileExtension-abstexdir.size()-2);
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "%s", filename.c_str());
				SetWindowText(GetDlgItem(hWnd, IDC_EDIT_EXTEXNAME), cbuf);
			}
			break;
		}

		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}