#include "../stdafx.h"

#include <iostream>
using namespace std;
#include "../Include/Ccamera.h"

Ccamera::Ccamera()
{
}

Ccamera::~Ccamera()
{
}

Ccamera::Ccamera(CVector3 worldUp, CVector3 eyePos, CVector3 lookAT, int frameBufferWidth, int frameBufferHeight, float fieldOfView, float nearPlane, float farPlane)
{
	m_worldUp = worldUp;
	m_eyePos = eyePos;
	m_lookAT = lookAT;
	m_frameBufferWidth = frameBufferWidth;
	m_frameBufferHeight = frameBufferHeight;

	m_fieldOfView = fieldOfView;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}

void Ccamera::updateFramebuffersize(int w, int h)
{
}

void Ccamera::update(float deltaTime)
{
}

void Ccamera::moveForward(float deltaMove)
{
}

void Ccamera::moveUp(float deltaMove)
{
}

void Ccamera::strafe(float deltaMove)
{
}

void Ccamera::orbitAroundLookAt(float deltaMove)
{
}

bool Ccamera::isAABBVisible(AABB_2D & aabb)
{
	if (!m_cacheValid)
	{
		recalculateViewMatrix();
		recalculateViewFrustum();
	}

	// If the AABB is either completely visible (all corners inside the frustum), or intersecting (at least one corner inside the frustum)
	return (
		m_viewFrustum.isAABBVisible(aabb) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT ||
		m_viewFrustum.isAABBVisible(aabb) == CFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE
		);
}

void Ccamera::recalculateViewMatrix()
{
}

void Ccamera::recalculateProjectionMatrix()
{
}

void Ccamera::recalculateViewFrustum()
{
	// Avoid division by zero
	if (m_frameBufferHeight > 0)
	{
		// Geometric approach
		m_viewFrustum.update(
			m_eyePos,
			m_lookAT,
			m_up,
			m_right,
			m_nearPlane,
			m_farPlane,
			m_fieldOfView,
			(float)m_frameBufferWidth / (float)m_frameBufferHeight);
	}
}
