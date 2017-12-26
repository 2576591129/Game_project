#include "CycleShow.h"
class Player
{
public:
	CycleShow player_show;
	float x_pos;				//��¼���������
	float y_pos;				
	int time_first;			// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	int time_second;		
	string  state;
public:
	Player()
	{
		x_pos = 20;
		y_pos =300;
		time_first = 0;
		time_second = 0;
		state = "��վ��";
	}
	~Player(){}
public:
	void ChangeState( const string key_value)
	{
		state = key_value;
	}
	void Show(HDC hMemDC)
	{
		player_show.show(hMemDC,state,this->x_pos,this->y_pos,130);
	}
};
