#include "stdafx.h"

#include "dynarray.h"
class Queue {
public:
	//----------------
	// Queue
	//----------------
	Queue(UINT limit)
	{
		handles[SemaphoreIndex] = ::CreateSemaphore(NULL,  // no security attributes
			0,     // initial count
			limit, // max count
			NULL); // anonymous

		handles[StopperIndex] = ::CreateEvent(NULL,        // no security attributes
			TRUE,        // manual reset
			FALSE,       // initially non-signaled
			NULL);       // anonymous


		handles[CancelIndex] = ::CreateEvent(NULL,        // no security attributes
			TRUE,        // manual reset
			FALSE,       // initially non-signaled
			NULL);       // anonymous

		::InitializeCriticalSection(&lock);
	} // Queue

	//----------------
	// ~Queue
	//----------------
	~Queue()
	{
		::CloseHandle(handles[SemaphoreIndex]);
		::CloseHandle(handles[StopperIndex]);
		::CloseHandle(handles[CancelIndex]);
		::DeleteCriticalSection(&lock);
	} // ~Queue

	//----------------
	// AddTail
	//----------------
	BOOL AddTail(LPVOID p)
	{ 
		BOOL result;
		::EnterCriticalSection(&lock);
		queue.Add(p);
		result = ::ReleaseSemaphore(handles[SemaphoreIndex], 1, NULL);
		if(!result)
			queue.Delete(0);
		::LeaveCriticalSection(&lock);
		return result;
	}

	LPVOID RemoveHead()
	{ 
		LPVOID result;
		lastResult=mylastResult;
		switch(::WaitForMultipleObjects(3, handles, FALSE, INFINITE))
		{
		case CancelIndex:
			if(queue.GetCount()>0){
				::EnterCriticalSection(&lock);
				result = queue[0];
				queue.Delete(0);
				delete result;
				::LeaveCriticalSection(&lock);
			}else
				::ResetEvent(handles[CancelIndex]);
			return NULL;
			break;
		case StopperIndex:   // shut down thread
			ExitThread(0); //KillThread
			return NULL;     
		case SemaphoreIndex: // semaphore
			::EnterCriticalSection(&lock);
			result = queue[0];
			delQueue.Add(result);
			queue.Delete(0);
			lastResult=result;
			mylastResult=result;
			::LeaveCriticalSection(&lock);
			return result;
		}
	} // RemoveHead

	//----------------
	// shutdown
	//----------------
	void shutdown()
	{
		::SetEvent(handles[StopperIndex]);
	} // shutdown

	void clear(){
		::SetEvent(handles[CancelIndex]);
	}
	
	void enterlock(){
		::EnterCriticalSection(&lock);
	}
	void unlock(){
		::LeaveCriticalSection(&lock);
	}

	LPVOID GetLastResult()
	{
		return lastResult;

	}
protected:
	enum {StopperIndex, SemaphoreIndex, CancelIndex};
	HANDLE handles[3];
	CRITICAL_SECTION lock;
	LPVOID lastResult;
	LPVOID mylastResult;
public:
	DynArray<LPVOID> queue;
	DynArray<LPVOID> delQueue;
};
