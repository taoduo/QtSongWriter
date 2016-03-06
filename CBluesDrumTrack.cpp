// CBluesDrumTrack.cpp
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* CBluesDrumTrack.cpp
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef CBLUESDRUMTRACK_H_
#include "CBluesDrumTrack.h"
#endif

#ifndef BLUESUTILS_H_
#include "bluesUtils.h"
#endif

// These strings are like rows of buttons on a drum machine matrix
// Each string represents one measure
// there are 8 bits in the string covering 4000 ms
// so each bit advances time by 500 ms
// if the bit is 1 a drom sound is played
// if the bit is 0 time advances but nothing is played
const std::string kRIDE_PATTERN = "11111111";
const std::string kBD_PATTERN = "00100010";
const std::string kHHC_PATTERN = "00100010";

const std::vector<uint32_t> tm_vec = {0,    250,  500, 750,
                                      1000, 1250, 1500, 1750,
                                      2000, 2250, 2500, 2750,
                                      3000, 3250, 3500, 3750};

// This is the implementation of the base class pure virtual function
void CBluesDrumTrack::write_track() {}

void CBluesDrumTrack::write_track(uint16_t vol,int note, std::string str) {
  m_trk.clear();
  uint16_t chan = 9;
  control_volume(0, chan, vol);

  for (int n = 0; n < m_repeats; ++n)
    for (int ix = 1; ix <= m_measures; ++ix) {
      write_one_measure(ix + (n * 12), note, str);

    }
}

void CBluesDrumTrack::write_one_measure(int meas_num, uint16_t note,
                                        std::string str) {
  CMidiPacket43 mp;
  uint32_t meas_tm = (meas_num - 1) * 4000;
  uint16_t chan = 9;
  for (int ix = 0; ix < static_cast<int>(str.size()); ++ix) {
    if (str.at(ix) == '1') {
      note_on(tm_vec.at(ix) + meas_tm, chan, note, 100);
      note_off(tm_vec.at(ix) + meas_tm + 200, chan, note);
    }
  }
}
