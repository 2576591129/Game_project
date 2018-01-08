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
	float x_pos,y_pos;								//��¼���������(����ڴ��ڵ�����)			
	float ab_x_pos;									//�����x����, (����ڱ���������)

	int time_first, time_second;			// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	string  state;									//����״̬,������ҵ�ͼ��, ����ȫ�ֵ�map: loadRes ����ƥ��

	PLAYER_DIRECT direct;					//-1 ������,  1 ������
	bool flag_cycle_show;						// ѭ����־	
	float player_speed ;							//����ٶ�

	float player_jump_x_speed,player_jump_y_speed;		// ���x,y������ٶ� , ��������Ծʱʹ��
	int jumping_height;							// ����Ѿ���Ծ�ĸ߶�
	bool flag_jump;								//�Ƿ�������ı�־
	float begin_jump_y_pos;

	//int floor_height;								//�װ�߶�
public:
	Player()
	{
		direct = RIGHT;
		x_pos = 500;
		y_pos = 0;
		ab_x_pos = x_pos;
		time_first = 0;
		time_second = 0;

		state = "վ��";
		
		flag_cycle_show = true;
		flag_jump = false;

		player_speed =(float) PLAYER_RUN_SPEED;
		player_jump_y_speed = -5;
		jumping_height = 100;


	}
	~Player(){}
public:

	//�ı������״̬, ���ݲ�ͬ��״̬���в�ͬ��ͼ
	void ChangeState(string key) {state = key;}

	//�Ƿ�ѭ����ʾ, ����Ծ���߶��¾Ͳ���Ҫѭ����ʾ
	void ChangeShowFlag()
	{
		if (!state.compare(0,4,"վ��") || !state.compare(0,2,"��"))flag_cycle_show = true;			
		else flag_cycle_show = false;			
	}

	//ͨ����ҵ�״̬, ����mapƥ��, ��ʾͼƬ
	void Show(HDC hMemDC)
	{
		ChangeShowFlag();
		if (state == "��")
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

	//����ҷ���ָ��, ���������(GameApp)�Ľӿ�
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
			ChangeState("��");
		}
		else if (nKey == JUMP )ChangeState("��");
		else if (nKey == SQUAT )ChangeState("��");
	}


	//����ƶ�
	void Move( WPARAM nkey)
	{
		int dis = back.obstacle->GetObstaclePosition(ab_x_pos + PLAYER_WIDTH,y_pos + PLAYER_HEIGHT/2,(float)ab_x_pos + WINDOW_WEIGHT - x_pos , y_pos + PLAYER_HEIGHT/2 , 0);
		if (dis <= 0 && dis != -999 )//���ϰ������ƶ�, ֱ��return
		{
			return;
		}

		if ( nkey == FORWARD)//�����ƶ�
		{
			if (this->x_pos<WINDOW_WEIGHT/2 )
				this->x_pos +=player_speed; //��û�е���Ļ����, ��� x ��++
			else 
				back.Move(player_speed);			//������Ļ�м���ҾͲ��ƶ� , �����ƶ�	
			this->ab_x_pos += player_speed;		// ������ô��, ��ҵ�ab_x_pos��Ҫ�ı�
		}
		else //�����ƶ�
		{		
			if(this->x_pos<0)this->x_pos = 0;  //��ֹ�������߽�
			else
			{
				this->x_pos -= player_speed ;
				this->ab_x_pos -= player_speed;
			}	
		}
	}

	//�����бȽ϶��bug, ��Ҫ�ú��޸�
	//�����Ծ , �м��и��ַ������ж�
	void Jump(HDC hMemDC)
	{
		//static int jump_begin_y_pos = y_pos; // ��̬����, ������ʱ������������� , �����ڵ�y��������õ���Ծ�߶�
		if (!flag_jump)begin_jump_y_pos = y_pos;

		player_jump_x_speed = 0;         //�����Ծʱx�����ϵ��ٶȵ���0; ֻ�е�����A���Ҽ�ʱ, �Ż����ٶ�
		flag_jump = true;							//��Ծ��־, �����Ծ�Ĺ�����, �������Ҽ�, ����ָ����Ч, �� Action�����ж�
		if (begin_jump_y_pos - y_pos >=jumping_height  && player_jump_y_speed <0)// ����һ���߶�, y�ٶȷ���			
			player_jump_y_speed = -player_jump_y_speed;				
		if(GetAsyncKeyState(FORWARD)/*&& IsObstacle() == false*/ ) // �����Ҽ� ,����û���ϰ���, ����ҷ�����ٶ�
			player_jump_x_speed =  3; 
		if(GetAsyncKeyState(RETREAT) && this->x_pos -player_jump_x_speed >0) // ��������������ﻹû����Ļ�߽�, ��������ϵ��ٶ�
			player_jump_x_speed =  -3; 

		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   �����ʱ���½���������ж� , ������Ҫ�޸�
		int tempdist = back.obstacle->GetObstaclePosition(ab_x_pos,y_pos + PLAYER_HEIGHT, ab_x_pos , 449 , 0);
		if (tempdist <= 0 && tempdist != -999&& player_jump_y_speed>0) // ���Ｔ�����������Ծ���
		{							
			SetPlayerYPos();
			ChangeState("վ��");
			player_jump_y_speed = -5;
			flag_jump = false;					//��Ծ��־�ı�
		}
		//��Ծʱ������ǰ���ķ����, ��������Ѿ�����Ļ����, ��ʱ, ��ҵ���Ծ��ǰ�����ٶ�Ϊ0, ���ñ����ƶ�, �ٶ�Ϊ��ҵ���Ծǰ�����ٶ�
		if(GetAsyncKeyState(FORWARD)
			&& this->x_pos >= WINDOW_WEIGHT/2 )
		{
			back.Move(2);
			this->ab_x_pos += 2;
			player_jump_x_speed = 0;
		}
		player_show.show(hMemDC,state,&x_pos,&y_pos,direct,20,flag_cycle_show,player_jump_x_speed,player_jump_y_speed ,&ab_x_pos);
	}

	//���״̬��λ, �ڰ���̧���ʱ�򱻵���
	void Restore(WPARAM nKey){if ( (nKey == FORWARD || nKey == RETREAT) &&  !flag_jump) ChangeState("վ��");}
};

