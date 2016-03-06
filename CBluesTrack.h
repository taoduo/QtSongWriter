// CBluesTrack.h
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* CBluesTrack.h
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef CBLUESTRACK_H_
#define CBLUESTRACK_H_

#ifndef CMIDITRACK_H_
#include "CMidiTrack.h"
#endif

class CBluesTrack : public CMidiTrack {
 public:
  int m_measures;
  int m_repeats;
  CBluesTrack() : m_measures{12}, m_repeats{1} {}
  // The sublcass MUST declare and implement the pure virtual function
  void write_track(){};
};
#endif  // CBLUESTRACK_H_
