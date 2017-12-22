#pragma once
#include "Sys.h"
#include "PlayerPlane.h"
#include "Gunner.h"

class CFoePlane
{
public:
	CFoePlane(void);
	virtual ~CFoePlane(void);
public:
	HBITMAP m_hBmpFoePlane;
	int m_nBlood;
	int m_nShowID;
	int x;
	int y;
public:
	bool IsBoom()  // �ж��Ƿ�ը
	{
		if(m_nBlood == 0)
			return true;
		return false;
	}
	void DownBlood() //  ��Ѫ
	{
		m_nBlood--;
	}
public:
	virtual bool IsHitPlayerPlane(CPlayerPlane& plane)=0;
	virtual bool IsGunnerHitFoePlane(CGunner* pGunner)=0;
	virtual void FoePlaneInit(HINSTANCE hIns)=0;
	virtual void FoePlaneMove()=0;
	virtual void FoePlaneShow(HDC hMemDC)=0;
};

