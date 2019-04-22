#include "myopenglwidget.h"
#include <iostream>
#include <math.h>
#include <QKeyEvent>
#include <QOpenGLTexture>

using namespace std;
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    this->size = 0;
    translate = -6.0;
    xRot = zRot = yRot = 0.0;
    mode = GL_FILL;
    this->Add_tex = false;
}

void MyOpenGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    this->initializeOpenGLFunctions();
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
    GLfloat triangle[] =
    {
        0.0f, 0.75f, 0.0f, 0.25f, 0.750f, 0.0f,
        0.5f, 0.75f, 0.0f, 0.75f, 0.75f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.25f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.75f, 0.5f, 0.0f,
        0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.0f,
        0.5f, 0.25f, 0.3f, 0.75f, 0.25f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.25f, 0.0f, 0.2f,
        0.5f, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f,

        0.0f, -0.75f, 0.0f, 0.25f, -0.75f, 0.0f,
        0.5f, -0.75f, 0.0f, 0.75f, -0.75f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.25f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.75f, -0.5f, 0.0f,
        0.0f, -0.25f, 0.0f, 0.25f, -0.25f, 0.0f,
        0.5f, -0.25f, 0.3f, 0.75f, -0.25f, 0.0f,
        0.0f, -0.0f, 0.0f, 0.25f, -0.0f, 0.2f,
        0.5f, -0.0f, 0.0f, 0.75f, -0.0f, 0.0f
    };
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(triangle, sizeof(triangle));
    //cout << "Size:" << static_cast<int>(triangle.size()*sizeof (GLfloat)) << endl;
    this->size = static_cast<int>(sizeof(triangle) / sizeof (GLfloat) / 3);
    cout << "Size:" << this->size << endl;

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
    glBufferData (GL_ARRAY_BUFFER, sizeof(TeapotVertices),
                  TeapotVertices, GL_STATIC_DRAW );

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(TeapotIndices),
                  TeapotIndices, GL_STATIC_DRAW );
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
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MyOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void MyOpenGLWidget::paintGL()
{


    //设置背景颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        glPolygonMode(GL_FRONT_AND_BACK, mode);
        if(Add_tex)
        {
            textures[0]->bind(0);
        }

        glDrawArrays(GL_PATCHES, 0, this->size);

        if(Add_tex)
        {
            textures[0]->release();
        }

        vao.release();
    }
    program.release();

}

void MyOpenGLWidget::Draw(vector<GLfloat> x, vector<GLfloat> tex, QImage texture)
{
    program.bind();
    vbo.bind();
    vao.bind();
    //需要额外分配空间给tex数组
    vbo.allocate(&x[0], static_cast<int>((x.size() + tex.size())*sizeof(GLfloat)));
    this->size = static_cast<int>(x.size() / 3);

    this->textures[0] = new QOpenGLTexture(texture.mirrored());
    this->Add_tex = true;

    vbo.write(static_cast<int>(x.size()*sizeof(GLfloat)), &tex[0], static_cast<int>(tex.size()*sizeof(GLfloat)));

    //GLuint vTexCoord = program.attributeLocation("vTexCoord");
    program.setAttributeBuffer(1, GL_FLOAT, static_cast<int>(x.size()*sizeof(GLfloat)), 2, 0);
    program.enableAttributeArray(1);
    program.setUniformValue("tex", 0);

    vao.release();
    vbo.release();
    program.release();

    translate = -6.0;
    xRot = zRot = yRot = 0.0;

    update();
    cout << "READ" << endl;


}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        xRot += 10;
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
    case Qt::Key_M:
        mode = (mode == GL_LINE ? GL_FILL : GL_LINE);
        break;
    default:
        break;
    }
    update();
    QOpenGLWidget::keyPressEvent(event);
}
