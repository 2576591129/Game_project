#include "Sys.h"

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
public:

	int max_num;
	Picture arr[10];			//初期可以先用固定长度, 后期再动态分配空间 

	HDC hMemDC;			//两个DC用于贴图使用
	HDC hTempDC;

public:
	CycleShow()
	{
		memset(arr,0,10);
	}
	~CycleShow()
	{
		for (int i = 0; i < 10; i++)
		{
			DeleteObject(arr[i].bitmap);
		}
		DeleteDC(hMemDC);
	}

	// 这里需要加载图片信息
	void Init()
	{
		
	}

	//这里需要去显示图片
	void show()
	{



		DeleteDC(hTempDC) ;
	}

};


