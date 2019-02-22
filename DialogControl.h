#pragma once
#include <CommCtrl.h>

enum ItemType { MESH, DOCK, ATTACHMENT, LIGHT, CAMERA, SPECIAL, SETTINGS,ROOTS,NONE,ANIMATIONS,ANIM_COMP,PROPELLANT,THRUSTERS };
struct TREE_ITEM_REF {
	ItemType Type;
	UINT idx;
	HTREEITEM hitem;
	TREE_ITEM_REF() {
		Type = NONE;
		idx = (UINT)-1;
		hitem = NULL;
	}
};


class DialogControl {
public:
	DialogControl(StationBuilder1 *_SB1);
	~DialogControl();
	BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL MeshDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL DockDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AnimDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AnimCompDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AttDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL PrpDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ThrDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



	map<HTREEITEM, TREE_ITEM_REF> TreeItem;
	TREE_ITEM_REF CurrentSelection;

	HWND hDlg;
	void Open(HINSTANCE hDLL);
	void Close();
	bool IsOpen();
	void InitDialog(HWND hWnd);
	void InitTree(HWND hWnd);
	void MeshNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void DockNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	
	HTREEITEM hrootVessel;
	HTREEITEM hrootMeshes;
	HTREEITEM hrootDocks;
	HTREEITEM hrootAttachments;
	HTREEITEM hrootLights;
	HTREEITEM hrootCameras;
	HTREEITEM hrootAnimations;
	HTREEITEM hrootSettings;
	HTREEITEM hrootPropellant;
	HTREEITEM hrootThrusters;

	void SetDlgItemsTextVector3(HWND hWnd, int id1, int id2, int id3, VECTOR3 v3);
	VECTOR3 GetDlgItemsVector3(HWND hWnd, int id1, int id2, int id3);
	void DlgRotateMesh(UINT msh_idx,VECTOR3 axis, double angle );
	double speedrotation;
	void ShowTheRightDialog(ItemType type);
	void UpdateMeshDialog(HWND hWnd);
	void UpdateDockDialog(HWND hWnd);
	void UpdateAnimDialog(HWND hWnd);
	void UpdateAnimCompDialog(HWND hWnd);
	void UpdateAttDialog(HWND hWnd);
	void UpdatePrpDialog(HWND hWnd);
	void UpdateThrDialog(HWND hWnd);
	HTREEITEM FindHtreeItem(ItemType type, UINT idx);
	void InitAnimKeyCombo(HWND hWnd);
	void UpdateTree(HWND hWnd,ItemType type,HTREEITEM select);
	void InitOapiKeys();
	int ComboFindItemData(HWND hWnd, DWORD Data);
	int ComboFindItemData(HWND hWnd, void* data);
	void UpdateAnimCompGroupLists(HWND hWnd, def_idx meshdef_idx);
	void ShowAnimCompArmTip(HWND hWnd, bool show);
	bool IsUintInVector(UINT u, vector<UINT>v);
	HWND hwnd_Mesh;
	HWND hWnd_Dock;
	HWND hWnd_Anim;
	HWND hWnd_AnimComp;
	HWND hWnd_Atts;
	HWND hWnd_Prp;
	HWND hWnd_Thr;

	HWND GetDlg() { return hDlg; }
	map<DWORD, string> oapi_keys;
	AnimationManager *AnimMng;
	AttachmentManager *AttMng;
	PropellantManager *PrpMng;
	ThrusterManager *ThrMng;
	bool AnimTesting;
	double GetDlgItemDouble(HWND hWnd, int control_id);
//	HTREEITEM ItemToSelect;
protected:
	StationBuilder1 *SB1;
	bool open;
	
//	vector<MSH_DEF> md_restore;
	double test;
};
