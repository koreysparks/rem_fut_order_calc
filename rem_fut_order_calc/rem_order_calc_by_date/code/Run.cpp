#include "db.hpp"
#include "calcOrder.h"
#include "thread.hpp"

#include "../../common_libs_pub/include/linux/shengli_xml/xml_inc.h"

using namespace SHENGLI::XML;
//using namespace SHENGLI::TOOLS;
using namespace pp;


struct dbParam
{
	string          m_host;
	int             m_port;
	string          m_user;
	string          m_pwd;
	string          m_dbName;
};

CLog				g_log("g_log", false, true);
vector<dbParam>		g_dbParamV;

bool loadParam()
{
	string paramName = "rem_trade_statistical.xml";
	xml_document doc;
	doc.load_file(paramName.c_str());

	xml_element grp = doc.get_root().first_child_element("param");
	if(!grp.is_good())
	{
		g_log.log("load xml node param failed!");
		return false;
	}

	xml_element item = grp.first_child_element();
	if(!item.is_good())
	{
		g_log.log("first xml node is not good!");
		return false;
	}

	while (item.is_good())
	{
		string name = item.get_element_name();
		string value = item.get_attribute("value");

		if(string::npos != name.find("db_conf"))
		{
			dbParam p;
			p.m_host = item.get_attribute("host");
			p.m_port = atoi(item.get_attribute("port").c_str());
			p.m_dbName = item.get_attribute("db_name");
			p.m_user = item.get_attribute("user");
			p.m_pwd = item.get_attribute("pwd");
			g_dbParamV.push_back(p);
		}

		item = item.next_element();
	}

	return true;
}

void* rmCsvFun(void* lParam)
{
	system("rm -rf *.csv");
	return NULL;
}

void* calcFun(void* lParam)
{
	dbParam* dbP = (dbParam*)lParam;
	CLog log(dbP->m_host + "_" + dbP->m_dbName, false, false);
	CDB db(dbP->m_host, dbP->m_port, dbP->m_dbName, dbP->m_user, dbP->m_pwd, &log);
	CCalcTrade calc(&db);
	calc.Run();

	return NULL;
}

void Sleep(int nMilliSeconds)
{
#ifdef WIN32

	Sleep(nMilliSeconds);
#else

	usleep(1000* nMilliSeconds);

#endif

}

int main()
{
	if(!loadParam())
	{
		return -1;
	}

	CThread rmCsv;
	CThread tradeThread[g_dbParamV.size()];

	rmCsv.init(rmCsvFun, (void*)NULL);
	Sleep(1000);

	for(size_t i = 0; i < g_dbParamV.size(); ++i)
	{
		tradeThread[i].init(calcFun, (void*)(&g_dbParamV[i]));
		Sleep(3000);
	}

	string quit;
	while(1)
	{
		cin >> quit;
		if(quit == "quit")
		{
			break;
		}

		Sleep(1000);
	}

	
	return 0;
}









