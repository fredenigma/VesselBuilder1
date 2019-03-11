#include "VesselBuilder1.h"
#include "resource.h"
#include "LaserManager.h"
#include "DialogControl.h"
#include "TextReaderDlg.h"
#include "MeshManager.h"
#include "DockManager.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"
#include "PropellantManager.h"
#include "ParticleManager.h"
#include "ThrusterManager.h"
#include "TouchdownPointsManager.h"
#include "AirfoilsManager.h"
#include "ControlSurfacesManager.h"
#include "CameraManager.h"
#include "ExTexManager.h"
#include "VCManager.h"
#include "LightsManager.h"
#include "VariableDragManager.h"
#pragma comment(lib, "comctl32.lib")



extern HINSTANCE hDLL;
//extern void RotateMeshClbk(MESHHANDLE, bool);

//Hooks...
BOOL CALLBACK DlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DialogControl *DlgCtrl = (uMsg == WM_INITDIALOG ? (DialogControl*)lParam : (DialogControl*)oapiGetDialogContext(hWnd));
	return DlgCtrl->DlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK MeshDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->MeshDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK DockDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->DockDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK AnimDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->AnimDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK AnimCompDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->AnimCompDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK AttDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->AttDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK PrpDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->PrpDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK ExTexDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->ExTexDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK PartDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->PartDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK ThrDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->ThrDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK ThrGrpDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->ThrGrpDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK TdpDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->TdpDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK SettingsDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->SettingsDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK AirfoilsDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->AirfoilsDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK CtrlSurfDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->CtrlSurfDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK CamDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->CamDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK VCPosDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->VCPosDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK VCHUDDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->VCHUDDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK VCMFDDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->VCMFDDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK BeaconsDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->BeaconsDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK LightCreationDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->LightCreationDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK LightsDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->LightsDlgProc(hWnd, uMsg, wParam, lParam);
}
BOOL CALLBACK VarDragDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->VarDragDlgProc(hWnd, uMsg, wParam, lParam);
}
DialogControl::DialogControl(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	MshMng = VB1->MshMng;
	DckMng = VB1->DckMng;
	AnimMng = VB1->AnimMng;
	AttMng = VB1->AttMng;
	PrpMng = VB1->PrpMng;
	ThrMng = VB1->ThrMng;
	ThrGrpMng = VB1->ThrGrpMng;
	PartMng = VB1->PartMng;
	TdpMng = VB1->TdpMng;
	AirfoilMng = VB1->AirfoilMng;
	CtrSurfMng = VB1->CtrSurfMng;
	CamMng = VB1->CamMng;
	ExTMng = VB1->ExTMng;
	VCMng = VB1->VCMng;
	LightsMng = VB1->LightsMng;
	VardMng = VB1->VardMng;

	open = false;
	hDlg = NULL;
	TreeItem.clear();
	CurrentSelection = TREE_ITEM_REF();
	speedrotation = 1*RAD;
	test = 0;
	InitOapiKeys();
	AnimTesting = false;
	ShowingAFGraph = false;
	ShowingExhausts = false;
	penblack = CreatePen(PS_SOLID,2,RGB(0,0,0));
	pengray = CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
	penblue_l = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));

//	ItemToSelect = NULL;

}
DialogControl::~DialogControl() {
	VB1 = NULL;
	MshMng = NULL;
	DckMng = NULL;
	AnimMng = NULL;
	AttMng = NULL;
	PrpMng = NULL;
	ThrMng = NULL;
	ThrGrpMng = NULL;
	PartMng = NULL;
	TdpMng = NULL;
	AirfoilMng = NULL;
	CtrSurfMng = NULL;
	CamMng=NULL;
	ExTMng = NULL; 
	VCMng = NULL;
	LightsMng = NULL;
	VardMng = NULL;
	hDlg = NULL;
	DeleteObject(penblack);
	DeleteObject(pengray);
	DeleteObject(penblue_l);
	open = false;
}
void DialogControl::Open(HINSTANCE hDLL) {
	hDlg = oapiOpenDialogEx(hDLL, IDD_DIALOG1, DlgProcHook, DLG_CAPTIONHELP|DLG_CAPTIONCLOSE, this);
	
	open = true;
}
void DialogControl::Close() {
	open = false;
	VB1->MshMng->ResetHighlights();
	ClearLasers(DockLaserMap);
	ClearLasers(AttLaserMap);
	ClearLasers(ThLaserMap);
	ClearLasers(TdpLaserMap);
	

	oapiCloseDialog(hDlg);
	hDlg = NULL;
}
bool DialogControl::IsOpen() {
	return open;
}

void DialogControl::InitDialog(HWND hWnd) {
	InitTree(hWnd);
	ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_HIDE);
	ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
	MshMng->md_restore = MshMng->GetAllDefs();
	hwnd_Mesh = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_MESH), hWnd, MeshDlgProcHook, (LPARAM)this);
	hWnd_Dock = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_DOCK), hWnd, DockDlgProcHook, (LPARAM)this);
	hWnd_Anim = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_ANIM), hWnd, AnimDlgProcHook, (LPARAM)this);
	hWnd_AnimComp = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_ANIMCOMP), hWnd, AnimCompDlgProcHook, (LPARAM)this);
	hWnd_Atts = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_ATT), hWnd, AttDlgProcHook, (LPARAM)this);
	hWnd_Prp = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_PROPELLANT), hWnd, PrpDlgProcHook, (LPARAM)this);
	hWnd_ExTex = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_EXTEX), hWnd, ExTexDlgProcHook, (LPARAM)this);
	hWnd_Part = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_PARTICLES), hWnd, PartDlgProcHook, (LPARAM)this);
	hWnd_Thr = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_THRUSTERS), hWnd, ThrDlgProcHook, (LPARAM)this);
	hWnd_ThrGrp = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_THRGRP), hWnd, ThrGrpDlgProcHook, (LPARAM)this);
	hWnd_Tdp = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_TDPOINTS), hWnd, TdpDlgProcHook, (LPARAM)this);
	hWnd_Settings = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_SETTINGS), hWnd, SettingsDlgProcHook, (LPARAM)this);
	hWnd_Airfoils = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_AIRFOILS), hWnd, AirfoilsDlgProcHook, (LPARAM)this);
	hWnd_CtrlSurfaces = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_CTRLSURF), hWnd, CtrlSurfDlgProcHook, (LPARAM)this);
	hWnd_Cam = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_CAMERA), hWnd, CamDlgProcHook, (LPARAM)this);
	hWnd_VCPos = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_VCPOS), hWnd, VCPosDlgProcHook, (LPARAM)this);
	hWnd_VCHud = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_VCHUD), hWnd, VCHUDDlgProcHook, (LPARAM)this);
	hWnd_VCMFD = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_VCMFD), hWnd, VCMFDDlgProcHook, (LPARAM)this);
	hWnd_Beacons = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_BEACONS), hWnd, BeaconsDlgProcHook, (LPARAM)this);
	hWnd_LightCreation = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_LIGHTCREATION), hWnd, LightCreationDlgProcHook, (LPARAM)this);
	hWnd_Lights= CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_LIGHTS), hWnd, LightsDlgProcHook, (LPARAM)this);
	hWnd_VarDrag = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_VARDRAG), hWnd, VarDragDlgProcHook, (LPARAM)this);
	SetWindowPos(hwnd_Mesh, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hwnd_Mesh, SW_HIDE);
	SetWindowPos(hWnd_Dock, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Dock, SW_HIDE);
	SetWindowPos(hWnd_Anim, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Anim, SW_HIDE);
	SetWindowPos(hWnd_AnimComp, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_AnimComp, SW_HIDE);
	SetWindowPos(hWnd_Atts, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Atts, SW_HIDE);
	SetWindowPos(hWnd_Prp, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Prp, SW_HIDE);
	SetWindowPos(hWnd_ExTex, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_ExTex, SW_HIDE);
	SetWindowPos(hWnd_Part, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Part, SW_HIDE);
	SetWindowPos(hWnd_Thr, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Thr, SW_HIDE);
	SetWindowPos(hWnd_ThrGrp, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_ThrGrp, SW_HIDE);
	SetWindowPos(hWnd_Tdp, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Tdp, SW_HIDE);
	SetWindowPos(hWnd_Settings, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Settings, SW_HIDE);
	SetWindowPos(hWnd_Airfoils, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Airfoils, SW_HIDE);
	SetWindowPos(hWnd_CtrlSurfaces, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_CtrlSurfaces, SW_HIDE);
	SetWindowPos(hWnd_Cam, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Cam, SW_HIDE);
	SetWindowPos(hWnd_VCPos, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_VCPos, SW_HIDE);
	SetWindowPos(hWnd_VCHud, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_VCHud, SW_HIDE);
	SetWindowPos(hWnd_VCMFD, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_VCMFD, SW_HIDE);
	SetWindowPos(hWnd_Beacons, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Beacons, SW_HIDE);
	SetWindowPos(hWnd_LightCreation, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_LightCreation, SW_HIDE);
	SetWindowPos(hWnd_Lights, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Lights, SW_HIDE);
	SetWindowPos(hWnd_VarDrag, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_VarDrag, SW_HIDE);

	char verbuf[256] = { '\0' };
	sprintf(verbuf, "Ver:%i", VBVERSION);
	SetDlgItemText(hWnd, IDC_STATIC_VBVERSION, verbuf);
	
	

	return;
}

HTREEITEM DialogControl::FindHtreeItem(ItemType type, UINT idx) {
	map<HTREEITEM, TREE_ITEM_REF>::iterator it = TreeItem.begin();
		while (it != TreeItem.end()) {
			if ((it->second.Type == type) && (it->second.idx == idx)) {
				return it->first;
			}
			it++;
		}
	
	return NULL;
}

void DialogControl::UpdateTree(HWND hWnd, ItemType type, HTREEITEM select) {
	TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootVessel, TVE_EXPAND);
	switch (type) {
	case MESH:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootMeshes);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootMeshes);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;

		for (UINT i = 0; i < MshMng->GetMeshCount(); i++) {
			insertstruct.hParent = hrootMeshes;
			if (MshMng->GetMeshName(i).size() > 0) {
				char cbuf[256] = { '\0' };
				sprintf(cbuf, MshMng->GetMeshName(i).c_str());
				insertstruct.item.pszText = (LPSTR)cbuf;
				insertstruct.item.cchTextMax = ARRAYSIZE(cbuf);
				//insertstruct.item.pszText = (LPSTR)SB1->MshMng->GetMeshName(i).c_str();
				//insertstruct.item.cchTextMax = SB1->MshMng->GetMeshName(i).size();
			}
			else {
				char cbuf[256] = { '\0' };
				sprintf(cbuf, "Mesh_%i", i);
				insertstruct.item.pszText = (LPSTR)cbuf;
				insertstruct.item.cchTextMax = 10;
			}
			
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = MESH;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootMeshes, TVE_EXPAND);
		//TreeView_Select(GetDlgItem(hWnd, IDC_TREE1), hrootMeshes, 0);
		break;
	}
	case DOCK:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootDocks);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootDocks);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootDocks;
		for (UINT i = 0; i < DckMng->GetDockCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "%s", DckMng->GetDockName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = 9;
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = DOCK;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootDocks, TVE_EXPAND);


		break;
	}
	case ATTACHMENT:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootAttachments);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootAttachments);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootAttachments;
		for (UINT i = 0; i < AttMng->GetAttCount(); i++) {
			char cbuf[256] = { '\0' };
			if (AttMng->AttIsCreated(i)) {
				sprintf(cbuf, "Attachment_%i", i);
			}
			else {
				sprintf(cbuf, "Attachment_%i*", i);
			}
			
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = 14;
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = ATTACHMENT;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootAttachments, TVE_EXPAND);


		break;
	}
	case ANIMATIONS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootAnimations);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootAnimations);
		}
		TVINSERTSTRUCT insertstruct = { 0 };
		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootAnimations;
		if (AnimMng->GetAnimDefsCount() <= 0) { break; }
		for (UINT i = 0; i < AnimMng->GetAnimDefsCount(); i++) {
			if (!AnimMng->IsAnimValid(i)) { continue; }
			char cbuf[256] = { '\0' };
			sprintf(cbuf, AnimMng->GetAnimName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			//insertstruct.item.pszText = (LPSTR)AnimMng->GetAnimName(i).c_str();
			//insertstruct.item.cchTextMax = AnimMng->GetAnimName(i).size();
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = ANIMATIONS;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
			TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootAnimations, TVE_EXPAND);
			TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), Tir.hitem, TVE_EXPAND);
			HTREEITEM hparent = Tir.hitem;
			for (UINT j = 0; j < AnimMng->GetAnimNComps(i); j++) {
				ANIMATIONCOMPONENT_HANDLE ach = AnimMng->GetAnimComp(i, j);
				UINT cidx = AnimMng->GetAnimCompDefIdx(ach);
				if (cidx == (UINT)-1) {continue;	}
				char cbuf2[256] = { '\0' };
				sprintf(cbuf2, AnimMng->GetAnimCompDefName(cidx).c_str());
				insertstruct.item.pszText = (LPSTR)cbuf2;
				insertstruct.item.cchTextMax = ARRAYSIZE(cbuf2);
				//insertstruct.item.pszText = (LPSTR)AnimMng->GetAnimCompDefName(cidx).c_str();
				//insertstruct.item.cchTextMax = AnimMng->GetAnimCompDefName(cidx).size();
				TREE_ITEM_REF Tir = TREE_ITEM_REF();
				Tir.Type = ANIM_COMP;
				Tir.idx = cidx;
				insertstruct.hParent = hparent;
				Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
				TreeItem[Tir.hitem] = Tir;
				TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hparent, TVE_EXPAND);
				insertstruct.hParent = hrootAnimations;
			}
		}
	
		break;
	}
	case PROPELLANT:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootPropellant);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootPropellant);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootPropellant;
		for (UINT i = 0; i < PrpMng->GetPropCount(); i++) {
			char cbuf[256] = { '\0' };
			if (PrpMng->GetTankName(i).size()> 0) {
				sprintf(cbuf, "%s", PrpMng->GetTankName(i).c_str());
			}
			else {
				sprintf(cbuf, "Tank_%i", i);
			}
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = PROPELLANT;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootPropellant, TVE_EXPAND);

		break;
	}

	case EXTEX:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootExTex);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootExTex);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootExTex;
		for (UINT i = 0; i < ExTMng->GetExTexCount(); i++) {
			char cbuf[256] = { '\0' };
			if (ExTMng->GetExTexName(i).size()> 0) {
				if (ExTMng->IsExTexCreated(i)) {
					sprintf(cbuf, "%s", ExTMng->GetExTexName(i).c_str());
				}
				else {
					sprintf(cbuf, "*%s", ExTMng->GetExTexName(i).c_str());
				}
				
			}
			else {
				sprintf(cbuf, "*ExhaustTexture_%i", i);
			}
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = EXTEX;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootExTex, TVE_EXPAND);

		break;
	}

	case PARTICLES:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootParticles);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootParticles);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootParticles;
		for (UINT i = 0; i < PartMng->GetParticleDefCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "%s", PartMng->GetParticleDefName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = PARTICLES;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootParticles, TVE_EXPAND);

		break;
	}
	


	case THRUSTERS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootThrusters);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootThrusters);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootThrusters;
		for (UINT i = 0; i < ThrMng->GetThrCount(); i++) {
			char cbuf[256] = { '\0' };
			if (ThrMng->GetThrName(i).size()> 0) {
				sprintf(cbuf, "%s", ThrMng->GetThrName(i).c_str());
			}
			else {
				sprintf(cbuf, "Thruster_%i", i);
			}
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = THRUSTERS;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootThrusters, TVE_EXPAND);

		break;
	}
	case THRUSTERGROUPS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootThrusterGroups);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootThrusterGroups);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootThrusterGroups;
		char cbuf[256] = { '\0' };
		if (ThrGrpMng->Defined[THGROUP_MAIN]) {
			sprintf(cbuf, "MAIN");
		}
		else {
			sprintf(cbuf, "*MAIN");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		TREE_ITEM_REF Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_MAIN;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }
		
		if (ThrGrpMng->Defined[THGROUP_RETRO]) {
			sprintf(cbuf, "RETRO");
		}
		else {
			sprintf(cbuf, "*RETRO");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_RETRO;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_HOVER]) {
			sprintf(cbuf, "HOVER");
		}
		else {
			sprintf(cbuf, "*HOVER");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_HOVER;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_PITCHUP]) {
			sprintf(cbuf, "PITCH UP");
		}
		else {
			sprintf(cbuf, "*PITCH UP");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_PITCHUP;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_PITCHDOWN]) {
			sprintf(cbuf, "PITCH DOWN");
		}
		else {
			sprintf(cbuf, "*PITCH DOWN");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_PITCHDOWN;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_YAWLEFT]) {
			sprintf(cbuf, "YAW LEFT");
		}
		else {
			sprintf(cbuf, "*YAW LEFT");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_YAWLEFT;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_YAWRIGHT]) {
			sprintf(cbuf, "YAW RIGHT");
		}
		else {
			sprintf(cbuf, "*YAW RIGHT");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_YAWRIGHT;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_BANKLEFT]) {
			sprintf(cbuf, "BANK LEFT");
		}
		else {
			sprintf(cbuf, "*BANK LEFT");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_BANKLEFT;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_BANKRIGHT]) {
			sprintf(cbuf, "BANK RIGHT");
		}
		else {
			sprintf(cbuf, "*BANK RIGHT");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_BANKRIGHT;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_RIGHT]) {
			sprintf(cbuf, "TRANSLATE RIGHT");
		}
		else {
			sprintf(cbuf, "*TRANSLATE RIGHT");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_RIGHT;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_LEFT]) {
			sprintf(cbuf, "TRANSLATE LEFT");
		}
		else {
			sprintf(cbuf, "*TRANSLATE LEFT");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_LEFT;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_UP]) {
			sprintf(cbuf, "TRANSLATE UP");
		}
		else {
			sprintf(cbuf, "*TRANSLATE UP");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_UP;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_DOWN]) {
			sprintf(cbuf, "TRANSLATE DOWN");
		}
		else {
			sprintf(cbuf, "*TRANSLATE DOWN");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_DOWN;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_FORWARD]) {
			sprintf(cbuf, "TRANSLATE FORWARD");
		}
		else {
			sprintf(cbuf, "*TRANSLATE FORWARD");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_FORWARD;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		if (ThrGrpMng->Defined[THGROUP_ATT_BACK]) {
			sprintf(cbuf, "TRANSLATE BACK");
		}
		else {
			sprintf(cbuf, "*TRANSLATE BACK");
		}
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = THRUSTERGROUPS;
		Tir.idx = (int)THGROUP_ATT_BACK;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }

		break;
	}
	case TOUCHDOWNPOINTS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootTouchdownPoints);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootTouchdownPoints);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootTouchdownPoints;
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "SET 1");
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		TREE_ITEM_REF Tir = TREE_ITEM_REF();
		Tir.Type = TOUCHDOWNPOINTS;
		Tir.idx = 1;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }
		if (TdpMng->IsSecondSetEnabled()) {
			sprintf(cbuf, "SET 2");
		}
		else {
			sprintf(cbuf, "*SET 2");
		}
		
		insertstruct.item.pszText = (LPSTR)cbuf;
		insertstruct.item.cchTextMax = strlen(cbuf);
		Tir = TREE_ITEM_REF();
		Tir.Type = TOUCHDOWNPOINTS;
		Tir.idx = 2;
		Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
		TreeItem[Tir.hitem] = Tir;
		for (UINT i = 0; i < 256; i++) { cbuf[i] = { '\0' }; }
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootTouchdownPoints, TVE_EXPAND);

		break;
	}
	case AIRFOILS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootAirfoils);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootAirfoils);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootAirfoils;
		for (UINT i = 0; i < AirfoilMng->GetAirfoilDefCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "%s", AirfoilMng->GetAirfoilDefName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = AIRFOILS;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootAirfoils, TVE_EXPAND);

		break;
	}
	case CTRLSURFACES:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootControlSurfaces);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootControlSurfaces);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootControlSurfaces;
		for (UINT i = 0; i < CtrSurfMng->CtrlSurfDefCount(); i++) {
			char cbuf[256] = { '\0' };
			if (CtrSurfMng->IsCtrlSurfDefDefined(i)) {
				sprintf(cbuf, "%s", CtrSurfMng->GetCtrlSurfDefName(i).c_str());
			}
			else {
				sprintf(cbuf, "*%s", CtrSurfMng->GetCtrlSurfDefName(i).c_str());
			}
			
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = CTRLSURFACES;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootControlSurfaces, TVE_EXPAND);

		break;
	}
	case CAMERA:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootCameras);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootCameras);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootCameras;
		for (UINT i = 0; i < CamMng->GetCamCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "%s", CamMng->GetCamName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = CAMERA;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootCameras, TVE_EXPAND);

		break;
	}
	case VCPOS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootVCPositions);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootVCPositions);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootVCPositions;
		for (UINT i = 0; i < VCMng->GetPositionCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "%s", VCMng->GetPositionName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = VCPOS;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootVCPositions, TVE_EXPAND);
		break;
	}
	case VCMFD:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootVCMFDs);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootVCMFDs);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootVCMFDs;
		for (UINT i = 0; i < VCMng->GetMFDCount(); i++) {
			char cbuf[256] = { '\0' };
			if (i == 0) {
				sprintf(cbuf, "MFD_LEFT");
			}
			else if (i == 1) {
				sprintf(cbuf, "MFD_RIGHT");
			}
			else {
				sprintf(cbuf, "MFD_USER_%i", i-1);
			}
			
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = VCMFD;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootVCMFDs, TVE_EXPAND);
		break;
	}
	case BEACONS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootBeacons);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootBeacons);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootBeacons;
		for (UINT i = 0; i < LightsMng->GetBeaconCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "%s", LightsMng->GetBeaconName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = BEACONS;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootBeacons, TVE_EXPAND);
		break;
	}
	case LIGHTS:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootLightEmitters);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootLightEmitters);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootLightEmitters;
		for (UINT i = 0; i < LightsMng->GetLightsCount(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "%s", LightsMng->GetLightName(i).c_str());
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = LIGHTS;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootLightEmitters, TVE_EXPAND);
		break;
	}
	case VARIABLEDRAG:
	{
		HTREEITEM ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootVariableDrag);
		while (ht != NULL) {
			TreeItem.erase(ht);
			TreeView_DeleteItem(GetDlgItem(hWnd, IDC_TREE1), ht);
			ht = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREE1, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hrootVariableDrag);
		}
		TVINSERTSTRUCT insertstruct = { 0 };

		insertstruct.hInsertAfter = TVI_ROOT;
		insertstruct.item.mask = TVIF_TEXT;
		insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;
		insertstruct.hParent = hrootVariableDrag;
		for (UINT i = 0; i < VardMng->GetVardDefCount(); i++) {
			char cbuf[256] = { '\0' };
			if (VardMng->IsVardDefDefined(i)) {
				sprintf(cbuf, "%s", VardMng->GetVardName(i).c_str());
			}
			else {
				sprintf(cbuf, "*%s", VardMng->GetVardName(i).c_str());
			}
			
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = strlen(cbuf);
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = VARIABLEDRAG;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
		}
		TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootVariableDrag, TVE_EXPAND);
		break;
	}



	}
	
	//if (ItemToSelect != NULL) { TreeView_SelectItem(GetDlgItem(hWnd, IDC_TREE1), ItemToSelect); }
	//ItemToSelect = NULL;
	//while (SendDlgItemMessage(hWnd, IDC_TREE1, TVM_DELETEITEM, 0, (LPARAM)hrootMeshes)) {}
	
	
}

void DialogControl::InitTree(HWND hWnd) {
	
	LONG style = GetWindowLong(GetDlgItem(hWnd, IDC_TREE1), GWL_STYLE);
	SetWindowLong(GetDlgItem(hWnd, IDC_TREE1), GWL_STYLE, style | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);
	TreeView_DeleteAllItems(GetDlgItem(hWnd, IDC_TREE1));
	TVINSERTSTRUCT insertstruct = { 0 };
	
	insertstruct.hInsertAfter = TVI_ROOT;
	insertstruct.hParent = TVI_ROOT;
	insertstruct.item.mask = TVIF_TEXT;
	insertstruct.item.stateMask = TVIS_STATEIMAGEMASK | TVIS_EXPANDED;

	char vname[256] = { '\0' };
	sprintf(vname, "%s", VB1->GetName());
	string sname(vname);
	insertstruct.item.pszText = (LPSTR)sname.c_str();
	insertstruct.item.cchTextMax = sname.size();
	hrootVessel = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	TREE_ITEM_REF Tir = TREE_ITEM_REF();
	Tir.hitem = hrootVessel;
	Tir.Type = ROOTS;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.hParent = hrootVessel;


	insertstruct.item.pszText = (LPSTR)TEXT("General Settings\0");
	insertstruct.item.cchTextMax = 18;
	hrootSettings = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootSettings;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Meshes\0");
	insertstruct.item.cchTextMax = 7;
	hrootMeshes = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootMeshes;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Docks\0");
	insertstruct.item.cchTextMax = 6;
	hrootDocks = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootDocks;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Attachments\0");
	insertstruct.item.cchTextMax = 12;
	hrootAttachments = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootAttachments;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Animations\0");
	insertstruct.item.cchTextMax = 11;
	hrootAnimations = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootAnimations;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Propellants\0");
	insertstruct.item.cchTextMax = 12;
	hrootPropellant = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootPropellant;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Exhaust Textures\0");
	insertstruct.item.cchTextMax = 18;
	hrootExTex = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootExTex;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Particle Streams\0");
	insertstruct.item.cchTextMax = 18;
	hrootParticles = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootParticles;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Thrusters\0");
	insertstruct.item.cchTextMax = 10;
	hrootThrusters = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootThrusters;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Thruster Groups\0");
	insertstruct.item.cchTextMax = 18;
	hrootThrusterGroups = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootThrusterGroups;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Touchdown Points\0");
	insertstruct.item.cchTextMax = 18;
	hrootTouchdownPoints = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootTouchdownPoints;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Airfoils\0");
	insertstruct.item.cchTextMax = 10;
	hrootAirfoils = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootAirfoils;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Control Surfaces\0");
	insertstruct.item.cchTextMax = 18;
	hrootControlSurfaces = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootControlSurfaces;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Lights\0");
	insertstruct.item.cchTextMax = 7;
	hrootLights = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootLights;
	TreeItem[Tir.hitem] = Tir;
	insertstruct.hParent = hrootLights;
	insertstruct.item.pszText = (LPSTR)TEXT("Beacons\0");
	insertstruct.item.cchTextMax = 10;
	hrootBeacons = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootBeacons;
	TreeItem[Tir.hitem] = Tir;
	insertstruct.item.pszText = (LPSTR)TEXT("Light Emitters\0");
	insertstruct.item.cchTextMax = 16;
	hrootLightEmitters = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootLightEmitters;
	TreeItem[Tir.hitem] = Tir;


	insertstruct.hParent = hrootVessel;

	insertstruct.item.pszText = (LPSTR)TEXT("Cameras\0");
	insertstruct.item.cchTextMax = 8;
	hrootCameras = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootCameras;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Virtual Cockpit\0");
	insertstruct.item.cchTextMax = 18;
	hrootVC = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootVC;
	TreeItem[Tir.hitem] = Tir;
	insertstruct.hParent = hrootVC;
	insertstruct.item.pszText = (LPSTR)TEXT("Positions\0");
	insertstruct.item.cchTextMax = 11;
	hrootVCPositions = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootVCPositions;
	TreeItem[Tir.hitem] = Tir;
	insertstruct.item.pszText = (LPSTR)TEXT("MFDs\0");
	insertstruct.item.cchTextMax = 6;
	hrootVCMFDs = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootVCMFDs;
	TreeItem[Tir.hitem] = Tir;
	insertstruct.item.pszText = (LPSTR)TEXT("HUD\0");
	insertstruct.item.cchTextMax = 5;
	hrootVCHud= TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootVCHud;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.hParent = hrootVessel;
	insertstruct.item.pszText = (LPSTR)TEXT("Variable Drag Elems\0");
	insertstruct.item.cchTextMax = 21;
	hrootVariableDrag = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootVariableDrag;
	TreeItem[Tir.hitem] = Tir;

	
	UpdateTree(hWnd, MESH,0);
	UpdateTree(hWnd, DOCK,0);
	UpdateTree(hWnd, ATTACHMENT, 0);
	UpdateTree(hWnd, ANIMATIONS,0);
	UpdateTree(hWnd, PROPELLANT, 0);
	UpdateTree(hWnd, EXTEX, 0);
	UpdateTree(hWnd, PARTICLES, 0);
	UpdateTree(hWnd, THRUSTERS, 0);
	UpdateTree(hWnd, THRUSTERGROUPS, 0);
	UpdateTree(hWnd, TOUCHDOWNPOINTS, 0);
	UpdateTree(hWnd, AIRFOILS, 0);
	UpdateTree(hWnd, CTRLSURFACES, 0);
	UpdateTree(hWnd, CAMERA, 0);
	UpdateTree(hWnd, VCPOS, 0);
	UpdateTree(hWnd, VCMFD, 0);
	UpdateTree(hWnd, BEACONS, 0);
	UpdateTree(hWnd, LIGHTS, 0);
	UpdateTree(hWnd, VARIABLEDRAG, 0);
	return;
}

void DialogControl::ShowTheRightDialog(ItemType type) {
	//SB1->AnimEditingMode = false;
	if (type != MESH) {
		ShowWindow(hwnd_Mesh, SW_HIDE);
		VB1->MshMng->ResetHighlights();
	}
	if (type != DOCK) {
		ShowWindow(hWnd_Dock, SW_HIDE);
	}
	if (type != ANIMATIONS) {
		ShowWindow(hWnd_Anim, SW_HIDE);	
	}
	if (type != ANIM_COMP) {
		ShowWindow(hWnd_AnimComp, SW_HIDE);
	}
	if (type != ATTACHMENT) {
		ShowWindow(hWnd_Atts, SW_HIDE);
	}
	if (type != PROPELLANT) {
		ShowWindow(hWnd_Prp, SW_HIDE);
	}
	if (type != EXTEX) {
		ShowWindow(hWnd_ExTex, SW_HIDE);
	}
	if (type != PARTICLES) {
		ShowWindow(hWnd_Part, SW_HIDE);
	}
	if (type != THRUSTERS) {
		ShowWindow(hWnd_Thr, SW_HIDE);
		if (ShowingExhausts) {
			ShowExhaustsWin(hWnd_Thr, false);
		}
	}
	if (type != THRUSTERGROUPS) {
		ShowWindow(hWnd_ThrGrp, SW_HIDE);
	}
	if (type != TOUCHDOWNPOINTS) {
		ShowWindow(hWnd_Tdp, SW_HIDE);
	}
	if (type != AIRFOILS) {
		ShowWindow(hWnd_Airfoils, SW_HIDE);
		if (ShowingAFGraph) {
			ShowAirfoilFuncGraph(hWnd_Airfoils, false);
		}
	}
	if (type != CTRLSURFACES) {
		ShowWindow(hWnd_CtrlSurfaces, SW_HIDE);
	}
	if (type != CAMERA) {
		ShowWindow(hWnd_Cam, SW_HIDE);
	}
	if (type != VCPOS) {
		ShowWindow(hWnd_VCPos, SW_HIDE);
	}
	if (type != VCMFD) {
		ShowWindow(hWnd_VCMFD, SW_HIDE);
	}
	if (type != BEACONS) {
		ShowWindow(hWnd_Beacons, SW_HIDE);
	}
	if (type != LIGHTS) {
		ShowWindow(hWnd_Lights, SW_HIDE);
	}
	if (type != VARIABLEDRAG) {
		ShowWindow(hWnd_VarDrag, SW_HIDE);
	}



	switch (type) {
	case MESH:
	{
		ShowWindow(hwnd_Mesh, SW_SHOW);
		break;
	}
	case DOCK:
	{
		ShowWindow(hWnd_Dock, SW_SHOW);
		break;
	}
	case ANIMATIONS:
	{
		ShowWindow(hWnd_Anim, SW_SHOW);
	//	SB1->AnimEditingMode = true;
		break;
	}
	case ANIM_COMP:
	{
		ShowWindow(hWnd_AnimComp, SW_SHOW);
//		SB1->AnimEditingMode = true;
		break;
	}
	case ATTACHMENT:
	{
		ShowWindow(hWnd_Atts, SW_SHOW);
		break;
	}
	case PROPELLANT:
	{
		ShowWindow(hWnd_Prp, SW_SHOW);
		break;
	}
	case EXTEX:
	{
		ShowWindow(hWnd_ExTex, SW_SHOW);
		break;
	}
	case PARTICLES:
	{
		ShowWindow(hWnd_Part, SW_SHOW);
		break;
	}
	case THRUSTERS:
	{
		ShowWindow(hWnd_Thr, SW_SHOW);
		break;
	}
	case THRUSTERGROUPS:
	{
		ShowWindow(hWnd_ThrGrp, SW_SHOW);
		break;
	}
	case TOUCHDOWNPOINTS:
	{
		ShowWindow(hWnd_Tdp, SW_SHOW);
		break;
	}
	case AIRFOILS:
	{
		ShowWindow(hWnd_Airfoils, SW_SHOW);
		break;
	}
	case CTRLSURFACES:
	{
		ShowWindow(hWnd_CtrlSurfaces, SW_SHOW);
		break;
	}
	case CAMERA:
	{
		ShowWindow(hWnd_Cam, SW_SHOW);
		break;
	}
	case VCPOS:
	{
		ShowWindow(hWnd_VCPos, SW_SHOW);
		break;
	}
	case VCMFD:
	{
		ShowWindow(hWnd_VCMFD, SW_SHOW);
		break;
	}
	case BEACONS:
	{
		ShowWindow(hWnd_Beacons, SW_SHOW);
		break;
	}
	case LIGHTS:
	{
		ShowWindow(hWnd_Lights, SW_SHOW);
		break;
	}
	case VARIABLEDRAG:
	{
		ShowWindow(hWnd_VarDrag, SW_SHOW);
		break;
	}
	}

	return;
}



BOOL CALLBACK DialogControl::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
	{
		InitCommonControls();
		InitDialog(hWnd);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_ADD:
		{
			if (CurrentSelection.hitem == hrootMeshes) {
				VB1->MshMng->AddMeshDef();
				UpdateTree(hWnd, MESH,hrootMeshes);
			}
			else if (CurrentSelection.hitem == hrootDocks) {
				DckMng->AddDockDef();
				UpdateTree( hWnd, DOCK,hrootDocks);
			}
			else if (CurrentSelection.hitem == hrootAnimations) {
				AnimMng->AddAnimDef();
				UpdateTree(hWnd, ANIMATIONS,hrootAnimations);
			}
			else if (CurrentSelection.hitem == hrootAttachments) {
				AttMng->AddAttDefNoCreate();
				UpdateTree(hWnd, ATTACHMENT, 0);
			}
			else if (CurrentSelection.hitem == hrootPropellant) {
				PrpMng->AddTankDef();				
				UpdateTree(hWnd, PROPELLANT, 0);
			}
			else if (CurrentSelection.hitem == hrootExTex) {
				ExTMng->AddExTexDef();
				UpdateTree(hWnd, EXTEX, 0);
			}
			else if (CurrentSelection.hitem == hrootParticles) {
				PartMng->AddParticleDef();
				UpdateTree(hWnd, PARTICLES, 0);
			}
			else if (CurrentSelection.hitem == hrootThrusters) {
				ThrMng->AddThrDef();
				UpdateTree(hWnd, THRUSTERS, 0);
			}
			else if (CurrentSelection.hitem == hrootAirfoils) {
				AirfoilMng->CreateAirfoilDef(LIFT_VERTICAL);
				UpdateTree(hWnd, AIRFOILS, 0);
				///
			}
			else if (CurrentSelection.hitem == hrootControlSurfaces) {
				CtrSurfMng->CreateUndefinedCtrlSurfDef();
				UpdateTree(hWnd, CTRLSURFACES, 0);
			}
			else if (CurrentSelection.hitem == hrootCameras) {
				CamMng->AddCamDef();
				UpdateTree(hWnd, CAMERA, 0);
			}
			else if (CurrentSelection.hitem == hrootVCPositions) {
				VCMng->AddPosition();
				UpdateTree(hWnd, VCPOS, 0);
			}
			else if (CurrentSelection.hitem == hrootVCMFDs) {
				VCMng->AddMFD();
				UpdateTree(hWnd, VCMFD, 0);
			}
			else if (CurrentSelection.hitem == hrootBeacons) {
				LightsMng->AddBeaconDef();
				UpdateTree(hWnd, BEACONS, 0);
			}
			else if (CurrentSelection.hitem == hrootThrusterGroups) {
				VB1->AddDefaultRCS();
			}
			else if (CurrentSelection.hitem == hrootVariableDrag) {
				VardMng->AddUndefinedVardDef();
				UpdateTree(hWnd, VARIABLEDRAG, 0);
			}
			break;
		}
		case IDC_BUTTON_ADD2:
		{
			if (CurrentSelection.hitem == hrootAirfoils) {
				AirfoilMng->CreateAirfoilDef(LIFT_HORIZONTAL);
				UpdateTree(hWnd, AIRFOILS, 0);

			}
			break;
		}
		case IDC_BUTTON_TEXTREADER:
		{
			VB1->TextDlg->Open(hDLL);
			break;
		}
		case IDC_BUTTON_CLOSE:
		{
			Close();
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
	{
		Close();
		break;
	}
	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{
			case IDC_TREE1:
			{
				if (((LPNMHDR)lParam)->code == TVN_SELCHANGED)
				{
					HTREEITEM Selected = TreeView_GetSelection(GetDlgItem(hWnd, IDC_TREE1));
					TREE_ITEM_REF old_Selection = CurrentSelection;
					CurrentSelection = TreeItem[Selected];
					
					if (CurrentSelection.hitem == hrootMeshes) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD MESH");
					}else if (CurrentSelection.hitem == hrootDocks) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD DOCK");
					}
					else if (CurrentSelection.hitem == hrootAttachments) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD ATTACHMENT");
					}
					else if (CurrentSelection.hitem == hrootLights) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD LIGHT");
					}
					else if (CurrentSelection.hitem == hrootCameras) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD CAMERA");
					}
					else if (CurrentSelection.hitem == hrootSettings) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_HIDE);
						
					}
					else if (CurrentSelection.hitem == hrootAnimations) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD ANIMATION");
					}
					else if (CurrentSelection.hitem == hrootPropellant) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD PROPELLANT TANK");
					}
					else if (CurrentSelection.hitem == hrootExTex) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD EXHAUST TEXTURE");
					}
					else if (CurrentSelection.hitem == hrootParticles) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD PARTICLE STREAM");
					}
					else if (CurrentSelection.hitem == hrootThrusters) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD THRUSTER");
					}
					else if (CurrentSelection.hitem == hrootAirfoils) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD VERTICAL AIRFOIL");
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD2), (LPCSTR)"ADD HORIZONTAL AIRFOIL");
					}
					else if (CurrentSelection.hitem == hrootControlSurfaces) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD CONTROL SURFACE");
					}
					else if (CurrentSelection.hitem == hrootVCPositions) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD VIRTUAL COCKPIT POSITION");
					}
					else if (CurrentSelection.hitem == hrootVCMFDs) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD MFD DEFINITION");
					}
					else if (CurrentSelection.hitem == hrootBeacons) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD BEACON");
					}
					else if (CurrentSelection.hitem == hrootThrusterGroups) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD DEFAULT RCS SYSTEM");
					}
					else if (CurrentSelection.hitem == hrootVariableDrag) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD VARIABLE DRAG ELEMENT");
					}
					else {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_HIDE);
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD2), SW_HIDE);
					}


					////NON CHILDREN
					if (CurrentSelection.hitem == hrootSettings) {
						ShowWindow(hWnd_Settings, SW_SHOW);
						UpdateSettingsDialog(hWnd_Settings);
					}
					else {
						ShowWindow(hWnd_Settings, SW_HIDE);
					}
					if (CurrentSelection.hitem == hrootVCHud) {
						ShowWindow(hWnd_VCHud, SW_SHOW);
						UpdateVCHUDDialog(hWnd_VCHud);
					}
					else {
						ShowWindow(hWnd_VCHud, SW_HIDE);
					}
					if (CurrentSelection.hitem == hrootLightEmitters) {
						ShowWindow(hWnd_LightCreation, SW_SHOW);
					}
					else {
						ShowWindow(hWnd_LightCreation, SW_HIDE);
					}
					
					
					ShowTheRightDialog(CurrentSelection.Type);


					switch (CurrentSelection.Type) {
						case MESH:
						{
							VB1->MshMng->ResetHighlights();
							LRESULT getcheck = SendDlgItemMessage(hwnd_Mesh, IDC_CHECK_HIGHLIGHT_MSH, BM_GETCHECK, 0, 0);
							if (getcheck == BST_CHECKED) {
								VB1->MshMng->HighlightMesh(VB1->MshMng->IdxDef2Msh(CurrentSelection.idx),true);
							}
							UpdateMeshDialog(hwnd_Mesh);
							break;
						}
						case DOCK:
						{
							UpdateDockDialog(hWnd_Dock);
							break;
						}
						case ANIMATIONS:
						{
							UpdateAnimDialog(hWnd_Anim);
							break;
						}
						case ANIM_COMP:
						{
							UpdateAnimCompDialog(hWnd_AnimComp);
							break;
						}
						case ATTACHMENT:
						{
							UpdateAttDialog(hWnd_Atts);
							break;
						}
						case PROPELLANT:
						{
							UpdatePrpDialog(hWnd_Prp);
							break;
						}
						case EXTEX:
						{
							UpdateExTexDialog(hWnd_ExTex);
							break;
						}
						case PARTICLES:
						{
							UpdatePartDialog(hWnd_Part);
							break;
						}
						case THRUSTERS:
						{
							UpdateThrDialog(hWnd_Thr);
							break;
						}
						case THRUSTERGROUPS:
						{
							UpdateThrGrpDialog(hWnd_ThrGrp);
							break;
						}
						case TOUCHDOWNPOINTS:
						{
							UpdateTdpDialog(hWnd_Tdp);
							break;
						}
						case AIRFOILS:
						{
							UpdateAirfoilDialog(hWnd_Airfoils);
							break;
						}
						case CTRLSURFACES:
						{
							UpdateCtrlSurfDialog(hWnd_CtrlSurfaces);
							break;
						}
						case CAMERA:
						{
							UpdateCamDialog(hWnd_Cam);
							break;
						}
						case VCPOS:
						{
							UpdateVCPosDialog(hWnd_VCPos);
							break;
						}
						case VCMFD:
						{
							UpdateVCMFDDialog(hWnd_VCMFD);
							break;
						}
						case BEACONS:
						{
							UpdateBeaconsDialog(hWnd_Beacons);
							break;
						}
						case LIGHTS:
						{
							UpdateLightsDialog(hWnd_Lights);
							break;
						}
						case VARIABLEDRAG:
						{
							UpdateVarDragDialog(hWnd_VarDrag);
							break;
						}
					}
				}
				break;
			}
		}
	}
	}

	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}

void DialogControl::InitOapiKeys() {

	oapi_keys[0x02] = "1\0";
	oapi_keys[0x03] = "2\0";
	oapi_keys[0x04] = "3\0";
	oapi_keys[0x05] = "4\0";
	oapi_keys[0x06] = "5\0";
	oapi_keys[0x07] = "6\0";
	oapi_keys[0x08] = "7\0";
	oapi_keys[0x09] = "8\0";
	oapi_keys[0x0A] = "9\0";
	oapi_keys[0x0B] = "0\0";
	oapi_keys[0x10] = "Q\0";
	oapi_keys[0x11] = "W\0";
	oapi_keys[0x12] = "E\0";
	oapi_keys[0x13] = "R\0";
	oapi_keys[0x14] = "T\0";
	oapi_keys[0x15] = "Y\0";
	oapi_keys[0x16] = "U\0";
	oapi_keys[0x17] = "I\0";
	oapi_keys[0x18] = "O\0";
	oapi_keys[0x19] = "P\0";
	oapi_keys[0x1E] = "A\0";
	oapi_keys[0x1F] = "S\0";
	oapi_keys[0x20] = "D\0";
	oapi_keys[0x21] = "F\0";
	oapi_keys[0x22] = "G\0";
	oapi_keys[0x23] = "H\0";
	oapi_keys[0x24] = "J\0";
	oapi_keys[0x25] = "K\0";
	oapi_keys[0x26] = "L\0";
	oapi_keys[0x2C] = "Z\0";
	oapi_keys[0x2D] = "X\0";
	oapi_keys[0x2E] = "C\0";
	oapi_keys[0x2F] = "V\0";
	oapi_keys[0x30] = "B\0";
	oapi_keys[0x31] = "N\0";
	oapi_keys[0x32] = "M\0";
	oapi_keys[0x47] = "NUMPAD7\0";
	oapi_keys[0x48] = "NUMPAD8\0";
	oapi_keys[0x49] = "NUMPAD9\0";
	oapi_keys[0x4B] = "NUMPAD4\0";
	oapi_keys[0x4C] = "NUMPAD5\0";
	oapi_keys[0x4D] = "NUMPAD6\0";
	oapi_keys[0x4F] = "NUMPAD1\0";
	oapi_keys[0x50] = "NUMPAD2\0";
	oapi_keys[0x51] = "NUMPAD3\0";
	oapi_keys[0x52] = "NUMPAD0\0";
	oapi_keys[0xC8] = "UP\0";
	oapi_keys[0xCB] = "LEFT\0";
	oapi_keys[0xCD] = "RIGHT\0";
	oapi_keys[0xD0] = "DOWN\0";

	return;
}


int DialogControl::ComboFindItemData(HWND hWnd, void* data) {
	int item_count = SendMessage(hWnd, CB_GETCOUNT, 0, 0);
	for (int i = 0; i < item_count; i++) {
		void* result = (void*)SendMessage(hWnd, CB_GETITEMDATA, i, 0);
		if (result == data) {
			return i;
		}
	}
	return -1;
}

int DialogControl::ComboFindItemData(HWND hWnd, DWORD Data) {
	int item_count = SendMessage(hWnd, CB_GETCOUNT, 0, 0);
	for (int i = 0; i < item_count; i++) {
		DWORD result = (DWORD)SendMessage(hWnd, CB_GETITEMDATA, i, 0);
		if (result == Data) {
			return i;
		}
	}
	return -1;
}


double DialogControl::GetDlgItemDouble(HWND hWnd, int control_id) {
	char cbuf[256] = { '\0' };
	GetDlgItemText(hWnd, control_id, cbuf, 256);
	double val = atof(cbuf);
	return val;
}
void DialogControl::SetDlgItemDouble(HWND hWnd, int control_id, double val, UINT precision) {
	char cbuf[256] = { '\0' };
	if (precision == 0) {
		sprintf(cbuf, "%.0f", val);
	}
	else if (precision == 1) {
		sprintf(cbuf, "%.1f", val);
	}
	else if (precision == 2) {
		sprintf(cbuf, "%.2f", val);
	}
	else if (precision == 3) {
		sprintf(cbuf, "%.3f", val);
	}
	else if (precision == 4) {
		sprintf(cbuf, "%.4f", val);
	}
	else if (precision == 5) {
		sprintf(cbuf, "%.5f", val);
	}
	else if (precision == 6) {
		sprintf(cbuf, "%.6f", val);
	}
	SetDlgItemText(hWnd, control_id, cbuf);
	return;
}
void DialogControl::SetDlgItemsTextVector3(HWND hWnd, int id1, int id2, int id3, VECTOR3 v3, int precision) {
	char cbuf[256] = { '\0' };
	if (precision == 1) {
		sprintf(cbuf, "%.1f", v3.x);
		SetDlgItemText(hWnd, id1, cbuf);
		sprintf(cbuf, "%.1f", v3.y);
		SetDlgItemText(hWnd, id2, cbuf);
		sprintf(cbuf, "%.1f", v3.z);
		SetDlgItemText(hWnd, id3, cbuf);
	}
	else if (precision == 2) {
		sprintf(cbuf, "%.2f", v3.x);
		SetDlgItemText(hWnd, id1, cbuf);
		sprintf(cbuf, "%.2f", v3.y);
		SetDlgItemText(hWnd, id2, cbuf);
		sprintf(cbuf, "%.2f", v3.z);
		SetDlgItemText(hWnd, id3, cbuf);
	}
	else if (precision == 3) {
		sprintf(cbuf, "%.3f", v3.x);
		SetDlgItemText(hWnd, id1, cbuf);
		sprintf(cbuf, "%.3f", v3.y);
		SetDlgItemText(hWnd, id2, cbuf);
		sprintf(cbuf, "%.3f", v3.z);
		SetDlgItemText(hWnd, id3, cbuf);
	}
	else if (precision == 4) {
		sprintf(cbuf, "%.4f", v3.x);
		SetDlgItemText(hWnd, id1, cbuf);
		sprintf(cbuf, "%.4f", v3.y);
		SetDlgItemText(hWnd, id2, cbuf);
		sprintf(cbuf, "%.4f", v3.z);
		SetDlgItemText(hWnd, id3, cbuf);
	}
	else if (precision == 5) {
		sprintf(cbuf, "%.5f", v3.x);
		SetDlgItemText(hWnd, id1, cbuf);
		sprintf(cbuf, "%.5f", v3.y);
		SetDlgItemText(hWnd, id2, cbuf);
		sprintf(cbuf, "%.5f", v3.z);
		SetDlgItemText(hWnd, id3, cbuf);
	}
	else if (precision >= 6) {
		sprintf(cbuf, "%.6f", v3.x);
		SetDlgItemText(hWnd, id1, cbuf);
		sprintf(cbuf, "%.6f", v3.y);
		SetDlgItemText(hWnd, id2, cbuf);
		sprintf(cbuf, "%.6f", v3.z);
		SetDlgItemText(hWnd, id3, cbuf);
	}
	
	return;
}
VECTOR3 DialogControl::GetDlgItemsVector3(HWND hWnd, int id1, int id2, int id3) {
	char cbuf[256] = { '\0' };
	double val1 = 0;
	double val2 = 0;
	double val3 = 0;
	GetDlgItemText(hWnd, id1, (LPSTR)cbuf, 256);
	val1 = atof(cbuf);
	GetDlgItemText(hWnd, id2, (LPSTR)cbuf, 256);
	val2 = atof(cbuf);
	GetDlgItemText(hWnd, id3, (LPSTR)cbuf, 256);
	val3 = atof(cbuf);
	return _V(val1, val2, val3);
}

bool DialogControl::IsCheckBoxChecked(HWND hWnd, int control_id) {
	LRESULT getcheck = SendDlgItemMessage(hWnd, control_id, BM_GETCHECK, 0, 0);
	if (getcheck == BST_CHECKED) {
		return true;
	}
	else {
		return false;
	}
}

void DialogControl::ClearLasers(map<UINT, LASER_HANDLE> &m) {
	map<UINT, LASER_HANDLE>::iterator it;
	for (it = m.begin(); it != m.end(); it++) {
		if (it->second != NULL) {
			VB1->Laser->DeleteLaser(it->second);
		}
	}
	m.clear();
	return;
}