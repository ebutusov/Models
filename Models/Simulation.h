#pragma once
#include "SimObj.h"
#include "Concept.h"

class CSimulation : ConceptNoCopy
{
private:
	int m_start_time,
			m_last_frame;
	bool m_active, m_paused;
	bool m_initialized;
public:
	CSimulation();
	virtual ~CSimulation();

	void Start(int start_time);
	void Pause();
	void Play();
	void Stop();
	void Reset();
	bool IsActive() { return m_active; }
	bool IsPaused() { return m_paused; }

	void DoFrame(int time);

	virtual void Draw() {};
	virtual void Init() {};

protected:
	virtual void OnFrame(float total, float dt);
	virtual void OnStart();
	virtual void OnReset();
};

class CSimulationGL : public CSimulation
{
protected:
	IRenderer *m_renderer;
public:
	void SetRenderer(IRenderer *r) { m_renderer = r; }
	virtual void Draw()=0;
};

