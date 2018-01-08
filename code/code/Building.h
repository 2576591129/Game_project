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
	HBITMAP hbitmap;
	int floor;
public:
	Building()
	{
		floor = 0;
	}
	~Building(){DeleteObject(hbitmap);}

public:
	//���һ���µ��ϰ��ﵽ������
	void AddBuilding(int x, int y , int width, int height ,bool flag_pass) 
	{
		BuildInfo new_build = {x,y,width,height,flag_pass};
		vct_building.push_back(new_build);
		hbitmap = (HBITMAP)LoadImage(NULL,"res\\angrybird.bmp",IMAGE_BITMAP,2*width,height,LR_LOADFROMFILE);//����λͼ
	}

	void show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		SelectObject(hTempDC,hbitmap);
		BitBlt(hMemDC,vct_building.front().x_pos,vct_building.front().y_pos,vct_building.front().width,vct_building.front().height,hTempDC,vct_building.front().width,0,SRCAND);
		BitBlt(hMemDC,vct_building.front().x_pos,vct_building.front().y_pos,vct_building.front().width,vct_building.front().height,hTempDC,0,0,SRCPAINT);
		DeleteDC(hTempDC);
	}

	//�ϰ��ﳬ����Ұ��Χ֮����ɾ��
	void DeleteBuild(int player_x_pos)
	{

	}

	void ChangeFloorHight()
	{

	}
};