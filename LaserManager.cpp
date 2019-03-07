
#include "VesselBuilder1.h"
#include "LaserManager.h"



Laser::Laser(VesselBuilder1 *_VB1, VECTOR3* pos_ptr, VECTOR3* dir_ptr, int tex, double lsize, double wsize){
	VB1 = _VB1;
	EXHAUSTSPEC es;
	double level = 1;
	es.flags = EXHAUST_CONSTANTLEVEL;
	es.th = NULL;
	if (tex == LASER_GREEN_TEX) {
		es.tex = VB1->greenL;
	}
	else if (tex == LASER_BLUE_TEX) {
		es.tex = VB1->blueL;
	}
	else if (tex == LASER_RED_TEX) {
		es.tex = VB1->redL;
	}
	es.lsize = lsize;
	es.wsize = wsize;
	es.level = &level;
	es.lpos = pos_ptr;
	es.ldir = dir_ptr;
	es.modulate = 0;
	es.lofs = 0;
	ID.clear();
	ID.push_back(VB1->AddExhaust(&es));
	
	return;
}
Laser::Laser(VesselBuilder1 *_VB1, VECTOR3* pos_ptr, VECTOR3* dir_ptr, VECTOR3 *rot_ptr, int tex1, int tex2, double lsize, double wsize ) {
	VB1 = _VB1;
	EXHAUSTSPEC es;
	double level = 1;
	es.flags = EXHAUST_CONSTANTLEVEL;
	es.th = NULL;
	if (tex1 == LASER_GREEN_TEX) {
		es.tex = VB1->greenL;
	}
	else if (tex1 == LASER_BLUE_TEX) {
		es.tex = VB1->blueL;
	}
	else if (tex1 == LASER_RED_TEX) {
		es.tex = VB1->redL;
	}
	es.lsize = lsize;
	es.wsize = wsize;
	es.level = &level;
	es.lpos = pos_ptr;
	es.ldir = dir_ptr;
	es.modulate = 0;
	es.lofs = 0;
	ID.clear();
	ID.push_back(VB1->AddExhaust(&es));
	
	if (tex2 == LASER_GREEN_TEX) {
		es.tex = VB1->greenL;
	}
	else if (tex2 == LASER_BLUE_TEX) {
		es.tex = VB1->blueL;
	}
	else if (tex2 == LASER_RED_TEX) {
		es.tex = VB1->redL;
	}
	es.ldir = rot_ptr;
	ID.push_back(VB1->AddExhaust(&es));
	
}
Laser::Laser(VesselBuilder1 *_VB1, VECTOR3 pos, int tex, double lsize, double wsize ) {
	VB1 = _VB1;
	EXHAUSTSPEC es;
	double level = 1;
	VECTOR3 xp = _V(1, 0, 0);
	VECTOR3 xm = _V(-1, 0, 0);
	VECTOR3 yp = _V(0, 1, 0);
	VECTOR3 ym = _V(0, -1, 0);
	VECTOR3 zp = _V(0, 0, 1);
	VECTOR3 zm = _V(0, 0, -1);
	VECTOR3 lpos = pos;
	es.flags = EXHAUST_CONSTANTLEVEL | EXHAUST_CONSTANTDIR | EXHAUST_CONSTANTPOS;
	es.th = NULL;
	if (tex == LASER_GREEN_TEX) {
		es.tex = VB1->greenL;
	}
	else if (tex == LASER_BLUE_TEX) {
		es.tex = VB1->blueL;
	}
	else if (tex == LASER_RED_TEX) {
		es.tex = VB1->redL;
	}
	es.lsize = lsize;
	es.wsize = wsize;
	es.level = &level;
	es.lpos = &lpos;
	es.ldir = &xp;
	es.modulate = 0;
	es.lofs = 0;
	ID.clear();
	ID.push_back(VB1->AddExhaust(&es));
	

	es.ldir = &xm;
	ID.push_back(VB1->AddExhaust(&es));
	

	es.ldir = &yp;
	ID.push_back(VB1->AddExhaust(&es));
	
	es.ldir = &ym;
	ID.push_back(VB1->AddExhaust(&es));
	
	es.ldir = &zp;
	ID.push_back(VB1->AddExhaust(&es));
	
	es.ldir = &zm;
	ID.push_back(VB1->AddExhaust(&es));
	return;

}
Laser::~Laser(){
	VB1 = NULL;
}
void Laser::Delete() {
	for (UINT i = 0; i < ID.size(); i++) {
		VB1->DelExhaust(ID[i]);
	}
}
/*Laser(VesselBuilder1 *VB1,VECTOR3* pos_ptr, VECTOR3* dir_ptr, VECTOR3 *rot_ptr, int tex1, int tex2, double lsize = 100, double wsize = 0.005);
~Laser();
UINT ID;
*/
LaserManager::LaserManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	return;
}
LaserManager::~LaserManager() {
	VB1 = NULL;
}
LASER_HANDLE LaserManager::CreateLaser(VECTOR3* pos_ptr, VECTOR3* dir_ptr, int tex, double lsize, double wsize) {
	Laser* Ls = new Laser(VB1, pos_ptr, dir_ptr, tex, lsize, wsize);
	return Ls;
}
LASER_HANDLE LaserManager::CreateLaserL(VECTOR3* pos_ptr, VECTOR3* dir_ptr, VECTOR3 *rot_ptr, int tex1,int tex2, double lsize, double wsize ) {
	Laser* Ls = new Laser(VB1, pos_ptr, dir_ptr, rot_ptr, tex1, tex2, lsize, wsize);
	return Ls;
}
LASER_HANDLE LaserManager::CreateFixedLaserStar(VECTOR3 pos, int tex, double lsize , double wsize ) {
	Laser* Ls = new Laser(VB1, pos, tex, lsize, wsize);
	return Ls;
}
void LaserManager::DeleteLaser(LASER_HANDLE &lh){
	((Laser*)lh)->Delete();
	delete (Laser*)lh;
	(Laser*)lh = NULL;
	return;
}
