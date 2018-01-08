#pragma once
#include "Sys.h"
#define PICTURE_RES_PATH "res\\picture\\"
#define SOUND_RES_PATH "res\\sound\\"

class  LoadRes;
extern LoadRes  loadres;


//����ͼƬ��Դ��
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

//����ѭ����ʾ��
class CycleShow
{
	int show_id;				// �������Ƶڼ���ͼƬ��ʾ�õ�, ��ȫ��map�Ķ���loadres��, ��ͬ�ļ�ֵ�в�ͬ������λͼ, ��Ҫ��show_id���п���
	int time_first , time_second;// ��¼�ػ��ʱ�� , �����Ͳ���ʹ�ö�ʱ��
	int x,y;						//������¼�����ƶ�ʱ������
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
		time_first = GetTickCount();	//��¼�ػ��ʱ��
		if ( pos.x == 0 && pos.y == 0 && auto_move == true)   // ��Ҫ�Զ��ƶ�����
		{
			pos.x = x_pos; 
			pos.y = y_pos;
		}
		if (auto_move == false) // ����Ҫ�Զ��ƶ�  (��Ȼ������if�ж���������ʱһ����, ���Ǿ��Բ��ܷ�һ��, ��Ϊѭ����������һ��
		{
			pos.x = x_pos; 
			pos.y = y_pos;
		}
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if(loadres.map_picture[key_value].size())//�����ص�λͼΪ��ʱ, ��������ͼ, ��ֹ���ʿ�ָ��
		{
			if ((unsigned)show_id>=loadres.map_picture[key_value].size())show_id =0;	//�������ж�, ����, key_value�ı�ʱ��, ��Ӧ��show_idû�л�ԭ, ���п��ܻ��Խ��
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,pos.x + (direct+1)*loadres.map_picture[key_value][show_id].width/2,pos.y,(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id++].height,SRCAND);
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,pos.x + (direct+1)*loadres.map_picture[key_value][show_id].width/2,pos.y,(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id--].height,SRCPAINT);
			if (time_first - time_second > draw_time) //�ж��Ƿ�ﵽʱ��
			{
				show_id +=2 ;
				pos.x+=x_speed;
				pos.y+=y_speed;
				time_second = GetTickCount();//��¼�ػ��ʱ��
			}
		}
		if (show_id > loadres.map_picture[key_value].size()-1)   //ѭ�����ڵ���, show_id��0�ٿ�ʼ
		{
			if (flag_cycle )show_id =0;
			else show_id -=2;
		}	
		DeleteDC(hTempDC);
		return pos;
	}


	void show(HDC hMemDC,string key_value,float *x_pos, float *y_pos,int direct, int draw_time,bool flag_cycle,float x_speed = 0, float y_speed = 0 , float *extend_num = NULL)
	{
		time_first = GetTickCount();	//��¼�ػ��ʱ��
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		if(loadres.map_picture[key_value].size())//�����ص�λͼΪ��ʱ, ��������ͼ, ��ֹ���ʿ�ָ��
		{
			if ((unsigned)show_id>=loadres.map_picture[key_value].size())show_id =0;	//�������ж�, ����, key_value�ı�ʱ��, ��Ӧ��show_idû�л�ԭ, ���п��ܻ��Խ��
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,((int)*x_pos) + (direct+1)*loadres.map_picture[key_value][show_id].width/2,((int)*y_pos),(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id++].height,SRCAND);
			SelectObject(hTempDC,loadres.map_picture[key_value][show_id].bitmap);
			StretchBlt(hMemDC,((int)*x_pos) + (direct+1)*loadres.map_picture[key_value][show_id].width/2,((int)*y_pos),(-direct)*loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id].height,hTempDC,0,0,loadres.map_picture[key_value][show_id].width,loadres.map_picture[key_value][show_id--].height,SRCPAINT);
			if (time_first - time_second > draw_time) //�ж��Ƿ�ﵽʱ��
			{
				show_id +=2 ;
				(*x_pos)+=x_speed;
				(*y_pos)+=y_speed;
				if (extend_num)(*extend_num) += x_speed;				
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

//�����ϰ�����
class BackObstacle	
{
private:
	BITMAPINFOHEADER bih;   //λͼ����Ϣͷ
	BYTE *Buffer ;						//���ڶ�ȡλͼ��Ϣ�Ŀռ�
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


	//���µĺ�����������ʵ����, �������о���һ�� 
	//����Ĳ���һ���ǵ�ǰ���� , һ�����յ�����(����ȷ��һ��ֱ��), ���һ���Ƿ���ֵ����   0: ��ǰ��������ϰ���ľ���, 1: �ϰ����x����, 2: �ϰ���� y���� , ������ֵΪ-999 ��û���ϰ�����߲�����������
	int GetObstaclePosition(float firX_pos, float firY_pos, float secX_pos, float secY_pos , int return_type = 0) 
	{	
		if ( firX_pos<0 || firY_pos <0 ||  secX_pos <0 || secY_pos <0 || firY_pos > WINDOW_HIGNT || secY_pos > WINDOW_HIGNT ) return -999;	//������������
	
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
		float K= (secY_pos - firY_pos)/(secX_pos - firX_pos);	//����������б�� K
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
			if (GetColor((int)x , (int ) y ))//���ϰ����򷵻ص�ǰ�����봫������֮��ľ���
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
		return -999;	//û���ϰ����򷵻�-1
	}

};




