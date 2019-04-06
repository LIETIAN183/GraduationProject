#ifndef PROCESSPICTURE_H
#define PROCESSPICTURE_H

#include <QObject>

class ProcessPicture : public QObject
{
    Q_OBJECT
public:
    explicit ProcessPicture(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PROCESSPICTURE_H