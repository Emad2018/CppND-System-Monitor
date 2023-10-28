#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string standardformat(long number) {
  string result;
  if (number < 10) {
    result = "0" + std::to_string(number);
  } else {
    result = std::to_string(number);
  }
  return result;
}
string Format::ElapsedTime(long seconds) {
  int fhours, fmins, fseconds;
  string format;
  fhours = seconds / 3600;
  fmins = (seconds % 3600) / 60;
  fseconds = seconds % 60;
  format = standardformat(fhours) + ":" + standardformat(fmins) + ":" +
           standardformat(fseconds);

  return format;
}