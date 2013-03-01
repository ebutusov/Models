#include "StdAfx.h"
#include "MySimObjects.h"



CCylinderAD::CCylinderAD(CMySimulation &sim, tReal radius, tReal length, tReal mass)
	:CODECylinder(sim, radius, length, mass), m_fallen_time(0.0f)
{
}

void CCylinderAD::Frame(tReal total, tReal dt)
{
	dVector3 pos, pos2;
	// those are given in geom's local coords (and cylinders are aligned along Z+ axis)
	// the output is in world coordinates (we're using Y+ as up direction)
	dBodyGetRelPointPos(m_body, 0.0f, 0.0f, 0.0f, pos);
	dBodyGetRelPointPos(m_body, 0.0f, 0.0f, m_length, pos2);
	if (fabs(pos[1] - pos2[1]) < 1.0f)
	{
		// we're on the ground, check for movement
		const dReal *vel = dBodyGetLinearVel(m_body);
		Vector3df v(vel);
		if (v.Magnitude() < 0.5f)
		{
			dBodySetForce(m_body, 0.0f, 0.0f, 0.0f);
			dBodySetTorque(m_body, 0.0f, 0.0f, 0.0f);

			// let's check for how long it is lying
			if (m_fallen_time > 0.0f)
			{
				SetColor(1.0f, 0.0f, 0.0f, 1.0f); 
				tReal duration = total - m_fallen_time;
				if (duration > 5.0f)
					Dispose();	// lying on the ground for 10s, get rid of it
			}
			else
				m_fallen_time = total;
		}
		else
		{
			SetColor(1.0f, 0.0f, 1.0f, 1.0f);
			m_fallen_time = 0.0f;	// since it's moving, don't delete it
		}
	}
}

// ### CPlayerBallAD

CPlayerBallAD::CPlayerBallAD(CMySimulation &sim, tReal radius, tReal mass)
	: CODEPlayerBall(sim, radius, mass), m_inact_time(0.0f), m_sim(&sim)
{
}

void CPlayerBallAD::Frame(tReal total, tReal dt)
{
	const dReal *vel = dBodyGetLinearVel(m_body);
	Vector3df v(vel);
	if (v.Magnitude() < 0.5f)
	{
		// let's check for how long it is lying
		if (m_inact_time > 0.0f)
		{
			tReal duration = total - m_inact_time;
			if (duration > 5.0f)
				Dispose();	// just lying and wasting memory, get rid of it...
		}
		else
			m_inact_time = total;
	}
	else
		m_inact_time = 0.0f;	// since it's moving, don't delete it yet
}

void CPlayerBallAD::RenderGLEx(IRenderer *r)
{
	// TODO: optimize
	static const tReal min_mag = _R(0.0), max_mag = _R(10.0),
		mag_upper = _R(5.0), mag_lower = _R(2.0);

	if (!m_sim->GetShowVectors()) return;

	const dReal *pos = dBodyGetPosition(m_body);
	const dReal *vel = dBodyGetLinearVel(m_body);
	Vector3df v(vel), p(pos);
	float vmag = v.Magnitude(); // mag - min / max_mag - min_mag
	if (vmag > max_mag) vmag = max_mag;
	tReal mag = (mag_upper - mag_lower) * ((vmag - min_mag) / (max_mag - min_mag));
	Vector3df dir = v;
	dir.Normalize();
	r->Begin();
	float gamma = dir.GetGamma();
	r->Translate(p);
	r->Rotate(Vector3df(0.0f, 1.0f, 0.0f), 90-gamma);
	r->Rotate(Vector3df(1.0f, 0.0f, 0.0f), dir.GetAlfa()-90);
	r->Translate(Vector3df(0.0f, 0.0f, mag/2.0f));
	r->DrawCone(0.1f, 0.0f, mag);
	r->End();
}

// ### CThrowBox

CThrowBox::CThrowBox(CMySimulation &sim, tReal x, tReal y, tReal z, tReal mass)
:CODEBox(sim, x, y, z, mass)
{}

void CThrowBox::Throw(const Vector3dT &velocity)
{
	dBodySetLinearVel(m_body, velocity.x, velocity.y, velocity.z);
}

