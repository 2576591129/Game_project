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
	float x_pos;				//��¼���������
	float y_pos;				
	int time_first;			// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	int time_second;		
	string  state;			//����״̬
	PLAYER_DIRECT direct;
	bool flag_cycle_show;  // ѭ����־
	bool flag_jump;			//�Ƿ�������ı�־
public:
	Player()
	{
		direct = RIGHT;
		flag_cycle_show = true;
		x_pos = 20;
		y_pos =265;
		time_first = 0;
		time_second = 0;
		state = "վ��";
		flag_jump = true;
	}
	~Player(){}
public:
	//�ı������״̬
	void ChangeState(  string key_value)
	{
		state = key_value;
	}

	//�Ƿ�ѭ����ʾ, ����Ծ���߶��¾Ͳ���Ҫѭ����ʾ
	void ChangeShowFlag()
	{
		if (!state.compare(0,4,"վ��") || !state.compare(0,2,"��"))flag_cycle_show = true;			
		else flag_cycle_show = false;			
	}

	//ͨ����ҵ�״̬, ����mapƥ��, ��ʾͼƬ
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
		case DOWN: this->ChangeState("��");	break;
		case LEFT: 		this->ChangeState("��");   		
			if(this->x_pos<0)this->x_pos = 0;  //��ֹ�������߽�
			this->x_pos -= PLAYER_SPEED ;
			break;
		case RIGHT: this->ChangeState("��");  // ���״̬�����		
			if ( !Game_building.IsObstacle(RIGHT,this->x_pos+63,this->y_pos + 75)) // �ж��Ƿ����ϰ��� , û���ϰ��������ǰ�ƶ�
			{
				if (this->x_pos<WINDOW_WEIGHT/2 )this->x_pos +=PLAYER_SPEED; //��û�е���Ļ����, ��� x ��++
				else back.Move();			//������Ļ�м���ҾͲ��ƶ� , �����ƶ�
			}		
			break;
		case JUMP:Jump(hwnd);	
		}
		ChangeShowFlag();
		Game_building.DeleteBuild(this->x_pos);
	}


	//��ʱ���ص����� , �����Ǿ�̬��
	static VOID CALLBACK TimerProc(HWND hwnd,  UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		player.flag_jump = false;
		if (idEvent == 0)  //����Ķ�ʱ��
		{
			if (GetAsyncKeyState(0x44)) //������D��, 
			{
				player.ChangeState("����");
				player.x_pos += 3;
				if (player.x_pos >= WINDOW_WEIGHT/2)
				{
					player.x_pos = WINDOW_WEIGHT/2;
					back.Move();
				}
			}
			else 
				player.ChangeState("��");
			player.ChangeShowFlag();
			player.y_pos -= 5;
			if (player.y_pos <= 180) //�ﵽһ���߶�
			{
				KillTimer(hwnd,0);	//�ر�����Ķ�ʱ��		
				SetTimer(hwnd,1,16,TimerProc); //�������µĶ�ʱ��

			}
		}

		if (idEvent == 1)//���µĶ�ʱ��
		{
			if (GetAsyncKeyState(0x44)) //������D��, 
			{
				player.ChangeState("����");
				player.x_pos += 3;
				if (player.x_pos >= WINDOW_WEIGHT/2)
				{
					player.x_pos = WINDOW_WEIGHT/2;
					back.Move();
				}
			}
			else
				player.ChangeState("��");
			player.y_pos += 5;
			if (player.y_pos >= 265)
			{
				KillTimer(hwnd,1);
				player.ChangeState("վ��");
				player.ChangeShowFlag();
				player.flag_jump = true;
			}
		}

	}

	//�����Ծ��ʵ�ֺ���
	void Jump(HWND hwnd )
	{
		if (flag_jump)SetTimer(hwnd,0,16,TimerProc); //���ö�ʱ��

	}

};

