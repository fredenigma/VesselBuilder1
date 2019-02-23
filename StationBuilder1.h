

#define YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
+ (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))


#define MONTH (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
: __DATE__ [2] == 'b' ? 2 \
: __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
: __DATE__ [2] == 'y' ? 5 \
: __DATE__[2] == 'l' ? 7 \
: __DATE__[2] == 'g' ? 8 \
: __DATE__[2] == 'p' ? 9 \
: __DATE__[2] == 't' ? 10 \
: __DATE__[2] == 'v' ? 11 : 12)

#define DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
+ (__DATE__ [5] - '0'))

#define SBVERSION (((YEAR - 2000) * 100 + MONTH) * 100 + DAY)

#define HIGHLIGHTCOLORGREEN 0
#define HIGHLIGHTCOLORBLUE 1
#define HIGHLIGHTCOLORRED 2


#include <Orbitersdk.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Log.h"
using namespace std;

typedef UINT anim_idx;
typedef UINT msh_idx;
typedef UINT def_idx;

struct V_CLIPBOARD {
	VECTOR3 pos;
	VECTOR3 dir;
	VECTOR3 rot;
	bool valid;
	V_CLIPBOARD() {
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 1);
		rot = _V(0, 1, 0);
		valid = false;
	}
};




struct DCK_DEF {
	VECTOR3 pos;
	VECTOR3 dir;
	VECTOR3 rot;
	VECTOR3 antidir;
	VECTOR3 antirot;
	DOCKHANDLE dh;
	DCK_DEF() {
		pos = _V(0, 0, 0);
		dir = _V(0, 0, 1);
		antidir = _V(0, 0, -1);
		rot = _V(0, 1, 0);
		antirot = _V(0, -1, 0);
		dh = NULL;
	}
};

struct EXTEX_DEF {
	string TexName;
	SURFHANDLE tex;
	bool created;
	EXTEX_DEF() {
		TexName.clear();
		tex = NULL;
		created = false;
	}
};

class DialogControl;
class FollowMeDlg;
class AnimCompDef;
class MeshManager;
class AttachmentManager;
class AnimationManager;
class PropellantManager;
class ThrusterManager;

class StationBuilder1 :public VESSEL4{
public:
StationBuilder1(OBJHANDLE hObj,int fmodel);
 ~StationBuilder1();
 void clbkSetClassCaps(FILEHANDLE cfg);
 void clbkLoadStateEx(FILEHANDLE scn,void *vs);
 void clbkSaveState(FILEHANDLE scn);
 void clbkPostStep(double simt, double simdt, double mjd);
 void clbkPreStep(double simt, double simdt, double mjd);
 int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
 int clbkConsumeDirectKey(char *kstate);
 void clbkPostCreation();
 void clbkVisualCreated(VISHANDLE  vis, int  refcount);
 void clbkVisualDestroyed(VISHANDLE  vis, int  refcount);
 bool clbkLoadVC(int id);
 
 void CreateDocks();

 MeshManager* MshMng;
 AttachmentManager* AttMng;
 AnimationManager* AnimMng;
 PropellantManager* PrpMng;
 ThrusterManager* ThrMng;
 VISHANDLE visual;
 
 vector <DCK_DEF> dock_definitions;
 vector <EXTEX_DEF> extex_defs;
 void AddExTexDef();
 void AddExTexDef(string texname);
 bool StoreExTexDef(string texname,def_idx d_idx);
 void DelExTedDef(def_idx d_idx);
 SURFHANDLE GetExTexSurf(def_idx d_idx);
 string GetExTexName(def_idx d_idx);
 UINT GetExTexCount();
 bool IsExTexCreated(def_idx d_idx);
 UINT GetExTexIdx(SURFHANDLE tex);
// vector <ATT_DEF> att_definitions;
 //void WriteUniqueMeshFile(string filename);
 void WriteCfgFile(string filename);
 void WriteBackupFile();
 
 vector<string> ReadMeshTextures(string filename);
 void ResetVehicle();
 void ParseCfgFile(FILEHANDLE cfg);

 MATRIX3 FindRM(VECTOR3 dir1, VECTOR3 rot1, VECTOR3 dir2, VECTOR3 rot2);
 MATRIX3 FindRM(VECTOR3 dir2, VECTOR3 rot2);
 void FindDirRot(MATRIX3 rm, VECTOR3 &dir, VECTOR3 &rot);
 void VehicleSetup();

 VECTOR3 follow_me_pos;
 VECTOR3 follow_me_dir;
 VECTOR3 follow_me_rot;
 MATRIX3 follow_me_rm;
 bool follow_me;
 double follow_me_translation_speed;
 double follow_me_rotation_speed;
 void CreateFollowMe();
 void CreateFollowMe(VECTOR3 initpos);
 void DeleteFollowMe(bool reset=false);
 void UpdateFollowMe();
 void ResetFollowMe();
 void MoveFollowMe(VECTOR3 axis);
 void RotateFollowMe(VECTOR3 axis);
 OBJHANDLE h_follow_me;
 NOTEHANDLE follow_me_noteh;
 V_CLIPBOARD vclip;
 string cfgfilename;
 void ConsumeFollowMeKey(char *kstate);

 //HWND hDlg;

 DialogControl *Dlg;
 FollowMeDlg *FMDlg;

 string OrbiterRoot;
 void GetOrbiterDirs();
 string meshdir;
 string configdir;
 string texturedir;
 string texture2dir;
 string scenariodir;
 

 void AddDockDef();
 void AddDockDef(DCK_DEF dd);
 bool DeleteDockDef(int idx);
 //void AddAttDef();
 //void AddAttDef(ATT_DEF ad);
 //bool DeleteAttDef(int idx);

 
// BEACONSPOTS *dockbcn;
// void CreateDockBeacons();
// void DeleteDockBeacons();
// void UpdateDockBeaconsPos();
// bool DockBeaconsActive;
// VECTOR3 colwhite;
// VECTOR3 colblue;
// VECTOR3 colgreen;
// VECTOR3 colred;
 bool DockExhaustsActive;
 vector<UINT> DockExhaustsID;
 SURFHANDLE greenL, redL, blueL, whiteL;
 void CreateDockExhausts();
 void DeleteDockExhausts();
 bool AttExhaustsActive;
 vector<UINT> AttExhaustsID;
 void CreateAttExhausts();
 void DeleteAttExhausts();
 bool thExhaustsActive;
 vector<UINT>ThExaustsID;
 void CreateThExhausts();
 void DeleteThExhausts();
 
 bool UsingD3D9();
 bool wD3D9;
 MATRIX3 Inverse(MATRIX3 m);
 bool AreVector3Equal(VECTOR3 v1, VECTOR3 v2);
 bool GrabMode;
 int currentGrabAtt;
 bool ToggleGrapple();
 bool NoEditor;
 
 
 /*
 vector<ANIM_COMPDEF> animcomps_definitions;
 vector<ANIM_DEF> animations_definitions;
 vector<MGROUP_ROTATE*> mgr;
 void DefineAnimations();
 void ClearDelete(vector<MGROUP_ROTATE*>vmgr);
 */

 

};



