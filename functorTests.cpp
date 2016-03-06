// functor.cpp
// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* functor.cpp
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/
#ifndef FUNCTOR_H_
#include "functorTests.h"
#endif

#ifndef BLUESUTILS_H_
#include "bluesUtils.h"
#endif

void AddSwing::operator()(CMidiPacket43& mp) {
  int amount = 110;  // 0 - 200
  uint32_t tm = mp.get_timestamp();
  if ((tm - 500) % 1000 == 0) mp.set_timestamp(tm + amount);
}

void AccentBeatsOneThree::operator()(CMidiPacket43& mp) {
  short vel = 0;
  if (mp.is_note_off())
    return;
  else if (mp.is_status_9n()) {
    vel = mp.get_data2();
    if (mp.get_timestamp() % 4000 == 0)
      vel = 127;
    else if (mp.get_timestamp() % 4000 == 1000)
      vel = 70;
    else if (mp.get_timestamp() % 4000 == 2000)
      vel = 120;
    else if (mp.get_timestamp() % 4000 == 3000)
      vel = 70;
    mp.set_data2(vel);
  }
}
