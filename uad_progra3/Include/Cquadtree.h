#pragma once

#include "CQuadtreeNode.h"
#include "Globals.h"
#include "CApp.h"

class Cquadtree : public CApp
{
public:
	Cquadtree();
	~Cquadtree();

private:
	int m_limitSubdiv;
	CQuadtreeNode * m_root;

};

