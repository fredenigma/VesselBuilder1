#include "StationBuilder1.h"
#include "Log.h"

Logger::Logger(VESSEL *_v, string filename) {
	v = _v;
	logfile = oapiOpenFile(filename.c_str(), FILE_APP, ROOT);
}
Logger::~Logger(){
	v = NULL;
	oapiCloseFile(logfile, FILE_APP);
}
void Logger::Log(std::string str) {
	char line[1024] = { '\0' };
	sprintf(line, str.c_str());
	char buf2[2048] = { '\0' };
	sprintf_s(buf2, "%.3f - %s: %s", oapiGetSimTime(), v->GetName(), line);
	oapiWriteLine(logfile, buf2);
}
void Logger::Log(const char* str, ...) {
	char buf[1024] = { '\0' };
	va_list va;
	va_start(va, str);
	vsprintf(buf, str, va);
	char buf2[2048] = { '\0' };
	sprintf_s(buf2, "%.3f - %s: %s", oapiGetSimTime(), v->GetName(),buf);
	oapiWriteLine(logfile, buf2);

}
void Logger::ResetLog() {
	oapiWriteLine(logfile, "LOGFILE");
	oapiWriteLine(logfile, "******************************************************************************");
	char line[256] = { '\0' };
	sprintf_s(line, _ARRAYSIZE(line), "StationBuilder Version: %i", SBVERSION);
	oapiWriteLine(logfile, line);
}
void Logger::CloseLog(){
	oapiWriteLine(logfile, "Closing....");
	oapiWriteLine(logfile, "******************************************************************************");
}


void ResetSBLog() {
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
	sprintf_s(buf2, "%.3f - %s", oapiGetSimTime(), line);
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
	sprintf_s(buf2, "%.3f - %s", oapiGetSimTime(), buf);
	oapiWriteLine(logfile, buf2);
	oapiCloseFile(logfile, FILE_APP);
	return;
}
void CloseSBLog() {
	FILEHANDLE logfile = oapiOpenFile("StationBuilder1.log", FILE_APP, ROOT);
	oapiWriteLine(logfile, "Closing....");
	oapiWriteLine(logfile, "******************************************************************************");
	oapiCloseFile(logfile, FILE_OUT);
}