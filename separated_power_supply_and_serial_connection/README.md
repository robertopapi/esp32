# separated power supply and serial connection for ESP32
## Problema: come alimentare con la dovuta potenza un circuito con ESP32 e utilizzare contemporaneamente il collegamento con la porta seriale via USB

Durante l'esecuzione dei miei esperimenti, mi sono imbattuto nel problema di dover continuare a fare debug, analizzando l'output del controllore via seriale/USB e contemporaneamente provvedere alla quantità di corrente sufficiente ad alimentare sia il controllore che tutti gli altri circuiti collegati. Tale fenomeno si è manifestato quando la porta USB del vecchio PC che utilizzavo fino a qualche tempo fa, costretto a causa di alcuni problemi di compatibilità con i vari driver, non erogava più dei soliti 400-500mA a fronte di una richiesta di oltre 2A del circuito che stavo testando.
Il problema dunque è stato quello di alimentare il circuito utilizzando una fonte alternativa e contemporaneamente continuare ad utilizzare la porta seriale tramite la USB senza tuttavia sottoporre il PC ad una richiesta di energia tale da poterlo usare da li a poco solo come scaldavivande.
Consultando la documentazione ufficiale di espressif ( [rif](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp-dev-kits-en-master-esp32.pdf) ) è emerso quanto segue:

![separated_power_supply_and_serial_connection/power_supply.png](https://github.com/robertopapi/esp32/blob/aff6d53e1dd5cae917c822b71488a9577f9bc633/separated_power_supply_and_serial_connection/power_supply.png)

Quindi ho provveduto a collegare un nuovo connettore micro USB al circuito dal quale prelevare la sola alimentazione a +5V. A questo punto, per continuare ad utilizzare la porta seriale/USB senza bruciare ne controllore ne PC, ho realizzato un piccolo circuito tramite il quale collegare la porta microUSB del ESP32 con il PC senza tuttavia collegare il pin relativo ai +5V, quindi la sola massa con i pin +D e -D.

in questo modo è possibile alimentare il circuito e contemporaneamente continuare ad utilizzare la porta seriale/USB per il debug.




