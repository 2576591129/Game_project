#include "MyGame.h"

IMPLEMENT(MygameApp)
	MygameApp::MygameApp(void) 
{
	dc = 0;            //  获取 窗口DC
	hMemDC = 0;   //  创建一个 兼容性DC
	hBitamp = 0; 
	SetDROP fall = {"res\\angrybird.bmp",20,3,10,-0.2,0,300,0,0.20,40,true};
	bird.Init(&fall);
	SetFALL fall_snow = {"res\\snow.bmp",50,1000,0,300,10,0,40,true};
	snow.Init(&fall_snow);
	SetStar set_star = {"res\\star.bmp",0,0,1000,450,1000,450,40,20,100,40,true};
	star.Init(&set_star);
	time_first = 0;
	time_second =0;
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
		::BitBlt(pthis->dc,0,0,WINDOW_WEIGHT,WINDOW_HIGNT,pthis->hMemDC,0,0,SRCCOPY);  // 把 兼容性DC 拷贝到窗口上
	}

	return 0;
}
void MygameApp::OnCreateGame()   // WM_CREATE
{
	show_thread = (HANDLE)_beginthreadex(nullptr,0,Show_thread,this,CREATE_SUSPENDED,0);
	srand((unsigned)time(NULL));
	//  0.  加载资源
	dc = ::GetDC(m_hMainWnd);            //  获取 窗口DC
	hMemDC = ::CreateCompatibleDC(dc);   //  创建一个 兼容性DC
	hBitamp = ::CreateCompatibleBitmap(dc,WINDOW_WEIGHT,WINDOW_HIGNT);   //  创建位图   和 窗口dc 兼容的图片可以画颜色
	::SelectObject(hMemDC,hBitamp);
	//  1.  初始化背景
	back.BackInit(m_hIns);
	player.Init(m_hIns);
	//  2.  初始化玩家飞机

	//  3.  启动定时器
	::SetTimer(m_hMainWnd,BACK_MOVE_TIME_ID,50,NULL);   // 控制背景移动


}



void MygameApp::OnGameDraw()     // WM_PAINT
{
	//------------------------------------------
	ResumeThread(show_thread);
	//------------------------------------------

}

void MygameApp::OnGameRun(WPARAM nTimerID)     // WM_TIMER
{
	if(nTimerID == BACK_MOVE_TIME_ID)
	{
		//if(::GetAsyncKeyState(VK_LEFT))   //  判断 键盘的状态

		//if(::GetAsyncKeyState(VK_RIGHT))   //  判断 键盘的状态
		//
		//if(::GetAsyncKeyState(VK_UP))   //  判断 键盘的状态

		//if(::GetAsyncKeyState(VK_DOWN))   //  判断 键盘的状态

	}

	if(nTimerID == BACK_MOVE_TIME_ID)  //  背景移动
	{
		back.BackMove();
	}
	//  重绘
	this->OnGameDraw();
}
void MygameApp::OnKeyDown(WPARAM nKey)
{
	if (nKey == VK_ESCAPE)
	{
		::PostQuitMessage(NULL);
	}
	if (nKey == 0x41)  //A键
	{
		player.state = 2;
		player.direct = 3;
		player.x_pos -=5;
	}
	if (nKey == 0x44)  //D键
	{
		player.state = 2;
		player.direct = 4;
		player.x_pos +=5;
	}
	//if(::GetAsyncKeyState(0x44)& 0x8000)   //  判断 键盘的状态
	//{
	//	time_first = GetTickCount();
	//	while (true)
	//	{

	//	}
	//	player.state = 2;
	//	player.direct = 4;
	//	player.x_pos ++;
	//}

	this->OnGameDraw();
}

void MygameApp::OnKeyUp(WPARAM nKey)
{
	if (nKey == 0x44)  //D键
	{
		player.state = 1;

	}
	if (nKey == 0x41)  //A键
	{
		player.state = 1;

	}
	this->OnGameDraw();
}

