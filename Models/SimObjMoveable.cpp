#include "StdAfx.h"
#include "SimObjMoveable.h"

// move paths

void CMovePath::AddNode(const Vector3d &node)
{
	m_path.push_back(node);
}

const Vector3d& CMovePath::GetNode(int i) const
{
	return m_path.at(i);
}

// CSimObjMoveable

CSimObjMoveable::CSimObjMoveable(void) : m_speed(0.0f)
{
	m_next_node = 0;
}

CSimObjMoveable::~CSimObjMoveable(void)
{

}

bool Reached(Vector3d &u, Vector3d &v)
{
	static const float tol = 0.1f;
	return fabs(u.x - v.x) < tol &&
			fabs(u.y - v.y) < tol &&
			fabs(u.z - v.z) < tol;
}

void CSimObjMoveable::AddNode(const Vector3d &v)
{
	m_path.AddNode(v);
	m_pos = m_path.GetNode(0);
	if (m_path.Count() > 1)
	{
		m_next_node = 1;
		m_dir = m_path.GetNode(1);
	}
}

void CSimObjMoveable::Calc(float total, float dt)
{
	Vector3d v = (m_dir-m_pos);
	v.Normalize();
	m_pos.x += v.x * m_speed * dt;
	m_pos.y += v.y * m_speed * dt;
	m_pos.z += v.z * m_speed * dt;

	if (Reached(m_dir, m_pos))
	{
		m_pos = m_dir;
		if (m_path.Count() > 1)
		{ 
			m_next_node = (m_next_node + 1) % m_path.Count();
			m_dir = m_path.GetNode(m_next_node);
		}
	}
	//m_pos += v * m_speed * dt;
}
