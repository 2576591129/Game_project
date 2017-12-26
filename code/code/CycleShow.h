#include "Sys.h"
#include <vector>
#include <string>
#include<io.h> 
#include <map>
#define PICTURE_RES_PATH "res\\picture\\"
#define SOUND_RES_PATH "res\\sound\\"

class  LoadRes;
extern LoadRes  loadres;
//现在需要一个另一个加载资源的类 , 类中有两个资源vector的数组, 分别用来存放图片 和 声音资源
// 详细加载的思路和CycleShow类的Init函数基本相同,在固定文件夹下自动查找

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

class CycleShow
{
	int show_id;
	int time_first;
	int time_second;		// 记录重绘的时间 , 这样就不用使用定时器
public:
	CycleShow()
	{
		show_id = 0;
		time_first = 0;
		time_second = 0;
	}
	~CycleShow(){}
	void show(HDC hMemDC,string key_value,int x_pos, int y_pos, int draw_time)
	{
		time_first = GetTickCount();	//记录重绘的时间
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if(loadres.map_picture[key_value].size())//当加载的位图为空时, 并进行贴图, 防止访问空指针
		{
			if (show_id>=loadres.map_picture[key_value].size())show_id =0;	//这句必须判断, 否则, key_value改变时候, 相应的show_id没有还原, 将会访越界
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			TransparentBlt(hMemDC,x_pos,y_pos,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,RGB(0,0,0));
			if (time_first - time_second > draw_time)
			{
				show_id++;
				time_second = GetTickCount();//记录重绘的时间
			}
		}
		if (show_id > loadres.map_picture[key_value].size()-1)show_id =0;	
		DeleteDC(hTempDC);
	}
};






