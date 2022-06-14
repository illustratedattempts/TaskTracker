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

    //*TASK 4* Need to add ListWidgetItem context menu actions using itemAt() for EDIT and 'TRUE' DELETE [Completed] =)
    //*TASK 4a* Add Delete Warning Notifier [Done with message box] =)
    //*TASK 4b* EDIT Dialog Input Form [Completed] =)
    editForm = new EditForm(this);
    editForm->setWindowTitle("Edit Form");
    editForm->setModal(true);
    //*TASK 4b-a* Dialog box gets old task name from the ListWidget [Completed] =)
    //*TASK 4b-b* Dialog box sends over new task name [Completed] =)

    //*TASK 5* Have some sort of alarm timer (Vague because there's definitely going to be more subtasks for this)
    //*TASK 5a* The times need to be a combobox. Question: 24hr or 12hr time? 12hr because I'm not fancy.
    //*TASK 5b* Sort the list based off of time to do task
    //*TASK OPTIONAL* Allow the tasks to not have an have an associated time and push to the bottom
    // Hash data structure??? Item pointer as key and timer as value? Maybe.
    //*TASK 5c* Modify the ListView search perhaps to find item with the alarm timer through binary search
    //*TASK 6* Get the system timer, need to make several assumptions and observations for correct implementation -- will abbreviate later on.
    // Threads???

    // Bug report: 2 tasks that share the same name are changed when using the edit button, why? What about the transitional and removal stuff? Deletion shouldn't do anything like that is my guess.

    // FUTURE STUFF:
    // These comments should have been multi-line comments instead and not single line ones.
    // Too lazy right now.
    // Implement a pomodoro timer application. Practice with QTimer.
    // Have a calendar application. Practice with QCalendarView.
    // Combine all of these applications to create one big one.

    // REALLY IMPORTANT SHORT TERM WISE...make sure that keyboard shortcuts are implemented correctly. I haven't bothered to deal with copy, paste, and CTRL+C, CTRL+V, CTRL+X, and ENTER.

    // Note that I haven't implemented any keyboard inputs for the submission action. For example the enter key is not valid and will crash the program.

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
    some_task->setFlags(some_task->flags() | Qt::ItemIsEditable);
    ui->listWidget->addItem(some_task);

    // Add to QList
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
void TaskTracker::deleteTask(QListWidgetItem *item)
{
    // Create a new message warning box
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Verify Deletion", "Are you sure you want to delete this task?", QMessageBox::Yes | QMessageBox::No);
    if(confirmation == QMessageBox::Yes)
    {
        // Remove item from vector
        tasks.removeOne(item);
        // Remove it from ListWidget like in checked
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }

}

void TaskTracker::showContextMenu_listWidget(const QPoint &pos)
{
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);

    QMenu myMenu;
    QAction* delete_task = myMenu.addAction("Remove Task");
    QAction* edit_task = myMenu.addAction("Edit Task");

    /*
     * Interestingly if I click on a widget item that does not exist it will crash for obvious reasons -- it doesn't exist.
     * I need something to catch it just before and see that itemAt at that position is null and not show anything.
    */
    if(ui->listWidget->itemAt(pos) != nullptr)
    {
        QListWidgetItem* itemToChange = ui->listWidget->itemAt(pos);
        connect(delete_task, &QAction::triggered, this, [=](){ this->deleteTask(itemToChange); });
        connect(edit_task, &QAction::triggered, this, [=]() { editForm->setOldTaskName(itemToChange->text()); editForm->show(); });
        connect(editForm, &QDialog::accepted, this, [=](){ itemToChange->setText(editForm->getNewTaskName()); });
        myMenu.exec(globalPos);
    }
}

void TaskTracker::showContextMenu_listView(const QPoint &pos)
{
    QPoint globalPos = ui->listView->mapToGlobal(pos);

    QMenu myMenu;
    // I might need to use a Signal Mapper as each item corresponds to differently. Nvm, I am going to use a lambda function.
    // I should learn to write correct lambda functions. **IMPORTANT**
    if(ui->listView->indexAt(pos).isValid())
    {
        QModelIndex index = ui->listView->indexAt(pos);
        QString itemText = index.data(Qt::DisplayRole).toString();
        QAction* return_task = myMenu.addAction("Return Task");
        connect(return_task, &QAction::triggered, this, [=](){ this->placeBackTask(itemText); });
        connect(return_task, &QAction::triggered, this, [=](){ this->deleteFromListView(index.row()); });
        myMenu.exec(globalPos);
    }
}

// Function should go through the QList and look for it.
// There might a design flaw here with the deletion operation but I'm not sure what it is right now.
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
