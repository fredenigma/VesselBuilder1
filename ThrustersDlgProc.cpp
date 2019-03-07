#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "PropellantManager.h"
#include "ParticleManager.h"
#include "ExTexManager.h"
#include "ThrusterManager.h"
#include "LaserManager.h"
#pragma comment(lib, "comctl32.lib")

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

	if (ThrMng->ThrHasExhaust(idx)) {
		SendDlgItemMessage(hWnd, IDC_CHECK_THEX, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_THEXL, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_THEXW, EM_SETREADONLY, false, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_THEXTEX), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_THEXLSET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_THEXWSET), true);
		SendDlgItemMessage(hWnd, IDC_CHECK_THEX, BM_SETCHECK, BST_CHECKED, 0);
		sprintf(cbuf, "%.1f", ThrMng->GetThrExhaustLength(idx));
		SetDlgItemText(hWnd, IDC_EDIT_THEXL, cbuf);
		sprintf(cbuf, "%.1f", ThrMng->GetThrExhaustWidth(idx));
		SetDlgItemText(hWnd, IDC_EDIT_THEXW, cbuf);

		SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_ADDSTRING, 0, (LPARAM)"DEFAULT");
		SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETITEMDATA, index, (LPARAM)NULL);
		int topick2 = -1;
		for (UINT i = 0; i <ExTMng->GetExTexCount(); i++) {
			if (!ExTMng->IsExTexCreated(i)) { continue; }
			index = SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_ADDSTRING, 0, (LPARAM)ExTMng->GetExTexName(i).c_str());
			SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETITEMDATA, index, (LPARAM)ExTMng->GetExTexSurf(i));
			if (ThrMng->ThrHasExhaust(idx)) {
				if (ThrMng->GetThrExhaustTex(idx) == ExTMng->GetExTexSurf(i)) {
					topick2 = index;
				}
			}
		}
		if (topick2 != -1) {
			SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETCURSEL, topick2, 0);
		}
		else {
			SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_SETCURSEL, 0, 0);
		}
		
	}
	else {
		SendDlgItemMessage(hWnd, IDC_CHECK_THEX, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_THEXL, EM_SETREADONLY, true, 0);
		SetDlgItemText(hWnd, IDC_EDIT_THEXL, "");
		SendDlgItemMessage(hWnd, IDC_EDIT_THEXW, EM_SETREADONLY, true, 0);
		SetDlgItemText(hWnd, IDC_EDIT_THEXW, "");
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_THEXTEX), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_THEXLSET), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_THEXWSET), false);
	}

	SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_RESETCONTENT, 0, 0);
	//sprintf(oapiDebugString(), "time:%.3f", oapiGetSimTime());
	for (UINT i = 0; i < PartMng->GetParticleDefCount(); i++) {
		SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_INSERTSTRING, i, (LPARAM)PartMng->GetParticleDefName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_SETITEMDATA, i, (LPARAM)PartMng->GetParticleDefID(i));
		SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_SETSEL, false, i);
		vector<UINT> p_id = ThrMng->GetThrParticlesIDs(idx);
		for (UINT j = 0; j < p_id.size(); j++) {
			if (p_id[j] == PartMng->GetParticleDefID(i)) {
				SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_SETSEL, true, i);
			}
		}
	}





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
			UpdateTree(hDlg, THRUSTERS, 0);
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
					ThLaserMap[idx] = VB1->Laser->CreateLaser(ThrMng->GetThrPosPtr(idx), ThrMng->GetThrAntiDirPtr(idx), LASER_GREEN_TEX);
					//VB1->CreateThExhausts();
				}
				else {
					VB1->Laser->DeleteLaser(ThLaserMap[idx]);
					//VB1->DeleteThExhausts();
				}
			}
			break;
		}
		case IDC_CHECK_THEX:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				LRESULT getcheck = SendDlgItemMessage(hWnd, IDC_CHECK_THEX, BM_GETCHECK, 0, 0);
				if (getcheck == BST_CHECKED) {
					ThrMng->SetThrHasExhaust(idx, true);
					UpdateThrDialog(hWnd);
				}
				else {
					ThrMng->SetThrHasExhaust(idx, false);
					UpdateThrDialog(hWnd);
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
		case IDC_BUTTON_THEXLSET:
		{
			double newlength = GetDlgItemDouble(hWnd, IDC_EDIT_THEXL);
			ThrMng->SetThrExhaustLength(idx, newlength);
			break;
		}
		case IDC_BUTTON_THEXWSET:
		{
			double newwidth = GetDlgItemDouble(hWnd, IDC_EDIT_THEXW);
			ThrMng->SetThrExhaustWidth(idx, newwidth);
			break;
		}
		case IDC_COMBO_THEXTEX:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_GETCURSEL, 0, 0);
				SURFHANDLE surf= (SURFHANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_THEXTEX, CB_GETITEMDATA, index, 0);
				ThrMng->SetThrExhaustTex(idx, surf);
			}
			break;
		}
		case IDC_BUTTON_THPARTSET:
		{
			ThrMng->ClearThrParticles(idx);
			int nsel = SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_GETSELCOUNT, 0, 0);
			if (nsel <= 0) { break; }
			UINT *sellist = new UINT[nsel];
			SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_GETSELITEMS, nsel, (LPARAM)sellist);
			vector<UINT> _ids;
			for (UINT i = 0; i < nsel; i++) {
				UINT _id = (UINT)SendDlgItemMessage(hWnd, IDC_LIST_THPART, LB_GETITEMDATA, (WPARAM)sellist[i], 0);
				_ids.push_back(_id);
			}
			ThrMng->SetThrParticles(idx, _ids);
			delete[] sellist;
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