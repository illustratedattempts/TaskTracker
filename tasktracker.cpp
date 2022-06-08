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
    connect(ui->listWidget, &QListWidget::itemChanged, this, &TaskTracker::taskChecked);
    //*TASK 1* When the user check marks the item, I want the item to then be moved to the List View

    model = new QStringListModel(this);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //*TASK 2* (Abbreviation on previous task) Once the user check marks the item, the item will no longer exist in the QListWidget and only exist in the QListView

    //*TASK 3* After the item is moved to ListView, we can allow the user to bring back the item (Really vague task). I need to bring in a context menu in the ListView that can interact
    //model to delete the item. We can work on moving it back later. Although the process of moving it back should be similar to 'task 2'...who knows.

    //*TASK 4* Have some sort of alarm timer

}

TaskTracker::~TaskTracker()
{
    delete ui;
}

// Adds to ListWidget, sets Checkable, and Appends to a 'tasks' list
// I created the vector to move it around to another listWidget for completion.
void TaskTracker::addData()
{
    QListWidgetItem *some_task = new QListWidgetItem(inputForm->getName(), ui->listWidget);
    some_task->setCheckState(Qt::Unchecked);
    ui->listWidget->addItem(some_task);



    tasks.append(some_task);
}

// To check which item is checked
void TaskTracker::taskChecked(QListWidgetItem *item)
{
   if(item->checkState() ==  Qt::Checked)
   {
       // Add a new row to the model
       model->insertRow(model->rowCount());
       // Get the last index
       QModelIndex index = model->index(model->rowCount()-1);
       // Change the data
       model->setData(index, item->text());
   }
}

// Meant for deletion of task, might not be used at all lol
void TaskTracker::deleteData()
{

}
// Founds on Stack Overflow and QT Forums, modify as necessary
void TaskTracker::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Something", this, &TaskTracker::deleteData);
    myMenu.addAction("Hello", this, &TaskTracker::deleteData);

    myMenu.exec(globalPos);
}


