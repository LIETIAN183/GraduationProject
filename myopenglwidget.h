#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <vector>
using namespace std;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    void Draw(vector<GLfloat> x, vector<GLfloat> tex, QImage texture);
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
    int size;//控制glDrawArrays点数量自动更新

    GLfloat translate, xRot, yRot, zRot;//视觉转换
    GLenum mode;

    bool Add_tex;
    QOpenGLTexture *textures[2];
};

#endif // MYOPENGLWIDGET_H
