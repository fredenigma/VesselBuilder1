#pragma once
#include <CommCtrl.h>

class FollowMeDlg {
public:
	FollowMeDlg(VesselBuilder1 *_VB1);
	~FollowMeDlg();
	BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND hDlg;
	void Open(HINSTANCE hDLL);
	void Close();
	bool IsOpen();
	void UpdatePosDirRot();
protected:
	VesselBuilder1 *VB1;
	bool open;
	double test;
};