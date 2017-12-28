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

public:
	Building()
	{

	}
	~Building(){}

public:
	//添加一个新的障碍物到数组中
	void AddBuilding(int x, int y , int width, int height ,bool flag_pass) 
	{
		BuildInfo new_build = {x,y,width,height,flag_pass};
		vct_building.push_back(new_build);
	}

	//判断左方向或者右方向上是否有障碍物
	bool IsObstacle(PLAYER_DIRECT direct , int x_pos, int y_pos) //这里规定传进来的坐标是需要判断的那个物体的右下角坐标, 例如玩家(假如方向为右), 这里就是玩家右脚下的坐标
	{
		if (direct == RIGHT )
		{
			// 当障碍物的坐标大于玩家坐标, 并且玩家加上速度马上就要到达障碍物, 并且玩家右下角y的坐标比障碍物的y要底, 并且该障碍物不能被越过 , 则视为有障碍物
			if (this->vct_building.front().x_pos > x_pos && x_pos + PLAYER_SPEED >= this->vct_building.front().x_pos && y_pos > this->vct_building.front().y_pos && this->vct_building.front().flag_pass == false) 
				return true;
			else
				return false;
		}
		if (direct == LEFT)
		{

		}
	}

	//障碍物超出视野范围之外则删除
	void DeleteBuild(int player_x_pos)
	{
		
	}

};