#pragma once
#include "Sys.h"
#include <atlimage.h>	
#include "CycleShow.h"
#include "Building.h"

extern Building Game_building;
class CBack
{
public:
	HBITMAP m_hBmpBackDown;   //  背景图片
	HBITMAP m_hBmpBackUp;

	CycleShow player_jump;
	int x_pos;
	int y_pos;
	bool state;				//标志位, 背景是否移动


	BackObstacle *obstacle;
public:
	CBack(void) 
	{
		m_hBmpBackDown = 0;   //  背景图片
		m_hBmpBackUp = 0;
		x_pos = 0;
		y_pos = 0;
		state = true; //背景不移动

		
	}
	~CBack(void)
	{
		::DeleteObject(m_hBmpBackDown);   // 删除图片
		::DeleteObject(m_hBmpBackUp);
		m_hBmpBackDown = 0;   //  背景图片
		m_hBmpBackUp = 0;
		delete obstacle;
	}

public:
	void BackInit(HDC hMemDC)
	{
		//  加载位图
		m_hBmpBackUp =(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//加载位图
		obstacle = new BackObstacle;
		obstacle->Init("res\\back3.bmp");
		//m_hBmpBackDown=(HBITMAP)LoadImage(NULL,"res\\back3.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//加载位图	


		//image.Attach(m_hBmpBackDown);

		//temp_DC = hMemDC;
	}

	//背景移动由玩家调用, 并且背景移动的时候, 全部的障碍物也需要移动
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

