#include "Sys.h"
#include <vector>
class CycleShow
{
	//一个自定义的结构体, 保存图片的信息
	struct Picture
	{
		HBITMAP bitmap;
		BITMAP	bmp;
	};

	int show_id;
	int time_first;
	int time_second;		// 记录重绘的时间 , 这样就不用使用定时器
	vector<Picture> vct_picture;
	HDC hMemDC;			//此DC用于贴图使用

public:
	CycleShow()
	{
		 show_id = 0;
		 time_first = 0;
		 time_second = 0;
	}
	~CycleShow()
	{
		DeleteObject(hMemDC);
		for (int i = 0; i < vct_picture.size(); i++)
			DeleteObject(vct_picture[i].bitmap);
	}

	void LoadRes(const char * path)
	{
		Picture temp_picture;
		temp_picture.bitmap = (HBITMAP)LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//加载位图
		if(!temp_picture.bitmap)return;
		GetObject(temp_picture.bitmap, sizeof(BITMAP), (LPBYTE)&temp_picture.bmp);//获取位图信息(高度与宽度)
		vct_picture.push_back(temp_picture);	//添加至数组中
	}

	// 这里需要加载图片信息
	void Init(HDC hMemDC, char * picture_path,...)  //可变参数的, 参数的个数有外界来定
	{
		this->hMemDC = hMemDC;
		char ** str = &picture_path; //确定第一个参数的地址
		while(!strncmp(*str,"res\\",4)) // 判断后面还是否有参数(因此, 约定好资源的路径必须是要在当前文件夹下的res文件夹下面)
		{
			LoadRes(*str);
			str++;      // 参数指针后移, 取后面的字符串
		}
	}
	void Init(HDC hMemDC, int num , char * picture_path, ...)
	{
		this->hMemDC = hMemDC;
		if(num > 9999) num = 9999; //暂时定的最多加载9999张, 多了不会崩
		int _num_ = 0;						//用于string下标访问字符
		char ** str = &picture_path; //用指针一个一个取可变参数的中的字符串, 78行中 str++
		while(!strncmp(*str,"res\\",4))//判断传入的是否有效, 因此, 图片路径必须要以 res\ 开头, 其他路径认为无效
		{
			if (strncmp(*(str+1),"res\\",4))//判断是否为最后一个参数
			{
				string s = *str;				//使用string的系列函数进行操作, 比char快不少
				while (s[_num_]<'0' || s[_num_]>'9' ||  s[_num_+1] != '.')_num_ ++ ; // 找到第一次出现数字, 并且在 " . " 前面的字符位置
				for (int i =s[_num_]-'0'; i < num + s[_num_]-'0'; i++)			//进行循环, 这里注意 i 的判断为 i < num + s[_num_]-'0'
				{
					string s2 = s;		// 临时string,因为72行中会改变s的内容
					int bit =1;				//设置位数, 72行中替换字符会用到
					char str[10] ;		//itoa用到的临时空间
					if (i>=10)bit = 2;	//改变字符替换的位数
					if (i>=100)bit = 3;
					if (i>=1000)bit = 4;
					s2.replace(_num_,bit,itoa(i,str,10));	//string 的替换函数
					LoadRes(s2.c_str());							//根据已经处理好的字符串进行加载资源
				}
				return;															// for循环结束之后, 已经加载完毕, 退出即可
			}
			LoadRes(*str);
			str++;      // 参数指针后移, 取后面参数中的字符串
		}
	}
	//这里需要逐张显示图片
	void show(int x_pos, int y_pos, int draw_time)
	{
		time_first = GetTickCount();
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if (vct_picture.size())
		{
			SelectObject(hTempDC,vct_picture[show_id].bitmap);
			TransparentBlt(hMemDC,x_pos,y_pos,vct_picture[show_id].bmp.bmWidth,vct_picture[show_id].bmp.bmHeight,hTempDC,0,0,vct_picture[show_id].bmp.bmWidth,vct_picture[show_id].bmp.bmHeight,RGB(0,0,0));
			if (time_first - time_second > draw_time)
			{
				show_id++;
				time_second = GetTickCount();
			}
		}
		
		if (show_id > vct_picture.size()-1)show_id =0;
		DeleteDC(hTempDC) ;
	}

};


