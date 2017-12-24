#include "Sys.h"
#include <vector>
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

	void LoadRes(const char * path)
	{
		Picture temp_picture;
		temp_picture.bitmap = (HBITMAP)LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����λͼ
		if(!temp_picture.bitmap)return;
		GetObject(temp_picture.bitmap, sizeof(BITMAP), (LPBYTE)&temp_picture.bmp);//��ȡλͼ��Ϣ(�߶�����)
		vct_picture.push_back(temp_picture);	//�����������
	}

	// ������Ҫ����ͼƬ��Ϣ
	void Init(HDC hMemDC, char * picture_path,...)  //�ɱ������, �����ĸ������������
	{
		this->hMemDC = hMemDC;
		char ** str = &picture_path; //ȷ����һ�������ĵ�ַ
		while(!strncmp(*str,"res\\",4)) // �жϺ��滹�Ƿ��в���(���, Լ������Դ��·��������Ҫ�ڵ�ǰ�ļ����µ�res�ļ�������)
		{
			LoadRes(*str);
			str++;      // ����ָ�����, ȡ������ַ���
		}
	}
	void Init(HDC hMemDC, int num , char * picture_path, ...)
	{
		this->hMemDC = hMemDC;
		if(num > 9999) num = 9999; //��ʱ����������9999��, ���˲����
		int _num_ = 0;						//����string�±�����ַ�
		char ** str = &picture_path; //��ָ��һ��һ��ȡ�ɱ�������е��ַ���, 78���� str++
		while(!strncmp(*str,"res\\",4))//�жϴ�����Ƿ���Ч, ���, ͼƬ·������Ҫ�� res\ ��ͷ, ����·����Ϊ��Ч
		{
			if (strncmp(*(str+1),"res\\",4))//�ж��Ƿ�Ϊ���һ������
			{
				string s = *str;				//ʹ��string��ϵ�к������в���, ��char�첻��
				while (s[_num_]<'0' || s[_num_]>'9' ||  s[_num_+1] != '.')_num_ ++ ; // �ҵ���һ�γ�������, ������ " . " ǰ����ַ�λ��
				for (int i =s[_num_]-'0'; i < num + s[_num_]-'0'; i++)			//����ѭ��, ����ע�� i ���ж�Ϊ i < num + s[_num_]-'0'
				{
					string s2 = s;		// ��ʱstring,��Ϊ72���л�ı�s������
					int bit =1;				//����λ��, 72�����滻�ַ����õ�
					char str[10] ;		//itoa�õ�����ʱ�ռ�
					if (i>=10)bit = 2;	//�ı��ַ��滻��λ��
					if (i>=100)bit = 3;
					if (i>=1000)bit = 4;
					s2.replace(_num_,bit,itoa(i,str,10));	//string ���滻����
					LoadRes(s2.c_str());							//�����Ѿ�����õ��ַ������м�����Դ
				}
				return;															// forѭ������֮��, �Ѿ��������, �˳�����
			}
			LoadRes(*str);
			str++;      // ����ָ�����, ȡ��������е��ַ���
		}
	}
	//������Ҫ������ʾͼƬ
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


