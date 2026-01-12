
#include "FS.h"

//////////////////////////////////
// ftpserver esp32
//////////////////////////////////
#include "ftpserveresp32.h"

void createDir(fs::FS &fs, const char * path)
{
 Serial.printf("Creating Dir: %s\r\n", path);
 if(fs.mkdir(path))
 {
  Serial.printf("Dir %s created\r\n", path);
 }
 else
 {
  Serial.printf("mkdir %s failed\r\n", path);
 }
}

void checkDir(fs::FS &fs, const char * dirname)
{
 File root = fs.open(dirname);
 if((!(root)) || (!root.isDirectory()))
 {
  Serial.printf("Dir %s not exist\r\n", dirname);
  createDir(fs, dirname);
 }
 else
 {
  Serial.printf("Dir %s ok\r\n", dirname);
 }
}


void checkFS()
{
// verifica la presenza delle directory e nel caso opposto le crea
 checkDir(SD, "/data");
 checkDir(SD, "/log");
}

File openLog(fs::FS &fs, String path)
{
//#ifdef LOGDEBUG
 File file;
 
 path = "/log/" + path;
 
 if (fs.exists(path.c_str()))
  file = fs.open(path.c_str(), FILE_APPEND);
 else
  file = fs.open(path.c_str(), FILE_WRITE);

 if(!file)
 {
  Serial.println("Failed to openLog file for appending");
 }
 return (file);
//#endif
}

void writeLog(String msg)
{
//#ifdef LOGDEBUG
 String timestamp = "";

 if (getLocalTime(&ti))
 {
  //  tm.tm_year
  //  tm.tm_mon
  //  tm.tm_mday
  //  tm_wday
  //  tm.tm_hour
  //  tm.tm_min
  //  tm.tm_sec
  //  tm.tm_isdst
  if ((ti.tm_hour) < 10)
   timestamp += "0" + String(ti.tm_hour) + ":";
  else
   timestamp += String(ti.tm_hour) + ":";
  if (ti.tm_min < 10)
   timestamp += "0" + String(ti.tm_min) + ":";
  else
   timestamp += String(ti.tm_min) + ":";
  if (ti.tm_sec < 10)
   timestamp += "0" + String(ti.tm_sec);
  else
   timestamp += String(ti.tm_sec);
 }
 else
  timestamp += "--:--:--";

 fsCurrentLog.println(timestamp + " " + msg);
// Serial.println(timestamp + " " + msg);
//#endif
}

void defLogFile()
{
//#ifdef LOGDEBUG
 if (getLocalTime(&ti))
 {
  //  tm.tm_year
  //  tm.tm_mon
  //  tm.tm_mday
  //  tm_wday
  //  tm.tm_hour
  //  tm.tm_min
  //  tm.tm_sec
  //  tm.tm_isdst
  pathCurrentLog = String(ti.tm_year + 1900);
  if ((ti.tm_mon+1) < 10)
   pathCurrentLog += "0" + String(ti.tm_mon+1);
  else
   pathCurrentLog += String(ti.tm_mon+1);
  if (ti.tm_mday < 10)
   pathCurrentLog += "0" + String(ti.tm_mday);
  else
   pathCurrentLog += String(ti.tm_mday);
  pathCurrentLog += ".log";
 }
 else
 {
  pathCurrentLog = "noconnection.log";
 }  
 fsCurrentLog = openLog(SD, pathCurrentLog);
//#endif
}
