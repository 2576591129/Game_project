#include <windows.h>
#include <iostream>
#include <time.h>
#pragma comment (lib,"Msimg32.lib")
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
BUFFER Tempbuff;
char *  WcharToChar(WCHAR* widestr)
{
	int num = WideCharToMultiByte(CP_OEMCP,NULL,widestr,-1,NULL,0,NULL,FALSE);
	char *pchar = new char[num];
	Tempbuff.Destory(pchar);
	WideCharToMultiByte (CP_OEMCP,NULL,widestr,-1,pchar,num,NULL,FALSE);
	return pchar;
}
LPCWSTR CharToWchar(char* ch)
{
	int num = MultiByteToWideChar(0,0,ch,-1,NULL,0);
	wchar_t *wide = new wchar_t[num];
	Tempbuff.Destory(wide);
	MultiByteToWideChar(0,0,ch,-1,wide,num);
	return wide;
}

//***********************************************物体下落(模拟重力以及摩擦力)动画**************************************//
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
class DROP
{
public:
	BITMAP bmp;												//存放位图的信息, 这里主要用来获取位图的高度和宽度
	HBITMAP bit_bird;									//鸟的位图
	SetDROP fall;												//存放物体下落信息的结构体
	float x_position_bird;								//物体的当前位置
	float y_position_bird;								//物体的当前位置
	bool sign;													//用于标识动画是否循环
	float x_s;													//保留物体的初始x速度
	float y_s;													//保留物体的初始y速度				
	int time_first;
	int time_second;
public:
	DROP()
	{
		time_first = 0;
		time_second = 0;
		sign = true;
		x_position_bird = 0;
		y_position_bird = 0;
	}
	~DROP()
	{
		DeleteObject(bit_bird);
	}

	void Init(SetDROP* falled )
	{
		fall = * falled;							// 存放好要设置的参数														
		x_s = fall.x_speed;				//保存下落的初始速度, 因为该值会被修改(下落的速度回越来越快,还有阻力影响)
		y_s = fall.y_speed;
		bit_bird =(HBITMAP)LoadImage(NULL,fall.picture_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); //加载位图
		GetObject(bit_bird, sizeof(BITMAP), (LPBYTE)&bmp);//获取位图信息(高度与宽度)
	}

	void Show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		::SelectObject(hTempDC,bit_bird);
		//使用黑白贴图
		BitBlt(hMemDC,(int)x_position_bird,(int)y_position_bird,bmp.bmWidth/2,bmp.bmHeight,hTempDC,bmp.bmWidth/2,0,SRCAND);
		BitBlt(hMemDC,(int)x_position_bird,(int)y_position_bird,bmp.bmWidth/2,bmp.bmHeight,hTempDC,0,0,SRCPAINT);	

		time_first = GetTickCount();
		// ********************************************* 小鸟下落的重力效果与阻力效果*************************************************
		if (sign && time_first - time_second >= fall.drow_speed) // 控制动画是否循环的 , 并且动画控制动画的重绘的速度
		{
			if (y_position_bird >= fall.high) //下落到指定的高度时, 速度反向
			{
				y_position_bird =(float) fall.high;
				fall.y_speed = -fall.y_speed;
			}
			fall.y_speed =fall.y_speed +fall.G + fall.y_F;		//速度回收到重力与阻力的影响, 这里使用的是速度公式
			if (fall.x_speed + fall.x_F>=0)
			{
				fall.x_speed += fall.x_F;										//如果x方向上的速度不小于0, 则收到阻力的影响, 依然会改变
			}		
			y_position_bird += fall.y_speed;								//调整物体的方向
			x_position_bird +=fall.x_speed;


			//这个超级长的逻辑判断, 是用来确定物体是否已经完全运动完毕, 用的是绝对值的判断
			if (y_position_bird >= fall.high && fall.x_speed+fall.x_F <=0 && fall.y_speed >=-fall.G && fall.y_speed <= fall.G )
			{			
				if (fall.repet == true)		//如果重复动画, 则将物体的位置重置, 并且初始速度重置
				{
					fall.y_speed = y_s;
					fall.x_speed = x_s;
					x_position_bird = fall.x_begin_pos;
					y_position_bird = fall.y_begin_pos;
				}
				else
					sign = false;					//如果不循环动画, 改变sign的值, 将不再有动画效果, 但是物体的显示依然存在
			}
			time_second = GetTickCount();
		}
		::DeleteDC(hTempDC);
	}
};

//****************************************************************************************************************//


//******************************************************雪花动画*********************************************//
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
class SNOWFALL
{
private:
	//这个结构体是用于内部使用的, 用户无法获取到, 用于保存物体的坐标以及是否存在
	struct Snow
	{
		float x_pos;
		float y_pos;
		bool exists;
	};
public:
	SNOWFALL()
	{
		snow_num = 0 ;
		arr_Snow = NULL;
		sign = true;
		time_second = 0;
		time_first = 0;
	}
	~SNOWFALL()
	{
		delete arr_Snow;
		arr_Snow = NULL;
		DeleteObject(bit_snow_flower);
	}

	void Init(SetFALL * falled)
	{
		fall = *falled;			//保存动画参数
		arr_Snow = new Snow[fall.max_num]; // 使用动态分配空间的方式,设置物体的数量
		bit_snow_flower = (HBITMAP)LoadImage(NULL,fall.picture_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//加载位图
		GetObject(bit_snow_flower, sizeof(BITMAP), (LPBYTE)&bmp);//获取位图信息(高度与宽度)
	}

	void Show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		for (int i = 0; i < fall.max_num; i++)
		{
			if (arr_Snow[i].exists)//如果物体存在则贴图
			{
				::SelectObject(hTempDC,bit_snow_flower);
				TransparentBlt(hMemDC,(int)arr_Snow[i].x_pos,(int)arr_Snow[i].y_pos,bmp.bmWidth,bmp.bmHeight,hTempDC,0,0,bmp.bmWidth,bmp.bmHeight,RGB(0,0,0));
			}
		}

		time_first = GetTickCount();	
		if (sign && time_first - time_second >= fall.drow_speed)//控制动画训循环的标志
		{
			if (snow_num < fall.max_num)//如果物体还没有达到设定的最大值, 则继续添加
			{
				arr_Snow[snow_num].x_pos =(float)(rand()% (fall.x_rand));		//物体的初始X坐标在设定的范围内随机
				arr_Snow[snow_num].y_pos = fall.y_begin_pos;				// 物体开始下落的初始Y值
				arr_Snow[snow_num].exists = true;									
				snow_num ++ ;
			}
			for (int i = 0; i < fall.max_num; i++)
			{
				//如果设定的左右摇摆的值不为0, 则在设定的值之内随机一个数, 进行左右摇摆(左或者右也是随机)
				if(rand()%2==0 && fall.swing!=0)
					arr_Snow[i].x_pos += rand()%fall.swing;  //x坐标加上0~5之间的一个随机值
				else if(rand()%2==1 && fall.swing!=0)
					arr_Snow[i].x_pos-=rand()%fall.swing;	 //y坐标加上0~5之间的一个随机值
				arr_Snow[i].y_pos +=fall.speed;
				if (arr_Snow[i].y_pos >fall.high)  
				{
					if (fall.repet == true) // 如果下降到最大高度并且动画效果重复 , 则将物体的y值置为设定的y_begin_pos, x则在设定的范围内随机一个数
					{
						arr_Snow[i].y_pos = fall.y_begin_pos;
						arr_Snow[i].x_pos = (float) (rand()% (fall.x_rand));
					}
					else if(fall.repet == false && snow_num>=fall.max_num) //如果动画不随机,并且物体达到最大值, 则动画显示完毕 , 物体也将不存在
						sign = false;
				}

			}
			time_second = GetTickCount();
		}

		::DeleteDC(hTempDC);
	}

private:
	SetFALL  fall;
	int snow_num;											//当前物体的数量
	HBITMAP bit_snow_flower;		
	BITMAP bmp;
	Snow* arr_Snow;									//雪花的数组, 最大为50
	bool sign;													//控制动画重复
	int time_first;
	int time_second;
};
//****************************************************************************************************************//


//****************************************************星光绽放动画*******************************************//
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
class STAR		
{
	struct Star
	{
		float x_pos;
		float y_pos;
		int x_speed;
		int y_speed;
		int exist_time;
		bool exist;
	};

public:
	BITMAP bmp;
	HBITMAP bit_star;									//星星的位图
	Star *arr_star;										//星星的数组, 最大为50
	int stat_num;												//当前星星的数量
	SetStar set_star;
	bool sign;
	int time_first;
	int time_second;
public:
	STAR()
	{
		stat_num = 0;
		sign = true;
		time_second = 0;
		time_first = 0;
	}
	~STAR()
	{
		if (arr_star != NULL)
		{
			delete arr_star;
			arr_star = NULL;
		}
		DeleteObject(bit_star);
	}

	void Init(SetStar * temp_star)
	{
		set_star = *temp_star;
		arr_star = new Star[set_star.max_num];
		bit_star = (HBITMAP)LoadImage(NULL,set_star.picture_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//加载位图
		GetObject(bit_star, sizeof(BITMAP), (LPBYTE)&bmp);//获取位图信息(高度与宽度)
	}

	void Show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		
		if (sign == true)   //控制是否循环
		{
			if (stat_num == 0)	// 如果当先的数量为0 , 一次性就把需要的星星全部创建好
			{
				int x_bom ,y_bom;	//星星初始爆炸的位置
				if (set_star.x_begin ==0&& set_star.y_begin ==0)  //如果设置的初始爆炸的位置为0 , 则会在 x_range与y_range中随机爆炸点
				{
					x_bom = rand() % (set_star.x_range);
					y_bom =  rand() % (set_star.y_range);
				}
				else
				{
					x_bom =(int) set_star.x_begin;
					y_bom = (int)set_star.y_begin;
				}
				for (int i = 0; i < set_star.max_num; i++)		//给星星的结构体内容赋值
				{
					arr_star[i].x_pos =(float) x_bom;
					arr_star[i].y_pos = (float)y_bom;
					arr_star[i].exist_time = 0;
					switch ( i%4)													//依次决定星星飞行的象限
					{
					case 0:
						arr_star[i].x_speed = 1+rand() % set_star.max_speed;
						arr_star[i].y_speed = 1+rand() % set_star.max_speed;
						break;
					case 1:
						arr_star[i].x_speed = 1 + rand() % set_star.max_speed;
						arr_star[i].y_speed= -(1+ rand() % set_star.max_speed);
						break;
					case 2:
						arr_star[i].x_speed= - (1+rand() % set_star.max_speed);
						arr_star[i].y_speed =-(1+ rand() % set_star.max_speed);
						break;
					case 3:
						arr_star[i].x_speed = -(1+rand() % set_star.max_speed);
						arr_star[i].y_speed = 1+rand() % set_star.max_speed;
						break;
					}
					arr_star[i].exist =true;
				}
				stat_num = set_star.max_num;			
			}

			for(int i=0;i<set_star.max_num;i++) //判断粒子是否还存在,若存在，则根据其坐标（arr_star[i].x_pos,arr_star[i].y_pos）进行贴图操作
			{
				if(arr_star[i].exist)  
				{
					SelectObject(hTempDC,bit_star);
					TransparentBlt(hMemDC,(int)arr_star[i].x_pos,(int)arr_star[i].y_pos,bmp.bmWidth,bmp.bmHeight,hTempDC,0,0,bmp.bmWidth,bmp.bmHeight,RGB(0,0,0));
				}
			}

			time_first = GetTickCount();	
			if (time_first - time_second >=set_star.drow_time)
			{
				for(int i=0;i<set_star.max_num;i++)
				{
					if(arr_star[i].exist )   //判断粒子是否还存在,若存在，则根据其坐标（FlyStars[i].x,FlyStars[i].y）进行贴图操作
					{
						//计算下一次贴图的坐标
						arr_star[i].x_pos+=arr_star[i].x_speed;
						arr_star[i].y_pos+=arr_star[i].y_speed;

						//在每进行一次贴图后，将粒子的存在时间累加1.
						arr_star[i].exist_time++;
						//进行条件判断，若某粒子跑出窗口区域一定的范围，则将该粒子设为不存在，且粒子数随之递减
						if(arr_star[i].x_pos<=-10 || arr_star[i].x_pos>set_star.x_max_range || arr_star[i].y_pos<=-10 || arr_star[i].y_pos>set_star.x_max_range || arr_star[i].exist_time>set_star.exist_time)
						{
							arr_star[i].exist = false;  //删除星光粒子 
							stat_num--;                    //递减星光总数
							if (stat_num == 0 && set_star.repet == false) sign = false;				
						}
					}
				}
				time_second = GetTickCount();
			}
		}
		DeleteDC(hTempDC);
	}
};