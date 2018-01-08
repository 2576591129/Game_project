#include "MyGame.h"

LoadRes  loadres;
Building Game_building;
Player player;
CBack back;  //  背景
IMPLEMENT(MygameApp)
	MygameApp::MygameApp(void) 
{
	dc = 0;            //  获取 窗口DC
	hMemDC = 0;   //  创建一个 兼容性DC
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
		pthis->time_first = GetTickCount();
		if (pthis->time_first - pthis->time_second < 10 )continue;
		back.BackShow(pthis->hMemDC);
		/*pthis->bird.Show(pthis->hMemDC);
		pthis->snow.Show(pthis->hMemDC);
		pthis->star.Show(pthis->hMemDC);*/
		player.Show(pthis->hMemDC);

		::BitBlt(pthis->dc,0,0,WINDOW_WEIGHT,WINDOW_HIGNT,pthis->hMemDC,0,0,SRCCOPY);  // 把 兼容性DC 拷贝到窗口上
		pthis->time_second = GetTickCount();
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
	back.BackInit(hMemDC);
	//  2.  初始化其他
	SetDROP fall = {"res\\angrybird.bmp",20,3,10,-0.2,0,300,0,0.20,40,true};
	bird.Init(&fall);
	SetFALL fall_snow = {"res\\snow.bmp",1000,0,400,5,10,50,40,true};
	snow.Init(&fall_snow);
	SetStar set_star = {"res\\star.bmp",0,0,1000,450,1000,450,40,20,100,40,true};
	star.Init(&set_star);
	Game_building.AddBuilding(1081,260,120,70,false );

	ResumeThread(show_thread);

}

void MygameApp::OnGameDraw()     // WM_PAINT
{
	//------------------------------------------
	//back.BackShow(this->hMemDC);
	//this->bird.Show(this->hMemDC);
	//this->snow.Show(this->hMemDC);
	//this->star.Show(this->hMemDC);
	//player.Show(this->hMemDC);
	//::BitBlt(this->dc,0,0,WINDOW_WEIGHT,WINDOW_HIGNT,this->hMemDC,0,0,SRCCOPY);  // 把 兼容性DC 拷贝到窗口上
	//------------------------------------------

}
void MygameApp::OnGameRun(WPARAM nTimerID)     // WM_TIMER
{
	
}
void MygameApp::OnKeyDown(WPARAM nKey)
{
	if (nKey  == VK_ESCAPE)::PostQuitMessage(NULL);
	player.Action(nKey);
	
}

void MygameApp::OnKeyUp(WPARAM nKey)
{
	player.Restore(nKey);
	
	
}

