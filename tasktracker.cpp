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
    connect(ui->listWidget, &QWidget::customContextMenuRequested, this, &TaskTracker::showContextMenu_listWidget);

    // For if QListWidget is checked
    connect(ui->listWidget, &QListWidget::itemChanged, this, &TaskTracker::taskChecked);

    // QListView, trying to add a context menu to it
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, &QWidget::customContextMenuRequested, this, &TaskTracker::showContextMenu_listView);

    //*TASK 1* When the user check marks the item, I want the item to then be moved to the List View [COMPLETED] =)

    model = new QStringListModel(this);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //*TASK 2* (Abbreviation on previous task) Once the user check marks the item, the item will no longer exist in the QListWidget and only exist in the QListView [COMPLETED] =)


    //*TASK 3* After the item is moved to ListView, we can allow the user to bring back the item (Really vague task). I need to bring in a context menu in the ListView that can interact
    //with the model to delete the item. We can work on moving it back later. Although the process of moving it back should be similar to 'task 2'...who knows. [Completed] [Completed the moving back as well] =)

    //*TASK 4* Need to add ListWidgetItem context menu actions using itemAt() for EDIT and 'TRUE' DELETE

    //*TASK 5* Have some sort of alarm timer (Vague because there's definitely going to be more subtasks for this)

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
       // Takes the item from the ListWidget
       item->setCheckState(Qt::Unchecked);
       ui->listWidget->takeItem(ui->listWidget->row(item));

   }
}

// Meant for deletion of task, might not be used at all lol
void TaskTracker::deleteData()
{

}

void TaskTracker::showContextMenu_listWidget(const QPoint &pos)
{
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Something", this, &TaskTracker::deleteData);
    myMenu.addAction("Hello", this, &TaskTracker::deleteData);

    myMenu.exec(globalPos);
}

void TaskTracker::showContextMenu_listView(const QPoint &pos)
{
    QPoint globalPos = ui->listView->mapToGlobal(pos);

    QMenu myMenu;
    // I might need to use a Signal Mapper as each item corresponds to differently. Nvm, I am going to use a lambda function.
    QModelIndex index = ui->listView->indexAt(pos);
    QString itemText = index.data(Qt::DisplayRole).toString();
    QAction* return_task = myMenu.addAction("Return Task");
    connect(return_task, &QAction::triggered, this, [=](){ this->placeBackTask(itemText); });
    connect(return_task, &QAction::triggered, this, [=](){ this->deleteFromListView(index.row()); });

    myMenu.exec(globalPos);
}

// Function should go through the vector and look for it.
void TaskTracker::placeBackTask(QString task)
{
    QListWidgetItem* item = nullptr;
    for(QListWidgetItem* i: tasks)
    {
        if(task == i->text())
        {
            item = i;
            break;
        }
    }
    ui->listWidget->addItem(item);
}

void TaskTracker::deleteFromListView(int row)
{
    model->removeRow(row);
}
