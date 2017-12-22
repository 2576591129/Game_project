#include "Sys.h"
#include "CGameCtrl.h"
#include <time.h>

HINSTANCE hIns;

PFUN_CREATE_OBJECT CGameCtrl::pfnCreateObject = 0;

CGameCtrl* pCtrl = NULL;

//  窗口消息 处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) 
	{
	case WM_CREATE:  // 初始化程序
		if(CGameCtrl::pfnCreateObject == NULL)
		{
			::MessageBox(NULL,"游戏创建失败","提示",MB_OK);
			::DestroyWindow(hWnd);  //  销毁窗口;
			::PostQuitMessage(0);  //  退出
			return 0;
		}
		else
		{ 
			//  调用函数指针  创建子类的对象
			pCtrl = (*CGameCtrl::pfnCreateObject)();
			//  设置句柄
			pCtrl->SetHandle(hWnd,hIns);
			//  初始化游戏
			pCtrl->OnCreateGame();
		}
		break;
	case WM_PAINT:
		if(pCtrl != NULL)
		{
			pCtrl->OnGameDraw();
		}
		break;
	case WM_TIMER:
		if(pCtrl != NULL)
		{
			pCtrl->OnGameRun(wParam);
		}
		break;
	case WM_KEYDOWN:
		if(pCtrl != NULL)
		{
			pCtrl->OnKeyDown(wParam);
		}
		break;
	case WM_KEYUP:
		if(pCtrl != NULL)
		{
			pCtrl->OnKeyUp(wParam);
		}
		break;
	case WM_LBUTTONDOWN:
		if(pCtrl != NULL)
		{
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			pCtrl->OnLButtonDown(point);
		}
		break;
	case WM_LBUTTONUP:
		if(pCtrl != NULL)
		{
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			pCtrl->OnLButtonUp(point);
		}
		break;
	case WM_MOUSEMOVE:
		if(pCtrl != NULL)
		{
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			pCtrl->OnMouseMove(point);
		}
		break;
	case WM_CLOSE:   //  点X  窗口关闭的消息
		if (pCtrl)
		::PostQuitMessage(0);    //  发送退出的消息
		break;
	}
	return ::DefWindowProc( hWnd,  uMsg,  wParam,  lParam);
}



int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPreInstance, LPSTR pCmdLine,int nCmdShow)
{
	srand((unsigned int)time(0));

	hIns = hInstance;
	

	//  1.  设计
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;   //  是否要分配额外的空间
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hbrBackground = NULL;   //  背景颜色
	wndclass.hCursor = NULL; //  光标
	wndclass.hIcon = NULL;
	wndclass.hIconSm = NULL;  //  窗口左上小图标
	wndclass.hInstance = hInstance;    //  当前实例的句柄
	wndclass.lpfnWndProc = WndProc;    //  消息处理函数
	wndclass.lpszClassName = "lele";   //  注册窗口类的名
	wndclass.lpszMenuName = NULL;       //  菜单名
	wndclass.style = CS_HREDRAW|CS_VREDRAW;   //  窗口类的样式


	//  2.  注册
	if(::RegisterClassEx(&wndclass) == FALSE)
	{
		::MessageBox(NULL,"注册失败","提示",MB_OK);
		return 0;
	}


	//  3.  创建
	HWND hWnd = ::CreateWindow("lele","jucheng's game",WS_POPUP,200,100,WINDOW_WEIGHT/*+16*/,WINDOW_HIGNT/*+38*/,NULL,NULL,hInstance,NULL);
	if(hWnd == NULL)
	{
		::MessageBox(NULL,"创建失败","提示",MB_OK);
		return 0;
	}


	//  4.   显示
	::ShowWindow(hWnd,SW_SHOW);

	//  5.   消息循环
	MSG msg;
	while(::GetMessage(&msg,0,0,0))
	{
		//  翻译
		::TranslateMessage(&msg);
		//  分发
		::DispatchMessage(&msg);
	}

		return 0;
}