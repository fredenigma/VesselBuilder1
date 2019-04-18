#pragma once

class VesselBuilderMFD :public MFD2 {
public:
	VesselBuilderMFD(DWORD w, DWORD h, VESSEL* vessel);
	~VesselBuilderMFD();
	char* ButtonLabel(int bt);
	int ButtonMenu(const MFDBUTTONMENU** menu) const;
	enum PAGE{INTRO_PAGE, DOCK_PAGE, ATTACHMENT_PAGE, ANIMATION_PAGE, LIGHTS_PAGE,CAMERA_PAGE,EVENT_PAGE,CONFIGURATION_PAGE}current_page;
	
	bool Update(oapi::Sketchpad* skp);
	bool UpdateIntroPage(oapi::Sketchpad* skp, int line);
	bool UpdateDockPage(oapi::Sketchpad* skp, int line);
	bool UpdateAttachmentPage(oapi::Sketchpad* skp, int line);
	static int MsgProc(UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam);
	bool ConsumeButton(int bt, int event);
	DWORD H, W;
	DWORD rgb(int r, int g, int b);
	void StoreStatus(void) const;
	void RecallStatus(void);
	void SwitchPage(PAGE newpg);
protected:
	VesselBuilder1* VB1;
	static struct SavePrm {
		PAGE _current;
	} saveprm;
	int margin;
	oapi::Pen *penwhite;
	int CurrentSelection;
	int CurrentScroll;
	UINT nLines;
};