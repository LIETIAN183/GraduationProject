#include "myopenglwidget.h"
#include <iostream>
using namespace std;
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

void MyOpenGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    this->initializeOpenGLFunctions();




    // 创建顶点着色器
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vshader->compileSourceFile(":/shaders/vsrc.vert");
    // 创建片段着色器
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fshader->compileSourceFile(":/shaders/fsrc.frag");

    // 创建着色器程序
    program.addShader(vshader);
    //program.bindAttributeLocation("aPos", 0);
    program.addShader(fshader);
    program.link();
    assert(program.bind());
    // 三角形的顶点数据
    //const float triangle[] =
    GLfloat triangle[] =
    {
        //---- 位置 ----
        -0.5f, -0.5f, 0.0f, // 左下
            0.5f, -0.5f, 0.0f,// 右下
            0.0f, 0.5f, 0.0f// 正上
        };

    vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    vbo.create();
    vbo.bind();
    vbo.allocate(triangle, sizeof (triangle));
    vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    f->glEnableVertexAttribArray(0);
    vbo.release();
    program.release();
    /*
    // 生成并绑定 VBO
    GLuint vertex_buffer_object ;
    glGenBuffers (1, &vertex_buffer_object );
    glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer_object );
    // 将顶点数据绑定至当前默认的缓冲中
    glBufferData (GL_ARRAY_BUFFER, sizeof (triangle), triangle, GL_STATIC_DRAW );
    // 生成并绑定 VAO
    GLuint vertex_array_object ;
    glGenVertexArrays (1, &vertex_array_object );
    glBindVertexArray (vertex_array_object );
    // 设置顶点属性指针
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float ), (void*)0);
    glEnableVertexAttribArray (0);
    // 解绑VAO和VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    */


}

void MyOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void MyOpenGLWidget::paintGL()
{
    /*
    //设置背景颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    // 绘制三角形
    program.bind();
    glBindVertexArray (vertex_array_object);
    glDrawArrays (GL_TRIANGLES, 0, 3);//GL_POINTS无效果？
    glBindVertexArray (0);

    */
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    program.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    program.release();
}

void MyOpenGLWidget::TestFunction(int i)
{
    Q_UNUSED(i);
}
