#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >>version>> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string key,line;
  float value,mem_used,mem_total=0; 
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) 
  {
    for(int i=0;i<5;i++)
    {  std::getline(filestream, line);
      std::replace(line.begin(), line.end(), ':', ' ');      
      std::istringstream linestream(line);
      (linestream >> key >> value);       
     // used Total used memory = MemTotal - MemFree
     // Non cache/buffer memory (green) = Total used memory - (Buffers + Cached memory)
        if (key == "MemTotal") 
        {
          mem_used =value;
          mem_total=value;
        }
        else if(key == "MemFree") 
        {
          mem_used -=value;
        }
        else if(key == "Buffers")
        {
          mem_used -=value;
        }
        else if(key == "Cached")
        {
          mem_used -=value; 
        }
    }
  }
  return mem_used/mem_total;

}
 
// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line;
  long value=0; 
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) 
  {
      std::getline(filestream, line);    
      std::istringstream linestream(line);
      (linestream >> value);     
  }
  return value;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{
  string line,cpu;
  long value,total_sum=0;
  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) 
  {
      std::getline(filestream, line);    
      std::istringstream linestream(line);
      linestream >>cpu;
      while(linestream>>value)
      {
        total_sum +=value;       
      }
    
  }
  return total_sum; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  string line,cpu,value;
  long total_sum=0;
  int counter=0;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  while (filestream.is_open()) 
  {    
      std::getline(filestream, line);    
      std::istringstream linestream(line);         
      while(linestream>>value)
      { 
        counter++;    
        if(counter==14)
        {                
          total_sum +=std::stol(value);  //13 utime 
          linestream>>value;
          total_sum +=std::stol(value);  //14 stime
          linestream>>value;
          total_sum +=std::stol(value);  //15  cutime  
          linestream>>value;
          total_sum +=std::stol(value);  //16  cstime
          return total_sum;
        }
      }
    
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  string line,cpu;
  long value,total_sum=0;
  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) 
  {
      std::getline(filestream, line);    
      std::istringstream linestream(line);
      linestream >>cpu;
      for(int i=0;i<6;i++)
      {
        linestream>>value;
        if((i !=3) && (i!=4))          // exclude idle: idle time,iowait: waiting for I/O to complete
        {
        	
       		 total_sum +=value;
        }
        
      }
    
  }
  return total_sum;  
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  string line,cpu;
  long result,value=0;  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) 
  {
      std::getline(filestream, line);    
      std::istringstream linestream(line);
      linestream >>cpu;
      for(int i=0;i<4;i++)
      {
          linestream>>value; 
      }
     result = value;
     linestream>>value;  // take idle: idle time [3],iowait: waiting for I/O to complete[4]
     result += value;
  }
  return result;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
 
  vector<string> CpuUtilization_string={std::to_string((float)LinuxParser::ActiveJiffies()/LinuxParser::Jiffies())}; 
  return CpuUtilization_string; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      while (linestream >> key >> value) {
        
        if (key == "processes") {         
          return value;
        }
      }
    }
  }
return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      
      while (linestream >> key >> value) 
      {
        
        if (key == "procs_running") 
        {      
          return value;
        }
      }
    }
  }
  
return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  string line,command,value;
  
  
  std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) 
  {    
      std::getline(filestream, line);         
  } 
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  string key,line;
  string value;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
       std::istringstream linestream(line);
       while(linestream >> key >> value)  
       {
      
          if (key == "VmSize:") 
          { 

            return value;
          }
       }
     }
  }
   return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
  string key,line;
  string value;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
       std::istringstream linestream(line);
       while(linestream >> key >> value)  
       {
      
          if (key == "Uid:") 
          { 
            
            return value;
          }
       }
     }
  }
   return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
  string key,line,value;
  string uid=Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
       while(linestream >> key >> value)  
       {
      
          if (value == uid) 
          { 
            return key;
          }
       }
     }
  }
   return key;

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  string line,value;
  int counter=0;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  while (filestream.is_open()) 
  {    
      std::getline(filestream, line);    
      std::istringstream linestream(line);         
      while(linestream>>value)
      { 
        counter++;    
        if(counter==22)
        {                          
          return  std::stol(value)/sysconf(_SC_CLK_TCK);
        }
      }
    
  }
  return 0;
}