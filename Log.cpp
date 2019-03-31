#include "VesselBuilder1.h"
#include "Log.h"

Logger::Logger(VESSEL *_v) {
	v = _v;
	/*char nbuf[256] = { '\0' };
	sprintf(nbuf, "%s", v->GetName());
	string nn(nbuf);
	filename = "\\Vessels\\VesselBuilder1\\Logs\\" + nn + ".log";*/
	
	filename = "VesselBuilder1.log";
}
Logger::~Logger(){
	v = NULL;
}
void Logger::Log(std::string str) {
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_APP, ROOT);
	char line[CMAXLEN] = { '\0' };
	sprintf(line, str.c_str());
	char buf2[CMAXLEN] = { '\0' };
	sprintf_s(buf2, "%.3f - %s: %s", oapiGetSimTime(), v->GetName(), line);
	oapiWriteLine(fh, buf2);
	oapiCloseFile(fh, FILE_APP);
	return;
}
void Logger::Log(const char* str, ...) {
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_APP, ROOT);
	char buf[CMAXLEN] = { '\0' };
	va_list va;
	va_start(va, str);
	vsprintf(buf, str, va);
	char buf2[CMAXLEN] = { '\0' };
	sprintf(buf2, "%.3f - %s: %s", oapiGetSimTime(), v->GetName(),buf);
	oapiWriteLine(fh, buf2);
	oapiCloseFile(fh, FILE_APP);
	//va_end(va);
	return;
}
void Logger::InitLog() {
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_OUT, ROOT);
	char cbuf[256] = { '\0' };
	sprintf(cbuf, "LOGFILE");
	oapiWriteLine(fh, cbuf);
	oapiWriteLine(fh, "******************************************************************************");
	char line[256] = { '\0' };
	sprintf_s(line, _ARRAYSIZE(line), "VesselBuilder Version: %i", VBVERSION);
	oapiWriteLine(fh, line);
	oapiCloseFile(fh, FILE_OUT);
}
void Logger::CloseLog(){
	FILEHANDLE fh = oapiOpenFile(filename.c_str(), FILE_APP, ROOT);
	oapiWriteLine(fh, "Closing....");
	oapiWriteLine(fh, "******************************************************************************");
	oapiCloseFile(fh, FILE_APP);
}


void InitVBExceptionLog() {
	FILEHANDLE fh = oapiOpenFile("VB1_Exceptions.log", FILE_OUT, ROOT);
	oapiWriteLine(fh,"VBVector Exceptions Log File");
	oapiCloseFile(fh, FILE_OUT);
	return;
}
void VBExceptionLog(const char* str, ...) {
	FILEHANDLE fh = oapiOpenFile("VB1_Exceptions.log", FILE_APP, ROOT);
	char buf[CMAXLEN] = { '\0' };
	va_list va;
	va_start(va, str);
	vsprintf(buf, str, va);
	char buf2[CMAXLEN] = { '\0' };
	sprintf(buf2, "%.3f - %s", oapiGetSimTime(), buf);
	oapiWriteLine(fh, buf2);
	oapiCloseFile(fh, FILE_APP);
	
	return;
}
