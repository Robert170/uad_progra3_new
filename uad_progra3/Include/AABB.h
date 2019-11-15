#pragma once
#include "CVector3.h"
class AABB
{
public:
	AABB();
	~AABB();
	void Getcorners(CVector3*C);
	void Setcorners(CVector3*C);

private:
	
	CVector3 corners[4];

};

