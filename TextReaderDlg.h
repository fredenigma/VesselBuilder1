#pragma once

class TextReaderDlg{
public:
	TextReaderDlg();
	~TextReaderDlg();
	BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND hDlg;
	string TextFileContent(string filename);
	void Open(HINSTANCE hDLL);
	void Close();
	bool IsOpen();
protected:
	bool open;
};