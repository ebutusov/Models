#pragma once

#include "Vectors.h"

class CCamera 
{
protected:
	Vector3dT m_min, m_max;
	int m_sensivity;
public:
	Vector3dT mPos;							
	Vector3dT mView;							
	Vector3dT mUp;				

	CCamera();

	void SetMouseSensivity(int s) { m_sensivity = s; }
	int GetMouseSensivity() { return m_sensivity; }
	void SetStand(float h);
	void SetLimits(const Vector3dT &m, const Vector3dT &n);

	void StrafeCamera(float speed);
	void RotatePosition(float speed);
	void MoveCamera(float speed);
	void FlyCamera(float speed);

	void MouseRotate(int xoff, int yoff, BOOL view);
	void RotateView(float speed);

	void PositionCamera(float pos_x,  float pos_y,  float pos_z,
						 float view_x, float view_y, float view_z,
						 float up_x,   float up_y,   float up_z);

protected:
	inline void InBox(const Vector3dT &pos, Vector3dT &pto);
};