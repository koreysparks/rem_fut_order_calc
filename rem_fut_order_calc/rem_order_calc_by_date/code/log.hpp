#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <map>
#include <assert.h>
#include <stdarg.h>

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>

#else 
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#endif

using namespace std;

namespace pp
{	
	enum LogLevel
	{
		_LOG_NONE_ = 0,
		_LOG_INFO_ ,
		_LOG_DEBUG_ ,
		_LOG_WARN_ ,
		_LOG_USER_ ,
		_LOG_ERROR_
	};
	
	struct Time
	{
		string Date;
		string TimeS;
		string Ns;
	};
	
	class CLog
	{
	public:
		CLog();
		CLog(const string& logName, bool isLogs = false, bool isPrint = false);
		~CLog();
	
	public:
		CLog(const CLog& log);
		void init();
	
	public:
		// 日志内容 日志级别
		//void log(const string& logTxt, LogLevel logLevel);
		void operator<<(const string& logStream);
		void log(const char* pformat, ...);
		void quit();
	
	private:
		void getTime(Time& dateTime);
		string getTimeStamp();
		//void log(const char* logTxt);
		void writeLogs(const string& logTxt, int logLevel);
		struct tm localTime();

#ifdef _WIN32
#else
		struct timespec localTimeNs();
#endif
	
	private:
		string m_logName;
		bool m_isPrint;					// 是否打印
		bool m_islogs;					// 普通日志或级别日志

		pair<LogLevel, fstream> m_log;					// 普通日志
	
		pair<LogLevel, fstream> m_logError;				// 错误
		pair<LogLevel, fstream> m_logInfo;				// 信息
		pair<LogLevel, fstream> m_logDebug;				// 调试
		pair<LogLevel, fstream> m_logWarn;				// 警告
		pair<LogLevel, fstream> m_logUser;				// 用户

		char					m_logBuf[2000];
	};
}
