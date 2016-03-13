#include "displaywindow.h"
#include "ui_displaywindow.h"
#include "CMidiPacket43.h"
#include <vector>
DisplayWindow::DisplayWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);
}

DisplayWindow::~DisplayWindow()
{
    delete ui;
}
void DisplayWindow::on_pushButton_close_display_clicked()
{
    this->close();
}

void DisplayWindow::set_display_text(std::vector<CMidiPacket43> mp_vector) {
    ui->plainTextEdit_display->clear();
    for(auto mp : mp_vector) {
        ui->plainTextEdit_display->appendPlainText(QString::fromStdString(mp.to_string()));
    }
}
