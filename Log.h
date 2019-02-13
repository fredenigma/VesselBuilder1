#pragma once

void ResetSBLog();
void SBLog(std::string str);
void SBLog(const char* str, ...);
void CloseSBLog();
class Logger {
public:
	Logger(VESSEL *_v,std::string filename);
	~Logger();
	void Log(std::string str);
	void Log(const char* str, ...);
	void ResetLog();
	void CloseLog();
	FILEHANDLE logfile;
	VESSEL *v;
};
