#include "tasktracker.h"
#include "ui_tasktracker.h"

TaskTracker::TaskTracker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TaskTracker)
{
    ui->setupUi(this);


    // MODAL -- Prioritizes this Dialog Window over all others
    // Dialog Option to submit new task inputs
    inputForm = new Dialog(this);
    inputForm->setWindowTitle("Input Form");
    inputForm->setModal(true);

    // Shows the dialog box to input stuff
    connect(ui->addTask_btn, &QPushButton::pressed, inputForm, &QDialog::show);

    // QDialog form submission good
    connect(inputForm, &QDialog::accepted, this, &TaskTracker::addData);

    // QListWidget, let's try to add a context menu to it
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QWidget::customContextMenuRequested, this, &TaskTracker::showContextMenu);

    //*TASK 1* When the user check marks the item, I want the item to then be moved to the List View
    //*TASK 2* After the item is moved to ListView, we can allow the user to bring back the item

}

TaskTracker::~TaskTracker()
{
    delete ui;
}

// Adds to ListWidget, sets Checkable, and Appends to a 'tasks' list
// I created the vector to move it around to another listWidget for completion.
// Calls
void TaskTracker::addData()
{
    QListWidgetItem *some_task = new QListWidgetItem(inputForm->getName(), ui->listWidget);
    some_task->setCheckState(Qt::Unchecked);
    ui->listWidget->addItem(some_task);
    tasks.append(some_task);
}

void TaskTracker::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Something", this, &TaskTracker::deleteData);
    myMenu.addAction("Hello", this, &TaskTracker::deleteData);

    myMenu.exec(globalPos);
}

// Meant for deletion of task
void TaskTracker::deleteData()
{

}
