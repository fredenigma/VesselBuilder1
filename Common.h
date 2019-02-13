#pragma once
#include <string>
#include <vector>


using namespace std;

/*void ResetSBLog();
void SBLog(std::string str);
void SBLog(const char* str, ...);*/
double StringToDouble(std::string str);
int StringToInt(std::string str);
VECTOR3 StringToVector3(std::string str);
VECTOR4 StringToVector4(std::string str);
bool StringToBool(std::string str);
std::string ToLower(std::string str);
MATRIX3 RotationMatrix(VECTOR3 angles, bool xyz);
VECTOR3 RotateVecZ(VECTOR3 input, double phase, int current, int N);

