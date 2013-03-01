#pragma once
#include "IControlListener.h"
#include "MySimulation.h"
#include "MySimObjects.h"
#include "ModelsView.h"

struct SSettings
{
	int BoxesCountInCircle;
	float ForceOfBall;
};

class CSimController : public IControlListener
{
private:
	CMySimulation m_sim;
	CModelsView *m_view;

	bool m_LPM, m_PPM;
	char m_keys[256];
	bool m_insert_mode;
	bool m_paused;

	int m_last_frame;
	int m_start_time;

	dJointGroupID m_snake;
	CODEObj *m_attached;

	SSettings m_settings;
public:
	CSimController(void);
	virtual ~CSimController(void);

	void SetView(CModelsView *v);
	void Init();
	void DoFrame();
	void StartSimulation();
	void ResetSimulation();

	void SetShowVectors(bool show) { m_sim.SetShowVectors(show); }
	bool GetShowVectors() { return m_sim.GetShowVectors(); }

	SSettings* GetSettings();

	// IControlListener methods
	virtual void OnMouseLDown(int x, int y);
	virtual void OnMouseLUp(int x, int y);
	virtual void OnMouseRDown(int x, int y);
	virtual void OnMouseRUp(int x, int y);
	virtual void OnKeyDown(char key);
	virtual void OnKeyUp(char key);
};
