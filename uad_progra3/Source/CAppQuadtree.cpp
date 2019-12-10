#include "../stdafx.h"

#include <iostream>

using namespace std;
#include "../Include/CAppQuadtree.h"
#include "../Dependencies/JSON/nlohmann/json.hpp"
#include <fstream>


using json = nlohmann::json;

CAppQuadtree::CAppQuadtree() :
	CAppQuadtree(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT) // C++11 ability to call one constructor from another
{
	
	cout << "Constructor: CappHexGrid()" << endl;
}

CAppQuadtree::CAppQuadtree(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_Grid(nullptr)
{
	CVector3 m_worldUp = { 0,1,0 };

	CVector3 m_eyePos = { 0,3,0 };
	CVector3 m_lookAT = { 0,0,-3 };
	float m_fieldOfView = 75.0f;
	float m_nearPlane = 0.001f;
	float m_farPlane = 1000.0f;

	//Camara = new Ccamera(m_worldUp, m_eyePos, m_lookAT, window_width, window_height, m_fieldOfView, m_nearPlane, m_farPlane);
	cout << "Constructor: CappHexGrid(int window_width, int window_height)" << endl;
}

CAppQuadtree::~CAppQuadtree()
{
	if (Camara != nullptr)
	{
		delete Camara;
	}
}


DWORD WINAPI ThreadCreateGrid(LPVOID P)
{
	CAppQuadtree *appQuadtree = (CAppQuadtree*)P;
	appQuadtree->StartGridThread();
	return 0;
}



bool CAppQuadtree::StartGridThread()
{
	//hexParameters Parametros;
	//// crear obj de tipo CHexGrid (este obj tiene la confde json, col, filas, etc, y un array o vector de celdas)
	//json Datos;

	//ifstream in_file("hexgrid_cfg.json", ifstream::binary);


	//m_colorModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	//if (m_colorModelShaderId == 0)
	//{
	//	cout << "ERROR: Unable to load color shader" << endl;
	//	return false;
	//}

	//// Get shader for textured objects
	//// -------------------------------------------------------------------------------------------------------------

	//m_texturedModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	//if (m_texturedModelShaderId == 0)
	//{
	//	cout << "ERROR: Unable to load texture shader" << endl;
	//	return false;
	//}

	//// Texture
	//// Load texture file, create OpenGL Texture Object
	//// -------------------------------------------------------------------------------------------------------------
	//std::wstring wresourceFilenameTexture;
	//std::string resourceFilenameTexture;

	//

	//m_initialized = true;

	//Datos << in_file;

	//Parametros.Col = Datos["HexGrid"]["numCols"];
	//Parametros.Row = Datos["HexGrid"]["numRows"];
	//Parametros.CellSZ = Datos["HexGrid"]["cellSize"];
	//ToF = Datos["HexGrid"]["orientation"].get<std::string>();

	//if (ToF == "flat")
	//{
	//	Parametros.Ponty = false;
	//}
	//else
	//{
	//	Parametros.Ponty = true;
	//}



	//m_Grid = new CHexGrid(Parametros);
	//m_Grid->createHexGeometry(getOpenGLRenderer(), m_colorModelShaderId);
	//return true;
	return false;
}

void CAppQuadtree::initialize()
{
	//crear thrad1
	/*hThread[0] = CreateThread(NULL, 0, ThreadCreateGrid, this, 0, &Thread_ID[0]);
	WaitForSingleObject(hThread[0],50.0f);*/
	//...

	m_colorModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	if (m_colorModelShaderId == 0)
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}
	m_Grid = new CHexGrid();
	m_Grid->Col = 3;
	m_Grid->Row = 3;
	m_Grid->CellSZ=1.5;
	m_Grid->Ponty = true;
	m_Grid->createHexGeometry(getOpenGLRenderer(), m_colorModelShaderId);

	//Obtener numero de filas y columnas
	int Cols = m_Grid->Col;
	int Rows = m_Grid->Row;


	float MinX = 0.0f;
	float MaxX = 0.0f;
	float MinZ = 0.0f; 
	float MaxZ = 0.0f;

	for (int i = 0; i < Cols; i++)
	{
		for (int j = 0; j < Rows; j++)
		{
			CHexGridCell * Cell = &m_Grid->HEXA[i][j];
			m_Cells.push_back(Cell);

			for (int k = 0; k < 6; k++)
			{
				//CVector3 Corner = Cell->getCorner(k); //falta implementar
				CVector3 Corner = Cell->calePoint(Cell->Position, k, m_Grid->CellSZ, m_Grid->Ponty);

				if (Corner.X < MinX)
				{
					MinX = Corner.X;
				}
				if (Corner.X > MaxX)
				{
					MaxX = Corner.X;
				}
				if (Corner.Z < MinZ)
				{
					MinZ = Corner.Z;
				}
				if (Corner.Z>MaxZ)
				{
					MaxZ = Corner.Z;
				}
			}
		}

	}

	CVector3 corners[4];
	corners[0].X = MinX;
	corners[0].Z = MinZ;
	corners[0].Y = 0.0f;

	corners[1].X = MaxX;
	corners[1].Z = MinZ;
	corners[1].Y = 0.0f;

	corners[2].X = MinX;
	corners[2].Z = MaxZ;
	corners[2].Y = 0.0f;

	corners[3].X = MaxX;
	corners[3].Z = MaxZ;
	corners[3].Y = 0.0f;
	AABB_2D BOX;
	BOX.Setcorners(corners);
	
	Quadtree = new Cquadtree();
	Quadtree->SubDivide(BOX, 8, 10, *m_Grid, m_Grid->Row, m_Grid->Col);
	if (!Quadtree->m_root->Carga(getOpenGLRenderer(), m_colorModelShaderId))
	{
		std::cout << "No sirvio en el CAppQuadtree" << std::endl;
	}
}

void CAppQuadtree::update(double deltaTime)
{
	if (deltaTime <= 0.0f)
	{
		return;
	}
}

void CAppQuadtree::run()
{
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
			if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

void CAppQuadtree::render()
{
	CGameMenu *menu = getMenu();
	CVector3 objPos2;


	bool Change = true;

	objPos2.setValues(m_objectPosition.getX() + 2.5f, m_objectPosition.getY(), m_objectPosition.getZ());

	// If menu is active, render menu
	if (menu != nullptr && menu->isInitialized() && menu->isActive())
	{
		menu->render(getOpenGLRenderer());
	}
	else // Otherwise, render active object if loaded (or test cube if no object is loaded)
	{
		float Scale;
		// White 
		// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
		float color[3] = { 255.0f, 0.0f, 0.0f };
		unsigned int noTexture = 0;
		MathHelper::Matrix4 gameObjectMatrix;
		unsigned int tempVAO;

		// convert total degrees rotated to radians;
		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;
		for (int i = 0; i < m_Grid->Row; i++)
		{
			for (int j = 0; j < m_Grid->Col; j++)
			{
				MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(/*(float)totalDegreesRotatedRadians*/0, m_Grid->HEXA[i][j].Position);

				if (m_Grid->m_HexadVertexArrayObject > 0 && m_Grid->m_numFacesHexa > 0)
				{


					// Render pyramid in the first position, using the color shader
					getOpenGLRenderer()->renderObject(
						&m_colorModelShaderId,
						&m_Grid->m_HexadVertexArrayObject,
						&noTexture,
						m_Grid->m_numFacesHexa,
						color,
						&modelMatrix,
						COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
						false
					);

				}
			}

		}

	}
}

void CAppQuadtree::executeMenuAction()
{
}

void CAppQuadtree::onMouseMove(float deltaX, float deltaY)
{
}

void CAppQuadtree::onF2(int mods)
{
}

void CAppQuadtree::onF3(int mods)
{
}

void CAppQuadtree::onArrowUp(int mods)
{
	std::cout << "Si funciona w" << std::endl;
	Camara->moveForward(CAMERA_SPEED);
}

void CAppQuadtree::onArrowDown(int mods)
{
	std::cout << "si funciona s" << std::endl;
	Camara->moveForward(-CAMERA_SPEED);
}

void CAppQuadtree::onArrowLeft(int mods)
{
	std::cout << "si funciona a" << std::endl;
	Camara->strafe(-CAMERA_SPEED);
}

void CAppQuadtree::onArrowRight(int mods)
{
	std::cout << "si funciona d" << std::endl;
	Camara->strafe(CAMERA_SPEED);
}

bool CAppQuadtree::initializeMenu()
{
	return false;
}
