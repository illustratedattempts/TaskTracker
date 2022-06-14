#include "editform.h"
#include "ui_editform.h"

EditForm::EditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditForm)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::pressed, this, &QDialog::accept);
    connect(ui->pushButton_2, &QPushButton::pressed, this, &QDialog::reject);
}

EditForm::~EditForm()
{
    delete ui;
}

QString EditForm::getNewTaskName()
{
    return ui->lineEdit->text();
}

void EditForm::setOldTaskName(QString text)
{
    ui->lineEdit->setText(text);
}
