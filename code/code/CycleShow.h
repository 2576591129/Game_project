

#include "Sys.h"
#include <vector>
// 动画效果需要不停地循环贴图, 而图片的大小不能保证完全一样的大小, 在进行贴图的时候很麻烦
// 现在去写一个类, 实现的功能是: 
// 用户把图片的路径作为参数传进去(个数不定), 它能自动获取图片的大小, 并根据传参的顺序依次进行贴图


// 目前实现的思路:
// 首先它是一个类, 外界需要定义对象, 调用接口函数传入图片
// 类中有一个结构体, 结构体的内容大致为: HBIEMAP 类型的变量, BITMAP 类型的变量, 图片的大小, 用于显示图片的XY坐标
// 类中有一个数组, 能根据用户传进来的个数参数, 进行new不同的空间大小
// 类中有一个类似Show的函数, 用于循环显示所有图片
// 析构中进行空间已经资源的释放


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
	/*list<Picture> list_pacture; */ // 这里用链表虽然方便 , 但是最好不要用链表..... 因为访问慢, 用数组会比较快
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

	// 这里需要加载图片信息
	void Init(HDC hMemDC, char * picture_path,...)  //可变参数的, 参数的个数有外界来定
	{
		this->hMemDC = hMemDC;
		char ** str = &picture_path; //确定第一个参数的地址
		while(!strncmp(*str,"res\\",4)) // 判断后面还是否有参数(因此, 约定好资源的路径必须是要在当前文件夹下的res文件夹下面)
		{
			Picture temp_picture;
			temp_picture.bitmap = (HBITMAP)LoadImage(NULL,*str,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//加载位图
			GetObject(temp_picture.bitmap, sizeof(BITMAP), (LPBYTE)&temp_picture.bmp);//获取位图信息(高度与宽度)
			vct_picture.push_back(temp_picture);	//添加至数组中
			str++;      // 参数指针后移, 取后面的字符串
		}
	}

	//这里需要逐张显示图片
	void show(int x_pos, int y_pos, int draw_time)
	{
		time_first = GetTickCount();
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		//for (int i=0; i < vct_picture.size(); i++)
		//{
		//	SelectObject(hTempDC,vct_picture[i].bitmap);
		//	TransparentBlt(hMemDC,x_pos,y_pos,vct_picture[i].bmp.bmWidth,vct_picture[i].bmp.bmHeight,hTempDC,0,0,vct_picture[i].bmp.bmWidth,vct_picture[i].bmp.bmHeight,RGB(0,0,0));
		//}
		SelectObject(hTempDC,vct_picture[show_id].bitmap);
		TransparentBlt(hMemDC,x_pos,y_pos,vct_picture[show_id].bmp.bmWidth,vct_picture[show_id].bmp.bmHeight,hTempDC,0,0,vct_picture[show_id].bmp.bmWidth,vct_picture[show_id].bmp.bmHeight,RGB(0,0,0));
		if (time_first - time_second > draw_time)
		{
			show_id++;
			time_second = GetTickCount();
		}
		
		if (show_id > vct_picture.size()-1)show_id =0;
		DeleteDC(hTempDC) ;
	}
};


