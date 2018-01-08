#pragma once
#include "Sys.h"
#include "Back.h"

class Player;
extern Building Game_building;
extern LoadRes  loadres;
extern Player player;
extern CBack back;

class Player
{
public:
	CycleShow player_show; 
	float x_pos,y_pos;								//记录人物的坐标(相对于窗口的坐标)			
	float ab_x_pos;									//人物的x坐标, (相对于背景的坐标)

	int time_first, time_second;			// 记录重绘的时间 , 这样就不用使用定时器
	string  state;									//人物状态,决定玩家的图像, 根据全局的map: loadRes 进行匹配

	PLAYER_DIRECT direct;					//-1 代表左,  1 代表右
	bool flag_cycle_show;						// 循环标志	
	float player_speed ;							//玩家速度

	float player_jump_x_speed,player_jump_y_speed;		// 玩家x,y方向的速度 , 在人物跳跃时使用
	int jumping_height;							// 玩家已经跳跃的高度
	bool flag_jump;								//是否能跳起的标志
	float begin_jump_y_pos;

	//int floor_height;								//底板高度
public:
	Player()
	{
		direct = RIGHT;
		x_pos = 500;
		y_pos = 0;
		ab_x_pos = x_pos;
		time_first = 0;
		time_second = 0;

		state = "站立";
		
		flag_cycle_show = true;
		flag_jump = false;

		player_speed =(float) PLAYER_RUN_SPEED;
		player_jump_y_speed = -5;
		jumping_height = 100;


	}
	~Player(){}
public:

	//改变人物的状态, 根据不同的状态进行不同贴图
	void ChangeState(string key) {state = key;}

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
		if (state == "跳")
			Jump(hMemDC);		
		else 
		{
			SetPlayerYPos();
			player_show.show(hMemDC,state,&x_pos,&y_pos,direct,130,flag_cycle_show);					
		}
	}

	void SetPlayerYPos()
	{
		y_pos = back.obstacle->GetObstaclePosition(ab_x_pos,y_pos + PLAYER_HEIGHT/2 , ab_x_pos , 440 , 2) - PLAYER_HEIGHT;
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
	}


	//玩家移动
	void Move( WPARAM nkey)
	{
		int dis = back.obstacle->GetObstaclePosition(ab_x_pos + PLAYER_WIDTH,y_pos + PLAYER_HEIGHT/2,(float)ab_x_pos + WINDOW_WEIGHT - x_pos , y_pos + PLAYER_HEIGHT/2 , 0);
		if (dis <= 0 && dis != -999 )//有障碍物则不移动, 直接return
		{
			return;
		}

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
			if(this->x_pos<0)this->x_pos = 0;  //防止人物出左边届
			else
			{
				this->x_pos -= player_speed ;
				this->ab_x_pos -= player_speed;
			}	
		}
	}

	//这里有比较多的bug, 需要好好修改
	//玩家跳跃 , 中间有各种繁琐的判断
	void Jump(HDC hMemDC)
	{
		//static int jump_begin_y_pos = y_pos; // 静态变量, 用于临时标记起跳的坐标 , 与现在的y坐标相减得到跳跃高度
		if (!flag_jump)begin_jump_y_pos = y_pos;

		player_jump_x_speed = 0;         //玩家跳跃时x方向上的速度等于0; 只有当按下A左右键时, 才会有速度
		flag_jump = true;							//跳跃标志, 玩家跳跃的过程中, 除了左右键, 其他指令无效, 在 Action中有判断
		if (begin_jump_y_pos - y_pos >=jumping_height  && player_jump_y_speed <0)// 到达一定高度, y速度反向			
			player_jump_y_speed = -player_jump_y_speed;				
		if(GetAsyncKeyState(FORWARD)/*&& IsObstacle() == false*/ ) // 按下右键 ,并且没有障碍物, 获得右方向的速度
			player_jump_x_speed =  3; 
		if(GetAsyncKeyState(RETREAT) && this->x_pos -player_jump_x_speed >0) // 按下左键并且人物还没有屏幕边界, 获得左方向上的速度
			player_jump_x_speed =  -3; 

		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   人物何时才下降到地面的判断 , 这里需要修改
		int tempdist = back.obstacle->GetObstaclePosition(ab_x_pos,y_pos + PLAYER_HEIGHT, ab_x_pos , 449 , 0);
		if (tempdist <= 0 && tempdist != -999&& player_jump_y_speed>0) // 人物即将到达地面跳跃完毕
		{							
			SetPlayerYPos();
			ChangeState("站立");
			player_jump_y_speed = -5;
			flag_jump = false;					//跳跃标志改变
		}
		//跳跃时按下了前进的方向键, 并且玩家已经到屏幕中央, 这时, 玩家的跳跃的前进的速度为0, 调用背景移动, 速度为玩家的跳跃前进的速度
		if(GetAsyncKeyState(FORWARD)
			&& this->x_pos >= WINDOW_WEIGHT/2 )
		{
			back.Move(2);
			this->ab_x_pos += 2;
			player_jump_x_speed = 0;
		}
		player_show.show(hMemDC,state,&x_pos,&y_pos,direct,20,flag_cycle_show,player_jump_x_speed,player_jump_y_speed ,&ab_x_pos);
	}

	//玩家状态复位, 在按键抬起的时候被调用
	void Restore(WPARAM nKey){if ( (nKey == FORWARD || nKey == RETREAT) &&  !flag_jump) ChangeState("站立");}
};

