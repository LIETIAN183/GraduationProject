#include "myopenglwidget.h"
#include <iostream>
#include <QKeyEvent>
using namespace std;
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    this->size = 0;
    translate = -6.0;
    xRot = zRot = yRot = 0.0;
}

void MyOpenGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    this->initializeOpenGLFunctions();
    //this->glEnable(GL_DEPTH_TEST);
    // 创建顶点着色器
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vshader->compileSourceFile(":/shaders/vsrc.vert");
    // 创建细分控制着色器
    QOpenGLShader *tcs_shader = new QOpenGLShader(QOpenGLShader::TessellationControl, this);
    tcs_shader->compileSourceFile(":/shaders/tcs.cont");
    //创建细分计算着色器
    QOpenGLShader *tes_shader = new QOpenGLShader(QOpenGLShader::TessellationEvaluation, this);
    tes_shader->compileSourceFile(":/shaders/tes.eval");
    // 创建片段着色器
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fshader->compileSourceFile(":/shaders/fsrc.frag");
    // 创建着色器程序
    program.addShader(vshader);

    program.addShader(tcs_shader);
    program.addShader(tes_shader);

    program.addShader(fshader);
    program.link();
    program.bind();
    // 三角形的顶点数据
    /*
        vector<GLfloat> triangle =
        {
            //-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f
            1.4f / 4, 2.4f / 4, 0.0,
            1.4f / 4, 2.4f / 4, 0.784f / 4,
            0.784f / 4, 2.4f / 4, 1.4f / 4,
            0.0, 2.4f / 4, 1.4f / 4,
            1.3375f / 4, 2.53125f / 4, 0.0,
            1.3375f / 4, 2.53125f / 4, 0.749f / 4,
            0.749f / 4, 2.53125f / 4, 1.3375f / 4,
            0.0f / 4, 2.53125f / 4, 1.3375f / 4,
            1.4375f / 4, 2.53125f / 4, 0.0,
            1.4375f / 4, 2.53125f / 4, 0.805f / 4,
            0.805f / 4, 2.53125f / 4, 1.4375f / 4,
            0.0, 2.53125f / 4, 1.4375f / 4,
            1.5f / 4, 2.4f / 4, 0.0,
            1.5f / 4, 2.4f / 4, 0.84f / 4,
            0.84f / 4, 2.4f / 4, 1.5f / 4,
            0.0, 2.4f / 4, 1.5f / 4
        };
        */
    vector<GLfloat> triangle =
    {
        -1.5, -1.5, 4.0, -0.5, -1.5, 2.0,
            0.5, -1.5, -1.0, 1.5, -1.5, 2.0,
            -1.5, -0.5, 1.0, -0.5, -0.5, 3.0,
            0.5, -0.5, 0.0, 1.5, -0.5, -1.0,
            -1.5, 0.5, 4.0, -0.5, 0.5, 0.0,
            0.5, 0.5, 3.0, 1.5, 0.5, 4.0,
            -1.5, 1.5, -2.0, -0.5, 1.5, -2.0,
            0.5, 1.5, 0.0, 1.5, 1.5, -1.0
        };
    vbo.create();
    vbo.bind();
    vbo.allocate(&triangle[0], static_cast<int>(triangle.size()*sizeof (GLfloat)));
    this->size = static_cast<int>(triangle.size() / 3);
    cout << "Size:" << size << endl;

    vao.create();
    vao.bind();
    //QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    // f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // f->glEnableVertexAttribArray(0);


    program.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    program.enableAttributeArray(0);
    glPatchParameteri(GL_PATCH_VERTICES, 16);
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


    //线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
        QMatrix4x4 matrix;
        matrix.perspective(45.0f, static_cast<GLfloat>(width())  / static_cast<GLfloat>(height()), 0.1f, 100.0f);
        matrix.translate(0, 0, translate);
        matrix.rotate(xRot, 1.0, 0.0, 0.0);
        matrix.rotate(yRot, 0.0, 1.0, 0.0);
        matrix.rotate(zRot, 0.0, 0.0, 1.0);
        program.setUniformValue("matrix", matrix);
        //glPointSize(5);
        glDrawArrays(GL_PATCHES, 0, this->size);//GL_POINTS时未设置glPointSize大小，则点会太小而无法看见
        vao.release();
    }
    program.release();
}

void MyOpenGLWidget::Draw(vector<GLfloat> x)
{
    for(vector<GLfloat>::iterator i = x.begin(); i != x.end(); i++)
    {
        cout << *i << endl;
    }
    vbo.bind();
    vbo.allocate(&x[0], static_cast<int>(x.size()*sizeof(GLfloat)));
    this->size = static_cast<int>(x.size() / 3);
    vbo.release();
    update();
    cout << "READ" << endl;
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        xRot += 10;
        cout << "Pressed Key_Up" << endl;
        break;
    case Qt::Key_Left:
        yRot += 10;
        break;
    case Qt::Key_Right:
        zRot += 10;
        break;
    case Qt::Key_Down:
        translate -= 1;
        break;
    case Qt::Key_Space:
        translate += 1;
        break;
    case Qt::Key_R:
        translate = -6.0;
        xRot = zRot = yRot = 0.0;
        break;
    default:
        break;
    }
    update();
    QOpenGLWidget::keyPressEvent(event);
}
