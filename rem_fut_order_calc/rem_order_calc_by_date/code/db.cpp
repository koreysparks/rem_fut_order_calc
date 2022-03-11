#include "db.hpp"

CDB::CDB(string host, int port, string dbName, string user, string pwd, CLog* log)
    :m_host(host)
	,m_port(port)
    ,m_dbName(dbName)
    ,m_user(user)
    ,m_pwd(pwd)
    ,m_log(log)
{
    if (0 == mysql_library_init(0, NULL, NULL)) 
    {
        if (NULL != mysql_init(&m_db)) 
        {
            // 初始化成功,设置字符编码
            if (0 != mysql_options(&m_db, MYSQL_SET_CHARSET_NAME, "utf8"))
            {
                m_log->log( "mysql client set options failed!" );
            }
        }
        else
        {
            m_log->log("mysql client init failed!");
        }
    }
}
   


CDB::~CDB()
{
    mysql_close(&m_db);
    mysql_library_end();
}

bool CDB::init()
{
	if (NULL != mysql_real_connect(&m_db, m_host.c_str(), m_user.c_str(), m_pwd.c_str(), m_dbName.c_str(), m_port, NULL, 0)) 
    {
         m_log->log("connect mysql database, server:%s:%d database:%s user:%s pwd:%s", m_host.c_str(), m_port, m_dbName.c_str(), m_user.c_str(), m_pwd.c_str());  
		return true;
	}
	else
    {
        m_log->log("connect mysql database failed! server:%s:%d, %s user:%s pwd:%s", m_host.c_str(), m_port, m_dbName.c_str(), m_user.c_str(), m_pwd.c_str());  
		return false;
	}
}


bool CDB::query(string queryStr)
{	
	mysql_close(&m_db);
    m_log->log("mysql query:%s", queryStr.c_str()); 
	Sleep(1);
    if(init() && 0 == mysql_query(&m_db, queryStr.c_str()))
    {
        return true;
    }
    else
    {
        m_log->log("%s\n", mysql_error(&m_db));
        return false;
    }
    
}

vector<map<string, string> >& CDB::getData()
{
        MYSQL_RES *result = mysql_store_result(&m_db);
		//int dataRow = mysql_num_rows(result);
        int fieldcount = mysql_num_fields(result);
		MYSQL_FIELD *field = NULL;
        MYSQL_ROW row = mysql_fetch_row(result);
        m_dbData.clear();

        map<string, string> retData;
		while(NULL != row)
        {
            for(size_t i = 0; i < fieldcount; ++i)
            {
                field = mysql_fetch_field_direct(result, i);
				if(NULL != row[i])
				{
					retData[field->name] = row[i];
				}
				else
				{
					retData[field->name] = "";
				}
            }

            m_dbData.push_back(retData);
            retData.clear();            
            row = mysql_fetch_row(result);
        }

		mysql_free_result(result);
        return m_dbData;
}


const string CDB::getDbName()
{
	return m_dbName;
}

const string CDB::getDbHost()
{
	return m_host;
}

void CDB::Sleep(int nMilliSeconds)
{
#ifdef WIN32

	Sleep(nMilliSeconds);
#else

	usleep(1000* nMilliSeconds);

#endif

}

