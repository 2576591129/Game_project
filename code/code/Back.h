#pragma once
#include "Sys.h"
#include "CycleShow.h"
#include "Building.h"

extern Building Game_building;
class CBack
{
public:
	HBITMAP m_hBmpBackDown;   //  ����ͼƬ
	HBITMAP m_hBmpBackUp;
	CycleShow back_show;
	int x_pos;
	int y_pos;
	bool state;				//��־λ, �����Ƿ��ƶ�

public:
	CBack(void)
	{
		m_hBmpBackDown = 0;   //  ����ͼƬ
		m_hBmpBackUp = 0;
		x_pos = 0;
		y_pos = 0;
		state = true; //�������ƶ�
	}
	~CBack(void)
	{
		::DeleteObject(m_hBmpBackDown);   // ɾ��ͼƬ
		::DeleteObject(m_hBmpBackUp);
		m_hBmpBackDown = 0;   //  ����ͼƬ
		m_hBmpBackUp = 0;
	}

public:
	void BackInit()
	{
		//  ����λͼ
		m_hBmpBackUp =(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//����λͼ
		m_hBmpBackDown=(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//����λͼ
	}

	//�����ƶ�����ҵ���, ���ұ����ƶ���ʱ��, ȫ�����ϰ���Ҳ��Ҫ�ƶ�
	void Move()
	{
		x_pos-=PLAYER_SPEED;	
		if (x_pos<=-3696)x_pos=0; // ����ѭ����ʾ
		for (int i = 0 ;i < Game_building.vct_building.size(); i ++) //�����ϰ�������, �����ϰ����ƶ�
		{
			Game_building.vct_building[i].x_pos -= PLAYER_SPEED ;
		}
	}

	void BackShow(HDC hMemDC)
	{
		//  ����һ��������DC
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		::SelectObject(hTempDC,m_hBmpBackUp);
		::BitBlt(hMemDC,x_pos,y_pos,3696,WINDOW_HIGNT,hTempDC,0,0,SRCCOPY);
		::SelectObject(hTempDC,m_hBmpBackDown);	
		::DeleteDC(hTempDC);
	}

	void AdjustState(WPARAM nKey)
	{

	}
};

