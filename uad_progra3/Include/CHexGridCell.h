#pragma once
#include "CVector3.h"

#include "../Include/COpenGLRenderer.h"
#include <vector>


class CHexGridCell 
{
private:




protected:

	// Method to initialize the menu 
	// (not needed here, so return false)
	bool initializeMenu() { return false; }

public:

	// Constructors and destructor
	CHexGridCell();
	CHexGridCell(int window_width, int window_height);
	~CHexGridCell();
	void P(CVector3 Pos);
	CVector3 getCorner(int k);
	int getTriangleCount();





	CVector3 Position;



	// --------------------------------------------------------------------------------------------------------------------------
	// Inherited methods from CApp
	// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
	// --------------------------------------------------------------------------------------------------------------------------

	// Method to update any objecs based on time elapsed since last frame
	//void update(double deltaTime);

	// Method to run the app
	//void run();

	// Method to render the app
	//void render();

	// -----------------------------------------------------------------------------------------------------------------------
	// Other inherited methods from CApp. These are optional to override.
	// Only provide an implementation in this DERIVED class if you need them to do something different than the base class CApp
	// -----------------------------------------------------------------------------------------------------------------------

	// Executes the action corresponding to the selected menu item
	//void executeMenuAction();

	// Called when mouse moves within the window
	//void onMouseMove(float deltaX, float deltaY);

	// Override on F2/F3
	//void onF2(int mods);
	//void onF3(int mods);

	//CHexGridCell Center(CVector3 center);

	CVector3 calePoint(CVector3 center, int numpoint, float cellsize, bool poiny);

private:

};


