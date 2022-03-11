#include "calcOrder.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream> 
using namespace std;

CCalcTrade::CCalcTrade(CDB* db)
	:m_db(db)
{}

CCalcTrade::~CCalcTrade()
{}


void CCalcTrade::Run()
{
	//m_db->init();
	vector<string> tables;
	m_db->query("show tables;");
	
	// 查询到的数据，只有一个表名字段
	vector<map<string, string> > tablesFileds = m_db->getData();

	for(size_t p = 0; p < tablesFileds.size(); ++p)
	{
		map<string, string> oneRow = tablesFileds[p];
		for(map<string, string>::const_iterator iter = oneRow.begin(); iter != oneRow.end(); ++iter)
		{
			tables.push_back(iter->second);
		}
	}

	// 按日期统计订单表数据
	for(size_t i = 0; i < tables.size(); ++i)
	{
		m_orderTimeCount.clear();
		if(string::npos != tables[i].find("t_fut_orders"))
		{
			// 获取历史订单表日期
			string date = tables[i].c_str() + 13;
			printf("---------- %s calc order, date:%s--------------------------\n", m_db->getDbHost().c_str(), date.c_str());

			// 获取单子总量
			if(m_db->query("select count(1) from " + tables[i] + ";"))
			{
				vector<map<string, string> > orderQty = m_db->getData();
				m_order[date].orderCount = atoi(orderQty[0].begin()->second.c_str());
			}
			
			

			// 获取总订单手数
			if(m_db->query("select sum(quantity) from " + tables[i] + ";"))
			{
				vector<map<string, string> > qtySum = m_db->getData();
				m_order[date].orderQty = atoi(qtySum[0].begin()->second.c_str());
			}
			

			// 获取成交手数
			if(m_db->query("select sum(executed_qty) from " + tables[i] + ";"))
			{
				vector<map<string, string> > execQtySum = m_db->getData();
				m_order[date].execQty = atoi(execQtySum[0].begin()->second.c_str());
			}
			

			// 按时间查询订单量
			if(m_db->query("select order_shengli_accept_time, login_id from " + tables[i] + ";"))
			{
				vector<map<string, string> > orderAcceptTime = m_db->getData();	
				unsigned long long int timeStamp;
				tm time;
				unsigned int ns;
				for(size_t j = 0; j < orderAcceptTime.size(); ++j)
				{
					// 8字节
					timeStamp = stol(orderAcceptTime[j]["order_shengli_accept_time"]);
					//memcpy(&timeStamp, (char*)&(atoi(orderAcceptTime[i].begin()->second.c_str())), sizeof(unsigned long long int));
					converTimeStamp(timeStamp, time, ns);
					char realTime[19]; 
					sprintf(realTime, "%d-%02d-%02d %02d:%02d:%02d", 
						time.tm_year + 1900, 
						time.tm_mon + 1, 
						time.tm_mday, 
						time.tm_hour,
						time.tm_min,
						time.tm_sec);

					m_orderTimeCount[realTime]++;

					string logonID = orderAcceptTime[j]["login_id"];
					m_userOrderTime[date][logonID][realTime]++;
				}
			}

			

			int orderTimeCount = m_orderTimeCount.size();
			if(0 == orderTimeCount)
			{
				printf("order time list is empty! date: %s", date.c_str());
			}

			// 取单量前10的统计
			if(orderTimeCount > 10)
			{
				orderTimeCount = 10;
			}

			for(size_t z = 0; z < orderTimeCount; z++)
			{
				map<string, int>::const_iterator iterMax = m_orderTimeCount.begin();
				string time = iterMax->first;
				int maxCount = iterMax->second;
				for(map<string, int>::const_iterator iter = m_orderTimeCount.begin(); iter != m_orderTimeCount.end(); ++iter)
				{
					if(iter->second > maxCount)
					{
						time = iter->first;
						maxCount = iter->second;
					}
				}

				m_orderPeakResult[date].push_back(make_pair(time, maxCount));
				m_orderTimeCount[time] = 0;
			}

			// 堆10个空的峰值，以防读取时崩溃
			for(size_t v = 0; v < 10; ++v)
			{
				m_orderPeakResult[date].push_back(make_pair("0000-00-00 00.00.00", 0));
			}

			if(0 == m_order[date].orderCount)
			{
				m_order[date].orderQty = 0;
				m_order[date].execQty = 0;
			}

		}
	}


	loadResult();
}

void CCalcTrade::loadResult()
{
	printf("---------- %s order date count:%d order date peak count:%d --------\n", m_db->getDbHost().c_str(), m_order.size(), m_orderPeakResult.size());
	CLog resCsv("order_info_" + m_db->getDbName() + "_" + m_db->getDbHost() );
	resCsv.log("订单日期,报单量,报单手数,成交手数");
	
	map<string, vector<pair<string, int> > >::const_iterator iterPeak = m_orderPeakResult.begin();
	map<string, map<string, map<string, int> > >::const_iterator iterUserOrserTime = m_userOrderTime.begin();
	// 每个交易日
	for(map<string, OrderInfo>::const_iterator iter = m_order.begin(); iter != m_order.end(); ++iter, ++iterPeak, ++iterUserOrserTime)
	{
		resCsv.log("%s,%d,%d,%d",
			iter->first.c_str(),
			iter->second.orderCount,
			iter->second.orderQty,
			iter->second.execQty);

		// 每个用户
		for(map<string, map<string, int> >::const_iterator iterUser = iterUserOrserTime->second.begin(); iterUser != iterUserOrserTime->second.end(); ++iterUser)
		{
			string formatTimeCount = " ,,,,用户名," + iterUser->first + ",";
			// 每个时间
			for(map<string, int>::const_iterator iterOrderTime = iterUser->second.begin(); iterOrderTime != iterUser->second.end(); ++iterOrderTime)
			{
				formatTimeCount += iterOrderTime->first;
				formatTimeCount += ",";
				formatTimeCount += toStr(iterOrderTime->second);
				formatTimeCount += ",";
			}

			resCsv.log("%s", formatTimeCount.c_str());
		}
	}

	printf("---------- %s load order info finish ----------------------------\n", m_db->getDbHost().c_str());
	resCsv.quit();

}

void CCalcTrade::converTimeStamp(unsigned long long int timeStamp, tm& tmResult, unsigned int& nanoSec)
{
	unsigned int sec;
	char* p = (char*)(&timeStamp);
	memcpy(&sec, p + 4, 4);
	memcpy(&nanoSec, p, 4);

#ifdef _WIN32
	time_t tt32 = (time_t)sec;
	localtime_s(&tmResult, &tt32);
#else
	time_t tt32 = (time_t)sec;
	localtime_r(&tt32, &tmResult);
#endif
	return;
}

unsigned long long int CCalcTrade::stol(string& str)
{
	unsigned long long int result;
	istringstream is(str.c_str());
	is >> result;
	return result;
}

string CCalcTrade::toStr(int src)
{
	char res[8];
	memset(res, 0, 8);
	sprintf(res, "%d", src);
	return string(res);
}

void CCalcTrade::strADD(string& str, string& strResult)
{
	strResult += str;
}



