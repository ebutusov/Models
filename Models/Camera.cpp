#include "StdAfx.h"
#include "Camera.h"

CCamera::CCamera()
	: m_min(-100.0f, 2.0f, -100.0f),
		m_max(100.0f, 100.0f, 100.0f),
		m_sensivity(200)
{
}

void CCamera::PositionCamera(float pos_x,  float pos_y,  float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x,   float up_y,   float up_z)
{
	mPos	= Vector3dT(pos_x,  pos_y,  pos_z ); // set position
	mView	= Vector3dT(view_x, view_y, view_z); // set view
	mUp		= Vector3dT(up_x,   up_y,   up_z  ); // set the up vector	
}

void CCamera::SetLimits(const Vector3dT &min, const Vector3dT &max)
{
	m_min = max;
	m_max = min;
}

inline void CCamera::InBox(const Vector3dT &pos, Vector3dT &pto)
{
	Vector3dT pt = pos + pto;

	if (pt.x > m_max.x || pt.x < m_min.x)
		pto.x = 0.0f;
	if (pt.y > m_max.y || pt.y < m_min.y)
		pto.y = 0.0f;
	if (pt.z > m_max.z || pt.z < m_min.z)
		pto.z = 0.0f;
}

void CCamera::FlyCamera(float speed)
{
	Vector3dT dir = mView - mPos;
	Vector3dT check = dir * speed;
	InBox(mPos, check);
	mPos += check;
	mView += check;
	//mPos += (dir * speed);
	//mView += (dir * speed);
}

void CCamera::MoveCamera(float speed)
{
	Vector3dT vVector = mView - mPos;	// Get the view vector
	// forward positive camera speed and backward negative camera speed.
	mPos.x  = mPos.x  + vVector.x * speed;
	mPos.z  = mPos.z  + vVector.z * speed;
	mView.x = mView.x + vVector.x * speed;
	mView.z = mView.z + vVector.z * speed;
}

void CCamera::SetStand(float h)
{
	float c = mPos.y + h;
	if (c >= m_min.y && c <= m_max.y)  
	{
		mPos.y = c;
		mView.y += h;
	}
}

void CCamera::RotateView(float speed)
{
	Vector3dT vVector = mView - mPos;	// Get the view vector

	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}

void CCamera::RotatePosition(float speed)
{	
	Vector3dT vVector = mPos - mView;

	mPos.z = (float)(mView.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mPos.x = (float)(mView.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}

void CCamera::StrafeCamera(float speed)
{
	Vector3dT vVector = mView - mPos;	// Get the view vector
	Vector3dT vOrthoVector;              // Orthogonal vector for the view vector

	vOrthoVector.x = -vVector.z;
	vOrthoVector.z =  vVector.x;

	// left positive cameraspeed and right negative -cameraspeed.
	mPos.x  = mPos.x  + vOrthoVector.x * speed;
	mPos.z  = mPos.z  + vOrthoVector.z * speed;
	mView.x = mView.x + vOrthoVector.x * speed;
	mView.z = mView.z + vOrthoVector.z * speed;
}


void CCamera::MouseRotate( int xoff, int yoff, BOOL view )
{
	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;							
	
	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angle_y = (float)( xoff ) / m_sensivity;		
	angle_z = (float)( yoff ) / m_sensivity;

	// The higher the value is the faster the camera looks around.
	angle_z = angle_z * 2;

	// prevent looking straight in the sky or ground (very confusing)
	Vector3dT dir = mView - mPos;
	dir.y += angle_z;
	float alfa = dir.GetAlfa();
	if (alfa < 170.0f && alfa > 15.0)
		mView.y += angle_z;

	// TODO: limit the rotation around the x-axis
	//if(mView.y > 10.0f)		mView.y = 10.0f;
	//if(mView.y < 0.4f)		mView.y = 0.4f;
	
	if (view)
		RotateView(-angle_y);
	else
		RotatePosition(-angle_y);
}