#pragma once

#include "CQuadtreeNode.h"



class Cquadtree 
{
public:
	Cquadtree();
	~Cquadtree();
	void SubDivide(AABB_2D Bounds, int TriangulosMaximos, int SubDicionesMaximas, CHexGrid C, int Row, int Colum);
	int m_Limittris;
	CQuadtreeNode*m_root;
	int m_limitSubdiv;

private:
	
};

