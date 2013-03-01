#include "StdAfx.h"
#include "ODESimulation.h"

CODESimulation::CODESimulation(void)
:m_world_id(0), m_space_id(0), m_jgroup(0), m_speed_factor(1.0f)
{

}

CODESimulation::~CODESimulation()
{
	for (cont_iter v = m_objects.begin(); v != m_objects.end(); ++v)
		delete *v;
	m_objects.clear();

	if (m_world_id)
		dWorldDestroy(m_world_id);

	if (m_space_id)
		dSpaceDestroy(m_space_id);

	if (m_jgroup)
		dJointGroupDestroy(m_jgroup);
}

void CODESimulation::Init()
{
	// protect from multiple initializations
	ATLASSERT(m_world_id == 0);
	if (!m_world_id)
	{
		m_world_id = dWorldCreate();
		m_space_id = dHashSpaceCreate(0);
		dHashSpaceSetLevels(m_space_id,8 , 32); // XXX
		dWorldSetGravity(m_world_id, 0.0, -9.81, 0.0);
		dWorldSetCFM (m_world_id,1e-5);
		dCreatePlane (m_space_id,0,1,0,0);
		m_jgroup = dJointGroupCreate (0);
	}
}

void CODESimulation::OnReset()
{
	for (cont_iter v = m_objects.begin(); v != m_objects.end(); ++v)
		delete *v;
	m_objects.clear();
}

void CODESimulation::Draw()
{
	if (!m_renderer)
		return;
	for (cont_iter v = m_objects.begin(); v != m_objects.end(); ++v)
		(*v)->Render(m_renderer);
}

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	CODESimulation *sim = static_cast<CODESimulation*>(data);
	sim->OnCollide(o1, o2);
}

void CODESimulation::OnCollide(dGeomID o1, dGeomID o2)
{
	ATLASSERT(FALSE);
	dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);

	const int N = 32;
  dContact contacts[N];
	if (int numc = dCollide (o1,o2,N,&(contacts[0].geom),sizeof(dContact))) 
	{
		for (int i=0; i<numc; i++) 
    {
			contacts[i].surface.mode = 0;
			contacts[i].surface.mode = dContactBounce | dContactSoftCFM;
			contacts[i].surface.mu = dInfinity;
			// bounce is the amount of "bouncyness".
			contacts[i].surface.bounce = _R(0.9);
			// bounce_vel is the minimum incoming velocity to cause a bounce
			contacts[i].surface.bounce_vel = _R(0.1);
			// constraint force mixing parameter
			contacts[i].surface.soft_cfm = _R(0.001); // 0.001  
      contacts[i].surface.mu = _R(50.0); // was: dInfinity
			dJointID c = dJointCreateContact (m_world_id,m_jgroup,&contacts[i]);
			//dJointAttach (c,b1,b2);
			dJointAttach(c, dGeomGetBody(contacts[i].geom.g1), dGeomGetBody(contacts[i].geom.g2));
		}
  }
}

void CODESimulation::OnFrame(tReal total, tReal dt)
{
	static tReal ss = _R(0.01); //  step size 0.01f - 10ms steps should be ok (100 steps/sec)

	int iters = ceil(dt/ss);	// XXX
	ATLTRACE("iters: %d\n", iters);
	for(int i=0;i<iters;++i)
	{
		dSpaceCollide (m_space_id,this,&nearCallback);
		dWorldQuickStep(m_world_id, ss);
		// allow all objects update their state
		dJointGroupEmpty (m_jgroup);
	}
	OnODEFrame(total, dt);
	// this seems to be a bit slow sometimes.
	//::dWorldStepFast1(m_world_id, dt, 10);
}

void CODESimulation::OnODEFrame(tReal total, tReal dt)
{
	for (cont_iter v = m_objects.begin(); v != m_objects.end(); ++v)
	{
		CSimObj *obj = *v;
			if (m_renderer)
			obj->Frame(total, dt);
	}
}

void CODESimulation::AddObject(CODEObj *obj)
{
	m_objects.push_back(obj);
}

CODEObj* CODESimulation::FindObject(int id)
{
	for (cont_iter v=m_objects.begin();v!=m_objects.end();++v)
		if ((*v)->GetID() == id)
			return *v;
	return NULL;
}

void CODESimulation::RemoveObject(CODEObj *obj)
{
	m_objects.remove(obj);
	delete obj;
}

void CODESimulation::RemoveObject(int id)
{
	cont_iter found = cont_iter();
	for (cont_iter v=m_objects.begin();v!=m_objects.end();++v)
		if ((*v)->GetID() == id)
		{
			found = v;
			break;
		}
	if (found != cont_iter())
	{
		delete *found;
		m_objects.erase(found);
	}
}

int CODESimulation::GetObjectsCount() const
{
	return m_objects.size();
}

void CODESimulation::RemoveAll()
{
	for (cont_iter v = m_objects.begin(); v != m_objects.end(); ++v)
		delete *v;
	m_objects.clear();
}