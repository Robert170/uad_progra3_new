#pragma once
#include "CVector3.h"
class AABB_2D
{
public:
	AABB_2D();
	~AABB_2D();
	AABB_2D(CVector3 * D);

	CVector3 Getcorners(int I);
	void Setcorners(CVector3*C);
	bool Points(CVector3 * Datos);

	CVector3 corners[4];

private:
	
	unsigned int Vertex;

	float Vertices[12];
	float VerticesUV[4];
	unsigned short FaceIndex[6];
	float Normal[6];
	unsigned short FNIndex[6];

};

