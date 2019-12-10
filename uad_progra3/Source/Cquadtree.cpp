#include "../stdafx.h"

#include <iostream>
using namespace std;
#include "../Include/Cquadtree.h"

Cquadtree::Cquadtree()
{
	m_root = nullptr;
}

Cquadtree::~Cquadtree()
{
}

void Cquadtree::SubDivide(AABB_2D Bounds, int TriangulosMaximos, int SubDicionesMaximas, CHexGrid C, int Row, int Colum)
{
	if (m_root == nullptr)
	{

		m_root = new CQuadtreeNode();
		m_root->SetLimits(Bounds);
		m_limitSubdiv = SubDicionesMaximas;
		m_Limittris = TriangulosMaximos;
		m_root->FirstDivide(m_Limittris, Bounds, C, Row, Colum, 0, m_limitSubdiv);
	}
}

