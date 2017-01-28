#include "guiaftm.h"


guiaFTM::guiaFTM(QWidget *parent) 
: QGLWidget(QGLFormat(QGL::AlphaChannel),parent)
{

}

guiaFTM::~guiaFTM()
{

}
void guiaFTM::initializeGL()
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
}
void guiaFTM::resizeGL(int width, int height)
{
	int side = qMin(width, height);
     glViewport((width - side) / 2, (height - side) / 2, side, side);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-1.8, +1.8, +1.8, -1.8, 4.0, 15.0);
     glMatrixMode(GL_MODELVIEW);
}
void guiaFTM::paintGL()
{
	//qglClearColor(clearColor);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();

     glTranslated(0.0, 0.0, -5.0);

	 glRotated(45.0, 1.0, 0.0, 0.0);
     glRotated(45.0, 0.0, 1.0, 0.0);
     glRotated(0, 0.0, 0.0, 1.0);
	
	 glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	 glClearColor(0.0, 1.0, 0.0, 1.0);

	 glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);       //cara frontal
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);       //cara trasera
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);       //cara lateral izq
    glVertex3f(-1.0f,-1.0f, -1.0f);
    glVertex3f(-1.0f,-1.0f,  1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);       //cara lateral dcha
    glVertex3f(1.0f, -1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glEnd(); 

    glColor3f(0.0f,      1.0f, 1.0f);
    glBegin(GL_QUADS);       //cara arriba
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();
 
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);       //cara abajo
    glVertex3f( 1.0f,-1.0f, -1.0f);
    glVertex3f( 1.0f,-1.0f,  1.0f);
    glVertex3f(-1.0f,-1.0f,  1.0f);
    glVertex3f(-1.0f,-1.0f, -1.0f);
    glEnd();


	
}
void guiaFTM::mousePressEvent(QMouseEvent *event)
{
	updateGL();
}
