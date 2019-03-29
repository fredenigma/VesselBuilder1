#pragma once

#define LASER_GREEN_TEX 0
#define LASER_BLUE_TEX 1
#define LASER_RED_TEX 2



class Laser {
public:
	Laser(VesselBuilder1 *_VB1, VECTOR3* pos_ptr, VECTOR3* dir_ptr, int tex, double lsize = 100, double wsize = 0.005);
	Laser(VesselBuilder1 *_VB1, VECTOR3* pos_ptr, VECTOR3* dir_ptr, VECTOR3 *rot_ptr, int tex1, int tex2, double lsize = 100, double wsize = 0.005);
	//Laser(VesselBuilder1 *_VB1, VECTOR3* pos_ptr, int tex, double lsize = 100, double wsize = 0.005);
	Laser(VesselBuilder1 *_VB1, VECTOR3 pos, int tex, double lsize = 100, double wsize = 0.005);
	~Laser();
	void Delete();
	VesselBuilder1* VB1;
	VBVector<UINT> ID;
};

class LaserManager {
public:
	LaserManager(VesselBuilder1 *_VB1);
	~LaserManager();
	VesselBuilder1* VB1;
	LASER_HANDLE CreateLaser(VECTOR3* pos_ptr, VECTOR3* dir_ptr, int tex, double lsize=100, double wsize=0.005);
	LASER_HANDLE CreateFixedLaserStar(VECTOR3 pos, int tex, double lsize = 100, double wsize = 0.005);
	LASER_HANDLE CreateLaserL(VECTOR3* pos_ptr, VECTOR3* dir_ptr, VECTOR3 *rot_ptr, int tex1,int tex2, double lsize = 100, double wsize = 0.005);
	void DeleteLaser(LASER_HANDLE &LH);
};