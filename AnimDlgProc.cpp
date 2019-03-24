#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "AnimationManager.h"
#pragma comment(lib, "comctl32.lib")



void DialogControl::UpdateAnimDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= AnimMng->GetAnimDefsCount()) { return; }
	

	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%.2f", AnimMng->GetAnimDefState(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ANIMDEFSTATE, (LPCSTR)cbuf);
	SetDlgItemText(hWnd, IDC_EDIT_ANIMNAME, (LPCSTR)AnimMng->GetAnimName(idx).c_str());
	
	sprintf(cbuf, "%i", AnimMng->GetAnimNComps(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ANIMNCOMPS, (LPCSTR)cbuf);
	
	

	int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_ANIMKEY), AnimMng->GetAnimKey(idx));
	
	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_SETCURSEL, index, 0);

	sprintf(cbuf, "%.1f", AnimMng->GetAnimDuration(idx));
	SetDlgItemText(hWnd, IDC_EDIT_ANIMDURATION, (LPCSTR)cbuf);

	SendDlgItemMessage(hWnd, IDC_SLIDER_ANIMTEST, TBM_SETPOS, true, (int)(AnimMng->GetAnimDefState(idx)*100));

	switch (AnimMng->GetAnimCycle(idx)) {
	case GOANDSTOP:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETCURSEL, 0, 0);
		break;
	}
	case REVERSE:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETCURSEL, 1, 0);
		break;
	}
	case RESTART:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETCURSEL, 2, 0);
		break;
	}
	case MANUAL:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETCURSEL, 3, 0);
		break;
	}
	case AUTOMATIC:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETCURSEL, 4, 0);
		break;
	}
	}
	return;
}

BOOL DialogControl::AnimDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		InitCommonControls();
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_INSERTSTRING, 0, (LPARAM)"GO&STOP");
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETITEMDATA, 0, (LPARAM)AnimCycleType::GOANDSTOP);
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_INSERTSTRING, 1, (LPARAM)"REVERSE");
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETITEMDATA, 1, (LPARAM)AnimCycleType::REVERSE);
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_INSERTSTRING, 2, (LPARAM)"RESTART");
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETITEMDATA, 2, (LPARAM)AnimCycleType::RESTART);
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_INSERTSTRING, 3, (LPARAM)"MANUAL");
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETITEMDATA, 3, (LPARAM)AnimCycleType::MANUAL);
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_INSERTSTRING, 4, (LPARAM)"AUTOMATIC");
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_SETITEMDATA, 4, (LPARAM)AnimCycleType::AUTOMATIC);


		SendDlgItemMessage(hWnd, IDC_EDIT_ANIMNCOMPS, EM_SETREADONLY, true, 0);
		InitAnimKeyCombo(hWnd);
		SendDlgItemMessage(hWnd, IDC_SLIDER_ANIMTEST, TBM_SETRANGEMIN, false, 0);
		SendDlgItemMessage(hWnd, IDC_SLIDER_ANIMTEST, TBM_SETRANGEMAX, false,100);
		
		
		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_ANIMNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_ANIMNAME, cbuf, 256);
			string name(cbuf);
			AnimMng->SetAnimName(idx, name);
			
			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			sprintf(cbuf,"%s", AnimMng->GetAnimName(idx).c_str());
			tvi.pszText = (LPSTR)cbuf;
			tvi.hItem = CurrentSelection.hitem;
			TreeView_SetItem(GetDlgItem(hDlg, IDC_TREE1), &tvi);
			break;
		}
		case IDC_BUTTON_ANIMADDCOMPROT:
		{
			AnimMng->AddAnimCompDef(CurrentSelection.idx, MGROUP_TRANSFORM::ROTATE);
			UpdateTree(hDlg, ANIMATIONS,CurrentSelection.config);
			break;
		}
		case IDC_BUTTON_ANIMADDCOMPTR:
		{
			AnimMng->AddAnimCompDef(CurrentSelection.idx, MGROUP_TRANSFORM::TRANSLATE);
			UpdateTree(hDlg, ANIMATIONS, CurrentSelection.config);

			break;
		}
		case IDC_BUTTON_ANIMADDCOMPSC:
		{
			AnimMng->AddAnimCompDef(CurrentSelection.idx, MGROUP_TRANSFORM::SCALE);
			UpdateTree(hDlg, ANIMATIONS, CurrentSelection.config);

			break;
		}
		case IDC_BUTTON_ANIMDURATIONSET:
		{
			double duration = 10;
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_ANIMDURATION, (LPSTR)cbuf, 256);
			duration = atof(cbuf);
			AnimMng->SetAnimDuration(idx, duration);
			break;
		}
		case IDC_BUTTON_ANIMDEFSTATESET:
		{
			double defstate = 0;
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_ANIMDEFSTATE, (LPSTR)cbuf, 256);
			defstate = atof(cbuf);
			AnimMng->SetAnimDefState(idx, defstate);
			break;
		}
		case IDC_COMBO_ANIMKEY:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_GETCURSEL, 0, 0);
				DWORD Key = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_GETITEMDATA, index, 0);
				AnimMng->SetAnimKey(idx, Key);
			}
			break;
		}
		case IDC_COMBO_ANIMTYPE:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_GETCURSEL, 0, 0);
				AnimCycleType Type = (AnimCycleType)SendDlgItemMessage(hWnd, IDC_COMBO_ANIMTYPE, CB_GETITEMDATA, index, 0);
				AnimMng->SetAnimCycle(idx, Type);
			}
			break;
		}
		case IDC_BUTTON_ANIMTESTANIM:
		{
			if (!AnimTesting) {
				AnimTesting = true;
				SendDlgItemMessage(hWnd, IDC_EDIT_ANIMDEFSTATE, EM_SETREADONLY, true, 0);
				SendDlgItemMessage(hWnd, IDC_EDIT_ANIMDURATION, EM_SETREADONLY, true, 0);
				SendDlgItemMessage(hWnd, IDC_EDIT_ANIMNAME, EM_SETREADONLY, true, 0);

				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMDURATIONSET), false);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMDEFSTATESET), false);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMNAMESET), false);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMDELETE), false);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_ANIMKEY), false);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_ANIMTYPE), false);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMADDCOMPROT), false);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMADDCOMPSC), false);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMADDCOMPTR), false);
				EnableWindow(GetDlgItem(hWnd, IDC_SLIDER_ANIMTEST), true);
				EnableWindow(GetDlgItem(hDlg, IDC_TREE1), false);
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ANIMTESTANIM), "STOP");
				AnimMng->StartAnimation(idx);

			}
			else {
				AnimTesting = false;
				SendDlgItemMessage(hWnd, IDC_EDIT_ANIMDEFSTATE, EM_SETREADONLY, false, 0);
				SendDlgItemMessage(hWnd, IDC_EDIT_ANIMDURATION, EM_SETREADONLY, false, 0);
				SendDlgItemMessage(hWnd, IDC_EDIT_ANIMNAME, EM_SETREADONLY, false, 0);

				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMDURATIONSET), true);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMDEFSTATESET), true);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMNAMESET), true);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMDELETE), true);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_ANIMKEY), true);
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_ANIMTYPE), true);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMADDCOMPROT), true);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMADDCOMPSC), true);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ANIMADDCOMPTR), true);
				EnableWindow(GetDlgItem(hWnd, IDC_SLIDER_ANIMTEST), false);
				EnableWindow(GetDlgItem(hDlg, IDC_TREE1), true);
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ANIMTESTANIM), "TEST ANIMATION");
				AnimMng->StopAnimation(idx);
				AnimMng->ResetAnimation(idx);
				SendDlgItemMessage(hWnd, IDC_SLIDER_ANIMTEST, TBM_SETPOS, true, (int)(AnimMng->GetAnimDefState(idx) * 100));
			}
			break;
		}
		case IDC_BUTTON_ANIMDELETE:
		{
			TreeView_Select(GetDlgItem(hDlg, IDC_TREE1), Config_Items[CurrentSelection.config].hrootAnimations, 0);
			AnimMng->DeleteAnimDef(idx);
			
			UpdateTree(hDlg, ANIMATIONS, CurrentSelection.config);
		//	UpdateAnimDialog(hWnd);
			break;
		}



		}
		break;
	}
	case WM_HSCROLL:
	{
		if (lParam == (LPARAM)GetDlgItem(hWnd, IDC_SLIDER_ANIMTEST)) {
			LRESULT result = SendDlgItemMessage(hWnd, IDC_SLIDER_ANIMTEST, TBM_GETPOS, 0, 0);
			AnimMng->SetAnimationState(CurrentSelection.idx, (double)(result) / 100);
			break;
		}
	}



	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}


void DialogControl::InitAnimKeyCombo(HWND hWnd) {
	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_INSERTSTRING, 0, (LPARAM)"NONE");
	SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_SETITEMDATA, 0, (LPARAM)0);

	UINT counter = 1;
	for (map<DWORD, string>::iterator it = oapi_keys.begin(); it != oapi_keys.end(); ++it) {
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_INSERTSTRING, counter, (LPARAM)it->second.c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_ANIMKEY, CB_SETITEMDATA, counter, (LPARAM)it->first);
		counter++;
	}
	return;
}