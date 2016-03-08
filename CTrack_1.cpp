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

struct ChordTm {
  uint32_t note1;
  uint32_t note2;
  uint32_t note3;
  uint32_t note4;
  uint16_t dur;
};

const std::vector<ChordTm> one_meas_chord_vec = {
    {7, 10, 12, 16, 1667}, {7, 10, 12, 16, 2000}, {4, 7, 10, 12, 2667}};
const std::vector<uint32_t> dur_vec = {667, 1000, 1667, 2000, 2667};

// This is the implementation of the base class pure virtual function
void CTrack_1::write_track() {}

// This code used to be in CMidiTrack but was moved here
void CTrack_1::write_track(std::vector<std::string> notes, uint16_t chan,
                                   uint16_t patch, uint16_t vol, uint16_t pan) {
    m_trk.clear();
    patch_change(0, chan, patch);
    control_volume(0, chan, vol);
    control_pan(0, chan, pan);
    auto start_note = std::stoi(notes.at(0));
    std::cout<<notes.at(0)<<std::endl;
    for (int n = 0; n < m_repeats; ++n){
      for (int ix = 1; ix <= m_measures; ++ix) {
        write_one_measure(ix + (n * 12), notes, chan);
      }
}
}

void CTrack_1::write_one_measure(int meas_num, std::vector<std::string> notes,
                                         uint16_t chan) {
    CMidiPacket43 mp;
      uint32_t meas_tm = (meas_num - 1) * 4000;
      for (int i = 0; i < notes.size(); i++) {
          int note = std::stoi(notes.at(i));
        note_on(meas_tm + 50, chan, note, 100);


        note_off(meas_tm + 75 + 50, chan, note);

      }
}
