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
	float x_pos;				//记录人物的坐标
	float y_pos;				
	int time_first;			// 记录重绘的时间 , 这样就不用使用定时器
	int time_second;		
	string  state;			//人物状态
	PLAYER_DIRECT direct;
	bool flag_cycle_show;  // 循环标志
	bool flag_jump;			//是否能跳起的标志
public:
	Player()
	{
		direct = RIGHT;
		flag_cycle_show = true;
		x_pos = 20;
		y_pos =265;
		time_first = 0;
		time_second = 0;
		state = "站立";
		flag_jump = true;
	}
	~Player(){}
public:
	//改变人物的状态
	void ChangeState(  string key_value)
	{
		state = key_value;
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
		player_show.show(hMemDC,state,(int)this->x_pos,(int)this->y_pos,direct,130,flag_cycle_show);
		//if (GetAsyncKeyState(0x44))Move(RIGHT);

	}

	void Move(PLAYER_DIRECT direct, HWND hwnd= NULL)
	{
		if (direct == LEFT || direct == RIGHT)this->direct = direct;
		switch (direct)
		{
		case UP: 	break;
		case DOWN: this->ChangeState("蹲");	break;
		case LEFT: 		this->ChangeState("跑");   		
			if(this->x_pos<0)this->x_pos = 0;  //防止人物出左边届
			this->x_pos -= PLAYER_SPEED ;
			break;
		case RIGHT: this->ChangeState("跑");  // 玩家状态变成跑		
			if ( !Game_building.IsObstacle(RIGHT,this->x_pos+63,this->y_pos + 75)) // 判断是否有障碍物 , 没有障碍物才能向前移动
			{
				if (this->x_pos<WINDOW_WEIGHT/2 )this->x_pos +=PLAYER_SPEED; //还没有到屏幕中央, 玩家 x 就++
				else back.Move();			//到了屏幕中间玩家就不移动 , 背景移动
			}		
			break;
		case JUMP:Jump(hwnd);	
		}
		ChangeShowFlag();
		Game_building.DeleteBuild(this->x_pos);
	}


	//定时器回调函数 , 必须是静态的
	static VOID CALLBACK TimerProc(HWND hwnd,  UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		player.flag_jump = false;
		if (idEvent == 0)  //跳起的定时器
		{
			if (GetAsyncKeyState(0x44)) //按下了D键, 
			{
				player.ChangeState("跳走");
				player.x_pos += 3;
				if (player.x_pos >= WINDOW_WEIGHT/2)
				{
					player.x_pos = WINDOW_WEIGHT/2;
					back.Move();
				}
			}
			else 
				player.ChangeState("跳");
			player.ChangeShowFlag();
			player.y_pos -= 5;
			if (player.y_pos <= 180) //达到一定高度
			{
				KillTimer(hwnd,0);	//关闭跳起的定时器		
				SetTimer(hwnd,1,16,TimerProc); //设置跳下的定时器

			}
		}

		if (idEvent == 1)//跳下的定时器
		{
			if (GetAsyncKeyState(0x44)) //按下了D键, 
			{
				player.ChangeState("跳走");
				player.x_pos += 3;
				if (player.x_pos >= WINDOW_WEIGHT/2)
				{
					player.x_pos = WINDOW_WEIGHT/2;
					back.Move();
				}
			}
			else
				player.ChangeState("跳");
			player.y_pos += 5;
			if (player.y_pos >= 265)
			{
				KillTimer(hwnd,1);
				player.ChangeState("站立");
				player.ChangeShowFlag();
				player.flag_jump = true;
			}
		}

	}

	//玩家跳跃的实现函数
	void Jump(HWND hwnd )
	{
		if (flag_jump)SetTimer(hwnd,0,16,TimerProc); //设置定时器

	}

};

