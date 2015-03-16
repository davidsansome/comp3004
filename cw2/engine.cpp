#include "engine.h"
#include "gp2x.h"
#include "texture.h"
#include "contextstate.h"
#include "sphere.h"

Engine::Engine()
	: SDLWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "COMP3004 Coursework 2"),
	  m_mode(0),
	  m_texturing(false),
	  m_wireframe(false),
	  m_perspective(true)
{
	m_companionTex = new Texture("ccube.bmp");
	m_sphere = new Sphere(0.5, 20);
}

Engine::~Engine()
{
	delete m_companionTex;
}

void Engine::keyPressEvent(SDLKey keysym)
{
	switch (keysym)
	{
	case SDLK_w:
		m_wireframe = !m_wireframe;
		break;
	case SDLK_t:
		m_texturing = !m_texturing;
		break;
	case SDLK_m:
		m_mode = (m_mode + 1) % 3;
		break;
	case SDLK_p:
		m_perspective = !m_perspective;
		break;
	case SDLK_q:
		quit();
		break;
	default:
		break;
	}
}

void Engine::draw()
{
	clear();
	
	context()->setMatrixMode(ContextState::ProjectionMatrix);
	context()->loadIdentity();
	float aspect = float(width())/height();
	if (m_perspective)
		context()->perspective(45.0, aspect, 1.0, 50.0);
	else
		context()->ortho(3.0, -3.0, 3.0/aspect, -3.0/aspect, -3, 3);
	
	context()->setMatrixMode(ContextState::ModelViewMatrix);
	context()->loadIdentity();
	
	static float theta = M_PI * 0.1;
	theta += timeDelta() * M_PI/6.0;
	
	if (m_wireframe)
		context()->setPolygonMode(ContextState::Line);
	else
		context()->setPolygonMode(ContextState::Fill);
	
	switch (m_mode)
	{
	case 0:
		context()->rotate(theta, ContextState::YAxis);
		context()->translate(Vec3(0.0, 0.0, 2.0));
		context()->bindTexture(m_texturing ? m_companionTex : NULL);
		drawCube();
		break;
	case 1:
		context()->rotate(theta, ContextState::YAxis);
		context()->translate(Vec3(0.0, 0.0, 2.0));
		context()->bindTexture(NULL);
		drawSphere();
		break;
	case 2:
		context()->translate(Vec3(-0.75, 0.0, 0.0));
		context()->rotate(theta, ContextState::YAxis);
		context()->translate(Vec3(0.0, 0.0, 5.0));
		context()->bindTexture(NULL);
		drawSphere();
		context()->loadIdentity();
		context()->translate(Vec3(1.5, 0.0, 0.0));
		context()->rotate(theta, ContextState::YAxis);
		context()->translate(Vec3(0.0, 0.0, 5.0));
		context()->bindTexture(m_texturing ? m_companionTex : NULL);
		drawCube();
		break;
	}
	
	
	/*array<Vertex, 2> line;
	line[0] = Vertex(Vec3(1.0, 3.0, 1.0), Vec3(0.0, 0.0, 0.0));
	line[1] = Vertex(Vec3(1.0, 1.0, 1.0), Vec3(1.0, 1.0, 1.0));
	drawPoly<2>(line);*/
}

void Engine::drawCube()
{
	float r = 0.5;
	array<Vertex, 4> quad;
	
	using Utilities::newVertex;
	
	// Back face
	quad[0] = newVertex(Vec3(-r, -r, r), Vec3(1.0, 0.0, 0.0), Vec2(1.0, 1.0));
	quad[1] = newVertex(Vec3(r, -r, r), Vec3(0.0, 1.0, 0.0), Vec2(0.0, 1.0));
	quad[2] = newVertex(Vec3(r, r, r), Vec3(0.0, 0.0, 1.0), Vec2(0.0, 0.0));
	quad[3] = newVertex(Vec3(-r, r, r), Vec3(1.0, 1.0, 1.0), Vec2(1.0, 0.0));
	drawPoly<4>(quad);
	
	// Top face
	quad[0] = newVertex(Vec3(-r, r, r), Vec3(1.0, 1.0, 1.0), Vec2(0.0, 0.0));
	quad[1] = newVertex(Vec3(r, r, r), Vec3(0.0, 0.0, 1.0), Vec2(1.0, 0.0));
	quad[2] = newVertex(Vec3(r, r, -r), Vec3(1.0, 0.0, 0.0), Vec2(1.0, 1.0));
	quad[3] = newVertex(Vec3(-r, r, -r), Vec3(0.5, 0.5, 0.0), Vec2(0.0, 1.0));
	drawPoly<4>(quad);
	
	// Front face
	quad[0] = newVertex(Vec3(-r, -r, -r), Vec3(0.0, 0.0, 1.0), Vec2(0.0, 1.0));
	quad[1] = newVertex(Vec3(r, -r, -r), Vec3(0.0, 0.5, 0.5), Vec2(1.0, 1.0));
	quad[2] = newVertex(Vec3(r, r, -r), Vec3(1.0, 0.0, 0.0), Vec2(1.0, 0.0));
	quad[3] = newVertex(Vec3(-r, r, -r), Vec3(0.5, 0.5, 0.0), Vec2(0.0, 0.0));
	drawPoly<4>(quad);
	
	// Bottom face
	quad[0] = newVertex(Vec3(-r, -r, r), Vec3(1.0, 0.0, 0.0), Vec2(0.0, 1.0));
	quad[1] = newVertex(Vec3(r, -r, r), Vec3(0.0, 1.0, 0.0), Vec2(1.0, 1.0));
	quad[2] = newVertex(Vec3(r, -r, -r), Vec3(0.0, 0.5, 0.5), Vec2(1.0, 0.0));
	quad[3] = newVertex(Vec3(-r, -r, -r), Vec3(0.0, 0.0, 1.0), Vec2(0.0, 0.0));
	drawPoly<4>(quad);
	
	// Right face
	quad[0] = newVertex(Vec3(r, -r, r), Vec3(0.0, 1.0, 0.0), Vec2(1.0, 1.0));
	quad[1] = newVertex(Vec3(r, r, r), Vec3(0.0, 0.0, 1.0), Vec2(1.0, 0.0));
	quad[2] = newVertex(Vec3(r, r, -r), Vec3(1.0, 0.0, 0.0), Vec2(0.0, 0.0));
	quad[3] = newVertex(Vec3(r, -r, -r), Vec3(0.0, 0.5, 0.5), Vec2(0.0, 1.0));
	drawPoly<4>(quad);
	
	// Left face
	quad[0] = newVertex(Vec3(-r, -r, r), Vec3(1.0, 0.0, 0.0), Vec2(1.0, 1.0));
	quad[1] = newVertex(Vec3(-r, r, r), Vec3(1.0, 1.0, 1.0), Vec2(1.0, 0.0));
	quad[2] = newVertex(Vec3(-r, r, -r), Vec3(0.5, 0.5, 0.0), Vec2(0.0, 0.0));
	quad[3] = newVertex(Vec3(-r, -r, -r), Vec3(0.0, 0.0, 1.0), Vec2(0.0, 1.0));
	drawPoly<4>(quad);
}

void Engine::drawSphere()
{
	Vec4 light(1.0, 0.0, 0.0, 1.0);
	light = trans(context()->modelViewMatrix()) * light;
	light = norm(light);
	
	m_sphere->updateLighting(Vec3(light[0], light[1], light[2]), true);
	for (int i=0 ; i<m_sphere->quadCount() ; ++i)
		drawPoly<4>(m_sphere->quad(i));
}


