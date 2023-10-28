#include <iostream>

#include "format.h"
#include "linux_parser.h"
#include "ncurses_display.h"
#include "processor.h"
#include "system.h"

using namespace std;
int main() {
  System system;
  NCursesDisplay::Display(system);

  return 0;
}

/*

cout<<"MemoryUtilization= " <<LinuxParser::MemoryUtilization() <<"\n";
  cout<<"UpTime= " << LinuxParser::UpTime() <<"\n";
  cout<<"TotalProcesses= " << LinuxParser::TotalProcesses() <<"\n";
  cout<<"RunningProcesses= " << LinuxParser::RunningProcesses() <<"\n";
  cout<<"OperatingSystem= " << LinuxParser::OperatingSystem() <<"\n";
  cout<<"CpuUtilization= " << LinuxParser::CpuUtilization()[0] <<"\n";
  cout<<"Jiffies= " << LinuxParser::Jiffies() <<"\n";
  cout<<"ActiveJiffies= " << LinuxParser::ActiveJiffies() <<"\n";
  cout<<"IdleJiffies= " << LinuxParser::IdleJiffies() <<"\n";
  cout<<"ActiveJiffies pid= " << LinuxParser::ActiveJiffies(1) <<"\n";
  cout<<"Command= " << LinuxParser::Command(1) <<"\n";
  cout<<"Ram= " << LinuxParser::Ram(1) <<"\n";
  cout<<"Uid= " << LinuxParser::Uid(1) <<"\n";
  cout<<"User= " << LinuxParser::User(1) <<"\n";
  cout<<"UpTime= " << LinuxParser::UpTime(1) <<"\n";
  */