#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "AirfoilsManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateAirfoilDialog(HWND hWnd) {
	return;
}
BOOL DialogControl::AirfoilsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}