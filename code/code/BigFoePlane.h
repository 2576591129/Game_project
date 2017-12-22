#pragma once
#include "foeplane.h"
class CBigFoePlane :
	public CFoePlane
{
public:
	CBigFoePlane(void);
	~CBigFoePlane(void);
public:
	virtual bool IsHitPlayerPlane(CPlayerPlane& plane);
	virtual bool IsGunnerHitFoePlane(CGunner* pGunner);
	virtual void FoePlaneInit(HINSTANCE hIns);
	virtual void FoePlaneMove();
	virtual void FoePlaneShow(HDC hMemDC);
};

