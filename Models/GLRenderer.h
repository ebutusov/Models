#pragma once
#include "irenderer.h"

class CGLRenderer : public IRenderer
{
private:
	float m_light_pos[4], m_light_amb[4], 
				m_light_dif[4], m_light_spc[4];

public:
	CGLRenderer(void);
	~CGLRenderer(void);

	void Init();

	virtual void Begin();
	virtual void End();
	virtual void Translate(const Vector3df &vect);
	virtual void Rotate(const Vector3df &vect, float angle);
	virtual void SetColor(float r, float g, float b, float a);
	virtual void MatrixMultiply(float *m4x4);

	virtual void SetBlend(bool enable);
	virtual void SetIdentity();

	virtual void DrawBall(float r);
	virtual void DrawCylinder(float r, float l);
	virtual void DrawCone(float r1, float r2, float l);
	virtual void DrawCCylinder(float r, float l);
	virtual void DrawCube(float x, float y, float z);
};
