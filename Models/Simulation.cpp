#include "StdAfx.h"
#include "Simulation.h"
#include <algorithm>
#include <functional>

CSimulation::CSimulation(void) 
: m_last_frame(0),
m_start_time(0),
m_active(false),
m_paused(true)
{
}

CSimulation::~CSimulation(void)
{
}

void CSimulation::Reset()
{
	m_active = false;
	OnReset();
}

void CSimulation::Start(int time)
{ 
	if (m_active)
		return; // already running, Reset should be called first when restarting
	m_start_time = m_last_frame = time;
	OnStart();
	m_active = true;
	Play();
}

void CSimulation::Pause()
{
	m_paused = true;
}

void CSimulation::Stop()
{
	m_start_time = m_last_frame = 0;
	m_active = false;
}

void CSimulation::Play()
{
	if (m_paused)
	{
		// was started already, adjust start_time
		int now = ::GetTickCount();
		m_start_time += now - m_last_frame;
		m_last_frame = now;
	}
	m_paused = false;
}

void CSimulation::OnStart()
{
}

void CSimulation::OnReset()
{
}


void CSimulation::DoFrame(int time)
{
	if (!m_active || m_paused) 
		return;
	float dt = (float)(time - m_last_frame) / 1000.0f;
	float total = (float)(time - m_start_time) / 1000.0f;
	
	if (dt < 0.001) dt = 0.001;
	OnFrame(total, dt);
	m_last_frame = time;
}

void CSimulation::OnFrame(float total, float dt)
{
}



