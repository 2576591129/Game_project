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


struct SetDROP  //来一个结构体, 用于设置动画的各种参数, 这个结构体是与用户的接口
{
	TCHAR * picture_path;							//图片的路径
	float x_speed;											// X方向的初速度
	float y_speed;											// Y方向的初速度
	float G;														// 重力加速度
	float  x_F;													// X方向的摩擦力
	float  y_F;													// Y方向的摩擦力
	int high;														//下落的高度
	float x_begin_pos;									//下落的起始点x
	float y_begin_pos;									//下落的起始点y
	int drow_speed;										//重绘的速度(毫秒为单位)
	bool repet;													//是否重复
};
struct SetFALL
{
	TCHAR* picture_path;							//图片的路径
	int x_rand;												//需要随机的范围
	float y_begin_pos;								//开始下落的y的坐标
	float high;												//下落的高度
	float speed;											//下落的速度
	int swing	;											//左右摇摆的范围
	int max_num;										//最大的数量
	int drow_speed;									//重绘的速度(毫秒为单位)
	bool repet;												//是否重复
};
struct SetStar
{
	TCHAR* picture_path;							//图片的路径
	float x_begin;										//初始绽放的位置,如果为0,将会再x_range与y_range中随机
	float y_begin;
	int x_range;											//初始绽放的范围
	int y_range;
	int x_max_range;									//物体活动的范围
	int y_max_range;
	int max_speed;										//物体飞行的最大速度
	int exist_time;										//物体存在的时间
	int max_num;										//物体的最大数量
	int drow_time;										//重绘一次的时间(毫秒为单位)
	bool repet;												//是否重复
};
//用于临时存放指针地址空间的一个缓存区, WcharToChar 与 CharToWchar 都用到了,  用它的话函数可以直接返回字符数组, 不会导致内存泄漏等问题
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
//定义一个全局的临时buff
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
