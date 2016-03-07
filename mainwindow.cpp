#ifndef CBLUESDRUMTRACK_H
#include "CBluesDrumTrack.h"
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
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

  if (cur_pkt == play_trk.end()) {
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

    write_drums();

    // sort the play_trk using CMidiPacket43::operator<
    std::sort(play_trk.begin(), play_trk.end());
    cur_pkt = play_trk.begin();
    next_pkt = play_trk.begin();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(txTimerAction()));
    timer->start();
}
