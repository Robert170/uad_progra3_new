#include "../stdafx.h"

#include <iostream>
using namespace std;
#include "../Include/AABB_2D.h"

AABB_2D::AABB_2D()
{
}

AABB_2D::~AABB_2D()
{
}

AABB_2D::AABB_2D(CVector3 * D)
{
	corners[0] = *D;
	D++;
	corners[1] = *D;
	D++;
	corners[2] = *D;
	D++;
	corners[3] = *D;
}


CVector3 AABB_2D::Getcorners(int I)
{
	return corners[I];
}

void AABB_2D::Setcorners(CVector3 * C)
{
	for (int i = 0; i < 4; i++)
	{
		corners[i] = C[1];
	}
	
}

bool AABB_2D::Points(CVector3 * Datos)
{
	CVector3 P1;
	CVector3 P2;
	CVector3 P3;
	CVector3 P4;
	CVector3 P5;
	CVector3 P6;

	P1 = *Datos;
	Datos++;
	P2 = *Datos;
	Datos++;
	P3 = *Datos;
	Datos++;
	P4 = *Datos;
	Datos++;
	P5 = *Datos;
	Datos++;
	P6 = *Datos;

	if (P1.getX() >= corners[0].getX() && P1.getX() <= corners[1].getX() && P1.getZ() >= corners[0].getZ() && P1.getZ() <= corners[2].getZ())
	{
		return true;
	}
	if (P2.getX() >= corners[0].getX() && P2.getX() <= corners[1].getX() && P2.getZ() >= corners[0].getZ() && P2.getZ() <= corners[2].getZ())
	{
		return true;
	}
	if (P3.getX() >= corners[0].getX() && P3.getX() <= corners[1].getX() && P3.getZ() >= corners[0].getZ() && P3.getZ() <= corners[2].getZ())
	{
		return true;
	}
	if (P4.getX() >= corners[0].getX() && P4.getX() <= corners[1].getX() && P4.getZ() >= corners[0].getZ() && P4.getZ() <= corners[2].getZ())
	{
		return true;
	}
	if (P5.getX() >= corners[0].getX() && P5.getX() <= corners[1].getX() && P5.getZ() >= corners[0].getZ() && P5.getZ() <= corners[2].getZ())
	{
		return true;
	}
	if (P6.getX() >= corners[0].getX() && P6.getX() <= corners[1].getX() && P6.getZ() >= corners[0].getZ() && P6.getZ() <= corners[2].getZ())
	{
		return true;
	}
	

	return false;
}
