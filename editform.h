#ifndef EDITFORM_H
#define EDITFORM_H

#include <QDialog>

namespace Ui {
class EditForm;
}

class EditForm : public QDialog
{
    Q_OBJECT

public:
    explicit EditForm(QWidget *parent = nullptr);
    ~EditForm();

    QString getNewTaskName();
    void setOldTaskName(QString text);
private:
    Ui::EditForm *ui;
};

#endif // EDITFORM_H
