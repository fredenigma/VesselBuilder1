#include "VesselBuilder1.h"
#include "DialogControl.h"
#include "PropellantManager.h"

#define LogV(x,...) VB1->Log->Log(x,##__VA_ARGS__)

PropellantManager::PropellantManager(VesselBuilder1 *_VB1) {
	VB1 = _VB1;
	tanks.clear();
	return;
}
PropellantManager::~PropellantManager(){}

void PropellantManager::AddTankDef() {
	UINT n = tanks.size();
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "Tank_%i", n);
	string name(cbuf);
	return AddTankDef(name,1000,1,-1);
}
void PropellantManager::AddTankDef(string name, double MaxMass, double efficiency, double currentMass) {
	LogV("Adding Tank:%s MaxMass:%.3f efficiency:%.3f CurrentMass:%.3f", name.c_str(), MaxMass, efficiency, currentMass);
	PRP_DEF tank = PRP_DEF();
	tank.name = name;
	tank.ph = VB1->CreatePropellantResource(MaxMass, currentMass, efficiency);
	tanks.push_back(tank);
	return;
}
void PropellantManager::DelTankDef(def_idx d_idx) {
	LogV("Deleting Tank:%i", d_idx);
	VB1->DelPropellantResource(tanks[d_idx].ph);
	tanks.erase(tanks.begin() + d_idx);
	return;
}
string PropellantManager::GetTankName(def_idx d_idx) {
	return tanks[d_idx].name;
}
double PropellantManager::GetTankMaxMass(def_idx d_idx) {
	return VB1->GetPropellantMaxMass(tanks[d_idx].ph);
}
double PropellantManager::GetTankCurrentMass(def_idx d_idx) {
	return VB1->GetPropellantMass(tanks[d_idx].ph);
}
double PropellantManager::GetTankEfficiency(def_idx d_idx) {
	return VB1->GetPropellantEfficiency(tanks[d_idx].ph);
}
void PropellantManager::SetTankName(def_idx d_idx, string newname) {
	tanks[d_idx].name = newname;
	return;
}
void PropellantManager::SetTankMaxMass(def_idx d_idx, double newmaxmass) {
	VB1->SetPropellantMaxMass(tanks[d_idx].ph, newmaxmass);
	return;
}
void PropellantManager::SetTankCurrentMass(def_idx d_idx, double newcurrentmass) {
	VB1->SetPropellantMass(tanks[d_idx].ph, newcurrentmass);
	return;
}
void PropellantManager::SetTankEfficiency(def_idx d_idx, double newefficiency) {
	VB1->SetPropellantEfficiency(tanks[d_idx].ph, newefficiency);
	return;
}
DWORD PropellantManager::GetPropCount() {
	return VB1->GetPropellantCount();
}
void PropellantManager::MakePrimary(def_idx d_idx) {
	VB1->SetDefaultPropellantResource(tanks[d_idx].ph);
	return;
}
bool PropellantManager::IsPrimary(def_idx d_idx) {
	if (VB1->GetDefaultPropellantResource() == tanks[d_idx].ph) {
		return true;
	}
	else {
		return false;
	}
	
}
PROPELLANT_HANDLE PropellantManager::GetTankPH(def_idx d_idx) {
	return tanks[d_idx].ph;
}
PROPELLANT_HANDLE PropellantManager::GetMainTankPH() {
	for (UINT i = 0; i < tanks.size(); i++) {
		if (IsPrimary(i)) {
			return tanks[i].ph;
		}
	}
	return NULL;
}
int PropellantManager::GetPrpIdx(PROPELLANT_HANDLE ph) {
	for (UINT i = 0; i < tanks.size(); i++) {
		if (ph == tanks[i].ph) {
			return i;
		}
	}
	return -1;
}

void PropellantManager::ParseCfgFile(FILEHANDLE fh) {
	LogV("Parsing Tank Section");
	UINT prp_counter = 0;
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "PRP_%i_ID", prp_counter);
	int id;
	while (oapiReadItem_int(fh, cbuf, id)) {
		double maxmass = 0;
		double efficiency = 1;
		bool primary = false;
		char namebuf[256] = { '\0' };
		sprintf(cbuf, "PRP_%i_MAXMASS", prp_counter);
		oapiReadItem_float(fh, cbuf, maxmass);
		sprintf(cbuf, "PRP_%i_EFFICIENCY", prp_counter);
		oapiReadItem_float(fh, cbuf, efficiency);
		sprintf(cbuf, "PRP_%i_NAME", prp_counter);
		oapiReadItem_string(fh, cbuf, namebuf);
		string name(namebuf);
		AddTankDef(name, maxmass, efficiency, -1);
		sprintf(cbuf, "PRP_%i_PRIMARY", prp_counter);
		if (!oapiReadItem_bool(fh, cbuf, primary)) { primary = false; }
		if (primary) {
			MakePrimary(prp_counter);
		}
		sprintf(cbuf, "PRP_%i_RETAINFL", prp_counter);
		bool retain;
		if (!oapiReadItem_bool(fh, cbuf, retain)) { retain = false; }
		SetPrpRetainFuel(prp_counter, retain);

		prp_counter++;
		sprintf(cbuf, "PRP_%i_ID", prp_counter);
	}
	LogV("Parsing Tank Section Completed, found %i definitions",prp_counter);
	return;
}
void PropellantManager::WriteCfg(FILEHANDLE fh) {
	oapiWriteLine(fh, ";<-------------------------PROPELLANT DEFINITIONS------------------------->");
	oapiWriteLine(fh, " ");
	for (DWORD i = 0; i < GetPropCount(); i++) {
		char cbuf[256] = { '\0' };
		sprintf(cbuf, "PRP_%i_ID", i);
		oapiWriteItem_int(fh, cbuf, i);
		sprintf(cbuf, "PRP_%i_MAXMASS", i);
		oapiWriteItem_float(fh, cbuf, GetTankMaxMass(i));
		sprintf(cbuf, "PRP_%i_EFFICIENCY", i);
		oapiWriteItem_float(fh, cbuf, GetTankEfficiency(i));
		sprintf(cbuf, "PRP_%i_PRIMARY", i);
		oapiWriteItem_bool(fh, cbuf, IsPrimary(i));
		if (GetPrpRetainFuel(i)) {
			sprintf(cbuf, "PRP_%i_RETAINFL", i);
			oapiWriteItem_bool(fh, cbuf, GetPrpRetainFuel(i));
		}
		sprintf(cbuf, "PRP_%i_NAME", i);
		char name[256] = { '\0' };
		sprintf(name,"%s", GetTankName(i).c_str());
		oapiWriteItem_string(fh, cbuf, name);
		oapiWriteLine(fh, " ");
	}
	return;
}
bool PropellantManager::GetPrpRetainFuel(def_idx d_idx) {
	return tanks[d_idx].retain_fuel;
}
void PropellantManager::SetPrpRetainFuel(def_idx d_idx, bool set) {
	tanks[d_idx].retain_fuel = set;
	return;
}
void PropellantManager::Clear() {
	LogV("Clearing Tank Section");
	VB1->ClearPropellantResources();
	tanks.clear();
	LogV("Clearing Tank Section Completed");
	return;
}