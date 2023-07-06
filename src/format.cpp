#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int HH, MM, SS;
  string time;
  HH = seconds / 3600;
  MM = (seconds - HH * 3600) / 60;
  SS = seconds - HH * 3600 - MM * 60;
  time = std::to_string(HH) + ":" + std::to_string(MM) + ":" + std::to_string(SS) + ((SS < 10)? " " : "");
  return time;
}