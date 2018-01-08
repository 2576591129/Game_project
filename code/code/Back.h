#pragma once
#include "Sys.h"
#include <atlimage.h>	
#include "CycleShow.h"
#include "Building.h"

extern Building Game_building;
class CBack
{
public:
	HBITMAP m_hBmpBackDown;   //  ����ͼƬ
	HBITMAP m_hBmpBackUp;

	CycleShow player_jump;
	int x_pos;
	int y_pos;
	bool state;				//��־λ, �����Ƿ��ƶ�


	BackObstacle *obstacle;
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
		delete obstacle;
	}

public:
	void BackInit(HDC hMemDC)
	{
		//  ����λͼ
		m_hBmpBackUp =(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//����λͼ
		obstacle = new BackObstacle;
		obstacle->Init("res\\back3.bmp");
		//m_hBmpBackDown=(HBITMAP)LoadImage(NULL,"res\\back3.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//����λͼ	


		//image.Attach(m_hBmpBackDown);

		//temp_DC = hMemDC;
	}

	//�����ƶ�����ҵ���, ���ұ����ƶ���ʱ��, ȫ�����ϰ���Ҳ��Ҫ�ƶ�
	void Move(int move_size)
	{
		x_pos-=move_size;	
	}

	void BackShow(HDC hMemDC)
	{
		HDC  tempDC = ::CreateCompatibleDC(hMemDC);

		//::SelectObject(tempDC,temp_bitmap);
		//HDC tempDC2 = ::CreateCompatibleDC(tempDC);
		//::SelectObject(tempDC2,image);
		//BitBlt(tempDC,x_pos,y_pos,3696,WINDOW_HIGNT,tempDC2,0,0,SRCCOPY);
		//BitBlt(hMemDC,x_pos,y_pos,3696,WINDOW_HIGNT,tempDC,0,0,SRCCOPY);
		//temp_DC = tempDC;
		//HDC tdc =  ::CreateCompatibleDC(hMemDC);
		//::SelectObject(tdc,m_hBmpBackUp);
		//BitBlt(hMemDC,x_pos,y_pos,3696,WINDOW_HIGNT,tdc,0,0,SRCCOPY);
		//DeleteDC(tempDC2);	
		//DeleteDC(tdc);
		//DeleteDC(tempDC);

		SelectObject(tempDC,m_hBmpBackUp);
		BitBlt(hMemDC,x_pos,y_pos,3696,WINDOW_HIGNT,tempDC,0,0,SRCCOPY);
		DeleteDC(tempDC);

	}


};

