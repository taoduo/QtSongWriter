// bluesUtils.h
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* bluesUtils.h
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef BLUESUTILS_H_
#define BLUESUTILS_H_

#include <iostream>
#include <vector>

const std::vector<int16_t> kBLUES_SCALE = {0, 3, 4, 7, 9, 10};

extern int rand_range(int lo, int hi);
extern int weighted_dist();
extern uint16_t get_note_offset(int meas_num, uint16_t start_note);

#endif  // BLUESUTILS_H_
