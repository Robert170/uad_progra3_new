#pragma once

#include "CVector3.h"
#include "AABB_2D.h"
#include "CFrustum.h"


#define CAMERA_SPEED 0.025

class Ccamera
{
public:
	Ccamera();
	~Ccamera();

private:
	CVector3 m_worldUp, //*todos los que tengan * van en el constructor
		m_eyePos, //*
		m_lookAT, //*
		m_up,
		m_right;

	int m_frameBufferWidth, //*
		m_frameBufferHeight; //*

		float m_fieldOfView, //*
		m_nearPlane, //*
		m_farPlane; //*

		MathHelper::Matrix4 m_projectionMatrix,
		m_viewMatrix;

		bool m_cacheValid;

	CFrustum m_viewFrustum;

public:

	Ccamera(CVector3 worldUp, CVector3 eyePos, CVector3 lookAT, int frameBufferWidth, int frameBufferHeight, float fieldOfView, float nearPlane, float farPlane);
	void updateFramebuffersize(int w, int h);

	void update(float deltaTime);

	void moveForward(float deltaMove);// -- - w, s //mover la camara sobre vector lookAt

	void moveUp(float deltaMove); //--arrow up, arrow down //mover la camara sobre vector up

	void strafe(float deltaMove); //--a, d //mover la camara sobre vector right

	void orbitAroundLookAt(float deltaMove);//punto extra

	bool isAABBVisible(AABB_2D &aabb);

	//get para proj y view matrix
	//get para m_eyePos, m_lookAt, m_Up

private:
	void recalculateViewMatrix();
	void recalculateProjectionMatrix();
	void recalculateViewFrustum();

};

