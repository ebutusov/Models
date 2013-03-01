#pragma once

#include "SimObj.h"
#include "vect.h"
#include <vector>


class CMovePath
{
private:
	typedef std::vector<Vector3d> vect_t;
	typedef vect_t::iterator iter_t;
	vect_t m_path;
public:
	void AddNode(const Vector3d &node);
	const Vector3d & GetNode(int i) const;
	int Count() const { return m_path.size(); }
};

class CSimObjMoveable : public CSimObj
{
protected:
	CMovePath m_path;
	int m_next_node;
	float m_speed;
public:
	CSimObjMoveable(void);
	virtual ~CSimObjMoveable(void);

	void AddNode(const Vector3d &v);
	void SetSpeed(float speed) { m_speed = speed; }
	float GetSpeed() { return m_speed; }

	virtual void Calc(float total, float dt);
};
