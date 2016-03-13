// CTrack_1.cpp
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* CTrack_1.cpp
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef CTRACK_1_H_
#include "CTrack_1.h"
#endif

#ifndef BLUESUTILS_H_
#include "bluesUtils.h"
#endif

// This is the implementation of the base class pure virtual function
void CTrack_1::write_track() {}

// This code used to be in CMidiTrack but was moved here
void CTrack_1::write_track(std::vector<std::string> notes, uint16_t chan,
                           uint16_t patch, uint16_t vol, uint16_t pan) {
  m_trk.clear();
  patch_change(0, chan, patch);
  control_volume(0, chan, vol);
  control_pan(0, chan, pan);

  write_one_measure(1, notes, chan);
}

void CTrack_1::write_one_measure(int meas_num, std::vector<std::string> notes,
                                 uint16_t chan) {
  CMidiPacket43 mp;
  int playing_note;
  uint32_t meas_tm = (meas_num - 1) * 1000;
  for (auto i = 0; i < (int)notes.size(); i++) {
    if (notes.at(i).compare("-") != 0 || i == (int)notes.size() - 1) {
      if (i != 0 && (notes.at(i).compare("-") != 0)) {
        note_off(meas_tm + i * 62.5 - 12.5, chan, playing_note);
      }
      if (i != (int)notes.size() - 1) {
        int note = std::stoi(notes.at(i));
        note_on(meas_tm + i * 62.5, chan, note, 100);
        playing_note = note;
      } else {
        note_off(meas_tm + i * 62.5 - 12.5, chan, playing_note);
      }
    }
  }
}
