#ifndef POINT3FDIALOG_H
#define POINT3FDIALOG_H

#include <QDialog>

namespace Ui
{
class Point3fDialog;
}

class Point3fDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Point3fDialog(QWidget *parent = nullptr);
    ~Point3fDialog();
    void SetNumber(int i, int number);
    int getNumber(int i);
protected slots:
    void OK();
    void Cancel();
private:
    Ui::Point3fDialog *ui;
};

#endif // POINT3FDIALOG_H
