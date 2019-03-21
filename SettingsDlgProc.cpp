#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "GeneralSettingsManager.h"
#include "AnimationManager.h"
#include "ConfigurationManager.h"

#pragma comment(lib, "comctl32.lib")

BOOL CALLBACK EnableChilds(HWND, LPARAM);

void DialogControl::UpdateSettingsDialog(HWND hWnd) {
	SetDlgItemDouble(hWnd, IDC_EDIT_SETEM, SetMng->GetEmptyMass(), 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_SETSIZE, SetMng->GetSize(), 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_SETGGD, SetMng->GetGravityGradientDamping(), 1);
	VECTOR3 pmi = SetMng->GetPMI();
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_SETPMIX, IDC_EDIT_SETPMIY, IDC_EDIT_SETPMIZ, pmi);
	VECTOR3 cs = SetMng->GetCrossSections();
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_SETCSX, IDC_EDIT_SETCSY, IDC_EDIT_SETCSZ, cs);
	VECTOR3 rd = SetMng->GetRotDrag();
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_SETRDX, IDC_EDIT_SETRDY, IDC_EDIT_SETRDZ, rd);
	

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
		if (CurrentSelection.config != ConfigMng->GetCurrentConfiguration()) { break; }
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_SETEMSET:
		{
			double EM = GetDlgItemDouble(hWnd, IDC_EDIT_SETEM);
			SetMng->SetEmptyMass(EM);
			break;
		}
		case IDC_BUTTON_SETSIZESET:
		{
			double Size = GetDlgItemDouble(hWnd, IDC_EDIT_SETSIZE);
			SetMng->SetSize(Size);
			break;
		}
		case IDC_BUTTON_SETGGDSET:
		{
			double ggd = GetDlgItemDouble(hWnd, IDC_EDIT_SETGGD);
			SetMng->SetGravityGradientDamping(ggd);
			break;
		}
		case IDC_BUTTON_SETPMISET:
		{
			VECTOR3 pmi = GetDlgItemsVector3(hWnd, IDC_EDIT_SETPMIX, IDC_EDIT_SETPMIY, IDC_EDIT_SETPMIZ);
			SetMng->SetPMI(pmi);
			break;
		}
		case IDC_BUTTON_SETCSSET:
		{
			VECTOR3 cs = GetDlgItemsVector3(hWnd, IDC_EDIT_SETCSX, IDC_EDIT_SETCSY, IDC_EDIT_SETCSZ);
			SetMng->SetCrossSections(cs);
			break;
		}
		case IDC_BUTTON_SETRDSET:
		{
			VECTOR3 rd = GetDlgItemsVector3(hWnd, IDC_EDIT_SETRDX, IDC_EDIT_SETRDY, IDC_EDIT_SETRDZ);
			SetMng->SetRotDrag(rd);
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}