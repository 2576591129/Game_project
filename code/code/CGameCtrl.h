#pragma once 

#ifndef _GAMECTRL_H
#define _GAMECTRL_H
#include <windows.h>

#define DECLARE() static CGameCtrl* CreateObject();


#define IMPLEMENT(ThisClass) \
	CGameCtrl* ThisClass::CreateObject()\
	{\
		return new ThisClass;\
	}\
	Init init##ThisClass(&ThisClass::CreateObject);


class CGameCtrl; 
typedef CGameCtrl* (*PFUN_CREATE_OBJECT)();   //  定义函数指针类型


class CGameCtrl
{
public:
	static PFUN_CREATE_OBJECT pfnCreateObject;   //  创建对象的函数指针变量
protected:
	HWND m_hMainWnd;   //  装一个主窗口
	HINSTANCE m_hIns;  //  装一个实例句柄
public:
	CGameCtrl()
	{
		m_hMainWnd = 0;
		m_hIns = 0;
	}
	virtual ~CGameCtrl()  //  虚析构
	{
	
	}
public:
	void SetHandle(HWND hWnd,HINSTANCE hIns)  // 接口设置句柄
	{
		this->m_hIns = hIns;
		this->m_hMainWnd = hWnd;
	}
public:
	virtual void OnCreateGame()   // WM_CREATE
	{
	
	}
	virtual void OnGameDraw()     // WM_PAINT
	{
	
	}
	virtual void OnGameRun(WPARAM nTimerID)      // WM_TIMER
	{
	
	}
	virtual void OnKeyDown(WPARAM nKey)
	{
	
	}
	virtual void OnKeyUp(WPARAM nKey)
	{
	
	}
	virtual void OnLButtonDown(POINT point)
	{
		
	}
	virtual void OnLButtonUp(POINT point)
	{
	
	}
	virtual void OnMouseMove(POINT point)
	{
	
	}

};


class Init
{
public:
	Init(PFUN_CREATE_OBJECT pfn)   //  通过构造函数给函数指针赋值
	{
		CGameCtrl::pfnCreateObject = pfn;
	}
};


#endif//_GAMECTRL_H
