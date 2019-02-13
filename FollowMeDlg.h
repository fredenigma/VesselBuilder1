#pragma once
#include <CommCtrl.h>

class FollowMeDlg {
public:
	FollowMeDlg(StationBuilder1 *_SB1);
	~FollowMeDlg();
	BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND hDlg;
	void Open(HINSTANCE hDLL);
	void Close();
	bool IsOpen();
	void UpdatePosDirRot();
protected:
	StationBuilder1 *SB1;
	bool open;
	double test;
};