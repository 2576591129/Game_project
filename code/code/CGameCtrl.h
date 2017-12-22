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
typedef CGameCtrl* (*PFUN_CREATE_OBJECT)();   //  ���庯��ָ������


class CGameCtrl
{
public:
	static PFUN_CREATE_OBJECT pfnCreateObject;   //  ��������ĺ���ָ�����
protected:
	HWND m_hMainWnd;   //  װһ��������
	HINSTANCE m_hIns;  //  װһ��ʵ�����
public:
	CGameCtrl()
	{
		m_hMainWnd = 0;
		m_hIns = 0;
	}
	virtual ~CGameCtrl()  //  ������
	{
	
	}
public:
	void SetHandle(HWND hWnd,HINSTANCE hIns)  // �ӿ����þ��
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
	Init(PFUN_CREATE_OBJECT pfn)   //  ͨ�����캯��������ָ�븳ֵ
	{
		CGameCtrl::pfnCreateObject = pfn;
	}
};


#endif//_GAMECTRL_H
