/***********
Oguzhan TURK
040080232
***********/

#include<math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include "Scene.h"
#include "ScreenSpaceShaderNode.h"
#include "NormalShaderNode.h"
#include "ColorFillNode.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define PI 3.14159265

GLfloat ballSpeed;


class myWindow : public cwc::glutWindow{
	GLfloat player1X, player1Y, player1Z, player2X, player2Y, player2Z, ballX, ballY, ballZ, angle;
	GLint direction;
	SceneNode *pad1, *pad2, *ball;
	Camera *camera;
	Scene *scene;
public:
	myWindow(){}

	virtual void OnRender(void){
		ball->translate(ballX, ballY, ballZ);
		pad1->translate(player1X, player1Y, player1Z);
		pad2->translate(player2X, player2Y, player2Z);
		glPopMatrix();
		if(direction==1){ /* Yon saga ise */
			ballX+=ballSpeed;
			ballY+=(ballSpeed*cos(angle*PI/180)/sin(angle*PI/360));
			if(ballX>=player2X-0.025 && ballY<=player2Y+0.225 && ballY>=player2Y-0.225){ /* x ekseninde yon degistir */
				direction=0;
				angle=90-(ballY-player2Y)*45/0.225;
			}
			else if(ballY>=0.99 || ballY<=-0.99) angle*=-1; /* y ekseninde yon degistir */
			else if(ballX>player2X+0.02) direction=2; /* Topu durdur */
		}
		else if(!direction){ /* Yon sola ise */
			ballX-=ballSpeed;
			ballY+=(ballSpeed*cos(angle*PI/180)/sin(angle*PI/360));
			if(ballX<=player1X+0.025 && ballY<=player1Y+0.225 && ballY>=player1Y-0.225){ /* x ekseninde yon degistir */
				direction=1;
				angle=90-(ballY-player1Y)*45/0.225;
			}
			else if(ballY>=0.99 || ballY<=-0.99) angle*=-1; /* y ekseninde yon degistir */
			else if(ballX<player1X-0.02) direction=2; /* Topu durdur */
		}
		glPopMatrix();
		glutSwapBuffers();
	}

	virtual void OnIdle(){
		pad1->draw(camera);
		pad2->draw(camera);
		ball->draw(camera);
		glutPostRedisplay();
	}

	virtual void OnInit(){
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glMatrixMode(GL_PROJECTION);
		player1X=-0.95;
		player1Y=0;
		player1Z=0;
		player2X=0.95;
		player2Y=0;
		player2Z=0;
		ballX=0;
		ballY=0;
		ballZ=0;
		direction=1;
		angle=90;
		ballSpeed=0.006;
		scene = new Scene();
		pad1 = new ScreenSpaceShaderNode("capsule.obj");
		pad2 = new ColorFillNode("capsule.obj");
		ball = new ColorFillNode("bunny.obj");
		pad1->scale(0.01,0.01,0.01);
		pad2->scale(0.01,0.01,0.01);
		scene->addNode(pad1);
		scene->addNode(pad2);
		scene->addNode(ball);
		camera = new Camera();
		camera->translate(0.f, 0.f, 3.f);
		pad1->translate(-100.f, 0.f, 0.f);
		pad2->translate( 100.f, 0.f, 0.f);
		scene->setCamera(camera);
	}

	virtual void OnResize(int width, int height){
		GLfloat fAspect;
		if(height==0) height=1;
		glViewport(0, 0, width, height);
		fAspect = (GLfloat)width/(GLfloat)height;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y){}    
	virtual void OnMouseUp(int button, int x, int y){}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii){       
	    switch(nKey){
		   case GLUT_KEY_DOWN: /* 2. oyuncu icin asagi tusu */
			   if(player2Y>-0.78) player2Y-=0.025;
			   break;
		   case GLUT_KEY_UP: /* 2. oyuncu icin yukari tusu */
			   if(player2Y<0.78) player2Y+=0.025;
			   break;
		}
		switch(cAscii){
		   case 27: /* Cikis */
			   this->Close();
			   break;
		   case 's': /* 1. oyuncu icin asagi tusu */
		   case 'S':
			   if(player1Y>-0.78) player1Y-=0.025;
			   break;
		   case 'w': /* 1. oyuncu icin yukari tusu */
		   case 'W':
			   if(player1Y<0.78) player1Y+=0.025;
			   break;
		   case 'r': /* Albastan */
		   case 'R':
			   OnInit();
			   break;
		}
	};

	static void Timer(int value){
		ballSpeed+=0.000001; /* Top zamanla hizlanir */
		glutPostRedisplay();
		glutTimerFunc(15, Timer, 1);
	}
};


class myApplication : public cwc::glApplication{
public:
	virtual void OnInit(){}
};



int main(void){
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();
	myWin->Timer(0);
	pApp->run();
	delete myWin;
	delete pApp;
	return 0;
}
