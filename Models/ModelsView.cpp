// ModelsView.cpp : implementation of the CModelsView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "msModelRenderer.h"
#include "ModelsView.h"
#include "Vectors.h"
#include "MySimObjects.h"
#include "ODEObj.h"

#define TIMER_MOVE 1

CModelsView::CModelsView()
	: m_drag(false),
	m_camera_speed(1.0f), // units per second
	m_LPM(false),
	m_sink(NULL)
{
	memset(m_keys, 0, sizeof(m_keys));
}

BOOL CModelsView::PreTranslateMessage(MSG* pMsg)
{
	BOOL redrawScene = FALSE;

	switch(pMsg->message)
	{
		case WM_KEYUP:
			{
				m_keys[pMsg->wParam] = FALSE;
				redrawScene = TRUE;
				if (m_sink)
					m_sink->OnKeyUp(pMsg->wParam);
				break;
			}
		case WM_KEYDOWN:
			{
				m_keys[pMsg->wParam] = TRUE;
				redrawScene = TRUE;
				if (m_sink)
					m_sink->OnKeyDown(pMsg->wParam);
				break;
			}
		default:
			break;
	}
	if(redrawScene)
	{
		if (m_keys[VK_SPACE])
		{
			m_keys[VK_SPACE] = false;
			CaptureReleaseMouse();
		}
		FrameNow();
		return TRUE;
	}
	return FALSE;
}

void CModelsView::OnActivate(bool activate)
{
	if (!activate && m_drag)
		CaptureReleaseMouse();
}


void CModelsView::CaptureReleaseMouse()
{
	if (!m_drag) 
	{
		RECT rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		SetCursorPos(rc.left+(rc.right-rc.left)/2, rc.top+(rc.bottom-rc.top)/2);
		ShowCursor(FALSE);
		SetCapture();
		m_drag = true;
	}
	else
	{
		m_drag = false;
		::ReleaseCapture();
		ShowCursor(TRUE);
	}
}


LRESULT CModelsView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_LPM = true;
	if (m_sink)
		m_sink->OnMouseLDown(0,0);
	FrameNow();
	bHandled = TRUE;
	return 0;
}

LRESULT CModelsView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_LPM = false;
	if (m_sink)
		m_sink->OnMouseLUp(0,0);
	FrameNow();
	bHandled = TRUE;
	return 0;
}

LRESULT CModelsView::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_sink)
		m_sink->OnMouseRDown(0,0);
	FrameNow();
	bHandled = TRUE;
	return 0;
}

LRESULT CModelsView::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_sink)
		m_sink->OnMouseRUp(0,0);
	FrameNow();
	bHandled = TRUE;
	return 0;
}

void CModelsView::ProcessMouseEvents()
{
	if (m_drag == false) return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	int x_pos = pt.x;
	int y_pos = pt.y;
	
	RECT rc;
	GetClientRect(&rc);

	int dx = rc.right/2 - x_pos;
	int dy = rc.bottom/2 - y_pos;

	m_cam.MouseRotate(dx, dy, TRUE);
	ClientToScreen(&rc);
	SetCursorPos(rc.left+(rc.right-rc.left)/2, rc.top+(rc.bottom-rc.top)/2);
	//ATLTRACE("in MouseMove %d (%d,%d)\n", GetTickCount(), x_pos, y_pos);
}


LRESULT CModelsView::OnMouseWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	zDelta /= WHEEL_DELTA;
	float stand = zDelta * 0.3f;
	m_cam.SetStand(stand);
	FrameNow();
	bHandled = TRUE;
	return 0;
}

void CModelsView::OnInit()
{
	m_renderer.Init();

	m_cam.PositionCamera(-50.0f, 2.0f, 0.0f, 
											 -45.0f, 2.0f, 0.0f, 
											 0.0f, 1.0f, 0.0f);
	glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
	
	if(!CTextureLoader::LoadTextureToList(MAKEINTRESOURCE(IDB_FLOOR), &m_floor_texture, true))
		::MessageBox(NULL, _T("Failed to load texture"), _T("Error"), MB_OK | MB_ICONERROR);

	m_last_frame = ::GetTickCount();
}

void CModelsView::FrameNow()
{
	Frame(::GetTickCount());
}

void CModelsView::Frame(DWORD time)
{
	DWORD now = time;
	float ftime = (float) (now - m_last_frame) / 1000.0f;
	const float bps = 10.0f; 
	static DWORD last_thrown = now-10000;

	m_xsim->DoFrame(now);
	ProcessMouseEvents();

	if(m_keys[VK_UP] || m_keys['W'])
		m_cam.FlyCamera(m_camera_speed * ftime);
	else if(m_keys[VK_DOWN] || m_keys['S'])
		m_cam.FlyCamera(-m_camera_speed * ftime);
	
	if(m_keys[VK_RIGHT] || m_keys['D'])
		m_cam.StrafeCamera(m_camera_speed * ftime);
	else if(m_keys[VK_LEFT] || m_keys['A'])
		m_cam.StrafeCamera(-m_camera_speed * ftime);

	if (m_keys[VK_SHIFT])
		m_camera_speed = 4.0f;
	else
		m_camera_speed = 2.0f;

	/*float speed = 1.0f;
	for(miter_t m=m_models.begin();m!=m_models.end();++m)
	{
		msModel *model = *m;
		float current = model->GetCurrentFrame() + (ftime * speed) * model->GetAnimationFps();
		if (current > model->GetTotalFrames())
			current = 0.0f;
		model->SetFrame(current);
	}*/
	m_last_frame = GetTickCount();
	RedrawWindow();
}

void CModelsView::OnRender()
{
	static float d = 100.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(m_cam.mPos.x, m_cam.mPos.y, m_cam.mPos.z,
			m_cam.mView.x, m_cam.mView.y, m_cam.mView.z,
			m_cam.mUp.x, m_cam.mUp.y, m_cam.mUp.z);

	// draw ground

	glEnable(GL_TEXTURE_2D);	
	glCallList(m_floor_texture);
	glBegin(GL_QUADS);
		glNormal3f(0,1.0f,0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(d, 0.0f ,-d);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-d, 0.0f, -d);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-d, 0.0f, d);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(d, 0.0f, d);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	//float i = 0.0f;
	//float x = 0.0f, z = 0.0f;
	//float r = 20.0f;
	//float step = 2 * M_PI / m_models.size();

	//for (miter_t m = m_models.begin();m != m_models.end();++m)
	//{
	//	glPushMatrix();
	//		glTranslatef(r*sin(i), 0.0f, r*cos(i));
	//		glRotatef(180.0f + (i * 180.0f/M_PI), 0.0f, 1.0f, 0.0f);
	//		msModelRenderer renderer(*m);
	//		renderer.Render(true, false);
	//	glPopMatrix();
	//	i += step;
	//}

	GLfloat color[4] = { 0.4f, 0.7f, 0.2f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	m_xsim->Draw();

	glFlush();	
}

LRESULT CModelsView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	return 0;
}



LRESULT CModelsView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	srand(::GetTickCount());

	// load objects
	/*for (int i=0;i<10;++i)
	{
		msModel *m = new msModel();
		ATLVERIFY(m->Load("man.ms3d"));
		m->SetupJoints();
		m->SetFrame(-1.0f);
		m_models.push_back(m);
	}*/

	::glShadeModel(GL_SMOOTH);
	bHandled = FALSE; // pass message to the base class
	return 0;
}

void CModelsView::OnResize(int cx, int cy)
{
	GLfloat fFovy  = 50.0f; // Field-of-view
	GLfloat fZNear = 1.0f;  // Near clipping plane
	GLfloat fZFar = 200.0f;  // Far clipping plane

	// Calculate viewport aspect
	RECT rc;
	GetClientRect(&rc);

	GLfloat fAspect = (GLfloat)(rc.right-rc.left) / (GLfloat)(rc.bottom-rc.top);

	// Define viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fFovy, fAspect, fZNear, fZFar);
	glViewport(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
	glMatrixMode(GL_MODELVIEW);
	FrameNow();
}

void CModelsView::SetSimulation(CSimulationGL *sim)
{
	m_xsim = sim;
	m_xsim->SetRenderer(&m_renderer);
}
