////////////////////////////////////////
// Tester.h
////////////////////////////////////////

#pragma once

#include "Shader.h"
#include "Camera.h"
#include "SpinningCube.h"
#include "Skeleton.h"
#include "Joint.h"
#include "DOF.h"
#include "Skin.h"
#include "Vertex.h"
#include "Rig.h"
#include "Animation.h"
#include "Channel.h"
#include "Keyframe.h"
#include "Player.h"

////////////////////////////////////////////////////////////////////////////////

// The 'Tester' is a simple top level application class. It creates and manages a
// window with the GLUT extension to OpenGL and it maintains a simple 3D scene
// including a camera and some other components.

class Tester {
public:
	Tester(const char *windowTitle,int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Components
	ShaderProgram *Program;
	Camera *Cam;

	Rig* currRig;
	Animation* currAnimation;
	Player* currPlayer;

	bool drawSkel;
	bool drawSkin;

};

////////////////////////////////////////////////////////////////////////////////
