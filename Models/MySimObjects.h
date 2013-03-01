#pragma once

#include "ODEObj.h"
#include "MySimulation.h"

class CCylinderAD : public CODECylinder
{
private:
	tReal m_fallen_time;
public:
	CCylinderAD(CMySimulation &sim, tReal radius, tReal length, tReal mass);
	virtual void Frame(tReal total, tReal dt);
};

class CPlayerBallAD : public CODEPlayerBall
{
private:
	tReal m_inact_time;
	CMySimulation *m_sim;
public:
	CPlayerBallAD(CMySimulation &sim, tReal radius, tReal mass);
	virtual void Frame(tReal total, tReal dt);
	virtual void RenderGLEx(IRenderer *r);
};

class CThrowBox : public CODEBox
{
public:
	CThrowBox(CMySimulation &sim, tReal x, tReal y, tReal z, tReal mass);
	void Throw(const Vector3dT &velocity);
};