#include "CycleShow.h"

class Player
{
public:
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@����Ҫ�Ľ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//���غö�ö��λͼ, ���������ڽ�����ͼ��ʱ��ͬͼƬ�����д�С��ͬ������, ��������뿴CycleShow��ͷ�ļ�
	//HBITMAP bit_player_left_still;
	//HBITMAP bit_player_right_still;
	//HBITMAP bit_player_left_run;
	//HBITMAP bit_player_right_run;

	CycleShow play_show;

	float x_pos;				//��¼���������
	float y_pos;				
	int time_first;
	int time_second;		// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	int show_id;
	int direct;					//����, ��������-> 1234
	int state;					//״̬, վ, ��, ��, ��, -> 1234
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

	void Init(HDC hMemDC)
	{
		play_show.Init(hMemDC,100,"res\\player\\�󻨿����ܿ���2.bmp");
	}
	void Show()
	{
		play_show.show(this->x_pos,this->y_pos,200);
	}

	//void SelectBitmap(HDC hTempDC)
	//{
	//	switch (state)
	//	{
	//	case 1: 
	//		if (direct == 3)::SelectObject(hTempDC,bit_player_left_still);
	//		if (direct == 4)::SelectObject(hTempDC,bit_player_right_still);
	//		break;
	//	case 2: 
	//		if (direct == 3)::SelectObject(hTempDC,bit_player_left_run);
	//		if (direct == 4)::SelectObject(hTempDC,bit_player_right_run);
	//		break;
	//	case 3: 
	//		break;
	//	case 4: 
	//		break;
	//	}

	//}

	//void ChangeShowId()
	//{
	//	time_first = GetTickCount();	
	//	if (time_first - time_second >= 100)
	//	{
	//		switch (state)
	//		{
	//		case 1: 
	//			if (show_id >= 1)	
	//				show_id = 0;
	//			else
	//				show_id ++;
	//			break;
	//		case 2: 
	//			if (show_id >= 4)	
	//				show_id = 0;
	//			else 
	//				show_id ++;
	//			break;
	//		case 3: 
	//			break;
	//		case 4: 
	//			break;
	//		}
	//		time_second = GetTickCount();
	//	}
	//	
	//}

	//void DrowBit(HDC hMemDC,HDC hTempDC)
	//{
	//	switch (state)
	//	{
	//	case 1:
	//		TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,71+show_id,90,hTempDC,show_id*71,0,71+show_id,90,RGB(0,0,0));
	//		break;			
	//	case 2:
	//		if (show_id == 0 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,64,90,hTempDC,0,0,64,90,RGB(0,0,0));
	//		if (show_id == 1 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,73,90,hTempDC,64,0,73,90,RGB(0,0,0));
	//		if (show_id == 2 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,71,90,hTempDC,137,0,71,90,RGB(0,0,0));
	//		if (show_id == 3 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,76,90,hTempDC,208,0,76,90,RGB(0,0,0));
	//		if (show_id == 4 )TransparentBlt(hMemDC,(int)x_pos,(int)y_pos,76,90,hTempDC,284,0,76,90,RGB(0,0,0));
	//		break;
	//	case 3:
	//		break;
	//	case 4:
	//		break;
	//	}
	//}
};
// 1.��ͬ��ͼƬ  ��С ��һ��,  ��ô�����Զ���ȡ��С? ����һ������
//2. �����в�ͬ��״̬, վ, pao, ��, �� , ״̬����ҲҪ���в�ͬ����ͼ, ������һ��Ҫ�жϵ�, �ɼ������ı䲻ͬ��״̬
//3. �һ�Ҫ����ͬ������ͼƬ������һ��, ��ܷ�
// 4. show_id ҲҪ�����ж�, ����ҲҪ���