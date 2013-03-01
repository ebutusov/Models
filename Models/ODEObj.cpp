#include "StdAfx.h"
#include "ODEObj.h"
#include "Quaternion.h"
#include "ODESimulation.h"

CODEObj::CODEObj(CODESimulation &sim)
	:m_body(dBodyCreate(sim.GetWorld()))
{
	sim.AddObject(this);
	dBodySetData(m_body, reinterpret_cast<void *>(this));
}

CODEObj::~CODEObj(void)
{
	dBodyDestroy(m_body);
}

void CODEObj::RenderODE(IRenderer *r)
{
	r->SetColor(0.3f, 0.6f, 0.8f, 1.0f);
	r->DrawBall(1.0f);
}

void CODEObj::RenderGL(IRenderer *r)
{
	const tReal *p = dBodyGetPosition(m_body);
  const tReal *rot = dBodyGetRotation(m_body);

	tReal m[16];
  m[ 0] = rot[ 0];m[ 1] = rot[ 4];m[ 2] = rot[ 8];m[ 3] = 0;
  m[ 4] = rot[ 1];m[ 5] = rot[ 5];m[ 6] = rot[ 9];m[ 7] = 0;
  m[ 8] = rot[ 2];m[ 9] = rot[ 6];m[10] = rot[10];m[11] = 0;
  m[12] = p[ 0];m[13] = p[ 1];m[14] = p[ 2];m[15] = 1;
  
	r->MatrixMultiply(m);
	RenderODE(r);
}


void CODEObj::SetPosition(const Vector3dT &pos)
{
	dBodySetPosition(m_body, pos.x, pos.y, pos.z);
}

void CODEObj::SetPosition(const tReal x, const tReal y, const tReal z)
{
	dBodySetPosition(m_body, x, y, z);
}

void CODEObj::GetPosition(Vector3dT &pos)
{
	const tReal *p = dBodyGetPosition(m_body);
	pos.x = p[0];
	pos.y = p[1];
	pos.z = p[2];
}

void CODEObj::GetRotation(CQuaternionT &q)
{
	const tReal *qt = dBodyGetQuaternion(m_body);
	q.CreateWA((tReal *)qt);
}

void CODEObj::SetRotation(const CQuaternionT &q)
{
	tReal qt[4];
	q.GetQuaternionW(qt);
	dBodySetQuaternion(m_body, qt);
}

void CODEObj::SetLinearVel(const Vector3dT &vel)
{
	::dBodySetLinearVel(m_body, vel.x, vel.y, vel.z);
}

void CODEObj::GetLinearVel(Vector3dT &vel)
{
	const tReal* v = dBodyGetLinearVel(m_body);
	vel.x = v[0];
	vel.y = v[1];
	vel.z = v[2];
}

void CODEObj::Enable(bool enable)
{
	if (enable)
		dBodyEnable(m_body);
	else
		dBodyDisable(m_body);
}

bool CODEObj::IsEnabled()
{
	return dBodyIsEnabled(m_body);
}

void CODEObj::SetAutoDisable(bool autodisable)
{
	dBodySetAutoDisableFlag(m_body, autodisable);
}

void CODEObj::SetAutoDisableLinearVel(tReal threshold)
{
	dBodySetAutoDisableLinearThreshold(m_body, threshold);
}

void CODEObj::SetAutoDisableAngularVel(tReal threshold)
{
	dBodySetAutoDisableAngularThreshold(m_body, threshold);
}

void CODEObj::SetAutoDisableTime(tReal inactive_time)
{
	dBodySetAutoDisableTime(m_body, inactive_time);
}


// ### CODEObjSimple
CODESimple::CODESimple(CODESimulation &sim)
:CODEObj(sim), m_geom(0)
{
}

CODESimple::~CODESimple()
{
	if (m_geom)
		dGeomDestroy(m_geom);
}

// ### CODEPrimitive

void CODEPrimitive::RenderODE(IRenderer *r)
{
	r->SetColor(m_color[0], m_color[1], m_color[2], m_color[3]);
	r->DrawBall(1.0f);
}

// ### CODEBall

CODEBall::CODEBall(CODESimulation &sim, tReal radius, tReal mass) 
: CODEPrimitive(sim),
m_radius(radius)
{
	m_geom = ::dCreateSphere(sim.GetSpace(), m_radius);
	dGeomSetBody (m_geom,m_body);
	dMass m;
	m.setSphereTotal(mass, m_radius);
	dBodySetMass(m_body, &m);
	SetColor(1.0f, 0.2f, 0.2f, 1.0f);
}

void CODEBall::RenderODE(IRenderer *r)
{
	r->SetColor(m_color[0], m_color[1], m_color[2], m_color[3]);
	r->DrawBall(m_radius);
}

// ### CODECylinder

CODECylinder::CODECylinder(CODESimulation &sim, tReal radius, tReal length, tReal mass)
: CODEPrimitive(sim), m_radius(radius), m_length(length)
{
	m_geom = dCreateCylinder(sim.GetSpace(), m_radius, m_length);
	dGeomSetBody(m_geom, m_body);
	// initally position it around y axis (middle of the cylinder is at 0.0)
	CQuaternionT qx;
	qx.CreateFromAxisAngle(_R(1.0), 0.0f, 0.0f, -90.0f);
	tReal q[4];
	qx.GetQuaternionW(q);
	dBodySetQuaternion(m_body, q);
	dMass m;
	// was 2
	m.setCylinderTotal(mass, 2, m_radius, m_length);
	dBodySetMass(m_body, &m);
	SetColor(0.3, 0.6f, 0.8f, 1.0f);
}

void CODECylinder::RenderODE(IRenderer *r)
{
	r->SetColor(m_color[0], m_color[1], m_color[2], m_color[3]);
	r->DrawCylinder(m_radius, m_length);
}

// ### CODECube

CODEBox::CODEBox(CODESimulation &sim, tReal x, tReal y, tReal z, tReal mass)
	: CODEPrimitive(sim), m_x(x), m_y(y), m_z(z)
{
	m_geom = dCreateBox(sim.GetSpace(), m_x, m_y, m_z);
	dGeomSetBody(m_geom, m_body);
	dMass m;
	m.setBoxTotal(mass, m_x, m_y, m_z);
	dBodySetMass(m_body, &m);
	SetColor(0.8f, 0.4f, 0.6f, 1.0f);
}

void CODEBox::RenderODE(IRenderer *r)
{
	r->SetColor(m_color[0], m_color[1], m_color[2], m_color[3]);
	r->DrawCube(m_x, m_y, m_z);
}
// ### CODEPlayerBall

CODEPlayerBall::CODEPlayerBall(CODESimulation &sim, tReal radius, float mass)
: CODEBall(sim, radius, mass)
{
	SetColor(1.3f, 0.36f, 0.86f, 1.0f);
}

void CODEPlayerBall::Throw(const Vector3dT &force)
{
	dBodySetLinearVel(m_body, force.x, force.y, force.z);
}

void CODEPlayerBall::RenderODE(IRenderer *r)
{
	r->SetColor(m_color[0], m_color[1], m_color[2], m_color[3]);
	r->DrawBall(m_radius);
}