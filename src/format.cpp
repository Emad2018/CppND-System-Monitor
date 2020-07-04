#include <string>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string standardformat(long number) {
  if (number < 10)
    return "0" + std::to_string(number);
  else
    return std::to_string(number);
}
string Format::ElapsedTime(long seconds) 
{ 
  long hour_= seconds/3600;
  long min_=  ((seconds)-(hour_*3600))/60;
  long second_= seconds%60;
  
  return standardformat(hour_)+":"+standardformat(min_)+":"+standardformat(second_);
  
}