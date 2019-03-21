#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "AnimationManager.h"
#include "ConfigurationManager.h"

void DialogControl::UpdateReconfigDialog(HWND hWnd) {
	UINT config = CurrentSelection.config;
	map<ItemType, bool> Sections = ConfigMng->GetConfigurationSections(config);
	SetCheckBox(hWnd, IDC_CHECK_RCSET, Sections[SETTINGS] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCMSH, Sections[MESH] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCDOCK, Sections[DOCK] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCATT, Sections[ATTACHMENT] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCANIM, Sections[ANIMATIONS] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCPROP, Sections[PROPELLANT] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCTHR, Sections[THRUSTERS] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCTHRG, Sections[THRUSTERGROUPS] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCTDP, Sections[TOUCHDOWNPOINTS] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCAIRFOILS, Sections[AIRFOILS] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCCTRLSURF, Sections[CTRLSURFACES] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCLGT, Sections[LIGHTS] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCCAM, Sections[CAMERA] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCVC, Sections[VC] ? true : false);
	SetCheckBox(hWnd, IDC_CHECK_RCVARD, Sections[VARIABLEDRAG] ? true : false);
	//events to do
	return;
}

BOOL DialogControl::ReconfigDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
	{
		UINT config = CurrentSelection.config;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_RCACTIVATE:
		{
			UINT old_config = ConfigMng->GetCurrentConfiguration();

			ConfigMng->ApplyConfiguration(config);
			InitTree(hDlg);
			break;
		}
		case IDC_BUTTON_RCDELETE:
		{
			ConfigMng->DeleteConfiguration(config);
			InitTree(hDlg);
			break;
		}
		case IDC_CHECK_RCSET:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCSET);
				ConfigMng->SetSectionValid(config, SETTINGS, check);
				//ConfigMng->Configurations[config]->SetSectionValid(SETTINGS, check);
			}
			break;
		}
		}
	
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}