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


//void CHexGridCell::render()
//{
//	CGameMenu *menu = getMenu();
//
//	// If menu is active, render menu
//	if (menu != NULL
//		&& menu->isInitialized()
//		&& menu->isActive())
//	{
//		//...
//	}
//	else // Otherwise, render app-specific stuff here...
//	{
//		// =================================
//		//
//		// White 
//		// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
//		float color[3] = { 1.0f, 1.0f, 1.0f };
//		unsigned int noTexture = 0;
//
//		// convert total degrees rotated to radians;
//		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;
//		CVector3 Pos = m_objectPosition;
//
//		for (int i = 0; i < 2; i++)
//		{
//			// Get a matrix that has both the object rotation and translation
//			MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(/*(float)totalDegreesRotatedRadians*/1, Pos);
//
//			if (m_HexadVertexArrayObject > 0 && m_numFacesHexa > 0)
//			{
//
//
//				// Render pyramid in the first position, using the color shader
//				getOpenGLRenderer()->renderObject(
//					&m_colorModelShaderId,
//					&m_HexadVertexArrayObject,
//					&noTexture,
//					m_numFacesHexa,
//					color,
//					&modelMatrix,
//					COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
//					false
//				);
//
//				
//			}
//
//			// =================================
//		}
//
//	}
//}

//
//CHexGridCell CHexGridCell::Center(CVector3 center)
//{
//	center = { 0,0,0 };
//	//return CHexGridCell();
//}

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

//void CHexGridCell::createHexGeometry()
//{
//	int Cont = 0;
//	
//	float vertexData[18];
//
//	bool loaded = false;
//	CVector3 v1, v2, v3, v1v2, v1v3, norm;
//	
//	for (int i = 0; i < 6; i++)
//	{
//		Points = calePoint(Center, i, 1, true);
//
//		vertexData[Cont++] = Points.getX();
//		vertexData[Cont++] = Points.getY();
//		vertexData[Cont++] = Points.getZ();
//	}
//
//	float vertexuvs[6] =
//	{
//		0.0f,  0.0f,
//		0.0f,  0.0f,
//		0.0f,  0.0f
//	};
//
//	m_numfaceshexa = 4;
//
//	unsigned short faceindices[12] = {
//		0, 5, 4,
//		0, 4, 3,
//		0, 3, 1,
//		1, 3, 2
//	};
//
//	float normaldata[12] = {
//		0.0f, 1.0f, 0.0f,
//		0.0f, 1.0f, 0.0f,
//		0.0f, 1.0f, 0.0f,
//		0.0f, 1.0f, 0.0f
//
//	};
//
//	unsigned short facenormalindices[12] = {
//		0, 0, 0,
//		0, 0, 0,
//		0, 0, 0,
//		0, 0, 0
//	};
//
//
//	// allocate graphics memory for object
//	loaded = getopenglrenderer()->allocategraphicsmemoryforobject(
//		&m_colormodelshaderid,
//		&m_hexadvertexarrayobject,
//		vertexdata,
//		6,        // numero de vertices, internamente el codigo multiplica sizeof(float) * numvertices * 3
//		normaldata,
//		6,
//		vertexuvs,
//		6,
//		faceindices,
//		4,        // numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numindicesvert * 3
//		facenormalindices,
//		6,
//		faceindices,
//		6
//	);
//	
//	if (!loaded)
//	{
//		m_numfaceshexa = 0;
//
//		if (m_hexadvertexarrayobject > 0)
//		{
//			getopenglrenderer()->freegraphicsmemoryforobject(&m_hexadvertexarrayobject);
//			m_hexadvertexarrayobject = 0;
//		}
//	}
//}
