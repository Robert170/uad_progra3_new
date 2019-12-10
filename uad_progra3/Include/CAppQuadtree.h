#pragma once


#include <iostream>
#include <Windows.h>
#include "Globals.h"
#include "CApp.h"
#include "CHexGrid.h"
#include "AABB_2D.h"
#include "Cquadtree.h"
#include "Ccamera.h"

class CAppQuadtree : public CApp
{
public:
	HANDLE hThread[3];
	DWORD Thread_ID[3];
	string ToF;
	Ccamera * Camara;
	unsigned int m_texturedModelShaderId;

	CAppQuadtree();
	CAppQuadtree(int window_width, int window_height);
	~CAppQuadtree();
	bool StartGridThread();
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
	CHexGrid *m_Grid;

	unsigned int m_colorModelShaderId;

	
	Cquadtree * Quadtree;
	std::vector<CHexGridCell*>m_Cells;

	void onArrowUp(int mods);
	void onArrowDown(int mods);
	void onArrowLeft(int mods);
	void onArrowRight(int mods);

	bool m_initialized;

	//

protected:
	// Method to initialize the menu
	bool initializeMenu();

private:

};

