#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // Order of the function calls seem to matter:

    // Stores the text change before clearing it
    // *TASK* Needs to be linked to the SUBMIT button instead of just the 'finished' signal [COMPLETED]
    connect(this, &QDialog::accepted, this, &Dialog::storeChanges);

    // Accept and Rejected States? Apparently, by linking it the slot accept, it will send the accepted signal -- who knew?!
    connect(ui->submit_btn, &QPushButton::pressed, this, &QDialog::accept);

    // Makes sure to clear on completion. What's the difference between the signals finished and done?
    connect(this, &QDialog::finished, ui->task_input, &QLineEdit::clear);

}

Dialog::~Dialog()
{
    delete ui;
}

// SLOT to set stored variables before clear.
void Dialog::storeChanges()
{
    Name = ui->task_input->text();
}

QString Dialog::getName()
{
    return Name;
}
