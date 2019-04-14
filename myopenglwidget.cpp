#include "myopenglwidget.h"
#include <iostream>
using namespace std;
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    this->size = 0;
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
    program.addShader(fshader);
    program.link();
    program.bind();
    // 三角形的顶点数据
    GLfloat x[] =
    {
        -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };
    vector<GLfloat> triangle;
    for(int i = 0 ; i < sizeof (x) / sizeof (GLfloat); i++)
    {
        triangle.push_back(x[i]);
    }
    vbo.create();
    vbo.bind();
    vbo.allocate(&triangle[0], triangle.size()*sizeof (GLfloat));
    this->size = static_cast<int>(triangle.size() / 3);

    vao.create();
    vao.bind();
    //QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    // f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // f->glEnableVertexAttribArray(0);


    program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    program.enableAttributeArray(0);

    //释放
    vao.release();
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

    //设置背景颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制三角形
    program.bind();
    {
        vao.bind();
        glDrawArrays(GL_TRIANGLE_FAN, 0, this->size);
        vao.release();
    }
    program.release();
}

void MyOpenGLWidget::TestFunction(vector<GLfloat> x)
{
    //program.bind();
    for(vector<GLfloat>::iterator i = x.begin(); i != x.end(); i++)
    {
        cout << *i << endl;
    }
    vbo.bind();
    vbo.allocate(&x[0], x.size()*sizeof(GLfloat));
    this->size = static_cast<int>(x.size() / 3);
    vbo.release();
    //program.release();
    update();
    cout << "READ" << endl;
}
