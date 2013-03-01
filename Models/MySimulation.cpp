#include "StdAfx.h"
#include <functional>
#include <algorithm>
#include <vector>
#include "MySimulation.h"
#include "MySimObjects.h"

CMySimulation::CMySimulation(void) :
	m_show_vectors(true)
{
}

CMySimulation::~CMySimulation(void)
{
}

CPlayerBallAD* AsPlayerBall(dBodyID id)
{
	if (id == 0) return NULL;
	CODEObj *ode = reinterpret_cast<CODEObj*>(dBodyGetData(id));
	CPlayerBallAD *ball = NULL;
	if (ode->GetID() == eUserBall)
	ball = static_cast<CPlayerBallAD *>(ode);
	return ball;
}

CODEPrimitive* AsPrimitive(dBodyID id)
{
	if (id == 0) return NULL;
	CODEObj *ode = reinterpret_cast<CODEObj*>(dBodyGetData(id));
	return dynamic_cast<CODEPrimitive *>(ode);
}

CODECylinder* AsCylinder(dBodyID id)
{
	if (id == 0) return NULL;
	CODEObj *ode = reinterpret_cast<CODEObj*>(dBodyGetData(id));
	return dynamic_cast<CODECylinder *>(ode);
}

CODEBox* AsBox(dBodyID id)
{
	if (id == 0) return NULL;
	CODEObj *ode = reinterpret_cast<CODEObj*>(dBodyGetData(id));
	return dynamic_cast<CODEBox *>(ode);
}

void CMySimulation::OnCollide(dGeomID o1, dGeomID o2)
{
	dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);

	if (b1 && b2 && ::dAreConnected(b1, b2))
		return; // don't collide joined objects

	const int N = 32;
  dContact contacts[N];
	if (int numc = dCollide(o1,o2,N,&(contacts[0].geom),sizeof(dContact))) 
	{
		dGeomID other = o2;
		CPlayerBallAD *ball = AsPlayerBall(b1);
		if (!ball)
		{
			other = o1;
			ball = AsPlayerBall(b2);
		}
		
		if (ball)
		{
			int what = dGeomGetClass(other);
			dBodyID otherb = dGeomGetBody(other);
			CODEPrimitive *prim = AsPrimitive(otherb);
			if (prim)
			{
				const float *col = prim->GetColor();
				ball->SetColor(col);
			}
		}

		for (int i=0; i<numc; i++) 
    {
			contacts[i].surface.mode = 0;
			contacts[i].surface.mode = dContactBounce | dContactSoftCFM;
			contacts[i].surface.mu = dInfinity;
			// bounce is the amount of "bouncyness".
			contacts[i].surface.bounce = 0.4;
			// bounce_vel is the minimum incoming velocity to cause a bounce
			contacts[i].surface.bounce_vel = 1.;
			// constraint force mixing parameter
			contacts[i].surface.soft_cfm = 0.001; // 0.001  
      contacts[i].surface.mu = 50.0; // was: dInfinity
			dJointID c = dJointCreateContact (m_world_id,m_jgroup,&contacts[i]);
			dJointAttach(c, dGeomGetBody(contacts[i].geom.g1), dGeomGetBody(contacts[i].geom.g2));
		}
  }
}

void CMySimulation::OnODEFrame(float total, float dt)
{
	CODESimulation::OnODEFrame(total, dt);

	std::vector<CODEObj*> remove;

	for (cont_iter v = m_objects.begin();v!=m_objects.end();++v)
	{
		CODEObj *obj = *v;

		// remove disposed objects.
		if (obj->IsDisposed())
		{
			remove.push_back(obj);
			continue;
		}
		Vector3dT pos;
		obj->GetPosition(pos);
		if (fabs(pos.x) > 100.0f || fabs(pos.y) > 100.0f || fabs(pos.z) > 100.0f)
			remove.push_back(obj);
	}
	for(std::vector<CODEObj*>::const_iterator it = remove.begin();it!=remove.end();++it)
		RemoveObject(*it);
	remove.clear();
}

void CMySimulation::Draw()
{
	CODESimulation::Draw();

	// draw crosshair (transparent ball)
	if (m_renderer)
	{
		m_renderer->SetIdentity();
		m_renderer->SetBlend(true);
			m_renderer->Translate(Vector3df(0.0f, 0.0f, -2.0f));
			m_renderer->SetColor(1.0f, 1.0f, 0.0f, 0.5f);
			m_renderer->DrawBall(0.05f);
		m_renderer->SetBlend(false);
	}
}