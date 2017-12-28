#pragma once
#include "Sys.h"
#define PICTURE_RES_PATH "res\\picture\\"
#define SOUND_RES_PATH "res\\sound\\"

class  LoadRes;
extern LoadRes  loadres;
//������Ҫһ����һ��������Դ���� , ������������Դvector������, �ֱ��������ͼƬ �� ������Դ
// ��ϸ���ص�˼·��CycleShow���Init����������ͬ,�ڹ̶��ļ������Զ�����

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
		struct _finddata_t fileinfo;                          //�ļ���Ϣ�Ľṹ�� 
		long handle=_findfirst(picture_path.c_str(),&fileinfo);  //���ڲ��ҵľ��        //��һ�β��� 
		if(-1==handle)return ; 
		Save(fileinfo.name);     //������Ϣ
		while(!_findnext(handle,&fileinfo))Save(fileinfo.name);     //ѭ�������������ϵ��ļ���֪���Ҳ���������Ϊֹ 
		_findclose(handle);    
	}
	void Save(string s) // s �е������� res\picture\�ļ���������λͼ������, ����: ��վ��1.bmp
	{
		string str_path = PICTURE_RES_PATH + s;			//�õ��ļ���·����ȫ�� , ����λͼ��
		Pct_res pict;
		pict.bitmap = (HBITMAP)LoadImage(NULL,str_path.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����λͼ
		if(!pict.bitmap)return; //�������ʧ��, �򷵻�
		BITMAP bmp;				//���λͼ��Ϣ
		GetObject(pict.bitmap, sizeof(BITMAP), (LPBYTE)&bmp);//��ȡλͼ��Ϣ(�߶�����)
		pict.height = bmp.bmHeight;
		pict.width = bmp.bmWidth;
		int _num_ = 0;
		while (s[_num_]<'0' || s[_num_]>'9' )_num_ ++; // ��ȡ���һ���������ֵ�λ���±�
		s = s.substr(0,_num_);										// �ػ�ָ�����ȵ��ַ�, ���� : "��վ��1.bmp"  �����õ� "��վ��" ��Ϊmap�ļ�ֵ
		map_picture[s].push_back(pict);					// �Ѹü�ֵ��Ӧ��λͼ��Ϣ pict ��ӵ���Ӧ��������   �Ժ���ʾλͼ��ʱ��, ֱ��ָ����ֵ: ��վ��, ��ȡ��Ӧ��vector, ѭ����ʾ�ͺ�
	}
};

class CycleShow
{
	int show_id;				// �������Ƶڼ���ͼƬ��ʾ�õ�, ��ȫ��map�Ķ���loadres��, ��ͬ�ļ�ֵ�в�ͬ������λͼ, ��Ҫ��show_id���п���
	int time_first , time_second;// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	int x,y;						//������¼�����ƶ�ʱ������
public:
	CycleShow()
	{
		x = 0 ;
		y = 0;
		show_id = 0;
		time_first = 0;
		time_second = 0;		
	}
	~CycleShow(){}

	void show(HDC hMemDC,string key_value,int x_pos, int y_pos,int direct, int draw_time,bool flag_cycle ,bool auto_move = false ,int x_speed = 0, int y_speed = 0)
	{
		time_first = GetTickCount();	//��¼�ػ��ʱ��
		if ( x == 0 && y == 0 && auto_move == true)   // ��Ҫ�Զ��ƶ�����
		{
			x = x_pos; 
			y = y_pos;
		}
		if (auto_move == false) // ����Ҫ�Զ��ƶ�  (��Ȼ������if�ж���������ʱһ����, ���Ǿ��Բ��ܷ�һ��, ��Ϊѭ����������һ��
		{
			x = x_pos; 
			y = y_pos;
		}
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if(loadres.map_picture[key_value].size())//�����ص�λͼΪ��ʱ, ��������ͼ, ��ֹ���ʿ�ָ��
		{
			if ((unsigned)show_id>=loadres.map_picture[key_value].size())show_id =0;	//�������ж�, ����, key_value�ı�ʱ��, ��Ӧ��show_idû�л�ԭ, ���п��ܻ��Խ��
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,x_pos + (direct-3)*loadres.map_picture[key_value][show_id].width/2,y_pos,(-direct+4)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id++].height,SRCAND);
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,x_pos + (direct-3)*loadres.map_picture[key_value][show_id].width/2,y_pos,(-direct+4)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id--].height,SRCPAINT);
			if (time_first - time_second > draw_time) //�ж��Ƿ�ﵽʱ��
			{
				show_id +=2 ;
				x+=x_speed;
				y+=y_speed;
				time_second = GetTickCount();//��¼�ػ��ʱ��
			}
		}
		if (show_id > loadres.map_picture[key_value].size()-1)   //ѭ�����ڵ���, show_id��0�ٿ�ʼ
		{
			if (flag_cycle )show_id =0;
			else show_id -=2;
		}	
		DeleteDC(hTempDC);
	}
};






