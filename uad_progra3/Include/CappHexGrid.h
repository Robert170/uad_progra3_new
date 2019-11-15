#pragma once

#include "Globals.h"
#include "CApp.h"



#include "Globals.h"
#include "CApp.h"
#include "C3DModel.h"
#include "CVector3.h"
#include "CHexGrid.h"
#include "C3DModel_Obj.h"
#include <utility>
#include <map>

class CappHexGrid : public CApp
{
public:

	// Constructors and destructor
	CappHexGrid();
	~CappHexGrid();
	CappHexGrid(int window_width, int window_height);


	// Inherited methods from CApp
	// ---------------------------

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

	// Methods overriden by CAppObjLoader
	// ----------------------------------

	// This derived class only uses F2/F3
	void onF2(int mods);
	void onF3(int mods);

private:
	// Pointer to an object of type C3DModel
	C3DModel *m_p3DModel;

	vector<C3DModel*> m_gameobjects;
	// Current delta time (time of the current frame - time of the last frame)
	double m_currentDeltaTime;

	// Current object rotation, expressed in degrees
	double m_objectRotation;

	// Current object position
	CVector3 m_objectPosition;


	map<string, C3DModel*> M;
	map<string, float> F;

	//  Object rotation speed (degrees per second)
	double m_rotationSpeed;

	string ToF;

	int m_renderPolygonMode;


	// OpenGL Shader Program for the color-only object
	unsigned int m_colorModelShaderId;

	// OpenGL Shader Program for the textured object
	unsigned int m_texturedModelShaderId;

	// Identifier for the OpenGL Texture Object 
	unsigned int m_textureID;

	bool m_initialized;

	CHexGrid *m_Grid;

	int Cont = 0;

protected:
	// Method to initialize the menu
	bool initializeMenu();

private:

	// Load/unload 3D model
	bool load3DModel(const char * const filename);
	void unloadCurrent3DModel();
	

	// Move camera away/closer
	void moveCamera(float direction);
};




