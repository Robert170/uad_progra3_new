#include "../stdafx.h"

#include <iostream>
using namespace std;

#include <assert.h>
#include "../Include/Globals.h"
#include "..\Include\CHexGridCell.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"


CHexGridCell::CHexGridCell() 
	//CHexGridCell(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

CHexGridCell::CHexGridCell(int window_width, int window_height)
	/*CApp(window_width, window_height),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ 0.0f, 0.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED },
	m_HexadVertexArrayObject{ 0 },
	m_renderPolygonMode{ 0 }*/
{
	cout << "Constructor: CappHexGrid(int window_width, int window_height)" << endl;
}

CHexGridCell::~CHexGridCell()
{
	//cout << "Destructor: ~CAppGeometricFigures()" << endl;

	//// Free memory allocated in this class instance here
	//// =================================================
	////
	//if (m_textureID > 0)
	//{
	//	getOpenGLRenderer()->deleteTexture(&m_textureID);
	//}

	//if (m_HexadVertexArrayObject > 0)
	//{
	//	getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_HexadVertexArrayObject);
	//}
	//// =================================================
}

void CHexGridCell::P(CVector3 Pos)
{
	Position = Pos;
}

CVector3 CHexGridCell::getCorner(int k)
{

	return CVector3();
}

int CHexGridCell::getTriangleCount()
{
	//Pendiente obtener triangulos del modelo
	return 4;
}




CVector3 CHexGridCell::calePoint(CVector3 center, int numpoint, float cellsize, bool poiny)
{
	CVector3 point;
	if (poiny == true)
	{
		float angle = 60 * numpoint - 30;
		float angleR = angle * PI_OVER_180;

		point.Y = 0.0f;
		point.X = center.X + cellsize * cosf(angleR);
		point.Z = center.Y + cellsize * sinf(angleR);
	}
	else if (poiny == false)
	{
		float angle = 60 * numpoint;
		float angleR = angle * PI_OVER_180;

		point.Y = 0.0f;
		point.X = center.X + cellsize * cosf(angleR);
		point.Z = center.Y + cellsize * sinf(angleR);
	}
	return point;
}

