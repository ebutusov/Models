#pragma once
#include "Vectors.h"

interface IRenderer
{
	virtual void Begin()=0;
	virtual void End()=0;
	virtual void Translate(const Vector3df &vect)=0;
	virtual void Rotate(const Vector3df &vect, float angle)=0;
	virtual void SetColor(float r, float g, float b, float a)=0;
	virtual void SetBlend(bool enable)=0;
	virtual void SetIdentity()=0;

	virtual void MatrixMultiply(float *m4x4)=0;

	virtual void DrawBall(float r)=0;
	virtual void DrawCylinder(float r, float l)=0;
	virtual void DrawCCylinder(float r, float l)=0;
	virtual void DrawCone(float r1, float r2, float l)=0;
	virtual void DrawCube(float x, float y, float z)=0;

};

