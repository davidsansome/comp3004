#ifndef ENGINE_H
#define ENGINE_H

#include "sdlglwindow.h"
#include "types.h"
#include <vector>

class Sphere;
class Cone;
class Drawable;

using std::vector;

// Main class to do the drawing for cw1
class Engine : public SDLGLWindow
{
public:
	Engine(int width, int height, int depth);
	~Engine();
	
	void initGL();
	void resizeGL(int width, int height);
	void paintGL();

private:
	void keyPressEvent(const SDL_keysym& keySym);
	void draw(Drawable* drawable, bool wireframe, bool shaded, bool normals);
	void cycleLightMode();
	void setLightMode(int lightMode);
	
	int m_mode;
	Cone* m_cone;
	Sphere* m_sphere;
	vector<Drawable*> m_animated;
	
	float3 m_lightPos;
	int m_lightMode;
	bool m_wireframe;
	bool m_normals;
};

#endif

