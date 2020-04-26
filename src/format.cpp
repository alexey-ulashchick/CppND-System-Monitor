#include "format.h"
#include <string>

using std::string;

string Format::ElapsedTime(const long long& totalSeconds) {
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds / 60) % 60;
  int seconds = totalSeconds % 60;

  string s_hours(std::to_string(hours));
  string s_minutes((minutes < 10 ? "0" : "") + std::to_string(minutes));
  string s_seconds((seconds < 10 ? "0" : "") + std::to_string(seconds));

  return s_hours + ":" + s_minutes + ":" + s_seconds;
}