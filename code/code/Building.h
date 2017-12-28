#pragma once
#include "Sys.h"


//���������ϰ������Ϣ
struct BuildInfo 
{
	int x_pos, y_pos; // x,y����
	int width, height; // �߶�����
	bool flag_pass; // �Ƿ��ܴ����洩��
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
	//���һ���µ��ϰ��ﵽ������
	void AddBuilding(int x, int y , int width, int height ,bool flag_pass) 
	{
		BuildInfo new_build = {x,y,width,height,flag_pass};
		vct_building.push_back(new_build);
	}

	//�ж���������ҷ������Ƿ����ϰ���
	bool IsObstacle(PLAYER_DIRECT direct , int x_pos, int y_pos) //����涨����������������Ҫ�жϵ��Ǹ���������½�����, �������(���緽��Ϊ��), �����������ҽ��µ�����
	{
		if (direct == RIGHT )
		{
			// ���ϰ������������������, ������Ҽ����ٶ����Ͼ�Ҫ�����ϰ���, ����������½�y��������ϰ����yҪ��, ���Ҹ��ϰ��ﲻ�ܱ�Խ�� , ����Ϊ���ϰ���
			if (this->vct_building.front().x_pos > x_pos && x_pos + PLAYER_SPEED >= this->vct_building.front().x_pos && y_pos > this->vct_building.front().y_pos && this->vct_building.front().flag_pass == false) 
				return true;
			else
				return false;
		}
		if (direct == LEFT)
		{

		}
	}

	//�ϰ��ﳬ����Ұ��Χ֮����ɾ��
	void DeleteBuild(int player_x_pos)
	{
		
	}

};