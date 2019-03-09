#include "VesselBuilder1.h"
#include "Log.h"

Logger::Logger(VESSEL *_v) {
	v = _v;
	char nbuf[256] = { '\0' };
	sprintf(nbuf, "%s", v->GetName());
	string nn(nbuf);
	filename = "\\Vessels\\VesselBuilder1\\Logs\\" + nn + ".log";
}
Logger::~Logger(){
	v = NULL;
}
void Logger::Log(std::string str) {
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_APP, CONFIG);
	char line[MAX_PATH] = { '\0' };
	sprintf(line, str.c_str());
	char buf2[MAX_PATH] = { '\0' };
	sprintf_s(buf2, "%.3f - %s: %s", oapiGetSimTime(), v->GetName(), line);
	oapiWriteLine(fh, buf2);
	oapiCloseFile(fh, FILE_APP);
	return;
}
void Logger::Log(const char* str, ...) {
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_APP, CONFIG);
	char buf[MAX_PATH] = { '\0' };
	va_list va;
	va_start(va, str);
	vsprintf(buf, str, va);
	char buf2[MAX_PATH] = { '\0' };
	sprintf(buf2, "%.3f - %s: %s", oapiGetSimTime(), v->GetName(),buf);
	oapiWriteLine(fh, buf2);
	oapiCloseFile(fh, FILE_APP);
	//va_end(va);
	return;
}
void Logger::InitLog() {
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_OUT, CONFIG);
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "LOGFILE for VESSEL: %s", v->GetName());
	oapiWriteLine(fh, cbuf);
	oapiWriteLine(fh, "******************************************************************************");
	char line[256] = { '\0' };
	sprintf_s(line, _ARRAYSIZE(line), "VesselBuilder Version: %i", VBVERSION);
	oapiWriteLine(fh, line);
	oapiCloseFile(fh, FILE_OUT);
}
void Logger::CloseLog(){
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_APP, CONFIG);
	oapiWriteLine(fh, "Closing....");
	oapiWriteLine(fh, "******************************************************************************");
	oapiCloseFile(fh, FILE_APP);
}

/*
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
}*/