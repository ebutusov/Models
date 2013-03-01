#pragma once
#include "ODESimulation.h"

enum tagSIMOBJ { eInvalid = -1, eNotUsed = 0, eUserBall = 1 };

class CMySimulation : public CODESimulation
{
private:
	bool m_show_vectors;
public:
	CMySimulation();
	virtual ~CMySimulation();

	virtual void Draw();

	void SetShowVectors(bool show) { m_show_vectors = show; }
	bool GetShowVectors() { return m_show_vectors; }

protected:
	virtual void OnODEFrame(float total, float dt);
	virtual void OnCollide(dGeomID o1, dGeomID o2);
};
