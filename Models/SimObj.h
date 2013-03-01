#pragma once
#include "Real.h"
#include "IRenderer.h"
#include <ode/ode.h>
#include "Concept.h"

class CSimObj : ConceptNoCopy
{
private:
	int m_id;
	void *m_tag;
public:
	CSimObj();
	virtual ~CSimObj() {};
	
	int GetID() { return m_id; }
	void SetID(int id) { m_id = id; } 

	void SetTagData(void *tag) { m_tag = tag; }
	void* GetTagData() { return m_tag; } 

	virtual void Frame(float total, float dt);
	virtual void Render(IRenderer *r);
};

class CSimObjDisposable : public CSimObj
{
private:
	bool m_disposed;
public:
	CSimObjDisposable() 
		: m_disposed(false) {};
	bool IsDisposed() { return m_disposed; }
	void Dispose() { OnDispose(); m_disposed = true; };
private:
	virtual void OnDispose() {};
};

class CSimObjGL : public CSimObjDisposable
{
public:
	virtual void Render(IRenderer *r);
protected:
	virtual void RenderGL(IRenderer *r);
	virtual void RenderGLEx(IRenderer *r);
};