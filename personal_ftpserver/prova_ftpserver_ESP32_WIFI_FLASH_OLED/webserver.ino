////////////////////////////////////////////////////////////////////////////
// va incluso il modulo che contiene la definizione di variabili array contenenti il codice HTML che altrimenti risultano sconosciute
////////////////////////////////////////////////////////////////////////////
#include "htmlcode.h"

//////////////////////////////////
// ftpserver esp32
//////////////////////////////////
#include "ftpserveresp32.h"

//setup request distribution
void setup_webserver()
{
 server.onNotFound(handleNotFound);    
 server.on("/shelly",handleShelly);    
 server.on("/",handleRoot);
 server.on("/cmd/inline", [] ()
           {
            Serial.println("handle inline");
            server.send(200, "text/plain", "this works as well");
           } 
          );
 //handle ajax commands
 server.on("/cmd/restart",restart);
 server.on("/cmd/setaccess",setAccessData);
 server.on("/cmd/getaccess",getAccessData);
 //start webserver
 server.begin();
}

//called from main loop
void webserver_loop()
{
 server.handleClient();
}

//handle root request
void handleRoot()
{
 String page;
 
 if (connected)
 {
  //if connected send the main page
  
  page = FPSTR(HTML_head);
  page += FPSTR(HTML_start_script);
  
//{"grp":[{"grpname":"primo gruppo","attr":[{"attrname":"primo attributo","attrtype":"text","attrvalue":"primo valore"},{"attrname":"secondo attributo","attrtype":"password","attrvalue":"secondo valore"},{"attrname":"terzo attributo","attrtype":"number","attrvalue":123},{"attrname":"quarto attributo","attrtype":"select","attrvalue":["opzione uno","opzione uno","opzione due","opzione tre"]}]},{"grpname":"secondo gruppo","attr":[{"attrname":"quinto attributo","attrtype":"text","attrvalue":"quinto valore"},{"attrname":"sesto attributo","attrtype":"password","attrvalue":"sesto valore"}]}]}
// nella select la prima option per default assumo che sia il valore memorizzato nella flash. opzione che non può essere scelta, che può essere una stringa nulla e che eventualmente si ripete di seguito 

  page += "preloadedJSON=\'{\"grp\":[";
  
  page += "{\"grpname\":\"WiFi configuration\",\"attr\":[";
  
  page += "{\"attrname\":\"ssid\",";
  int numSsid = WiFi.scanNetworks();
  if (numSsid < 1)
  {
   page +=  "\"attrtype\":\"text\",";
   page +=  "\"attrvalue\":\"no network\"";
  }
  else
  {      
   page +=  "\"attrtype\":\"select\",";
   page +=  "\"attrvalue\":[\"";
//   if (pref.isKey("ssid")) // se esiste carica come primo il valore memorizzato altrimenti lo mette vuoto dato che la pagina html mostra di default il primo valore come quello memorizzato 
//   {
//    page += pref.getString("ssid");
//   }
   page += ssid;
   page += "\",";
       
   for (int thisNet = 0; thisNet < numSsid; thisNet++)
   {
    page += "\"";
    page += WiFi.SSID(thisNet);
    page += "\",";
   }
   page = page.substring(0, page.length()-1); // elimino l'ultima virgola   
  }
  page +=  "]},";
  
  page += "{\"attrname\":\"pkey\",";
  page +=  "\"attrtype\":\"password\",";
  page +=  "\"attrvalue\":\"";
//  if (pref.isKey("pkey"))
//   page += pref.getString("pkey");
  page += pkey;
  page +=  "\"}";
  
  page += "]},"; // chiude il gruppo
  
  page += "{\"grpname\":\"FTP server configuration\",\"attr\":[";

  page += "{\"attrname\":\"userftp\",";
  page +=  "\"attrtype\":\"text\",";
  page +=  "\"attrvalue\":\"";
//  if (pref.isKey("userftp"))
//   page += pref.getString("userftp");
  page += userftp;
  page +=  "\"},";
  
  page += "{\"attrname\":\"passwordftp\",";
  page +=  "\"attrtype\":\"password\",";
  page +=  "\"attrvalue\":\"";
//  if (pref.isKey("passwordftp"))
//   page += pref.getString("passwordftp");
  page += passwordftp;
  page +=  "\"}";
  
  page += "]},"; // chiude il gruppo
  
  page += "{\"grpname\":\"Stand By Interval\",\"attr\":[";

  page += "{\"attrname\":\"standbyInterval\",";
  page +=  "\"attrtype\":\"number\",";
  page +=  "\"attrvalue\":";
  page += String(standbyInterval);
  page +=  "}";
  
  page += "]},"; // chiude il gruppo
  
  page += "{\"grpname\":\"LED Flash\",\"attr\":[";

  page += "{\"attrname\":\"ledFlash\",";
  page +=  "\"attrtype\":\"number\",";
  page +=  "\"attrvalue\":";
  page += String(ledFlash);
  page +=  "}";

  page += "]},"; // chiude il gruppo
  
  page += "{\"grpname\":\"Button timeout\",\"attr\":[";

  page += "{\"attrname\":\"longInterval\",";
  page +=  "\"attrtype\":\"number\",";
  page +=  "\"attrvalue\":";
  page += longInterval;
  page +=  "},";
  
  page += "{\"attrname\":\"glitchInterval\",";
  page +=  "\"attrtype\":\"number\",";
  page +=  "\"attrvalue\":";
  page += glitchInterval;
  page +=  "}";
  
  page += "]}"; // chiude il gruppo --> manca la virgola perché è l'ultimo gruppo
  
  page += "]}\';\n";
  
  page += "urldatasave=\"/cmd/setaccess\";";
  page += "urlreset=\"/cmd/restart\";";

  page += FPSTR(HTML_global_script);
  page += FPSTR(HTML_end_script);
  page += FPSTR(HTML_main_body);
  page += FPSTR(HTML_tail);
  
  server.send(200,"text/html",page);

//  dispdebug("client loaded root page");
//  dispmsg("center", "center", "client load root page");
 }
 else
 {
  //if not connected we use the config page without ajax
  //we get parameters as arguments in the HTML-request
  Serial.println("Got config:");
  uint8_t a = server.args();
  Serial.print(a);
  Serial.println(" Arguments");
  for (uint8_t i = 0; i<a; i++)
  {
   Serial.print("arg(");
   Serial.print(i);
   Serial.print(") -> ");
   Serial.print(server.argName(i));
   Serial.print(" = ");
   Serial.println(server.arg(i));
  }
  if (server.hasArg("conf_ssid"))
  {
   pref.putString("ssid",server.arg("conf_ssid")); //save SSID in the preferences
   Serial.println(server.arg("conf_ssid"));
  }
  if (server.hasArg("conf_pkey"))
  {
   pref.putString("pkey",server.arg("conf_pkey")); // save PKEY in the preferences
   Serial.println(server.arg("conf_pkey"));
  }
  if (server.hasArg("reset"))
  {
   Serial.println("Restart!"); //initiate a restart
   ESP.restart();
  }
  server.send(200,"text/html",CONFIG_page); //send the config page to the client

//  dispdebug("client loaded config page");
//  clear_display();
//  dispmsg("center", "center","client load conf page");
//  u8g2.sendBuffer(); // invio i messaggi sul display
 }
}

//AJAX command /cmd/setaccess
void setAccessData()
{
 String data = server.arg("plain");
 const char* attrname;
 const char* attrvalue;
 String sattrvalue;

 StaticJsonDocument<1024> jBuffer;
 auto error = deserializeJson(jBuffer, data);

// clear_display();

 if (error)
 {
  //respond with NOT OK
  server.send(200,"text/plain","NOT OK");

//  dispdebug("client setAccessData fail");
//  dispmsg("center", "center","client setAccData fail");
 }
 else
 {
  for (JsonObject attr_item : jBuffer["attr"].as<JsonArray>())
  {
   attrname=attr_item["attrname"]; // è necessario copiare i valori su delle variabili prima di essere utilizzati direttamente con String()
   attrvalue=attr_item["attrvalue"];

// ssid e password di accesso al wifi
   if (String(attrname)=="ssid")
   {    
    ssid=String(attrvalue);
    pref.putString("ssid",ssid);
   }
   if (String(attrname)=="pkey")
   {
    pkey=String(attrvalue);
    pref.putString("pkey",pkey);
   }

// user e password di accesso al server ftp
   if (String(attrname)=="userftp")
   {
    userftp=String(attrvalue);
    pref.putString("userftp",userftp);
   }
   if (String(attrname)=="passwordftp")
   {
    passwordftp=String(attrvalue);
    pref.putString("passwordftp",passwordftp);
   }
   
// Stand By Interval
   if (String(attrname)=="standbyInterval")
   {
    sattrvalue = String(attrvalue);
    standbyInterval=(uint32_t) sattrvalue.toInt();
    
    if (standbyInterval==0)
     standbyInterval=STANDBYINTERVAL;
    pref.putUInt("standbyInterval",standbyInterval);
   }
   
// LED Flash
   if (String(attrname)=="ledFlash")
   {
    sattrvalue = String(attrvalue);
    ledFlash=(uint16_t) sattrvalue.toInt();

    if (ledFlash==0)
     ledFlash=LEDFLASH;
    pref.putUShort("ledFlash",ledFlash);
   }
   
// Button timeout
   if (String(attrname)=="longInterval")
   {
    sattrvalue = String(attrvalue);
    longInterval=(uint16_t) sattrvalue.toInt();

    if (longInterval==0)
     longInterval=LONGINTERVAL;
    pref.putUShort("longInterval",longInterval);
   }
   if (String(attrname)=="glitchInterval")
   {
    sattrvalue = String(attrvalue);
    glitchInterval=(uint8_t) sattrvalue.toInt();

    if (glitchInterval==0)
     glitchInterval=GLITCHINTERVAL;
    pref.putUChar("glitchInterval",glitchInterval);
   }
  }
  //respond with OK
  server.send(200,"text/plain","OK");

//  dispdebug("client setAccessData ok");
//  dispmsg("center", "center","client setAccData ok");
 }

// u8g2.sendBuffer(); // invio i messaggi sul display
}

//AJAX command /cmd/getaccess
void getAccessData()
{
 //send access data separated by new line
 String str = String(ssid) + "\n" + String(pkey);
 //respond with access data
 server.send(200,"text/plain",str);
}

//AJAX command /cmd/restart
void restart()
{
// dispdebug("restart!");

// clear_display();
// dispmsg("center", "center","restart!");
// u8g2.sendBuffer(); // invio i messaggi sul display
 
 ESP.restart();
}

void handleShelly()
{
 Serial.println("handle Shelly");

 //respond with OK
 server.send(200,"text/plain","OK");

// dispdebug("client handle Shelly");
// clear_display();
// dispmsg("center", "center", "client handle Shelly");
// u8g2.sendBuffer(); // invio i messaggi sul display
}

void handleNotFound()
{
 Serial.println("handle not found");

 String message = "File Not Found ";
 message += "URI: ";
 message += server.uri();
 message += " Method: ";
 message += (server.method() == HTTP_GET) ? "GET" : "POST";
 message += " Arguments: ";
 message += server.args();
 message += " ";
 for (uint8_t i = 0; i < server.args(); i++)
 {
  message += " " + server.argName(i) + ": " + server.arg(i) + ", ";
 }
 server.send(404, "text/plain", message);

// dispdebug("client handle not found " + message);
// clear_display();
// dispmsg("center", "center","client hand not found " + message);
// u8g2.sendBuffer(); // invio i messaggi sul display
}

//////////////////////////////////////////////
// WLAN
//////////////////////////////////////////////
//connect to the WLAN
boolean initWiFi(String ssid, String pkey)
{
 boolean connected = false;
 //first disconnect
 Serial.println("Disconnect");
 WiFi.disconnect();
 Serial.println("Soft AP disconnect");
 WiFi.softAPdisconnect(true);
 Serial.println("Set WLAN Mode");
 //satrt station mode
 WiFi.mode(WIFI_STA);
 Serial.print("connect to ");
 Serial.print(ssid);
 Serial.print(" pass ");
 Serial.println(pkey);
 //if we have a config we try to connect
 if (ssid != "")
 {
  WiFi.begin(ssid.c_str(),pkey.c_str());
  uint8_t cnt = 0;
  while ((WiFi.status() != WL_CONNECTED) && (cnt<20))
  {
   delay(500);
   Serial.print(".");
   cnt++;
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED)
  {
   Serial.print("IP Address = ");
   Serial.println(WiFi.localIP());
   connected = true;
  }
 }
 //if we have no connection /no config or wrong config)
 //we start an access point to allow configuration
 if (!connected)
 {
  Serial.println("No connection! \nStart AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ftpserver","",1);
 }
 return connected;
}
