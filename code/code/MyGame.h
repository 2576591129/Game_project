#pragma once
#include "cgamectrl.h"
#include "Back.h"
#include "Activity.h"
#include "Player.h"


class MygameApp :
	public CGameCtrl
{
public:
	HDC dc;            //  ��ȡ ����DC
	HDC hMemDC;   //  ����һ�� ������DC
	HBITMAP hBitamp;   //  ����λͼ   �� ����dc ���ݵ�ͼƬ���Ի���ɫ
public:
	CBack back;  //  ����


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

