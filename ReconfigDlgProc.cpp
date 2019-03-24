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
			//InitTree(hDlg);
			UpdateRoots(hDlg, old_config);
			UpdateRoots(hDlg, config);
			UpdateSubs(hDlg, config);
			break;
		}
		case IDC_BUTTON_RCDELETE:
		{
			ConfigMng->DeleteConfiguration(config);
			Config_Items.erase(Config_Items.begin() + config);
			InitTree(hDlg);
			break;
		}
		case IDC_CHECK_RCSET:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCSET);
				ConfigMng->SetSectionValid(config, SETTINGS, check);
				ConfigMng->CopyConfigurationSection(0, config, SETTINGS);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCMSH:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCMSH);
				ConfigMng->SetSectionValid(config, MESH, check);
				ConfigMng->CopyConfigurationSection(0, config, MESH);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCDOCK:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCDOCK);
				ConfigMng->SetSectionValid(config, DOCK, check);
				ConfigMng->CopyConfigurationSection(0, config, DOCK);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCATT:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCATT);
				ConfigMng->SetSectionValid(config, ATTACHMENT, check);
				ConfigMng->CopyConfigurationSection(0, config, ATTACHMENT);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCANIM:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCANIM);
				ConfigMng->SetSectionValid(config, ANIMATIONS, check);
				ConfigMng->CopyConfigurationSection(0, config, ANIMATIONS);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCPROP:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCPROP);
				ConfigMng->SetSectionValid(config, PROPELLANT, check);
				ConfigMng->CopyConfigurationSection(0, config, PROPELLANT);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCTHR:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCTHR);
				ConfigMng->SetSectionValid(config, THRUSTERS, check);
				ConfigMng->CopyConfigurationSection(0, config, THRUSTERS);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCTHRG:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCTHRG);
				ConfigMng->SetSectionValid(config, THRUSTERGROUPS, check);
				ConfigMng->CopyConfigurationSection(0, config, THRUSTERGROUPS);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCTDP:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCTDP);
				ConfigMng->SetSectionValid(config, TOUCHDOWNPOINTS, check);
				ConfigMng->CopyConfigurationSection(0, config, TOUCHDOWNPOINTS);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCAIRFOILS:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCAIRFOILS);
				ConfigMng->SetSectionValid(config, AIRFOILS, check);
				ConfigMng->CopyConfigurationSection(0, config, AIRFOILS);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCCTRLSURF:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCCTRLSURF);
				ConfigMng->SetSectionValid(config, CTRLSURFACES, check);
				ConfigMng->CopyConfigurationSection(0, config, CTRLSURFACES);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCLGT:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCLGT);
				ConfigMng->SetSectionValid(config, LIGHTS, check);
				ConfigMng->CopyConfigurationSection(0, config, LIGHTS);
				UpdateRoots(hDlg, config);
			}
			break;
		case IDC_CHECK_RCCAM:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCCAM);
				ConfigMng->SetSectionValid(config, CAMERA, check);
				ConfigMng->CopyConfigurationSection(0, config, CAMERA);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCVC:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCVC);
				ConfigMng->SetSectionValid(config, VC, check);
				ConfigMng->CopyConfigurationSection(0, config, VC);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		case IDC_CHECK_RCVARD:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_RCVARD);
				ConfigMng->SetSectionValid(config, VARIABLEDRAG, check);
				ConfigMng->CopyConfigurationSection(0, config, VARIABLEDRAG);
				UpdateRoots(hDlg, config);
			}
			break;
		}
		}
		
		}
	
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}