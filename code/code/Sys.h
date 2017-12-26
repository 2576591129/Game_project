#pragma once

#include <windows.h>
#include <process.h>
#include "resource.h"
#pragma comment (lib,"Msimg32.lib")

using namespace std;

#define WINDOW_WEIGHT 1000
#define WINDOW_HIGNT		450
#define PLAYER_SPEED 6
#define BACK_MOVE_TIME_ID      0



//用于临时存放指针地址空间的一个缓存区, WcharToChar 与 CharToWchar 都用到了,  用它的话函数可以直接返回字符数组, 不会导致内存泄漏等问题
//class BUFFER
//{
//public:
//	BUFFER(){pdelete = NULL;}
//	~BUFFER(){ Destory(nullptr);}
//
//	void Destory(void * temp)
//	{
//		if (!pdelete)
//			delete(pdelete);
//		pdelete = temp;
//	}
//private:
//	void * pdelete;
//};
////定义一个全局的临时buff
//static char *  WcharToChar(WCHAR* widestr)
//{
//	BUFFER Tempbuff;
//	int num = WideCharToMultiByte(CP_OEMCP,NULL,widestr,-1,NULL,0,NULL,FALSE);
//	char *pchar = new char[num];
//	Tempbuff.Destory(pchar);
//	WideCharToMultiByte (CP_OEMCP,NULL,widestr,-1,pchar,num,NULL,FALSE);
//	return pchar;
//}
//static LPCWSTR CharToWchar(char* ch)
//{
//	BUFFER Tempbuff;
//	int num = MultiByteToWideChar(0,0,ch,-1,NULL,0);
//	wchar_t *wide = new wchar_t[num];
//	Tempbuff.Destory(wide);
//	MultiByteToWideChar(0,0,ch,-1,wide,num);
//	return wide;
//}
