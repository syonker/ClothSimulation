////////////////////////////////////////
// Tester.cpp
////////////////////////////////////////

#include "Tester.h"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER=0;

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	TESTER=new Tester("Spinning Cube",argc,argv);
	glutMainLoop();
	delete TESTER;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(const char *windowTitle,int argc,char **argv) {
	WinX=800;
	WinY=600;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 100, 100 );
	WindowHandle = glutCreateWindow( windowTitle );
	glutSetWindowTitle( windowTitle );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize GLEW
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Initialize components
	Program=new ShaderProgram("Modify.glsl",ShaderProgram::eRender);
	Cam=new Camera;
	Cam->SetAspect(float(WinX)/float(WinY));

	//Prep Rig
	currRig = new Rig();
	currRig->Load("../skeletons/wasp2.skel", "../skins/wasp2.skin");

	drawSkel = true;
	drawSkin = true;

	currAnimation = new Animation();
	currAnimation->Load("../animations/wasp2_walk.anim");

	currPlayer = new Player(currAnimation, currRig->skeleton);

}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	delete Program;
	delete Cam;
	delete currRig;

	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam->Update();

	currPlayer->Evaluate();

	currRig->Update(currPlayer->rootTranslate);

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam->Reset();
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currRig->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID(), drawSkel, drawSkin);

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		case '1':
			if (drawSkel) {
				drawSkel = false;
			}
			else {
				drawSkel = true;
			}
			break;
		case '2':
			if (drawSkin) {
				drawSkin = false;
			}
			else {
				drawSkin = true;
			}
			break;
		case 'd':
			currRig->skeleton->upSelection();
			break;
		case 'a':
			currRig->skeleton->downSelection();
			break;
		case 'w':
			currRig->skeleton->incDOF();
			break;
		case 's':
			currRig->skeleton->decDOF();
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int maxDelta=100;
	int dx = glm::clamp(nx - MouseX,-maxDelta,maxDelta);
	int dy = glm::clamp(-(ny - MouseY),-maxDelta,maxDelta);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth()+dx*rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline()-dy*rate,-90.0f,90.0f));
	}
	if(RightDown) {
		const float rate=0.005f;
		float dist=glm::clamp(Cam->GetDistance()*(1.0f-dx*rate),0.01f,1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
