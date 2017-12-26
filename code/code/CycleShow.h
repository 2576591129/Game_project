#include "Sys.h"
#include <vector>
#include <string>
#include<io.h> 
#include <map>
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
	int show_id;
	int time_first;
	int time_second;		// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
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
		time_first = GetTickCount();	//��¼�ػ��ʱ��
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if(loadres.map_picture[key_value].size())//�����ص�λͼΪ��ʱ, ��������ͼ, ��ֹ���ʿ�ָ��
		{
			if (show_id>=loadres.map_picture[key_value].size())show_id =0;	//�������ж�, ����, key_value�ı�ʱ��, ��Ӧ��show_idû�л�ԭ, �����Խ��
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			TransparentBlt(hMemDC,x_pos,y_pos,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,RGB(0,0,0));
			if (time_first - time_second > draw_time)
			{
				show_id++;
				time_second = GetTickCount();//��¼�ػ��ʱ��
			}
		}
		if (show_id > loadres.map_picture[key_value].size()-1)show_id =0;	
		DeleteDC(hTempDC);
	}
};






