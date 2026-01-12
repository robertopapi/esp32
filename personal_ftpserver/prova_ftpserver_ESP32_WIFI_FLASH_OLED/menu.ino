#include "FS.h"

//////////////////////////////////
// ftpserver esp32
//////////////////////////////////
#include "ftpserveresp32.h"

// definisco i tipi di item: ciascuno implica una diversa gestione
#define ITEM_LOCK 0 // blocca la viasualizzazione dell'item e impedisce la sua selezione e lo scroll
#define ITEM_TEXT 1 // visualizza l'item, ne permette il suo scroll con i bottoni UP e DOWN ma ne impedisce la selezione
#define ITEM_MENU 2 // visualizza l'item in modo caratterizzato, ne permette lo scroll e la sua selezione, eseguendo la corrispondente funzione di gestione
#define ITEM_DIRECTORY 3 // visualizza l'item in modo caratterizzato, ne permette lo scroll e la selezione, eseguendo la corrispondente funzione di gestione
#define ITEM_FILE 4 // visualizza l'item in modo caratterizzato, ne permette lo scroll e la selezione, eseguendo la corrispondente funzione di gestione

void show_menu()
{
 clear_display();
 
 if (currentRowPosition > 0)
  draw_box(currentRowPosition, 1, 1); // disegno un box di sfondo all'item selezionato a partire dalla posizione currentRowPosition per 1 riga del colore 1
//  draw_frame(currentRowPosition, 1, 1); // disegno un box di sfondo all'item selezionato a partire dalla posizione currentRowPosition per 1 riga del colore 1
 
  // DA MODIFICARE
  // deve partire dalla posizione del primo item visibile per il numero degli item che entrano nel display
 for (uint8_t i=0; i < totalItem; i++)
 {
//  dispmsg(String(u8g2.getMaxCharWidth()), String((HSMALLFONT*i)+HSMALLFONT), displayItem[i].itemName); // faccio partire la scrittura dell'item dal numero di pixel pari alla massima dimensione del carattere più a destra
  dispmsg("2", String((HSMALLFONT*i)+HSMALLFONT), displayItem[i].itemName); // faccio partire la scrittura dell'item da un paio di pixel più a destra
 }

 u8g2.sendBuffer(); // invio i messaggi sul display
}

// creo una funzione dummy per gestire gli item che non richiedono una funzione di gestione del reload
void menu_dummy()
{
}

// ripulisce tutti gli item
void clean_item()
{
 for (uint8_t i=0; i < MAX_ITEM; i++)
 {
  displayItem[i].manager =  menu_dummy;
  displayItem[i].internalItem = "";
  displayItem[i].itemName = "";
  displayItem[i].type = ITEM_LOCK;
 }
}

// 
void menu_conn()
{
 currentRowPosition = 0; // imposto a zero perché non ci sono item selezionabili
 currentIdxItemSelected = 0; // imposto l'item selezionato per primo all'ingresso nel menu, nel caso in cui deve essere previsto lo scroll
 
 clean_item();
 
 totalItem = 9;
 
 displayItem[0].manager = menu_home; // imposto per tutti gli item il menu_home oome gestore in modo da gestire la selezione con il bottone select
 displayItem[0].internalItem = "";
 displayItem[0].itemName = conn_WiFiSSID;
 displayItem[0].type = ITEM_TEXT;

 displayItem[1].manager = menu_home;
 displayItem[1].internalItem = "";
 displayItem[1].itemName = conn_localIP;
 displayItem[1].type = ITEM_TEXT;

 displayItem[2].manager = menu_home;
 displayItem[2].internalItem = "";
 displayItem[2].itemName = conn_date;
 displayItem[2].type = ITEM_TEXT;

 displayItem[3].manager = menu_home;
 displayItem[3].internalItem = "";
 displayItem[3].itemName = conn_time;
 displayItem[3].type = ITEM_TEXT;

 displayItem[4].manager = menu_home;
 displayItem[4].internalItem = "";
 displayItem[4].itemName = "";
 displayItem[4].type = ITEM_TEXT;

 displayItem[5].manager = menu_home;
 displayItem[5].internalItem = "";
 displayItem[5].itemName = SD_type;
 displayItem[5].type = ITEM_TEXT;

 displayItem[6].manager = menu_home;
 displayItem[6].internalItem = "";
 displayItem[6].itemName = SD_size;
 displayItem[6].type = ITEM_TEXT;

 displayItem[7].manager = menu_home;
 displayItem[7].internalItem = "";
 displayItem[7].itemName = "";
 displayItem[7].type = ITEM_TEXT;

 displayItem[8].manager = menu_home;
 displayItem[8].internalItem = "";

 displayItem[8].itemName = "FTPserver ";
 
 if (ftpserveractive)
  displayItem[8].itemName += "ACTIVE";
 else
  displayItem[8].itemName += "INACTIVE";
  
 displayItem[8].type = ITEM_TEXT;

 show_menu(); 
}

// 
void menu_log()
{
 currentRowPosition = 0; // resetto la posizione all'ingresso del menu
 currentIdxItemSelected = 0; // imposto l'item selezionato per primo all'ingresso nel menu
 
 clean_item();
 
 totalItem = 1;
 
 displayItem[0].manager = menu_home;
 displayItem[0].internalItem = "";
 displayItem[0].itemName = "menu LOG";
 displayItem[0].type = ITEM_TEXT;

 show_menu(); 
}

// 
void menu_conf()
{
 currentRowPosition = 0; // imposto a zero perché non ci sono item selezionabili
 currentIdxItemSelected = 0; // imposto l'item selezionato per primo all'ingresso nel menu, nel caso in cui deve essere previsto lo scroll
 
 clean_item();
 
 totalItem = 1;
 
 displayItem[0].manager = menu_home;
 displayItem[0].internalItem = "";
 displayItem[0].itemName = "menu conf";
 displayItem[0].type = ITEM_TEXT;

 show_menu(); 
}

// 
void menu_data()
{
 currentRowPosition = 0; // resetto la posizione all'ingresso del menu
 currentIdxItemSelected = 0; // imposto l'item selezionato per primo all'ingresso nel menu
 
 clean_item();
 
 totalItem = 1;
 
 displayItem[0].manager = menu_home;
 displayItem[0].internalItem = "";
 displayItem[0].itemName = "menu data";
 displayItem[0].type = ITEM_TEXT;

 show_menu(); 
}

// 
void menu_stat()
{
 currentRowPosition = 0; // resetto la posizione all'ingresso del menu
 currentIdxItemSelected = 0; // imposto l'item selezionato per primo all'ingresso nel menu
 
 clean_item();
 
 totalItem = 1;
 
 displayItem[0].manager = menu_home;
 displayItem[0].internalItem = "";
 displayItem[0].itemName = "menu stat";
 displayItem[0].type = ITEM_TEXT;

 show_menu(); 
}

void menu_home()
{
 currentRowPosition = 1; // resetto la posizione all'ingresso del menu
 currentIdxItemSelected = 0; // imposto l'item selezionato per primo all'ingresso nel menu

 clean_item();

 totalItem = 5; // totale degli item selezionabili
 
 displayItem[0].manager = menu_conn;
 displayItem[0].internalItem = "";
 displayItem[0].itemName = "Connect Data";
 displayItem[0].type = ITEM_MENU;
 
 displayItem[1].manager = menu_log;
 displayItem[1].internalItem = "/log";
 displayItem[1].itemName = "Log";
 displayItem[1].type = ITEM_MENU;
 
 displayItem[2].manager = menu_conf;
 displayItem[2].internalItem = "";
 displayItem[2].itemName = "Configuration";
 displayItem[2].type = ITEM_MENU;
 
 displayItem[3].manager = menu_data;
 displayItem[3].internalItem = "";
 displayItem[3].itemName = "Data";
 displayItem[3].type = ITEM_MENU;
 
 displayItem[4].manager = menu_stat;
 displayItem[4].internalItem = "";
 displayItem[4].itemName = "Statistics";
 displayItem[4].type = ITEM_MENU;

 show_menu(); 
}