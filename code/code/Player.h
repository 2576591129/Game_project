#pragma once
#include "Sys.h"
#include "Back.h"

extern LoadRes  loadres;
extern CBack back;
class Player
{
public:
	int player_height,player_width;
	CycleShow player_show; 
	float x_pos,y_pos;								//记录人物的坐标(相对于窗口的坐标)			
	float ab_x_pos;									//人物的x坐标, (相对于背景的坐标)

	int time_first, time_second;			// 记录重绘的时间 , 这样就不用使用定时器
	string  state;									//人物状态,决定玩家的图像, 根据全局的map: loadRes 进行匹配

	PLAYER_DIRECT direct;					//-1 代表左,  1 代表右
	bool flag_cycle_show;						// 循环标志
	bool flag_jump;								//是否能跳起的标志
	float player_speed ;							//玩家速度

	float player_jump_x_speed,player_jump_y_speed;		// 玩家x,y方向的速度 , 在人物跳跃时使用

	int jumping_height;							// 玩家已经跳跃的高度
	int begin_jump_y_pos;						//记录玩家起跳的y坐标

public:
	Player()
	{
		direct = RIGHT;
		flag_cycle_show = true;
		x_pos = 500;
		y_pos = 0;
		ab_x_pos = x_pos;
		time_first = 0;
		time_second = 0;
		state = "站立";
		flag_jump = false;
		player_speed =(float) PLAYER_RUN_SPEED;

		player_jump_y_speed = -5;
		jumping_height = 110;
	}
	~Player(){}
public:

	//改变人物的状态, 根据不同的状态进行不同贴图
	void ChangeState(string key) 
	{
		state = key;
		player_height = loadres.map_picture[state].back().height;
		player_width = loadres.map_picture[state].back().width;
	}

	//是否循环显示, 如跳跃或者蹲下就不需要循环显示
	void ChangeShowFlag()
	{
		if (!state.compare(0,4,"站立") || !state.compare(0,2,"跑"))flag_cycle_show = true;			
		else flag_cycle_show = false;			
	}

	//通过玩家的状态, 进行map匹配, 显示图片
	void Show(HDC hMemDC)

	{	
		ChangeShowFlag();
		if (state == "跳")	Jump(hMemDC);	
		else if (state == "掉下")Down(hMemDC);
		else 
		{
			SetPlayerYPos();
			player_show.show(hMemDC,state,&x_pos,&y_pos,direct,130,flag_cycle_show);					
		}
	}

	void SetPlayerYPos()
	{
		//如果玩家脚与下方地板距离过大,玩家状态改为掉下
		if (back.obstacle->GetObstaclePosition(ab_x_pos + player_width/2,y_pos+ player_height,ab_x_pos+ player_width/2 ,WINDOW_HIGNT ,0) > 10 &&! flag_jump)   
			ChangeState("掉下");	
		else
			y_pos = back.obstacle->GetObstaclePosition(ab_x_pos + player_width/2,y_pos + player_height/2 , ab_x_pos + player_width/2, 440 , 2) - player_height;
	}

	//给玩家分配指令, 这是与外界(GameApp)的接口
	void Action(WPARAM nKey , HWND hwnd = NULL)
	{
		if (flag_jump)
		{
			if ( nKey ==FORWARD || nKey ==RETREAT)
				this->direct = ( nKey ==FORWARD)?RIGHT:LEFT;
			return;
		}
		if (nKey == FORWARD || nKey ==RETREAT)
		{
			this->direct = ( nKey ==FORWARD)?RIGHT:LEFT;
			this->Move(nKey);	
			ChangeState("跑");
		}
		else if (nKey == JUMP )ChangeState("跳");
		else if (nKey == SQUAT )ChangeState("蹲");
		if (nKey == VK_SPACE)           //  玩家发射子弹
		{
			PlaySound("开枪");
		}

	}


	//玩家移动
	void Move( WPARAM nkey)
	{
		int dis = back.obstacle->GetObstaclePosition(ab_x_pos + (direct +1)/2 *player_width,y_pos + player_height/2,(float)ab_x_pos - x_pos +(direct +1)/2 *WINDOW_WEIGHT, y_pos + player_height/2 , 0);	
		if (dis <= 0 && dis != -999 )return;
		if ( nkey == FORWARD)//向右移动
		{
			if (this->x_pos<WINDOW_WEIGHT/2 )
				this->x_pos +=player_speed; //还没有到屏幕中央, 玩家 x 就++
			else 
				back.Move(player_speed);			//到了屏幕中间玩家就不移动 , 背景移动	
			this->ab_x_pos += player_speed;		// 不管怎么样, 玩家的ab_x_pos都要改变
		}
		else //向左移动
		{	
			if(this->x_pos - player_speed<0)return;  //防止人物出左边届
			else
			{
				this->x_pos -= player_speed ;
				this->ab_x_pos -= player_speed;
				this->SetPlayerYPos();
			}	
		}
	}
	//玩家掉下
	void Down(HDC hMemDC)
	{
		int player_down_y_speed = 5 ;
		int floor_distance = back.obstacle->GetObstaclePosition(ab_x_pos + (direct + 1)/2 * player_width/2,y_pos+player_height,ab_x_pos+ (direct + 1)/2 * player_width/2,WINDOW_HIGNT,0);
		if (floor_distance<=0 && floor_distance != -999) // 人物即将到达地面
		{							
			SetPlayerYPos();
			ChangeState("站立");
		}
		player_show.show(hMemDC,state,&x_pos,&y_pos,direct,20,flag_cycle_show,0,player_down_y_speed ,&ab_x_pos);
	}
	//玩家跳跃 , 中间有各种繁琐的判断
	void Jump(HDC hMemDC)
	{
		if (!flag_jump) //记录开始跳跃时, 玩家的位置
			begin_jump_y_pos = y_pos; 
		player_jump_x_speed = 0;         //玩家跳跃时x方向上的速度等于0; 只有当按下A左右键时, 才会有速度
		flag_jump = true;							//跳跃标志, 玩家跳跃的过程中, 除了左右键, 其他指令无效, 在 Action中有判断 , 当为true时, 认为人物正在跳跃的过程中
		if (begin_jump_y_pos - y_pos >=jumping_height  && player_jump_y_speed <0 )// 到达一定高度, y速度反向			
			player_jump_y_speed = -player_jump_y_speed;		
		if(GetAsyncKeyState(FORWARD) ) // 按下右键 ,并且没有障碍物, 获得右方向的速度
			player_jump_x_speed =  3; 
		if(GetAsyncKeyState(RETREAT) && this->x_pos -player_jump_x_speed >0) // 按下左键并且人物还没有屏幕边界, 获得左方向上的速度
			player_jump_x_speed =  -3; 	
		//这里是判断下降到地面上 
		int floor_distance = back.obstacle->GetObstaclePosition(ab_x_pos + player_width /2,y_pos+88,ab_x_pos +player_width /2,WINDOW_HIGNT,0);
		if (floor_distance<=0 && floor_distance != -999 && player_jump_y_speed>0) // 人物即将到达地面跳跃完毕
		{			
			ChangeState("站立");			//恢复状态为站立 , 并设置玩家的y坐标
			SetPlayerYPos();
			player_jump_y_speed = -5;		
			flag_jump = false;					//跳跃标志复位
		}
		//跳跃时按下了前进的方向键, 并且玩家已经到屏幕中央, 这时, 玩家的跳跃的前进的速度为0, 调用背景移动, 速度为玩家的跳跃前进的速度
		if(GetAsyncKeyState(FORWARD) && this->x_pos >= WINDOW_WEIGHT/2 )
		{
			back.Move(2);
			this->ab_x_pos += 2;			//玩家的绝对坐标改变
			player_jump_x_speed = 0;
		}
		//执行跳跃的显示函数, 自带移动
		player_show.show(hMemDC,state,&x_pos,&y_pos,direct,20,flag_cycle_show,player_jump_x_speed,player_jump_y_speed ,&ab_x_pos);
	}

	//玩家状态复位, 在按键抬起的时候被调用
	void Restore(WPARAM nKey){if ( (nKey == FORWARD || nKey == RETREAT) &&  !flag_jump) ChangeState("站立");}
};

