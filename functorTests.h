// functor.h
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* functor.h
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef FUNCTORTESTS_H_
#define FUNCTORTESTS_H_

#ifndef CMIDIPACKET32_H_
#include "CMidiPacket43.h"
#endif

class AddSwing {
 public:
  void operator()(CMidiPacket43& mp);
};

class AccentBeatsOneThree {
 public:
  void operator()(CMidiPacket43& mp);
};

#endif  // FUNCTORTESTS_H_
