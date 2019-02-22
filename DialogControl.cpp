#include "StationBuilder1.h"
#include "resource.h"
#include "DialogControl.h"
#include "MeshManager.h"
#include "AnimCompDef.h"
#include "AnimDef.h"
#include "AttachmentManager.h"
#include "AnimationManager.h"
#include "PropellantManager.h"
#include "ThrusterManager.h"
#pragma comment(lib, "comctl32.lib")



extern HINSTANCE hDLL;
//extern void RotateMeshClbk(MESHHANDLE, bool);
//Hooks...

BOOL CALLBACK DlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DialogControl *DlgCtrl = (uMsg == WM_INITDIALOG ? (DialogControl*)lParam : (DialogControl*)oapiGetDialogContext(hWnd));
	return DlgCtrl->DlgProc(hWnd, uMsg, wParam, lParam);
}
//DialogControl* dlg;
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
BOOL CALLBACK ThrDlgProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)lParam);
	}
	DialogControl *DlgCtrl = (DialogControl*)GetWindowLong(hWnd, GWL_USERDATA);
	return DlgCtrl->ThrDlgProc(hWnd, uMsg, wParam, lParam);
}


DialogControl::DialogControl(StationBuilder1 *_SB1) {
	SB1 = _SB1;
	AnimMng = SB1->AnimMng;
	AttMng = SB1->AttMng;
	PrpMng = SB1->PrpMng;
	ThrMng = SB1->ThrMng;
	open = false;
	hDlg = NULL;
	TreeItem.clear();
	CurrentSelection = TREE_ITEM_REF();
	speedrotation = 1*RAD;
	test = 0;
	InitOapiKeys();
	AnimTesting = false;
//	ItemToSelect = NULL;

}
DialogControl::~DialogControl() {
	SB1 = NULL;
	AnimMng = NULL;
	AttMng = NULL;
	PrpMng = NULL;
	ThrMng = NULL;
	hDlg = NULL;
	open = false;
}
void DialogControl::Open(HINSTANCE hDLL) {
	hDlg = oapiOpenDialogEx(hDLL, IDD_DIALOG1, DlgProcHook, DLG_CAPTIONHELP|DLG_CAPTIONCLOSE, this);
	open = true;
}
void DialogControl::Close() {
	open = false;
	SB1->MshMng->ResetHighlights();
	if (SB1->DockExhaustsActive) {
		SB1->DeleteDockExhausts();
	}
	if (SB1->AttExhaustsActive) {
		SB1->DeleteAttExhausts();
	}
	if (SB1->thExhaustsActive) {
		SB1->DeleteThExhausts();
	}
//	if (SB1->DockBeaconsActive) {
//		SB1->DeleteDockBeacons();
//	}
	oapiCloseDialog(hDlg);
	hDlg = NULL;
}
bool DialogControl::IsOpen() {
	return open;
}

void DialogControl::InitDialog(HWND hWnd) {
	InitTree(hWnd);
	ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_HIDE);
	SB1->MshMng->md_restore = SB1->MshMng->GetAllDefs();
	hwnd_Mesh = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_MESH), hWnd, MeshDlgProcHook, (LPARAM)this);
	hWnd_Dock = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_DOCK), hWnd, DockDlgProcHook, (LPARAM)this);
	hWnd_Anim = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_ANIM), hWnd, AnimDlgProcHook, (LPARAM)this);
	hWnd_AnimComp = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_ANIMCOMP), hWnd, AnimCompDlgProcHook, (LPARAM)this);
	hWnd_Atts = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_ATT), hWnd, AttDlgProcHook, (LPARAM)this);
	hWnd_Prp = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_PROPELLANT), hWnd, PrpDlgProcHook, (LPARAM)this);
	hWnd_Thr = CreateDialogParam(hDLL, MAKEINTRESOURCE(IDD_DIALOG_THRUSTERS), hWnd, ThrDlgProcHook, (LPARAM)this);
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
	SetWindowPos(hWnd_Thr, NULL, 255, 10, 0, 0, SWP_NOSIZE);
	ShowWindow(hWnd_Thr, SW_HIDE);
	return;
}

void DialogControl::SetDlgItemsTextVector3(HWND hWnd, int id1, int id2, int id3, VECTOR3 v3) {
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "%.3f", v3.x);
	SetDlgItemText(hWnd, id1, cbuf);
	sprintf(cbuf, "%.3f", v3.y);
	SetDlgItemText(hWnd, id2, cbuf);
	sprintf(cbuf, "%.3f", v3.z);
	SetDlgItemText(hWnd, id3, cbuf);
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

		for (UINT i = 0; i < SB1->MshMng->GetMeshCount(); i++) {
			insertstruct.hParent = hrootMeshes;
			if (SB1->MshMng->GetMeshName(i).size() > 0) {
				char cbuf[256] = { '\0' };
				sprintf(cbuf, SB1->MshMng->GetMeshName(i).c_str());
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
		for (UINT i = 0; i < SB1->dock_definitions.size(); i++) {
			char cbuf[256] = { '\0' };
			sprintf(cbuf, "Dock_%i", i);
			insertstruct.item.pszText = (LPSTR)cbuf;
			insertstruct.item.cchTextMax = 9;
			TREE_ITEM_REF Tir = TREE_ITEM_REF();
			Tir.Type = DOCK;
			Tir.idx = i;
			Tir.hitem = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
			TreeItem[Tir.hitem] = Tir;
			TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootDocks, TVE_EXPAND);
		}
		

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
			TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootAttachments, TVE_EXPAND);
		}


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
		if (AnimMng->GetAnimDefCount() <= 0) { break; }
		for (UINT i = 0; i < AnimMng->GetAnimDefCount(); i++) {
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
				AnimCompDef* acd = AnimMng->GetAnimComp(i, j);
				UINT cidx = AnimMng->GetAnimCompDefCompIdx(acd);
				if (cidx == -1) {continue;	}
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
			TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootPropellant, TVE_EXPAND);
		}
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
			TreeView_Expand(GetDlgItem(hWnd, IDC_TREE1), hrootThrusters, TVE_EXPAND);
		}
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
	sprintf(vname, "%s", SB1->GetName());
	string sname(vname);
	insertstruct.item.pszText = (LPSTR)sname.c_str();
	insertstruct.item.cchTextMax = sname.size();
	hrootVessel = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	TREE_ITEM_REF Tir = TREE_ITEM_REF();
	Tir.hitem = hrootVessel;
	Tir.Type = ROOTS;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.hParent = hrootVessel;
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

	insertstruct.item.pszText = (LPSTR)TEXT("Thrusters\0");
	insertstruct.item.cchTextMax = 10;
	hrootThrusters = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootThrusters;
	TreeItem[Tir.hitem] = Tir;


	insertstruct.item.pszText = (LPSTR)TEXT("Lights\0");
	insertstruct.item.cchTextMax = 7;
	hrootLights = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootLights;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Cameras\0");
	insertstruct.item.cchTextMax = 8;
	hrootCameras = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	Tir.hitem = hrootCameras;
	TreeItem[Tir.hitem] = Tir;

	insertstruct.item.pszText = (LPSTR)TEXT("Settings\0");
	insertstruct.item.cchTextMax = 9;
	hrootSettings = TreeView_InsertItem(GetDlgItem(hWnd, IDC_TREE1), &insertstruct);
	UpdateTree(hWnd, MESH,0);
	UpdateTree(hWnd, DOCK,0);
	UpdateTree(hWnd, ATTACHMENT, 0);
	UpdateTree(hWnd, ANIMATIONS,0);
	UpdateTree(hWnd, PROPELLANT, 0);
	UpdateTree(hWnd, THRUSTERS, 0);
	return;
}

void DialogControl::ShowTheRightDialog(ItemType type) {
	//SB1->AnimEditingMode = false;
	if (type != MESH) {
		ShowWindow(hwnd_Mesh, SW_HIDE);
		SB1->MshMng->ResetHighlights();
	}
	if (type != DOCK) {
		ShowWindow(hWnd_Dock, SW_HIDE);
		if (SB1->DockExhaustsActive) {
			SB1->DeleteDockExhausts();
			SendDlgItemMessage(hWnd_Dock, IDC_CHECK_HIGHLIGHT_DOCK, BM_SETCHECK, BST_UNCHECKED, 0);
		}
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
	if (type != THRUSTERS) {
		ShowWindow(hWnd_Thr, SW_HIDE);
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
	case THRUSTERS:
	{
		ShowWindow(hWnd_Thr, SW_SHOW);
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
				SB1->MshMng->AddMeshDef();
				UpdateTree(hWnd, MESH,hrootMeshes);
			}
			else if (CurrentSelection.hitem == hrootDocks) {
				SB1->AddDockDef();
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
			else if (CurrentSelection.hitem == hrootThrusters) {
				ThrMng->AddThrDef();
				UpdateTree(hWnd, THRUSTERS, 0);
			}
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
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD MESH");
					}else if (CurrentSelection.hitem == hrootDocks) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD DOCK");
					}
					else if (CurrentSelection.hitem == hrootAttachments) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD ATTACHMENT");
					}
					else if (CurrentSelection.hitem == hrootLights) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD LIGHT");
					}
					else if (CurrentSelection.hitem == hrootCameras) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD CAMERA");
					}
					else if (CurrentSelection.hitem == hrootSettings) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_HIDE);
						//ADD SHOW SETTINGS DIALOG
					}
					else if (CurrentSelection.hitem == hrootAnimations) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD ANIMATION");
					}
					else if (CurrentSelection.hitem == hrootPropellant) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD PROPELLANT TANK");
					}
					else if (CurrentSelection.hitem == hrootThrusters) {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_SHOW);
						SetWindowText(GetDlgItem(hWnd, IDC_BUTTON_ADD), (LPCSTR)"ADD THRUSTER");
					}
					else {
						ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ADD), SW_HIDE);
					}



					ShowTheRightDialog(CurrentSelection.Type);


					switch (CurrentSelection.Type) {
						case MESH:
						{
							SB1->MshMng->ResetHighlights();
							LRESULT getcheck = SendDlgItemMessage(hwnd_Mesh, IDC_CHECK_HIGHLIGHT_MSH, BM_GETCHECK, 0, 0);
							if (getcheck == BST_CHECKED) {
								SB1->MshMng->HighlightMesh(SB1->MshMng->IdxDef2Msh(CurrentSelection.idx),true);
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
						case THRUSTERS:
						{
							UpdateThrDialog(hWnd_Thr);
							//TO DO
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

bool DialogControl::IsUintInVector(UINT u, vector<UINT>v) {
	for (UINT i = 0; i < v.size(); i++) {
		if (u == v[i]) {
			return true;
		}
	}
	return false;
}

double DialogControl::GetDlgItemDouble(HWND hWnd, int control_id) {
	char cbuf[256] = { '\0' };
	GetDlgItemText(hWnd, control_id, cbuf, 256);
	double val = atof(cbuf);
	return val;
}