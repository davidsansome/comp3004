#include "contextstate.h"
#include "texture.h"

ContextState* ContextState::m_activeContext = NULL;

ContextState::ContextState()
	: m_polygonMode(Fill),
	  m_texture(NULL)
{
	setMatrixMode(ProjectionMatrix);
	makeActive();
}


ContextState::~ContextState()
{
}

void ContextState::loadIdentity()
{
	*m_activeMatrix = vl_I;
}

void ContextState::translate(Vec3 d)
{
	Mat4 t
	(
		1.0, 0.0, 0.0, d[0],
		0.0, 1.0, 0.0, d[1],
		0.0, 0.0, 1.0, d[2],
		0.0, 0.0, 0.0, 1.0
	);
	
	*m_activeMatrix = t * *m_activeMatrix;
}

void ContextState::scale(Vec3 d)
{
	Mat4 s
	(
		d[0], 0.0,  0.0,  0.0,
		0.0,  d[1], 0.0,  0.0,
		0.0,  0.0,  d[2], 0.0,
		0.0,  0.0,  0.0,  1.0
	);
	
	*m_activeMatrix = s * *m_activeMatrix;
}

void ContextState::rotate(float radians, Axis axis)
{
	Mat4 r;
	float c = cos(radians);
	float s = sin(radians);
	
	switch (axis)
	{
	case XAxis:
		r = Mat4
		(
			1.0, 0.0, 0.0, 0.0,
			0.0, c,   -s,  0.0,
			0.0, s,   c,   0.0,
			0.0, 0.0, 0.0, 1.0
		);
		break;
	case YAxis:
		r = Mat4
		(
			c,   0.0, s,   0.0,
			0.0, 1.0, 0.0, 0.0,
			-s,  0.0, c,   0.0,
			0.0, 0.0, 0.0, 1.0
		);
		break;
	case ZAxis:
		r = Mat4
		(
			c,   -s,  0.0, 0.0,
			s,   c,   0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		break;
	}
	
	*m_activeMatrix = r * *m_activeMatrix;
}

void ContextState::frustum(float l, float r, float b, float t, float n, float f)
{
	Mat4 fru
	(
		(2.0*n)/(r-l), 0.0, (r+l)/(r-l), 0.0,
		0.0, (2.0*n)/(t-b), (t+b)/(t-b), 0.0,
		0.0, 0.0, -(f+n)/(f-n), -(2*f*n)/(f-n),
		0.0, 0.0, -1.0, 0.0
	);
	
	*m_activeMatrix = fru * *m_activeMatrix;
}

void ContextState::perspective(float fov, float aspect, float zNear, float zFar)
{
	float yMax = zNear * tan(fov * M_PI / 360.0);
	float yMin = -yMax;
	float xMin = yMin * aspect;
	float xMax = yMax * aspect;
	
	frustum(xMin, xMax, yMin, yMax, zNear, zFar);
}

void ContextState::ortho(float l, float r, float b, float t, float n, float f)
{
	Mat4 o
	(
		2.0/(r-l), 0.0, 0.0, -(r+l)/(r-l),
		0.0, 2.0/(t-b), 0.0, -(t+b)/(t-b),
		0.0, 0.0, -2.0/(f-n), -(f+n)/(f-n),
		0.0, 0.0, 0.0, 1.0
	);
	
	*m_activeMatrix = o * *m_activeMatrix;
}

void ContextState::setMatrixMode(MatrixType type)
{
	switch (type)
	{
		case ModelViewMatrix:  m_activeMatrix = &m_modelView;  break;
		case ProjectionMatrix: m_activeMatrix = &m_projection; break;
	}
}


