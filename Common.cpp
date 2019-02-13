

#include "StationBuilder1.h"
#include "Common.h"

/*void ResetSBLog() {
	FILEHANDLE logfile = oapiOpenFile("StationBuilder1.log", FILE_OUT, ROOT);
	oapiWriteLine(logfile, "STATION BUILDER1 LOGFILE");
	oapiWriteLine(logfile, "******************************************************************************");
	char line[256] = { '\0' };
	sprintf_s(line, _ARRAYSIZE(line), "StationBuilder Version: %i", SBVERSION);
	oapiWriteLine(logfile, line);
	oapiCloseFile(logfile, FILE_OUT);
	
	return;
}
void SBLog(std::string str) {
	FILEHANDLE logfile = oapiOpenFile("StationBuilder1.log", FILE_APP, ROOT);
	char line[1024] = { '\0' };
	sprintf(line, str.c_str());
	char buf2[2048] = { '\0' };
	sprintf_s(buf2, "%.3f - %s", oapiGetSimTime(),line);
	oapiWriteLine(logfile, buf2);
	oapiCloseFile(logfile, FILE_APP);
	return;
}
void SBLog(const char* str, ...) {
	FILEHANDLE logfile = oapiOpenFile("StationBuilder1.log", FILE_APP, ROOT);
	char buf[1024] = { '\0' };
	va_list va;
	va_start(va, str);
	vsprintf(buf, str, va);
	char buf2[2048] = { '\0' };
	sprintf_s(buf2, "%.3f - %s", oapiGetSimTime(),buf);
	oapiWriteLine(logfile, buf2);
	oapiCloseFile(logfile, FILE_APP);
	return;
}*/

double StringToDouble(string str) {
	return atof(str.c_str());
}
int StringToInt(string str) {
	return atoi(str.c_str());
}
VECTOR3 StringToVector3(string str) {
	VECTOR3 v;

	std::size_t par_found = 0;
	while ((par_found = str.find_first_of('(')) != string::npos) {
		str.erase(str.begin() + par_found);
	}

	par_found = 0;
	while ((par_found = str.find_first_of(')')) != string::npos) {
		str.erase(str.begin() + par_found);
	}

	std::size_t pos = 0, found;

	std::vector<std::string> val;
	val.clear();
	while ((found = str.find_first_of(',', pos)) != std::string::npos) {
		val.push_back(str.substr(pos, found - pos));
		pos = found + 1;
	}
	val.push_back(str.substr(pos));
	if (val.size() == 3) {
		v.x = atof(val[0].c_str());
		v.y = atof(val[1].c_str());
		v.z = atof(val[2].c_str());
	}
	else {
		v = _V(0, 0, 0);
	}


	return v;
}
VECTOR4 StringToVector4(string str) {
	VECTOR4 v;

	std::size_t par_found = 0;
	while ((par_found = str.find_first_of('(')) != string::npos) {
		str.erase(str.begin() + par_found);
	}

	par_found = 0;
	while ((par_found = str.find_first_of(')')) != string::npos) {
		str.erase(str.begin() + par_found);
	}

	std::size_t pos = 0, found;

	std::vector<std::string> val;
	val.clear();
	while ((found = str.find_first_of(',', pos)) != std::string::npos) {
		val.push_back(str.substr(pos, found - pos));
		pos = found + 1;
	}
	val.push_back(str.substr(pos));
	if (val.size() == 4) {
		v.x = atof(val[0].c_str());
		v.y = atof(val[1].c_str());
		v.z = atof(val[2].c_str());
		v.w = atof(val[3].c_str());
	}
	else if (val.size() == 3)
	{
		VECTOR3 v3 = StringToVector3(str);
		v = _V(v3.x, v3.y, v3.z, 1);
	}
	else {
		v = _V(0, 0, 0, 0);
	}


	return v;
}

bool StringToBool(string str) {
	bool output = false;
	int check = atoi(str.c_str());
	if (check == 1) {
		output = true;
	}
	else if (check == 0) {
		output = false;
	}
	return output;
}

string ToLower(string str) {
	string output;
	output.clear();
	for (UINT i = 0; i < str.size(); i++) {
		output.push_back(tolower(str[i]));
	}
	return output;
}

MATRIX3 RotationMatrix(VECTOR3 angles, bool xyz) {
	MATRIX3 m;
	MATRIX3 RM_X, RM_Y, RM_Z;
	RM_X = _M(1, 0, 0, 0, cos(angles.x), -sin(angles.x), 0, sin(angles.x), cos(angles.x));
	RM_Y = _M(cos(angles.y), 0, sin(angles.y), 0, 1, 0, -sin(angles.y), 0, cos(angles.y));
	RM_Z = _M(cos(angles.z), -sin(angles.z), 0, sin(angles.z), cos(angles.z), 0, 0, 0, 1);
	if (!xyz)
	{
		m = mul(RM_Z, mul(RM_Y, RM_X));
	}
	else {
		m = mul(RM_X, mul(RM_Y, RM_Z));
	}
	return m;
}

VECTOR3 RotateVecZ(VECTOR3 input, double phase, int current, int N) {
	double arg = phase + (current) * 2 * PI / N;
	return _V(input.x*cos(arg) - input.y*sin(arg), input.x*sin(arg) + input.y*cos(arg), input.z);
}