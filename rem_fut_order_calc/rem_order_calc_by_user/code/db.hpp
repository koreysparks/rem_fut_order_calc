#pragma once
#include "log.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <mysql/mysql.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


using namespace std;
using namespace pp;

typedef vector<map<string, string> > DataType;


class CDB
{
public:
    CDB(string host, int port, string dbName, string user, string pwd, CLog* log);
    ~CDB();

public:

    
    bool query(string queryStr);
    vector<map<string, string> >& getData();
	const string getDbName();
	const string getDbHost();
	void Sleep(int nMilliSeconds);
    

private:

	bool init();
    string          m_host;
    int             m_port;
    string          m_user;
    string          m_pwd;
    string          m_dbName;
    CLog*           m_log;

    MYSQL           m_db;

    // 数据    <filed, data>
    vector<map<string, string> > m_dbData;
};
