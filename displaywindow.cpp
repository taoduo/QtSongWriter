#include "displaywindow.h"
#include "ui_displaywindow.h"
#include "CMidiPacket43.h"
#include <vector>
#include <QDebug>
int g_chan[5] = {1,1,1,1,1};
int g_note_count[5] = {0,0,0,0,0};
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

int get_trk_num(int chan) {
    for(int i = 0; i < 5; i++) {
        if(g_chan[i] == chan) {
            return i;
        }
    }
    return -1;
}

void DisplayWindow::set_display_text(std::vector<CMidiPacket43> mp_vector) {
    for(int i = 0; i < 5; i++) {
        g_note_count[i] = 0;
    }
    ui->plainTextEdit_display->clear();
    int trk;
    int max_count = -1;
    for(auto mp : mp_vector) {
        ui->plainTextEdit_display->appendPlainText(QString::fromStdString(mp.to_string()));
        trk = get_trk_num(mp.get_status() % 16);
        if(trk != -1 && mp.is_status_9n()) {
            g_note_count[trk] = g_note_count[trk] + 1;
        }
    }
    //get maximum of note count
    for (int i = 0; i < 5; i++) {
        if(g_note_count[i] > max_count) {
            max_count = g_note_count[i];
        }
    }
    ui->progressBar_1->setMaximum(max_count);
    ui->progressBar_2->setMaximum(max_count);
    ui->progressBar_3->setMaximum(max_count);
    ui->progressBar_4->setMaximum(max_count);
    ui->progressBar_5->setMaximum(max_count);
    ui->progressBar_1->setValue(g_note_count[0]);
    ui->progressBar_2->setValue(g_note_count[1]);
    ui->progressBar_3->setValue(g_note_count[2]);
    ui->progressBar_4->setValue(g_note_count[3]);
    ui->progressBar_5->setValue(g_note_count[4]);
    ui->label_length_1->setText(QString::number(g_note_count[0]));
    ui->label_length_2->setText(QString::number(g_note_count[1]));
    ui->label_length_3->setText(QString::number(g_note_count[2]));
    ui->label_length_4->setText(QString::number(g_note_count[3]));
    ui->label_length_5->setText(QString::number(g_note_count[4]));
}

void DisplayWindow::set_channels(int chan[]) {
    for(int i = 0; i < 5; i++) {
        g_chan[i] = chan[i];
    }
}

