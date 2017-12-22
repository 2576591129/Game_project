#include "Sys.h"

class Player
{
public:
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@这需要改进@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//加载好多好多的位图, 并且任务在进行贴图的时候不同图片还会有大小不同的问题, 解决方案请看CycleShow的头文件
	HBITMAP bit_player_left_still;
	HBITMAP bit_player_right_still;
	HBITMAP bit_player_left_run;
	HBITMAP bit_player_right_run;

	float x_pos;				//记录人物的坐标
	float y_pos;				
	int time_first;
	int time_second;		// 记录重绘的时间 , 这样就不用使用定时器
	int show_id;
	int direct;					//方向, 上下左右-> 1234
	int state;					//状态, 站, 跑, 跳, 蹲, -> 1234
public:
	Player()
	{
		x_pos = 20;
		y_pos =300;
		time_first = 0;
		time_second = 0;
		show_id = 0;
		direct = 4;
		state = 1;
	}

	~Player()
	{

	}

public:

	void Init(HINSTANCE hIns)
	{
		//先加载位图
		bit_player_left_still = (HBITMAP)LoadImage(NULL,"res\\player\\左站立.bmp",IMAGE_BITMAP,143,90,LR_LOADFROMFILE);//加载位图
		bit_player_right_still = (HBITMAP)LoadImage(NULL,"res\\player\\右站立.bmp",IMAGE_BITMAP,143,90,LR_LOADFROMFILE);//加载位图
		bit_player_left_run = (HBITMAP)LoadImage(NULL,"res\\player\\左跑.bmp",IMAGE_BITMAP,360,90,LR_LOADFROMFILE);//加载位图
		bit_player_right_run = (HBITMAP)LoadImage(NULL,"res\\player\\右跑.bmp",IMAGE_BITMAP,360,90,LR_LOADFROMFILE);//加载位图
	}
	void Show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);

		//根据方向选择位图
		SelectBitmap(hTempDC);
		ChangeShowId();
		

		DrowBit(hMemDC,hTempDC);

		//  选入一张位图
		//  删除DC
		::DeleteDC(hTempDC);
	}

	void SelectBitmap(HDC hTempDC)
	{
		switch (state)
		{
		case 1: 
			if (direct == 3)::SelectObject(hTempDC,bit_player_left_still);
			if (direct == 4)::SelectObject(hTempDC,bit_player_right_still);
			break;
		case 2: 
			if (direct == 3)::SelectObject(hTempDC,bit_player_left_run);
			if (direct == 4)::SelectObject(hTempDC,bit_player_right_run);
			break;
		case 3: 
			break;
		case 4: 
			break;
		}

	}

	void ChangeShowId()
	{
		time_first = GetTickCount();	
		if (time_first - time_second >= 100)
		{
			switch (state)
			{
			case 1: 
				if (show_id >= 1)	
					show_id = 0;
				else
					show_id ++;
				break;
			case 2: 
				if (show_id >= 4)	
					show_id = 0;
				else 
					show_id ++;
				break;
			case 3: 
				break;
			case 4: 
				break;
			}
			time_second = GetTickCount();
		}
		
	}

	void DrowBit(HDC hMemDC,HDC hTempDC)
	{
		switch (state)
		{
		case 1:
			TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,71+show_id,90,hTempDC,show_id*71,0,71+show_id,90,RGB(0,0,0));
			break;			
		case 2:
			if (show_id == 0 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,64,90,hTempDC,0,0,64,90,RGB(0,0,0));
			if (show_id == 1 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,73,90,hTempDC,64,0,73,90,RGB(0,0,0));
			if (show_id == 2 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,71,90,hTempDC,137,0,71,90,RGB(0,0,0));
			if (show_id == 3 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,76,90,hTempDC,208,0,76,90,RGB(0,0,0));
			if (show_id == 4 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,76,90,hTempDC,284,0,76,90,RGB(0,0,0));
			break;
		case 3:
			break;
		case 4:
			break;
		}
	}
};
