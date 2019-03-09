#pragma once
#include <CommCtrl.h>

enum ItemType { MESH, DOCK, ATTACHMENT, LIGHTS, CAMERA, SPECIAL, SETTINGS,ROOTS,NONE,ANIMATIONS,ANIM_COMP,PROPELLANT,EXTEX,THRUSTERS,THRUSTERGROUPS,PARTICLES,TOUCHDOWNPOINTS,AIRFOILS,CTRLSURFACES,VCPOS,VCMFD,VCHUD,BEACONS,VARIABLEDRAG };
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
	DialogControl(VesselBuilder1 *_VB1);
	~DialogControl();
	BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL MeshDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL DockDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AnimDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AnimCompDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AttDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL PrpDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ThrDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ExTexDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL ThrGrpDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL PartDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL TdpDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL SettingsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL AirfoilsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL CtrlSurfDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL CamDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL VCPosDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL VCHUDDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL VCMFDDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL BeaconsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL LightCreationDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL LightsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL VarDragDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	

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
	HTREEITEM hrootExTex;
	HTREEITEM hrootThrusterGroups;
	HTREEITEM hrootParticles;
	HTREEITEM hrootTouchdownPoints;
	HTREEITEM hrootAirfoils;
	HTREEITEM hrootControlSurfaces;
	HTREEITEM hrootVC;
	HTREEITEM hrootVCPositions;
	HTREEITEM hrootVCMFDs;
	HTREEITEM hrootVCHud;
	HTREEITEM hrootBeacons;
	HTREEITEM hrootLightEmitters;
	HTREEITEM hrootVariableDrag;
	

	void SetDlgItemsTextVector3(HWND hWnd, int id1, int id2, int id3, VECTOR3 v3, int precision = 3);
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
	void UpdateExTexDialog(HWND hWnd);
	void UpdateThrDialog(HWND hWnd);
	void UpdateThrGrpDialog(HWND hWnd);
	void UpdatePartDialog(HWND hWnd);
	void UpdateTdpDialog(HWND hWnd);
	void UpdateSettingsDialog(HWND hWnd);
	void UpdateAirfoilDialog(HWND hWnd);
	void UpdateCtrlSurfDialog(HWND hWnd);
	void UpdateCamDialog(HWND hWnd);
	void UpdateVCPosDialog(HWND hWnd);
	void UpdateVCHUDDialog(HWND hWnd);
	void UpdateVCMFDDialog(HWND hWnd);
	void UpdateBeaconsDialog(HWND hWnd);
	void UpdateLightsDialog(HWND hWnd);
	void UpdateVarDragDialog(HWND hWnd);


	HTREEITEM FindHtreeItem(ItemType type, UINT idx);
	void InitAnimKeyCombo(HWND hWnd);
	void UpdateTree(HWND hWnd,ItemType type,HTREEITEM select);
	void InitOapiKeys();
	int ComboFindItemData(HWND hWnd, DWORD Data);
	int ComboFindItemData(HWND hWnd, void* data);
	void UpdateAnimCompGroupLists(HWND hWnd, def_idx meshdef_idx);
	void UpdateAirfoilFuncList(HWND hWnd);
	void ShowAirfoilFuncGraph(HWND hWnd, bool show);
	bool ShowingAFGraph;
	void UpdateAirfoilFuncGraph(HWND hWnd);
	void ShowAnimCompArmTip(HWND hWnd, bool show);
	void EnableVCHudWindows(HWND hWnd, bool enable);
	void UpdateColorExamples(HWND hWnd,VECTOR3 col);
	void UpdateAttachmentListBox(HWND hWnd, int id,UINT CurrAtt);
	bool ShowingExhausts;
	void ShowExhaustsWin(HWND hWnd, bool show);
	void UpdateExhaustWin(HWND hWnd, UINT idx);
	map<UINT, LASER_HANDLE>DockLaserMap;
	map<UINT, LASER_HANDLE>AttLaserMap;
	map<UINT, LASER_HANDLE>ThLaserMap;
	map<UINT, LASER_HANDLE>TdpLaserMap;
	UINT TdpCurOn[2];
	UINT TdpSetOn[2];
	HWND hwnd_Mesh;
	HWND hWnd_Dock;
	HWND hWnd_Anim;
	HWND hWnd_AnimComp;
	HWND hWnd_Atts;
	HWND hWnd_Prp;
	HWND hWnd_ExTex;
	HWND hWnd_Thr;
	HWND hWnd_ThrGrp;
	HWND hWnd_Part;
	HWND hWnd_Tdp;
	HWND hWnd_Settings;
	HWND hWnd_Airfoils;
	HWND hWnd_CtrlSurfaces;
	HWND hWnd_Cam;
	HWND hWnd_VCPos;
	HWND hWnd_VCHud;
	HWND hWnd_VCMFD;
	HWND hWnd_Beacons;
	HWND hWnd_LightCreation;
	HWND hWnd_Lights;
	HWND hWnd_VarDrag;
	

	HWND GetDlg() { return hDlg; }
	map<DWORD, string> oapi_keys;
	MeshManager *MshMng;
	DockManager *DckMng;
	AnimationManager *AnimMng;
	AttachmentManager *AttMng;
	PropellantManager *PrpMng;
	ThrusterManager *ThrMng;
	ThrusterGroupManager *ThrGrpMng;
	ParticleManager *PartMng;
	TouchdownPointsManager *TdpMng;
	AirfoilsManager *AirfoilMng;
	ControlSurfacesManager *CtrSurfMng;
	CameraManager *CamMng;
	ExTexManager *ExTMng;
	VCManager *VCMng;
	LightsManager *LightsMng;
	VariableDragManager *VardMng;
	
	bool AnimTesting;
	double GetDlgItemDouble(HWND hWnd, int control_id);
	void SetDlgItemDouble(HWND hWnd, int control_id, double val, UINT precision);
	bool IsCheckBoxChecked(HWND hWnd, int control_id);
	HPEN penblack;
	HPEN pengray;
	HPEN penblue_l;
	void ClearLasers(map<UINT, LASER_HANDLE> &m);
//	HTREEITEM ItemToSelect;
protected:
	VesselBuilder1 *VB1;
	bool open;
	
//	vector<MSH_DEF> md_restore;
	double test;
};
