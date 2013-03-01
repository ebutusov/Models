#include "StdAfx.h"
#include "GLRenderer.h"

CGLRenderer::CGLRenderer(void)
{
	
}

CGLRenderer::~CGLRenderer(void)
{
}

void Set4f(float *p, float a, float b, float c, float d)
{
	p[0] = a; p[1] = b;
	p[2] = c; p[3] = d;
}

void CGLRenderer::Init()
{	
	// some GL settins
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// lights
	Set4f(m_light_pos, -1.0f, 0.4f, 0.9f, 0.0f); 
	Set4f(m_light_amb, 0.0f, 0.0f, 0.0f, 1.0f);
  Set4f(m_light_dif, 0.5f, 0.5f, 0.5f, 1.0f);
  Set4f(m_light_spc, 0.3f, 0.3f, 0.3f, 1.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, m_light_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  m_light_amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  m_light_dif);
  //glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// background color
	glClearColor(0.3f, 0.5f, 0.7f, 1.0f);

	// texturing
	glEnable(GL_TEXTURE_2D);

}

void CGLRenderer::Begin() 
{
	glPushMatrix();
}

void CGLRenderer::End()
{
	glPopMatrix();
}

void CGLRenderer::Translate(const Vector3df &vect)
{
	glTranslatef(vect.x, vect.y, vect.z);
}

void CGLRenderer::Rotate(const Vector3df &vect, float angle)
{
	glRotatef(angle, vect.x, vect.y, vect.z);
}

void CGLRenderer::MatrixMultiply(float *m4x4)
{
	glMultMatrixf(m4x4);
}


void CGLRenderer::SetColor(float r, float g, float b, float a)
{
	float m[4] = { r, g, b, a };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m);
}

void CGLRenderer::DrawBall(float r)
{
	GLUquadric *q = ::gluNewQuadric();
	glPushMatrix();
		gluSphere(q, r, 10, 10);
		gluDeleteQuadric(q);
	glPopMatrix();
}

void CGLRenderer::DrawCylinder(float r, float l)
{
	DrawCone(r, r, l);
}

void CGLRenderer::DrawCone(float r1, float r2, float l)
{
	GLUquadric *q = ::gluNewQuadric();
	GLUquadric *d = ::gluNewQuadric();
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -l/2.0f);
		//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		gluCylinder(q, r1, r2, l, 10, 10);
		//glPushMatrix();
			if (r1 > 0.1f)
				::gluDisk(d, 0, r1, 10, 10);
			if (r2 > 0.1f)
			{
				glTranslatef(0.0f, 0.0f, l);
				::gluDisk(d, 0, r2, 10, 10);
			}
		//glPopMatrix();
	glPopMatrix();
	gluDeleteQuadric(q);
	gluDeleteQuadric(d);
}

void CGLRenderer::DrawCCylinder(float r, float l)
{
	GLUquadric *q = ::gluNewQuadric();
	GLUquadric *d = ::gluNewQuadric();
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -(l+2*r)/2.0f);
		//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		gluCylinder(q, r, r, l+2*r, 10, 10);
		glPushMatrix();
			::gluDisk(d, 0, r, 10, 10);
			glTranslatef(0.0f, 0.0f, (l+2*r));
			::gluDisk(d, 0, r, 10, 10);
		glPopMatrix();
	glPopMatrix();
	gluDeleteQuadric(q);
	gluDeleteQuadric(d);
}

void CGLRenderer::DrawCube(float x, float y, float z)
{
	float x1,x2,y1,y2,z1,z2;
	
	x1 = x/2; x2 = -x1;
	y1 = y/2; y2 = -y1;
	z1 = z/2; z2 = -z1;

	// back
	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(x2, y2, z2);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y1, z2);
	glVertex3f(x2, y1, z2);
	glEnd();

	// front
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(x2, y2, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y2, z1);
	glEnd();

	// right
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x1, y2, z2);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y1, z1);
	glVertex3f(x1, y1, z2);
	glEnd();

	// left
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y1, z2);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y2, z1);
	glEnd();
	
	// bottom 
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, z2);
	glVertex3f(x2, y2, z2);
	glVertex3f(x2, y2, z1);
	glEnd();

	// top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y1, z1);
	glVertex3f(x2, y1, z2);
	glVertex3f(x1, y1, z2);
	glEnd();
	glPopMatrix();
}

void CGLRenderer::SetIdentity()
{
	glLoadIdentity();
}

void CGLRenderer::SetBlend(bool enable)
{
	if (enable)
	{	
		glEnable (GL_BLEND); 
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable (GL_BLEND);
}