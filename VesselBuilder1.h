

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

#define VBVERSION (((YEAR - 2000) * 100 + MONTH) * 100 + DAY)

#define HIGHLIGHTCOLORGREEN 0
#define HIGHLIGHTCOLORBLUE 1
#define HIGHLIGHTCOLORRED 2

#define SETTINGS_SECTION 0
#define MESH_SECTION 1
#define DOCK_SECTION 2
#define ATT_SECTION 3
#define ANIM_SECTION 4
#define PROP_SECTION 5
#define EXTEX_SECTION 6
#define PART_SECTION 7
#define THRUST_SECTION 8
#define TDP_SECTION 9
#define AIRFOIL_SECTION 10
#define CTRLSURF_SECTION 11
#define LIGHTS_SECTION 12
#define CAMERAS_SECTION 13
#define VC_SECTION 14
#define VARDRAG_SECTION 15
#define EVENTS_SECTION 16
#define N_SECTIONS 17

#include <Orbitersdk.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <bitset>
#include "Log.h"
using namespace std;

typedef UINT anim_idx;
typedef UINT msh_idx;
typedef UINT def_idx;
class Laser;
class Event;
typedef Laser* LASER_HANDLE;
typedef Event* EVENT_HANDLE;

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




class Logger;



class DialogControl;
class FollowMeDlg;
class TextReaderDlg;
class AnimCompDef;
class MeshManager;
class DockManager;
class AttachmentManager;
class AnimationManager;
class PropellantManager;
class ThrusterManager;
class ThrusterGroupManager;
class ParticleManager;
class TouchdownPointsManager;
class AirfoilsManager;
class ControlSurfacesManager;
class CameraManager;
class ExTexManager;
class VCManager;
class LightsManager;
class LaserManager;
class VariableDragManager;
class EventManager;

class MET;

class VesselBuilder1 :public VESSEL4{
public:
 VesselBuilder1(OBJHANDLE hObj,int fmodel);
 ~VesselBuilder1();
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
 bool clbkVCMouseEvent(int id, int event, VECTOR3 &p);
 void clbkDockEvent(int dock, OBJHANDLE mate);


 MeshManager* MshMng;
 DockManager* DckMng;
 AttachmentManager* AttMng;
 AnimationManager* AnimMng;
 PropellantManager* PrpMng;
 ThrusterManager* ThrMng;
 ThrusterGroupManager *ThrGrpMng;
 ParticleManager *PartMng;
 TouchdownPointsManager *TdpMng;
 AirfoilsManager *AirfoilMng;
 ControlSurfacesManager *CtrSurfMng;
 CameraManager *CamMng;
 ExTexManager *ExTMng;
 VCManager *VCMng;
 LightsManager *LightsMng;
 LaserManager *Laser;
 VariableDragManager *VardMng;
 Logger* Log;
 
 MET *Met;
 EventManager *EvMng;


 VISHANDLE visual;
 


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
 void ToggleFollowMeSuprePrecision();
 bool FollowMeSuperPrecision;

 DialogControl *Dlg;
 FollowMeDlg *FMDlg;
 TextReaderDlg *TextDlg;
 
 string OrbiterRoot;
 void GetOrbiterDirs();
 string meshdir;
 string configdir;
 string texturedir;
 string texture2dir;
 string scenariodir;
 SURFHANDLE greenL, redL, blueL, whiteL;
 
 vector<DOCKHANDLE>docks_to_del;
 vector<UINT>docks_jettisoned;

 string WriteVectorUINT(vector<UINT> v,bool spaces=true);
 vector<UINT> readVectorUINT(string s);
 bool IsUintInVector(UINT u, vector<UINT>v);

 bool UsingD3D9();
 bool wD3D9;
 MATRIX3 Inverse(MATRIX3 m);
 bool AreVector3Equal(VECTOR3 v1, VECTOR3 v2);
 bool GrabMode;
 int currentGrabAtt;
 bool ToggleGrapple();
 bool NoEditor;
 
 double level1;
 
 void AddDefaultRCS();
 double GetMET();
 bool JettisonNextDock();
 bool JettisonDock(UINT idx);
};



