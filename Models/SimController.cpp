#include "StdAfx.h"
#include "SimController.h"
#include "MySimObjects.h"

CSimController::CSimController(void)
:m_view(NULL), 
m_LPM(false), m_PPM(false),
m_insert_mode(false),
m_insert_kind(EInsertKind::insertCylinder),
m_paused(false),
m_last_frame(0),
m_start_time(0)
{
	m_attached = NULL;
	memset(m_keys, 0, sizeof(m_keys));
	m_settings.BoxesCountInCircle = 30;
	m_settings.ForceOfBall = 40.0f;
}

CSimController::~CSimController(void)
{
}

void CSimController::Init()
{
	ATLASSERT(m_view != NULL);
	m_sim.Init();
	m_view->SetSimulation(&m_sim);
	m_snake = ::dJointGroupCreate(0);
}

void CSimController::DoFrame()
{
	DWORD now = GetTickCount();
	const float bps = 5.0f; 
	static DWORD last_thrown = now-10000;

	if (m_LPM)
	{
		// points from cam to view
		Vector3dT dir = m_view->m_cam.mView - m_view->m_cam.mPos;
		Vector3dT st = dir;
		st.Normalize();
		//st.y -= 1.0f;
		//dir = m_view->m_cam.mPos + st;
		if (!m_insert_mode)
		{
			// throw a ball if we can
			if ((now - last_thrown) / _R(1000.0) * bps > 1.0f)
			{
				// move away towards view direction (st*4)
				Vector3dT d = m_view->m_cam.mPos + st*2.0f;
				//int size = (rand()%5)+1;
				CPlayerBallAD *ball = new CPlayerBallAD(m_sim, _R(0.20), _R(0.5));
				ball->SetID(eUserBall);
				ball->SetPosition(d);
				Vector3dT f = m_view->m_cam.mView - m_view->m_cam.mPos;
				f.Normalize();
				// force used to throw the ball
				f *= m_settings.ForceOfBall;
				ball->Throw(f);
				last_thrown = now;
			}
		}
		else
		{
			// insert mode
			float height = 10.0f;
			Vector3dT cp = m_view->m_cam.mPos + (st*5); // move away from camera
			cp.y = max(height/2, cp.y); // stand on ground or fall
			
			switch (m_insert_kind)
			{
			case EInsertKind::insertCylinder:
				{
					CCylinderAD *cyl = new CCylinderAD(m_sim, _R(1.0), height, _R(30.0));
					cyl->SetPosition(cp); 
				}
				break;
			default:
			case EInsertKind::insertBox:
				{
					CODEBox *box = new CODEBox(m_sim, _R(1.0), height, _R(1.0), _R(10.0));
					box->SetPosition(cp);
				}
				break;
			}
			m_LPM = false;
		}
	}
	// it doesn't work like this...
	//if(m_attached)
	//	m_attached->SetPosition(m_view->m_cam.mPos);
	m_view->Frame(now);
	// kick the snake in random direction
	static int last_snake = now - 11000;
	static Vector3dT cp;
	static float an = rand()%360+1;
	
	if (m_attached)
	{
		Vector3dT cv;
		m_attached->GetLinearVel(cv);

		if((now - last_snake > 10000) || cv.Magnitude() < _R(1.0))
		{
			float da = rand()%20+1;
			an += da;
			cp = (Vector3dT(
				sin(an),
				(rand()%10)*_R(0.01),
				cos(an)
			));
			cp.Normalize();
			cp *= _R(15.0);
			CQuaternionT qt(0.0f, 1.0f, 0.0f, an);
			m_attached->SetRotation(qt);
			m_attached->SetLinearVel(cp);
			last_snake = now;
		}
		else
			m_attached->SetLinearVel(cp);

	}
	m_last_frame = ::GetTickCount();
}

void CSimController::ResetSimulation()
{
	m_sim.Stop();
	m_sim.Reset();;
}

void CSimController::StartSimulation()
{
	if (m_sim.IsActive()) 
		return;

	int CYLINDERS = m_settings.BoxesCountInCircle;
	float step = _R(2.0) * M_PI / CYLINDERS;
	float angle = _R(0.0), radius = _R(20.0);

	for (int i=0;i<CYLINDERS;++i)
	{
		//Vector3dT pt(radius * sin(angle), _R(3.5), radius * cos(angle));
		//Vector3dT yv(_R(0.0), _R(1.0), _R(0.0));
		//Vector3dT res = pt;// ^ yv;
		
		
		CODEBox *box = new CODEBox(m_sim, _R(2.0), _R(10.0), _R(2.0), _R(20.0));
		box->SetPosition(Vector3dT(radius * sin(angle), 5.0f, radius * cos(angle)));
		CQuaternionT br;
		br.CreateFromAxisAngle(_R(0.0), _R(1.0), _R(0.0), _R(0.0)+(angle * _R(180.0)/M_PI));
		box->SetRotation(br);
		
		tReal rc = radius + _R(10.0);
		CCylinderAD *cyl = new CCylinderAD(m_sim, 1.0f, 7.0f, 30.0f);
		cyl->SetPosition(Vector3dT(rc * sin(angle), 3.5f, rc * cos(angle))); // 3.5 because (5.0 + 2.0f = 7.0) / 2 - cap's size is radius
		//CQuaternionT rt;
		//cyl->GetRotation(rt);
		//CQuaternionT qt = CQuaternionf::NewFromAxisAndAngle(res.x, res.y, res.z, 0.0f);
		//rt = qt * rt;
		//cyl->SetRotation(rt); 
		//cyl->SetRotation(CQuaternionf::NewFromAxisAndAngle(cos(angle), 0.0f, -sin(angle), -90.0f)); 
		
		/*CODEBall * obj = new CODEBall(m_sim, 1.5f);
		obj->SetMass(1.0f);
		float r = radius + 5.0f;
		obj->SetPosition(Vector3dT(r * sin(angle), 2.0f, r * cos(angle)));
		*/

		//cyl = new CODECylinder(m_sim, 1.0f, 3.0f);
		//cyl->SetPosition(Vector3dT(radius * sin(angle), 8.5f+i*8.5f+6.0f, radius * cos(angle))); // 2.5 + the bottom cap's size (1.0) ???
		//cyl->SetMass(10.0f);

		angle += step;
	}

	CODEBox *box = new CODEBox(m_sim, _R(2.0), _R(10.0), _R(2.0), _R(10.0));
	box->SetPosition(Vector3dT(_R(-5.0), _R(5.0), _R(0.0)));

	box = new CODEBox(m_sim, _R(2.0), _R(10.0), _R(2.0), _R(10.0));
	box->SetPosition(Vector3dT(_R(5.0), _R(5.0), _R(0.0)));

	box = new CODEBox(m_sim, _R(2.0), _R(12.0), _R(2.0), _R(10.0));
	box->SetPosition(Vector3dT(_R(0.0), _R(11.0), _R(0.0)));
	CQuaternionT br;
	br.CreateFromAxisAngle(_R(0.0), _R(0.0), _R(1.0), _R(90.0));
	box->SetRotation(br);

	// snake of chaos
	/*int NUM_BALLS = 20;

	CODEObj *fball = new CODEBox(m_sim, _R(0.6), _R(0.6),_R(0.6),_R(1.0));
	fball->SetPosition(_R(0.0), _R(12.0), _R(-7.5));
	m_attached = fball;
	for (int i=1;i<NUM_BALLS;++i)
	{
		CODEPlayerBall *sball = new CODEPlayerBall(m_sim, _R(0.2), _R(2.0));
		sball->SetPosition(_R(0.0), _R(12.0),_R(-7.5)+i*_R(0.3));
		dJointID jid = ::dJointCreateHinge(m_sim.GetWorld(), m_snake);
		::dJointAttach(jid, fball->GetBodyID(), sball->GetBodyID());
		fball = sball;
	}*/
	m_start_time = m_last_frame = ::GetTickCount();
	m_sim.Start(m_last_frame);
}

void CSimController::SetView(CModelsView *v)
{
	m_view = v;
	v->SetControlListener(this);
}

SSettings* CSimController::GetSettings()
{
	return &m_settings;
}

// IControlListener implementation

void CSimController::OnMouseLDown(int x, int y)
{
	m_LPM = true;
}

void CSimController::OnMouseLUp(int x, int y)
{
	m_LPM = false;
}

void CSimController::OnMouseRDown(int x, int y)
{
	m_PPM = true;
	if (m_paused)
		m_sim.Play();
	else
		m_sim.Pause();
	m_paused = !m_paused;
}

void CSimController::OnMouseRUp(int x, int y)
{
	m_PPM = false;
}

#define ON_KEY_ONCE(x) if(m_keys[x]) { m_keys[x] = false;
#define END }

void CSimController::OnKeyDown(char key)
{
	m_keys[key] = true;

	ON_KEY_ONCE('I')
		m_insert_mode = !m_insert_mode;
	END
	
	ON_KEY_ONCE('B')
		m_insert_kind = EInsertKind::insertBox;
	END

	ON_KEY_ONCE('C')
		m_insert_kind = EInsertKind::insertCylinder;
	END

	ON_KEY_ONCE('R')
		m_sim.RemoveAll();
	END
}

void CSimController::OnKeyUp(char key)
{
	m_keys[key] = false;
}

