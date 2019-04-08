#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

signals:

public slots:
};

#endif // MYOPENGLWIDGET_H
