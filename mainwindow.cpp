#ifndef CBLUESDRUMTRACK_H
#include "CBluesDrumTrack.h"
#endif
#ifndef CTRACK_1_H
#include "CTrack_1.h"
#endif
#ifndef FUNCTOR_H_
#include "functorTests.h"
#endif
#include <cstdlib>
#include <numeric>
#include <QTimer>
#include <QMessageBox>
#include "RtMidi.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <limits>
#include <QDebug>

RtMidiOut *midiout = 0;
std::vector<CMidiPacket43> play_trk;  // chrono vectors
std::vector<CMidiPacket43>::iterator cur_pkt;
std::vector<CMidiPacket43>::iterator next_pkt;
std::vector<std::string> cur_input_note_string;
std::vector<std::string> track_1_notes;

QStringList g_piano_list;
QStringList g_chroma_list;
QStringList g_organ_list;
QStringList g_guitar_list;
QStringList g_bass_list;
QStringList g_strings_list;
QStringList g_ensemble_list;
QStringList g_brass_list;
QStringList g_reed_list;
QStringList g_pipe_list;
QStringList g_synth_lead_list;
QStringList g_synth_pad_list;
QStringList g_synth_effect_list;

void write_track_1() {  // P I A N O
  uint16_t start_note = 48;
  uint16_t chan = 1;
  uint16_t patch = 0;  // piano
  uint16_t vol = 110;
  uint16_t pan = 80;  // pan right
  CTrack_1 trk_1(1);
  trk_1.write_track(cur_input_note_string, chan, patch, vol, pan);
  std::copy(trk_1.m_trk.begin(), trk_1.m_trk.end(),
            std::back_inserter(play_trk));
}
void write_drums() {  // D R U M S
  uint16_t vol = 100;
  CBluesDrumTrack drum_trk(1);
  drum_trk.write_track(100, 36, "11111111");
  std::copy(drum_trk.m_trk.begin(), drum_trk.m_trk.end(),
            std::back_inserter(play_trk));
}






bool chooseMidiOutPort(RtMidiOut *rtmidi) {
  unsigned int nPorts = rtmidi->getPortCount();
  if (nPorts == 0) {
    std::cout << "No output ports available!" << std::endl;
    return false;
  }

  const std::string kLOOK_FOR_NAME = "DLSMusicDevice";
  std::string portName;

  uint32_t dls_port = 0xBAD;

  for (unsigned int i = 0; i < nPorts; i++) {
    portName = rtmidi->getPortName(i);
    if (portName == kLOOK_FOR_NAME) dls_port = i;
  }

  if (dls_port == 0xBAD) return false;

  rtmidi->openPort(dls_port);
  return true;
}

void sendCMidiPacket(const CMidiPacket43 &mp) {
  std::vector<unsigned char> message;
  message.push_back(mp.get_status());
  message.push_back(mp.get_data1());
  if (mp.get_length() == 3) message.push_back(mp.get_data2());
  midiout->sendMessage(&message);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setup_comboBox_category();
    setup_instru_name_list();
    try {
      midiout = new RtMidiOut();
    }
    catch (RtMidiError &error) {
      error.printMessage();
      exit(EXIT_FAILURE);
    }

    // Call function to select port.
    try {
      if (chooseMidiOutPort(midiout) == false) {
        delete midiout;
        std::exit(0);
      }
    }
    catch (RtMidiError &error) {
      delete midiout;
      std::exit(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::txTimerAction() {
  sendCMidiPacket(*cur_pkt);
  (*cur_pkt) = (*next_pkt);
  next_pkt++;
  QTimer *timer = qobject_cast<QTimer *>(sender());

  if (cur_pkt == play_trk.end()-1) {
    timer->stop();
    return;
  }
  auto delay =
      ((*next_pkt).get_timestamp() - (*cur_pkt).get_timestamp());
  timer->start(delay);
  if (next_pkt != play_trk.end()) {
    cur_pkt++;
  }
}


void MainWindow::on_pushButton_play_clicked()
{
    //CMidiPacket43 mp1{0,0x90,60,100};
    //CMidiPacket43 mp2{5000,0x80,60,0};
    //sendCMidiPacket(mp1);
    //sendCMidiPacket(mp2);

    //write_drums();

    // sort the play_trk using CMidiPacket43::operator<
    std::sort(play_trk.begin(), play_trk.end());
    cur_pkt = play_trk.begin();
    next_pkt = play_trk.begin();
    for(CMidiPacket43 mp : play_trk) {
        std::cout << mp;
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(txTimerAction()));
    timer->start();
}

void MainWindow::on_pushButton_Enter_clicked()
{
    cur_input_note_string.push_back(ui->lineEdit_0->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_1->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_2->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_3->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_4->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_5->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_6->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_7->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_8->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_9->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_10->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_11->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_12->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_13->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_14->text().toStdString());
    cur_input_note_string.push_back(ui->lineEdit_15->text().toStdString());
    write_track_1();
}




















//interface stuffs, easy to understand
void MainWindow::on_horizontalSlider_tempo_valueChanged(int value)
{
    ui->label_tempo->setText(QString::number(value));
}

void MainWindow::on_horizontalSlider_1_vol_valueChanged(int value)
{
    ui->label_1_vol->setText(QString::number(value));
}

void MainWindow::on_horizontalSlider_2_vol_valueChanged(int value)
{
    ui->label_2_vol->setText(QString::number(value));
}


void MainWindow::on_horizontalSlider_3_vol_valueChanged(int value)
{
    ui->label_3_vol->setText(QString::number(value));
}


void MainWindow::on_horizontalSlider_4_vol_valueChanged(int value)
{
    ui->label_4_vol->setText(QString::number(value));
}


void MainWindow::on_horizontalSlider_5_vol_valueChanged(int value)
{
    ui->label_5_vol->setText(QString::number(value));
}

void MainWindow::setup_comboBox_category(){
    QStringList categories;
    categories << "<None>"
          << "1 Piano"
          << "2 Chromatic Percussion"
          << "3 Organ"
          << "4 Guitar"
          << "5 Bass"
          << "6 Strings"
          << "7 Ensemble"
          << "8 Brass"
          << "9 Reed"
          << "10 Pipe"
          << "11 Synth Lead"
          << "12 Synth Pad"
          << "13 Synth Effects"
          << "14 Ethnic"
          << "15 Percussive"
          << "16 Sound Effects";
    ui->comboBox_1_cat->addItems(categories);
    ui->comboBox_2_cat->addItems(categories);
    ui->comboBox_3_cat->addItems(categories);
    ui->comboBox_4_cat->addItems(categories);
    ui->comboBox_5_cat->addItems(categories);
}


void MainWindow::setup_instru_name_list() {


  g_piano_list<<"<None>"
          <<"1 Acoustic Grand Piano"
          <<"2 Bright Acoustic Piano"
          <<"3 Electric Grand Piano"
          <<"4 Honky-tonk Piano"
          <<"5 Electric Piano 1"
          <<"6 Electric Piano 2"
          <<"7 Harpsichord"
          <<"8 Clavinet";
  g_chroma_list<<"<None>"
                  <<"9 Celesta"
                  <<"10 Glockenspiel"
                  <<"11 Music Box"
                  <<"12 Vibraphone"
                  <<"13 Marimba"
                  <<"14 Xylophone"
                  <<"15 Tubular Bells"
                  <<"16 Dulcimer";
  g_organ_list<<"<None>";
  /**g_guitar_list;
  g_bass_list;
  g_strings_list;
  g_ensemble_list;
  g_brass_list;
  g_reed_list;
  g_pipe_list;
  g_synth_lead_list;
  g_synth_pad_list;
  g_synth_effect_list;**/
    ui->comboBox_1_instru->addItems(g_piano_list);
}
