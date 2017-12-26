#pragma once
#include "cgamectrl.h"
#include "Back.h"
#include "Activity.h"
#include "Player.h"


class MygameApp :
	public CGameCtrl
{
public:
	HDC dc;            //  获取 窗口DC
	HDC hMemDC;   //  创建一个 兼容性DC
	HBITMAP hBitamp;   //  创建位图   和 窗口dc 兼容的图片可以画颜色
public:
	CBack back;  //  背景


public:
	DECLARE()
public:
	MygameApp(void);
	~MygameApp(void);
public:
	virtual void OnCreateGame();   // WM_CREATE
	virtual void OnGameDraw();     // WM_PAINT
	virtual void OnGameRun(WPARAM nTimerID);      // WM_TIMER
	virtual void OnKeyDown(WPARAM nKey);
	virtual void OnKeyUp(WPARAM nKey);
	void Show();

public:
	HANDLE show_thread;

	DROP bird;
	SNOWFALL snow;
	STAR star;
	int time_first;
	int time_second;

	Player player;
	
};

