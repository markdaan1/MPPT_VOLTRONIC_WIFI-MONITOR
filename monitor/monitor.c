#include <stdlib.h>
#include <assert.h>

#include <string.h>
#include <arpa/inet.h>
 #include <netinet/in.h>
 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <unistd.h>
 
 #include <SDL.h>
 /*FONT*/
#include <SDL_ttf.h>

 
 #include<pthread.h>
 #include <stdbool.h>
#include <curl/curl.h>
 /*DICHIARAZIONE FONT*/
 
TTF_Font *fntmoltopiccolo;
TTF_Font *fntpiccolo;
TTF_Font *fntmedio;
TTF_Font *fntgrande;
SDL_Color FondoTesto;
SDL_Color FondoTestoTMP;

/*DICHIARAZIONE TELE*/
SDL_Surface *Schermata_Tab;
SDL_Surface  *SFONDO_NODATI;
SDL_Surface  *SFONDO_CONDATI;
SDL_Surface  *BYPASS_ATTIVO;
SDL_Rect PosDest;

int active=1;
SDL_Rect PosBypass;


CURL *curl;
CURLcode response;
char str[480];
char Pronto=0;

 #define BUFLEN 512
 #define NPACK 10
 #define PORT 8888
 
 unsigned char PktStart[5]={0x50,0x4B,0x54,0x5F,0x30};
 
 int FlagBypass=0;
 
 int ProduzionePannelli=0;
 int AssorbimentoCasa=0;
 
 int pkt=0;
 float VAC =0;
 float HzAC   =0; 
 float VOUT   =0;  
 float HzOUT  =0;   
 //NC
 int WASSORBITI=0;
 //NC
 //NC
 float VBATT=0;
 int ChgA=0;
 int BATTPERCENTO=0;
 int TMP_INVERTER	=0;
 float PVA=0;
 float PVV=0;
 //NC
 float DISCHARGE_A=0;
 unsigned char STATOCHG=0;
 //NC
 //NC  
 int PV_W=0;
 
/* 
 VAC   
 HzAC    
 VOUT     
 HzOUT     
 APPARENT    
 WASSORBITI
 NC
 NC
 VBATT
 ChgA
 BATTPERCENTO
 TMP_INVERTER	
 PVA
 PVV
 NC
 DISCHARGE_A
 STATOCHG
 NC
 NC  
 PV_W
 

227.1    50.0       227.1     50.0      0522         0428           010        361    51.30     019          066 			0040 			05.9    182.3 00.00   00000               		00010110 								00 00 01083 		 010•¢.
ENEL V   ENEL Hz    OUT V     OUT hz     APPARENT    ACTIVE PWR     carico %   bus V  V batt    batt chg A   carica %		TEMP INVERTER	PV A    PV V   SCC	CORRENTE DI SCARICA		b7b6b5b4b3b2b1b0							xx xx  PV W			LOAD %
																																														b2 1 in carica //0 no
																																														b1 carica da pannelli
																																														b0 carica da rete
												  227.1 50.0 227.
31 20 35 30 2e 30 20 30 35 32 32 20 30 34 32 38   1 50.0 0522 0428
20 30 31 30 20 33 36 31 20 35 31 2e 33 30 20 30    010 361 51.30 0
31 39 20 30 36 36 20 30 30 34 30 20 30 35 2e 39   19 066 0040 05.9
20 31 38 32 2e 33 20 30 30 2e 30 30 20 30 30 30    182.3 00.00 000
30 30 20 30 30 30 31 30 31 31 30 20 30 30 20 30   00 00010110 00 0
30 20 30 31 30 38 33 20 30 31 30 95 a2 0d         0 01083 010•¢.

*/
 void diep(char *s)
   {
     perror(s);
   }
  
  
  
void scritta(TTF_Font *FNT,const char* Scritta,SDL_Color Fondo,SDL_Rect *PosDest ,SDL_Surface *Schermata_Tab)
{
	SDL_Surface  *SCRITTE = TTF_RenderText_Solid(FNT, Scritta, Fondo);

	if (SDL_BlitSurface(SCRITTE,NULL,Schermata_Tab,PosDest) != 0)
	{
		fprintf(stderr, "ERRORE STAMPA TESTO", SDL_GetError());
	}
	SDL_FreeSurface(SCRITTE);
}
/*
void* threadFunction(void* args)
{
	unsigned char Letto=0;
	
	while(active)
	{
		if (Pronto!= Letto)
		{
			printf("TEST->%s\n\n",str);
			 
				
			 
				curl_easy_setopt(curl, CURLOPT_URL, str);
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
				curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
				 
				printf("4.1");
				curl_easy_perform(curl);
				printf("4.2");
				curl_easy_cleanup(curl);
				printf("4.3");
				Pronto=Letto;
			 
			
			//system("pwd");
		}
	}
    
}
*/
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int n = 0;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        n++;
    }
    return n;
}

void scrivilog(char *str)
{
	FILE *pFile;

	pFile=fopen("myfile.txt", "a");
	fprintf(pFile, "%s\n", str);
    fclose(pFile);
}

 
void strclean (char* src) {
    // Run two pointers in parallel.

    char *dst = src;

    // Process every source character.

    while (*src) 
	{
        // Only copy (and update destination pointer) if suitable.
        // Update source pointer always.

        if (islower(*src) || isupper(*src) || *src==')' || *src==':' || *src=='/' || *src=='=' || *src=='\\' || *src=='(' || *src=='&' || *src=='?'  || *src=='0' || *src=='1' || *src=='2' || *src=='3' || *src=='4' || *src=='5' || *src=='6' || *src=='7' || *src=='8' || *src=='9' || *src=='_' || *src=='.' ) *dst++ = *src;
        src++;
    }

    // Finalise destination string.

    *dst = '\0';
}

 int main(void)
 {
	char Stringa[100];
	
	char MiniBuff[50];
	char** VALORI;
	int pos=0;
	struct sockaddr_in si_me, si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];
	SDL_Event ev;
	pthread_t id;
	int ret;
	
	
	
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
	
	scrivilog("START APPLICAZIONE");
	
	/* INIZIALIZZO SDL */
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			fprintf(stderr, "ERRORE INIZIALIZZAZIONE SDL %s", SDL_GetError());
			scrivilog("ERRORE INIZIALIZZAZIONE SDL");
		}
		else
		{
			scrivilog("INIZIALIZZAZIONE SDL OK!");
		}
	/* INIZIALIZZO VIDEO */
	Schermata_Tab = SDL_SetVideoMode(800,480,0,SDL_HWSURFACE|SDL_DOUBLEBUF	|SDL_FULLSCREEN); //SDL_HWSURFACE|SDL_DOUBLEBUF	|SDL_FULLSCREEN);//1500,1000

	if (Schermata_Tab == NULL)
	{
		fprintf(stderr, "SCHERMATA NON INIZIALIZZATA %s", SDL_GetError());
	}

/* CARICO GLI SFONDI */
SFONDO_NODATI = SDL_LoadBMP("/home/pi/FONDO_CLEAN.bmp");
BYPASS_ATTIVO=SDL_LoadBMP("/home/pi/ImgBypass.bmp");


if (SFONDO_NODATI == NULL)
{
	fprintf(stderr, "ERRORE APERTURA FILE BMP", SDL_GetError());
}

int valTmp=0;
/* NASCONDO IL MOUSE*/
SDL_ShowCursor(SDL_DISABLE);

/* INIZIALIZZO I FONT */
TTF_Init();

 
/* FONT PER MESSAGGI SU SCHERMO */
fntmoltopiccolo= TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",14);
fntmedio = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",42);
fntpiccolo = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",32);
fntgrande = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",68);
if (fntpiccolo == NULL)
{
	fprintf(stderr, "ERRORE APERTURA FONT", SDL_GetError());
}
if (fntmedio == NULL)
{
	fprintf(stderr, "ERRORE APERTURA FONT MEDIO", SDL_GetError());
}
if (fntgrande == NULL)
{
	fprintf(stderr, "ERRORE APERTURA FONT GRANDE", SDL_GetError());
}
FondoTesto.r=255;
FondoTesto.g=255;
FondoTesto.b=255;


/* INIZIALIZZO IL CONTROLLER TASTIERA */
Uint8 *keys = SDL_GetKeyState(NULL);


  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

   memset((char *) &si_me, 0, sizeof(si_me));
   si_me.sin_family = AF_INET;
   si_me.sin_port = htons(PORT);
   si_me.sin_addr.s_addr = htonl(INADDR_ANY);
   if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me))==-1)
       diep("bind");


 if (SDL_BlitSurface(SFONDO_NODATI,NULL,Schermata_Tab,NULL) != 0)
     {
        fprintf(stderr, "ERRORE STAMPA FILE BMP", SDL_GetError());
     }
	 
	PosBypass.x = 0;
	PosBypass.y = 300;
	PosBypass.h = 70;
	PosBypass.w = 799;
	
	/*
	ret=pthread_create(&id,NULL,&threadFunction,NULL);
	if(ret==0){
		printf("Thread created successfully.\n");
	}
	 */
   while (active) {
	 
		if (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			active=0;
		}
		if(keys[SDLK_ESCAPE])
			active=0;

		SDL_Flip(Schermata_Tab);    
		
		scrivilog("1");
		int error = 0;
		socklen_t len = sizeof (error);
		int retval = getsockopt (s, SOL_SOCKET, SO_ERROR, &error, &len);
		if (retval != 0) {
			/* there was a problem getting the error code */
			fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
			
			scrivilog("error getting socket error code:");
			scrivilog(strerror(retval));
		}

		if (error != 0) {
			/* socket has a non zero error status */
			fprintf(stderr, "socket error: %s\n", strerror(error));
			
			scrivilog("socket error::");
			scrivilog(strerror(error));
		}
   
   scrivilog("2");
		if (sendto(s, PktStart, 5, 0, (struct sockaddr *)&si_other, slen)==-1)
            diep("sendto()");
	   
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)==-1)
		{
			fprintf(stderr, "ERRORE RECVFROM", SDL_GetError());
		}
		else
		{
			
			scrivilog("3");
			printf("Received packet from %s:%d\nData: %s\n\n", 
            inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
			
			 

				
			
			snprintf(str, sizeof(str), "curlLhttps://www.xxxx.xxx/xxxx/xxxx/ins.php?cmd=%s", buf);
			replacechar(str,' ','_');
			replacechar(str,'(','X');
			strclean(str);
			replacechar(str,'L',' ');
			
			
			
			
			/* 
			LNL -> ALIMENTAZIONE DA ENEL
			
			
 VAC   
 HzAC    
 VOUT     
 HzOUT     
 APPARENT    
 WASSORBITI
 NC
 NC
 VBATT
 ChgA
 BATTPERCENTO
 TMP_INVERTER	
 PVA
 PVV
 NC
 DISCHARGE_A
 STATOCHG
 NC
 NC  
 PV_W
 

227.1    50.0       227.1     50.0      0522         0428           010        361    51.30     019          066 			0040 			05.9    182.3 00.00   00000               		00010110 								00 00 01083 		 010•¢.
ENEL V   ENEL Hz    OUT V     OUT hz     APPARENT    ACTIVE PWR     carico %   bus V  V batt    batt chg A   carica %		TEMP INVERTER	PV A    PV V   SCC	CORRENTE DI SCARICA		b7b6b5b4b3b2b1b0							xx xx  PV W			LOAD %
																																														b2 1 in carica //0 no
																																														b1 carica da pannelli
	*/
	
	if ((buf[0] == 'L') && (buf[1] == 'N') )
	{
		switch (buf[2])
		{
			case 'L':
			{
				
					FlagBypass=1;
				
				
			}break;
			default:
			{
				FlagBypass=0;
			}break;
		}
	}
	
	
	if ((buf[0] == 'P') && (buf[1] == 'K') && (buf[2] == 'T'))
	{
		if (SDL_BlitSurface(SFONDO_NODATI,NULL,Schermata_Tab,NULL) != 0)
		{
			fprintf(stderr, "ERRORE STAMPA FILE BMP", SDL_GetError());
		}
		
		
		
		
		
		
		VALORI = str_split(buf, ' ');
		
		//int contatore=0;
		
		//while(active)
		{
			/*contatore+=1;
			if (contatore > 1000)
				contatore=1;
			
			
			
			char snum1[5];

			sprintf(snum1,"%i",contatore);
				
			PosDest.x = 10;
		PosDest.y = 240;
		PosDest.h = 40;
		PosDest.w = 700;
			
			FondoTestoTMP.r=255;
			FondoTestoTMP.g=0;
			FondoTestoTMP.b=0;
			
			scritta(fntpiccolo,snum1,FondoTestoTMP,&PosDest,Schermata_Tab);
			
			
			*/
			
			
		
		pos=4;
		MiniBuff[0] = buf[pos];
		MiniBuff[1] = '\0';
		pkt = atoi(MiniBuff);
		printf("pkt %i\n",pkt);
		
		
		for (i = 0; i<pkt; i++)
		{
			MiniBuff[i]='.';
		}
		MiniBuff[i]='\0';
		
		
		FondoTestoTMP.r = 255;
		FondoTestoTMP.g=255;
		FondoTestoTMP.b =255;    

		PosDest.x = 10;
		PosDest.y = 440;
		PosDest.h = 40;
		PosDest.w = 700;
		
		scritta(fntpiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);

				
		FondoTestoTMP.r = 255;
		FondoTestoTMP.g=255;
		FondoTestoTMP.b =255;    



		PosDest.x = 700;
		PosDest.y = 40;
		PosDest.h = 20;
		PosDest.w = 100;


		MiniBuff[0]='A';
		MiniBuff[1]='C';
		MiniBuff[2]=' ';
		

		for (i = 0; i<3;i++)//i<pkt; i++)
		{
			MiniBuff[3+i]=VALORI[0][i+6];
		}
		
		MiniBuff[6]=' ';
		MiniBuff[7]='V';
		MiniBuff[8]=' ';
		
		MiniBuff[9]='\0';
		
		scritta(fntmoltopiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		
		
		
		
		
		

		PosDest.x = 700;
		PosDest.y = 60;
		PosDest.h = 20;
		PosDest.w = 100;


		MiniBuff[0]='A';
		MiniBuff[1]='C';
		MiniBuff[2]=' ';
		

		for (i = 0; i<4;i++)//i<pkt; i++)
		{
			MiniBuff[3+i]=VALORI[1][i];
		}
		
		MiniBuff[7]=' ';
		MiniBuff[8]='H';
		MiniBuff[9]='z';
		
		MiniBuff[10]='\0';
		
		scritta(fntmoltopiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		
		
		//OUT
		PosDest.x = 700;
		PosDest.y = 130;
		PosDest.h = 20;
		PosDest.w = 100;


		MiniBuff[0]='O';
		MiniBuff[1]='U';
		MiniBuff[2]='T';
		MiniBuff[3]=' ';
		

		for (i = 0; i<3;i++)//i<pkt; i++)
		{
			MiniBuff[4+i]=VALORI[2][i];
		}
		
		MiniBuff[7]=' ';
		MiniBuff[8]='V';
		
		
		MiniBuff[9]='\0';
		
		scritta(fntmoltopiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		
		
		
		
		 
		

		PosDest.x = 700;
		PosDest.y = 150;
		PosDest.h = 20;
		PosDest.w = 100;


		MiniBuff[0]='O';
		MiniBuff[1]='U';
		MiniBuff[2]='T';
		MiniBuff[3]=' ';
		

		for (i = 0; i<4;i++)//i<pkt; i++)
		{
			MiniBuff[4+i]=VALORI[3][i];
		}
		
		MiniBuff[8]=' ';
		MiniBuff[9]='H';
		MiniBuff[10]='z';
		
		MiniBuff[11]='\0';
		
		scritta(fntmoltopiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		
		//PRODUZIONE PANNELLI
		PosDest.x = 140;
		PosDest.y = 60;
		PosDest.h = 40;
		PosDest.w = 100;


		 
		

		for (i = 0; i<4;i++)//i<pkt; i++)
		{
			MiniBuff[i]=VALORI[19][i+1];
		}
		if (MiniBuff[0]=='0')
			MiniBuff[0]=' ';
		MiniBuff[4]=' ';
		MiniBuff[5]='W';
		MiniBuff[6]='\0';
		
		scritta(fntmedio,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		MiniBuff[4]='\0';
		
		ProduzionePannelli = atoi(MiniBuff);
		
		
		
		//CONSUMO ATTUALE
		PosDest.x = 420;
		PosDest.y = 60;
		PosDest.h = 40;
		PosDest.w = 200;


		for (i = 0; i<4;i++)//i<pkt; i++)
		{
			MiniBuff[i]=VALORI[5][i];
		}
		if (MiniBuff[0]=='0')
			MiniBuff[0]=' ';
		
		MiniBuff[4]=' ';
		MiniBuff[5]='W';
		MiniBuff[6]='\0';
		
		scritta(fntmedio,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		
		MiniBuff[4]='\0';
		
		AssorbimentoCasa = atoi(MiniBuff);
	
		
		//POTENZA RESIDUA
		
		PosDest.x = 270;
		PosDest.y = 220;
		PosDest.h = 40;
		PosDest.w = 200;
		
		if (FlagBypass>0)
		{
			//ASSORBO DALLA RETE, LA SCRITTA E' SEMPRE ROSSA
			valTmp=AssorbimentoCasa;
			char snum[5];

			sprintf(snum,"%d W",valTmp);
				
			MiniBuff[4]=' ';
			
			FondoTestoTMP.r=255;
			FondoTestoTMP.g=0;
			FondoTestoTMP.b=0;
			
			scritta(fntgrande,snum,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		}
		else
		{
			
			//controllo se la produzione è maggiore del consumo
			if (ProduzionePannelli>AssorbimentoCasa)
			{
				valTmp=ProduzionePannelli-AssorbimentoCasa;
				char snum[5];

				sprintf(snum,"%d W" ,valTmp);
				
				
				FondoTestoTMP.r=0;
				FondoTestoTMP.g=255;
				FondoTestoTMP.b=0;
				
				scritta(fntgrande,snum,FondoTestoTMP,&PosDest,Schermata_Tab);
				
				
			}
			else
			{
				valTmp=AssorbimentoCasa-ProduzionePannelli;
				char snum[5];

				// convert 123 to string [buf]
				sprintf(snum,"%d W",valTmp);
				
				FondoTestoTMP.r=255;
				FondoTestoTMP.g=0;
				FondoTestoTMP.b=0;
				
				scritta(fntgrande,snum,FondoTestoTMP,&PosDest,Schermata_Tab);
			}
			
		}
		
		
		
		
		FondoTestoTMP.r=255;
		FondoTestoTMP.g=255;
		FondoTestoTMP.b=255;

		//CARICA BATTERIA
		PosDest.x = 110;
		PosDest.y = 420;
		PosDest.h = 40;
		PosDest.w = 100;


		for (i = 0; i<3;i++)//i<pkt; i++)
		{
			MiniBuff[i]=VALORI[10][i];
		}
		if (MiniBuff[0]=='0')
			MiniBuff[0]=' ';
		
		MiniBuff[3]='%';
		MiniBuff[4]='\0';
		
		scritta(fntpiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		


		//CORRENTE CARICA BATTERIA
		PosDest.x = 350;
		PosDest.y = 420;
		PosDest.h = 40;
		PosDest.w = 100;


		for (i = 0; i<3;i++)//i<pkt; i++)
		{
			MiniBuff[i]='0';
			MiniBuff[i]=VALORI[9][i];
		}
		if (MiniBuff[0]=='0')
			MiniBuff[0]=' ';
		if (MiniBuff[1]=='0')
			MiniBuff[1]=' ';
		
		
		MiniBuff[3]=' ';
		MiniBuff[4]='A';
		MiniBuff[5]='\0';
		
		scritta(fntpiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);
		
		
		
		//CORRENTE SCARICA BATTERIA
		PosDest.x = 630;
		PosDest.y = 420;
		PosDest.h = 40;
		PosDest.w = 100;


		for (i = 0; i<3;i++)//i<pkt; i++)
		{
			MiniBuff[i]='0';
			MiniBuff[i]=VALORI[15][i+2];
		}
		if (MiniBuff[0]=='0')
			MiniBuff[0]=' ';
		if (MiniBuff[1]=='0')
			MiniBuff[1]=' ';
		
		
		MiniBuff[3]=' ';
		MiniBuff[4]='A';
		MiniBuff[5]='\0';
		
		scritta(fntpiccolo,MiniBuff,FondoTestoTMP,&PosDest,Schermata_Tab);





 


		if (FlagBypass>0)
		{
			
			if (SDL_BlitSurface(BYPASS_ATTIVO,NULL,Schermata_Tab,&PosBypass) != 0)
			{
				fprintf(stderr, "ERRORE STAMPA FILE BMP", SDL_GetError());
			}
		}


//SDL_Delay(100);
		
		}
		
		if (VALORI)
		{
			int i;
			for (i = 1; *(VALORI + i); i++)
			{
				printf("VALORE %i =[%s]\n",i, *(VALORI + i));
				free(*(VALORI + i));
			}
			printf("\n");
			free(VALORI);
		}
		else
			free(VALORI);
		
		 
		
		

		//fprintf(stderr, "FINE STRINGA");

		//SDL_FreeSurface(SCRITTE);  
		
		
		/*
		
		
		pos++;
		MiniBuff[0] = TMP[pos];
		pos++;
		MiniBuff[1] = TMP[pos];
		MiniBuff[2] = '\0';
		VAC = atoi(MiniBuff);
	 */
	 
	 scrivilog("4");
	  
	 
	 if(curl) 
	 {
		
		if (Pronto<10)
			Pronto++;
		else
		{
			printf("TEST->%s\n\n",str);
			system(str);
			Pronto=0;
		}

		
	/*	if(response != CURLE_OK) {
			fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(response));
		} else {
			printf("%u\n",response);
//sprintf(rstr,"%u\n",response);
			
		}*/
		
	}
 
	 
	}
	
	}
		
   }
   
   printf("USCITA\n");
   
   TTF_CloseFont(fntmoltopiccolo);
   TTF_CloseFont(fntpiccolo);
   TTF_CloseFont(fntmedio);
   TTF_CloseFont(fntgrande);

SDL_Quit();
TTF_Quit();
   

   close(s);
   return 0;
}
