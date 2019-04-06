#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>

class MyOpenGLWidget : public QObject
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MYOPENGLWIDGET_H