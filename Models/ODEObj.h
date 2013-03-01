#pragma once
#include "SimObj.h"
#include "Quaternion.h"

class CODESimulation;

class CODEObj : public CSimObjGL
{
protected:
	dBodyID m_body;
public:
	CODEObj(CODESimulation &sim);
	virtual ~CODEObj();

	dBodyID GetBodyID() { return m_body; }

	void SetPosition(const Vector3dT &pos);
	void SetPosition(const tReal x, const tReal y, const tReal z);
	void GetPosition(Vector3dT &pos);
	void SetRotation(const CQuaternionf &q);
	void GetRotation(CQuaternionf &q);

	void SetLinearVel(const Vector3dT &vel);
	void GetLinearVel(Vector3dT &vel);

	void Enable(bool enable);
	bool IsEnabled();

	bool operator==(const CODEObj &other) { return m_body == other.m_body; }

	void SetAutoDisable(bool autodisable);
	void SetAutoDisableLinearVel(tReal threshold);
	void SetAutoDisableAngularVel(tReal threshold);
	void SetAutoDisableTime(tReal inactive_time);

private:
	virtual void RenderGL(IRenderer *r);
protected:
	virtual void RenderODE(IRenderer *r);
};

class CODESimple : public CODEObj
{
protected:
	dGeomID m_geom;
public:
	CODESimple(CODESimulation &sim);
	virtual ~CODESimple();
};

// TODO
class CODEComplex : public CODEObj
{
public:
	CODEComplex(CODESimulation &sim) : CODEObj(sim) {}
};

class CODEPrimitive : public CODESimple
{
protected:
	tReal m_color[4];
public:
	CODEPrimitive(CODESimulation &sim) : CODESimple(sim) {};
	void SetColor(const tReal *col4)
	{
		memcpy(m_color, col4, sizeof(tReal)*4);
	}
	void SetColor(tReal r, tReal g, tReal b, tReal a)
	{
		m_color[0] = r;
		m_color[1] = g;
		m_color[2] = b;
		m_color[3] = a;
	}
	const tReal* GetColor() { return m_color; }
	
protected:
	virtual void RenderODE(IRenderer *r);
};

class CODEBall : public CODEPrimitive
{
protected:
	tReal m_radius;
public:
	CODEBall(CODESimulation &sim, tReal radius, tReal mass);
	virtual ~CODEBall() {};

	tReal GetRadius() { return m_radius; }

	void SetMass(tReal m);
protected:
	virtual void RenderODE(IRenderer *r);
};

class CODECylinder : public CODEPrimitive
{
protected:
	tReal m_radius, m_length;
public:
	CODECylinder(CODESimulation &sim, tReal radius, tReal length, tReal mass);
	virtual ~CODECylinder() {};

protected:
	virtual void RenderODE(IRenderer *r);
};

class CODEBox : public CODEPrimitive
{
protected:
	tReal m_x, m_y, m_z;
public:
	CODEBox(CODESimulation &sim, tReal x, tReal y, tReal z, tReal mass);
	virtual ~CODEBox() {};

	void GetDimensions(tReal x, tReal y, tReal z);
protected:
	virtual void RenderODE(IRenderer *r);
};

class CODEPlayerBall : public CODEBall
{
public:
	CODEPlayerBall(CODESimulation &sim, tReal radius, tReal mass);
	void Throw(const Vector3dT &force);
protected:
	virtual void RenderODE(IRenderer *r);
};