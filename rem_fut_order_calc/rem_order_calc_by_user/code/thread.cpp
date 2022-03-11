#include "thread.hpp"

#include <iostream>
using namespace std;


CThread::CThread()
{}

CThread::~CThread()
{}


#ifdef _WIN32
CThread::CThread(LPTHREAD_START_ROUTINE fun, void* lParam)
{
	init(fun, lParam);
}

void CThread::init(_In_ LPTHREAD_START_ROUTINE fun, void* lParam)
{
	m_threadFun = fun;
	init(lParam);
}

void CThread::init(void* lParam)
{
	m_hThread = CreateThread(NULL, 0, m_threadFun, lParam, 0, &m_threadID);
}

void CThread::Cancl()
{
	if(NULL != m_hThread)
	{
		CloseHandle(m_hThread);
	}
}

#else
CThread::CThread(void* (*fun)(void*), void* lParam)
{
	init(fun, lParam);
}

void CThread::init(void* (*fun)(void*), void* lParam)
{
	m_threadFun = fun;
	init(lParam);
}

void CThread::init(void* lParam)
{
	int ret = pthread_create(&m_threadID, NULL, m_threadFun, lParam);
	if(ret)
	{
		cout << "create thread failed!" << endl;
	}
}
 
void CThread::Cancl()
{
	if(0 != m_hThread)
	{
		pthread_cancel(m_threadID);
	}
}

#endif