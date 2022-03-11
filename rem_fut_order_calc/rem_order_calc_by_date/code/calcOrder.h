#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <string>
#include <unistd.h>


#include "db.hpp"

using std::string;

struct OrderInfo
{
	int orderCount;
	int orderQty;
	int execQty;
	double execPueim;

	OrderInfo()
	{
		memset(this, 0, sizeof(OrderInfo));
	}
};

class CCalcTrade
{
public:
	CCalcTrade(CDB* db);
	virtual ~CCalcTrade();

	void Run();

	void converTimeStamp(unsigned long long inttimeStamp, tm& tmResult, unsigned int& nanoSsec);
	void loadResult();
	unsigned long long int stol(string& str);

private:
	CDB* m_db;

	map<string, OrderInfo>						m_order; //订单及成交数量

	map<string, int>							m_orderTimeCount;  // 报单时间统计
	map<string, vector<pair<string, int> > >	m_orderPeakResult; // 订单时间最终排名
};

