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
	bool met_enabled = SetMng->IsMETEnabled();
	SetCheckBox(hWnd, IDC_CHECK_SETMETENABLE, met_enabled);
	EnableWindow(GetDlgItem(hWnd, IDC_CHECK_SETSHOWMET), met_enabled);
	if (met_enabled) {
	//	GeneralSettingsManager::SHOWMET show = SetMng->GetShowMet();
		bool show = SetMng->GetShowMet();
		SetCheckBox(hWnd, IDC_CHECK_SETSHOWMET, show);
	}
	/*
	bool hudtxtenabled = SetMng->HUDTextEnabled();
	SetCheckBox(hWnd, IDC_CHECK_SETHUDTXTENABLE, hudtxtenabled);
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_HUDTXTSET), hudtxtenabled);
	SendDlgItemMessage(hWnd, IDC_EDIT_SETHUDTXT, EM_SETREADONLY, !hudtxtenabled,0);
	if (hudtxtenabled) {
		string text = SetMng->GetHUDText();
		SetDlgItemText(hWnd, IDC_EDIT_SETHUDTXT, text.c_str());
	}
	else {
		SetDlgItemText(hWnd, IDC_EDIT_SETHUDTXT, "");
	}
	*/
	return;
}
BOOL DialogControl::SettingsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_VESSELNAME), (LPCSTR)VB1->GetName());
		int hudtxt[4] = { IDC_STATIC_SETHUDTXT,IDC_CHECK_SETHUDTXTENABLE,IDC_EDIT_SETHUDTXT,IDC_BUTTON_HUDTXTSET };
		for (UINT i = 0; i < 4; i++) {
			ShowWindow(GetDlgItem(hWnd, hudtxt[i]), SW_HIDE);
		}
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
		/*case IDC_BUTTON_HUDTXTSET:
		{
			char cbuf[CMAXLEN] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_SETHUDTXT, cbuf, CMAXLEN);
			string hudtxt(cbuf);
			SetMng->SetHUDText(hudtxt);
			break;
		}*/
		case IDC_CHECK_SETMETENABLE:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_SETMETENABLE);
				SetMng->EnableMET(check);
				EnableWindow(GetDlgItem(hWnd, IDC_CHECK_SETSHOWMET), check);
			}
			break;
		}
		case IDC_CHECK_SETSHOWMET:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_SETSHOWMET);
				SetMng->SetShowMet(check);
			}
			break;
		}
		/*case IDC_CHECK_SETHUDTXTENABLE:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_SETHUDTXTENABLE);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_HUDTXTSET), check);
				SendDlgItemMessage(hWnd, IDC_EDIT_SETHUDTXT, EM_SETREADONLY, 0, !check);
				SetMng->EnableHUDText(check);				
			}
			break;
		}
		case IDC_COMBO_SETMETSHOW:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_SETMETSHOW, CB_GETCURSEL, 0, 0);
				if (index == 0) {
					SetMng->SetShowMet(GeneralSettingsManager::SHOWMET::NOSHOW);
				}
				else if (index == 1) {
					SetMng->SetShowMet(GeneralSettingsManager::SHOWMET::HUD);
				}
				else if (index == 2) {
					SetMng->SetShowMet(GeneralSettingsManager::SHOWMET::NOTE);
				}
			
			}
			break;
		}*/
		
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}