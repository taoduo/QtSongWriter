// Copyright CS 312 free open source
/***************************************************************
*
* CS 312
* 7.2 Homework 22
* CMidiPacket43.cpp
* TranKiet    trank
* TaoDuo    taod
* 2/17/2016
*
***************************************************************/

#ifndef CMIDIPACKET43_H_
#include "CMidiPacket43.h"
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

CMidiPacket43::CMidiPacket43()
    : timestamp_{0}, length_{3}, status_{0x80}, data1_{0}, data2_{0} {
  invariant_check();
}

CMidiPacket43::CMidiPacket43(uint32_t ts, uint16_t st, uint16_t d1)
    : timestamp_{ts}, length_{2}, status_{st}, data1_{d1} {
  invariant_check();
}

CMidiPacket43::CMidiPacket43(uint32_t ts, uint16_t st, uint16_t d1, uint16_t d2)
    : timestamp_{ts}, length_{3}, status_{st}, data1_{d1}, data2_{d2} {
  invariant_check();
}

CMidiPacket43::CMidiPacket43(const std::string& str) {
  std::istringstream iss(str);
  iss >> timestamp_;
  std::string st;
  iss >> st;
  status_ = std::stoi(st, 0, 16);
  iss >> data1_;
  switch (status_ & 0xF0) {
    case 0xC0:
    case 0xD0:
      length_ = 2;
      break;
    case 0x80:
    case 0x90:
    case 0xA0:
    case 0xB0:
    case 0xE0:
      length_ = 3;
      iss >> data2_;
      break;
    case 0xF0:  // we're not going to deal with Fn status but we need to account
                // for it.
      length_ = 0;
      break;
    default:
      // harmless message
      timestamp_ = 0;
      length_ = 3;
      status_ = 0x80;
      data1_ = 0;
      data2_ = 0;
      break;
  }
  invariant_check();
}

std::string CMidiPacket43::to_string() const {
  invariant_check();
  std::stringstream ss;
  std::string res;
  std::string tab = "\t";
  ss << timestamp_ << tab;
  ss << std::hex << status_ << tab;
  ss << std::dec << data1_;
  if (length_ == 3) ss << tab << data2_;
  return ss.str();
}

// void CMidiPacket43::print() const { std::cout << to_string() << std::endl; }

// getters
uint32_t CMidiPacket43::get_timestamp() const {
  invariant_check();
  return timestamp_;
}
uint16_t CMidiPacket43::get_length() const {
  invariant_check();
  return length_;
}
uint16_t CMidiPacket43::get_status() const {
  invariant_check();
  return status_;
}
uint16_t CMidiPacket43::get_data1() const {
  invariant_check();
  return data1_;
}
uint16_t CMidiPacket43::get_data2() const {
  invariant_check();
  return data2_;
}

// setters
void CMidiPacket43::set_timestamp(const uint32_t& ts) {
  timestamp_ = ts;
  invariant_check();
}
void CMidiPacket43::set_status_length(const uint16_t& st) {
  status_ = st;
  uint16_t test = st & 0xF0;
  switch (test) {
    case 0x80:
    case 0x90:
    case 0xA0:
    case 0xB0:
    case 0xE0:
      length_ = 3;
      break;
    case 0xC0:
    case 0xD0:
      length_ = 2;
      break;
    case 0xF0:
      length_ = 0;
      break;
    default:
      //      std::cout << "BAD STATUS LENGTH" << std::endl;
      break;
  }
  invariant_check();
}

void CMidiPacket43::set_data1(const uint16_t& d1) {
  data1_ = d1;
  invariant_check();
}
void CMidiPacket43::set_data2(const uint16_t& d2) {
  data2_ = d2;
  invariant_check();
}

// Utility functions
bool CMidiPacket43::is_note_off() const {
  if (is_status_8n())
    return true;
  else if (is_status_9n() && data2_ == 0)
    return true;
  else
    return false;
}

// return true if status is between 0x80-0x8F
bool CMidiPacket43::is_status_8n() const {
  if ((status_ & 0xF0) == 0x80) return true;
  return false;
}
bool CMidiPacket43::is_status_9n() const {
  if ((status_ & 0xF0) == 0x90) return true;
  return false;
}
bool CMidiPacket43::is_status_An() const {
  if ((status_ & 0xF0) == 0xA0) return true;
  return false;
}
bool CMidiPacket43::is_status_Bn() const {
  if ((status_ & 0xF0) == 0xB0) return true;
  return false;
}
bool CMidiPacket43::is_status_Cn() const {
  if ((status_ & 0xF0) == 0xC0) return true;
  return false;
}
bool CMidiPacket43::is_status_Dn() const {
  if ((status_ & 0xF0) == 0xD0) return true;
  return false;
}
bool CMidiPacket43::is_status_En() const {
  if ((status_ & 0xF0) == 0xE0) return true;
  return false;
}
bool CMidiPacket43::is_status_Fn() const {
  if ((status_ & 0xF0) == 0xF0) return true;
  return false;
}

////////// NEW 32 ///////////
std::istream& operator>>(std::istream& is, CMidiPacket43& mp) {
  std::string sts;
  std::string sst;
  std::string sd1;
  std::string sd2;
  is >> sts;
  is >> sst;
  is >> sd1;
  uint32_t ts = std::stoi(sts);
  uint16_t st = std::stoi(sst, 0, 16);
  uint16_t d1 = std::stoi(sd1);
  uint16_t d2 = 0;

  switch (st & 0xF0) {
    case 0x80:
    case 0x90:
    case 0xA0:
    case 0xB0:
    case 0xE0:
      is >> sd2;
      d2 = std::stoi(sd2);
      mp = {ts, st, d1, d2};
      break;
    case 0xC0:
    case 0xD0:
      mp = {ts, st, d1};
      break;
    case 0xF0:
      mp = {0, 0x80, 0, 0};
      break;
    default:
      std::cout << "# BAD >> CMidiPacket43" << std::endl;
      break;
  }
  mp.invariant_check();
  return is;
}

std::ostream& operator<<(std::ostream& os, const CMidiPacket43& mp) {
  mp.invariant_check();
  const char tab = '\t';
  uint16_t len = mp.length_;
  os << mp.timestamp_ << tab << std::hex << mp.status_ << tab << std::dec
     << mp.data1_;
  if (len == 3)
    os << tab << mp.data2_ << std::endl;
  else if (len == 2)
    os << std::endl;
  else
    os << "# BAD << CMidiPacket43" << std::endl;
  return os;
}

bool operator==(const CMidiPacket43& a, const CMidiPacket43& b) {
  a.invariant_check();
  b.invariant_check();
  bool lenOK = false;
  bool tStampOK = false;
  bool statusOK = false;
  bool data1OK = false;
  bool data2OK = false;

  if (a.length_ == b.length_) lenOK = true;
  if (a.timestamp_ == b.timestamp_) tStampOK = true;
  if (a.status_ == b.status_) statusOK = true;
  if (a.data1_ == b.data1_) data1OK = true;
  if (a.data2_ == b.data2_) data2OK = true;

  if (a.length_ == 2)
    return (lenOK && tStampOK && statusOK && data1OK);
  else
    return (lenOK && tStampOK && statusOK && data1OK && data2OK);
}

bool operator<(const CMidiPacket43& a, const CMidiPacket43& b) {
  a.invariant_check();
  b.invariant_check();
  // case 1 a == b
  if (a == b) return false;

  // case 2 timestamps less
  else if (a.timestamp_ < b.timestamp_)
    return true;

  // ORDER IMPORTANT from here down
  // case 3 timestamps equal and a.status_ == 0x8n
  else if (is_timestamp_equal(a, b) && a.is_status_8n())
    return (a.status_ < b.status_);

  // case 4 timestamps equal
  // a.status == 0x9n and a.data2_ == 0
  else if (is_timestamp_equal(a, b) && a.is_status_9n() && a.data2_ == 0)
    return (a.data2_ < b.data2_);

  // case 5 timestamps equal
  // status equal
  // data1 equal
  else if (is_timestamp_equal(a, b) && is_status_equal(a, b) &&
           (a.data1_ == b.data1_))
    return (a.data2_ < b.data2_);

  // case 6 timestamps equal
  // status equal
  else if (is_timestamp_equal(a, b) && is_status_equal(a, b))
    return (a.data1_ < b.data1_);

  // case 7 timestamps equal and status categories equal
  // a.status < b.status
  else if (is_timestamp_equal(a, b) && is_status_category_equal(a, b))
    return (a.status_ < b.status_);

  // case 8 timestamps equal
  // previous checks caught 0x8n and 0x9n
  // not a.status < b.status
  else if (is_timestamp_equal(a, b))
    return !(a.status_ < b.status_);

  else
    return false;
}

bool is_timestamp_equal(const CMidiPacket43& a, const CMidiPacket43& b) {
  if (a.timestamp_ == b.timestamp_) return true;
  return false;
}

bool is_status_equal(const CMidiPacket43& a, const CMidiPacket43& b) {
  if (a.status_ == b.status_) return true;
  return false;
}

bool is_status_category_equal(const CMidiPacket43& a, const CMidiPacket43& b) {
  if ((a.status_ & 0xF0) == (b.status_ & 0xF0)) return true;
  return false;
}
////////// END NEW ///////////

////////// NEW 32 Invariant checks ///////////
void CMidiPacket43::ERROR(const std::string& msg) const {
  if (1) {
    std::cout << "ERROR: " << msg << std::endl;
  } else {
    throw std::logic_error(msg);
  }
}

void CMidiPacket43::invariant_check() const {
  // range check
  if ((status_ < 0x80) || (status_ >= 0xFF)) ERROR(kERR_STATUS_OUT_OF_RANGE);
  // data 1 and 2 range check
  else if (data1_ > 0x7F)
    ERROR(kERR_DATA1_OUT_OF_RANGE);
  else if ((data2_ > 0x7F) && length_ == 3)
    ERROR(kERR_DATA2_OUT_OF_RANGE);

  //  status length check
  else if (is_status_8n() && length_ != 3)
    ERROR(kERR_BAD_8n_MIDIPACKET_LENGTH);
  else if (is_status_9n() && length_ != 3)
    ERROR(kERR_BAD_9n_MIDIPACKET_LENGTH);
  else if (is_status_An() && length_ != 3)
    ERROR(kERR_BAD_An_MIDIPACKET_LENGTH);
  else if (is_status_Bn() && length_ != 3)
    ERROR(kERR_BAD_Bn_MIDIPACKET_LENGTH);
  else if (is_status_Cn() && length_ != 2)
    ERROR(kERR_BAD_Cn_MIDIPACKET_LENGTH);
  else if (is_status_Dn() && length_ != 2)
    ERROR(kERR_BAD_Dn_MIDIPACKET_LENGTH);
  else if (is_status_En() && length_ != 3)
    ERROR(kERR_BAD_En_MIDIPACKET_LENGTH);

  //  unsupported check
  else if (is_status_Fn())
    ERROR(kERR_Fn_MESSAGES_NOT_SUPPORTED);
}
////////// END NEW 32 Invariant checks ///////////
