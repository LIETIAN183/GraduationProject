#include "myopenglwidget.h"
#include <iostream>
using namespace std;
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void MyOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width / 2, width / 2, -height / 2, height / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void MyOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-5, -5, 0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f( 5, -5, 0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f( 0.0,  5, 0);
    glEnd();

}

void MyOpenGLWidget::TestFunction(int i)
{
    cout << "OpenGL:" << i << endl;
}
