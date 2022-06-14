#ifndef TASKTRACKER_H
#define TASKTRACKER_H

#include <QMainWindow>
#include <QDialog>
#include <QCheckBox>
#include <QGridLayout>
#include <QAbstractButton>
#include <QLabel>
#include <QListWidgetItem>
#include <QStringListModel>
#include <QMessageBox>

#include "dialog.h"
#include "editform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TaskTracker; }
QT_END_NAMESPACE

class TaskTracker : public QMainWindow
{
    Q_OBJECT

public:
    TaskTracker(QWidget *parent = nullptr);
    ~TaskTracker();
    Dialog *inputForm;
    EditForm *editForm;

private:
    Ui::TaskTracker *ui;
    QList<QListWidgetItem*> tasks; // Highly suggested by https://stackoverflow.com/questions/6602036/qvector-vs-qlist
    QStringListModel* model;

    void placeBackTask(QString task);
    void deleteFromListView(int row);
    void setNewTaskName(const EditForm &form);

private slots:
    void addData();
    void deleteTask(QListWidgetItem* item);
    void taskChecked(QListWidgetItem* item);

    void showContextMenu_listWidget(const QPoint &pos);
    void showContextMenu_listView(const QPoint &pos);
};
#endif // TASKTRACKER_H
