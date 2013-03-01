#include "StdAfx.h"
#include "SimObj.h"
#include "Simulation.h"

CSimObj::CSimObj()
:m_id(-1)
{
}

void CSimObj::Frame(float total, float dt)
{	
}

void CSimObj::Render(IRenderer *r)
{
}


void CSimObjGL::Render(IRenderer *r)
{
	r->Begin();
		RenderGL(r);
	r->End();
	RenderGLEx(r);
}

void CSimObjGL::RenderGL(IRenderer *r)
{
}

void CSimObjGL::RenderGLEx(IRenderer *r)
{

}