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
#include <vector>
#include <limits>
#include <QDebug>

RtMidiOut *midiout = 0;
std::vector<CMidiPacket43> play_trk;  // chrono vectors
std::vector<CMidiPacket43>::iterator cur_pkt;
std::vector<CMidiPacket43>::iterator next_pkt;
std::vector<std::string> cur_input_note_string;
std::vector<std::string> track_1_notes;
std::vector<std::string> track_2_notes;
std::vector<std::string> track_3_notes;
std::vector<std::string> track_4_notes;
std::vector<std::string> track_5_notes;

std::vector<QStringList> g_all_lists;
QStringList g_none_list;
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
QStringList g_percussive_list;
QStringList g_ethnic_list;
QStringList g_effects_list;

int g_tempo = 60;
int g_vol_1 = 100;
int g_vol_2 = 100;
int g_vol_3 = 100;
int g_vol_4 = 100;
int g_vol_5 = 100;
int g_current_track = 1;

int g_track_1_patch;
int g_track_2_patch;
int g_track_3_patch;
int g_track_4_patch;
int g_track_5_patch;

int g_trk_chan[5] = {1,1,1,1,1};
bool g_is_play[5] = {true, true, true, true, true};
bool g_stop = true;
void write_track_1() {

  uint16_t pan = 80;  // pan right
  CTrack_1 trk_1(1);

  trk_1.write_track(track_1_notes, g_trk_chan[0], g_track_1_patch, g_vol_1,
                    pan);

  std::copy(trk_1.m_trk.begin(), trk_1.m_trk.end(),
            std::back_inserter(play_trk));
}
void write_track_2() {

  uint16_t pan = 80;  // pan right
  CTrack_1 trk_2(1);

  trk_2.write_track(track_2_notes, g_trk_chan[1], g_track_2_patch, g_vol_2,
                    pan);

  std::copy(trk_2.m_trk.begin(), trk_2.m_trk.end(),
            std::back_inserter(play_trk));
}
void write_track_3() {

  uint16_t pan = 80;  // pan right
  CTrack_1 trk_3(1);

  trk_3.write_track(track_3_notes, g_trk_chan[2], g_track_3_patch, g_vol_3,
                    pan);

  std::copy(trk_3.m_trk.begin(), trk_3.m_trk.end(),
            std::back_inserter(play_trk));
}
void write_track_4() {

  uint16_t pan = 80;  // pan right
  CTrack_1 trk_4(1);

  trk_4.write_track(track_4_notes, g_trk_chan[3], g_track_4_patch, g_vol_4,
                    pan);

  std::copy(trk_4.m_trk.begin(), trk_4.m_trk.end(),
            std::back_inserter(play_trk));
}
void write_track_5() {

  uint16_t pan = 80;  // pan right
  CTrack_1 trk_5(1);

  trk_5.write_track(track_5_notes, g_trk_chan[4], g_track_5_patch, g_vol_5,
                    pan);

  std::copy(trk_5.m_trk.begin(), trk_5.m_trk.end(),
            std::back_inserter(play_trk));
}
void chan_combo_box_init(QComboBox *combo_box) {
  QStringList chan_list;
  chan_list << "1"
            << "2"
            << "3"
            << "4"
            << "5"
            << "6"
            << "7"
            << "8"
            << "9"
            << "10"
            << "11"
            << "12"
            << "13"
            << "14"
            << "15"
            << "16";
  (*combo_box).addItems(chan_list);
}

void MainWindow::setup_trk_combox_box() {
  ui->comboBox_input_track->addItem("Track 1");
  ui->comboBox_input_track->addItem("Track 2");
  ui->comboBox_input_track->addItem("Track 3");
  ui->comboBox_input_track->addItem("Track 4");
  ui->comboBox_input_track->addItem("Track 5");
}

void MainWindow::init_all_chan_combo_box() {
  chan_combo_box_init(ui->comboBox_1_chan);
  chan_combo_box_init(ui->comboBox_2_chan);
  chan_combo_box_init(ui->comboBox_3_chan);
  chan_combo_box_init(ui->comboBox_4_chan);
  chan_combo_box_init(ui->comboBox_5_chan);
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  setup_instru_name_list();
  setup_comboBox_category();
  setup_trk_combox_box();
  init_all_chan_combo_box();

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

MainWindow::~MainWindow() { delete ui; }
void MainWindow::txTimerAction() {
  int chan = (*cur_pkt).get_status() % 16;
  qDebug() << chan;
  int trk_num; // 0 ~ 4
  for(int i = 0; i < 5; i++) {
      if(g_trk_chan[i] == chan) {
          trk_num = i;
          break;
      }
  }
  qDebug() << "track:" << trk_num;
  bool play = g_is_play[trk_num];
  qDebug() << play;
  if(play) {
    sendCMidiPacket(*cur_pkt);
  }
  (*cur_pkt) = (*next_pkt);
  next_pkt++;
  QTimer *timer = qobject_cast<QTimer *>(sender());

  if (cur_pkt == play_trk.end() - 1 || g_stop) {
    timer->stop();
    g_stop = true;
    return;
  }
  auto delay = ((*next_pkt).get_timestamp() - (*cur_pkt).get_timestamp()) * (60 / g_tempo);
  timer->start(delay);
  if (next_pkt != play_trk.end()) {
    cur_pkt++;
  }
}

void MainWindow::on_pushButton_play_clicked() {
  // CMidiPacket43 mp1{0,0x90,60,100};
  // CMidiPacket43 mp2{5000,0x80,60,0};
  // sendCMidiPacket(mp1);
  // sendCMidiPacket(mp2);
  play_trk.clear();
    write_track_1();
    write_track_2();
    write_track_3();
    write_track_4();
    write_track_5();
  if (play_trk.size() == 0) {
    QMessageBox msg;
    msg.setText("You have not add any notes yet.");
    msg.exec();
    return;
  }
  // sort the play_trk using CMidiPacket43::operator<
  std::sort(play_trk.begin(), play_trk.end());
  cur_pkt = play_trk.begin();
  next_pkt = play_trk.begin();
  /*for (CMidiPacket43 mp : play_trk) {
    std::cout << mp;
  }*/
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(txTimerAction()));
  g_stop = false;
  timer->start();
}

void MainWindow::on_pushButton_Enter_clicked() {
  cur_input_note_string.clear();
  if (!ui->lineEdit_0->text().toStdString().empty()) {
    cur_input_note_string.push_back(ui->lineEdit_0->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_1->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_1->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_2->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_2->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_3->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_3->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_4->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_4->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_5->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_5->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_6->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_6->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_7->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_7->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_8->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_8->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_9->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_9->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_10->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_10->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_11->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_11->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_12->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_12->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_13->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_13->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_14->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_14->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  if (!ui->lineEdit_15->text().toStdString().empty()) {

    cur_input_note_string.push_back(ui->lineEdit_15->text().toStdString());
  } else {
    cur_input_note_string.push_back("0");
  }
  switch (g_current_track) {
    case 1:
      track_1_notes.insert(track_1_notes.end(), cur_input_note_string.begin(),
                           cur_input_note_string.end());
      break;
    case 2:
      track_2_notes.insert(track_2_notes.end(), cur_input_note_string.begin(),
                           cur_input_note_string.end());
      break;
    case 3:
      track_3_notes.insert(track_3_notes.end(), cur_input_note_string.begin(),
                           cur_input_note_string.end());
      break;
    case 4:
      track_4_notes.insert(track_4_notes.end(), cur_input_note_string.begin(),
                           cur_input_note_string.end());
      break;
    case 5:
      track_5_notes.insert(track_5_notes.end(), cur_input_note_string.begin(),
                           cur_input_note_string.end());
      break;
  }
}

// interface stuffs, easy to understand
void MainWindow::on_horizontalSlider_tempo_valueChanged(int value) {
  ui->label_tempo->setText(QString::number(value));
  g_tempo = value;
}

void MainWindow::on_horizontalSlider_1_vol_valueChanged(int value) {
  ui->label_1_vol->setText(QString::number(value));
  g_vol_1 = value;
  CMidiPacket43 vol_ctrl(0, 0xB0 + g_trk_chan[0], 7, g_vol_1);
  //std::cout << "vol control: " << vol_ctrl;
  sendCMidiPacket(vol_ctrl);
}

void MainWindow::on_horizontalSlider_2_vol_valueChanged(int value) {
  ui->label_2_vol->setText(QString::number(value));
  g_vol_2 = value;
  CMidiPacket43 vol_ctrl(0, 0xB0 + g_trk_chan[1], 7, g_vol_2);
  sendCMidiPacket(vol_ctrl);
}

void MainWindow::on_horizontalSlider_3_vol_valueChanged(int value) {
  ui->label_3_vol->setText(QString::number(value));
  g_vol_3 = value;
  CMidiPacket43 vol_ctrl(0, 0xB0 + g_trk_chan[2], 7, g_vol_3);
  sendCMidiPacket(vol_ctrl);
}

void MainWindow::on_horizontalSlider_4_vol_valueChanged(int value) {
  ui->label_4_vol->setText(QString::number(value));
  g_vol_4 = value;
  CMidiPacket43 vol_ctrl(0, 0xB0 + g_trk_chan[3], 7, g_vol_4);
  sendCMidiPacket(vol_ctrl);
}

void MainWindow::on_horizontalSlider_5_vol_valueChanged(int value) {
  ui->label_5_vol->setText(QString::number(value));
  g_vol_5 = value;
  CMidiPacket43 vol_ctrl(0, 0xB0 + g_trk_chan[4], 7, g_vol_5);
  sendCMidiPacket(vol_ctrl);
}

void MainWindow::setup_comboBox_category() {
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
  g_none_list << "<None>";

  g_piano_list << "<None>"
               << "1 Acoustic Grand Piano"
               << "2 Bright Acoustic Piano"
               << "3 Electric Grand Piano"
               << "4 Honky-tonk Piano"
               << "5 Electric Piano 1"
               << "6 Electric Piano 2"
               << "7 Harpsichord"
               << "8 Clavinet";
  g_chroma_list << "<None>"
                << "9 Celesta"
                << "10 Glockenspiel"
                << "11 Music Box"
                << "12 Vibraphone"
                << "13 Marimba"
                << "14 Xylophone"
                << "15 Tubular Bells"
                << "16 Dulcimer";
  g_organ_list << "<None>"
               << "17 Drawbar Organ"
               << "18 Percussive Organ"
               << "19 Rock Organ"
               << "20 Church Organ"
               << "21 Reed Organ"
               << "22 Accordion"
               << "23 Harmonica"
               << "24 Tango Accordion";
  g_guitar_list << "<None>"
                << "25 Acoustic Guitar (nylon)"
                << "26 Acoustic Guitar (steel)"
                << "27 Electric Guitar (jazz)"
                << "28 Electric Guitar (clean)"
                << "29 Electric Guitar (muted)"
                << "30 Overdriven Guitar"
                << "31 Distortion Guitar"
                << "32 Guitar Harmonics";
  g_bass_list << "<None>"
              << "33 Acoustic Bass"
              << "34 Electric Bass (finger)"
              << "35 Electric Bass (pick)"
              << "36 Fretless Bass"
              << "37 Slap Bass 1"
              << "38 Slap Bass 2"
              << "39 Synth Bass 1"
              << "40 Synth Bass 2";
  g_strings_list << "<None>"
                 << "41 Violin"
                 << "42 Viola"
                 << "43 Cello"
                 << "44 Contrabass"
                 << "45 Tremolo Strings"
                 << "46 Pizzicato Strings"
                 << "47 Orchestral Harp"
                 << "48 Timpani";
  g_ensemble_list << "<None>"
                  << "49 String Ensemble 1"
                  << "50 String Ensemble 2"
                  << "51 Synth Strings 1"
                  << "52 Synth Strings 2"
                  << "53 Choir Aahs"
                  << "54 Voice Oohs"
                  << "55 Synth Choir"
                  << "56 Orchestra Hit";

  g_brass_list << "<None>"
               << "57 Trumpet"
               << "58 Trombone"
               << "59 Tuba"
               << "60 Muted Trumpet"
               << "61 French Horn"
               << "62 Brass Section"
               << "63 Synth Brass 1"
               << "64 Synth Brass 2";
  g_reed_list << "<None>"
              << "65 Soprano Sax"
              << "66 Alto Sax"
              << "67 Tenor Sax"
              << "68 Baritone Sax"
              << "69 Oboe"
              << "70 English Horn"
              << "71 Bassoon"
              << "72 Clarinet";
  g_pipe_list << "<None>"
              << "73 Piccolo"
              << "74 Flute"
              << "75 Recorder"
              << "76 Pan Flute"
              << "77 Blown bottle"
              << "78 Shakuhachi"
              << "79 Whistle"
              << "80 Ocarina";
  g_synth_lead_list << "<None>"
                    << "81 Lead 1 (square)"
                    << "82 Lead 2 (sawtooth)"
                    << "83 Lead 3 (calliope)"
                    << "84 Lead 4 chiff"
                    << "85 Lead 5 (charang)"
                    << "86 Lead 6 (voice)"
                    << "87 Lead 7 (fifths)"
                    << "88 Lead 8 (bass + lead)";
  g_synth_pad_list << "<None>"
                   << "89 Pad 1 (new age)"
                   << "90 Pad 2 (warm)"
                   << "91 Pad 3 (polysynth)"
                   << "92 Pad 4 (choir)"
                   << "93 Pad 5 (bowed)"
                   << "94 Pad 6 (metallic)"
                   << "95 Pad 7 (halo)"
                   << "96 Pad 8 (sweep)";
  g_synth_effect_list << "<None>"
                      << "97 FX 1 (rain)"
                      << "98 FX 2 (soundtrack)"
                      << "99 FX 3 (crystal)"
                      << "100 FX 4 (atmosphere)"
                      << "101 FX 5 (brightness)"
                      << "102 FX 6 (goblins)"
                      << "103 FX 7 (echoes)"
                      << "104 FX 8 (sci-fi)";
  g_ethnic_list << "<None>"
                << "105 Sitar"
                << "106 Banjo"
                << "107 Shamisen"
                << "108 Koto"
                << "109 Kalimba"
                << "110 Bagpipe"
                << "111 Fiddle"
                << "112 Shanai";
  g_percussive_list << "<None>"
                    << "113 Tinkle Bell"
                    << "114 Agogo"
                    << "115 Steel Drums"
                    << "116 Woodblock"
                    << "117 Taiko Drum"
                    << "118 Melodic Tom"
                    << "119 Synth Drum"
                    << "120 Reverse Cymbal";
  g_effects_list << "<None>"
                 << "121 Guitar Fret Noise"
                 << "122 Breath Noise"
                 << "123 Seashore"
                 << "124 Bird Tweet"
                 << "125 Telephone Ring"
                 << "126 Helicopter"
                 << "127 Applause"
                 << "128 Gunshot";
  g_all_lists.push_back(g_none_list);
  g_all_lists.push_back(g_piano_list);
  g_all_lists.push_back(g_chroma_list);
  g_all_lists.push_back(g_organ_list);
  g_all_lists.push_back(g_guitar_list);
  g_all_lists.push_back(g_bass_list);
  g_all_lists.push_back(g_strings_list);
  g_all_lists.push_back(g_ensemble_list);
  g_all_lists.push_back(g_brass_list);
  g_all_lists.push_back(g_reed_list);
  g_all_lists.push_back(g_pipe_list);
  g_all_lists.push_back(g_synth_lead_list);
  g_all_lists.push_back(g_synth_pad_list);
  g_all_lists.push_back(g_synth_effect_list);
  g_all_lists.push_back(g_ethnic_list);
  g_all_lists.push_back(g_percussive_list);
  g_all_lists.push_back(g_effects_list);
}

void MainWindow::on_comboBox_1_cat_currentIndexChanged(int index) {
  ui->comboBox_1_instru->clear();
  ui->comboBox_1_instru->addItems(g_all_lists.at(index));
}

void MainWindow::on_comboBox_2_cat_currentIndexChanged(int index) {
  ui->comboBox_2_instru->clear();
  ui->comboBox_2_instru->addItems(g_all_lists.at(index));
}

void MainWindow::on_comboBox_3_cat_currentIndexChanged(int index) {
  ui->comboBox_3_instru->clear();
  ui->comboBox_3_instru->addItems(g_all_lists.at(index));
}

void MainWindow::on_comboBox_4_cat_currentIndexChanged(int index) {
  ui->comboBox_4_instru->clear();
  ui->comboBox_4_instru->addItems(g_all_lists.at(index));
}

void MainWindow::on_comboBox_5_cat_currentIndexChanged(int index) {
  ui->comboBox_5_instru->clear();
  ui->comboBox_5_instru->addItems(g_all_lists.at(index));
}

void MainWindow::on_comboBox_1_instru_currentIndexChanged(const QString &arg1) {
  g_track_1_patch = arg1.split(QRegExp("\\s"))[0].toInt();
}

void MainWindow::on_comboBox_2_instru_currentIndexChanged(const QString &arg1) {
  g_track_2_patch = arg1.split(QRegExp("\\s"))[0].toInt();
}

void MainWindow::on_comboBox_3_instru_currentIndexChanged(const QString &arg1) {
  g_track_3_patch = arg1.split(QRegExp("\\s"))[0].toInt();
}

void MainWindow::on_comboBox_4_instru_currentIndexChanged(const QString &arg1) {
  g_track_4_patch = arg1.split(QRegExp("\\s"))[0].toInt();
}

void MainWindow::on_comboBox_5_instru_currentIndexChanged(const QString &arg1) {
  g_track_5_patch = arg1.split(QRegExp("\\s"))[0].toInt();
}

void MainWindow::on_comboBox_input_track_currentIndexChanged(int index) {
  g_current_track = index + 1;
}

void stop_one_track(int trk_num) {
  if (trk_num > 5 || trk_num < 1) {
    qDebug() << "track number to stop is unknown";
  }
  int chan = g_trk_chan[trk_num - 1];
  for (int i = 0; i < 127; i++) {
    CMidiPacket43 note_off_1(0, 0x90 + chan, i, 0);
    CMidiPacket43 note_off_2(0, 0x90 + chan, i, 0);
    CMidiPacket43 note_off_3(0, 0x90 + chan, i, 0);
    CMidiPacket43 note_off_4(0, 0x90 + chan, i, 0);
    CMidiPacket43 note_off_11(0, 0x80 + chan, i, 0);
    CMidiPacket43 note_off_21(0, 0x80 + chan, i, 0);
    CMidiPacket43 note_off_31(0, 0x80 + chan, i, 0);
    CMidiPacket43 note_off_41(0, 0x80 + chan, i, 0);
    sendCMidiPacket(note_off_1);
    sendCMidiPacket(note_off_2);
    sendCMidiPacket(note_off_3);
    sendCMidiPacket(note_off_4);
    sendCMidiPacket(note_off_11);
    sendCMidiPacket(note_off_21);
    sendCMidiPacket(note_off_31);
    sendCMidiPacket(note_off_41);
  }
}

void MainWindow::on_pushButton_stop_clicked() {
  stop_one_track(1);
  stop_one_track(2);
  stop_one_track(3);
  stop_one_track(4);
  stop_one_track(5);
  g_stop = true;
}

void MainWindow::on_comboBox_1_chan_currentIndexChanged(int index) {
  g_trk_chan[0] = index;
}

void MainWindow::on_comboBox_2_chan_currentIndexChanged(int index) {
  g_trk_chan[1] = index;
}

void MainWindow::on_comboBox_3_chan_currentIndexChanged(int index) {
  g_trk_chan[2] = index;
}

void MainWindow::on_comboBox_4_chan_currentIndexChanged(int index) {
  g_trk_chan[3] = index;
}

void MainWindow::on_comboBox_5_chan_currentIndexChanged(int index) {
  g_trk_chan[4] = index;
}

void MainWindow::on_clear_button_clicked() {
  if (ui->select_1->checkState()) {
    track_1_notes.clear();
  }
  if (ui->select_2->checkState()) {
    track_2_notes.clear();
  }
  if (ui->select_3->checkState()) {
    track_3_notes.clear();
  }
  if (ui->select_4->checkState()) {
    track_4_notes.clear();
  }
  if (ui->select_5->checkState()) {
    track_5_notes.clear();
  }
}

void MainWindow::on_pushButton_display_clicked()
{
    play_trk.clear();
    write_track_1();
    write_track_2();
    write_track_3();
    write_track_4();
    write_track_5();
    DisplayWindow display_window;
    display_window.set_channels(g_trk_chan);
    display_window.set_display_text(play_trk);
    display_window.setModal(true);
    display_window.exec();
}

void MainWindow::on_play_1_clicked(bool checked)
{
    g_is_play[0] = checked;
    if(!g_stop && !checked) {
        stop_one_track(1);
    }
}

void MainWindow::on_play_2_clicked(bool checked)
{
    g_is_play[1] = checked;
    if(!g_stop && !checked) {
        stop_one_track(2);
    }
}

void MainWindow::on_play_3_clicked(bool checked)
{
    g_is_play[2] = checked;
    if(!g_stop && !checked) {
        stop_one_track(3);
    }
}

void MainWindow::on_play_4_clicked(bool checked)
{
    g_is_play[3] = checked;
    if(!g_stop && !checked) {
        stop_one_track(4);
    }
}

void MainWindow::on_play_5_clicked(bool checked)
{
    g_is_play[4] = checked;
    if(!g_stop && !checked) {
        stop_one_track(5);
    }
}
