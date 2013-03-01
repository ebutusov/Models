#pragma once
#include <ode/ode.h>
#include <list>
#include "Simulation.h"
#include "ODEObj.h"

class CODESimulation : public CSimulationGL
{
protected:
	typedef std::list<CODEObj *> cont_t;
	typedef cont_t::iterator cont_iter;
	cont_t m_objects;

	dWorldID m_world_id;
	dSpaceID m_space_id;
	dJointGroupID m_jgroup;
	float m_speed_factor;

public:
	CODESimulation();
	virtual ~CODESimulation();
	void AddObject(CODEObj *obj);

	virtual void Draw();
	virtual void Init();

	dWorldID GetWorld() { ATLASSERT(m_world_id); return m_world_id; }
	dSpaceID GetSpace() { ATLASSERT(m_space_id); return m_space_id; }
	dJointGroupID GetJointGroup() { ATLASSERT(m_space_id); return m_jgroup; }

	CODEObj* FindObject(int id);
	void RemoveObject(int id);
	void RemoveObject(CODEObj *obj);
	void RemoveAll();

	void SetSpeedFactor(float f);

	int GetObjectsCount() const;

private:
	virtual void OnFrame(tReal total, tReal dt);
protected:
	virtual void OnODEFrame(tReal total, tReal dt);
	virtual void OnCollide(dGeomID o1, dGeomID o2);
	virtual void OnReset();
	
	friend void nearCallback(void *data, dGeomID o1, dGeomID o2);
};
