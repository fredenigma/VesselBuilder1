#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "EventManager.h"

void DialogControl::UpdateEventsDialog(HWND hWnd) {
	return;
}

BOOL DialogControl::EventsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}