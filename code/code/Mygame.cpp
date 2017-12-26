#include "MyGame.h"

 LoadRes  loadres;
IMPLEMENT(MygameApp)
	MygameApp::MygameApp(void) 
{
	dc = 0;            //  ��ȡ ����DC
	hMemDC = 0;   //  ����һ�� ������DC
	hBitamp = 0; 
	time_first = 0;
	time_second =0;
	loadres.Init();

}

MygameApp::~MygameApp(void)
{
	::DeleteObject(hBitamp);
	::DeleteDC(hMemDC);
	::ReleaseDC(m_hMainWnd,dc);

}

unsigned __stdcall Show_thread( void * lp)
{
	MygameApp* pthis = (MygameApp*) lp;
	while (true)
	{
		pthis->back.BackShow(pthis->hMemDC);
		pthis->bird.Show(pthis->hMemDC);
		pthis->snow.Show(pthis->hMemDC);
		pthis->star.Show(pthis->hMemDC);
		pthis->player.Show(pthis->hMemDC);
		::BitBlt(pthis->dc,0,0,WINDOW_WEIGHT,WINDOW_HIGNT,pthis->hMemDC,0,0,SRCCOPY);  // �� ������DC ������������
	}
	return 0;
}
void MygameApp::OnCreateGame()   // WM_CREATE
{
	show_thread = (HANDLE)_beginthreadex(nullptr,0,Show_thread,this,CREATE_SUSPENDED,0);
	srand((unsigned)time(NULL));
	//  0.  ������Դ
	dc = ::GetDC(m_hMainWnd);            //  ��ȡ ����DC
	hMemDC = ::CreateCompatibleDC(dc);   //  ����һ�� ������DC
	hBitamp = ::CreateCompatibleBitmap(dc,WINDOW_WEIGHT,WINDOW_HIGNT);   //  ����λͼ   �� ����dc ���ݵ�ͼƬ���Ի���ɫ
	::SelectObject(hMemDC,hBitamp);
	//  1.  ��ʼ������
	back.BackInit(m_hIns);

	//  2.  ��ʼ������
	SetDROP fall = {"res\\angrybird.bmp",20,3,10,-0.2,0,300,0,0.20,40,true};
	bird.Init(&fall);
	SetFALL fall_snow = {"res\\snow.bmp",1000,0,400,5,10,50,40,true};
	snow.Init(&fall_snow);
	SetStar set_star = {"res\\star.bmp",0,0,1000,450,1000,450,40,20,100,40,true};
	star.Init(&set_star);
	//player.Init(hMemDC);
	//  3.  ������ʱ��
	::SetTimer(m_hMainWnd,BACK_MOVE_TIME_ID,50,NULL);   // ���Ʊ����ƶ� 
	ResumeThread(show_thread);

}

void MygameApp::OnGameDraw()     // WM_PAINT
{
	//------------------------------------------

		

	
	//------------------------------------------

}
void MygameApp::OnGameRun(WPARAM nTimerID)     // WM_TIMER
{
	if(nTimerID == BACK_MOVE_TIME_ID)
	{
		//if(::GetAsyncKeyState(VK_LEFT))   //  �ж� ���̵�״̬

		//if(::GetAsyncKeyState(VK_RIGHT))   //  �ж� ���̵�״̬
		//
		//if(::GetAsyncKeyState(VK_UP))   //  �ж� ���̵�״̬

		//if(::GetAsyncKeyState(VK_DOWN))   //  �ж� ���̵�״̬

	}

	if(nTimerID == BACK_MOVE_TIME_ID)  //  �����ƶ�
	{
		back.BackMove(player.x_pos);
	}
	//  �ػ�
	this->OnGameDraw();
}
void MygameApp::OnKeyDown(WPARAM nKey)
{
	if (nKey == VK_ESCAPE)
	{
		::PostQuitMessage(NULL);
	}
	if (nKey == 0x41)  //A��
	{
		back.state = true;
		player.x_pos -= PLAYER_SPEED;
		player.ChangeState("����");
	}
	if (nKey == 0x44)  //D��
	{
		if (player.x_pos <WINDOW_WEIGHT/2)player.x_pos +=PLAYER_SPEED;
		player.ChangeState("����");
		back.state = true;
	}
	this->OnGameDraw();
}

void MygameApp::OnKeyUp(WPARAM nKey)
{
	if (nKey == 0x44)  //D��
	{
		back.state = false;
		player.ChangeState("��վ��");
	}
	if (nKey == 0x41)  //A��
	{
		back.state = true;
		player.ChangeState("��վ��");
	}
	this->OnGameDraw();
}

