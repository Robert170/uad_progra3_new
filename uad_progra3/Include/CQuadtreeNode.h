#pragma once
#include "Ccamera.h"
#include "AABB_2D.h"
#include "CHexGrid.h"
#include <vector>
#include "CHexGridCell.h"
#include "COpenGLRenderer.h"

class CQuadtreeNode
{
public:
	CQuadtreeNode();
	~CQuadtreeNode();
	void Subdivide();
	void Render(COpenGLRenderer*Render, unsigned int ShaderID);
	void LimitSetCorner();
	void LimitGetCorner();
	void SetBound(AABB_2D Datos);
	void FirstDivide(int Lim, AABB_2D BounParentBoundsds, CHexGrid C, int R, int C2, int SubActual, int MaxDiv);
	unsigned int VertexArrayObjetc;
	bool Carga(COpenGLRenderer*Render, unsigned int ShaderID);

private:
	AABB_2D bounds;
	CHexGrid *cell;
	CQuadtreeNode * h1;
	CQuadtreeNode * h2;
	CQuadtreeNode * h3;
	CQuadtreeNode * h4;
	std::vector<CHexGridCell*>m_Data;
	CHexGridCell ** D;

	float Vertices[12];
	unsigned short Indices[6];
	float Normales[6] = { 0 };
	unsigned short Indices_Normales[6] = { 0 };
	float UV[4] = { 0 };
	
	

public:
	
	void SubDivide(int Limit, AABB_2D ParentBounds, std::vector<CHexGridCell*>Cell, int SubActual, int MaxDiv);
	void SetLimits(AABB_2D Limits);

};

