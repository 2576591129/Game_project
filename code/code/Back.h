#pragma once
#include "Sys.h"


class CBack
{
public:
	HBITMAP m_hBmpBackDown;   //  ����ͼƬ
	HBITMAP m_hBmpBackUp;
	int x;
	int y;

public:
	CBack(void)
	{
		m_hBmpBackDown = 0;   //  ����ͼƬ
		m_hBmpBackUp = 0;
		x = 0;
		y = 0;
	}
	~CBack(void)
	{
		::DeleteObject(m_hBmpBackDown);   // ɾ��ͼƬ
		::DeleteObject(m_hBmpBackUp);
		m_hBmpBackDown = 0;   //  ����ͼƬ
		m_hBmpBackUp = 0;
	}

public:
	void BackInit(HINSTANCE hIns)
	{
		//  ����λͼ
		m_hBmpBackUp =(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//����λͼ

	}
	void BackMove()
	{
		/*x--;*/
	}
	void BackShow(HDC hMemDC)
	{
		//  ����һ��������DC
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		//  ѡ��һ��λͼ
		::SelectObject(hTempDC,m_hBmpBackUp);
		//  ����λͼ
		::BitBlt(hMemDC,x,y,3696,WINDOW_HIGNT,hTempDC,0,0,SRCCOPY);
		//  ѡ��һ��λͼ
		//  ɾ��DC
		::DeleteDC(hTempDC);
	}
};

