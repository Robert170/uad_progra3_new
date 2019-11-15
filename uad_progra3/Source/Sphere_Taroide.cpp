#pragma once
#include "../stdafx.h"

#include <iostream>
using namespace std;

#include "../Include/Globals.h"
#include "..\Include\Sphere_Taroide.h"


/* */
CAppSphere_Taroide::CAppSphere_Taroide() :
	CAppSphere_Taroide(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}


/* */
CAppSphere_Taroide::CAppSphere_Taroide(int window_width, int window_height) :
	CApp(window_width, window_height),
	m_sphereShaderProgramID{ 0 },
	m_sphereVertexArrayObject{ 0 },
	m_sphereTextureID{ 0 },
	m_numFaces{ 0 }
{
	cout << "Constructor: CAppSphere_Taroide(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}


/* */
CAppSphere_Taroide::~CAppSphere_Taroide()
{
	cout << "Destructor: ~CAppSphere_Taroide()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================
	// Free graphics memory used for the geometry
	if (m_sphereTextureID > 0 && getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->deleteTexture(&m_sphereTextureID);
	}

	// Free graphics memory used for the texture
	if (m_sphereVertexArrayObject > 0 && getOpenGLRenderer() != nullptr)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_sphereVertexArrayObject);
	}
}
/* */
void CAppSphere_Taroide::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	//
	/*std::cout << "Ingrese el radio de su esfera: "; std::cin >> radius;
	std::cout << "Ingrese la longitud de su esfera: "; std::cin >> sectorCount;
	std::cout << "Ingrese la latitud su esfera: "; std::cin >> stackStep;*/
	m_sphereShaderProgramID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);
	lengthInv = 1.0f / radius;
	sectorStep = 2 * PI / sectorCount;
	stackStep = PI / stackCount;
	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	// generate CCW index list of sphere triangles
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	glDrawElements(GL_TRIANGLES,                    // primitive type
		indices.size(),          // # of indices
		GL_UNSIGNED_INT,                 // data type
		(void*)0);
	// ==================================
}

/* */
void CAppSphere_Taroide::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.20f, 0.10f, 0.60f);

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
void CAppSphere_Taroide::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Update app-specific stuff here
	// ===============================
	//
	// ===============================
}

/* */
void CAppSphere_Taroide::render()
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
		if (m_numFaces > 0 && m_sphereVertexArrayObject > 0 && m_sphereShaderProgramID > 0)
		{
			// White 
			// Colors are in the 0..1 range, if you want to use RGB, use (R/255, G/255, G/255)
			float color[3] = { 1.0f, 1.0f, 1.0f };

			// Position object in (0, 0, 0)
			CVector3 pos = CVector3::ZeroVector();

			// Angle in RADIANS! (multiply * PI / 180.0f to convert to radians)
			float rotationAroundY = 125.0f * PI_OVER_180;

			// Create a simple model matrix, with given position and rotation of zero degrees around Y axis
			MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(rotationAroundY, pos);

			// Render the cube
			getOpenGLRenderer()->renderObject(
				&m_sphereShaderProgramID,                      // Shader to use to render the object
				&m_sphereVertexArrayObject,                    // ID for the geometry
				&m_sphereTextureID,                            // ID for the texture
				m_numFaces,                                  // Number of triangles that the object has
				color,                                       // Color (tint)
				&modelMatrix,                                // Object transformation
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES, //
				false                                        //
			);
		}
	}
}

/* */
void CAppSphere_Taroide::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppSphere_Taroide::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

