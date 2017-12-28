#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>
#include <string>
#include<io.h> 
#include <map>
#include <process.h>
#include "resource.h"
#pragma comment (lib,"Msimg32.lib")

using namespace std;

#define WINDOW_WEIGHT 1000
#define WINDOW_HIGNT		450
#define PLAYER_SPEED 6


enum PLAYER_DIRECT
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT=5,
	JUMP = 6,
};


struct SetDROP  //��һ���ṹ��, �������ö����ĸ��ֲ���, ����ṹ�������û��Ľӿ�
{
	TCHAR * picture_path;							//ͼƬ��·��
	float x_speed;											// X����ĳ��ٶ�
	float y_speed;											// Y����ĳ��ٶ�
	float G;														// �������ٶ�
	float  x_F;													// X�����Ħ����
	float  y_F;													// Y�����Ħ����
	int high;														//����ĸ߶�
	float x_begin_pos;									//�������ʼ��x
	float y_begin_pos;									//�������ʼ��y
	int drow_speed;										//�ػ���ٶ�(����Ϊ��λ)
	bool repet;													//�Ƿ��ظ�
};
struct SetFALL
{
	TCHAR* picture_path;							//ͼƬ��·��
	int x_rand;												//��Ҫ����ķ�Χ
	float y_begin_pos;								//��ʼ�����y������
	float high;												//����ĸ߶�
	float speed;											//������ٶ�
	int swing	;											//����ҡ�ڵķ�Χ
	int max_num;										//��������
	int drow_speed;									//�ػ���ٶ�(����Ϊ��λ)
	bool repet;												//�Ƿ��ظ�
};
struct SetStar
{
	TCHAR* picture_path;							//ͼƬ��·��
	float x_begin;										//��ʼ���ŵ�λ��,���Ϊ0,������x_range��y_range�����
	float y_begin;
	int x_range;											//��ʼ���ŵķ�Χ
	int y_range;
	int x_max_range;									//�����ķ�Χ
	int y_max_range;
	int max_speed;										//������е�����ٶ�
	int exist_time;										//������ڵ�ʱ��
	int max_num;										//������������
	int drow_time;										//�ػ�һ�ε�ʱ��(����Ϊ��λ)
	bool repet;												//�Ƿ��ظ�
};
//������ʱ���ָ���ַ�ռ��һ��������, WcharToChar �� CharToWchar ���õ���,  �����Ļ���������ֱ�ӷ����ַ�����, ���ᵼ���ڴ�й©������
class BUFFER
{
public:
	BUFFER(){pdelete = NULL;}
	~BUFFER(){ Destory(nullptr);}

	void Destory(void * temp)
	{
		if (!pdelete)
			delete(pdelete);
		pdelete = temp;
	}
private:
	void * pdelete;
};
//����һ��ȫ�ֵ���ʱbuff
static char *  WcharToChar(WCHAR* widestr)
{
	BUFFER Tempbuff;
	int num = WideCharToMultiByte(CP_OEMCP,NULL,widestr,-1,NULL,0,NULL,FALSE);
	char *pchar = new char[num];
	Tempbuff.Destory(pchar);
	WideCharToMultiByte (CP_OEMCP,NULL,widestr,-1,pchar,num,NULL,FALSE);
	return pchar;
}
static LPCWSTR CharToWchar(char* ch)
{
	BUFFER Tempbuff;
	int num = MultiByteToWideChar(0,0,ch,-1,NULL,0);
	wchar_t *wide = new wchar_t[num];
	Tempbuff.Destory(wide);
	MultiByteToWideChar(0,0,ch,-1,wide,num);
	return wide;
}
