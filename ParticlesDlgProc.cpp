#include "VesselBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "ExTexManager.h"
#include "ParticleManager.h"
#pragma comment(lib, "comctl32.lib")

void DialogControl::UpdatePartDialog(HWND hWnd) {
	UINT idx = CurrentSelection.idx;
	if (idx >= PartMng->GetParticleDefCount()) { return; }
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%s", PartMng->GetParticleDefName(idx).c_str());
	SetDlgItemText(hWnd, IDC_EDIT_PARTNAME, cbuf);
	PARTICLESTREAMSPEC pss = PartMng->GetPArticleDefSpecs(idx);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTSIZE, pss.srcsize, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTRATE, pss.srcrate, 0);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTV0, pss.v0, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTSPREAD, pss.srcspread, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTLIFETIME, pss.lifetime, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTGROWTH, pss.growthrate, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTATMSLOW, pss.atmslowdown, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTLMIN, pss.lmin, 3);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTLMAX, pss.lmax, 1);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTAMIN, pss.amin, 3);
	SetDlgItemDouble(hWnd, IDC_EDIT_PARTAMAX, pss.amax, 1);
	int indx = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_PARTLTYPE), (void*)pss.ltype);
	SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_SETCURSEL, indx, 0);
	indx = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_PARTLVLMAP), (void*)pss.levelmap);
	SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_SETCURSEL, indx, 0);
	indx = ComboFindItemData(GetDlgItem(hWnd, IDC_COMBO_PARTATMMAP), (void*)pss.atmsmap);
	SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_SETCURSEL, indx, 0);


	SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_RESETCONTENT, 0, 0);
	int index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_ADDSTRING, 0, (LPARAM)"DEFAULT");
	SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_SETITEMDATA, index, (LPARAM)NULL);
	int topick = -1;
	for (UINT i = 0; i <ExTMng->GetExTexCount(); i++) {
		if (!ExTMng->IsExTexCreated(i)) { continue; }
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_ADDSTRING, 0, (LPARAM)ExTMng->GetExTexName(i).c_str());
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_SETITEMDATA, index, (LPARAM)ExTMng->GetExTexSurf(i));
		if (pss.tex == ExTMng->GetExTexSurf(i)) {
			topick = index;
		}
	}
	if (topick != -1) {
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_SETCURSEL, topick, 0);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_SETCURSEL, 0, 0);
	}

	if ((idx == 0) || (idx == 1)) {
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTNAME, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTSIZE, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTRATE, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTV0, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTSPREAD, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTLIFETIME, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTGROWTH, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTATMSLOW, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTLMIN, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTLMAX, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTAMIN, EM_SETREADONLY, true, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTAMAX, EM_SETREADONLY, true, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_PARTNAMESET), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_PARTSTORE), false);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_DELETEPART), false);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTLTYPE), false);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTLVLMAP), false);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTATMMAP), false);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTTEX), false);
	}
	else {
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTNAME, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTSIZE, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTRATE, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTV0, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTSPREAD, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTLIFETIME, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTGROWTH, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTATMSLOW, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTLMIN, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTLMAX, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTAMIN, EM_SETREADONLY, false, 0);
		SendDlgItemMessage(hWnd, IDC_EDIT_PARTAMAX, EM_SETREADONLY, false, 0);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_PARTNAMESET), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_PARTSTORE), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_DELETEPART), true);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTLTYPE), true);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTLVLMAP), true);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTATMMAP), true);
		EnableWindow(GetDlgItem(hWnd, IDC_COMBO_PARTTEX), true);
	}
	
	if (PartMng->IsParticleTesting(idx)) {
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_PARTTEST), "STOP TESTING");
	}
	else {
		SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_PARTTEST), "TEST PARTICLE");
	}


	return;
}
BOOL DialogControl::PartDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_RESETCONTENT, 0, 0);
		int index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_ADDSTRING, 0, (LPARAM)"EMISSIVE");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::EMISSIVE);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_ADDSTRING, 0, (LPARAM)"DIFFUSE");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::DIFFUSE);

		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_RESETCONTENT, 0, 0);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_ADDSTRING, 0, (LPARAM)"LVL_FLAT");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::LVL_FLAT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_ADDSTRING, 0, (LPARAM)"LVL_LIN");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::LVL_LIN);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_ADDSTRING, 0, (LPARAM)"LVL_SQRT");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::LVL_SQRT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_ADDSTRING, 0, (LPARAM)"LVL_PLIN");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::LVL_PLIN);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_ADDSTRING, 0, (LPARAM)"LVL_PSQRT");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::LVL_PSQRT);

		SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_RESETCONTENT, 0, 0);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_ADDSTRING, 0, (LPARAM)"ATM_FLAT");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::ATM_FLAT);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_ADDSTRING, 0, (LPARAM)"ATM_PLIN");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::ATM_PLIN);
		index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_ADDSTRING, 0, (LPARAM)"ATM_PLOG");
		SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_SETITEMDATA, index, (LPARAM)PARTICLESTREAMSPEC::ATM_PLOG);

		break;
	}
	case WM_COMMAND:
	{
		UINT idx = CurrentSelection.idx;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_PARTNAMESET:
		{
			char cbuf[256] = { '\0' };
			GetDlgItemText(hWnd, IDC_EDIT_PARTNAME, cbuf, 256);
			string newname(cbuf);
			PartMng->SetParticleDefName(idx, newname);

			TVITEM tvi;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPSTR)cbuf;
			tvi.cchTextMax = strlen(cbuf);
			tvi.hItem = CurrentSelection.hitem;
			SendDlgItemMessage(hDlg, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)&tvi);

			break;
		}
		case IDC_BUTTON_DELETEPART:
		{
			PartMng->DeleteParticleDef(idx);
			UpdateTree(hDlg, PARTICLES, CurrentSelection.config);
			break;
		}
		case IDC_BUTTON_PARTSTORE:
		{
			PARTICLESTREAMSPEC pss = PartMng->GetPArticleDefSpecs(idx);
			pss.srcsize = GetDlgItemDouble(hWnd, IDC_EDIT_PARTSIZE);
			pss.srcrate = GetDlgItemDouble(hWnd, IDC_EDIT_PARTRATE);
			pss.v0 = GetDlgItemDouble(hWnd, IDC_EDIT_PARTV0);
			pss.srcspread = GetDlgItemDouble(hWnd, IDC_EDIT_PARTSPREAD);
			pss.lifetime = GetDlgItemDouble(hWnd, IDC_EDIT_PARTLIFETIME);
			pss.growthrate = GetDlgItemDouble(hWnd, IDC_EDIT_PARTGROWTH);
			pss.atmslowdown = GetDlgItemDouble(hWnd, IDC_EDIT_PARTATMSLOW);
			pss.lmin = GetDlgItemDouble(hWnd, IDC_EDIT_PARTLMIN);
			pss.lmax = GetDlgItemDouble(hWnd, IDC_EDIT_PARTLMAX);
			pss.amin = GetDlgItemDouble(hWnd, IDC_EDIT_PARTAMIN);
			pss.amax = GetDlgItemDouble(hWnd, IDC_EDIT_PARTAMAX);
			int index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_GETCURSEL, 0, 0);
			pss.ltype = (PARTICLESTREAMSPEC::LTYPE)SendDlgItemMessage(hWnd, IDC_COMBO_PARTLTYPE, CB_GETITEMDATA, index, 0);
			index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_GETCURSEL, 0, 0);
			pss.levelmap = (PARTICLESTREAMSPEC::LEVELMAP)SendDlgItemMessage(hWnd, IDC_COMBO_PARTLVLMAP, CB_GETITEMDATA, index, 0);
			index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_GETCURSEL, 0, 0);
			pss.atmsmap = (PARTICLESTREAMSPEC::ATMSMAP)SendDlgItemMessage(hWnd, IDC_COMBO_PARTATMMAP, CB_GETITEMDATA, index, 0);
			index = SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_GETCURSEL, 0, 0);
			pss.tex = (SURFHANDLE)SendDlgItemMessage(hWnd, IDC_COMBO_PARTTEX, CB_GETITEMDATA, index, 0);
			
			PartMng->SetParticleDefSpecs(idx, pss);

			break;
		}
		case IDC_BUTTON_PARTTEST:
		{
			PartMng->ToggleParticleTesting(idx);
			if (PartMng->IsParticleTesting(idx)) {
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_PARTTEST), "STOP TESTING");
			}
			else {
				SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_PARTTEST), "TEST PARTICLE");
			}
			break;
		}
		}
		break;
	}
	}
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}