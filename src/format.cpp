#include "format.h"

#include <string>
#define SECONDS_IN_HOUR (60 * 60)
#define SECONDS_IN_MINUTE 60

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long elapsed_time_s [[maybe_unused]]) {
  int seconds, minutes, hours;
  seconds = elapsed_time_s % SECONDS_IN_MINUTE;
  minutes = ((elapsed_time_s - seconds) % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
  hours = elapsed_time_s / SECONDS_IN_HOUR;
  char buffer[15];
  sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);

  return std::string(buffer);
}