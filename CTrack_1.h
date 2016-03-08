// CTrack_1.h
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* CTrack_1.h
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef CTRACK_1_H_
#define CTRACK_1_H_

#ifndef CBLUESTRACK_H_
#include "CBluesTrack.h"
#endif

class CTrack_1 : public CBluesTrack {
 public:
  CTrack_1(int n) {
    m_repeats = n;
  };
  // The sublcass MUST declare and implement the pure virtual function
  void write_track();
  // This overloaded constructor will do the work
  void write_track(std::vector<std::string> notes, uint16_t chan, uint16_t patch,
                   uint16_t vol, uint16_t pan);

 private:
  void write_one_measure(int meas_num, std::vector<std::string> notes, uint16_t chan);
};
#endif  // CTRACK_1_H
