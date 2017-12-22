#pragma once
#include "foeplane.h"
class CSmallFoePlane :
	public CFoePlane
{
public:
	CSmallFoePlane(void);
	~CSmallFoePlane(void);
public:
	virtual bool IsHitPlayerPlane(CPlayerPlane& plane);
	virtual bool IsGunnerHitFoePlane(CGunner* pGunner);
	virtual void FoePlaneInit(HINSTANCE hIns);
	virtual void FoePlaneMove();
	virtual void FoePlaneShow(HDC hMemDC);
};

