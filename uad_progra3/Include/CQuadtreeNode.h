#pragma once
#include "Ccamera.h"
#include "AABB.h"
#include "CHexGrid.h"

class CQuadtreeNode
{
public:
	CQuadtreeNode();
	~CQuadtreeNode();
	void Subdivide();
	CHexGrid *Render(Ccamera*Cam);

private:
	AABB bounds;
	CHexGrid *cell;


};

