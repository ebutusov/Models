#include "StdAfx.h"
#include "Moveables.h"

// orbiter
CSimOrbiter::CSimOrbiter(const Vector3d &center, const Vector3d &start)
:m_center(center),
 m_start_pos(start),
 m_angle(0.0f)
{
	m_pos = m_start_pos;
}

CSimOrbiter::~CSimOrbiter(void)
{
}

void CSimOrbiter::Calc(float total, float dt)
{
	m_angle += 2 * M_PI * m_speed * dt;
	if (m_angle > 2 * M_PI)
		m_angle = m_angle - 2 *M_PI;
	m_pos.x = (m_start_pos.x-m_center.x)*cos(m_angle) - (m_start_pos.z-m_center.z)*sin(m_angle) + m_center.x;
	m_pos.z = (m_start_pos.z-m_center.z)*cos(m_angle) + (m_start_pos.x-m_center.x)*sin(m_angle) + m_center.z;
	//m_pos.y = (m_start_pos.y-m_center.y)*cos(m_angle) - (m_start_pos.y-m_center.y)*sin(m_angle) + m_center.y;
}
