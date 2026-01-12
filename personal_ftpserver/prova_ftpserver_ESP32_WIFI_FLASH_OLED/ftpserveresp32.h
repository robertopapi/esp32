#ifndef FTPSERVERESP32_H
#define FTPSERVERESP32_H

// variabili di appoggio per l'accesso al wifi
String ssid = "";                 //ssid for WLAN connection
String pkey = "";                 //passkey for WLAN connection
boolean connected;                //flag to signal active connection
String ntp = "it.pool.ntp.org";   //NTP server url
struct tm ti;                     //time structure with current time

// variabili di appoggio per l'accesso al server ftp con impostati i default
String userftp = "ftpserver"; //user server ftp 
String passwordftp = "ftpserver"; //password server ftp 

bool ftpserveractive = false; // ftp server status

// stringa per i messaggi per la seriale e per il display
String msg;

String SD_type;
String SD_size;

String conn_WiFiSSID;
String conn_localIP;
String conn_date;
String conn_time;

//giorni e mesi
const char* const PROGMEM days[] = {"Domenica","Lunedì","Martedì","Mercoledì","Giovedì","Venerdì","Sabato"};
const char* PROGMEM months[] = {"Gennaio","Febbraio","Marzo","Aprile","Maggio","Giugno","Luglio","Agosto","Settembre","Ottobre","Novembre","Dicembre"};

////////////////////////////////////////////////////////////
// DISPLAY
////////////////////////////////////////////////////////////
//w=16 h=32 u8g2_font_logisoso28_tn

#define BIGFONT u8g2_font_logisoso28_tn
#define HBIGFONT 32

//w=5 h=9 u8g2_font_profont10_mr
//w=6 h=11 u8g2_font_profont12_mr
//w=7 h=15 u8g2_font_profont15_mr
//w=8 h=13 u8g2_font_crox1c_mr
//w=6 h=11 u8g2_font_t0_11_tr
//w=7 h=8 u8g2_font_NokiaSmallPlain_tf 
//w=5 h=8 u8g2_font_5x8_tf
//w=5 h=7 u8g2_font_5x7_tf

//#define SMALLFONT u8g2_font_t0_11_te 
//#define SMALLFONT u8g2_font_profont10_mr
//#define SMALLFONT u8g2_font_NokiaSmallPlain_tf

#define SMALLFONT u8g2_font_5x7_tf

#define HSMALLFONT 7
#define WSMALLFONT 5

#define NROWDISPLAY 9 // 64 pixel verticali display / 7 pixel altezza font

// variabili per la gestione dello standby
bool displaystandby = false; // indica che il timer è scaduto e il display è spento
unsigned long displaystandby_time; // conserva l'istante iniziale in cui il display è acceso

#define STANDBYINTERVAL 120000 // intervallo che definisce lo standby necessario per disattivare il display in assenza di interazioni con i bottoni

/////////////////////////////////////////////////
// inizializzazione display menu
/////////////////////////////////////////////////
// creo un tipo di puntatore a funzione per una maggiore leggibilità
typedef void (*functionPtr)();

// creo la struttura con i parametri e necessari a ciascun item
struct displayItemStruct
{
  functionPtr manager;
  String internalItem;
  String itemName;
  uint8_t type;
};

#define MAX_ITEM 30 // numero massimo di item che posso gestire a display senza fare il reload dei dati

// creo l'array di item disponibili
displayItemStruct displayItem[MAX_ITEM];

//////////////////////////////////////////////
// variabili per la visualizzazione e per l'uso dei bottoni di navigazione
//////////////////////////////////////////////

uint8_t currentRowPosition = 0; // indica la riga corrente visualizzata rispetto al display; è zero se non ci sono item selezionabili
uint8_t currentIdxItemSelected = 0; // indica l'item corrente selezionato rispetto alla struct displayItem e che non coincide con la riga corrente visualizzata 
uint8_t totalItem = 0; // indica il totale degli Item visualizzabili e selezionabili corrispondenti a quelli della struct displayItem
uint8_t firstIdxItem = 0; // indica il primo indice dell'array displayItem visualizzato/visualizzabile correntemente
uint8_t lastIdxItem = 0; // indica l'ultimo indice dell'array displayItem visualizzato/visualizzabile correntemente

//////////////////////////////////////////////////////
// variabili per la gestione del log
//////////////////////////////////////////////////////
File fsCurrentLog;
String pathCurrentLog = "noconnection.log";

//////////////////////////////////////////////////////////////////////////
// attivazione del LOG
//////////////////////////////////////////////////////////////////////////
// #define LOGDEBUG

//////////////////////////////////////////////////////////////////////////
// Inizializzazione LED UPLOAD DATA
//////////////////////////////////////////////////////////////////////////
#define LED_FTPOPERATION_PIN   2
#define LED_FTPOPERATION_FLASH 500

#endif
