#include "../stdafx.h"

#include <iostream>
using namespace std;
#include "../Include/Cquadtree.h"

Cquadtree::Cquadtree()
{
}

Cquadtree::~Cquadtree()
{
}

void Cquadtree::SubDivide(AABB_2D Bounds, std::vector<CHexGridCell*>* Cell)
{
	if (m_root == nullptr)
	{
		
		m_root = new CQuadtreeNode();
		m_root->SetLimits(Bounds);
		m_root->SubDivide(m_Limit,Bounds, Cell);
	}
}
