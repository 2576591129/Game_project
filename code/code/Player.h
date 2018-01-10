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
	float x_pos,y_pos;								//��¼���������(����ڴ��ڵ�����)			
	float ab_x_pos;									//�����x����, (����ڱ���������)

	int time_first, time_second;			// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	string  state;									//����״̬,������ҵ�ͼ��, ����ȫ�ֵ�map: loadRes ����ƥ��

	PLAYER_DIRECT direct;					//-1 ������,  1 ������
	bool flag_cycle_show;						// ѭ����־
	bool flag_jump;								//�Ƿ�������ı�־
	float player_speed ;							//����ٶ�

	float player_jump_x_speed,player_jump_y_speed;		// ���x,y������ٶ� , ��������Ծʱʹ��

	int jumping_height;							// ����Ѿ���Ծ�ĸ߶�
	int begin_jump_y_pos;						//��¼���������y����

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
		state = "վ��";
		flag_jump = false;
		player_speed =(float) PLAYER_RUN_SPEED;

		player_jump_y_speed = -5;
		jumping_height = 110;
	}
	~Player(){}
public:

	//�ı������״̬, ���ݲ�ͬ��״̬���в�ͬ��ͼ
	void ChangeState(string key) 
	{
		state = key;
		player_height = loadres.map_picture[state].back().height;
		player_width = loadres.map_picture[state].back().width;
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
		ChangeShowFlag();
		if (state == "��")	Jump(hMemDC);	
		else if (state == "����")Down(hMemDC);
		else 
		{
			SetPlayerYPos();
			player_show.show(hMemDC,state,&x_pos,&y_pos,direct,130,flag_cycle_show);					
		}
	}

	void SetPlayerYPos()
	{
		//�����ҽ����·��ذ�������,���״̬��Ϊ����
		if (back.obstacle->GetObstaclePosition(ab_x_pos + player_width/2,y_pos+ player_height,ab_x_pos+ player_width/2 ,WINDOW_HIGNT ,0) > 10 &&! flag_jump)   
			ChangeState("����");	
		else
			y_pos = back.obstacle->GetObstaclePosition(ab_x_pos + player_width/2,y_pos + player_height/2 , ab_x_pos + player_width/2, 440 , 2) - player_height;
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
		if (nKey == VK_SPACE)           //  ��ҷ����ӵ�
		{
			PlaySound("��ǹ");
		}

	}


	//����ƶ�
	void Move( WPARAM nkey)
	{
		int dis = back.obstacle->GetObstaclePosition(ab_x_pos + (direct +1)/2 *player_width,y_pos + player_height/2,(float)ab_x_pos - x_pos +(direct +1)/2 *WINDOW_WEIGHT, y_pos + player_height/2 , 0);	
		if (dis <= 0 && dis != -999 )return;
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
			if(this->x_pos - player_speed<0)return;  //��ֹ�������߽�
			else
			{
				this->x_pos -= player_speed ;
				this->ab_x_pos -= player_speed;
				this->SetPlayerYPos();
			}	
		}
	}
	//��ҵ���
	void Down(HDC hMemDC)
	{
		int player_down_y_speed = 5 ;
		int floor_distance = back.obstacle->GetObstaclePosition(ab_x_pos + (direct + 1)/2 * player_width/2,y_pos+player_height,ab_x_pos+ (direct + 1)/2 * player_width/2,WINDOW_HIGNT,0);
		if (floor_distance<=0 && floor_distance != -999) // ���Ｔ���������
		{							
			SetPlayerYPos();
			ChangeState("վ��");
		}
		player_show.show(hMemDC,state,&x_pos,&y_pos,direct,20,flag_cycle_show,0,player_down_y_speed ,&ab_x_pos);
	}
	//�����Ծ , �м��и��ַ������ж�
	void Jump(HDC hMemDC)
	{
		if (!flag_jump) //��¼��ʼ��Ծʱ, ��ҵ�λ��
			begin_jump_y_pos = y_pos; 
		player_jump_x_speed = 0;         //�����Ծʱx�����ϵ��ٶȵ���0; ֻ�е�����A���Ҽ�ʱ, �Ż����ٶ�
		flag_jump = true;							//��Ծ��־, �����Ծ�Ĺ�����, �������Ҽ�, ����ָ����Ч, �� Action�����ж� , ��Ϊtrueʱ, ��Ϊ����������Ծ�Ĺ�����
		if (begin_jump_y_pos - y_pos >=jumping_height  && player_jump_y_speed <0 )// ����һ���߶�, y�ٶȷ���			
			player_jump_y_speed = -player_jump_y_speed;		
		if(GetAsyncKeyState(FORWARD) ) // �����Ҽ� ,����û���ϰ���, ����ҷ�����ٶ�
			player_jump_x_speed =  3; 
		if(GetAsyncKeyState(RETREAT) && this->x_pos -player_jump_x_speed >0) // ��������������ﻹû����Ļ�߽�, ��������ϵ��ٶ�
			player_jump_x_speed =  -3; 	
		//�������ж��½��������� 
		int floor_distance = back.obstacle->GetObstaclePosition(ab_x_pos + player_width /2,y_pos+88,ab_x_pos +player_width /2,WINDOW_HIGNT,0);
		if (floor_distance<=0 && floor_distance != -999 && player_jump_y_speed>0) // ���Ｔ�����������Ծ���
		{			
			ChangeState("վ��");			//�ָ�״̬Ϊվ�� , ��������ҵ�y����
			SetPlayerYPos();
			player_jump_y_speed = -5;		
			flag_jump = false;					//��Ծ��־��λ
		}
		//��Ծʱ������ǰ���ķ����, ��������Ѿ�����Ļ����, ��ʱ, ��ҵ���Ծ��ǰ�����ٶ�Ϊ0, ���ñ����ƶ�, �ٶ�Ϊ��ҵ���Ծǰ�����ٶ�
		if(GetAsyncKeyState(FORWARD) && this->x_pos >= WINDOW_WEIGHT/2 )
		{
			back.Move(2);
			this->ab_x_pos += 2;			//��ҵľ�������ı�
			player_jump_x_speed = 0;
		}
		//ִ����Ծ����ʾ����, �Դ��ƶ�
		player_show.show(hMemDC,state,&x_pos,&y_pos,direct,20,flag_cycle_show,player_jump_x_speed,player_jump_y_speed ,&ab_x_pos);
	}

	//���״̬��λ, �ڰ���̧���ʱ�򱻵���
	void Restore(WPARAM nKey){if ( (nKey == FORWARD || nKey == RETREAT) &&  !flag_jump) ChangeState("վ��");}
};

