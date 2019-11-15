#pragma once

#include "CHexGridCell.h" 
#include <iostream>

#include <fstream>
struct hexParameters {
	int Col;
	int Row;
	float CellSZ;
	bool Ponty;
};


class CHexGrid
{
public:


	CHexGridCell **HEXA ;// cpp .... = new CHexGridCell[Colum * Fil]

	std::string **Obj_Po;

	unsigned int m_HexadVertexArrayObject;
	int m_numFacesHexa;
	int Col;
	int Row;
	bool Ponty;
	float CellSZ;
	void render();
	CHexGrid(hexParameters Datos);
	CHexGrid();
	~CHexGrid();
	void createHexGeometry(COpenGLRenderer * const ptrRenderer, unsigned int m_colorModelShaderId);

private:

	CVector3 Center;
	CVector3 Points;
	

	int m_renderPolygonMode;
	int X1;
	int Y1;
	int Z1;

	/*unsigned int m_HexadVertexArrayObject;*/
	// Current object rotation, expressed in degrees
	double m_objectRotation;

	// Current object position
	CVector3 m_objectPosition;

	//  Object rotation speed (degrees per second)
	double m_rotationSpeed;

};
