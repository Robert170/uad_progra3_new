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

	Camara = new Ccamera(m_worldUp, m_eyePos, m_lookAT, window_width, window_height, m_fieldOfView, m_nearPlane, m_farPlane);
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
	hexParameters Parametros;
	// crear obj de tipo CHexGrid (este obj tiene la confde json, col, filas, etc, y un array o vector de celdas)
	json Datos;

	ifstream in_file("hexgrid_cfg.json", ifstream::binary);


	m_colorModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	if (m_colorModelShaderId == 0)
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return false;
	}

	// Get shader for textured objects
	// -------------------------------------------------------------------------------------------------------------

	m_texturedModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (m_texturedModelShaderId == 0)
	{
		cout << "ERROR: Unable to load texture shader" << endl;
		return false;
	}

	// Texture
	// Load texture file, create OpenGL Texture Object
	// -------------------------------------------------------------------------------------------------------------
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	

	m_initialized = true;

	Datos << in_file;

	Parametros.Col = Datos["HexGrid"]["numCols"];
	Parametros.Row = Datos["HexGrid"]["numRows"];
	Parametros.CellSZ = Datos["HexGrid"]["cellSize"];
	ToF = Datos["HexGrid"]["orientation"].get<std::string>();

	if (ToF == "flat")
	{
		Parametros.Ponty = false;
	}
	else
	{
		Parametros.Ponty = true;
	}



	m_Grid = new CHexGrid(Parametros);
	m_Grid->createHexGeometry(getOpenGLRenderer(), m_colorModelShaderId);
	return true;
}

void CAppQuadtree::initialize()
{
	//crear thrad1
	hThread[0] = CreateThread(NULL, 0, ThreadCreateGrid, this, 0, &Thread_ID[0]);
	WaitForSingleObject(hThread[0],50.0f);
	//...

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
			//CHexGridCell * Cell = m_Grid->getCell(i, j); //falta implementar 
			//m_Cells.push_back(Cell);
			//for (int k = 0; k < 6; k++)
			//{
			//	CVector3 Corner = Cell->getCorner(k); //falta implementar

			//	if (Corner.X < MinX)
			//	{
			//		MinX = Corner.X;
			//	}
			//	if (Corner.X > MaxX)
			//	{
			//		MaxX = Corner.X;
			//	}
			//	if (Corner.Z < MinZ)
			//	{
			//		MinZ = Corner.Z;
			//	}
			//	if (Corner.Z>MaxZ)
			//	{
			//		MaxZ = Corner.Z;
			//	}
			//}
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
	Quadtree->SubDivide(BOX, &m_Cells);
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

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		// =================================
		//
		// =================================
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
