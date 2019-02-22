#include "StationBuilder1.h"
#include "DialogControl.h"
#include "PropellantManager.h"

PropellantManager::PropellantManager(StationBuilder1 *_SB1) {
	SB1 = _SB1;
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
	PRP_DEF tank = PRP_DEF();
	tank.name = name;
	tank.ph = SB1->CreatePropellantResource(MaxMass, currentMass, efficiency);
	tanks.push_back(tank);
	return;
}
void PropellantManager::DelTankDef(def_idx d_idx) {
	SB1->DelPropellantResource(tanks[d_idx].ph);
	tanks.erase(tanks.begin() + d_idx);
	return;
}
string PropellantManager::GetTankName(def_idx d_idx) {
	return tanks[d_idx].name;
}
double PropellantManager::GetTankMaxMass(def_idx d_idx) {
	return SB1->GetPropellantMaxMass(tanks[d_idx].ph);
}
double PropellantManager::GetTankCurrentMass(def_idx d_idx) {
	return SB1->GetPropellantMass(tanks[d_idx].ph);
}
double PropellantManager::GetTankEfficiency(def_idx d_idx) {
	return SB1->GetPropellantEfficiency(tanks[d_idx].ph);
}
void PropellantManager::SetTankName(def_idx d_idx, string newname) {
	tanks[d_idx].name = newname;
	return;
}
void PropellantManager::SetTankMaxMass(def_idx d_idx, double newmaxmass) {
	SB1->SetPropellantMaxMass(tanks[d_idx].ph, newmaxmass);
	return;
}
void PropellantManager::SetTankCurrentMass(def_idx d_idx, double newcurrentmass) {
	SB1->SetPropellantMass(tanks[d_idx].ph, newcurrentmass);
	return;
}
void PropellantManager::SetTankEfficiency(def_idx d_idx, double newefficiency) {
	SB1->SetPropellantEfficiency(tanks[d_idx].ph, newefficiency);
	return;
}
DWORD PropellantManager::GetPropCount() {
	return SB1->GetPropellantCount();
}
void PropellantManager::MakePrimary(def_idx d_idx) {
	SB1->SetDefaultPropellantResource(tanks[d_idx].ph);
	return;
}
bool PropellantManager::IsPrimary(def_idx d_idx) {
	if (SB1->GetDefaultPropellantResource() == tanks[d_idx].ph) {
		return true;
	}
	else {
		return false;
	}
	
}
PROPELLANT_HANDLE PropellantManager::GetTankPH(def_idx d_idx) {
	return tanks[d_idx].ph;
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
		oapiReadItem_bool(fh, cbuf, primary);
		if (primary) {
			MakePrimary(prp_counter);
		}

		prp_counter++;
		sprintf(cbuf, "PRP_%i_ID", prp_counter);
	}

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
		sprintf(cbuf, "PRP_%i_NAME", i);
		char name[256] = { '\0' };
		sprintf(name,"%s", GetTankName(i).c_str());
		oapiWriteItem_string(fh, cbuf, name);
		oapiWriteLine(fh, " ");
	}
	return;
}