// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* bluesUtils.cpp
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef BLUESUTILS_H_
#include "bluesUtils.h"
#endif

#include <random>

int rand_range(int lo, int hi) {
  std::random_device rd;
  std::default_random_engine dre{rd()};
  std::uniform_int_distribution<int> dist{lo, hi};
  return dist(dre);
}

int weighted_dist() {
  // const int k0 = 4;  // 0
  // const int k1 = 1;  // 3
  // const int k2 = 1;  // 4
  // const int k3 = 2;  // 7
  // const int k4 = 1;  // 9
  // const int k5 = 2;  // 10

  // const int k0 = 1;
  // const int k1 = 0;
  // const int k2 = 0;
  // const int k3 = 0;
  // const int k4 = 0;
  // const int k5 = 0;

  const int k0 = 2;
  const int k1 = 1;
  const int k2 = 0;
  const int k3 = 0;
  const int k4 = 0;
  const int k5 = 0;

  std::random_device rd;
  std::default_random_engine engine(rd());
  std::discrete_distribution<int> distributionA{k0, k1, k2, k3, k4, k5};

  int n = distributionA(engine);
  return kBLUES_SCALE.at(n);
}

uint16_t get_note_offset(int meas_num, uint16_t start_note) {
  uint16_t offset;
  switch (meas_num % 12) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 11:
      offset = start_note;
      break;
    case 5:
    case 6:
    case 10:
      offset = start_note + 5;
      break;
    case 9:
      offset = start_note + 7;
      break;
    case 0:
      offset = start_note - 5;
      break;
    default:
      offset = start_note;
      break;
  }
  return offset;
}
