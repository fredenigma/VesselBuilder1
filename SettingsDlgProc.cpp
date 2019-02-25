#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdateSettingsDialog(HWND hWnd) {
	SetDlgItemDouble(hWnd, IDC_EDIT_SETEM, VB1->GetEmptyMass(), 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_SETSIZE, VB1->GetSize(), 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_SETGGD, VB1->GetGravityGradientDamping(), 1);
	VECTOR3 pmi;
	VB1->GetPMI(pmi);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_SETPMIX, IDC_EDIT_SETPMIY, IDC_EDIT_SETPMIZ, pmi);
	VECTOR3 cs;
	VB1->GetCrossSections(cs);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_SETCSX, IDC_EDIT_SETCSY, IDC_EDIT_SETCSZ, cs);
	return;
}
BOOL DialogControl::SettingsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_VESSELNAME), (LPCSTR)VB1->GetName());
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_SETEMSET:
		{
			double EM = GetDlgItemDouble(hWnd, IDC_EDIT_SETEM);
			VB1->SetEmptyMass(EM);
			break;
		}
		case IDC_BUTTON_SETSIZESET:
		{
			double Size = GetDlgItemDouble(hWnd, IDC_EDIT_SETSIZE);
			VB1->SetSize(Size);
			break;
		}
		case IDC_BUTTON_SETGGDSET:
		{
			double ggd = GetDlgItemDouble(hWnd, IDC_EDIT_SETGGD);
			VB1->SetGravityGradientDamping(ggd);
			break;
		}
		case IDC_BUTTON_SETPMISET:
		{
			VECTOR3 pmi = GetDlgItemsVector3(hWnd, IDC_EDIT_SETPMIX, IDC_EDIT_SETPMIY, IDC_EDIT_SETPMIZ);
			VB1->SetPMI(pmi);
			break;
		}
		case IDC_BUTTON_SETCSSET:
		{
			VECTOR3 cs = GetDlgItemsVector3(hWnd, IDC_EDIT_SETCSX, IDC_EDIT_SETCSY, IDC_EDIT_SETCSZ);
			VB1->SetCrossSections(cs);
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}