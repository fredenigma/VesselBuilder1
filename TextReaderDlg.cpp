#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "TextReaderDlg.h"
#pragma comment(lib, "comctl32.lib")

extern HINSTANCE hDLL;

BOOL CALLBACK TextReaderDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	TextReaderDlg *textreader = (uMsg == WM_INITDIALOG ? (TextReaderDlg*)lParam : (TextReaderDlg*)oapiGetDialogContext(hWnd));
	return textreader->DlgProc(hWnd, uMsg, wParam, lParam);
}

TextReaderDlg::TextReaderDlg(){
	open = false;
	hDlg = NULL;
}
string TextReaderDlg::TextFileContent(string filename) {
	string line;
	string tempbuffer;
	tempbuffer.clear();
	ifstream file(filename.c_str());
	if (file.is_open()) {
		while (getline(file, line)) {
			tempbuffer += line;
			tempbuffer += "\r\n";
		}
		file.close();
	}
	return tempbuffer;
}
TextReaderDlg::~TextReaderDlg() {}
BOOL CALLBACK TextReaderDlg::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_EDIT_TEXT, EM_SETREADONLY, true, 0);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case ID_TEXT_EXIT:
		{
			Close();
			break;
		}
		case IDC_BUTTON_TEXTOPENORBLOG:
		{
			SetDlgItemText(hWnd, IDC_EDIT_TEXT, (LPCSTR)TextFileContent("Orbiter.log").c_str());
			break;
		}
		case IDC_BUTTON_TEXTOPENVBLOG:
		{
			SetDlgItemText(hWnd, IDC_EDIT_TEXT, (LPCSTR)TextFileContent("VesselBuilder1.log").c_str());
			break;
		}
		case IDC_BUTTON_TEXTOPENFILE:
		{
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
			ofn.lpstrFilter = "All files\0*.*\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = fTitle;
			ofn.nMaxFileTitle = sizeof(fTitle);
			//ofn.lpstrInitialDir = GetCurrent
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			char currentdir[MAX_PATH];
			GetCurrentDirectory(sizeof(currentdir) / sizeof(char), currentdir);

			if (GetOpenFileName(&ofn) == TRUE)
			{
				SetCurrentDirectory(currentdir);
				string filename(szFile);
				SetDlgItemText(hWnd, IDC_EDIT_TEXT, (LPCSTR)TextFileContent(filename.c_str()).c_str());
			}
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
	{
		Close();
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}
void TextReaderDlg::Open(HINSTANCE hDLL) {
	hDlg = oapiOpenDialogEx(hDLL, IDD_DIALOG_TEXTREADER, TextReaderDlgProcHook, DLG_CAPTIONHELP | DLG_CAPTIONCLOSE, this);
	open = true;
	return;
}
void TextReaderDlg::Close() {
	open = false;
	oapiCloseDialog(hDlg);
	hDlg = NULL;
	return;
}
bool TextReaderDlg::IsOpen() {
	return open;
}