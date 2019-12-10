#include "../stdafx.h"

#include "windows.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CAppHexGrid.h"
#include "../Include/C3DModel.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureLoader.h"
#include "../Include/CHexGrid.h"
#include "../Dependencies/JSON/nlohmann/json.hpp"
#include <fstream>


using json = nlohmann::json;

/* */
CappHexGrid::CappHexGrid() :
	CappHexGrid(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT) // C++11 ability to call one constructor from another
{
	cout << "Constructor: CappHexGrid()" << endl;
}

/* */
CappHexGrid::CappHexGrid(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_p3DModel(nullptr),
	m_currentDeltaTime{ 0.0 },
	m_objectRotation{ 0.0 },
	m_objectPosition{ 0.0f, 0.0f, 0.0f },
	m_rotationSpeed{ DEFAULT_ROTATION_SPEED },
	m_Grid(nullptr),
	m_renderPolygonMode(0)
{
	cout << "Constructor: CAppFbxLoader(int window_width, int window_height)" << endl;
}

/* */
CappHexGrid::~CappHexGrid()
{
	cout << "Destructor: ~CappHexGrid()" << endl;
	unloadCurrent3DModel();

	if (m_Grid != nullptr)
	{
		delete m_Grid;
	}
}

/* */
void CappHexGrid::initialize()
{
	// crear obj de tipo CHexGrid (este obj tiene la confde json, col, filas, etc, y un array o vector de celdas)
	json Datos;

	ifstream in_file("hexgrid_cfg.json", ifstream::binary);

	
	m_colorModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);

	if (m_colorModelShaderId == 0)
	{
		cout << "ERROR: Unable to load color shader" << endl;
		return;
	}

	// Get shader for textured objects
	// -------------------------------------------------------------------------------------------------------------

	m_texturedModelShaderId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

	if (m_texturedModelShaderId == 0)
	{
		cout << "ERROR: Unable to load texture shader" << endl;
		return;
	}

	// Texture
	// Load texture file, create OpenGL Texture Object
	// -------------------------------------------------------------------------------------------------------------
	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	// Check texture file exists for the textured cube
	if (!CWideStringHelper::GetResourceFullPath(MC_LEAVES_TEXTURE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	// Initialize the texture
	m_textureID = 0;
	if (!CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &m_textureID, getOpenGLRenderer()))
	{
		cout << "ERROR: Unable load texture:" << endl;
		cout << "  " << MC_LEAVES_TEXTURE << endl;
		return;
	}

	m_initialized = true;

	Datos << in_file;
	hexParameters Parametros;

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

	

	int objects_present = (int)Datos.count("Models");

	if (objects_present > 0 && Datos["Models"].type() == json::value_t::array)
	{
		std::string objName = "";
		std::string objFileName = "";
		C3DModel * Model;
		// iterate the array
		for (json::iterator it = Datos["Models"].begin(); it != Datos["Models"].end(); ++it)
		{
			objName= it.value().value("name", objName);
			objFileName = it.value().value("filename", objFileName);
			objFileName = "Resources/MEDIA/" + objFileName;

			Model = new C3DModel_Obj();
			
			if(M.find(objName) == M.end())
			{
				M[objName] = Model->load(objFileName.c_str(), getOpenGLRenderer());
			}

		}
	}


	
	//Meter a stargrid
	m_Grid = new CHexGrid(Parametros);
	m_Grid->createHexGeometry(getOpenGLRenderer(), m_colorModelShaderId);

	
	

	int objects_Pos = (int)Datos.count("ModelInstances");

	if (objects_Pos > 0 && Datos["ModelInstances"].type() == json::value_t::array)
	{
		int C_P = 0;
		int R_P = 0;
		std::string NameObj;
		float Scale;
		CVector3 Rotation;
		// iterate the array
		for (json::iterator it = Datos["ModelInstances"].begin(); it != Datos["ModelInstances"].end(); ++it)
		{
			C_P = it.value().value("column", C_P);
			R_P = it.value().value("row", R_P);
			NameObj = it.value().value("model", NameObj);
			Scale = it.value().value("scale", Scale);
			//Rotation = it.value().value("rotation", Rotation);
			m_Grid->Obj_Po[R_P][C_P] = NameObj;
			F[NameObj] = Scale;
		}
	}

	in_file.close();
	// al crear el obj de tipo CHexGrid, crear cada objeto de CHexGridCell
}

/* */
void CappHexGrid::run()
{
	// Check if CGameWindow object and third-party Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_OBJ_LOADER_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.15f, 0.75f, 0.75f);

			

			// Create our menu (add all menu items)
			if (!initializeMenu())
			{
				return;
			}

			if (m_initialized)
			{
				getOpenGLRenderer()->setWireframePolygonMode();
				cout << "Entering Main loop" << endl;
				getGameWindow()->mainLoop(this);
			}

			// Enter main loop
			
		}
	}
}

/* */
bool CappHexGrid::initializeMenu()
{
	cout << "CAppCubeTest::initializeMenu()" << endl;

	std::wstring wresourceFilenameTexture;
	std::string resourceFilenameTexture;

	// If resource files cannot be found, return
	if (!CWideStringHelper::GetResourceFullPath(MENU_TEXTURE_FILE, wresourceFilenameTexture, resourceFilenameTexture))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << MENU_TEXTURE_FILE << endl;
		return false;
	}

	if (getMenu() != nullptr)
	{
		CGameMenu *const menu = getMenu();

		unsigned int menuShaderProgramId = 0;
		unsigned int textureObjectId = 0;
		unsigned int vaoMenuItemId = 0;
		int colorUniformLocation = -1;
		int textureUniformLocation = -1;
		bool generatedMenuItemGeometry = false;
		float currentX = 0.65f, currentY = 0.8f;
		float menuItemHeight = 0.2f;
		float deltaY = 0.25f;
		float UV[8];

		menuShaderProgramId = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_MENU);

		// Create a shader program to use for the menu
		if (menuShaderProgramId == 0)
		{
			cout << "ERROR: Unable to load menu shader program" << endl;
			return false;
		}

		// Set the generated shader program in the menu object
		menu->setShaderProgramId(menuShaderProgramId);

		// Read texture file and generate an OpenGL texture object
		if (CTextureLoader::loadTexture(resourceFilenameTexture.c_str(), &textureObjectId, getOpenGLRenderer()))
		{
			// Set the generated texture object in the menu object
			menu->setTextureObjectId(textureObjectId);
		}
		else
		{
			return false;
		}

		std::vector<std::string> menuOptions;
		menuOptions.push_back("Load 3D Model");
		menuOptions.push_back("Options");
		menuOptions.push_back("Exit");

		UV[0] = 0.01f;  // top left U
		UV[1] = 1.0f;   // top left V
		UV[2] = 0.99f;  // top right U
		UV[3] = 1.0f;   // top right V
		UV[4] = 0.01f;  // bottom left U
		UV[5] = 0.75f;  // bottom left V
		UV[6] = 0.99f;  // bottom right U
		UV[7] = 0.75f;  // bottom right V

		for (size_t i = 0; i < menuOptions.size(); i++)
		{
			// Allocate graphics memory for menu item
			generatedMenuItemGeometry = getOpenGLRenderer()->allocateGraphicsMemoryForMenuItem(
				currentX,
				currentY,
				menuItemHeight,
				UV,
				&menuShaderProgramId,
				&vaoMenuItemId
			);

			// If operation failed
			if (!generatedMenuItemGeometry)
			{
				menu->cleanupGraphicsObjects(getOpenGLRenderer());
				return false;
			}

			menu->addMenuItem(menuOptions[i].c_str(), currentX, currentY, vaoMenuItemId);
			currentY -= deltaY;

			UV[1] -= 0.25f;
			UV[3] -= 0.25f;
			UV[5] -= 0.25f;
			UV[7] -= 0.25f;

			// Custom hack, texture used is the same for the exam, but the "exit" option is the 4th, not 3rd, so increment twice
			if (i == 1)
			{
				UV[1] -= 0.25f;
				UV[3] -= 0.25f;
				UV[5] -= 0.25f;
				UV[7] -= 0.25f;
			}
		}

		return true;
	}

	return false;
}

/* */
void CappHexGrid::update(double deltaTime)
{
	double degreesToRotate = 0.0;

	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Save current delta time
	m_currentDeltaTime = deltaTime;

	// Calculate degrees to rotate
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// degrees = rotation speed * delta time 
	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degreesToRotate = m_rotationSpeed * (deltaTime / 1000.0);
	// accumulate rotation degrees
	m_objectRotation += degreesToRotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0)
	{
		m_objectRotation -= 360.0;
	}
	if (m_objectRotation < 0.0)
	{
		m_objectRotation = 0.0;
	}
}

/* */
void CappHexGrid::render()
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


					if (m_Grid->Obj_Po[i][j] != " ")
					{
						Scale = F[m_Grid->Obj_Po[i][j]];
						MathHelper::Matrix4 ScaleMatrix(
							Scale, 0.0f, 0.0f, 0.0f,
							0.0f, Scale, 0.0f, 0.0f,
							0.0f, 0.0f, Scale, 0.0f,
							m_Grid->HEXA[i][j].Position.X, m_Grid->HEXA[i][j].Position.Y, m_Grid->HEXA[i][j].Position.Z, 1.0f
						);
						

						//gameObjectMatrix = MathHelper::SimpleModelMatrixRotationTranslation(0.0f, m_Grid->HEXA[i][j].Position);
						tempVAO = M[m_Grid->Obj_Po[i][j]]->getGraphicsMemoryObjectId();
						getOpenGLRenderer()->renderObject(
							&m_colorModelShaderId,
							&tempVAO,
							0,
							M[m_Grid->Obj_Po[i][j]]->getNumFaces(),
							color,
							&ScaleMatrix,
							COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
							false);
					}
				}
			}

		}
		
	}

}

/* */
bool CappHexGrid::load3DModel(const char * const filename)
{
	unsigned int modelVertexArrayObject = 0;

	// Unload any current 3D model
	unloadCurrent3DModel();

	// Create new 3D object
	m_p3DModel = C3DModel::load(filename, getOpenGLRenderer());

	if (m_p3DModel == nullptr)
	{
		cout << "ERROR: Unable to read model from file" << endl;
		return false;
	}
	else if (!m_p3DModel->isGeometryLoaded())
	{
		cout << "ERROR: Unable to read model geometry." << endl;
		unloadCurrent3DModel();
		return false;
	}
	else if (m_p3DModel->getGraphicsMemoryObjectId() == 0)
	{
		cout << "ERROR: Unable to save geometry to graphics card." << endl;
		unloadCurrent3DModel();
		return false;
	}

	// 
	return (m_p3DModel->getGraphicsMemoryObjectId() > 0);
}

/* */
void CappHexGrid::unloadCurrent3DModel()
{
	if (m_p3DModel != nullptr)
	{
		// Free up graphics memory
		unsigned int vaoID = m_p3DModel->getGraphicsMemoryObjectId();
		getOpenGLRenderer()->freeGraphicsMemoryForObject(
			&vaoID
		);

		// Free up texture object memory
		if (m_p3DModel->getTextureObjectId() > 0)
		{
			unsigned int texID = m_p3DModel->getTextureObjectId();
			getOpenGLRenderer()->deleteTexture(&texID);
		}

		// Delete 3D object
		delete m_p3DModel;
		m_p3DModel = nullptr;
	}
}

/* */
void CappHexGrid::onF2(int mods)
{
	if (m_renderPolygonMode == 0)
	{
		getOpenGLRenderer()->setFillPolygonMode();
		m_renderPolygonMode = 1;
	}
	else
	{
		getOpenGLRenderer()->setWireframePolygonMode();
		m_renderPolygonMode = 0;
	}

}

/* */
void CappHexGrid::onF3(int mods)
{
	// Check BITWISE AND to detect shift/alt/ctrl
	if (mods & KEY_MOD_SHIFT)
	{
		moveCamera(-1.0f);
	}
	else
	{
		moveCamera(1.0f);
	}
}

/* */
void CappHexGrid::onMouseMove(float deltaX, float deltaY)
{
	if (deltaX < 100.0f && deltaY < 100.0f)
	{
		float moveX = -deltaX * DEFAULT_CAMERA_MOVE_SPEED;
		float moveZ = -deltaY * DEFAULT_CAMERA_MOVE_SPEED;

		float currPos[3];
		for (int i = 0; i < m_Grid->Row; i++)
		{
			for (int j = 0; j < m_Grid->Col; j++)
			{
				m_Grid->HEXA[i][j].Position.getValues(currPos);
				//m_objectPosition.getValues(currPos);
				currPos[0] += moveX;
				currPos[2] += moveZ;
				//m_objectPosition.setValues(currPos);
				m_Grid->HEXA[i][j].Position.setValues(currPos);
			}
		}
		
	}
}

/* */
void CappHexGrid::moveCamera(float direction)
{
	if (getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->simpleCameraZoom(direction);
	}
}

/* */
void CappHexGrid::executeMenuAction()
{
	if (getMenu() != nullptr)
	{
		int option = getMenu()->getSelectedMenuItemNumber();

		switch (option)
		{
		case 1:
			onF2(0);
			break;
		case 2:
			// Not implemented
			cout << "<MENU OPTION NOT IMPLEMENTED>" << endl;
			break;
		case 3:
			if (getGameWindow() != nullptr)
			{
				getGameWindow()->requestWindowClose();
			}
			break;
		}
	}
}