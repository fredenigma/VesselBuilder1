#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "PropellantManager.h"
#include "ParticleManager.h"
#include "ExTexManager.h"
#include "ThrusterManager.h"
#include "LaserManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::ShowExhaustsWin(HWND hWnd, bool show) {
	RECT rect;
	GetWindowRect(hDlg, &rect);
	int currwheight = rect.bottom - rect.top;
	int currwwidth = rect.right - rect.left;

	if (show && !ShowingExhausts) {
		SetWindowPos(hDlg, HWND_TOP, rect.left, rect.top, currwwidth + 500, currwheight, 0);
		ShowingExhausts= true;
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_TOGEXHWIN), "CLOSE \n<<");
	}
	else if (!show&&ShowingExhausts) {
		SetWindowPos(hDlg, HWND_TOP, rect.left, rect.top, currwwidth - 500, currwheight, 0);
		ShowingExhausts = false;
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_TOGEXHWIN), "EXHAUSTS AND STREAMS \n>>");
	}
	else {
		return;
	}
	return;
}
void DialogControl::UpdateExhaustWin(HWND hWnd, UINT idx) {


	SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < VB1->ExTMng->GetExTexCount(); i++) {
		char ebuf[256] = { '\0' };
		sprintf(ebuf, "%s", ExTMng->GetExTexName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_INSERTSTRING, i, (LPARAM)ebuf);
		SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETITEMDATA, i, (LPARAM)VB1->ExTMng->GetExTexSurf(i));
	}
	SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_INSERTSTRING, 0, (LPARAM)"DEFAULT");
	SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETITEMDATA, 0, (LPARAM)NULL);
	SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETCURSEL, 0, 0);

	SendDlgItemMessage(hWnd, IDC_CHECK_THCUSPOSDIR, BM_SETCHECK, BST_UNCHECKED, 0);
	int edits[6] = { IDC_EDIT_THEXPOSX,IDC_EDIT_THEXPOSY ,IDC_EDIT_THEXPOSZ,IDC_EDIT_THEXDIRX,IDC_EDIT_THEXDIRY,IDC_EDIT_THEXDIRZ };
	for (UINT i = 0; i < 6; i++) {
		SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, true, 0);
	}

	SendDlgItemMessage(hWnd, IDC_LIST_THEXS, LB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < ThrMng->GetThrExCount(idx); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "Exhaust %i", i);
		SendDlgItemMessage(hWnd, IDC_LIST_THEXS, LB_INSERTSTRING, i, (LPARAM)cbuf);
	}

	
	SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < PartMng->GetParticleDefCount(); i++) {
		SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_INSERTSTRING, i, (LPARAM)PartMng->GetParticleDefName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_SETITEMDATA, i, (LPARAM)PartMng->GetParticleDefID(i));
	}
	SendDlgItemMessage(hWnd, IDC_CHECK_THPARTCUS, BM_SETCHECK, BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_EDIT_THPARTX, EM_SETREADONLY, true, 0);
	SendDlgItemMessage(hWnd, IDC_EDIT_THPARTY, EM_SETREADONLY, true, 0);
	SendDlgItemMessage(hWnd, IDC_EDIT_THPARTZ, EM_SETREADONLY, true, 0);
		

	SendDlgItemMessage(hWnd, IDC_LIST_THPARTADDED, LB_RESETCONTENT, 0, 0);
	for (UINT i = 0; i < ThrMng->GetThrParticlesCount(idx); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "Stream %i", i);
		SendDlgItemMessage(hWnd, IDC_LIST_THPARTADDED, LB_INSERTSTRING, i, (LPARAM)cbuf);
	}
	return;
}
void DialogControl::UpdateThrDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= ThrMng->GetThrCount()) { return; }
	char cbuf[256] = { '\0' };
	sprintf(cbuf, ThrMng->GetThrName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_THNAME, cbuf);

	VECTOR3 pos = ThrMng->GetThrPos(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_THPOSX, IDC_EDIT_THPOSY, IDC_EDIT_THPOSZ, pos);
	VECTOR3 dir = ThrMng->GetThrDir(idx);
	SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, dir);

	double max0 = ThrMng->GetThrMax0(idx);
	sprintf(cbuf, "%.2f", max0);
	SetDlgItemText(hWnd, IDC_EDIT_THMAX, cbuf);
	double isp = ThrMng->GetThrIsp0(idx);
	sprintf(cbuf, "%.2f", isp);
	SetDlgItemText(hWnd, IDC_EDIT_THISP, cbuf);
	double ispref = ThrMng->GetThrIspRef(idx);
	sprintf(cbuf, "%.2f", ispref);
	SetDlgItemText(hWnd, IDC_EDIT_THISPREF, cbuf);
	double pref = ThrMng->GetThrPRef(idx);
	sprintf(cbuf, "%.2f", pref);
	SetDlgItemText(hWnd, IDC_EDIT_THPREF, cbuf);

	SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_RESETCONTENT, 0, 0);
	int index = SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_ADDSTRING, 0, (LPARAM)"NONE");
	SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETITEMDATA, index, (LPARAM)NULL);
	int topick = -1;
	for (UINT i = 0; i < PrpMng->GetPropCount(); i++) {
		index = SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_ADDSTRING, 0, (LPARAM)PrpMng->GetTankName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETITEMDATA, index, (LPARAM)PrpMng->GetTankPH(i));
		if (PrpMng->GetTankPH(i) == ThrMng->GetThrPh(idx)) {
			topick = index;
		}
	}
	if (topick != -1) {
		SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETCURSEL, topick, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_SETCURSEL, 0, 0);
	}

	UpdateExhaustWin(hWnd, idx);
	

	

	if (ThrMng->ThrIsTesting(idx)) {
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_THTEST), "STOP TEST");
	}
	else {
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_THTEST), "TEST THRUSTER");
	}

	if (ThLaserMap[idx] != NULL) {
		SendDlgItemMessage(hWnd, IDC_CHECK_THSHOW, BM_SETCHECK, BST_CHECKED, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_THSHOW, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	return;
}
BOOL DialogControl::ThrDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_THPOSY, UDM_SETRANGE32, -10000, 10000);
		SendDlgItemMessage(hWnd, IDC_SPIN_THPOSZ, UDM_SETRANGE32, -10000, 10000);
		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_THNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_THNAME, cbuf, 256);
			string newname(cbuf);
			ThrMng->SetThrName(idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);

			break;
		}
		case IDC_BUTTON_THPOSSET:
		{
			VECTOR3 newpos = GetDlgItemsVector3(hWnd, IDC_EDIT_THPOSX, IDC_EDIT_THPOSY, IDC_EDIT_THPOSZ);
			ThrMng->SetThrPos(idx, newpos);
			break;
		}
		case IDC_BUTTON_THDIRSET:
		{
			VECTOR3 newdir = GetDlgItemsVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ);
			ThrMng->SetThrDir(idx, newdir);
			break;
		}
		case IDC_BUTTON_THMAXSET:
		{
			double newmax = GetDlgItemDouble(hWnd, IDC_EDIT_THMAX);
			ThrMng->SetThrMax0(idx, newmax);
			break;
		}
		case IDC_BUTTON_THISPSET:
		{
			double newisp = GetDlgItemDouble(hWnd, IDC_EDIT_THISP);
			ThrMng->SetThrIsp(idx, newisp);
			break;
		}
		case IDC_BUTTON_THISPREFSET:
		{
			double newispref = GetDlgItemDouble(hWnd, IDC_EDIT_THISPREF);
			ThrMng->SetThrIspRef(idx, newispref);
			break;
		}
		case IDC_BUTTON_THPREFSET:
		{
			double newpref = GetDlgItemDouble(hWnd, IDC_EDIT_THPREF);
			ThrMng->SetThrPRef(idx, newpref);
			break;
		}
		case IDC_BUTTON_DELETETH:
		{
			if (ThLaserMap[idx] != NULL) {
				VB1->Laser->DeleteLaser(ThLaserMap[idx]);
			}
			ThrMng->DelThrDef(idx);
			UpdateTree(hDlg, THRUSTERS, CurrentSelection.config);
			break;
		}
		case IDC_BTN_THPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_THPOSX, IDC_EDIT_THPOSY, IDC_EDIT_THPOSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, VB1->vclip.dir);
			}
			break;
		}
		case IDC_BUTTON_THXP:
		{
			VECTOR3 xp = _V(1, 0, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, xp);
			break;
		}
		case IDC_BUTTON_THXM:
		{
			VECTOR3 xm = _V(-1, 0, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, xm);
			break;
		}
		case IDC_BUTTON_THYP:
		{
			VECTOR3 yp = _V(0, 1, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, yp);
			break;
		}
		case IDC_BUTTON_THYM:
		{
			VECTOR3 ym = _V(0, -1, 0);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, ym);
			break;
		}
		case IDC_BUTTON_THZP:
		{
			VECTOR3 zp = _V(0, 0, 1);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, zp);
			break;
		}
		case IDC_BUTTON_THZM:
		{
			VECTOR3 zm = _V(0, 0, -1);
			SetDlgItemsTextVector3(hWnd, IDC_EDIT_THDIRX, IDC_EDIT_THDIRY, IDC_EDIT_THDIRZ, zm);
			break;
		}
		case IDC_COMBO_THPH:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_GETCURSEL, 0, 0);
				PROPELLANT_HANDLE ph = (PROPELLANT_HANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_THPH, CB_GETITEMDATA, index, 0);
				ThrMng->SetThrPH(idx, ph);
			}
			break;
		}
		case IDC_CHECK_THSHOW:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_THSHOW, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					ThLaserMap[idx] = VB1->Laser->CreateLaser(ThrMng->GetThrPosPtr(idx), ThrMng->GetThrDirPtr(idx), LASER_GREEN_TEX);
					//VB1->CreateThExhausts();
				}
				else {
					VB1->Laser->DeleteLaser(ThLaserMap[idx]);
					//VB1->DeleteThExhausts();
				}
			}
			break;
		}
		case IDC_BUTTON_THTEST:
		{
			ThrMng->ToggleThrusterTest(idx);
			if (ThrMng->ThrIsTesting(idx)) {
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_THTEST), "STOP TEST");
			}
			else {
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_THTEST), "TEST THRUSTER");
			}
			
			break;
		}
		case IDC_BUTTON_TOGEXHWIN:
		{
			ShowExhaustsWin(hWnd, !ShowingExhausts);
			break;
		}
		case IDC_LIST_THEXS:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_LIST_THEXS, LB_GETCURSEL, 0, 0);
				if ((index >= 0)&&(index<ThrMng->GetThrExCount(idx))) {
					double lsize, wsize;
					SURFHANDLE tex;
					bool customposdir;
					VECTOR3 pos, dir;
					ThrMng->GetThrExParams(idx, index, lsize, wsize, tex, customposdir, pos, dir);
					SetDlgItemDouble(hWnd, IDC_EDIT_THEXL, lsize, 2);
					SetDlgItemDouble(hWnd, IDC_EDIT_THEXW, wsize, 2);
					if (tex != NULL) {
						int extex = ExTMng->GetExTexIdx(tex);
						SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETCURSEL, extex + 1, 0);
					}
					else {
						SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETCURSEL, 0, 0);
					}
					int edits[6] = { IDC_EDIT_THEXPOSX , IDC_EDIT_THEXPOSY ,IDC_EDIT_THEXPOSZ ,IDC_EDIT_THEXDIRX,IDC_EDIT_THEXDIRY,IDC_EDIT_THEXDIRZ };
					if (customposdir) {
						SendDlgItemMessage(hWnd, IDC_CHECK_THCUSPOSDIR, BM_SETCHECK, BST_CHECKED, 0);
						for (UINT i = 0; i < 6; i++) {
							SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, false, 0);
						}
						SetDlgItemsTextVector3(hWnd, IDC_EDIT_THEXPOSX, IDC_EDIT_THEXPOSY, IDC_EDIT_THEXPOSZ, pos);
						SetDlgItemsTextVector3(hWnd, IDC_EDIT_THEXDIRX, IDC_EDIT_THEXDIRY, IDC_EDIT_THEXDIRZ, dir);
					}
					else {
						SendDlgItemMessage(hWnd, IDC_CHECK_THCUSPOSDIR, BM_SETCHECK, BST_UNCHECKED, 0);
						for (UINT i = 0; i < 6; i++) {
							SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, true, 0);
							SetDlgItemText(hWnd, edits[i], "");
						}
					}
				}
			}
			break;
		}
		case IDC_LIST_THPARTADDED:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_LIST_THPARTADDED, LB_GETCURSEL, 0, 0);
				if ((index >= 0) && (index < ThrMng->GetThrParticlesCount(idx))) {
					UINT pss_index;
					bool custompos;
					VECTOR3 pos;
					ThrMng->GetThrParticleParams(idx, index, pss_index, custompos, pos);
					SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_SETCURSEL, pss_index, 0);
					int edits[3] = { IDC_EDIT_THPARTX,IDC_EDIT_THPARTY,IDC_EDIT_THPARTZ };
					if (custompos) {
						for (UINT i = 0; i < 3; i++) {
							SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, false, 0);
						}
						SetDlgItemsTextVector3(hWnd, IDC_EDIT_THPARTX, IDC_EDIT_THPARTY, IDC_EDIT_THPARTZ, pos);
						SendDlgItemMessage(hWnd, IDC_CHECK_THPARTCUS, BM_SETCHECK, BST_CHECKED, 0);
					}
					else {
						for (UINT i = 0; i < 3; i++) {
							SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, true, 0);
							SetDlgItemText(hWnd, edits[i], "");
						}
						SendDlgItemMessage(hWnd, IDC_CHECK_THPARTCUS, BM_SETCHECK, BST_UNCHECKED, 0);
					}
				}
			}
			break;
		}
		case IDC_BUTTON_THADDEX:
		{
			double lsize = GetDlgItemDouble(hWnd, IDC_EDIT_THEXL);
			double wsize = GetDlgItemDouble(hWnd, IDC_EDIT_THEXW);
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_GETCURSEL, 0, 0);
			SURFHANDLE tex = (SURFHANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_GETITEMDATA, index, 0);
			
			bool customposdir = IsCheckBoxChecked(hWnd, IDC_CHECK_THCUSPOSDIR);
			VECTOR3 pos, dir;
			if (customposdir) {
				pos = GetDlgItemsVector3(hWnd, IDC_EDIT_THEXPOSX, IDC_EDIT_THEXPOSY, IDC_EDIT_THEXPOSZ);
				dir = GetDlgItemsVector3(hWnd, IDC_EDIT_THEXDIRX, IDC_EDIT_THEXDIRY, IDC_EDIT_THEXDIRZ);
			}
			ThrMng->AddThrExhaust(idx, lsize, wsize, tex, customposdir, pos, dir);
			UpdateExhaustWin(hWnd, idx);
			break;
		}
		case IDC_BUTTON_THREMEX:
		{
			int index = SendDlgItemMessage(hWnd, IDC_LIST_THEXS, LB_GETCURSEL, 0, 0);
			if (index >= 0) {
				ThrMng->RemoveThrExhaust(idx, index);
			}
			UpdateExhaustWin(hWnd, idx);
			break;
		}
		case IDC_CHECK_THCUSPOSDIR:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_THCUSPOSDIR);
				int edits[6] = { IDC_EDIT_THEXPOSX,IDC_EDIT_THEXPOSY, IDC_EDIT_THEXPOSZ,IDC_EDIT_THEXDIRX,IDC_EDIT_THEXDIRY,IDC_EDIT_THEXDIRZ };
				for (UINT i = 0; i < 6; i++) {
					SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY,check?false:true,0);
				}
				
			}
			break;
		}
		case IDC_BUTTON_THPARTADD:
		{
			int index = SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_GETCURSEL, 0, 0);
			if (index >= 0) {
				bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_THPARTCUS);
				VECTOR3 pos = _V(0, 0, 0);
				if (check) {
					pos = GetDlgItemsVector3(hWnd, IDC_EDIT_THPARTX, IDC_EDIT_THPARTY, IDC_EDIT_THPARTZ);
				}
				ThrMng->AddThrParticle(idx, index, check, pos);
				UpdateExhaustWin(hWnd,idx);
			}
			break;
		}
		case IDC_CHECK_THPARTCUS:
		{
			int edits[3] = { IDC_EDIT_THPARTX, IDC_EDIT_THPARTY, IDC_EDIT_THPARTZ };
			bool check = IsCheckBoxChecked(hWnd, IDC_CHECK_THPARTCUS);
			for (UINT i = 0; i < 3; i++) {
				SendDlgItemMessage(hWnd, edits[i], EM_SETREADONLY, check ? false : true, 0);
			}
			break;
		}
		case IDC_BUTTON_THPARTREM:
		{
			int index = SendDlgItemMessage(hWnd, IDC_LIST_THPARTADDED, LB_GETCURSEL, 0, 0);
			if (index >= 0) {
				ThrMng->RemoveThrParticle(idx, index);
				UpdateExhaustWin(hWnd, idx);
			}
			break;
		}
		case IDC_BUTTON_THEXPASTEV:
		{
			if (VB1->vclip.valid) {
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_THEXPOSX, IDC_EDIT_THEXPOSY, IDC_EDIT_THEXPOSZ, VB1->vclip.pos);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_THEXDIRX, IDC_EDIT_THEXDIRY, IDC_EDIT_THEXDIRZ, VB1->vclip.dir);
				SetDlgItemsTextVector3(hWnd, IDC_EDIT_THPARTX, IDC_EDIT_THPARTY, IDC_EDIT_THPARTZ, VB1->vclip.pos);
			}
			break;
		}
		}
		break;
	}
	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SPIN_THPOSX:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.15;
				VECTOR3 pos = ThrMng->GetThrPos(CurrentSelection.idx);
				pos.x += spd*(double)lpd->iDelta;
				ThrMng->SetThrPos(CurrentSelection.idx, pos);
				UpdateThrDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_THPOSY:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.15;
				VECTOR3 pos = ThrMng->GetThrPos(CurrentSelection.idx);
				pos.y += spd*(double)lpd->iDelta;
				ThrMng->SetThrPos(CurrentSelection.idx, pos);
				UpdateThrDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETPOS, 0, 0);
			}
			break;
		}
		case IDC_SPIN_THPOSZ:
		{
			if (((LPNMHDR)lParam)->code == UDN_DELTAPOS)
			{
				LPNMUPDOWN lpd = (LPNMUPDOWN)lParam;
				double spd = 0.15;
				VECTOR3 pos = ThrMng->GetThrPos(CurrentSelection.idx);
				pos.z += spd*(double)lpd->iDelta;
				ThrMng->SetThrPos(CurrentSelection.idx, pos);
				UpdateThrDialog(hWnd);
				SendDlgItemMessage(hWnd, IDC_SPIN_THPOSX, UDM_SETPOS, 0, 0);
			}
			break;
		}

		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}