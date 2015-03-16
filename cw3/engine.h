#ifndef ENGINE_H
#define ENGINE_H

#include <svl/SVL.h>
#include <vector>

#include "sdlglwindow.h"
#include "timedependentvariable.h"
#include "bezier.h"

#define SHADOW_SIZE 512

class Model;
class Shader;
class Terrain;
class WaterStuff;
class TankWalls;
class Texture;
class Outside;
class Seaweed;
class Fish;
class WaterSurface;
class Castle;
class SolidObject;
class Starfish;

// Main class to do the drawing for cw3
class Engine : public SDLGLWindow
{
public:
	Engine(int width, int height, int depth);
	~Engine();
	
	void initGL();
	void resizeGL(int width, int height);
	void paintGL();

private:
	enum CameraMode
	{
		FreeLook,
		FishCam,
		Tour
	};
	
	void keyPressEvent(const SDL_keysym& keySym);
	void keyReleaseEvent(const SDL_keysym& keySym);
	void updateVariables();
	void drawScene(bool insideTank);
	int nextPowerOfTwo(int size);
	void drawRect(float texWidth, float texHeight);
	void drawAxes();
	void drawBlinds();
	void drawHelp();
	void drawShadows();
	Vec cameraPos();
	void setCamMode(CameraMode mode);
	void resetAnimation();
	
	Vec3 m_lightPos;
	Vec3 m_lightVector;
	
	Terrain* m_terrain;
	WaterStuff* m_waterStuff;
	TankWalls* m_tankWalls;
	TankWalls* m_highTankWalls;
	Outside* m_outside;
	WaterSurface* m_waterSurface;
	Castle* m_castle;
	vector<Seaweed*> m_seaweed;
	vector<Fish*> m_fish;
	vector<Starfish*> m_starfish;
	vector<SolidObject*> m_solidObjects;
	
	float m_t;
	
	Shader* m_ppShader[3];
	Texture* m_ppColorTex;
	Texture* m_ppGlowTex;
	int m_ppColorBufLoc;
	int m_ppGlowLoc[3];
	int m_ppNoiseLoc;
	int m_ppT;
	int m_ppPixelSizeLoc;
	int m_ppScreenSizeLoc;
	int m_ppViewportSizeLoc;
	int m_ppOffsetLoc;
	int m_ppWidth;
	int m_ppHeight;
	bool m_ppEnable;
	int m_blurAmount;
	
	Texture* m_noiseTexture;
	
	TimeDependentVariable m_horizRotation;
	TimeDependentVariable m_vertRotation;
	TimeDependentVariable m_position[3];
	float m_cameraSpeed;
	
	bool m_leftPressed;
	bool m_rightPressed;
	bool m_upPressed;
	bool m_downPressed;
	bool m_pgupPressed;
	bool m_pgdownPressed;
	bool m_lookupPressed;
	bool m_lookdownPressed;
	bool m_forwardPressed;
	bool m_backwardPressed;
	
	CameraMode m_camMode;
	uint m_fishCam;
	
	Bezier m_tourPath;
	vector<Vec3> m_tourFocus;
	TimeDependentVariable m_currentFocus[3];
	float m_blinds;
	bool m_blindsDown;
	bool m_wireframe;
	bool m_shadowVolumes;
	
	Texture* m_helpTexture;
	TimeDependentVariable m_helpOpacity;
};

#endif

