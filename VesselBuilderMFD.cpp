#include "VesselBuilder1.h"
#include "DockManager.h"
#include "Orbitersdk.h"
#include "VesselBuilderMFD.h"


using namespace std;

VesselBuilderMFD::VesselBuilderMFD(DWORD w, DWORD h, VESSEL* vessel) :MFD2(w, h, vessel) {
	H = h;
	W = w;
	VB1 = (VesselBuilder1*)vessel;
	current_page = INTRO_PAGE;
	penwhite = oapiCreatePen(1, 1, 0xFFFFFF);
	CurrentSelection = 0;
	CurrentScroll = 0;
	nLines = 18;
}
VesselBuilderMFD::~VesselBuilderMFD(){
	VB1 = NULL;
	oapiReleasePen(penwhite);
}

DWORD VesselBuilderMFD::rgb(int r, int g, int b) {
	DWORD rgb;
	b = b * 256 * 256;
	g = g * 256;
	int i_rgb = b + g + r;
	rgb = static_cast<DWORD>(i_rgb);
	return rgb;
}

char* VesselBuilderMFD::ButtonLabel(int bt) {
	switch (current_page) {
	case INTRO_PAGE:
	{
		char* label[7] = { "DCK","ATT","ANI","LGT","CAM","EVT","CFG" };
		return (bt < 7 ? label[bt] : 0);
		break;
	}
	case DOCK_PAGE:
	{
		char* label[12] = { "UP","DWN","","","","","UND","","","","","BCK" };
		return (bt < 12 ? label[bt] : 0);
	}
	case ATTACHMENT_PAGE:
	{
		char* label[12] = { "UP","DWN","","","","","UND","","","","","BCK" };
		return (bt < 12 ? label[bt] : 0);
	}
	}
}
int VesselBuilderMFD::ButtonMenu(const MFDBUTTONMENU** menu) const
{
	static const MFDBUTTONMENU intromnu[7] = {
		{"Docking","Ports",0},
		{"Attachments","Points",0},
		{"Animations",0,0},
		{"Lights",0,0},
		{"Cameras",0,0},
		{"Events",0,0},
		{"Configurations",0,0}
	};
	static const MFDBUTTONMENU dockmnu[12] = {
		{"Select","Previous",0},
		{"Select","Next",0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{"Undock","Selected",0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{"Back to","Main Page",0}
	};
	if (menu) {
		switch (current_page) {
		case INTRO_PAGE:
		{
			*menu = intromnu;
			return 7;
		}
		case DOCK_PAGE:
		{
			*menu = dockmnu;
			return 12;
		}
		case ATTACHMENT_PAGE:
		{
			*menu = dockmnu;
			return 12;
		}
		}
	}
}
void VesselBuilderMFD::SwitchPage(PAGE newpg) {
	current_page = newpg;
	InvalidateButtons();
	CurrentScroll = 0;
	CurrentSelection = 0;
	return;
}
bool VesselBuilderMFD::ConsumeButton(int bt, int event)
{
	if (!(event & PANEL_MOUSE_LBDOWN)) return false;
	switch (current_page) {
	case INTRO_PAGE:
	{
		switch (bt) {
		case 0:
		{
			SwitchPage(DOCK_PAGE);
			break;
		}
		case 1:
		{
			SwitchPage(ATTACHMENT_PAGE);
			break;
		}
		}
		break;
	}
	case DOCK_PAGE:
	{
		switch (bt) {
		case 0:
		{
			CurrentSelection--;
			if (CurrentSelection < (CurrentScroll)) {
				CurrentScroll--;
				CurrentSelection = CurrentScroll;
				if (CurrentSelection < 0) {
					CurrentSelection = 0;
					CurrentScroll = 0;
				}
				
			}
			break;
		}
		case 1:
		{
			CurrentSelection++;
			if (CurrentSelection > (CurrentScroll + nLines-1)) {
				CurrentScroll++;
				CurrentSelection = CurrentScroll + nLines-1;
				
			}
			UINT dock_count = VB1->DckMng->GetDockCount();
			if (CurrentSelection >= dock_count) {
				CurrentSelection = dock_count - 1;
			}
			break;
		}
		case 6:
		{
			
			OBJHANDLE hves = VB1->GetDockStatus(VB1->DckMng->GetDH(CurrentSelection));
			if (hves == NULL) { break; }
			VB1->Undock(VB1->DckMng->GetOrbiterDockIdx(VB1->DckMng->GetDH(CurrentSelection)));
			break;
		}
		case 11:
		{
			SwitchPage(INTRO_PAGE);
			break;
		}
		}
		break;
	}
	}
}

bool VesselBuilderMFD::Update(oapi::Sketchpad* skp) {
	margin = 5;
	Title(skp, "Vessel Builder MFD");
	skp->SetTextColor(rgb(255, 255, 255));
	DWORD charsize = skp->GetCharSize();
	WORD c_height = LOWORD(charsize);
	WORD c_width = HIWORD(charsize);
	int line = (int)((H / c_height)*1.5);
	switch (current_page) {
	case INTRO_PAGE:
	{
		return UpdateIntroPage(skp, line);
	}
	case DOCK_PAGE:
	{
		return UpdateDockPage(skp, line);
	}
	case ATTACHMENT_PAGE:
	{
		return UpdateAttachmentPage(skp, line);
	}
	}
	
	
	return true;
}
bool VesselBuilderMFD::UpdateIntroPage(oapi::Sketchpad* skp, int line) {
	skp->SetTextColor(rgb(156, 156, 156));
	NEWCHAR(cbuf);
	sprintf(cbuf, "Vessel Name: %s", VB1->GetName());
	skp->Text(margin, line * 2, cbuf, strlen(cbuf));
	sprintf(cbuf, "VesselBuilder Version: %i", VBVERSION);
	skp->Text(margin, line * 4, cbuf, strlen(cbuf));
	skp->SetTextAlign(oapi::Sketchpad::CENTER, oapi::Sketchpad::TAlign_vertical::BASELINE);
	skp->Text(W/2, line * 8, "<-- Choose Page -->", strlen("<-- Choose Page -->"));
	return true;
}
bool VesselBuilderMFD::UpdateDockPage(oapi::Sketchpad* skp, int line) {
	Title(skp, "Vessel Builder MFD : Docking Ports");
	//skp->SetPen(penwhite);
	//skp->Rectangle(margin, line * 2, W - margin, H - margin);
	UINT DockCount = VB1->DckMng->GetDockCount();
	if (DockCount <= 0) {
		return true;
	}
	for (UINT i = CurrentScroll; i < CurrentScroll+nLines; i++) {
		if (i >= DockCount) { break; }

		NEWCHAR(cbuf);
		OBJHANDLE hves = VB1->GetDockStatus(VB1->DckMng->GetDH(i));
		sprintf(cbuf, "Port: %s - %s", VB1->DckMng->GetDockName(i).c_str(),hves==NULL ? "FREE":oapiGetVesselInterface(hves)->GetName());
		skp->SetTextColor(i == CurrentSelection ? rgb(255, 255, 255) : rgb(120, 120, 120));
		skp->Text(5, line * ((i-CurrentScroll) + 2), cbuf, strlen(cbuf));
	}
	return true;
}
bool VesselBuilderMFD::UpdateAttachmentPage(oapi::Sketchpad* skp, int line) {
	Title(skp, "Vessel Builder MFD : Attachments");
	for (UINT i = 0; i < 50; i++) {
		NEWCHAR(cbuf);
		sprintf(cbuf, "Line:%i", i);
		skp->SetTextColor(rgb(255, 255, 255));
		skp->Text(5, line * (i + 2), cbuf, strlen(cbuf));
	}
	return true;
}
int VesselBuilderMFD::MsgProc(UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case OAPI_MSG_MFD_OPENED:
		// Our new MFD mode has been selected, so we create the MFD and
		// return a pointer to it.
		//SN = SpaceN;
		return (int)(new VesselBuilderMFD(LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam));
	}
	return 0;
}

void VesselBuilderMFD::StoreStatus(void) const
{
	saveprm._current = current_page;
	return;
}
void VesselBuilderMFD::RecallStatus(void)
{
	current_page = saveprm._current;
	return;
}

VesselBuilderMFD::SavePrm VesselBuilderMFD::saveprm = { INTRO_PAGE };