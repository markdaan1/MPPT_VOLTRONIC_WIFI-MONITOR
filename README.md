# MPPT_VOLTRONIC_WIFI-MONITOR
WIFI MONITOR PER INVERTER MPPT VOLTRONIC MUNITO DI USCITA SERIALE
E' PREVISTA ANCHE UNA CHIAMATA PER LOGGARE I DATI MOSTRATI


FUNZIONAMENTO 

IL MONITOR MOSTRA SOLO LE INFORMAZIONI UTILI AI FINI DEI CONSUMI, E' PREVISTA ANCHE UNA CHIAMATA AD UNA API PER IL LOG DELLE INFO MOSTRATE
QUANDO I WATT SONO SCRITTI IN VERDE SI INTENDONO I WATT RESIDUI DALLA PRODUZIONE DEI PANNELLI

QUANTO I WATT SONO SCRITTI IN ROSSO SI INTENDE LA POTENZA PRELEVATA DALLE BATTERIE

QUANDO C'E' LA SCRITTA "BYPASS ATTIVO" VUOL DIRE CHE SI STA PRELEVANDO CORRENTE DALLA RETE ELETTRICA (ES. DI NOTTE QUANDO I PANNELLI NON PRODUCONO E LE BATTERIE SONO SCARICHE)

![FONDO](https://user-images.githubusercontent.com/68028438/190213352-56dcf71b-41de-40a3-930d-c9edf5be166e.jpg)


QUESTO PROGETTO UTILIZZA UN'INTERFACCIA ESP8266 PER LA COMUNICAZIONE CON L'INVERTER.
UNA VECCHIA RASPBERRY B+ E' UTILIZZATA PER CATTURARE IL SEGNALE ALL'INTERNO DI CASA E MOSTRARE SUL MONITOR I DATI.

MONITOR-> https://www.waveshare.com/product/displays/lcd-oled/lcd-oled-1/5inch-hdmi-lcd.htm
CONVERTITORE->  https://www.amazon.it/gp/product/B00WBD5RCU/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1


------------                                                          ---------
|   MPPT   | -> CONV 232/ttl -> ESP8266 -> WIFI AP -> RASPBERRY B+ ->| MONITOR | -> API PHP -> DB
------------                                                          ---------

IL CODICE DA INSERIRE NELL'INTERFACCIA E' CONTENUTO NEL FILE UDP.c

IL CODICE PER LA RASPBERRY E' CONTENUTO NEL FILE MONITOR.c

IL CODICE PER LA CHIAMATA E' CONTENUTO NEL FILE INSTAB.PHP

IL DUMP DEL DATABASE E' CONTENUTO NEL FILE MONITOR.SQL

E' POSSIBILE ANNULLARE LA SCRITTURA SU DB REMMANDO LA RIGA RESPONSABILE DELLA SCRITTURA

PER IL FUNZIONAMENTO E' NECESSARIO INSTALLARE LE LIBRERIE RICHIESTE NEL CODICE


COMANDO DI COMPILAZIONE 
gcc -I"/usr/include/SDL" -o monitor monitor.c -lSDL -lSDL_ttf -lcurl -lpthread




QUI VA INSERITA LA CHIAMATA ALLA PAGINA DI INSERIMENTO NEL DB
curlLhttps://www.xxxx.xxx/xxxx/xxxx/ins.php?cmd=%s

![image](https://user-images.githubusercontent.com/68028438/190207062-70cd6867-8ae5-48dd-8b48-bd9f89fe123c.png)
![image](https://user-images.githubusercontent.com/68028438/190207255-343afdac-2276-4e15-8e2a-3be1ecb34171.png)
![image](https://user-images.githubusercontent.com/68028438/190207534-30725f9c-6817-44c4-8516-68ee0f4a3bad.png)

![image](https://user-images.githubusercontent.com/68028438/190208415-44a734c4-ed16-48ff-914e-3a5a6af8fdd4.png)

![image](https://user-images.githubusercontent.com/68028438/190215666-00d9140f-2f2f-44d6-a29c-25fe43180ab9.png)
![image](https://user-images.githubusercontent.com/68028438/190215785-58382f08-6459-4c54-9ff9-814b2972b789.png)


![image](https://user-images.githubusercontent.com/68028438/190208846-401e3e5f-8bef-4275-8ac8-738628f1a271.png)


![image](https://user-images.githubusercontent.com/68028438/190209680-157e6198-883a-4d19-ab0d-97c7568ca53f.png)

![image](https://user-images.githubusercontent.com/68028438/190209794-08100825-40ac-48e8-b3bd-c3e6e81a490c.png)
![image](https://user-images.githubusercontent.com/68028438/190209903-01f28677-6e07-4aba-a243-13dea3b3880b.png)
![image](https://user-images.githubusercontent.com/68028438/190209953-4fc82831-5d94-4243-8ca7-afcd9cb668ec.png)
![image](https://user-images.githubusercontent.com/68028438/190210036-e08c8d85-408d-4a87-b31a-08512536ca60.png)


