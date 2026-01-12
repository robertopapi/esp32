
//prepare OTA
void setup_ota()
{
  //set host name and passwort
  ArduinoOTA.setHostname("ftpserver");
  ArduinoOTA.setPassword("ftpserver");
  //register callback routines
  ArduinoOTA.onStart(ota_onStart);
  ArduinoOTA.onEnd(ota_onEnd);
  ArduinoOTA.onProgress(ota_onProgress);
  ArduinoOTA.onError(ota_onError);
  //start OTA handling
  ArduinoOTA.begin();
}

//on start decide between program or file upload
void ota_onStart()
{
 String type;
 if (ArduinoOTA.getCommand() == U_FLASH)
  type = "sketch";
 else // U_SPIFFS
  type = "filesystem";

  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()

 standby_reset(millis());
 clear_display();
 dispmsg("center", "center", "Updating Sketch");
 u8g2.sendBuffer(); // invio i messaggi sul display
}

//on end no extra processing
void ota_onEnd()
{
 standby_reset(millis());
 clear_display();
 dispmsg("center", String(HSMALLFONT*4), "Done");
 u8g2.sendBuffer(); // invio i messaggi sul display
}

void ota_onProgress(unsigned int progress, unsigned int total)
{
 uint32_t prc = progress / (total / 100);

 clear_row(4, 3);
 dispmsg("center", String(HSMALLFONT*4), "Updating Sketch");
 dispmsg("center", String(HSMALLFONT*6), String(prc) + "%");
 u8g2.sendBuffer(); // invio i messaggi sul display
}

void ota_onError(ota_error_t error)
{
 if (error == OTA_AUTH_ERROR)
  msg = "Error[" + String(error) + "]: Auth Failed";
 else if (error == OTA_BEGIN_ERROR)
  msg = "Error[" + String(error) + "]: Begin Failed";
 else if (error == OTA_CONNECT_ERROR)
  msg = "Error[" + String(error) + "]: Connect Failed";
 else if (error == OTA_RECEIVE_ERROR)
  msg = "Error[" + String(error) + "]: Receive Failed";
 else if (error == OTA_END_ERROR)
  msg = "Error[" + String(error) + "]: End Failed";
 else
  msg = "Error[" + String(error) + "]";
   
 standby_reset(millis());
 clear_display();
 dispmsg("center", "center", msg);
 u8g2.sendBuffer(); // invio i messaggi sul display
}
