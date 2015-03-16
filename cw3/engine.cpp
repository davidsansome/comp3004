#include <math.h>
#include <iostream>

#include "utilities.h"
#include "engine.h"
#include "model.h"
#include "shader.h"
#include "terrain.h"
#include "texture.h"
#include "waterstuff.h"
#include "tankwalls.h"
#include "outside.h"
#include "seaweed.h"
#include "fish.h"
#include "watersurface.h"
#include "castle.h"
#include "starfish.h"

#define BORDER 5
#define TWOBORDER (BORDER * 2)

#define WATER_HEIGHT 20.0
#define WALLS_HEIGHT (WATER_HEIGHT + 10.0)

Engine::Engine(int width, int height, int depth)
	: SDLGLWindow(width, height, depth),
	  m_t(0.0),
	  m_ppEnable(true),
	  m_blurAmount(4),
	  m_cameraSpeed(0.0),
	  m_leftPressed(false),
	  m_rightPressed(false),
	  m_upPressed(false),
	  m_downPressed(false),
	  m_pgupPressed(false),
	  m_pgdownPressed(false),
	  m_lookupPressed(false),
	  m_lookdownPressed(false),
	  m_forwardPressed(false),
	  m_backwardPressed(false),
	  m_camMode(FreeLook),
	  m_fishCam(0),
	  m_blinds(0.0),
	  m_blindsDown(false),
	  m_wireframe(false),
	  m_shadowVolumes(false)
{
	setTitle("Titan Terror Fishtank", "COMP3004 CW3");
	
	m_position[0] = 2.0;
	m_position[0].updateImmediately();
	m_position[1] = 5.0;
	m_position[1].updateImmediately();
	m_position[2] = 2.0;
	m_position[2].updateImmediately();
	m_horizRotation = M_PI/4.0;
	m_horizRotation.updateImmediately();
	m_horizRotation.setModulo(M_PI*2);
	
	m_currentFocus[0].setSpeed(0.001);
	m_currentFocus[1].setSpeed(0.001);
	m_currentFocus[2].setSpeed(0.001);
	
	m_lightVector = Vec3(1.0, 0.5, 0.0);
	
	// Semi circle ascending
	m_tourPath.addPoint(Vec3(40.0, 6.0, 5.0), Vec3(45.0, 4.0, 0.0), 5.0);
	m_tourFocus.push_back(Vec3(40.0, 6.0, 40.0)); // Looking at center
	
	// Moving to left middle
	m_tourPath.addPoint(Vec3(40.0, 18.0, 75.0), Vec3(-45.0, 0.0, 0.0), 3.5);
	m_tourFocus.push_back(Vec3(-10.0, 2.0, 40.0)); // Looking outside at reflections
	
	// Moving to left top
	m_tourPath.addPoint(Vec3(10.0, 6.0, 47.0), Vec3(0.0, 0.0, -10.0), 2.0);
	m_tourFocus.push_back(Vec3(-10.0, 2.0, 80.0)); // Looking at bottom left corner at reflections
	
	// Moving up to water's surface
	m_tourPath.addPoint(Vec3(10.0, 8.0, 10.0), Vec3(10.0, 0.0, -10.0), 2.0);
	m_tourFocus.push_back(Vec3(40.0, 20.0, 80.0)); // Looking towards top of tank
	
	// Moving back to the left, above tank
	m_tourPath.addPoint(Vec3(70.0, 25.0, 10.0), Vec3(0.0, 5.0, -15.0), 5.0);
	m_tourFocus.push_back(Vec3(80.0, 20.0, 80.0)); // Looking to far corner
	
	// Moving back down outside tank
	m_tourPath.addPoint(Vec3(-20.0, 35.0, -20.0), Vec3(-10.0, -5.0, 10.0), 5.0);
	m_tourFocus.push_back(Vec3(40.0, 6.0, 40.0)); // Looking at center
	
	// Still outside, move towards bottom right
	m_tourPath.addPoint(Vec3(0.0, 6.0, 100.0), Vec3(30.0, 0.0, 20.0), 5.0);
	m_tourFocus.push_back(Vec3(40.0, 6.0, 40.0)); // Looking at center
	
	// Move back into the tank, skimming across the terrain
	m_tourPath.addPoint(Vec3(85.0, 6.0, 80.0), Vec3(0.0, 0.0, -25.0), 3.5);
	m_tourFocus.push_back(Vec3(8.0, -2.0, 15.0)); // Look down at the terrain
	
	// Move towards the back of the castle
	m_tourPath.addPoint(Vec3(8.0, 6.0, 33.0), Vec3(0.0, 0.0, -50.0), 3.5);
	m_tourFocus.push_back(Vec3(60.0, 4.0, 40.0)); // Look at the castle
	
	// Move around the castle
	m_tourPath.addPoint(Vec3(68.0, 6.0, 45.0), Vec3(16.0, 0.0, -22.0), 3.5);
	m_tourFocus.push_back(Vec3(60.0, 4.0, 40.0)); // Look at the castle
	
	// End
	m_tourPath.addPoint(Vec3(56.0, 6.0, 57.0), Vec3(40.0, 0.0, 20.0), 5.0);
	m_tourFocus.push_back(Vec3(40.0, 6.0, 40.0)); // Looking at center
	
	m_tourPath.finalize();
}

Engine::~Engine()
{
	delete m_terrain;
	delete m_waterStuff;
	delete m_tankWalls;
	delete m_highTankWalls;
	delete m_outside;
	delete m_waterSurface;
	
	delete m_ppShader[0];
	delete m_ppShader[1];
	
	delete m_noiseTexture;
	
	for (vector<Seaweed*>::iterator it = m_seaweed.begin() ; it != m_seaweed.end() ; it++)
		delete (*it);
		
	for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
		delete (*it);
		
	for (vector<Starfish*>::iterator it = m_starfish.begin() ; it != m_starfish.end() ; it++)
		delete (*it);
	
	delete m_ppColorTex;
}

void Engine::initGL()
{
	m_terrain = new Terrain(20, 20);
	m_terrain->generate(500, 7, 12, 2.0);
	
	m_waterStuff = new WaterStuff();
	m_waterStuff->generate(600, m_terrain, WATER_HEIGHT);
	
	m_tankWalls = new TankWalls(m_terrain, -SAND_DEPTH, WATER_HEIGHT);
	m_highTankWalls = new TankWalls(m_terrain, WATER_HEIGHT, WALLS_HEIGHT);
	m_outside = new Outside(m_terrain);
	m_waterSurface = new WaterSurface(m_terrain, WATER_HEIGHT);
	m_castle = new Castle(Vec3(60, m_terrain->heightAt(60, 40), 40));
	m_solidObjects.push_back(m_castle);
	
	m_helpTexture = Texture::load("help.jpg");
	
	for (int i=0 ; i<35 ; ++i)
		m_seaweed.push_back(new Seaweed(m_terrain));
	
	for (int i=0 ; i<3 ; ++i)
	{
		Fish* fish = new Fish(m_terrain, i);
		m_fish.push_back(fish);
		m_solidObjects.push_back(fish);
	}
	
	for (int i=0 ; i<8 ; ++i)
	{
		Starfish* starfish = new Starfish(m_terrain);
		m_starfish.push_back(starfish);
	}
	
	m_noiseTexture = Texture::load("noise.jpg");
	
	m_ppShader[0] = new Shader("pp_vert_p0", "pp_frag_p0");
	m_ppShader[1] = new Shader("pp_vert_p1", "pp_frag_p1");
	m_ppShader[2] = new Shader("pp_vert_p2", "pp_frag_p2");
	
	m_ppGlowLoc[0] = m_ppShader[0]->uniformLocation("glow");
	
	m_ppOffsetLoc = m_ppShader[1]->uniformLocation("offset");
	m_ppGlowLoc[1] = m_ppShader[1]->uniformLocation("glow");
	
	m_ppColorBufLoc = m_ppShader[2]->uniformLocation("colorBuffer");
	m_ppNoiseLoc = m_ppShader[2]->uniformLocation("noise");
	m_ppGlowLoc[2] = m_ppShader[2]->uniformLocation("glow");
	m_ppPixelSizeLoc = m_ppShader[2]->uniformLocation("pixelSize");
	m_ppScreenSizeLoc = m_ppShader[2]->uniformLocation("screenSize");
	m_ppViewportSizeLoc = m_ppShader[2]->uniformLocation("viewportSize");
	m_ppT = m_ppShader[2]->uniformLocation("t");
	
	m_ppColorTex = new Texture(Texture::Mirrored);
	m_ppGlowTex = new Texture(Texture::Mirrored);
	
	m_lightPos = Vec3((m_terrain->width()-1) / 2.0, 50.0, (m_terrain->height()-1) / 2.0);
}

void Engine::resizeGL(int width, int height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearStencil(0x08);
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CW);
	
	m_ppWidth = nextPowerOfTwo(width);
	m_ppHeight = nextPowerOfTwo(height);
	
	m_terrain->finalize();
	m_tankWalls->generate();
	m_highTankWalls->generate();
	m_waterSurface->finalize();
	
	static bool firstRun = true;
	if (firstRun)
		firstRun = false;
	else
	{
		Shader::reloadAll();
		Texture::reloadAll();
	}
}

int Engine::nextPowerOfTwo(int size)
{
	int i=0;
	while (::pow(2, i) < size)
		++i;
	return int(::pow(2, i));
}

Vec Engine::cameraPos()
{
	switch (m_camMode)
	{
	case FreeLook:
		return Vec(9,
			m_position[0].currentValue(),
			m_position[1].currentValue(),
			m_position[2].currentValue(),
			m_position[0] + cos(m_horizRotation) * cos(m_vertRotation),
			m_position[1] + sin(m_vertRotation),
			m_position[2] + sin(m_horizRotation) * cos(m_vertRotation),
			0.0, 1.0, 0.0
		);
	case FishCam:
	{
		Vec3 pos = m_fish[m_fishCam]->pos();
		Vec3 nextPos = m_fish[m_fishCam]->nextPos();
		Vec3 direction = m_fish[m_fishCam]->direction();
		pos += direction;
		nextPos += direction * 5.0;
		
		return Vec(9,
			pos[0],
			pos[1] + 2.5,
			pos[2],
			nextPos[0],
			nextPos[1] + 0.5,
			nextPos[2],
			0.0, 1.0, 0.0
		);
	}
	case Tour:
	{
		Vec3 pos = m_tourPath.calc();
		Vec3 focus = m_tourFocus[m_tourPath.section()];
		return Vec(9,
			pos[0],
			pos[1],
			pos[2],
			m_currentFocus[0].currentValue(),
			m_currentFocus[1].currentValue(),
			m_currentFocus[2].currentValue(),
			0.0, 1.0, 0.0
		);
	}
	}
	return Vec(9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

void Engine::paintGL()
{
	// Update position and TDVs
	m_t += timeDelta();
	updateVariables();
	Vec pos = cameraPos();
	
	//cout << pos << " vert: " << m_vertRotation << " horiz: " << m_horizRotation << endl;
	
	// Is the camera inside the tank?
	bool insideTank =
		pos[0] > 0.0 && pos[1] > 0.0 && pos[2] > 0.0 &&
		pos[0] < m_terrain->width() - 1 &&
		pos[1] < WATER_HEIGHT &&
		pos[2] < m_terrain->height() - 1;
	
	// Clear buffers and draw the scene
	glViewport(0, 0, (GLsizei)width(), (GLsizei)height());
	glClearStencil(0x00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, float(width()) / (height()), 0.1, 200.0);
	
	glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5], pos[6], pos[7], pos[8]);
	drawScene(insideTank);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// Only do shadows inside the tank.  It's too hard to think about outside
	if (insideTank)
	{
		// Put the walls in the depth buffer to make the shadows fall on them
		m_tankWalls->bind();
		glColorMask(false, false, false, false);
		glDepthMask(true);
		m_tankWalls->draw();
		m_tankWalls->unbind();
	
		// Draw the shadow volumes into the stencil buffer
		glColorMask(false, false, false, false);
		glDepthMask(false);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0x00, 0x0F);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR);
		
		Fish::bindShadowVolume();
		for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
			(*it)->drawShadowVolume();
		Fish::unbindShadowVolume();
		
		Castle::bindShadowVolume();
		m_castle->drawShadowVolume();
		Castle::unbindShadowVolume();
		
		// Clear the top ends of the fish
		glStencilFunc(GL_ALWAYS, 0x08, 0xFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
		Fish::bind(0.0);
		for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
			(*it)->draw();
		Fish::unbind();
		
		glDisable(GL_STENCIL_TEST);
		glColorMask(true, true, true, true);
		glDepthMask(true);
		
		// Draw wireframe shadow volumes to the color buffers
		if (m_shadowVolumes)
		{
			glEnable(GL_BLEND);
			
			Fish::bindShadowVolume();
			for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
				(*it)->drawShadowVolume();
			Fish::unbindShadowVolume();
			
			Castle::bindShadowVolume();
			m_castle->drawShadowVolume();
			Castle::unbindShadowVolume();
			
			glDisable(GL_BLEND);
			
			/*glDisable(GL_DEPTH_TEST);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
				(*it)->drawShadowVolume();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);*/
		}
	}
	
	// Reset matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (insideTank)
		drawShadows();
	
	drawHelp();
	
	if (m_ppEnable && insideTank)
	{
		// Copy the scene to a texture
		m_ppColorTex->bind();
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_ppWidth, m_ppHeight, 0);
		
		// Copy the color buffer to another glow texture
		m_ppGlowTex->bind();
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_ppWidth, m_ppHeight, 0);
		
		// Clear buffers and do post-processing
		glViewport(0, 0, (GLsizei)width(), (GLsizei)height());
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthMask(false);
		
		// Darkening pass
		m_ppShader[0]->bind();
		m_ppGlowTex->bind(0, m_ppGlowLoc[0]);
		drawRect(float(width())/m_ppWidth, float(height())/m_ppHeight);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_ppWidth, m_ppHeight, 0);
		
		// Blurring passes
		m_ppShader[1]->bind();
		for (int pass=0 ; pass<m_blurAmount ; ++pass)
		{
			m_ppGlowTex->bind(0, m_ppGlowLoc[1]);
			
			glUniform2f(m_ppOffsetLoc, 1.0 / m_ppWidth, 0.0);
			drawRect(float(width())/m_ppWidth, float(height())/m_ppHeight);
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_ppWidth, m_ppHeight, 0);
			
			glUniform2f(m_ppOffsetLoc, 0.0, 1.0 / m_ppHeight);
			drawRect(float(width())/m_ppWidth, float(height())/m_ppHeight);
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_ppWidth, m_ppHeight, 0);
		}
		
		// Wobbling pass
		m_ppShader[2]->bind();
		glUniform1f(m_ppT, m_t);
		glUniform2f(m_ppPixelSizeLoc, 1.0 / m_ppWidth, 1.0 / m_ppHeight);
		glUniform2f(m_ppScreenSizeLoc, width(), height());
		glUniform2f(m_ppViewportSizeLoc, float(width()-1) / m_ppWidth, float(height()-1) / m_ppHeight);
		
		// Setup our textures
		m_ppColorTex->bind(0, m_ppColorBufLoc);
		m_noiseTexture->bind(2, m_ppNoiseLoc);
		m_ppGlowTex->bind(3, m_ppGlowLoc[2]);
		
		drawRect(float(width() - 3)/m_ppWidth, float(height() - 3)/m_ppHeight);
		
		// Unbind
		m_ppShader[1]->unbind();
		glDepthMask(true);
	}
	
	// Draw blinds
	drawBlinds();
}

void Engine::drawScene(bool insideTank)
{
	const float lightPos[] = {m_lightPos[0], m_lightPos[1], m_lightPos[2], 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	// Draw the fish
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	Fish::setParams(m_t);
	Fish::bind();
	for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
		(*it)->draw();
	Fish::unbind();
	glDisable(GL_CULL_FACE);
	
	// Draw the table
	m_outside->setParams(m_lightPos);
	m_outside->bind();
	m_outside->draw();
	m_outside->unbind();
	
	// Draw the inside faces of the fish tank into the stencil buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_STENCIL_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glColorMask(false, false, false, false);
	glDepthMask(false);
	
	m_tankWalls->bind();
	m_tankWalls->draw();
	m_tankWalls->unbind();
	
	glDepthMask(true);
	glColorMask(true, true, true, true);
	glDisable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	
	// Draw the terrain
	m_terrain->setParams(m_lightVector);
	m_terrain->setCaustic(int(m_t * 20.0) % 32);
	m_terrain->bind();
	m_terrain->draw();
	
	m_terrain->bindReflections();
	m_terrain->drawReflections();
	m_terrain->unbind();
	
	// Draw the castle
	Castle::bind();
	m_castle->draw();
	Castle::unbind();
	
	// Draw the starfish
	Starfish::setParams(m_t, m_lightPos);
	Starfish::bind();
	for (vector<Starfish*>::iterator it = m_starfish.begin() ; it != m_starfish.end() ; it++)
	{
		(*it)->updatePosition(timeElapsed());
		(*it)->draw();
	}
	Starfish::unbind();
	
	// Now draw the tank again into the depth and color buffers
	if (insideTank)
		m_tankWalls->bind(Vec3(0.0, 0.5, 1.0), Vec3(1.0, 1.0, 1.0), 1.0);
	else
		m_tankWalls->bind(Vec3(0.56, 0.66, 0.8), Vec3(1.0, 1.0, 1.0), 3.0);
	m_tankWalls->draw();
	m_tankWalls->unbind();
	
	m_highTankWalls->bind(Vec3(1.0, 1.0, 1.0), Vec3(1.0, 1.0, 1.0));
	m_highTankWalls->draw();
	m_highTankWalls->unbind();
	
	// Draw the water's surface
	m_waterSurface->setParams(m_t, m_lightPos);
	m_waterSurface->bind();
	m_waterSurface->draw();
	m_waterSurface->unbind();
	
	// Draw our particles
	m_waterStuff->bind();
	m_waterStuff->draw();
	m_waterStuff->unbind();
	
	//glDisable(GL_DEPTH_TEST);
	//drawAxes();
	//glEnable(GL_DEPTH_TEST);
	
	// Draw seaweed
	if (insideTank)
		Seaweed::bind(1.0);
	else
		Seaweed::bind(0.2);
	for (vector<Seaweed*>::iterator it = m_seaweed.begin() ; it != m_seaweed.end() ; it++)
		(*it)->draw();
	Seaweed::unbind();
}

void Engine::keyPressEvent(const SDL_keysym& keySym)
{
	switch (keySym.sym)
	{
		// Close keys
		case SDLK_q:
		case SDLK_ESCAPE:
			close();
			break;
		
		// Movement keys
		case SDLK_LEFT:
		case SDLK_a:        m_leftPressed = true;     break;
		case SDLK_RIGHT:
		case SDLK_d:        m_rightPressed = true;    break;
		case SDLK_UP:       m_upPressed = true;       break;
		case SDLK_DOWN:     m_downPressed = true;     break;
		case SDLK_SPACE:    m_cameraSpeed = 0.0;      break;
		case SDLK_PAGEUP:   m_pgupPressed = true;     break;
		case SDLK_PAGEDOWN: m_pgdownPressed = true;   break;
		case SDLK_o:        m_lookupPressed = true;   break;
		case SDLK_k:        m_lookdownPressed = true; break;
		case SDLK_w:        m_forwardPressed = true;  break;
		case SDLK_s:        m_backwardPressed = true; break;
		
		// Graphics
		case SDLK_r:        resetAnimation();         break;
		case SDLK_i:        m_wireframe = !m_wireframe; break;
		case SDLK_u:        m_ppEnable = !m_ppEnable; break;
		case SDLK_LEFTBRACKET:  m_blurAmount = max(0,  m_blurAmount - 1); break;
		case SDLK_RIGHTBRACKET: m_blurAmount = min(16, m_blurAmount + 1); break;
		case SDLK_v:        m_shadowVolumes = !m_shadowVolumes; break;
		
		// Help
		case SDLK_h:        m_helpOpacity = (m_helpOpacity.targetValue() == 1.0) ? 0.0 : 1.0; break;
		
		// Camera keys
		case SDLK_f: // FishCam
			if (m_camMode == FishCam)
			{
				if (m_fishCam == m_fish.size() - 1)
					setCamMode(FreeLook);
				m_fishCam = (m_fishCam + 1) % m_fish.size();
			}
			else
				setCamMode(FishCam);
			break;
		case SDLK_t: // Tour
			m_tourPath.reset();
			m_currentFocus[0] = m_tourFocus[0][0];
			m_currentFocus[0].updateImmediately();
			m_currentFocus[1] = m_tourFocus[0][1];
			m_currentFocus[1].updateImmediately();
			m_currentFocus[2] = m_tourFocus[0][2];
			m_currentFocus[2].updateImmediately();
			setCamMode(Tour);
			break;
		case SDLK_e: // Exit FishCam or Tour
			setCamMode(FreeLook);
			break;
		case SDLK_p: // Predefined location
			m_position[0] = 61.5194;
			m_position[1] = m_terrain->heightAt(60, 40) + 3.0;
			m_position[2] = 47.1141;
			m_vertRotation = 0.0;
			m_horizRotation = 4.11235;
			m_position[0].updateImmediately();
			m_position[1].updateImmediately();
			m_position[2].updateImmediately();
			m_vertRotation.updateImmediately();
			m_horizRotation.updateImmediately();
		
		default: break;
	}
}

void Engine::resetAnimation()
{
	for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
		(*it)->reset();
	
	m_t = 0.0;
}

void Engine::keyReleaseEvent(const SDL_keysym& keySym)
{
	switch (keySym.sym)
	{
		// Movement keys
		case SDLK_LEFT:
		case SDLK_a:        m_leftPressed = false;     break;
		case SDLK_RIGHT:
		case SDLK_d:        m_rightPressed = false;    break;
		case SDLK_UP:       m_upPressed = false;       break;
		case SDLK_DOWN:     m_downPressed = false;     break;
		case SDLK_PAGEUP:   m_pgupPressed = false;     break;
		case SDLK_PAGEDOWN: m_pgdownPressed = false;   break;
		case SDLK_o:        m_lookupPressed = false;   break;
		case SDLK_k:        m_lookdownPressed = false; break;
		case SDLK_w:        m_forwardPressed = false;  break;
		case SDLK_s:        m_backwardPressed = false; break;
		
		default: break;
	}
}

void Engine::setCamMode(CameraMode mode)
{
	if (mode == FreeLook)
	{
		// Exiting the tour, so set position and viewing direction
		Vec camInfo = cameraPos();
		Vec3 pos(camInfo[0], camInfo[1], camInfo[2]);
		Vec3 look(camInfo[3], camInfo[4], camInfo[5]);
		Vec3 lookVector = norm(look - pos);
		
		// Set position
		m_position[0] = pos[0];
		m_position[1] = pos[1];
		m_position[2] = pos[2];
		
		// Set rotation
		m_vertRotation = asin(lookVector[1]);
		m_horizRotation = atan2(lookVector[2], lookVector[0]);
		
		// Set speed
		m_cameraSpeed = 0.0;
	
		// Force update TDVs
		m_position[0].updateImmediately();
		m_position[1].updateImmediately();
		m_position[2].updateImmediately();
		m_vertRotation.updateImmediately();
		m_horizRotation.updateImmediately();
	}
	
	m_camMode = mode;
	m_blindsDown = mode != FreeLook;
}

void Engine::updateVariables()
{
	// Handle left/right movement
	if (m_leftPressed)    m_horizRotation = m_horizRotation.targetValue() - timeDelta() * M_PI;
	if (m_rightPressed)   m_horizRotation = m_horizRotation.targetValue() + timeDelta() * M_PI;
	m_horizRotation.finishedEditing();
	
	// Handle look up/down
	if (m_lookupPressed)   m_vertRotation = min( M_PI_2 - 0.001, m_vertRotation.targetValue() + timeDelta() * M_PI_2);
	if (m_lookdownPressed) m_vertRotation = max(-M_PI_2 + 0.001, m_vertRotation.targetValue() - timeDelta() * M_PI_2);
	
	// Handle up/down movement
	if (m_pgupPressed)    m_position[1] = m_position[1].targetValue() + timeDelta() * 5.0;
	if (m_pgdownPressed)  m_position[1] = m_position[1].targetValue() - timeDelta() * 5.0;
	
	// Handle forward/back movement
	if (m_upPressed)      m_cameraSpeed += timeDelta() * 2.0;
	if (m_downPressed)    m_cameraSpeed -= timeDelta() * 2.0;
	
	float speed = m_cameraSpeed;
	if (m_forwardPressed) speed = 1.0;
	if (m_backwardPressed) speed = -1.0;
	
	float xDelta = speed * 10.0 * timeDelta() * cos(m_horizRotation) * cos(m_vertRotation);
	float yDelta = speed * 10.0 * timeDelta() * sin(m_vertRotation);
	float zDelta = speed * 10.0 * timeDelta() * sin(m_horizRotation) * cos(m_vertRotation);
	
	m_position[0] = m_position[0].targetValue() + xDelta;
	m_position[1] = m_position[1].targetValue() + yDelta;
	m_position[2] = m_position[2].targetValue() + zDelta;
	
	// Pre-update collision detection
	bool beneathWater = m_position[1] < WALLS_HEIGHT;
	bool insideTank = m_position[0] > 0.0 && m_position[1] > 0.0 && m_position[2] > 0.0 &&
		m_position[0] < m_terrain->width() - 1 &&
		m_position[1] < WATER_HEIGHT &&
		m_position[2] < m_terrain->height() - 1;
	
	if (m_camMode == FreeLook && beneathWater)
	{
		static const float threshold = 0.2;
		const float iw = m_terrain->width() - 1 - threshold;
		const float ih = m_terrain->height() - 1 - threshold;
		const float ow = m_terrain->width() - 1 + threshold;
		const float oh = m_terrain->height() - 1 + threshold;
		
		// Stop the camera from going through the tank walls
		if (insideTank)
		{
			if (m_position[0] >= threshold && m_position[0].targetValue() <= threshold)
				m_position[0] = threshold;
			if (m_position[2] >= threshold && m_position[2].targetValue() <= threshold)
				m_position[2] = threshold;
			if (m_position[0] <= iw && m_position[0].targetValue() >= iw)
				m_position[0] = iw;
			if (m_position[2] <= ih && m_position[2].targetValue() >= ih)
				m_position[2] = ih;
		}
		else
		{
			if (m_position[0] <= -threshold && m_position[0].targetValue() >= -threshold && m_position[2] >= -threshold && m_position[2] <= oh)
				m_position[0] = -threshold;
			if (m_position[2] <= -threshold && m_position[2].targetValue() >= -threshold && m_position[0] >= -threshold && m_position[0] <= oh)
				m_position[2] = -threshold;
			if (m_position[0] >= ow && m_position[0].targetValue() <= ow && m_position[2] >= -threshold && m_position[2] <= oh)
				m_position[0] = ow;
			if (m_position[2] >= oh && m_position[2].targetValue() <= oh && m_position[0] >= -threshold && m_position[0] <= oh)
				m_position[2] = oh;
		}
		
		Vec3 position(m_position[0].targetValue(), m_position[1].targetValue(), m_position[2].targetValue());
		
		// Collision with objects
		for (vector<SolidObject*>::iterator it = m_solidObjects.begin() ; it != m_solidObjects.end() ; it++)
		{
			Vec3 objectCenter = (*it)->pos();
			Vec3 objectToPos = position - objectCenter;
			float distance = len(objectToPos);
			if (distance < (*it)->boundingRadius())
			{
				Vec3 correctedPosition = objectCenter + norm(objectToPos) * (*it)->boundingRadius();
				m_position[0] = correctedPosition[0];
				m_position[1] = correctedPosition[1];
				m_position[2] = correctedPosition[2];
			}
		}
	}
	
	// Update TDVs
	m_horizRotation.update(timeElapsed());
	m_vertRotation.update(timeElapsed());
	m_position[0].update(timeElapsed());
	m_position[1].update(timeElapsed());
	m_position[2].update(timeElapsed());
	m_helpOpacity.update(timeElapsed());
	
	// Post-update collision detection
	if (m_camMode == FreeLook)
	{
		bool insideTank = m_position[0] > 0.0 &&
			m_position[0] < m_terrain->width() - 1 &&
			m_position[2] > 0.0 &&
			m_position[2] < m_terrain->height() - 1;
		
		float minY;
		if (insideTank) // Collision with seabed
			minY = m_terrain->heightAt(Vec2(m_position[0], m_position[2])) + 1.0;
		else // Collision with table
			minY = -SAND_DEPTH + 1.0;
		
		if (m_position[1].targetValue() < minY)
			m_position[1] = minY;
	}
	
	// Update tour
	if (m_camMode == Tour)
	{
		if (m_tourPath.updateProgress(timeDelta())) // The tour finished
			setCamMode(FreeLook);
		else
		{
			m_currentFocus[0] = m_tourFocus[m_tourPath.section()][0];
			m_currentFocus[1] = m_tourFocus[m_tourPath.section()][1];
			m_currentFocus[2] = m_tourFocus[m_tourPath.section()][2];
			
			m_currentFocus[0].update(timeElapsed());
			m_currentFocus[1].update(timeElapsed());
			m_currentFocus[2].update(timeElapsed());
		}
	}
	
	m_waterStuff->update(timeDelta());
	for (vector<Fish*>::iterator it = m_fish.begin() ; it != m_fish.end() ; it++)
		(*it)->update(timeDelta());
	
	m_blinds = Utilities::clamp(m_blinds + timeDelta() * (m_blindsDown ? 1.0 : -1.0), 0.0, 1.0);
}

void Engine::drawRect(float texWidth, float texHeight)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, texHeight);
		glVertex2f(-1.0f, 1.0f);

		glTexCoord2f(texWidth, texHeight);
		glVertex2f(1.0f, 1.0f);

		glTexCoord2f(texWidth, 0.0);
		glVertex2f(1.0f, -1.0f);

		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0f, -1.0f);
	glEnd();
}

void Engine::drawAxes()
{
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
		
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}

void Engine::drawBlinds()
{
	float extent = m_blinds * (1.0 - float(width())/(1.85*height()));
	glColor3f(0.0, 0.0, 0.0);
	
	glBegin(GL_QUADS);
		glVertex2f(-1.0, 1.0);
		glVertex2f( 1.0, 1.0);
		glVertex2f( 1.0, 1.0 - extent);
		glVertex2f(-1.0, 1.0 - extent);
		
		glVertex2f(-1.0, -1.0);
		glVertex2f( 1.0, -1.0);
		glVertex2f( 1.0, -1.0 + extent);
		glVertex2f(-1.0, -1.0 + extent);
	glEnd();
}

void Engine::drawHelp()
{
	if (m_helpOpacity < 0.01)
		return;
	
	float w = 640.0 / width();
	float h = 480.0 / height();
	
	m_helpTexture->bind();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	glColor4f(1.0, 1.0, 1.0, m_helpOpacity);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-w, h);

		glTexCoord2f(1.0, 0.0);
		glVertex2f(w, h);

		glTexCoord2f(1.0, 1.0);
		glVertex2f(w, -h);

		glTexCoord2f(0.0, 1.0);
		glVertex2f(-w, -h);
	glEnd();
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Engine::drawShadows()
{
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_LESS, 0x08, 0x0F);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	
	glEnable(GL_BLEND);
	glColor4f(0.0, 0.0, 0.0, 0.5);
	
	glBegin(GL_QUADS);
		glVertex2f(-1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(1.0, -1.0);
		glVertex2f(-1.0, -1.0);
	glEnd();
	
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
}

