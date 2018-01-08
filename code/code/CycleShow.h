#pragma once
#include "Sys.h"
#define PICTURE_RES_PATH "res\\picture\\"
#define SOUND_RES_PATH "res\\sound\\"

class  LoadRes;
extern LoadRes  loadres;


//加载图片资源类
class  LoadRes
{
	struct Pct_res
	{
		HBITMAP bitmap;
		int height;
		int width;
	};
public:
	map<string , vector<Pct_res>>map_picture;
	string picture_path;
public:
	LoadRes()
	{
		picture_path = PICTURE_RES_PATH;
		picture_path += "*.bmp";
	}
	~LoadRes(){}
public:
	void Init()
	{                                        
		struct _finddata_t fileinfo;                          //文件信息的结构体 
		long handle=_findfirst(picture_path.c_str(),&fileinfo);  //用于查找的句柄        //第一次查找 
		if(-1==handle)return ; 
		Save(fileinfo.name);     //保存信息
		while(!_findnext(handle,&fileinfo))Save(fileinfo.name);     //循环查找其他符合的文件，知道找不到其他的为止 
		_findclose(handle);    
	}
	void Save(string s) // s 中的内容是 res\picture\文件将下所有位图的名称, 例如: 右站立1.bmp
	{
		string str_path = PICTURE_RES_PATH + s;			//拿到文件的路径的全称 , 加载位图用
		Pct_res pict;
		pict.bitmap = (HBITMAP)LoadImage(NULL,str_path.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//加载位图
		if(!pict.bitmap)return; //如果加载失败, 则返回
		BITMAP bmp;				//存放位图信息
		GetObject(pict.bitmap, sizeof(BITMAP), (LPBYTE)&bmp);//获取位图信息(高度与宽度)
		pict.height = bmp.bmHeight;
		pict.width = bmp.bmWidth;
		int _num_ = 0;
		while (s[_num_]<'0' || s[_num_]>'9' )_num_ ++; // 获取最后一个出现数字的位置下标
		s = s.substr(0,_num_);										// 截获指定长度的字符, 例如 : "右站立1.bmp"  将会拿到 "右站立" 作为map的键值
		map_picture[s].push_back(pict);					// 把该键值对应的位图信息 pict 添加到对应的数组中   以后显示位图的时候, 直接指明键值: 右站立, 获取相应的vector, 循环显示就好
	}
};

//动画循环显示类
class CycleShow
{
	int show_id;				// 用来控制第几张图片显示用的, 在全局map的对象loadres中, 不同的键值有不同数量的位图, 需要用show_id进行控制
	int time_first , time_second;// 记录重绘的时间 , 这样就不用使用定时器
	int x,y;						//用来记录物体移动时的坐标
	POINT pos;
public:
	CycleShow()
	{
		pos.x = 0 ;
		pos.y = 0;
		show_id = 0;
		time_first = 0;
		time_second = 0;		
	}
	~CycleShow(){}

	POINT show(HDC hMemDC,string key_value,int x_pos, int y_pos,int direct, int draw_time,bool flag_cycle ,bool auto_move = false ,int x_speed = 0, int y_speed = 0)
	{
		time_first = GetTickCount();	//记录重绘的时间
		if ( pos.x == 0 && pos.y == 0 && auto_move == true)   // 需要自动移动功能
		{
			pos.x = x_pos; 
			pos.y = y_pos;
		}
		if (auto_move == false) // 不需要自动移动  (虽然这两个if判断里面的语句时一样的, 但是绝对不能放一起, 因为循环的条件不一样
		{
			pos.x = x_pos; 
			pos.y = y_pos;
		}
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if(loadres.map_picture[key_value].size())//当加载的位图为空时, 并进行贴图, 防止访问空指针
		{
			if ((unsigned)show_id>=loadres.map_picture[key_value].size())show_id =0;	//这句必须判断, 否则, key_value改变时候, 相应的show_id没有还原, 将有可能会访越界
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,pos.x + (direct+1)*loadres.map_picture[key_value][show_id].width/2,pos.y,(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id++].height,SRCAND);
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,pos.x + (direct+1)*loadres.map_picture[key_value][show_id].width/2,pos.y,(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id--].height,SRCPAINT);
			if (time_first - time_second > draw_time) //判断是否达到时间
			{
				show_id +=2 ;
				pos.x+=x_speed;
				pos.y+=y_speed;
				time_second = GetTickCount();//记录重绘的时间
			}
		}
		if (show_id > loadres.map_picture[key_value].size()-1)   //循环周期到了, show_id从0再开始
		{
			if (flag_cycle )show_id =0;
			else show_id -=2;
		}	
		DeleteDC(hTempDC);
		return pos;
	}


	void show(HDC hMemDC,string key_value,float *x_pos, float *y_pos,int direct, int draw_time,bool flag_cycle,float x_speed = 0, float y_speed = 0 , float *extend_num = NULL)
	{
		time_first = GetTickCount();	//记录重绘的时间
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if(loadres.map_picture[key_value].size())//当加载的位图为空时, 并进行贴图, 防止访问空指针
		{
			if ((unsigned)show_id>=loadres.map_picture[key_value].size())show_id =0;	//这句必须判断, 否则, key_value改变时候, 相应的show_id没有还原, 将有可能会访越界
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,((int)*x_pos) + (direct+1)*loadres.map_picture[key_value][show_id].width/2,((int)*y_pos),(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id++].height,SRCAND);
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,((int)*x_pos) + (direct+1)*loadres.map_picture[key_value][show_id].width/2,((int)*y_pos),(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id--].height,SRCPAINT);
			if (time_first - time_second > draw_time) //判断是否达到时间
			{
				show_id +=2 ;
				(*x_pos)+=x_speed;
				(*y_pos)+=y_speed;
				if (extend_num)(*extend_num) += x_speed;				
				time_second = GetTickCount();//记录重绘的时间
			}
		}
		if (show_id > loadres.map_picture[key_value].size()-1)   //循环周期到了, show_id从0再开始
		{
			if (flag_cycle )show_id =0;
			else show_id -=2;
		}	
		DeleteDC(hTempDC);
	}

};

//背景障碍物类
class BackObstacle	
{
private:
	BITMAPINFOHEADER bih;   //位图的信息头
	BYTE *Buffer ;						//用于读取位图信息的空间
	long LineByteWidth;  
	string name_bitmap;
private:
	int ReadBmp( )
	{
		FILE *file;  
		WORD bfh[7];  
		long dpixeladd;  
		if (NULL == (file = fopen(name_bitmap.c_str(), "rb")))  
			return 0;  
		fread(&bfh, sizeof(WORD), 7, file);  
		if (bfh[0] != (WORD)(((WORD)'B')|('M'<<8)))  
		{  
			fclose(file);  
			return 0;  
		}  
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, file);  
		dpixeladd = bih.biBitCount / 8;  
		LineByteWidth = bih.biWidth * (dpixeladd);  
		if ((LineByteWidth % 4) != 0)  
			LineByteWidth += 4 - (LineByteWidth % 4);  
		if ((Buffer = (BYTE*)malloc(sizeof(BYTE)* LineByteWidth * bih.biHeight)) != NULL)  
		{  
			fread(Buffer, LineByteWidth * bih.biHeight, 1, file);  
			fclose(file);  
			return 1;  
		}  
		fclose(file);  
		return 0;  
	}
	int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b)
	{
		int dpixeladd;  
		BYTE *ptr;  
		if (X < 0 || X >= bih.biWidth || Y < 0 || Y >= bih.biHeight)  
			return 0;  
		dpixeladd = bih.biBitCount / 8;  
		ptr = Buffer + X * dpixeladd + (bih.biHeight - 1 - Y) * LineByteWidth;  
		*b = *ptr;  
		*g = *(ptr + 1);  
		*r = *(ptr + 2);  
		return 1;  
	}
	bool GetColor( int Px, int Py )
	{
		BYTE r, g, b;  
		if (GetDIBColor(Px, Py, &r, &g, &b) == 1  &&(int) r==(int)g &&(int)g==(int)b &&(int)b ==0 )  return true;		
		return false;
	}
public:
	BackObstacle(){Buffer = NULL;}
	~BackObstacle(){free(Buffer);}
	void Init(string name)
	{
		name_bitmap = name;
		ReadBmp() ;
	}


	//以下的函数基本功能实现了, 把它进行精简一下 
	//传入的参数一个是当前坐标 , 一个是终点坐标(两点确定一条直线), 最后一个是返回值类型   0: 当前坐标距离障碍物的距离, 1: 障碍物的x坐标, 2: 障碍物的 y坐标 , 当返回值为-999 则没有障碍物或者参数传入有误
	int GetObstaclePosition(float firX_pos, float firY_pos, float secX_pos, float secY_pos , int return_type = 0) 
	{	
		if ( firX_pos<0 || firY_pos <0 ||  secX_pos <0 || secY_pos <0 || firY_pos > WINDOW_HIGNT || secY_pos > WINDOW_HIGNT ) return -999;	//参数传入有误
	
		if ( secX_pos == firX_pos)
		{
			for ( float temp_y = firY_pos ; temp_y < secY_pos ; temp_y++)
			{
				if (GetColor((int)firX_pos , (int ) temp_y ))
				{
					switch (return_type)
					{
					case 0 : return temp_y -  firY_pos;
					case 1: return (int) firX_pos;
					case 2:  return (int )temp_y;
					default: return -999;
					}			
				}
			}
			return -999;
		}
		float K= (secY_pos - firY_pos)/(secX_pos - firX_pos);	//计算两点间的斜率 K
		if (K== 0 )
		{
			for ( float temp_x = firX_pos ; temp_x < secX_pos ; temp_x++)
			{
				if (GetColor((int)temp_x , (int ) firY_pos ))
				{
					switch (return_type)
					{
					case 0 : return temp_x -  firX_pos;
					case 1: return (int) temp_x;
					case 2:  return (int )firY_pos;
					default: return -999;
					}			
				}
			}
			return -999;
		}
		int flag = (K>0)?1:-1;
		float x = firX_pos;
		float y = firY_pos;
		while (x+flag <= secX_pos &&  y+K <= secY_pos)
		{
			if (GetColor((int)x , (int ) y ))//有障碍物则返回当前坐标与传入坐标之间的距离
			{
				switch (return_type)
				{
				case 0 : return sqrt( (x-firX_pos) * (x-firX_pos) + (y-firY_pos)*(y-firY_pos));
				case 1: return (int) x;
				case 2:  return (int )y;
				default: return -999;
				}			
			}
			x+=flag;
			y+=K;
		}
		return -999;	//没有障碍物则返回-1
	}

};




