#pragma once
#include "Sys.h"


class CBack
{
public:
	HBITMAP m_hBmpBackDown;   //  背景图片
	HBITMAP m_hBmpBackUp;
	int x;
	int y;

public:
	CBack(void)
	{
		m_hBmpBackDown = 0;   //  背景图片
		m_hBmpBackUp = 0;
		x = 0;
		y = 0;
	}
	~CBack(void)
	{
		::DeleteObject(m_hBmpBackDown);   // 删除图片
		::DeleteObject(m_hBmpBackUp);
		m_hBmpBackDown = 0;   //  背景图片
		m_hBmpBackUp = 0;
	}

public:
	void BackInit(HINSTANCE hIns)
	{
		//  加载位图
		m_hBmpBackUp =(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//加载位图

	}
	void BackMove()
	{
		/*x--;*/
	}
	void BackShow(HDC hMemDC)
	{
		//  创建一个兼容性DC
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		//  选入一张位图
		::SelectObject(hTempDC,m_hBmpBackUp);
		//  拷贝位图
		::BitBlt(hMemDC,x,y,3696,WINDOW_HIGNT,hTempDC,0,0,SRCCOPY);
		//  选入一张位图
		//  删除DC
		::DeleteDC(hTempDC);
	}
};

