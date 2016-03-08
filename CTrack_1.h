// CBluesDrumTrack.h
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* CBluesDrumTrack.h
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef CBLUESDRUMTRACK_H_
#define CBLUESDRUMTRACK_H_

#ifndef CBLUESTRACK_H_
#include "CBluesTrack.h"
#endif

class CBluesDrumTrack : public CBluesTrack {
 public:
  CBluesDrumTrack(int n) { m_repeats = n; };
  // The sublcass MUST declare and implement the pure virtual function
  void write_track();
  // This overloaded constructor will do the work
  void write_track(uint16_t vol,int note, std::string str);

 private:
  void write_one_measure(int meas_num, uint16_t note, std::string str);
};
#endif  // CBLUESDRUMTRACK_H
