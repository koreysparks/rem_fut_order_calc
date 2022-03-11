#ifdef _WIN32

#include <Windows.h>

typedef DWORD pthread_t;

#else

#include <pthread.h>

typedef int HANDLE;

#endif

class CThread
{
public:
	CThread();

#ifdef _WIN32
	CThread(_In_ LPTHREAD_START_ROUTINE fun, void* lParam);
	void init(_In_ LPTHREAD_START_ROUTINE fun, void* lParam);

#else
	CThread(void* (*fun)(void*), void* lParam);
	void init(void* (*fun)(void*), void* lParam);
#endif

	~CThread();

public:
	void init(void* lParam);

	void Cancl();

private:
		HANDLE m_hThread;
		pthread_t m_threadID;

#ifdef _WIN32
		LPTHREAD_START_ROUTINE m_threadFun;
#else
		void* (*m_threadFun)(void*);
#endif		
};