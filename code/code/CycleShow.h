

#include "Sys.h"
#include <vector>
// ����Ч����Ҫ��ͣ��ѭ����ͼ, ��ͼƬ�Ĵ�С���ܱ�֤��ȫһ���Ĵ�С, �ڽ�����ͼ��ʱ����鷳
// ����ȥдһ����, ʵ�ֵĹ�����: 
// �û���ͼƬ��·����Ϊ��������ȥ(��������), �����Զ���ȡͼƬ�Ĵ�С, �����ݴ��ε�˳�����ν�����ͼ


// Ŀǰʵ�ֵ�˼·:
// ��������һ����, �����Ҫ�������, ���ýӿں�������ͼƬ
// ������һ���ṹ��, �ṹ������ݴ���Ϊ: HBIEMAP ���͵ı���, BITMAP ���͵ı���, ͼƬ�Ĵ�С, ������ʾͼƬ��XY����
// ������һ������, �ܸ����û��������ĸ�������, ����new��ͬ�Ŀռ��С
// ������һ������Show�ĺ���, ����ѭ����ʾ����ͼƬ
// �����н��пռ��Ѿ���Դ���ͷ�


class CycleShow
{
	//һ���Զ���Ľṹ��, ����ͼƬ����Ϣ
	struct Picture
	{
		HBITMAP bitmap;
		BITMAP	bmp;
	};

	int show_id;
	int time_first;
	int time_second;		// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	/*list<Picture> list_pacture; */ // ������������Ȼ���� , ������ò�Ҫ������..... ��Ϊ������, �������ȽϿ�
	vector<Picture> vct_picture;
	HDC hMemDC;			//��DC������ͼʹ��
	

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

	// ������Ҫ����ͼƬ��Ϣ
	void Init(HDC hMemDC, char * picture_path,...)  //�ɱ������, �����ĸ������������
	{
		this->hMemDC = hMemDC;
		char ** str = &picture_path; //ȷ����һ�������ĵ�ַ
		while(!strncmp(*str,"res\\",4)) // �жϺ��滹�Ƿ��в���(���, Լ������Դ��·��������Ҫ�ڵ�ǰ�ļ����µ�res�ļ�������)
		{
			Picture temp_picture;
			temp_picture.bitmap = (HBITMAP)LoadImage(NULL,*str,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����λͼ
			GetObject(temp_picture.bitmap, sizeof(BITMAP), (LPBYTE)&temp_picture.bmp);//��ȡλͼ��Ϣ(�߶�����)
			vct_picture.push_back(temp_picture);	//�����������
			str++;      // ����ָ�����, ȡ������ַ���
		}
	}

	//������Ҫ������ʾͼƬ
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


