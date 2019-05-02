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
    mode = GL_FILL;
    this->Add_tex = false;
    this->Light = false;

    cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    cameraUp = QVector3D(0.0f, 1.0f,  0.0f);

    //不可以在此处计算projection矩阵，因为此时窗口的width和height不是最终结果
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

        QMatrix4x4 projection;//在此处计算projection因为此时width和height才能返回正确值
        projection.perspective(45.0f, static_cast<float>(width()) / static_cast<float>(height()), 0.1f, 100.0f);

        QMatrix4x4 view;
        view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //绑定矩阵
        program.setUniformValue("projection", projection);
        program.setUniformValue("view", view);
        program.setUniformValue("model", model);

        //设置光照
        program.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
        program.setUniformValue("inverse", projection.inverted());
        program.setUniformValue("frag_view", view);
        program.setUniformValue("frag_model", model);
        program.setUniformValue("lightPos", QVector3D(0.0f, 0.0f, 3.0f));
        program.setUniformValue("viewPos", cameraPos);
        program.setUniformValue("IsLight", this->Light);


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

    program.setAttributeBuffer(1, GL_FLOAT, static_cast<int>(x.size()*sizeof(GLfloat)), 2, 0);
    program.enableAttributeArray(1);
    program.setUniformValue("tex", 0);

    vao.release();
    vbo.release();
    program.release();

    //重置view和model矩阵

    //不可删除，否则图片不刷新
    update();
    //消除刷新后的颜色异常
    glFlush();

}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ShiftModifier)
    {
        switch (event->key())
        {
        case Qt::Key_X:
            model.rotate(-10.0f, 1.0f, 0.0f, 0.0f);
            break;
        case Qt::Key_Y:
            model.rotate(-10.0f, 0.0f, 1.0f, 0.0f);
            break;
        case Qt::Key_Z:
            model.rotate(-10.0f, 0.0f, 0.0f, 1.0f);
            break;
        default:
            break;
        }

    }
    else//未按下Control键
    {
        GLfloat cameraSpeed = 0.1f;
        switch (event->key())
        {
        case Qt::Key_X:
            model.rotate(10.0f, 1.0f, 0.0f, 0.0f);
            break;
        case Qt::Key_Y:
            model.rotate(10.0f, 0.0f, 1.0f, 0.0f);
            break;
        case Qt::Key_Z:
            model.rotate(10.0f, 0.0f, 0.0f, 1.0f);
            break;
        case Qt::Key_R://重置
            cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
            cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
            cameraUp = QVector3D(0.0f, 1.0f,  0.0f);
            model = QMatrix4x4();
            break;
        case Qt::Key_P://线框模式
            mode = (mode == GL_LINE ? GL_FILL : GL_LINE);
            break;
        case Qt::Key_S:
            cameraPos -= cameraSpeed * cameraUp;
            break;
        case Qt::Key_W:
            cameraPos += cameraSpeed * cameraUp;
            break;
        case Qt::Key_D:
            cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
            break;
        case Qt::Key_A:
            cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
            break;
        case Qt::Key_M:
            cameraPos += cameraSpeed * cameraFront;
            break;
        case Qt::Key_N:
            cameraPos -= cameraSpeed * cameraFront;
            break;
        case Qt::Key_L:
            Light = (Light == true ? false : true);
            break;
        default:
            break;
        }
    }

    update();
    QOpenGLWidget::keyPressEvent(event);
}
