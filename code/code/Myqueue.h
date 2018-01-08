#include <iostream>
#include <Windows.h>
template <typename T>
class Myqueue
{
public:
	Myqueue(int Len)
	{
		arr_push = 0;
		arr_pop = 0;
		arr_size = 0;
		mysemaphore_push = NULL;
		InitializeCriticalSection(&cs_push);
		InitializeCriticalSection(&cs_pop);
		mysemaphore_push =  CreateSemaphore(0,0,1,0);
		my_event = CreateEvent(NULL,FALSE,TRUE,0);
		this->len = Len;
		arr = new T[len];
		for (int i = 0; i < len;i ++)arr[i] = NULL;
	}
	~Myqueue()
	{
		CloseHandle(mysemaphore_push);
		mysemaphore_push =NULL;
		CloseHandle(mysemaphore_push);
		mysemaphore_push =NULL;
		DeleteCriticalSection(&cs_push);
		DeleteCriticalSection(&cs_pop);
		for (int i = 0 ; i< this->len; i++)
		{
			if (arr[i] != NULL)
				delete arr[i];
		}
	}
	void push(T task )
	{
		if(IsFull())
			WaitForSingleObject(my_event,INFINITE);
			//WaitForSingleObject(mysemaphore_push,INFINITE);
		EnterCriticalSection(&cs_push);
		if(IsFull())
			WaitForSingleObject(my_event,INFINITE);
			//WaitForSingleObject(mysemaphore_push,INFINITE);
		arr[arr_push] =task;
		arr_push =  (arr_push+1)%len;
		arr_size +=1;
		LeaveCriticalSection(&cs_push);
	}
	T pop()
	{
		if(IsEmpty())return NULL;
		EnterCriticalSection(&cs_pop);
		if(IsEmpty())return NULL;
		T task = arr[arr_pop];
		arr_pop = (arr_pop+1)%len;
		arr_size-=1;
		if (arr_size + 1 == len)
			SetEvent(my_event);
			//ReleaseSemaphore(mysemaphore_push,1,0);
		LeaveCriticalSection(&cs_pop);
		return task;
	}
	int size(){return arr_size;}
	bool IsEmpty()
	{ 
		if(arr_size == 0)return true;
		return false;
	}
	bool IsFull()
	{
		if (arr_size == len) return true;
		return false;
	}
private:
	int len;				//队列的总长度
	int arr_push;
	int arr_pop;
	int arr_size;		//队列的当前长度
	HANDLE mysemaphore_push;
	CRITICAL_SECTION cs_push;
	CRITICAL_SECTION cs_pop;
	T* arr;
	HANDLE my_event;												//用事件实现
};