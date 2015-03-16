#ifndef ENGINE_H
#define ENGINE_H

#include "sdlwindow.h"

class Texture;
class Sphere;

class Engine : public SDLWindow
{
public:
	Engine();
	~Engine();

protected:
	void draw();
	void keyPressEvent(SDLKey keysym);

private:
	void drawCube();
	void drawSphere();
	void polarVertex(float radius, float theta, float phi, int i);
	
	Texture* m_companionTex;
	Sphere* m_sphere;
	
	int m_mode;
	bool m_texturing;
	bool m_wireframe;
	bool m_perspective;
};

#endif
