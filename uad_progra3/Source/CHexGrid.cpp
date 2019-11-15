
#include "../stdafx.h"

#include <iostream>
using namespace std;

#include <assert.h>
#include "../Include/Globals.h"
#include "..\Include\CHexGrid.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"



void CHexGrid::render()
{

	//int N = Col * Row;
	//// White 
	//// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
	//float color[3] = { 1.0f, 1.0f, 1.0f };
	//unsigned int noTexture = 0;

	//// convert total degrees rotated to radians;
	//double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;
	//CVector3 Pos = m_objectPosition;

	//for (int i = 0; i < N; i++)
	//{
	//	//Pos = vecCell[current col][current row].getpos();
	//	// Get a matrix that has both the object rotation and translation
	//	MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(/*(float)totalDegreesRotatedRadians*/1, Pos);

	//	if (m_HexadVertexArrayObject > 0 && m_numFacesHexa > 0)
	//	{


	//		// Render pyramid in the first position, using the color shader
	//		getOpenGLRenderer()->renderObject(
	//			&m_colorModelShaderId,
	//			&m_HexadVertexArrayObject,
	//			&noTexture,
	//			m_numFacesHexa,
	//			color,
	//			&modelMatrix,
	//			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
	//			false
	//		);
	//		/*
	//			if(vecCell[][].getmodelinstance().getindex()!= -1){
	//			
	//			}
	//		*/

	//	}


	//	// =================================

	//}
}


CHexGrid::CHexGrid(hexParameters Datos):
	Col(Datos.Col),
	Row(Datos.Row),
	CellSZ(Datos.CellSZ),
	Ponty(Datos.Ponty)
{
}

CHexGrid::CHexGrid()
{
}

CHexGrid::~CHexGrid()
{
	delete[] Obj_Po;
	delete[] HEXA;
}

void CHexGrid::createHexGeometry(COpenGLRenderer * const ptrRenderer, unsigned int m_colorModelShaderId)
{
	CHexGridCell C;

	int Cont = 0;
	
	float vertexData[18];
	
	bool loaded = false;
	CVector3 v1, v2, v3, v1v2, v1v3, norm;
	
	for (int i = 0; i < 6; i++)
		{
		Points = C.calePoint(Center, i, CellSZ, Ponty);
	
			vertexData[Cont++] = Points.getX();
			vertexData[Cont++] = Points.getY();
			vertexData[Cont++] = Points.getZ();
		}
	
		float vertexUVs[6] =
		{
			1.0f,  0.0f,
			1.0f,  0.0f,
			1.0f,  0.0f
		};
	
		m_numFacesHexa = 4;
	
		unsigned short faceIndices[12] = {
			4,5,0 , 
			3,4,0 ,
			1,3,0 , 
			2,3,1 
		};		
				
		float normalData[12] = {
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
	
		};
	
		unsigned short faceNormalIndices[12] = {
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0
		};

	
	
	//Allocate graphics memory for object
	loaded = ptrRenderer->allocateGraphicsMemoryForObject(
		&m_colorModelShaderId,
		&m_HexadVertexArrayObject,
		vertexData,
		6,        // Numero de vertices, internamente el codigo multiplica sizeof(float) * numVertices * 3
		normalData,
		6,
		vertexUVs,
		6,
		faceIndices,
		4,        // Numero de indices, internamente el codigo multiplica sizeof(unsigned short) * numIndicesVert * 3
		faceNormalIndices,
		6,
		faceIndices,
		6
	);
	
	if (!loaded)
	{
		m_numFacesHexa = 0;
	
		if (&m_HexadVertexArrayObject > 0)
		{
			ptrRenderer->freeGraphicsMemoryForObject(&m_HexadVertexArrayObject);
			m_HexadVertexArrayObject = 0;
		}
	}


	///
	///
	///
	///


	float W_P = (sqrtf(3) * CellSZ);
	float H_P = 2 * CellSZ;

	float W_F = 2 * CellSZ;
	float H_F = (sqrtf(3) * CellSZ);
	CVector3 Pos = { 0,0,0 };
	CVector3 Pos2 = { 0,0,0 };


	
	

	if (Col > 0 && Row > 0)
	{
		HEXA = new CHexGridCell*[Row];
	}
	
	for (int i = 0; i < Row; i++)
	{
		HEXA[i]= new CHexGridCell[Col];
	}

	if (Col > 0 && Row > 0)
	{
		Obj_Po = new std::string*[Row];
	}

	for (int i = 0; i < Row; i++)
	{
		Obj_Po[i] = new std::string[Col];
	}

	if(Ponty == true)
	{
		for (int i = 0; i < Row; i++)
		{
			for (int j = 0; j < Col; j++)
			{
				HEXA[i][j].P(Pos);
				Pos += CVector3(W_P, 0, 0);
				Obj_Po[i][j] = " ";
			}
			if (i % 2)
			{
				Pos2 += CVector3(W_P / 2, 0, H_P*(3.0f / 4.0f));
				Pos = Pos2;
				
			}
			else
			{
				Pos2 += CVector3(-(W_P / 2), 0, H_P*(3.0f / 4.0f));
				Pos = Pos2;

			}

		}
	}

	if (Ponty == false)
	{
		for (int j = 0; j < Col; j++)
		{
			for (int i = 0; i < Row; i++)
			{
				HEXA[i][j].P(Pos);
				Pos += CVector3(W_P, 0, 0);
				Obj_Po[i][j] = " ";
			}
			if (j % 2)
			{
				Pos2 += CVector3(W_F*(3.0f / 4.0f), 0, H_F / 2);
				Pos = Pos2;
			}
			else
			{
				Pos2 += CVector3(W_F*(3.0f / 4.0f), 0, -(H_F / 2));
				Pos = Pos2;
			}

		}
	}

}
