#pragma once
#include "SimObjMoveable.h"
#include "vect.h"

class CSimOrbiter : public CSimObjMoveable
{
private:
	Vector3d m_center, m_start_pos;
	float m_angle;
public:
	CSimOrbiter(const Vector3d &center, const Vector3d &pos);
	virtual ~CSimOrbiter(void);

	virtual void Calc(float total, float dt);
};
