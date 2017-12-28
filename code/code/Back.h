#pragma once
#include "Sys.h"
#include "CycleShow.h"
#include "Building.h"

extern Building Game_building;
class CBack
{
public:
	HBITMAP m_hBmpBackDown;   //  背景图片
	HBITMAP m_hBmpBackUp;
	CycleShow back_show;
	int x_pos;
	int y_pos;
	bool state;				//标志位, 背景是否移动

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
	}

public:
	void BackInit()
	{
		//  加载位图
		m_hBmpBackUp =(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//加载位图
		m_hBmpBackDown=(HBITMAP)LoadImage(NULL,"res\\back1.bmp",IMAGE_BITMAP,3696,WINDOW_HIGNT,LR_LOADFROMFILE);//加载位图
	}

	//背景移动由玩家调用, 并且背景移动的时候, 全部的障碍物也需要移动
	void Move()
	{
		x_pos-=PLAYER_SPEED;	
		if (x_pos<=-3696)x_pos=0; // 背景循环显示
		for (int i = 0 ;i < Game_building.vct_building.size(); i ++) //遍历障碍物数组, 所有障碍物移动
		{
			Game_building.vct_building[i].x_pos -= PLAYER_SPEED ;
		}
	}

	void BackShow(HDC hMemDC)
	{
		//  创建一个兼容性DC
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

