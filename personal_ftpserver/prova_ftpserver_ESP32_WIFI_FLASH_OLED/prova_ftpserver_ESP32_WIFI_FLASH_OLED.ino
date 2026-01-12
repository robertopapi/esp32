// configura display, SD e WIFI e memorizza i dati nella flash


// wifi e OTA
#include <WiFi.h> 
#include <ArduinoOTA.h>

//////////////////////////////////
//esp32 library to save preferences in flash
//////////////////////////////////
#include <Preferences.h>

//////////////////////////////////
// ftpserver
//////////////////////////////////
#include <MultiFTPServer.h>

//////////////////////////////////
// gestione SPI per ethernet e SD
//////////////////////////////////
#include <SPI.h>

//////////////////////////////////
// include delle librerie per l'accesso alla SD
//////////////////////////////////
#include <SD.h>

//////////////////////////////////
// webserver
//////////////////////////////////
#include <WebServer.h>
#include <ArduinoJson.h>

//////////////////////////////////
// ftpserver esp32
//////////////////////////////////
#include "ftpserveresp32.h"

WebServer server(80);

//istanza per memorizzare i dati nella flash
Preferences pref;

///////////////////////////////////////////////////////////////////////
// inizializzazione I2C e display
///////////////////////////////////////////////////////////////////////

#define ESP32_I2C_SDA 21
#define ESP32_I2C_SCL 22

#include <U8g2lib.h>

// inizializzazione I2C
#include <Wire.h>

//U8G2_SSD1306_128X64_NONAME_F_HW_I2C(rotation, [reset [, clk|SCL, data|SDA]])
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, ESP32_I2C_SCL, ESP32_I2C_SDA);

//////////////////////////////////////////////////////////////////////////
// Inizializzazione SPI2 e SD
//////////////////////////////////////////////////////////////////////////
#define SD_CS_PIN        15

// These pins will be use for SPI2
#define SD_CLK_PIN       14
#define SD_MOSI_PIN      13
#define SD_MISO_PIN      12

SPIClass SPI2(HSPI); // definisco la classe che utilizzerò per eseguire il begin del servizio

// tipo di SD card che è CARD_NONE (assente) di default
uint8_t cardType = CARD_NONE;

//////////////////////////////////
// ftpserver
//////////////////////////////////
FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial

void _callback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace)
{
  /* FTP_CONNECT,
   * FTP_DISCONNECT,
   * FTP_FREE_SPACE_CHANGE
   * freeSpace : totalSpace = x : 360
   */

 String m="";
 char cfreeSpace[100];
 char ctotalSpace[100];
 
 sprintf(cfreeSpace, "%u", freeSpace);
 sprintf(ctotalSpace, "%u", totalSpace);

// dispdebug("_callback");

 switch (ftpOperation)
 {
  case FTP_CONNECT:
   m = "FTP_CONNECT freeSpace " + String(cfreeSpace) + " totalSpace " + String(ctotalSpace);
   Serial.printf("FTP_CONNECT freeSpace %u, totalSpace %u\r\n", freeSpace, totalSpace);
   break;
  case FTP_DISCONNECT:
   m = "FTP_DISCONNECT freeSpace " + String(cfreeSpace) + " totalSpace " + String(ctotalSpace);
   Serial.printf("FTP_DISCONNECT freeSpace %u, totalSpace %u\r\n", freeSpace, totalSpace);
   break;
  case FTP_FREE_SPACE_CHANGE:
   m = "FTP_FREE_SPACE_CHANGE freeSpace " + String(cfreeSpace) + " totalSpace " + String(ctotalSpace);
   Serial.printf("FTP_FREE_SPACE_CHANGE freeSpace %u, totalSpace %u\r\n", freeSpace, totalSpace);
   break;
  default:
   m = "FTP_OPERATION_UNKNOWN (" + String(ftpOperation) + ") ";
   Serial.printf("FTP_OPERATION_UNKNOWN (%s)\r\n", String(ftpOperation));
   break;
 }
 if (m != "")
 {
  defLogFile();
  writeLog(m);
  fsCurrentLog.close();
 }
}

void _transferCallback(FtpTransferOperation ftpOperation, const char* name, unsigned int transferredSize)
{
 String m="";
 char ctransferredSize[100];
 
 sprintf(ctransferredSize, "%u", transferredSize);
 
 switch (ftpOperation)
 {
  case FTP_UPLOAD_START:
   m = "FTP_UPLOAD_START " + String(name);
   ledFTPoperation();
   break;
  case FTP_UPLOAD: // necessario per intercettare e gestire (senza output) il codice corrispondente
   ledFTPoperation();
   break;
  case FTP_DOWNLOAD_START:
   m = "FTP_DOWNLOAD_START " + String(name);
   ledFTPoperation();
   break;
  case FTP_DOWNLOAD: // necessario per intercettare e gestire (senza output) il codice
   ledFTPoperation();
   break;
  case FTP_TRANSFER_STOP: // lo aggiungo alle stringhe di start
   m = "FTP_TRANSFER_STOP " + String(name) + " total " + String(ctransferredSize);
   ledFTPoperationOFF();
   break;
  case FTP_TRANSFER_ERROR:
   m = "FTP_TRANSFER_ERROR " + String(name) + " total " + String(ctransferredSize);
   ledFTPoperationOFF();
   break;
  default:
   m = "FTP_OPERATION_UNKNOWN (" + String(ftpOperation) + ") " + String(name);
   ledFTPoperationOFF();
   break;
 }
  /* FTP_UPLOAD_START = 0,
   * FTP_UPLOAD = 1,
   *
   * FTP_DOWNLOAD_START = 2,
   * FTP_DOWNLOAD = 3,
   *
   * FTP_TRANSFER_STOP = 4,
   * FTP_DOWNLOAD_STOP = 4,
   * FTP_UPLOAD_STOP = 4,
   *
   * FTP_TRANSFER_ERROR = 5,
   * FTP_DOWNLOAD_ERROR = 5,
   * FTP_UPLOAD_ERROR = 5
   */
 if (m != "")
 {
  defLogFile();
  writeLog(m);
  fsCurrentLog.close();
  Serial.println(m);
 }
}

//============================================================================
//                                    Setup
//============================================================================
void setup()
{
 Serial.begin(115200);
 delay(5000);

// documentato in C:\Users\..\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.9\libraries\Wire\src
 Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL); // bool begin(int sda, int scl, uint32_t frequency=0);

 init_display();
 
 clear_display(); // ripulisco i buffer e il display, imposto il colore e il font
 
//////////////////////////////////////////////////////
// startup SD
//////////////////////////////////////////////////////

 pinMode(SD_CS_PIN, OUTPUT); // non è chiaro il perché

 if(!SD.begin(SD_CS_PIN, SPI2))
 {
  SD_type = "mount della SD fallito!";
  SD_size = "";
 }
 else
 { 
  cardType = SD.cardType();
 
  if(cardType == CARD_NONE)
  {
   SD_type = "Nessuna SD card presente";
   SD_size = "";
  }
  else
  { 
   SD_type = "SDCard Type:";
   if(cardType == CARD_MMC)
   {
    SD_type += "MMC";
   }
   else if(cardType == CARD_SD)
   {
    SD_type += "SDSC";
   }
   else if(cardType == CARD_SDHC)
   {
    SD_type += "SDHC";
   }
   else
   {
    SD_type += "UNKNOWN";
   }

   uint64_t cardSize = SD.cardSize() / (1024 * 1024);

   SD_size = "Size:" + (String) cardSize + "MB";
  }
 }
///////////////////////////////////////////////////////////
// loading dei parametri pre-impostati nella flash
///////////////////////////////////////////////////////////
 pref.begin("ftpserver", false);

 //get values from preferences
 if (pref.isKey("ssid"))
  ssid = pref.getString("ssid");
 if (pref.isKey("pkey"))
  pkey = pref.getString("pkey");
 
 if (pref.isKey("userftp"))
  userftp = pref.getString("userftp");
 if (pref.isKey("passwordftp"))
  passwordftp = pref.getString("passwordftp");

//////////////////////////////////////////
// preferences.h
//////////////////////////////////////////
// funzioni per tipo che permettono l'accesso alle variabili caricate su flash
// size_t putChar(const char* key, int8_t value)
// size_t putUChar(const char* key, uint8_t value)
// size_t putShort(const char* key, int16_t value)
// size_t putUShort(const char* key, uint16_t value)
// size_t putInt(const char* key, int32_t value)
// size_t putUInt(const char* key, uint32_t value)
// size_t putLong(const char* key, int32_t value)
// size_t putULong(const char* key, uint32_t value)
// size_t putFloat(const char* key, float_t value)
// size_t putBool(const char* key, bool value)
// size_t putString(const char* key, const char* value);
// size_t putString(const char* key, String value);
// int8_t getChar(const char* key [, int8_t defaultValue = 0])
// uint8_t getUChar(const char* key [, uint8_t defaultValue = 0])
// int16_t getShort(const char* key [, int16_t defaultValue = 0])
// uint16_t getUShort(const char* key [, uint16_t defaultValue = 0])
// int32_t getInt(const char* key [, int32_t defaultValue = 0])
// uint32_t getUInt(const char* key [, uint32_t defaultValue = 0])
// int32_t getLong(const char* key [, int32_t defaultValue = 0])
// uint32_t getULong(const char* key [, uint32_t defaultValue = 0])
// bool getBool(const char* key [, bool defaultValue = false]);
// String getString(const char* key [, String defaultValue = String()]);
//////  

/////////////////////////////////////////////////// 
// tenta la connessione al wifi se presenti ssid e password altrimenti apre la connesisone locale per permettere la loro impostazione tramite interfaccia web
// la funzione è definita nel modulo webserver.ino
///////////////////////////////////////////////////
//
// MAC 08:d1:f9:cb:53:00

 connected = initWiFi(ssid, pkey);

 if (connected)
 {
  //successful connection
  //setup real time clock
  configTzTime("CET-1CEST,M3.5.0/03,M10.5.0/03", ntp.c_str());
//              CET-1CEST,M3.5.0,M10.5.0/3
  //show date and time and the name of the station
  delay(500);

  conn_WiFiSSID = "Connect SSID " + String(ssid);
  conn_localIP = WiFi.localIP().toString();

  //fill ti, minutes and weekday with now
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

   conn_date = String(days[ti.tm_wday]) + ",";

   if (ti.tm_mday < 10)
    conn_date += "0" + String(ti.tm_mday) + " ";
   else
    conn_date += String(ti.tm_mday) + " ";

   conn_date += (String) months[ti.tm_mon] + " ";

   conn_date += String(ti.tm_year + 1900) + " ";

   if (ti.tm_hour < 10)
    conn_time = "0" + String(ti.tm_hour) + ":";
   else
    conn_time = String(ti.tm_hour) + ":";
    
   if (ti.tm_min < 10)
    conn_time += "0" + String(ti.tm_min) + ":";
   else
    conn_time += String(ti.tm_min) + ":";

   if (ti.tm_sec < 10)
    conn_time += "0" + String(ti.tm_sec);
   else
    conn_time += String(ti.tm_sec);

   conn_time += " (DST "+ String(ti.tm_isdst) + ")";
  }
 }
 else  //connection not successful
 {
  //we have no connection. A message will be shown on the display
  
  conn_WiFiSSID = "Not connected to WiFi";
  conn_localIP =  "Use SSID:ftpserver";
  conn_date =     "      IP:192.168.4.1";
  conn_time =     "";
 }

//#ifdef LOGDEBUG
 defLogFile();
 writeLog("-------------");
 writeLog("-- restart --");
 writeLog("-------------");
 writeLog("SD started");
 writeLog("Start webserver");
 fsCurrentLog.close();
//#endif
 
 //setup the web server and the over the air update
 setup_webserver();

 setup_ota();

// predispongo i PIN che devono essere controllati per i bottoni
 init_button();
 
 delay(5000);
 
//////////////////////////////////
// ftpserver
//////////////////////////////////
 if (connected && (cardType != CARD_NONE))
 {
// verifica se nella SD ci sono le directory opportune e nel caso opposto le crea
  checkFS();

  ftpSrv.setCallback(_callback);
  ftpSrv.setTransferCallback(_transferCallback);

  ftpSrv.begin(userftp.c_str(),passwordftp.c_str()); //username, password per l'accesso al server ftp. Le variabili String vanno cast-ate come const char * (default 21, 50009 for PASV)
//#ifdef LOGDEBUG
  defLogFile();
  writeLog("Start ftp server");
  fsCurrentLog.close();
//#endif
  
  ftpserveractive = true;
  
//////////////////////////////////////////////////////////////////////////
// Inizializzazione PORTA LED UPLOAD DATA
//////////////////////////////////////////////////////////////////////////
  pinMode(LED_FTPOPERATION_PIN, OUTPUT);
  ledFTPoperationOFF();
 }

// attivo per primo il menu che mostra lo stato della connessione
 menu_conn();
 
//////////////////////////////////
// standby off attivo
//////////////////////////////////
 standby_reset(millis());
}

//////////////////////////////////////////////////////////////////////
// loop
//////////////////////////////////////////////////////////////////////
void loop()
{
 //Check over the air update
 ArduinoOTA.handle();

 //Check for http requests
 webserver_loop();

///////////////////////////////////////////
// controllo dei bottoni
///////////////////////////////////////////
 manage_button();
 
//////////////////////////////////
// ftpserver
//////////////////////////////////
 if (connected && (cardType != CARD_NONE))
 {
  ftpSrv.handleFTP();
 }
//////////////////////////////////
// check_standby
//////////////////////////////////
 check_standby(); 
}
