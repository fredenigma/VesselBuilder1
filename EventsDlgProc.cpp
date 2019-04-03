#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "EventManager.h"
#include "DockManager.h"
#include "AnimationManager.h"
#include "ConfigurationManager.h"
#include "ThrusterManager.h"
#include "MET.h"
#include "MeshManager.h"

void DialogControl::SetTriggerDialog(HWND hWnd, int trigger_type) {
	Event::TRIGGER::TRIGGERTYPE tp = (Event::TRIGGER::TRIGGERTYPE)trigger_type;
	switch (tp) {
	case Event::TRIGGER::TRIGGERTYPE::SIMSTART: 
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), false);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::KEYPRESS:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), false);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), true);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), (LPCSTR)"Repeat");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ONCE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ONCE);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ALWAYS");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ALWAYS);


		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVCOND), (LPCSTR)"Key");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_RESETCONTENT, 0, 0);
		UINT counter = 0;
		for (map<DWORD, string>::iterator it = oapi_keys.begin(); it != oapi_keys.end(); ++it) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_INSERTSTRING, counter, (LPARAM)it->second.c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, counter, (LPARAM)it->first);
			counter++;
		}

		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::ALTITUDE:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), true);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), (LPCSTR)"Repeat");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ONCE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ONCE);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ALWAYS");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ALWAYS);


		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVCOND), (LPCSTR)"Condition");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_RESETCONTENT, 0, 0);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"BELOW");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::CONDITION::BELOW);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"ABOVE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::CONDITION::ABOVE);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTV), (LPCSTR)"Value");
				
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::MAINFUELTANK_LEVEL:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), true);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTV), (LPCSTR)"Value");

		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::VELOCITY:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), true);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTV), (LPCSTR)"Trigger Value");

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), (LPCSTR)"Repeat");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ONCE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ONCE);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ALWAYS");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ALWAYS);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVCOND), (LPCSTR)"Condition");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_RESETCONTENT, 0, 0);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"BELOW");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::CONDITION::BELOW);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"ABOVE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::CONDITION::ABOVE);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVMODE), (LPCSTR)"Mode");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_RESETCONTENT, 0, 0);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"GROUNDSPEED");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::VEL_MODE::GROUNDSPEED);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"AIRSPEED");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::VEL_MODE::AIRSPEED);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"MACH");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::VEL_MODE::MACH);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"ORBITAL SPEED");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::VEL_MODE::ORBITALSPEED);

		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::TIME:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), true);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVMODE), (LPCSTR)"Mode");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"MET");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TIME_MODE::MET);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"MJD");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TIME_MODE::MJD);
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTV), (LPCSTR)"Value");

		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), true);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTV), (LPCSTR)"Value");

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), (LPCSTR)"Repeat");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ONCE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ONCE);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)"ALWAYS");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::REPEAT_MODE::ALWAYS);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVCOND), (LPCSTR)"Condition");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_RESETCONTENT, 0, 0);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"BELOW");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::CONDITION::BELOW);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"ABOVE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::CONDITION::ABOVE);

		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::OTHER_EVENT:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), false);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), (LPCSTR)"Other Event");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < CurrentSelection.idx;i++){ //EvMng->GetEventsCount(); i++) {
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)EvMng->GetEventName(i).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)EvMng->GetEventH(i));
		}
		
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), true);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), true);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), true);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), true);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTV), (LPCSTR)"Mate Vessel Name");

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), (LPCSTR)"Docking Port");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < VB1->DckMng->GetDockCount(); i++) {
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_ADDSTRING, 0, (LPARAM)VB1->DckMng->GetDockName(i).c_str());
			//SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)VB1->DckMng->GetDH(i));
			SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETITEMDATA, index, (LPARAM)i);
		}
		
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVCOND), (LPCSTR)"Action");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"DOCKING");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)true);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_ADDSTRING, 0, (LPARAM)"UNDOCKING");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETITEMDATA, index, (LPARAM)false);

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC_EVMODE), (LPCSTR)"Vessel");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_RESETCONTENT, 0, 0);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"ANY VESSEL");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)true);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_ADDSTRING, 0, (LPARAM)"SPECIFIC VESSEL");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETITEMDATA, index, (LPARAM)false);
		
		break;
	}
	default:
	{
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTRIGREP), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVCOND), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMODE), false);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTV), false);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVTRIGVAL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVALT), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVCTRL), false);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVSHIFT), false);
		break;
	}

	}
	return;
}
void DialogControl::UpdateTriggerDialog(HWND hWnd, UINT idx) {
	Event::TRIGGER::TRIGGERTYPE etp = EvMng->GetEventTriggerType(idx);
	int etp_int = (int)etp;
	SetTriggerDialog(hWnd, etp_int);
	Event::TRIGGER trig = EvMng->GetEventTrigger(idx);
	int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTRIGTYPE), (void*)etp);
	SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETCURSEL, index, 0);

	switch (etp) {
	case Event::TRIGGER::TRIGGERTYPE::SIMSTART:
	{
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::KEYPRESS:
	{
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), (void*)trig.repeat_mode);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETCURSEL, index, 0);
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVCOND), trig.Key);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETCURSEL, index, 0);
		if (trig.KeyMods.Alt) {
			SendDlgItemMessage(hWnd, IDC_CHECK_EVALT, BM_SETCHECK, BST_CHECKED, 0);
		}
		else {
			SendDlgItemMessage(hWnd, IDC_CHECK_EVALT, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		if (trig.KeyMods.Ctrl) {
			SendDlgItemMessage(hWnd, IDC_CHECK_EVCTRL, BM_SETCHECK, BST_CHECKED, 0);
		}
		else {
			SendDlgItemMessage(hWnd, IDC_CHECK_EVCTRL, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		if (trig.KeyMods.Shift) {
			SendDlgItemMessage(hWnd, IDC_CHECK_EVSHIFT, BM_SETCHECK, BST_CHECKED, 0);
		}
		else {
			SendDlgItemMessage(hWnd, IDC_CHECK_EVSHIFT, BM_SETCHECK, BST_UNCHECKED, 0);
		}

		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::ALTITUDE:
	{
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), (void*)trig.repeat_mode);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETCURSEL, index, 0);
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVCOND), (void*)trig.condition);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETCURSEL, index, 0);
		SetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL, trig.TriggerValue, 2);
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::MAINFUELTANK_LEVEL:
	{
		SetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL, trig.TriggerValue, 3);
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::VELOCITY:
	{
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), (void*)trig.repeat_mode);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETCURSEL, index, 0);
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVCOND), (void*)trig.condition);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETCURSEL, index, 0);
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMODE), (void*)trig.vel_mode);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETCURSEL, index, 0);
		SetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL, trig.TriggerValue, 3);
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::TIME:
	{
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMODE), (void*)trig.time_mode);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_SETCURSEL, index, 0);
		if (trig.time_mode == Event::TRIGGER::TIME_MODE::MET) {
			SetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL, trig.TriggerValue, 2);
		}
		else if (trig.time_mode == Event::TRIGGER::TIME_MODE::MJD) {
			SetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL, trig.TriggerValue, 6);
		}
		
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE:
	{
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), (void*)trig.repeat_mode);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETCURSEL, index, 0);
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVCOND), (void*)trig.condition);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETCURSEL, index, 0);
		SetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL, trig.TriggerValue, 2);
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::OTHER_EVENT:
	{
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), (void*)trig.Other_event_h);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETCURSEL, index, 0);
		break;
	}
	case Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT:
	{
		SetDlgItemText(hWnd, IDC_EDIT_EVTRIGVAL, (LPCSTR)trig.MateName.c_str());
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTRIGREPEAT), (void*)trig.DockingPort);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_SETCURSEL, index, 0);
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVCOND), (void*)trig.WhenDocking);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_SETCURSEL, index, 0);
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_STATIC_EVMODE), (void*)trig.AnyMate);
		SendDlgItemMessage(hWnd, IDC_STATIC_EVMODE, CB_SETCURSEL, index, 0);

		break;
	}


	}
	return;
}

void DialogControl::SetEventsDialog(HWND hWnd, int event_type) {
	Event::TYPE type = (Event::TYPE)event_type;
	switch (type) {
	case Event::TYPE::CHILD_SPAWN:
	{
		ShowWindow(hWnd_ChildSpawn, SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);
		break;
	}
	case Event::TYPE::PAYLOAD_JETTISON:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVMAIN, (LPCSTR)"Jettisonable Docks");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < VB1->DckMng->GetDockCount(); i++) {
			if (!VB1->DckMng->IsDockJettisonable(i)) { continue; }
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)VB1->DckMng->GetDockName(i).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)VB1->DckMng->GetDH(i));
		}
		break;
	}
	case Event::TYPE::ANIMATION_TRIGGER:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVMAIN, (LPCSTR)"Animations");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < AnimMng->GetAnimDefsCount(); i++) {
			if ((AnimMng->GetAnimCycle(i) == AnimCycleType::AUTOMATIC) || (AnimMng->GetAnimCycle(i) == AnimCycleType::MANUAL)) { continue; }
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)AnimMng->GetAnimName(i).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)i);
		}
		SetDlgItemText(hWnd, IDC_STATIC_EVTEXTURES, (LPCSTR)"Going");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_ADDSTRING, 0, (LPARAM)"FORWARD");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETITEMDATA, index, (LPARAM)true);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_ADDSTRING, 0, (LPARAM)"BACKWARD");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETITEMDATA, index, (LPARAM)false);

		break;
	}
	case Event::TYPE::THRUSTER_FIRING:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVMAIN, (LPCSTR)"Thrusters");
		SetDlgItemText(hWnd, IDC_STATIC_EVLVL, (LPCSTR)"Thruster Lvl");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < VB1->ThrMng->GetThrCount(); i++) {
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)ThrMng->GetThrName(i).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)ThrMng->GetThrTH(i));
		}

		break;
	}
	case Event::TYPE::THRUSTERGROUP_LEVEL:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVMAIN, (LPCSTR)"Thruster Groups");
		SetDlgItemText(hWnd, IDC_STATIC_EVLVL, (LPCSTR)"Th Grp Lvl");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"MAIN");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_MAIN);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"RETRO");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_RETRO);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"HOVER");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_HOVER);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"PITCH UP");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_PITCHUP);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"PITCH DOWN");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_PITCHDOWN);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"YAW LEFT");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_YAWLEFT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"YAW RIGHT");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_YAWRIGHT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"BANK LEFT");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_BANKLEFT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"BANK RIGHT");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_BANKRIGHT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"TRANSLATE UP");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_UP);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"TRANSLATE DOWN");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_DOWN);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"TRANSLATE RIGHT");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_RIGHT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"TRANSLATE LEFT");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_LEFT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"TRANSLATE FORWARD");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_FORWARD);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)"TRANSLATE BACK");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)THGROUP_ATT_BACK);
		break;
	}
	case Event::TYPE::RESET_MET:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);
		SetDlgItemText(hWnd, IDC_STATIC_EVNEWMJD0, "New MJD0");
		
		break;
	}
	case Event::TYPE::RECONFIG:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVMAIN, (LPCSTR)"Configurations");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < ConfigMng->GetConfigurationsCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "Reconfiguration %i", i);
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)cbuf);
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)i);
		}
		break;
	}
	case Event::TYPE::TEXTURE_SWAP:
	{

		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVMAIN, (LPCSTR)"Meshes");
		SetDlgItemText(hWnd, IDC_STATIC_EVTEXTURES, (LPCSTR)"Mesh Textures");
		SetDlgItemText(hWnd, IDC_STATIC_EVNEWMJD0, "New Texture File");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < MshMng->GetMeshCount(); i++) {
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)MshMng->GetMeshDefName(i).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)i);
		}

		break;
	}
	case Event::TYPE::SHIFT_CG:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_SHOW);

		break;
	}
	case Event::TYPE::PLAYSOUND:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVNEWMJD0, "Sound File");

		break;
	}
	case Event::TYPE::ENABLE_EVENT:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		SetDlgItemText(hWnd, IDC_STATIC_EVMAIN, "Events");
		SetDlgItemText(hWnd, IDC_STATIC_EVTEXTURES, "Enable?");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_RESETCONTENT, 0, 0);
		for (UINT i = 0; i < CurrentSelection.idx; i++) { //EvMng->GetEventsCount(); i++) {
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_ADDSTRING, 0, (LPARAM)EvMng->GetEventName(i).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETITEMDATA, index, (LPARAM)EvMng->GetEventH(i));
		}
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_ADDSTRING, 0, (LPARAM)"ENABLE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETITEMDATA, index, (LPARAM)true);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_ADDSTRING, 0, (LPARAM)"DISABLE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETITEMDATA, index, (LPARAM)false);
		break;
	}
	default:
	{
		ShowWindow(hWnd_ChildSpawn, SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVNEXT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVMAIN), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_SLIDER_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVLVL), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVMJDSET), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHECK_EVRSTATEV), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_STATIC_EVSHIFT), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVX), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVY), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_EDIT_EVZ), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_EVSHIFTSET), SW_HIDE);

		break;
	}
	}

	return;
}

void DialogControl::UpdateEventsDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= EvMng->GetEventsCount()) { return;}
	char namebuf[256] = { '\0' };
	sprintf(namebuf,"%s", EvMng->GetEventName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_EVNAME, namebuf);
	UpdateTriggerDialog(hWnd, idx);
	//SetCheckBox(hWnd, IDC_CHECK_EVDISABLE, !EvMng->IsEventEnabled(idx));
	SetCheckBox(hWnd, IDC_CHECK_EVDISABLE, !EvMng->GetEventDefaultEnabled(idx));
	Event::TYPE type = EvMng->GetEventType(idx);
	int etp = (int)type;
	SetEventsDialog(hWnd, etp);
	switch (type) {
	case Event::TYPE::CHILD_SPAWN:
	{
		UpdateChildSpawnDialog(hWnd_ChildSpawn);
		break;
	}
	case Event::TYPE::PAYLOAD_JETTISON:
	{
		
		bool next = EvMng->GetNext(idx);
		SetCheckBox(hWnd, IDC_CHECK_EVNEXT, next);
		if (!next) {
			UINT dock_idx = EvMng->GetDockIdx(idx);
			if (dock_idx == (UINT)-1) { break; }
			DOCKHANDLE dh = DckMng->GetDH(dock_idx);
			int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), (void*)dh);
			if (index >= 0) {
				SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETCURSEL, index, 0);
			}
			
		}
		break;
	}
	case Event::TYPE::ANIMATION_TRIGGER:
	{
		UINT anim = EvMng->GetAnimIdx(idx);
		if (anim >= AnimMng->GetAnimDefsCount()) { break; }
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), (void*)anim);
		if (index >= 0) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETCURSEL, index, 0);
		}
		bool forward = EvMng->GetForward(idx);
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETCURSEL, forward ? 0 : 1, 0);
		break;
	}
	case Event::TYPE::THRUSTER_FIRING:
	{
		THRUSTER_HANDLE th = EvMng->GetThrusterTH(idx);
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), (void*)th);
		if (index >= 0) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETCURSEL, index, 0);
		}
		double thlevel = EvMng->GetThLevel(idx);
		thlevel *= 100;
		SendDlgItemMessage(hWnd, IDC_SLIDER_EVLVL, TBM_SETPOS, true, (LPARAM)thlevel);
		SetDlgItemDouble(hWnd, IDC_EDIT_EVLVL, thlevel, 2);
		
		break;
	}
	case Event::TYPE::THRUSTERGROUP_LEVEL:
	{
		THGROUP_TYPE thg = EvMng->GetThGroup(idx);
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), (void*)thg);
		if (index >= 0) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETCURSEL, index, 0);
		}
		double thglevel = EvMng->GetThGLevel(idx);
		thglevel *= 100;
		SendDlgItemMessage(hWnd, IDC_SLIDER_EVLVL, TBM_SETPOS, true, (LPARAM)thglevel);
		SetDlgItemDouble(hWnd, IDC_EDIT_EVLVL, thglevel, 2);
		break;
	}
	case Event::TYPE::RESET_MET:
	{
		bool now = EvMng->GetNow(idx);
		SetCheckBox(hWnd, IDC_CHECK_EVRSTATEV, now);
		if (!now) {
			double newmjd0 = EvMng->GetNewMJD0(idx);
			SetDlgItemDouble(hWnd, IDC_EDIT_EVNEWMJD0, newmjd0, 6);
		}
		else {
			SetDlgItemText(hWnd, IDC_EDIT_EVNEWMJD0, "");
		}
		break;
	}
	case Event::TYPE::RECONFIG:
	{
		UINT config = EvMng->GetNewConfig(idx);
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), (void*)config);
		if (index >= 0) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETCURSEL, index, 0);
		}
		break;
	}
	case Event::TYPE::TEXTURE_SWAP:
	{
		UINT mesh = EvMng->GetMesh(idx);
		DWORD texidx = EvMng->GetTexIdx(idx);
		string texture_name = EvMng->GetTextureName(idx);
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), (void*)mesh);
		if (index >= 0) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETCURSEL, index, 0);

			DWORD texcount = oapiMeshTextureCount(MshMng->GetMeshDefMH(mesh));
			SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_RESETCONTENT, 0, 0);
			for (DWORD j = 1; j <= texcount; j++) {
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "%i", j);
				SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_INSERTSTRING, j - 1, (LPARAM)cbuf);
				SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETITEMDATA, j - 1, (LPARAM)j);
			}
		}
		SetDlgItemText(hWnd, IDC_EDIT_EVNEWMJD0, (LPCSTR)texture_name.c_str());
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), texidx);
		if (index >= 0) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETCURSEL, index, 0);
		}
		break;
	}
	case Event::TYPE::SHIFT_CG:
	{
		VECTOR3 shift = EvMng->GetShift(idx);
		SetDlgItemsTextVector3(hWnd, IDC_EDIT_EVX, IDC_EDIT_EVY, IDC_EDIT_EVZ, shift);
		break;
	}
	case Event::TYPE::PLAYSOUND:
	{
		NEWCHAR(cbuf);
		sprintf(cbuf, "%s", EvMng->GetSoundFile(idx).c_str());
		SetDlgItemText(hWnd, IDC_EDIT_EVNEWMJD0, (LPCSTR)cbuf);
		break;
	}
	case Event::TYPE::ENABLE_EVENT:
	{
		Event* evtoe = EvMng->GetEventToEnable(idx);
		int index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVMAIN), evtoe);
		if (index != -1) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_SETCURSEL, index, 0);
		}
		index = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_EVTEXTURES), EvMng->GetToEnable(idx));
		if (index != -1) {
			SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETCURSEL, index, 0);
		}

		break;
	}
	}
	return;
}
//RESET_MET, RECONFIG, TEXTURE_SWAP, SHIFT_CG, DELETE_ME};
BOOL DialogControl::EventsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"SIMSTART");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::SIMSTART);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"KEYPRESS");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::KEYPRESS);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"ALTITUDE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::ALTITUDE);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"MAIN FUEL TANK LVL");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::MAINFUELTANK_LEVEL);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"VELOCITY");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::VELOCITY);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"TIME");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::TIME);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"DYNAMIC PRESSURE");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"OTHER EVENT");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::OTHER_EVENT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_ADDSTRING, 0, (LPARAM)"DOCKING");
		SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_SETITEMDATA, index, (LPARAM)Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT);

		SendDlgItemMessage(hWnd, IDC_EDIT_EVLVL, EM_SETREADONLY, true, 0);
		
		SendDlgItemMessage(hWnd, IDC_SLIDER_EVLVL, TBM_SETRANGE, true, MAKELPARAM(0, 100));

		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch LOWORD(wParam) {
		case IDC_COMBO_EVTRIGTYPE:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_GETCURSEL, 0, 0);
				if (index >= 0) {
					Event::TRIGGER::TRIGGERTYPE tp = (Event::TRIGGER::TRIGGERTYPE)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_GETITEMDATA, index, 0);
					int tp_int = (int)tp;
					SetTriggerDialog(hWnd, tp_int);
				}
			}
			break;
		}
		case IDC_BUTTON_EVNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_EVNAME, cbuf, 256);
			string newname(cbuf);
			EvMng->SetEventName(idx, newname);
						
			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);
			break;
		}
		case IDC_BUTTON_EVDELETE:
		{
			EvMng->DeleteEvent(EvMng->GetEventH(idx));
			UpdateTree(hDlg, EVENTS, CurrentSelection.config);
			break;
		}
		case IDC_BUTTON_EVTRIGGERSET:
		{
			Event::TRIGGER Trig = Event::TRIGGER();
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_GETCURSEL, 0, 0);
			Event::TRIGGER::TRIGGERTYPE type = (Event::TRIGGER::TRIGGERTYPE)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGTYPE, CB_GETITEMDATA, index, 0);
			Trig.Type = type;
			switch (type) {
			case Event::TRIGGER::TRIGGERTYPE::SIMSTART:
			{
				//EvMng->SetEventTrigger(idx, Trig);
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::KEYPRESS:
			{
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::REPEAT_MODE rp = (Event::TRIGGER::REPEAT_MODE)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETITEMDATA, index, 0);
				Trig.repeat_mode = rp;
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETCURSEL, 0, 0);
				DWORD key = (DWORD)SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETITEMDATA, index, 0);
				Trig.Key = key;
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_EVALT);
				Trig.KeyMods.Alt = check;
				check = IsCheckBoxChecked(hWnd, IDC_CHECK_EVCTRL);
				Trig.KeyMods.Ctrl = check;
				check = IsCheckBoxChecked(hWnd, IDC_CHECK_EVSHIFT);
				Trig.KeyMods.Shift = check;
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::ALTITUDE:
			{
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::REPEAT_MODE rp = (Event::TRIGGER::REPEAT_MODE)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETITEMDATA, index, 0);
				Trig.repeat_mode = rp;
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::CONDITION cond = (Event::TRIGGER::CONDITION)SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETITEMDATA, index, 0);
				Trig.condition = cond;
				double val = GetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL);
				Trig.TriggerValue = val;
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::MAINFUELTANK_LEVEL:
			{
				double val = GetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL);
				Trig.TriggerValue = val;
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::VELOCITY:
			{
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::REPEAT_MODE rp = (Event::TRIGGER::REPEAT_MODE)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETITEMDATA, index, 0);
				Trig.repeat_mode = rp;
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::CONDITION cond = (Event::TRIGGER::CONDITION)SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETITEMDATA, index, 0);
				Trig.condition = cond;
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::VEL_MODE vel_mode = (Event::TRIGGER::VEL_MODE)SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_GETITEMDATA, index, 0);
				Trig.vel_mode = vel_mode;
				double val = GetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL);
				Trig.TriggerValue = val;
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::TIME:
			{
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::TIME_MODE time_mode = (Event::TRIGGER::TIME_MODE)SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_GETITEMDATA, index, 0);
				Trig.time_mode = time_mode;
				double val = GetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL);
				Trig.TriggerValue = val;
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::DYNPRESSURE:
			{
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::REPEAT_MODE rp = (Event::TRIGGER::REPEAT_MODE)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETITEMDATA, index, 0);
				Trig.repeat_mode = rp;
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETCURSEL, 0, 0);
				Event::TRIGGER::CONDITION cond = (Event::TRIGGER::CONDITION)SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETITEMDATA, index, 0);
				Trig.condition = cond;
				double val = GetDlgItemDouble(hWnd, IDC_EDIT_EVTRIGVAL);
				Trig.TriggerValue = val;
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::OTHER_EVENT:
			{
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETCURSEL, 0, 0);
				Event* ev = (Event*)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETITEMDATA, index, 0);
				Trig.Other_event_h = ev;
				break;
			}
			case Event::TRIGGER::TRIGGERTYPE::DOCK_EVENT:
			{
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_GETCURSEL, 0, 0);
				bool anymate = (bool)SendDlgItemMessage(hWnd, IDC_COMBO_EVMODE, CB_GETITEMDATA, index, 0);
				if (!anymate) {
					char nbuf[256] = { '\0' };
					GetDlgItemText(hWnd, IDC_EDIT_EVTRIGVAL, nbuf, 256);
					string matename(nbuf);
					Trig.MateName = matename;
				}
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETCURSEL, 0, 0);
				bool docking= (bool)SendDlgItemMessage(hWnd, IDC_COMBO_EVCOND, CB_GETITEMDATA, index, 0);
				Trig.WhenDocking = docking;
				
				index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETCURSEL, 0, 0);
				UINT dockidx = (UINT)SendDlgItemMessage(hWnd, IDC_COMBO_EVTRIGREPEAT, CB_GETITEMDATA, index, 0);
				if (dockidx >= DckMng->GetDockCount()) { Trig.DockingPort=0; }
				Trig.DockingPort = dockidx;
				break;
			}
			

			}

			EvMng->SetEventTrigger(idx, Trig);
			break;
		}
		case IDC_BUTTON_EVSHIFTSET:
		{
			VECTOR3 shift = GetDlgItemsVector3(hWnd, IDC_EDIT_EVX, IDC_EDIT_EVY, IDC_EDIT_EVZ);
			EvMng->SetShift(idx, shift);
			break;
		}
		case IDC_BUTTON_EVMJDSET:
		{
			Event::TYPE tp = EvMng->GetEventType(idx);
			if (tp == Event::TYPE::RESET_MET) {
				double nmjd0 = GetDlgItemDouble(hWnd, IDC_EDIT_EVNEWMJD0);
				EvMng->SetNewMJD0(idx, nmjd0);
			}
			else if (tp == Event::TYPE::TEXTURE_SWAP) {
				char tbuf[256] = { '\0' };
				GetDlgItemText(hWnd, IDC_EDIT_EVNEWMJD0, tbuf, 256);
				string newtexture(tbuf);
				EvMng->SetTextureName(idx,newtexture);
			}
			else if (tp == Event::TYPE::PLAYSOUND) {
				string OrbiterDir= VB1->OrbiterRoot;
				OPENFILENAME ofn;       // common dialog box structure
				char szFile[260] = { '\0' };       
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "Sound Files\0*.wav\0\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = OrbiterDir.c_str();
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				char currentdir[MAX_PATH];

				GetCurrentDirectory(sizeof(currentdir) / sizeof(char), currentdir);

				if (GetOpenFileName(&ofn) == TRUE)
				{
					SetCurrentDirectory(currentdir);
					SetWindowText(GetDlgItem(hWnd, IDC_EDIT_EVNEWMJD0), szFile);
					string snd_file(szFile);
					EvMng->SetSoundFile(idx, snd_file);
				}
			}
			break;
		}
		case IDC_CHECK_EVRSTATEV:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_EVRSTATEV);
				EvMng->SetNow(idx, check);
			}
			break;
		}
		case IDC_COMBO_EVTEXTURES:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				Event::TYPE tp = EvMng->GetEventType(idx);
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_GETCURSEL, 0, 0);
				if (tp == Event::TYPE::ANIMATION_TRIGGER) {
					bool forward = (bool)SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_GETITEMDATA, index, 0);
					EvMng->SetForward(idx, forward);
				}
				else if (tp == Event::TYPE::TEXTURE_SWAP) {
					int texidx = SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_GETITEMDATA, index, 0);
					if (texidx >= 0) {
						EvMng->SetTexIdx(idx, texidx);
					}
				}
				else if (tp == Event::TYPE::ENABLE_EVENT) {
					bool enable = (bool)SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_GETITEMDATA, index, 0);
					EvMng->SetToEnable(idx, enable);
				}
			}
			break;
		}
		case IDC_CHECK_EVNEXT:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_EVNEXT);
				EvMng->SetNext(idx, check);
			}
			break;
		}
		case IDC_COMBO_EVMAIN:
		{
			Event::TYPE tp = EvMng->GetEventType(idx);
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETCURSEL, 0, 0);
			switch (tp) {
			case Event::TYPE::PAYLOAD_JETTISON:
			{
				DOCKHANDLE dh = (DOCKHANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETITEMDATA, index, 0);
				int dockidx = DckMng->GetDockIdx(dh);
				EvMng->SetDockIdx(idx,dockidx);
				break;
			}
			case Event::TYPE::ANIMATION_TRIGGER:
			{
				UINT animidx = (UINT)SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETITEMDATA, index, 0);
				EvMng->SetAnimIdx(idx, animidx);
				break;
			}
			case Event::TYPE::THRUSTER_FIRING:
			{
				THRUSTER_HANDLE th = (THRUSTER_HANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETITEMDATA, index, 0);
				EvMng->SetThrusterTH(idx, th);
				break;
			}
			case Event::TYPE::THRUSTERGROUP_LEVEL:
			{
				THGROUP_TYPE thgt = (THGROUP_TYPE)SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETITEMDATA, index, 0);
				EvMng->SetThGroup(idx, thgt);
				break;
			}
			case Event::TYPE::RECONFIG:
			{
				UINT reconfig_idx = (UINT)SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETITEMDATA, index, 0);
				EvMng->SetNewConfig(idx, reconfig_idx);
				break;
			}
			case Event::TYPE::TEXTURE_SWAP:
			{
				UINT mesh_idx = (UINT)SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETITEMDATA, index, 0);
				EvMng->SetMesh(idx, mesh_idx);

				DWORD texcount = oapiMeshTextureCount(MshMng->GetMeshDefMH(mesh_idx));
				SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_RESETCONTENT, 0, 0);
				for (DWORD j = 1; j <= texcount; j++) {
					char cbuf[256] = { '\0' };
					sprintf(cbuf, "%i", j);
					SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_INSERTSTRING, j-1, (LPARAM)cbuf);
					SendDlgItemMessage(hWnd, IDC_COMBO_EVTEXTURES, CB_SETITEMDATA, j-1, (LPARAM)j);
				}

				break;
			}
			case Event::TYPE::ENABLE_EVENT:
			{
				Event* evtoe = (Event*)SendDlgItemMessage(hWnd, IDC_COMBO_EVMAIN, CB_GETITEMDATA, index, 0);
				EvMng->SetEventToEnable(idx, evtoe);
				break;
			}

			}
			break;
		}
		case IDC_CHECK_EVDISABLE:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_EVDISABLE);
				EvMng->SetEnableEvent(idx, !check);
				EvMng->SetEventDefaultEnabled(idx, !check);
			}
			break;
		}



		}
		break;
	}
	case WM_HSCROLL:
	{
		if (lParam == (LPARAM)GetDlgItem(hWnd, IDC_SLIDER_EVLVL)) {
			LRESULT result = SendDlgItemMessage(hWnd, IDC_SLIDER_EVLVL, TBM_GETPOS, 0, 0);
			SetDlgItemDouble(hWnd, IDC_EDIT_EVLVL, (double)(result), 2);
			Event::TYPE tp = EvMng->GetEventType(CurrentSelection.idx);
			if (tp == Event::TYPE::THRUSTER_FIRING) {
				EvMng->SetThLevel(CurrentSelection.idx, (double)(result) / 100);
			}
			else if (tp == Event::TYPE::THRUSTERGROUP_LEVEL) {
				EvMng->SetThGLevel(CurrentSelection.idx, (double)(result) / 100);
			}
			//AnimMng->SetAnimationState(CurrentSelection.idx, (double)(result) / 100);
			break;
		}
	}



	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}

void DialogControl::UpdateChildSpawnDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	
	SetDlgItemText(hWnd, IDC_EDIT_CSCLASS, (LPCSTR)EvMng->GetSpawnedVesselClass(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_CSNAME, (LPCSTR)EvMng->GetSpawnedVesselName(idx).c_str());
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_CSVELX, IDC_EDIT_CSVELY, IDC_EDIT_CSVELZ, EvMng->GetVel(idx));
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_CSRVELX, IDC_EDIT_CSRVELY, IDC_EDIT_CSRVELZ, EvMng->GetRotVel(idx));
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_CSOFSX, IDC_EDIT_CSOFSY, IDC_EDIT_CSOFSZ, EvMng->GetOfs(idx));
	int mesh = EvMng->GetMeshToDel(idx);
	bool check = false;
	if (mesh >= 0) {
		check = true;
	}
	SetCheckBox(hWnd, IDC_CHECK_CSDELMESH, check);
	EnableWindow(GetDlgItem(hWnd, IDC_COMBO_CSMSHTODEL), check);
	SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < MshMng->GetMeshCount(); i++) {
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_ADDSTRING, 0, (LPARAM)MshMng->GetMeshName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_SETITEMDATA, index, (LPARAM)i);
	}
	if (check) {
		int idex = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_CSMSHTODEL), (void*)mesh);
		if (idex >= 0) {
			SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_SETCURSEL, idex, 0);
		}
	}
	
	return;
}

BOOL DialogControl::ChildSpawnDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_CSCLASSSET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_CSCLASS, cbuf, 256);
			string newclass(cbuf);
			EvMng->SetSpawnedVesselClass(idx, newclass);
			break;
		}
		case IDC_BUTTON_CSNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_CSNAME, cbuf, 256);
			string newname(cbuf);
			EvMng->SetSpawnedVesselName(idx, newname);
			break;
		}
		case IDC_BUTTON_CSVELSET:
		{
			VECTOR3 vel = GetDlgItemsVector3(hWnd, IDC_EDIT_CSVELX, IDC_EDIT_CSVELY, IDC_EDIT_CSVELZ);
			EvMng->SetVel(idx, vel);
			break;
		}
		case IDC_BUTTON_CSRVELSET:
		{
			VECTOR3 rot_vel = GetDlgItemsVector3(hWnd, IDC_EDIT_CSRVELX, IDC_EDIT_CSRVELY, IDC_EDIT_CSRVELZ);
			EvMng->SetRotVel(idx, rot_vel);
			break;
		}
		case IDC_BUTTON_CSOFSSET:
		{
			VECTOR3 ofs = GetDlgItemsVector3(hWnd, IDC_EDIT_CSOFSX, IDC_EDIT_CSOFSY, IDC_EDIT_CSOFSZ);
			EvMng->SetOfs(idx, ofs);
			break;
		}
		case IDC_BUTTON_CSUSEMSHOFS:
		{
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_GETCURSEL, 0, 0);
			if (index < 0) { break; }
			UINT msh_idx = SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_GETITEMDATA, index, 0);
			if (msh_idx >= MshMng->GetMeshCount()) { break; }
			VECTOR3 ofs = MshMng->GetMeshDefPos(msh_idx);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_CSOFSX, IDC_EDIT_CSOFSY, IDC_EDIT_CSOFSZ, ofs);
			break;
		}
		case IDC_CHECK_CSDELMESH:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_CSDELMESH);
				if (!check) {
					EvMng->SetMeshToDel(idx, -1);
				}
				EnableWindow(GetDlgItem(hWnd, IDC_COMBO_CSMSHTODEL), check);
				
			}
			break;
		}
		case IDC_COMBO_CSMSHTODEL:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_GETCURSEL, 0, 0);
				if (index < 0) { break; }
				int msh_idx = SendDlgItemMessage(hWnd, IDC_COMBO_CSMSHTODEL, CB_GETITEMDATA, index, 0);
				if (msh_idx >= MshMng->GetMeshCount()) { break; }
				EvMng->SetMeshToDel(idx, msh_idx);
			}
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}