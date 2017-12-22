#include <windows.h>
#include <iostream>
#include <time.h>
#pragma comment (lib,"Msimg32.lib")
//������ʱ���ָ���ַ�ռ��һ��������, WcharToChar �� CharToWchar ���õ���,  �����Ļ���������ֱ�ӷ����ַ�����, ���ᵼ���ڴ�й©������
class BUFFER
{
public:
	BUFFER(){pdelete = NULL;}
	~BUFFER(){ Destory(nullptr);}

	void Destory(void * temp)
	{
		if (!pdelete)
			delete(pdelete);
		pdelete = temp;
	}
private:
	void * pdelete;
};
//����һ��ȫ�ֵ���ʱbuff
BUFFER Tempbuff;
char *  WcharToChar(WCHAR* widestr)
{
	int num = WideCharToMultiByte(CP_OEMCP,NULL,widestr,-1,NULL,0,NULL,FALSE);
	char *pchar = new char[num];
	Tempbuff.Destory(pchar);
	WideCharToMultiByte (CP_OEMCP,NULL,widestr,-1,pchar,num,NULL,FALSE);
	return pchar;
}
LPCWSTR CharToWchar(char* ch)
{
	int num = MultiByteToWideChar(0,0,ch,-1,NULL,0);
	wchar_t *wide = new wchar_t[num];
	Tempbuff.Destory(wide);
	MultiByteToWideChar(0,0,ch,-1,wide,num);
	return wide;
}

//***********************************************��������(ģ�������Լ�Ħ����)����**************************************//
struct SetDROP  //��һ���ṹ��, �������ö����ĸ��ֲ���, ����ṹ�������û��Ľӿ�
{
	TCHAR * picture_path;							//ͼƬ��·��
	float x_speed;											// X����ĳ��ٶ�
	float y_speed;											// Y����ĳ��ٶ�
	float G;														// �������ٶ�
	float  x_F;													// X�����Ħ����
	float  y_F;													// Y�����Ħ����
	int high;														//����ĸ߶�
	float x_begin_pos;									//�������ʼ��x
	float y_begin_pos;									//�������ʼ��y
	int drow_speed;										//�ػ���ٶ�(����Ϊ��λ)
	bool repet;													//�Ƿ��ظ�
};
class DROP
{
public:
	BITMAP bmp;												//���λͼ����Ϣ, ������Ҫ������ȡλͼ�ĸ߶ȺͿ��
	HBITMAP bit_bird;									//���λͼ
	SetDROP fall;												//�������������Ϣ�Ľṹ��
	float x_position_bird;								//����ĵ�ǰλ��
	float y_position_bird;								//����ĵ�ǰλ��
	bool sign;													//���ڱ�ʶ�����Ƿ�ѭ��
	float x_s;													//��������ĳ�ʼx�ٶ�
	float y_s;													//��������ĳ�ʼy�ٶ�				
	int time_first;
	int time_second;
public:
	DROP()
	{
		time_first = 0;
		time_second = 0;
		sign = true;
		x_position_bird = 0;
		y_position_bird = 0;
	}
	~DROP()
	{
		DeleteObject(bit_bird);
	}

	void Init(SetDROP* falled )
	{
		fall = * falled;							// ��ź�Ҫ���õĲ���														
		x_s = fall.x_speed;				//��������ĳ�ʼ�ٶ�, ��Ϊ��ֵ�ᱻ�޸�(������ٶȻ�Խ��Խ��,��������Ӱ��)
		y_s = fall.y_speed;
		bit_bird =(HBITMAP)LoadImage(NULL,fall.picture_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE); //����λͼ
		GetObject(bit_bird, sizeof(BITMAP), (LPBYTE)&bmp);//��ȡλͼ��Ϣ(�߶�����)
	}

	void Show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		::SelectObject(hTempDC,bit_bird);
		//ʹ�úڰ���ͼ
		BitBlt(hMemDC,(int)x_position_bird,(int)y_position_bird,bmp.bmWidth/2,bmp.bmHeight,hTempDC,bmp.bmWidth/2,0,SRCAND);
		BitBlt(hMemDC,(int)x_position_bird,(int)y_position_bird,bmp.bmWidth/2,bmp.bmHeight,hTempDC,0,0,SRCPAINT);	

		time_first = GetTickCount();
		// ********************************************* С�����������Ч��������Ч��*************************************************
		if (sign && time_first - time_second >= fall.drow_speed) // ���ƶ����Ƿ�ѭ���� , ���Ҷ������ƶ������ػ���ٶ�
		{
			if (y_position_bird >= fall.high) //���䵽ָ���ĸ߶�ʱ, �ٶȷ���
			{
				y_position_bird =(float) fall.high;
				fall.y_speed = -fall.y_speed;
			}
			fall.y_speed =fall.y_speed +fall.G + fall.y_F;		//�ٶȻ��յ�������������Ӱ��, ����ʹ�õ����ٶȹ�ʽ
			if (fall.x_speed + fall.x_F>=0)
			{
				fall.x_speed += fall.x_F;										//���x�����ϵ��ٶȲ�С��0, ���յ�������Ӱ��, ��Ȼ��ı�
			}		
			y_position_bird += fall.y_speed;								//��������ķ���
			x_position_bird +=fall.x_speed;


			//������������߼��ж�, ������ȷ�������Ƿ��Ѿ���ȫ�˶����, �õ��Ǿ���ֵ���ж�
			if (y_position_bird >= fall.high && fall.x_speed+fall.x_F <=0 && fall.y_speed >=-fall.G && fall.y_speed <= fall.G )
			{			
				if (fall.repet == true)		//����ظ�����, �������λ������, ���ҳ�ʼ�ٶ�����
				{
					fall.y_speed = y_s;
					fall.x_speed = x_s;
					x_position_bird = fall.x_begin_pos;
					y_position_bird = fall.y_begin_pos;
				}
				else
					sign = false;					//�����ѭ������, �ı�sign��ֵ, �������ж���Ч��, �����������ʾ��Ȼ����
			}
			time_second = GetTickCount();
		}
		::DeleteDC(hTempDC);
	}
};

//****************************************************************************************************************//


//******************************************************ѩ������*********************************************//
struct SetFALL
{
	TCHAR* picture_path;							//ͼƬ��·��
	int x_rand;												//��Ҫ����ķ�Χ
	float y_begin_pos;								//��ʼ�����y������
	float high;												//����ĸ߶�
	float speed;											//������ٶ�
	int swing	;											//����ҡ�ڵķ�Χ
	int max_num;										//��������
	int drow_speed;									//�ػ���ٶ�(����Ϊ��λ)
	bool repet;												//�Ƿ��ظ�
};
class SNOWFALL
{
private:
	//����ṹ���������ڲ�ʹ�õ�, �û��޷���ȡ��, ���ڱ�������������Լ��Ƿ����
	struct Snow
	{
		float x_pos;
		float y_pos;
		bool exists;
	};
public:
	SNOWFALL()
	{
		snow_num = 0 ;
		arr_Snow = NULL;
		sign = true;
		time_second = 0;
		time_first = 0;
	}
	~SNOWFALL()
	{
		delete arr_Snow;
		arr_Snow = NULL;
		DeleteObject(bit_snow_flower);
	}

	void Init(SetFALL * falled)
	{
		fall = *falled;			//���涯������
		arr_Snow = new Snow[fall.max_num]; // ʹ�ö�̬����ռ�ķ�ʽ,�������������
		bit_snow_flower = (HBITMAP)LoadImage(NULL,fall.picture_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����λͼ
		GetObject(bit_snow_flower, sizeof(BITMAP), (LPBYTE)&bmp);//��ȡλͼ��Ϣ(�߶�����)
	}

	void Show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		for (int i = 0; i < fall.max_num; i++)
		{
			if (arr_Snow[i].exists)//��������������ͼ
			{
				::SelectObject(hTempDC,bit_snow_flower);
				TransparentBlt(hMemDC,(int)arr_Snow[i].x_pos,(int)arr_Snow[i].y_pos,bmp.bmWidth,bmp.bmHeight,hTempDC,0,0,bmp.bmWidth,bmp.bmHeight,RGB(0,0,0));
			}
		}

		time_first = GetTickCount();	
		if (sign && time_first - time_second >= fall.drow_speed)//���ƶ���ѵѭ���ı�־
		{
			if (snow_num < fall.max_num)//������廹û�дﵽ�趨�����ֵ, ��������
			{
				arr_Snow[snow_num].x_pos =(float)(rand()% (fall.x_rand));		//����ĳ�ʼX�������趨�ķ�Χ�����
				arr_Snow[snow_num].y_pos = fall.y_begin_pos;				// ���忪ʼ����ĳ�ʼYֵ
				arr_Snow[snow_num].exists = true;									
				snow_num ++ ;
			}
			for (int i = 0; i < fall.max_num; i++)
			{
				//����趨������ҡ�ڵ�ֵ��Ϊ0, �����趨��ֵ֮�����һ����, ��������ҡ��(�������Ҳ�����)
				if(rand()%2==0 && fall.swing!=0)
					arr_Snow[i].x_pos += rand()%fall.swing;  //x�������0~5֮���һ�����ֵ
				else if(rand()%2==1 && fall.swing!=0)
					arr_Snow[i].x_pos-=rand()%fall.swing;	 //y�������0~5֮���һ�����ֵ
				arr_Snow[i].y_pos +=fall.speed;
				if (arr_Snow[i].y_pos >fall.high)  
				{
					if (fall.repet == true) // ����½������߶Ȳ��Ҷ���Ч���ظ� , �������yֵ��Ϊ�趨��y_begin_pos, x�����趨�ķ�Χ�����һ����
					{
						arr_Snow[i].y_pos = fall.y_begin_pos;
						arr_Snow[i].x_pos = (float) (rand()% (fall.x_rand));
					}
					else if(fall.repet == false && snow_num>=fall.max_num) //������������,��������ﵽ���ֵ, �򶯻���ʾ��� , ����Ҳ��������
						sign = false;
				}

			}
			time_second = GetTickCount();
		}

		::DeleteDC(hTempDC);
	}

private:
	SetFALL  fall;
	int snow_num;											//��ǰ���������
	HBITMAP bit_snow_flower;		
	BITMAP bmp;
	Snow* arr_Snow;									//ѩ��������, ���Ϊ50
	bool sign;													//���ƶ����ظ�
	int time_first;
	int time_second;
};
//****************************************************************************************************************//


//****************************************************�ǹ����Ŷ���*******************************************//
struct SetStar
{
	TCHAR* picture_path;							//ͼƬ��·��
	float x_begin;										//��ʼ���ŵ�λ��,���Ϊ0,������x_range��y_range�����
	float y_begin;
	int x_range;											//��ʼ���ŵķ�Χ
	int y_range;
	int x_max_range;									//�����ķ�Χ
	int y_max_range;
	int max_speed;										//������е�����ٶ�
	int exist_time;										//������ڵ�ʱ��
	int max_num;										//������������
	int drow_time;										//�ػ�һ�ε�ʱ��(����Ϊ��λ)
	bool repet;												//�Ƿ��ظ�
};
class STAR		
{
	struct Star
	{
		float x_pos;
		float y_pos;
		int x_speed;
		int y_speed;
		int exist_time;
		bool exist;
	};

public:
	BITMAP bmp;
	HBITMAP bit_star;									//���ǵ�λͼ
	Star *arr_star;										//���ǵ�����, ���Ϊ50
	int stat_num;												//��ǰ���ǵ�����
	SetStar set_star;
	bool sign;
	int time_first;
	int time_second;
public:
	STAR()
	{
		stat_num = 0;
		sign = true;
		time_second = 0;
		time_first = 0;
	}
	~STAR()
	{
		if (arr_star != NULL)
		{
			delete arr_star;
			arr_star = NULL;
		}
		DeleteObject(bit_star);
	}

	void Init(SetStar * temp_star)
	{
		set_star = *temp_star;
		arr_star = new Star[set_star.max_num];
		bit_star = (HBITMAP)LoadImage(NULL,set_star.picture_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����λͼ
		GetObject(bit_star, sizeof(BITMAP), (LPBYTE)&bmp);//��ȡλͼ��Ϣ(�߶�����)
	}

	void Show(HDC hMemDC)
	{
		HDC hTempDC = ::CreateCompatibleDC(hMemDC);
		
		if (sign == true)   //�����Ƿ�ѭ��
		{
			if (stat_num == 0)	// ������ȵ�����Ϊ0 , һ���ԾͰ���Ҫ������ȫ��������
			{
				int x_bom ,y_bom;	//���ǳ�ʼ��ը��λ��
				if (set_star.x_begin ==0&& set_star.y_begin ==0)  //������õĳ�ʼ��ը��λ��Ϊ0 , ����� x_range��y_range�������ը��
				{
					x_bom = rand() % (set_star.x_range);
					y_bom =  rand() % (set_star.y_range);
				}
				else
				{
					x_bom =(int) set_star.x_begin;
					y_bom = (int)set_star.y_begin;
				}
				for (int i = 0; i < set_star.max_num; i++)		//�����ǵĽṹ�����ݸ�ֵ
				{
					arr_star[i].x_pos =(float) x_bom;
					arr_star[i].y_pos = (float)y_bom;
					arr_star[i].exist_time = 0;
					switch ( i%4)													//���ξ������Ƿ��е�����
					{
					case 0:
						arr_star[i].x_speed = 1+rand() % set_star.max_speed;
						arr_star[i].y_speed = 1+rand() % set_star.max_speed;
						break;
					case 1:
						arr_star[i].x_speed = 1 + rand() % set_star.max_speed;
						arr_star[i].y_speed= -(1+ rand() % set_star.max_speed);
						break;
					case 2:
						arr_star[i].x_speed= - (1+rand() % set_star.max_speed);
						arr_star[i].y_speed =-(1+ rand() % set_star.max_speed);
						break;
					case 3:
						arr_star[i].x_speed = -(1+rand() % set_star.max_speed);
						arr_star[i].y_speed = 1+rand() % set_star.max_speed;
						break;
					}
					arr_star[i].exist =true;
				}
				stat_num = set_star.max_num;			
			}

			for(int i=0;i<set_star.max_num;i++) //�ж������Ƿ񻹴���,�����ڣ�����������꣨arr_star[i].x_pos,arr_star[i].y_pos��������ͼ����
			{
				if(arr_star[i].exist)  
				{
					SelectObject(hTempDC,bit_star);
					TransparentBlt(hMemDC,(int)arr_star[i].x_pos,(int)arr_star[i].y_pos,bmp.bmWidth,bmp.bmHeight,hTempDC,0,0,bmp.bmWidth,bmp.bmHeight,RGB(0,0,0));
				}
			}

			time_first = GetTickCount();	
			if (time_first - time_second >=set_star.drow_time)
			{
				for(int i=0;i<set_star.max_num;i++)
				{
					if(arr_star[i].exist )   //�ж������Ƿ񻹴���,�����ڣ�����������꣨FlyStars[i].x,FlyStars[i].y��������ͼ����
					{
						//������һ����ͼ������
						arr_star[i].x_pos+=arr_star[i].x_speed;
						arr_star[i].y_pos+=arr_star[i].y_speed;

						//��ÿ����һ����ͼ�󣬽����ӵĴ���ʱ���ۼ�1.
						arr_star[i].exist_time++;
						//���������жϣ���ĳ�����ܳ���������һ���ķ�Χ���򽫸�������Ϊ�����ڣ�����������֮�ݼ�
						if(arr_star[i].x_pos<=-10 || arr_star[i].x_pos>set_star.x_max_range || arr_star[i].y_pos<=-10 || arr_star[i].y_pos>set_star.x_max_range || arr_star[i].exist_time>set_star.exist_time)
						{
							arr_star[i].exist = false;  //ɾ���ǹ����� 
							stat_num--;                    //�ݼ��ǹ�����
							if (stat_num == 0 && set_star.repet == false) sign = false;				
						}
					}
				}
				time_second = GetTickCount();
			}
		}
		DeleteDC(hTempDC);
	}
};