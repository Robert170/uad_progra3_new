#pragma once

#include "CQuadtreeNode.h"



class Cquadtree 
{
public:
	Cquadtree();
	~Cquadtree();
	void SubDivide(AABB_2D Bounds,std::vector<CHexGridCell*>*Cell);
	int m_Limit;

private:
	CQuadtreeNode*m_root;
	int m_limitSubdiv;
};

