#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <vector>
using namespace std;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    void Draw(vector<GLfloat> x);
    void keyPressEvent(QKeyEvent *event);


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

signals:

public slots:
private:
    QOpenGLShaderProgram program;
    GLuint vertex_array_object ;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int size;

    GLfloat translate, xRot, yRot, zRot;
    //QMatrix4x4 matrix;
};

#endif // MYOPENGLWIDGET_H
