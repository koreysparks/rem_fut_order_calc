#include "log.hpp"

namespace pp
{
	CLog::CLog()
	{}
	
	CLog::CLog(const string& logName, bool isLogs, bool isPrint)
		:m_isPrint(isPrint)
		,m_islogs(isLogs)
		,m_logName(logName)

	{
		init();
	}

	void CLog::init()
	{
		char date[12];
		struct tm timeNow = localTime();
		sprintf(date, "%d%02d%02d", timeNow.tm_year + 1900, timeNow.tm_mon + 1, timeNow.tm_mday);

		if(m_islogs)
		{

		#ifdef _WIN32
			int createLogsFlag = _mkdir("./logs");
		#else
			int createLogsFlag = mkdir("./logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
		#endif

			if(-1 == createLogsFlag)
			{
				cout << "createLogs failed!" << endl << endl;
			}

			m_logDebug.second.open(("./logs/" + m_logName + "_" + date + "_debug.log").c_str(), ios::app|ios::out);
			m_logError.second.open(("./logs/" + m_logName + "_" + date + "_error.log").c_str(), ios::app|ios::out);
			m_logUser.second.open(("./logs/" + m_logName + "_" + date + "_user.log").c_str(), ios::app|ios::out);
			m_logInfo.second.open(("./logs/" + m_logName + "_" + date + "_info.log").c_str(), ios::app|ios::out);
			m_logWarn.second.open(("./logs/" +  m_logName + "_" + date + "_warn.log").c_str(), ios::app|ios::out);
		}
		else
		{
			m_log.second.open((m_logName + ".csv").c_str(), ios::app|ios::out);
		}

	}
	

	CLog::~CLog()
	{
		quit();
	}

	CLog::CLog(const CLog& log)
	{
		m_isPrint = log.m_isPrint;
		m_islogs = log.m_islogs;
		m_logName = log.m_logName;
	}
	
	struct tm CLog::localTime()
	{
		time_t timeNow = time(NULL);
		struct tm timeInfo;
	
	#ifdef _WIN32
		localtime_s(&timeInfo, &timeNow);
		return timeInfo;
	#else
		return *localtime(&timeNow);
	#endif
	
	}
	
#ifdef _WIN32 
#else
	struct timespec CLog::localTimeNs()
	{
		struct timespec timeNow;
		clock_gettime(CLOCK_REALTIME, &timeNow);
		return timeNow;
	}
#endif
	
	void CLog::operator<<(const string& logStream)
	{
		//log(logStream);
	}

	/*void CLog::log(const string& logTxt, LogLevel logLevel)
	{
		if(_LOG_NONE_ == logLevel)
		{
			log(logTxt);
		}
		else
		{
			writeLogs(logTxt, logLevel);
		}
	}*/

	void CLog::log(const char* pformat, ...)
	{
		va_list args;
		va_start(args, pformat);
		va_end(args);
		vsnprintf(m_logBuf, sizeof(m_logBuf), pformat, args);
		assert(m_log.second.is_open());

		Time dateTime;
		getTime(dateTime);
		m_log.second << /*dateTime.Date << " "  << dateTime.TimeS << " " << dateTime.Ns << " " <<*/ m_logBuf << endl;
	
		if(m_isPrint)
		{
			cout << /*dateTime.Date << " "  << dateTime.TimeS << " " << dateTime.Ns << " " << */ m_logBuf << endl;
		}
	}
	
	/*void CLog::log(const char* logTxt)
	{
		assert(m_log.second.is_open());

		Time dateTime;
		getTime(dateTime);
		m_log.second << dateTime.Date << " "  << dateTime.TimeS << " " << dateTime.Ns << " " << logTxt << endl;
	
		if(m_isPrint)
		{
			cout << dateTime.Date << " "  << dateTime.TimeS << " " << dateTime.Ns << " " << logTxt << endl;
		}
	}*/
	
	void CLog::writeLogs(const string& logTxt, int logLevel)
	{
		Time dateTime;
		getTime(dateTime);
		struct tm timeNow = localTime();
		fstream* log;
		if(_LOG_ERROR_ == logLevel)
		{
			log = &(m_logError.second);
		}
		else if(_LOG_USER_ == logLevel)
		{
			log = &(m_logUser.second);
		}
		else if(_LOG_INFO_ == logLevel)
		{
			log  = &(m_logInfo.second);
		}
		else if(_LOG_DEBUG_ == logLevel)
		{
			log = &(m_logDebug.second);
		}
		else if(_LOG_WARN_ == logLevel)
		{
			log = &(m_logWarn.second);
		}
		else
		{
			cout << "invaild log level!" <<endl;
		}
	
		assert(log->is_open());
		(*log) << dateTime.Date << " "  << dateTime.TimeS << " " << dateTime.Ns << " " << logTxt << endl;
	
		if(m_isPrint)
		{
			cout << dateTime.Date << " "  << dateTime.TimeS << " " << dateTime.Ns << " " << logTxt << endl;
		}
	
	}
	
	void CLog::quit()
	{
		if(m_islogs)
		{
			m_logDebug.second.close();
			m_logError.second.close();
			m_logUser.second.close();
			m_logInfo.second.close();
			m_logWarn.second.close();
		}
		else
		{
			m_log.second.close();
		}
	}

	string CLog::getTimeStamp()
	{
		Time t;
		getTime(t);
		return t.Date + " " + t.TimeS + "." + t.Ns;
	}
	
	void CLog::getTime(Time& dateTime)
	{
		char dateString[12];
		char timeString[12];
		char nsString[10];
		struct tm timeNow;

	#ifdef _WIN32
		timeNow = localTime();
		memset(nsString, 0 ,10);
	#else
		struct timespec timeClock = localTimeNs();
		sprintf(nsString, "%09d", timeClock.tv_nsec);
		time_t localTime = (time_t)timeClock.tv_sec;
		timeNow = *localtime(&localTime);
	#endif	

		sprintf(dateString, "%d-%02d-%02d", timeNow.tm_year + 1900, timeNow.tm_mon + 1, timeNow.tm_mday);
		sprintf(timeString, "%02d:%02d:%02d", timeNow.tm_hour, timeNow.tm_min, timeNow.tm_sec);
		
		dateTime.Date = dateString;
		dateTime.TimeS = timeString;
		dateTime.Ns = nsString;
	}
	
}
