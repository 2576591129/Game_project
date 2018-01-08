#pragma once
#include "Sys.h"


//用来保存障碍物的信息
struct BuildInfo 
{
	int x_pos, y_pos; // x,y坐标
	int width, height; // 高度与宽度
	bool flag_pass; // 是否能从下面穿过
};

class Building
{
public:
	vector <BuildInfo> vct_building;
	HBITMAP hbitmap;
	int floor;
public:
	Building()
	{
		floor = 0;
	}
	~Building(){DeleteObject(hbitmap);}

public:
	//添加一个新的障碍物到数组中
	void AddBuilding(int x, int y , int width, int height ,bool flag_pass) 
	{
		BuildInfo new_build = {x,y,width,height,flag_pass};
		vct_building.push_back(new_build);
		hbitmap = (HBITMAP)LoadImage(NULL,"res\\angrybird.bmp",IMAGE_BITMAP,2*width,height,LR_LOADFROMFILE);//加载位图
	}

	void show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		SelectObject(hTempDC,hbitmap);
		BitBlt(hMemDC,vct_building.front().x_pos,vct_building.front().y_pos,vct_building.front().width,vct_building.front().height,hTempDC,vct_building.front().width,0,SRCAND);
		BitBlt(hMemDC,vct_building.front().x_pos,vct_building.front().y_pos,vct_building.front().width,vct_building.front().height,hTempDC,0,0,SRCPAINT);
		DeleteDC(hTempDC);
	}

	//障碍物超出视野范围之外则删除
	void DeleteBuild(int player_x_pos)
	{

	}

	void ChangeFloorHight()
	{

	}
};