#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <math.h>
#include <iostream>

#include "engine.h"
#include "sphere.h"
#include "cone.h"
#include "drawable.h"
#include "utilities.h"

using namespace std;
using namespace Utilities;

Engine::Engine(int width, int height, int depth)
	: SDLGLWindow(width, height, depth),
	  m_mode(0),
	  m_lightMode(-1),
	  m_wireframe(false),
	  m_normals(true)
{
	m_sphere = new Sphere(1.5, 50);
	m_sphere->setRotation(floatn<3>(0.0, 45.0, 0.0));
	
	m_cone = new Cone(2.0, 1.0, 50);
	m_cone->setRotation(floatn<3>(0.0, 45.0, 0.0));
	
	Drawable* drawable = new Sphere(0.4, 10);
	drawable->setRotation(floatn<3>(25.0, 0.0, 0.0));
	drawable->addKeyframe(2000, floatn<3>(-2.0, 2.0, 0.0));
	drawable->addKeyframe(2000, floatn<3>(0.0, 2.0, 0.0));
	m_animated.push_back(drawable);
	
	drawable = new Sphere(0.6, 20);
	drawable->setRotation(floatn<3>(25.0, 25.0, 0.0));
	drawable->addKeyframe(3100, floatn<3>(-2.0, 0.8, 0.0));
	drawable->addKeyframe(3100, floatn<3>(0.0, 0.8, 0.0));
	m_animated.push_back(drawable);
	
	drawable = new Sphere(1.0, 40);
	drawable->setRotation(floatn<3>(25.0, 25.0, 25.0));
	drawable->addKeyframe(4200, floatn<3>(-2.0, -1.2, 0.0));
	drawable->addKeyframe(4200, floatn<3>(0.0, -1.2, 0.0));
	m_animated.push_back(drawable);
	
	drawable = new Cone(1.5, 0.5, 10);
	drawable->setRotation(floatn<3>(0.0, 25.0, 0.0));
	drawable->addKeyframe(0, floatn<3>(1.7, 1.0, 0.0));
	m_animated.push_back(drawable);
	
	drawable = new Cone(1.5, 0.5, 50);
	drawable->setRotation(floatn<3>(25.0, 0.0, 0.0));
	drawable->addKeyframe(0, floatn<3>(1.7, -1.0, 0.0));
	m_animated.push_back(drawable);
	
	cycleLightMode();
	setTitle("In modes D and E, try keys W, N and L!", "Comp3004 CW1");
}

Engine::~Engine()
{
	delete m_sphere;
	delete m_cone;
}

void Engine::initGL()
{
	glEnable(GL_DEPTH_TEST);
}

void Engine::resizeGL(int width, int height)
{
	GLfloat ratio;

	/* Protect against a divide by zero */
	if (height == 0)
		height = 1;

	ratio = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Engine::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(0.0, 0.0, -6.0);
	
	switch (m_mode)
	{
	case 0: // Wireframe sphere
		draw(m_sphere, true, false, false);
		break;
	
	case 1: // Wireframe cone
		draw(m_cone, true, false, false);
		break;
		
	case 2: // Wireframe shapes with normals
		glPushMatrix();
			glTranslatef(-1.3, 0.0, 0.0);
			draw(m_sphere, true, false, true);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5, 0.0, 0.0);
			draw(m_cone, true, false, true);
		glPopMatrix();
		break;
	
	case 3: // Shaded shapes with normals
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushMatrix();
			glTranslatef(-1.3, 0.0, 0.0);
			draw(m_sphere, m_wireframe, true, m_normals);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5, 0.0, 0.0);
			draw(m_cone, m_wireframe, true, m_normals);
		glPopMatrix();
		break;
	
	case 4: // Animation
		for(vector<Drawable*>::iterator i=m_animated.begin(); i!=m_animated.end(); i++)
			draw(*i, m_wireframe, true, m_normals);
		break;
	}
}

// Draws a shape
void Engine::draw(Drawable* drawable, bool wireframe, bool shaded, bool normals)
{
	glPushMatrix();
		// Wireframe mode
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
		// Animation
		drawable->updatePosition(timeElapsed());
	
		// Transform the light vector into object space
		float3 lightPosObjectSpace = normalize(multMatrixTrans(m_lightPos));
	
		// Update the shading on the object
		drawable->updateLighting(lightPosObjectSpace, shaded);
	
		// Draw
		drawable->bind();
		drawable->draw();
		if (normals)
			drawable->drawNormals();
		drawable->unbind();
	glPopMatrix();
}

void Engine::keyPressEvent(const SDL_keysym& keySym)
{
	switch (keySym.sym)
	{
		case SDLK_q: case SDLK_ESCAPE: close(); break;
		
		case SDLK_a: m_mode = 0; break;
		case SDLK_b: m_mode = 1; break;
		case SDLK_c: m_mode = 2; break;
		case SDLK_d: m_mode = 3; break;
		case SDLK_e: m_mode = 4; setLightMode(4); break;
		
		case SDLK_l: cycleLightMode(); break;
		case SDLK_n: m_normals = !m_normals; break;
		case SDLK_w: m_wireframe = !m_wireframe; break;
		
		default: break;
	}
}

void Engine::cycleLightMode()
{
	setLightMode((m_lightMode + 1) % 5);
}

void Engine::setLightMode(int lightMode)
{
	m_lightMode = lightMode;
	
	switch (m_lightMode)
	{
	case 0: // Right
		m_lightPos[0] = 1.0;
		m_lightPos[1] = 0.0;
		m_lightPos[2] = 0.0;
		break;
	case 1: // Above
		m_lightPos[0] = 0.0;
		m_lightPos[1] = 1.0;
		m_lightPos[2] = 0.0;
		break;
	case 2: // Below
		m_lightPos[0] = 0.0;
		m_lightPos[1] = -1.0;
		m_lightPos[2] = 0.0;
		break;
	case 3: // From camera
		m_lightPos[0] = 0.0;
		m_lightPos[1] = 0.0;
		m_lightPos[2] = 1.0;
		break;
	case 4: // Side
		m_lightPos[0] = 1.0;
		m_lightPos[1] = 3.0;
		m_lightPos[2] = 1.0;
		m_lightPos = Utilities::normalize(m_lightPos);
		break;
	}
}
