# personal FTP server

## obiettivi
Dispongo di una IP-CAM che funge sostanzialmente da deterrente davanti la porta di casa. Al passaggio di estranei (e del gatto del vicino) la IP-CAM si attiva e registra un breve video che tuttavia è accessibile solo attraverso il cloud. Ovviamente tale cloud non è sempre disponibile e, a fronte di un allarme, qualche volta non è possibile prendere visione del video registrato proprio a causa di questa assenza di collegamento. Tra le opzioni disponibili nella configurazione della IP-CAM c'è la possibilità di inviare il video appena registra ad un server FTP. Da qui l'esigenza di disporre di un tale sistema.
Sfruttando qualche risorsa hardware che giaceva non usata nel cassetto, ho progettato (lo so, è una parola grossa) un piccolo circuito che permettesse di
* gestire le richieste di invio dei file da parte della IP-CAM attraverso il protocollo FTP
* poter memorizzare tali file in una SD
* poter disporre di una interfaccia ethernet (più che altro per alleggerire il WiFi di casa)
* poter visualizzare alcuni dati essenziali circo lo stato del piccolo server attraverso un display e un led
* permettere di eseguire piccole operazioni di manutenzione/configurazione attraverso tale display e pochi bottoni
* poter accedere a tale server anche attraverso una semplice interfaccia web per eseguireoperazioni di configurazione e di controllo dello stato più sofisticate e sopratutto utilizzando una via alternativa al protocollo FTP.
## configurazione hardware
![schema elettrico](https://github.com/robertopapi/esp32/blob/587e630d40c67c2ef1f819fcd351a06fc47dd77d/personal_ftpserver/schema1.png)
## software
![Interfaccia WEB](https://github.com/robertopapi/esp32/blob/7b93e4ae0f42f2d6a9d236c72bfa833daffc2b7c/personal_ftpserver/interfaccia_web.png)

## funzionamento
## conclusioni
