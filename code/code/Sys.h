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



//������ʱ���ָ���ַ�ռ��һ��������, WcharToChar �� CharToWchar ���õ���,  �����Ļ���������ֱ�ӷ����ַ�����, ���ᵼ���ڴ�й©������
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
////����һ��ȫ�ֵ���ʱbuff
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
