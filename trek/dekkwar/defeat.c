/**********************************************************/
/*** defeat.c                                           ***/
/*** Kommentar nach V.1.00                              ***/
/*** 11.02.03: Einbau Meldung HW Warnung und Schuss     ***/
/*** 12.02.03: Nachricht an alle eigenen Schiffe bei    ***/
/***           Annaeherung eines Feindes an die eig. HW ***/
/*** 14.02.03: String in send_msg auf 33 begrenzt       ***/
/*** 18.02.03: TMusleep weg wg. Linux, siehe dwmain.c   ***/
/*** 25.02.03: neue Variable xmsg in send_msg, da der   ***/
/***           String new_msg u.U. geaendert wird       ***/
/*** Kommentar nach V1.0.1                              ***/
/*** 04.04.03: Falsches if bei HW geaendert: Statt      ***/
/***           m_p->l_homeworld[FEDERAL] == ACTIVE nun ***/
/***           m_p->l_homeworld[IMPERIAL] == EXISTING   ***/
/***           und umgekehrt. Frueher feuerten zerst.HW ***/
/**********************************************************/
/*** 29.04.03: Version 1.1.0                            ***/
/**********************************************************/
/*** 27.04.04: Umstellung auf strncpy in sort_msg und   ***/
/***           send_msg                                 ***/
/*** 07.05.04: Umstellung struct fort                   ***/
/*** 29.05.04: explosion neu als Fork                   ***/
/*** 30.06.04: MSG_LENGTH neu in dw_def.h               ***/
/*** 30.06.04: Treffer auf Sektion 4 fuer Zerstoerer    ***/
/***           korrigiert; -> Sektion 5                 ***/
/*** 30.06.04: libdw weg                                ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 22.10.04: in Textausgabe " on x y" durch "at x y " ***/
/***           ersetzt.                                 ***/
/*** 24.07.07: recon_str mit Ort und Zeit eingefuhert   ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 28.07.07: Central Control-Text entfernt            ***/
/*** 29.07.07: CRTL-C-Handling, survey_area,map         ***/
/*** 04.08.07: Zerstoerung Schiff in recon_str          ***/
/*** 04.08.07: Uebersichtskarte m_p->map                ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 23.01.08: Umstellung struct satellite              ***/
/*** 24.01.08: Umstellung struct planet                 ***/
/*** 25.01.08: Umstellung struct base                   ***/
/*** 06.02.08: Anpassungen auf neues Basen-Docking und  ***/
/***           neue Elemente struct base                ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 17.02.08: Radarlee fuer Basen, Forts, Satelliten   ***/
/*** 22.02.08: Sprungtor-Funktion                       ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 25.02.08: Sprungtore in DISPLAY_MAP integriert     ***/
/*** 26.02.08: Symbol fuer DOCKING entfernt             ***/
/*** 27.02.08: struct scout eingefuehrt                 ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 05.03.08: Fehler bei Homeworld-Warnung Foed. gefixt***/
/*** 11.03.08: Scout wird jetzt beschossen, wenn Tarnung***/
/***           weg.                                     ***/
/*** 12.03.08: Missile neu                              ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 13.03.08: Fehler bei Abfrage Homeworld<->Scout und ***/
/***           bei Homeworld<->Missile beseitigt        ***/
/*** 13.03.08: Umstellung auf struct homeworld          ***/
/*** 15.03.08: HOOD_ON/OFF, RLEE_ON/OFF, BEAM_ON/OFF,   ***/
/***           RAMM_ON/OFF durch ON/OFF ersetzt,        ***/
/***           DEFEAT_ON/OFF weg. N_/EXISTING durch     ***/
/***           NOT_/ACTIVE ersetzt                     ***/
/*** 24.03.08: SH_DOWN/UP durch ON/OFF ersetzt          ***/
/*** 26.03.08: Umstellung auf struct ship               ***/
/*** 26.03.08  condition auch fuer Heimatwelten         ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 31.03.08: Format %4.1f durch %06.1f ersetzt        ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/**********************************************************/
/*** 04.05.08: Fehler bei score scout/missile beseitigt ***/
/*** 14.10.08: Anpassungen fuer Ubuntu 8.04 (includes)  ***/
/*** 23.10.08: Anpassungen -Wall bei gcc (Ubuntu 8.04)  ***/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 21.11.09: p_party, zaehler waren nicht deklariert, ***/
/***           str1 auf MSG_LENGTH, f_zaehler war de-   ***/
/***           klariert, aber nicht genutzt             ***/
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 01.01.10: Format sprintf fuer recon_str geaendert  ***/
/*** 02.01.10: Anzeige Forts, die DESTROYED, aber im    ***/
/***           Spielfeld sind                           ***/
/*** 03.01.10: Survey-Array Sat: Nur wenn nicht NEUTRAL ***/
/***           und Position nicht -99/-99               ***/                  
/*** 03.10.10: Sat-Umgebung checken: Bedingungen erwei- ***/
/***           tert: nur wenn Sat nicht NEUTRAL und im  ***/
/***           Spielfeld                                ***/
/*** 10.01.10: seed() neu; ran() mit seed ueberarbeitet ***/
/*** 13.01.10: Zwei Formatfehler bei printf beseitigt   ***/
/*** 16.01.10: usleep bei Romulaner-Expl. auf EXPL_TIME ***/
/*** 17.01.10: Kein Eliminieren des Rom. mehr; das er-  ***/
/***           folgt durch den Rom. selbst              ***/
/*** 23.01.10: Scoring bei Rom.-Zerst. fehlte           ***/
/*** 24.01.10: Robotship: Elimination und Explosion in  ***/
/***           robot.c verlagert                        ***/
/*** 26.01.10: Fehler bei Missile-Meldung korrigiert    ***/
/*** 20.02.10: DEF_DELAY neu, usleep statt pause        ***/
/*** 27.02.10: etliche printf-Ausgaben entfernt         ***/
/*** 24.04.10: neue Konstantendefinitionen aus dw_def.h ***/
/*** 25.04.10: IMAX,IMIN,IABS ueberall in () gesetzt    ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>

/*************************/
/*** Includes DECWAR   ***/
/*************************/
#include "mathfkt.h"     /* math. Funktionen (incl. <math.h> */
#include "dw_def.h"
#include "dw_shm.h"
#include "ships.h"

extern void sig_hnd();
extern void sig_hnd_ctrlc();
extern key_t qgetkey();
extern void delay();
extern float ran();
extern int seed();
extern void path();
extern void send_msg();
extern void hit_def();
extern int wegfrei();
extern void gauss2();
extern float star_date();
extern int behind();

int main()
/***************************************************************************/
/*** DEFEAT: Basenverteidigung, Fortvert., Planetenvert., Satellitenmeld. **/
/***************************************************************************/
{

/***************************************************************************/
/***                    lokale Variablen                                 ***/
/***************************************************************************/
int i,j,k,hit_energy,zaehler,xstart,ystart;
int ii,jj,xl,xr,yu,yo;
int xmap,ymap;
int xziel,yziel;
int warn_cnt[MAX_USER];
int p_party,f_party,b_party;
char str1[MSG_LENGTH];
char aa[ARRAY_SIZE][ARRAY_SIZE];
char dmap[2][MAP_SIZE][MAP_SIZE];
float stdt,mapfac;
long t1;

/****************************************************************************/
/*** sig_hnd setzen fuer SIGKILL, SIGALRM und SIGINT                      ***/
/****************************************************************************/
signal(SIGALRM,sig_hnd);
signal(SIGKILL,SIG_DFL);
signal(SIGINT,sig_hnd_ctrlc);

/****************************************************************************/
/***                         Shared Memory einbinden                      ***/
/****************************************************************************/
shmid = shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Error: SHM nicht einzubinden, shmid=%d m_p->shm_id=%d",shmid,m_p->shm_id);
   exit(1); 
   }
if (shmid != m_p->shm_id)
   {
   printf("Error: ID falsch: shmid=%d m_p->shm_id=%d",shmid,m_p->shm_id);
   exit(1);
   }

/*************************************************************************/
/*** Startpunkt mit Wartezustand                                       ***/
/*************************************************************************/
for (j=0;j<MAX_USER;j++) warn_cnt[j]=0;
zaehler = 0;
mapfac=( (float) ARRAY_SIZE / (float) MAP_SIZE );
alarm(5);
start:
/*********************/
/* eigentl. Programm */
/*********************/
printf(" DEFEAT: action!\n");

/* survey_area: Spielfeld vorbesetzen */
for (i=0; i<ARRAY_SIZE; i++)
  {
  for (j=0; j<ARRAY_SIZE; j++)
    {
    aa[i][j]=' ';
    }
  }
for (i=0; i<MAP_SIZE; i++)
  {
  for (j=0; j<MAP_SIZE; j++)
    {
    dmap[FEDERAL][i][j]=' ';
    dmap[IMPERIAL][i][j]=' ';
    }
  }

/* survey_area: HW-Umgebung */
 if (m_p->homeworld[IMPERIAL].status == ACTIVE)
   {
   xl=(IMAX(0, (m_p->homeworld[IMPERIAL].pos[0]-HOMEWORLD_RANGE)));
   xr=(IMIN((ARRAY_SIZE-1), (m_p->homeworld[IMPERIAL].pos[0]+HOMEWORLD_RANGE)));
   yu=(IMAX(0, (m_p->homeworld[IMPERIAL].pos[1]-HOMEWORLD_RANGE)));
   yo=(IMIN((ARRAY_SIZE-1), (m_p->homeworld[IMPERIAL].pos[1]+HOMEWORLD_RANGE)));
   xmap=(NINT( (float) m_p->homeworld[IMPERIAL].pos[0] / mapfac ));
   ymap=(NINT( (float) m_p->homeworld[IMPERIAL].pos[1] / mapfac ));
   dmap[IMPERIAL][xmap][ymap]='H';
   for (i=xl;i<=xr;i++)
     {
       for (j=yu;j<=yo;j++)
         {
	 aa[i][j]='E';
         }
     }
   }
 if (m_p->homeworld[FEDERAL].status == ACTIVE)
   {
   xl=(IMAX(0, (m_p->homeworld[FEDERAL].pos[0]-HOMEWORLD_RANGE)));
   xr=(IMIN((ARRAY_SIZE-1), (m_p->homeworld[FEDERAL].pos[0]+HOMEWORLD_RANGE)));
   yu=(IMAX(0, (m_p->homeworld[FEDERAL].pos[1]-HOMEWORLD_RANGE)));
   yo=(IMIN((ARRAY_SIZE-1), (m_p->homeworld[FEDERAL].pos[1]+HOMEWORLD_RANGE)));
   xmap=(NINT( (float) m_p->homeworld[FEDERAL].pos[0] / mapfac ));
   ymap=(NINT( (float) m_p->homeworld[FEDERAL].pos[1] / mapfac ));
   dmap[FEDERAL][xmap][ymap]='H';
   for (i=xl;i<=xr;i++)
     {
       for (j=yu;j<=yo;j++)
         {
	 if (aa[i][j]=='E')
	   {
	   aa[i][j]='X';
           }
         else
           {
	   aa[i][j]='F';
           }
         }
     }
   }

/* survey_array: Satelliten */
for (i=0;i<MAX_SATS;i++)
  {
  if (m_p->satellite[i].party!=NEUTRAL && m_p->satellite[i].pos[0] != -99 && m_p->satellite[i].pos[1] != -99)
    {
    xl=(IMAX(0, (m_p->satellite[i].pos[0]-SAT_RANGE)));
    xr=(IMIN((ARRAY_SIZE-1), (m_p->satellite[i].pos[0]+SAT_RANGE)));
    yu=(IMAX(0, (m_p->satellite[i].pos[1]-SAT_RANGE)));
    yo=(IMIN((ARRAY_SIZE-1), (m_p->satellite[i].pos[1]+SAT_RANGE)));
    xmap=(NINT( (float) m_p->satellite[i].pos[0] / mapfac ));
    ymap=(NINT( (float) m_p->satellite[i].pos[1] / mapfac ));
    if (m_p->satellite[i].party==FEDERAL)
      {
      if (dmap[FEDERAL][xmap][ymap] == ' ') dmap[FEDERAL][xmap][ymap]='S';
      for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='E') aa[j][k]='X';
         else if (aa[j][k]==' ') aa[j][k]='F';
         }
       }
      }
    else if (m_p->satellite[i].party==IMPERIAL)
      {
      if (dmap[IMPERIAL][xmap][ymap] == ' ') dmap[IMPERIAL][xmap][ymap]='S';
      for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='F') aa[j][k]='X';
         else if (aa[j][k]==' ') aa[j][k]='E';
         }
       }
      }
    }
  }

/* survey_array: Basen */
for (i=0;i<MAX_BASES;i++)
  {
  xl=(IMAX(0, (m_p->base[i].pos[0]-BASE_RANGE)));
  xr=(IMIN((ARRAY_SIZE-1), (m_p->base[i].pos[0]+BASE_RANGE)));
  yu=(IMAX(0, (m_p->base[i].pos[1]-BASE_RANGE)));
  yo=(IMIN((ARRAY_SIZE-1), (m_p->base[i].pos[1]+BASE_RANGE)));
  xmap=(NINT( (float) m_p->base[i].pos[0] / mapfac ));
  ymap=(NINT( (float) m_p->base[i].pos[1] / mapfac ));
  if (m_p->base[i].party==FEDERAL)
    {
    if (dmap[FEDERAL][xmap][ymap] != 'H') dmap[FEDERAL][xmap][ymap]='B';
    for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='E')
	   {
	   aa[j][k]='X';
           }
         else if (aa[j][k]==' ')
           {
	   aa[j][k]='F';
           }
         }
       }
    }
  else
    {
    if (dmap[IMPERIAL][xmap][ymap] != 'H') dmap[IMPERIAL][xmap][ymap]='B';
    for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='F')
	   {
	   aa[j][k]='X';
           }
         else if (aa[j][k]==' ')
           {
	   aa[j][k]='E';
           }
         }
       }
    }
  }

/* survey_array: Planeten */
for (i=0;i<MAX_PLANETS;i++)
  {
  xl=(IMAX(0, (m_p->planet[i].pos[0]-(PL_PH_RANGE*m_p->planet[i].level))));
  xr=(IMIN((ARRAY_SIZE-1), (m_p->planet[i].pos[0]+(PL_PH_RANGE*m_p->planet[i].level))));
  yu=(IMAX(0, (m_p->planet[i].pos[1]-(PL_PH_RANGE*m_p->planet[i].level))));
  yo=(IMIN((ARRAY_SIZE-1), (m_p->planet[i].pos[1]+(PL_PH_RANGE*m_p->planet[i].level))));
  xmap=(NINT( (float) m_p->planet[i].pos[0] / mapfac ));
  ymap=(NINT( (float) m_p->planet[i].pos[1] / mapfac ));
  if (m_p->planet[i].party==FEDERAL)
    {
    if (dmap[FEDERAL][xmap][ymap] != 'H' && dmap[FEDERAL][xmap][ymap] != 'B') dmap[FEDERAL][xmap][ymap]='P';
    for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='E')
	   {
	   aa[j][k]='X';
           }
         else if (aa[j][k]==' ')
           {
	   aa[j][k]='F';
           }
         }
       }
    }
  else if (m_p->planet[i].party==IMPERIAL) /* es gibt auch neutrale Pl. */
    {
    if (dmap[IMPERIAL][xmap][ymap] != 'H' && dmap[IMPERIAL][xmap][ymap] != 'B') dmap[IMPERIAL][xmap][ymap]='P';
    for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='F')
	   {
	   aa[j][k]='X';
           }
         else if (aa[j][k]==' ')
           {
	   aa[j][k]='E';
           }
         }
       }
    }
  }

/* survey_array: Forts */
for (i=0;i<MAX_FORTS;i++)
  {
  xl=(IMAX(0, (m_p->fort[i].pos[0]-FORT_RANGE)));
  xr=(IMIN((ARRAY_SIZE-1), (m_p->fort[i].pos[0]+FORT_RANGE)));
  yu=(IMAX(0, (m_p->fort[i].pos[1]-FORT_RANGE)));
  yo=(IMIN((ARRAY_SIZE-1), (m_p->fort[i].pos[1]+FORT_RANGE)));
  xmap=(NINT( (float) m_p->fort[i].pos[0] / mapfac ));
  ymap=(NINT( (float) m_p->fort[i].pos[1] / mapfac ));
  if (m_p->fort[i].party==FEDERAL && m_p->fort[i].status==ACTIVE)
    {
    if (dmap[FEDERAL][xmap][ymap] != 'H' && dmap[FEDERAL][xmap][ymap] != 'B' && dmap[FEDERAL][xmap][ymap] != 'P') dmap[FEDERAL][xmap][ymap]='F';
    for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='E')
	   {
	   aa[j][k]='X';
           }
         else if (aa[j][k]==' ')
           {
	   aa[j][k]='F';
           }
         }
       }
    }
  else if (m_p->fort[i].party==IMPERIAL && m_p->fort[i].status==ACTIVE)
    {
    if (dmap[IMPERIAL][xmap][ymap] != 'H' && dmap[IMPERIAL][xmap][ymap] != 'B' && dmap[IMPERIAL][xmap][ymap] != 'P') dmap[IMPERIAL][xmap][ymap]='F';
    for (j=xl;j<=xr;j++)
       {
       for (k=yu;k<=yo;k++)
         {
	 if (aa[j][k]=='F')
	   {
	   aa[j][k]='X';
           }
         else if (aa[j][k]==' ')
           {
	   aa[j][k]='E';
           }
         }
       }
    }
  }

/* survey_array: Jumpgates: kein Scanbereich, nur Anwesenheit! */
for (i=0;i<MAX_JUMPGATES;i++)
  {
  xmap=NINT( (float) m_p->jumpgate[i].pos[0] / mapfac );
  ymap=NINT( (float) m_p->jumpgate[i].pos[1] / mapfac );
  /* ueberschreibt Basen, Planeten, Satelliten, Forts ! */
  if (m_p->jumpgate[i].party==FEDERAL) dmap[FEDERAL][xmap][ymap]='J';
  if (m_p->jumpgate[i].party==IMPERIAL) dmap[IMPERIAL][xmap][ymap]='J';
  }


/* survey_array: Zaehlung der Felder: F=Fed., E=Emp., X=Ueberlappung (beide) */
ii=jj=0;
for (i=0; i<ARRAY_SIZE; i++)
  {
  for (j=0; j<ARRAY_SIZE; j++)
    {
    if (aa[i][j]=='F' || aa[i][j]=='X') ii++;
    if (aa[i][j]=='E' || aa[i][j]=='X') jj++;
    }
  }
m_p->isurvey_area[FEDERAL]=ii;
m_p->isurvey_area[IMPERIAL]=jj;
m_p->fsurvey_area[FEDERAL]=100.0 * ((float) ii / (float) (ARRAY_SIZE*ARRAY_SIZE));
m_p->fsurvey_area[IMPERIAL]=100.0 * ((float) jj / (float) (ARRAY_SIZE*ARRAY_SIZE));

for (j=0; j<MAP_SIZE; j++)
  {
  k=MAP_SIZE-1-j;
  for (i=0; i<MAP_SIZE; i++)
    {
    m_p->map[0][i][k]=dmap[0][i][j];
    m_p->map[1][i][k]=dmap[1][i][j];
    }
  }

/* Durchlauf Verteidigung */
t1=time(0);
stdt=star_date(t1);

if (m_p->homeworld[IMPERIAL].status == ACTIVE && m_p->a[m_p->homeworld[IMPERIAL].pos[0]][m_p->homeworld[IMPERIAL].pos[1]] != E_HOMEWORLD)
  {
  printf("ERROR HW (Imp.) is being corrected!\n");
  m_p->a[m_p->homeworld[IMPERIAL].pos[0]][m_p->homeworld[IMPERIAL].pos[1]]=E_HOMEWORLD;
  }
if (m_p->homeworld[FEDERAL].status == ACTIVE && m_p->a[m_p->homeworld[FEDERAL].pos[0]][m_p->homeworld[FEDERAL].pos[1]] != F_HOMEWORLD)
  {
  printf("ERROR HW (Fed.) is being corrected!\n");
  m_p->a[m_p->homeworld[FEDERAL].pos[0]][m_p->homeworld[FEDERAL].pos[1]]=F_HOMEWORLD;
  }

/* Check Forts */
for (i=0;i<MAX_FORTS;i++)
    {
      //    printf("Nr. %d Status=%d party=%d Pos: %d %d\n",i,m_p->fort[i].status,m_p->fort[i].party,m_p->fort[i].pos[0],m_p->fort[i].pos[1]);
    if (m_p->fort[i].status == ACTIVE)
       {
       if (m_p->a[m_p->fort[i].pos[0]][m_p->fort[i].pos[1]] == SPACE)        
          {
          printf("ERROR: Fort auf %d %d unsichtbar; korrigiert!\n",m_p->fort[i].pos[0],m_p->fort[i].pos[1]);
          if (m_p->fort[i].party == IMPERIAL) m_p->a[m_p->fort[i].pos[0]][m_p->fort[i].pos[1]] = E_FORT;
          if (m_p->fort[i].party == FEDERAL) m_p->a[m_p->fort[i].pos[0]][m_p->fort[i].pos[1]] = F_FORT;
          }
       }
    else if (m_p->fort[i].status != CLAIMED)
       {
       if (m_p->fort[i].pos[0] >= 0 && m_p->fort[i].pos[0]< ARRAY_SIZE)
         {
	 printf("ERROR: Fort nicht ACTIVE aber Position im Feld: Nr. %d!",i);
         }
       }    
    }

/* Schleife ueber alle User */
if (m_p->anz_user > 0 )  /* Nur wenn ueberhaupt jemand im Spiel ist */
   {
   for (j=0;j<MAX_USER;j++)
       {
	 if (strlen(m_p->recon_str[j]) < 5)
	   {
	   printf("recon-str ZU KLEIN: %d %s\n",j,m_p->recon_str[j]);
           }
       /**********/
       /* Scouts */
       /**********/
       if ( party[j] == IMPERIAL  
                     &&
          m_p->scout[FEDERAL].status == ACTIVE
                     &&
          m_p->luser[j] == ACTIVE
                     &&
          m_p->ship[j].dock == NOT_DOCKED
                     &&
          m_p->ship[j].hood == OFF)
	 {
         if (abs(m_p->ship[j].pos[0] - m_p->scout[FEDERAL].pos[0]) <= SAT_RANGE
                &&
             abs(m_p->ship[j].pos[1] - m_p->scout[FEDERAL].pos[1]) <= SAT_RANGE)
             {
	     if (m_p->radarlee == OFF)
               {  
	       sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, scout)",
                       ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
               }
             else
               {
	       if (behind(m_p->scout[FEDERAL].pos[0],m_p->scout[FEDERAL].pos[1],m_p->ship[j].pos[0],m_p->ship[j].pos[1],SHIP_RANGE)==0) 	       
                  {
                  sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, scout)",
                          ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
                  }
	       }
             }
         }
       if ( party[j] == FEDERAL
                     &&
          m_p->scout[IMPERIAL].status == ACTIVE
                     &&
          m_p->luser[j] == ACTIVE
                     &&
          m_p->ship[j].dock == NOT_DOCKED
                     &&
          m_p->ship[j].hood == OFF)
	 {
         if (abs(m_p->ship[j].pos[0] - m_p->scout[IMPERIAL].pos[0]) <= SAT_RANGE
                &&
             abs(m_p->ship[j].pos[1] - m_p->scout[IMPERIAL].pos[1]) <= SAT_RANGE)
             {
	     if (m_p->radarlee == OFF)
               {  
	       sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, scout)",
                       ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
               }
             else
               {
	       if (behind(m_p->scout[IMPERIAL].pos[0],m_p->scout[IMPERIAL].pos[1],m_p->ship[j].pos[0],m_p->ship[j].pos[1],SHIP_RANGE)==0) 	       
                  {
                  sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, scout)",
                          ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
                  }
	       }
             }
         }
       /****************/
       /* Heimatwelten */
       /****************/
       if ( party[j] == IMPERIAL   /* feuert nicht auf den Romulaner! */
                     &&
          m_p->homeworld[FEDERAL].status == ACTIVE
                     &&
          m_p->luser[j] == ACTIVE
                     &&
          m_p->ship[j].dock == NOT_DOCKED
                     &&
          m_p->ship[j].hood == OFF)
	 {
         if (abs(m_p->ship[j].pos[0] - m_p->homeworld[FEDERAL].pos[0]) <= HOMEWORLD_WRANGE
                &&
             abs(m_p->ship[j].pos[1] - m_p->homeworld[FEDERAL].pos[1]) <= HOMEWORLD_WRANGE)
             {
	     sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, HW)",ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
             send_msg(j,"  Federation to Imperial ship:");           
             send_msg(j,"             WARNING:");
             send_msg(j," YOU ENTERED A RESTRICTED AREA!");           
             send_msg(j,"     IF YOU VALUE YOUR LIFE");           
             send_msg(j,"       BE SOMEWHERE ELSE! ");           
             warn_cnt[j]++;
             /* Nachricht an alle eigenen Schiffe (Foederation) */
             for (i=1;i<MAX_USER;i++)
                {
	        if (m_p->luser[i] == ACTIVE && party[i] == FEDERAL) 
	          {
                  send_msg(i,"Imperial ship detected");
                  send_msg(i,"close to our homeworld");
	          }
                }
	     }
         else
   	    {
            warn_cnt[j]=0;
            }
         if (abs(m_p->ship[j].pos[0] - m_p->homeworld[FEDERAL].pos[0]) <= HOMEWORLD_RANGE
                &&
             abs(m_p->ship[j].pos[1] - m_p->homeworld[FEDERAL].pos[1]) <= HOMEWORLD_RANGE
                &&
             warn_cnt[j] > 1) /* 2 Warnungen vor dem ersten Schuss! */
             {
             xstart=m_p->homeworld[FEDERAL].pos[0]; 
             ystart=m_p->homeworld[FEDERAL].pos[1];
             xziel=m_p->ship[j].pos[0];
             yziel=m_p->ship[j].pos[1];
             /* Freie Schussbahn? */
             if (wegfrei(xstart,ystart,xziel,yziel)==0)
                {
                hit_energy=PH_HOMEWORLD;
                gauss2(&hit_energy); 
                hit_energy=hit_energy+300;
                send_msg(j,">>ATTACK by Federal HOMEWORLD");
                sprintf(str1,"We got a %d units hit!",hit_energy);
                send_msg(j,str1);
                hit_def(FEDERAL,j,hit_energy);
                }
	     }
        }
       else if ( party[j] == FEDERAL
                     &&
          m_p->homeworld[IMPERIAL].status == ACTIVE
                     &&
          m_p->luser[j] == ACTIVE
                     &&
          m_p->ship[j].dock == NOT_DOCKED
                     &&
          m_p->ship[j].hood == OFF)
	 {
         if (abs(m_p->ship[j].pos[0]-m_p->homeworld[IMPERIAL].pos[0])<=HOMEWORLD_WRANGE
                                 &&
             abs(m_p->ship[j].pos[1]-m_p->homeworld[IMPERIAL].pos[1])<=HOMEWORLD_WRANGE)
             {
	     sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, HW)",ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
             send_msg(j,"      Empire to Federal ship:");           
             send_msg(j,"              WARNING:");
             send_msg(j," YOU ENTERED A RESTRICTED AREA!");           
             send_msg(j,"     IF YOU VALUE YOUR LIFE");           
             send_msg(j,"       BE SOMEWHERE ELSE! ");           
             warn_cnt[j]++;
             for (i=1;i<MAX_USER;i++)
                {
	        if (m_p->luser[i] == ACTIVE && party[i] == IMPERIAL) 
		  {
                  send_msg(i,"Federal ship detected");
                  send_msg(i,"close to our homeworld");
	          }
                }
            }
         else
	   {
           warn_cnt[j]=0;
           }
         if (abs(m_p->ship[j].pos[0]-m_p->homeworld[IMPERIAL].pos[0])<=HOMEWORLD_RANGE
                &&
             abs(m_p->ship[j].pos[1]-m_p->homeworld[IMPERIAL].pos[1])<=HOMEWORLD_RANGE
                &&
             warn_cnt[j] > 1) /* eine Warnung vor dem ersten Schuss! */
             {
             xstart=m_p->homeworld[IMPERIAL].pos[0]; 
             ystart=m_p->homeworld[IMPERIAL].pos[1];
             xziel=m_p->ship[j].pos[0];
             yziel=m_p->ship[j].pos[1];
             /* Freie Schussbahn? */
             if (wegfrei(xstart,ystart,xziel,yziel)==0)
                {
                hit_energy=PH_HOMEWORLD;
                gauss2(&hit_energy); 
                hit_energy=hit_energy+300;
                send_msg(j,">>ATTACK by Imperial HOMEWORLD");
                sprintf(str1,"We got a %d units hit!",hit_energy);
                send_msg(j,str1);
                hit_def(IMPERIAL,j,hit_energy);
                }
	     }
        }
       /*********/
       /* Basen */
       /*********/
       for (i=0;i<MAX_BASES;i++)
           {
           if (m_p->luser[j] == ACTIVE
                       &&
               m_p->ship[j].dock == NOT_DOCKED
                       &&
               m_p->ship[j].hood == OFF
                       &&
               m_p->base[i].party != party[j]
               /* d.h. Rom. wird an beide Seiten gemeldet!! */
                       &&
               m_p->base[i].status == ACTIVE
                       &&
               (abs((m_p->ship[j].pos[0]-m_p->base[i].pos[0])))<=BASE_RANGE 
                       &&
               (abs((m_p->ship[j].pos[1]-m_p->base[i].pos[1])))<=BASE_RANGE )
             {              
	     if ((m_p->radarlee == OFF) || 
                (m_p->radarlee == ON && 
                 behind(m_p->base[i].pos[0],m_p->base[i].pos[1],
		 m_p->ship[j].pos[0],m_p->ship[j].pos[1],BASE_RANGE)==0))
 	         sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, star base)"
                         ,ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
             xstart=m_p->base[i].pos[0]; 
             ystart=m_p->base[i].pos[1];
             xziel=m_p->ship[j].pos[0];
             yziel=m_p->ship[j].pos[1];
             /* Freie Schussbahn? */
             if (wegfrei(xstart,ystart,xziel,yziel)==0)
                {
                b_party=m_p->base[i].party;
                hit_energy=PH_BASE;
                gauss2(&hit_energy); 
                send_msg(j,">> ATTACK by HOSTILE BASE <<");
                sprintf(str1,"We got a %d units hit!",hit_energy);
                send_msg(j,str1);
                hit_def(b_party,j,hit_energy);
                }
             } /* if in Reichweite */
           } /* i Basen */
       /************/
       /* Planeten */
       /************/
       for (i=0;i<MAX_PLANETS;i++)
           {
           if (m_p->luser[j] == ACTIVE
                       &&
               m_p->ship[j].dock == NOT_DOCKED
                       &&
               m_p->ship[j].hood == OFF
                       &&
               m_p->planet[i].party != party[j]
                       &&
               m_p->planet[i].party != NEUTRAL
                       &&
               m_p->planet[i].status != ACTIVE
                       &&
               (abs((m_p->ship[j].pos[0]-m_p->planet[i].pos[0])))<=(PL_PH_RANGE*m_p->planet[i].level)
                       &&
               (abs((m_p->ship[j].pos[1]-m_p->planet[i].pos[1])))<=(PL_PH_RANGE*m_p->planet[i].level))
             {              
	     if ((m_p->radarlee == OFF) || 
                (m_p->radarlee == ON && 
                 behind(m_p->planet[i].pos[0],m_p->planet[i].pos[1],
		 m_p->ship[j].pos[0],m_p->ship[j].pos[1],PL_PH_RANGE*m_p->planet[i].level)==0))
         	 sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, star base)"
                         ,ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
             /* o.k. Ziel in Reichweite */   
             xstart=m_p->planet[i].pos[0]; 
             ystart=m_p->planet[i].pos[1];
             xziel=m_p->ship[j].pos[0];
             yziel=m_p->ship[j].pos[1];
             /* Freie Schussbahn? */
             if (wegfrei(xstart,ystart,xziel,yziel)==0)
                {
                p_party=m_p->planet[i].party;
                hit_energy=PH_PLANET*m_p->planet[i].level;
                gauss2(&hit_energy);       
                send_msg(j,">> ATTACK by HOSTILE PLANET <<");
                sprintf(str1,"We got a %d units hit!",hit_energy);
                send_msg(j,str1);
                hit_def(p_party,j,hit_energy);
                }
             } /* if in Reichweite */
           } /* i Planeten */


       /************/
       /*   Forts  */
       /************/
       for (i=0;i<MAX_FORTS;i++)
           {
           if (m_p->luser[j] == ACTIVE
                       &&
               m_p->ship[j].dock == NOT_DOCKED
                       &&
               m_p->ship[j].hood == OFF
                       &&
               m_p->fort[i].party != party[j]
                       &&
               m_p->fort[i].status == ACTIVE
                       &&
               (abs((m_p->ship[j].pos[0]-m_p->fort[i].pos[0])))<=FORT_RANGE
                       &&
               (abs((m_p->ship[j].pos[1]-m_p->fort[i].pos[1])))<=FORT_RANGE)
             {              
             /* o.k. Ziel in Reichweite */
	     if ((m_p->radarlee == OFF) || 
                (m_p->radarlee == ON && 
                 behind(m_p->fort[i].pos[0],m_p->fort[i].pos[1],
		 m_p->ship[j].pos[0],m_p->ship[j].pos[1],FORT_RANGE) == 0) )
                 sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, fort)",
                 ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
             xstart=m_p->fort[i].pos[0]; 
             ystart=m_p->fort[i].pos[1];
             xziel=m_p->ship[j].pos[0];
             yziel=m_p->ship[j].pos[1];
             /* Freie Schussbahn? */
             if (wegfrei(xstart,ystart,xziel,yziel)==0)
                {
                f_party=m_p->fort[i].party;
                hit_energy=PH_FORT;
                gauss2(&hit_energy);       
                send_msg(j,">> ATTACK by HOSTILE FORT <<");
                sprintf(str1,"We got a %d units hit!",hit_energy);
                send_msg(j,str1);
                hit_def(f_party,j,hit_energy);
                }
             } /* if in Reichweite */
           } /* i Forts */
        /**************/
        /* Satelliten */
        /**************/
        for (i=0;i<MAX_SATS;i++)
            {
            if (m_p->luser[j] == ACTIVE
                       &&
                m_p->ship[j].dock == NOT_DOCKED
                       &&
                m_p->ship[j].hood == OFF
                       &&
                m_p->satellite[i].party != party[j]
                       &&
                m_p->satellite[i].party != NEUTRAL
                       &&
                m_p->satellite[i].pos[0] >= 0
                       &&
                m_p->satellite[i].pos[0] < ARRAY_SIZE
                       &&
                m_p->satellite[i].pos[1] >= 0
                       &&
                m_p->satellite[i].pos[1] < ARRAY_SIZE
                       &&
              (abs((m_p->ship[j].pos[0]-m_p->satellite[i].pos[0])))<=SAT_RANGE
                       &&
              (abs((m_p->ship[j].pos[1]-m_p->satellite[i].pos[1])))<=SAT_RANGE)
               {       
	       if ((m_p->radarlee == OFF) || 
                 (m_p->radarlee == ON && 
                 behind(m_p->satellite[i].pos[0],m_p->satellite[i].pos[1],
		 m_p->ship[j].pos[0],m_p->ship[j].pos[1],SAT_RANGE)==0))
	         {
 	         sprintf(m_p->recon_str[j],"%-12s: at %03d %03d (%06.1f, satellite)",ships[j],m_p->ship[j].pos[0],m_p->ship[j].pos[1],stdt);
                 for (k=1;k<MAX_USER;k++)
                   {
                   if (party[k] == m_p->satellite[i].party
                                &&
                       m_p->luser[k]==ACTIVE )
                      {
                      sprintf(str1,"> %c detected at %d %d! <",ships[j][0],m_p->ship[j].pos[0],m_p->ship[j].pos[1]);
                      send_msg(k,str1);     
                      }
                   }
                 }
               }
           } /* i Satelliten */            
        } /* j Schiffe */
   } /* if anz_user */
/****************************************/
/* Reparatur Basen/Planeten/Heimatwelt **/
/****************************************/
zaehler++;
if (zaehler>20)
   {
   /* Basen */
   for (i=0;i<MAX_BASES;i++)
       {
       if (m_p->base[i].dam>0) m_p->base[i].dam=m_p->base[i].dam-200;
       if (m_p->base[i].dam<0) m_p->base[i].dam=0;
       if (m_p->base[i].dam<600 && m_p->base[i].sh_energy<BASE_SH_EN) m_p->base[i].sh_energy=m_p->base[i].sh_energy+200;
       if (m_p->base[i].sh_energy>BASE_SH_EN) m_p->base[i].sh_energy=BASE_SH_EN;
       if (m_p->base[i].sh_energy>200 && m_p->base[i].sh==OFF) m_p->base[i].sh=ON;
       if (m_p->base[i].red_count > 0) 
	 {
	 m_p->base[i].condition=RED;
         m_p->base[i].red_count--;
	 }
       else if (m_p->base[i].sh_energy<BASE_SH_EN || m_p->base[i].sh==OFF || m_p->base[i].dam > 0)
         {
	 m_p->base[i].condition=YELLOW;
         }
       else
         {
         m_p->base[i].condition=GREEN;
         }
       }

   /* Planeten */
   for (i=0;i<MAX_PLANETS;i++)
       {
       if (m_p->planet[i].dam>0) m_p->planet[i].dam=m_p->planet[i].dam-(40*m_p->planet[i].level);
       if (m_p->planet[i].dam<0) m_p->planet[i].dam=0;
       }

   /* Heimatwelten */
   if (m_p->homeworld[IMPERIAL].dam > 500) m_p->homeworld[IMPERIAL].dam=m_p->homeworld[IMPERIAL].dam-500;
   if (m_p->homeworld[FEDERAL].dam > 500) m_p->homeworld[FEDERAL].dam=m_p->homeworld[FEDERAL].dam-500;
   if (m_p->homeworld[IMPERIAL].dam <= 500) m_p->homeworld[IMPERIAL].dam=0;
   if (m_p->homeworld[FEDERAL].dam <= 500) m_p->homeworld[FEDERAL].dam=0;
   if (m_p->homeworld[FEDERAL].red_count > 0) 
      {
      m_p->homeworld[FEDERAL].condition=RED;
      m_p->homeworld[FEDERAL].red_count--;
      }
   else if (m_p->homeworld[FEDERAL].dam > 0)
      {
      m_p->homeworld[FEDERAL].condition=YELLOW;
      }
    else
      {
      m_p->homeworld[FEDERAL].condition=GREEN;
      }

   if (m_p->homeworld[IMPERIAL].red_count > 0) 
      {
      m_p->homeworld[IMPERIAL].condition=RED;
      m_p->homeworld[IMPERIAL].red_count--;
      }
   else if (m_p->homeworld[IMPERIAL].dam > 0)
      {
      m_p->homeworld[IMPERIAL].condition=YELLOW;
      }
    else
      {
      m_p->homeworld[IMPERIAL].condition=GREEN;
      }

   /* Sprungtore */
   for (i=0;i<MAX_JUMPGATES;i++)
     {
     if (m_p->jumpgate[i].dam>0) m_p->jumpgate[i].dam=m_p->jumpgate[i].dam-200;
     if (m_p->jumpgate[i].dam<0) m_p->jumpgate[i].dam=0;
     }

   zaehler=0;
   }

/* Feuer auf bewegliche Nicht-User-Objekte (ungetarnte Scouts und Missiles, Robotschiffe) */

/* Scout Foederation */
if (m_p->scout[FEDERAL].status == ACTIVE && m_p->scout[FEDERAL].dam >= MAX_DAM_SCOUT/2)
   {
   /* Meldung durch Satelliten */
   for (i=0;i<MAX_SATS;i++)
     {
     if (m_p->satellite[i].party==IMPERIAL && (abs((m_p->scout[FEDERAL].pos[0]-m_p->satellite[i].pos[0]))) <= SAT_RANGE && (abs((m_p->scout[FEDERAL].pos[1]-m_p->satellite[i].pos[1]))) <= SAT_RANGE)
       {
       sprintf(str1,"> Hostile Scout at %d %d <",
                     m_p->scout[FEDERAL].pos[0],m_p->scout[FEDERAL].pos[1]);
       for (j=1;j<MAX_USER;j++)
	 {
	 if (party[j]==IMPERIAL) send_msg(j,str1);
         }
       }
     }
   /* Forts */
   for (i=0;i<MAX_FORTS;i++)
      {
      if (m_p->fort[i].party != FEDERAL
                       &&
          m_p->fort[i].status == ACTIVE
                       &&
          (abs((m_p->scout[FEDERAL].pos[0]-m_p->fort[i].pos[0])))<=FORT_RANGE
                       &&
          (abs((m_p->scout[FEDERAL].pos[1]-m_p->fort[i].pos[1])))<=FORT_RANGE)
         {              
         /* o.k. Ziel in Reichweite */
         xstart=m_p->fort[i].pos[0]; 
         ystart=m_p->fort[i].pos[1];
         xziel=m_p->scout[FEDERAL].pos[0];
         yziel=m_p->scout[FEDERAL].pos[1];
         /* Freie Schussbahn? */
         if (wegfrei(xstart,ystart,xziel,yziel)==0)
            {
            hit_energy=PH_FORT;
            gauss2(&hit_energy);   
            m_p->scout[FEDERAL].dam=m_p->scout[FEDERAL].dam+hit_energy;
	    m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
            if (m_p->scout[FEDERAL].dam>=MAX_DAM_SCOUT)
	      {
	      m_p->scout[FEDERAL].status=DESTROYED;
	      m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
              }
            } /* if wegfrei */
         } /* if in Reichweite */
      } /* i Forts */
   /* Basen */
   for (i=0;i<MAX_BASES;i++)
      {
      if (m_p->base[i].party != FEDERAL
			&&
          m_p->base[i].status == ACTIVE
			&&
          (abs((m_p->scout[FEDERAL].pos[0]-m_p->base[i].pos[0])))<=BASE_RANGE
			&&
          (abs((m_p->scout[FEDERAL].pos[1]-m_p->base[i].pos[1])))<=BASE_RANGE)
	  {              
	  /* o.k. Ziel in Reichweite */
	  xstart=m_p->base[i].pos[0]; 
	  ystart=m_p->base[i].pos[1];
	  xziel=m_p->scout[FEDERAL].pos[0];
	  yziel=m_p->scout[FEDERAL].pos[1];
	  /* Freie Schussbahn? */
	  if (wegfrei(xstart,ystart,xziel,yziel)==0)
	     {
	     hit_energy=PH_BASE;
	     gauss2(&hit_energy);   
	     m_p->scout[FEDERAL].dam=m_p->scout[FEDERAL].dam+hit_energy;
	     m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
	     if (m_p->scout[FEDERAL].dam>=MAX_DAM_SCOUT)
	       {
	       m_p->scout[FEDERAL].status=DESTROYED;
	       m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
	       }
	     } /* if wegfrei */
	  } /* if in Reichweite */
       } /* i Basen */
       /* Heimatwelt */
     if (m_p->homeworld[IMPERIAL].status == ACTIVE
			&&
     (abs((m_p->scout[FEDERAL].pos[0]-m_p->homeworld[IMPERIAL].pos[0])))<=HOMEWORLD_RANGE
			&&
     (abs((m_p->scout[FEDERAL].pos[1]-m_p->homeworld[IMPERIAL].pos[1])))<=HOMEWORLD_RANGE)
	{              
	/* o.k. Ziel in Reichweite */
        xstart=m_p->homeworld[IMPERIAL].pos[0]; 
        ystart=m_p->homeworld[IMPERIAL].pos[1];
        xziel=m_p->scout[FEDERAL].pos[0];
        yziel=m_p->scout[FEDERAL].pos[1];
        /* Freie Schussbahn? */
        if (wegfrei(xstart,ystart,xziel,yziel)==0)
           {
           hit_energy=PH_HOMEWORLD;
           gauss2(&hit_energy);   
           m_p->scout[FEDERAL].dam=m_p->scout[FEDERAL].dam+hit_energy;
	   m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
	   if (m_p->scout[FEDERAL].dam>=MAX_DAM_SCOUT)
             {
             m_p->scout[FEDERAL].status=DESTROYED;
             m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
             }
           } /* if wegfrei */
        } /* if in Reichweite */

   }
/* Scout Imperium */
if (m_p->scout[IMPERIAL].status == ACTIVE && m_p->scout[IMPERIAL].dam >= MAX_DAM_SCOUT/2)
   {
   /* Meldung durch Satelliten */
   for (i=0;i<MAX_SATS;i++)
     {
     if (m_p->satellite[i].party==FEDERAL && (abs((m_p->scout[IMPERIAL].pos[0]-m_p->satellite[i].pos[0]))) <= SAT_RANGE && (abs((m_p->scout[IMPERIAL].pos[1]-m_p->satellite[i].pos[1]))) <= SAT_RANGE)
       {
       sprintf(str1,"> Hostile Scout at %d %d <",
                     m_p->scout[IMPERIAL].pos[0],m_p->scout[IMPERIAL].pos[1]);
       for (j=1;j<MAX_USER;j++)
	 {
	 if (party[j]==FEDERAL) send_msg(j,str1);
         }
       }
     }
   /* Forts */
   for (i=0;i<MAX_FORTS;i++)
      {
      if (m_p->fort[i].party != IMPERIAL
                       &&
          m_p->fort[i].status == ACTIVE
                       &&
          (abs((m_p->scout[IMPERIAL].pos[0]-m_p->fort[i].pos[0])))<=FORT_RANGE
                       &&
          (abs((m_p->scout[IMPERIAL].pos[1]-m_p->fort[i].pos[1])))<=FORT_RANGE)
         {              
         /* o.k. Ziel in Reichweite */
         xstart=m_p->fort[i].pos[0]; 
         ystart=m_p->fort[i].pos[1];
         xziel=m_p->scout[IMPERIAL].pos[0];
         yziel=m_p->scout[IMPERIAL].pos[1];
         /* Freie Schussbahn? */
         if (wegfrei(xstart,ystart,xziel,yziel)==0)
            {
            hit_energy=PH_FORT;
            gauss2(&hit_energy);   
            m_p->scout[IMPERIAL].dam=m_p->scout[IMPERIAL].dam+hit_energy;
	    m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
            if (m_p->scout[IMPERIAL].dam>=MAX_DAM_SCOUT)
	      {
	      m_p->scout[IMPERIAL].status=DESTROYED;
	      m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
              }
            } /* if wegfrei */
         } /* if in Reichweite */
      } /* i Forts */
   /* Basen */
   for (i=0;i<MAX_BASES;i++)
      {
      if (m_p->base[i].party != IMPERIAL
			&&
          m_p->base[i].status == ACTIVE
			&&
          (abs((m_p->scout[IMPERIAL].pos[0]-m_p->base[i].pos[0])))<=BASE_RANGE
			&&
          (abs((m_p->scout[IMPERIAL].pos[1]-m_p->base[i].pos[1])))<=BASE_RANGE)
	  {              
	  /* o.k. Ziel in Reichweite */
	  xstart=m_p->base[i].pos[0]; 
	  ystart=m_p->base[i].pos[1];
	  xziel=m_p->scout[IMPERIAL].pos[0];
	  yziel=m_p->scout[IMPERIAL].pos[1];
	  /* Freie Schussbahn? */
	  if (wegfrei(xstart,ystart,xziel,yziel)==0)
	     {
	     hit_energy=PH_BASE;
	     gauss2(&hit_energy);   
	     m_p->scout[IMPERIAL].dam=m_p->scout[IMPERIAL].dam+hit_energy;
	     m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
	     if (m_p->scout[IMPERIAL].dam>=MAX_DAM_SCOUT)
	       {
	       m_p->scout[IMPERIAL].status=DESTROYED;
	       m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
	       }
	     } /* if wegfrei */
	  } /* if in Reichweite */
       } /* i Basen */
       /* Heimatwelt */
     if (m_p->homeworld[FEDERAL].status == ACTIVE
			&&
     (abs((m_p->scout[IMPERIAL].pos[0]-m_p->homeworld[FEDERAL].pos[0])))<=HOMEWORLD_RANGE
			&&
     (abs((m_p->scout[IMPERIAL].pos[1]-m_p->homeworld[FEDERAL].pos[1])))<=HOMEWORLD_RANGE)
	{              
	/* o.k. Ziel in Reichweite */
        xstart=m_p->homeworld[FEDERAL].pos[0]; 
        ystart=m_p->homeworld[FEDERAL].pos[1];
        xziel=m_p->scout[IMPERIAL].pos[0];
        yziel=m_p->scout[IMPERIAL].pos[1];
        /* Freie Schussbahn? */
        if (wegfrei(xstart,ystart,xziel,yziel)==0)
           {
           hit_energy=PH_HOMEWORLD;
           gauss2(&hit_energy);   
           m_p->scout[IMPERIAL].dam=m_p->scout[IMPERIAL].dam+hit_energy;
	   m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
	   if (m_p->scout[IMPERIAL].dam>=MAX_DAM_SCOUT)
             {
             m_p->scout[IMPERIAL].status=DESTROYED;
             m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
             }
           } /* if wegfrei */
        } /* if in Reichweite */
   }

/* Missile Foederation falls ungetarnt*/
if (m_p->missile[FEDERAL].status == ACTIVE && m_p->missile[FEDERAL].dam >= MAX_DAM_MISSILE/2)
   {
   /* Meldung durch Satelliten */
   for (i=0;i<MAX_SATS;i++)
     {
     if (m_p->satellite[i].party==IMPERIAL && (abs((m_p->missile[FEDERAL].pos[0]-m_p->satellite[i].pos[0]))) <= SAT_RANGE && (abs((m_p->missile[FEDERAL].pos[1]-m_p->satellite[i].pos[1]))) <= SAT_RANGE)
       {
       sprintf(str1,"> Hostile missile at %d %d <",
               m_p->missile[FEDERAL].pos[0],m_p->missile[FEDERAL].pos[1]);
       for (j=1;j<MAX_USER;j++)
	 {
	 if (party[j]==IMPERIAL) send_msg(j,str1);
         }
       }
     }
   /* Forts */
   for (i=0;i<MAX_FORTS;i++)
      {
      if (m_p->fort[i].party != FEDERAL
                       &&
          m_p->fort[i].status == ACTIVE
                       &&
          (abs((m_p->missile[FEDERAL].pos[0]-m_p->fort[i].pos[0])))<=FORT_RANGE
                       &&
          (abs((m_p->missile[FEDERAL].pos[1]-m_p->fort[i].pos[1])))<=FORT_RANGE)
         {              
         /* o.k. Ziel in Reichweite */
         xstart=m_p->fort[i].pos[0]; 
         ystart=m_p->fort[i].pos[1];
         xziel=m_p->missile[FEDERAL].pos[0];
         yziel=m_p->missile[FEDERAL].pos[1];
         /* Freie Schussbahn? */
         if (wegfrei(xstart,ystart,xziel,yziel)==0)
            {
            hit_energy=PH_FORT;
            gauss2(&hit_energy);   
            m_p->missile[FEDERAL].dam=m_p->missile[FEDERAL].dam+hit_energy;
	    m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
            if (m_p->missile[FEDERAL].dam>=MAX_DAM_MISSILE)
	      {
	      m_p->missile[FEDERAL].status=DESTROYED;
	      m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
              }
            } /* if wegfrei */
         } /* if in Reichweite */
      } /* i Forts */
   /* Basen */
   for (i=0;i<MAX_BASES;i++)
      {
      if (m_p->base[i].party != FEDERAL
			&&
          m_p->base[i].status == ACTIVE
			&&
          (abs((m_p->missile[FEDERAL].pos[0]-m_p->base[i].pos[0])))<=BASE_RANGE
			&&
          (abs((m_p->missile[FEDERAL].pos[1]-m_p->base[i].pos[1])))<=BASE_RANGE)
	  {              
	  /* o.k. Ziel in Reichweite */
	  xstart=m_p->base[i].pos[0]; 
	  ystart=m_p->base[i].pos[1];
	  xziel=m_p->missile[FEDERAL].pos[0];
	  yziel=m_p->missile[FEDERAL].pos[1];
	  /* Freie Schussbahn? */
	  if (wegfrei(xstart,ystart,xziel,yziel)==0)
	     {
	     hit_energy=PH_BASE;
	     gauss2(&hit_energy);   
	     m_p->missile[FEDERAL].dam=m_p->missile[FEDERAL].dam+hit_energy;
	     m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
	     if (m_p->missile[FEDERAL].dam>=MAX_DAM_MISSILE)
	       {
	       m_p->missile[FEDERAL].status=DESTROYED;
	       m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
	       }
	     } /* if wegfrei */
	  } /* if in Reichweite */
       } /* i Basen */
       /* Heimatwelt */
     if (m_p->homeworld[IMPERIAL].status == ACTIVE
			&&
     (abs((m_p->missile[FEDERAL].pos[0]-m_p->homeworld[IMPERIAL].pos[0])))<=HOMEWORLD_RANGE
			&&
     (abs((m_p->missile[FEDERAL].pos[1]-m_p->homeworld[IMPERIAL].pos[1])))<=HOMEWORLD_RANGE)
	{              
	/* o.k. Ziel in Reichweite */
        xstart=m_p->homeworld[IMPERIAL].pos[0]; 
        ystart=m_p->homeworld[IMPERIAL].pos[1];
        xziel=m_p->missile[FEDERAL].pos[0];
        yziel=m_p->missile[FEDERAL].pos[1];
        /* Freie Schussbahn? */
        if (wegfrei(xstart,ystart,xziel,yziel)==0)
           {
           hit_energy=PH_HOMEWORLD;
           gauss2(&hit_energy);   
           m_p->missile[FEDERAL].dam=m_p->missile[FEDERAL].dam+hit_energy;
	   m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
	   if (m_p->missile[FEDERAL].dam>=MAX_DAM_MISSILE)
             {
             m_p->missile[FEDERAL].status=DESTROYED;
             m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
             }
           } /* if wegfrei */
        } /* if in Reichweite */
   } /* Missile Fed. */

/* Missile Imperium falls ungetarnt*/
if (m_p->missile[IMPERIAL].status == ACTIVE && m_p->missile[IMPERIAL].dam >= MAX_DAM_MISSILE/2)
   {
   /* Meldung durch Satelliten */
   for (i=0;i<MAX_SATS;i++)
     {
     if (m_p->satellite[i].party==FEDERAL && (abs((m_p->missile[IMPERIAL].pos[0]-m_p->satellite[i].pos[0]))) <= SAT_RANGE && (abs((m_p->missile[IMPERIAL].pos[1]-m_p->satellite[i].pos[1]))) <= SAT_RANGE)
       {
       sprintf(str1,"> Hostile missile at %d %d <",
                     m_p->missile[IMPERIAL].pos[0],m_p->missile[IMPERIAL].pos[1]);
       for (j=1;j<MAX_USER;j++)
	 {
	 if (party[j]==IMPERIAL) send_msg(j,str1);
         }
       }
     }
   /* Forts */
   for (i=0;i<MAX_FORTS;i++)
      {
      if (m_p->fort[i].party != IMPERIAL
                       &&
          m_p->fort[i].status == ACTIVE
                       &&
          (abs((m_p->missile[IMPERIAL].pos[0]-m_p->fort[i].pos[0])))<=FORT_RANGE
                       &&
          (abs((m_p->missile[IMPERIAL].pos[1]-m_p->fort[i].pos[1])))<=FORT_RANGE)
         {              
         /* o.k. Ziel in Reichweite */
         xstart=m_p->fort[i].pos[0]; 
         ystart=m_p->fort[i].pos[1];
         xziel=m_p->missile[IMPERIAL].pos[0];
         yziel=m_p->missile[IMPERIAL].pos[1];
         /* Freie Schussbahn? */
         if (wegfrei(xstart,ystart,xziel,yziel)==0)
            {
            hit_energy=PH_FORT;
            gauss2(&hit_energy);   
            m_p->missile[IMPERIAL].dam=m_p->missile[IMPERIAL].dam+hit_energy;
	    m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
            if (m_p->missile[IMPERIAL].dam>=MAX_DAM_MISSILE)
	      {
	      m_p->missile[IMPERIAL].status=DESTROYED;
	      m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
              }
            } /* if wegfrei */
         } /* if in Reichweite */
      } /* i Forts */
   /* Basen */
   for (i=0;i<MAX_BASES;i++)
      {
      if (m_p->base[i].party != IMPERIAL
			&&
          m_p->base[i].status == ACTIVE
			&&
          (abs((m_p->missile[IMPERIAL].pos[0]-m_p->base[i].pos[0])))<=BASE_RANGE
			&&
          (abs((m_p->missile[IMPERIAL].pos[1]-m_p->base[i].pos[1])))<=BASE_RANGE)
	  {              
	  /* o.k. Ziel in Reichweite */
	  xstart=m_p->base[i].pos[0]; 
	  ystart=m_p->base[i].pos[1];
	  xziel=m_p->missile[IMPERIAL].pos[0];
	  yziel=m_p->missile[IMPERIAL].pos[1];
	  /* Freie Schussbahn? */
	  if (wegfrei(xstart,ystart,xziel,yziel)==0)
	     {
	     hit_energy=PH_BASE;
	     gauss2(&hit_energy);   
	     m_p->missile[IMPERIAL].dam=m_p->missile[IMPERIAL].dam+hit_energy;
	     m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
	     if (m_p->missile[IMPERIAL].dam>=MAX_DAM_MISSILE)
	       {
	       m_p->missile[IMPERIAL].status=DESTROYED;
	       m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
	       }
	     } /* if wegfrei */
	  } /* if in Reichweite */
       } /* i Basen */
       /* Heimatwelt */
     if (m_p->homeworld[FEDERAL].status == ACTIVE
			&&
     (abs((m_p->missile[IMPERIAL].pos[0]-m_p->homeworld[FEDERAL].pos[0])))<=HOMEWORLD_RANGE
			&&
     (abs((m_p->missile[IMPERIAL].pos[1]-m_p->homeworld[FEDERAL].pos[1])))<=HOMEWORLD_RANGE)
	{              
	/* o.k. Ziel in Reichweite */
        xstart=m_p->homeworld[FEDERAL].pos[0]; 
        ystart=m_p->homeworld[FEDERAL].pos[1];
        xziel=m_p->missile[IMPERIAL].pos[0];
        yziel=m_p->missile[IMPERIAL].pos[1];
        /* Freie Schussbahn? */
        if (wegfrei(xstart,ystart,xziel,yziel)==0)
           {
           hit_energy=PH_HOMEWORLD;
           gauss2(&hit_energy);   
           m_p->missile[IMPERIAL].dam=m_p->missile[IMPERIAL].dam+hit_energy;
	   m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
	   if (m_p->missile[IMPERIAL].dam>=MAX_DAM_MISSILE)
             {
             m_p->missile[IMPERIAL].status=DESTROYED;
             m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
             }
           } /* if wegfrei */
        } /* if in Reichweite */
   } /* Missile Fed. */

/* Robotschiffe Foederation */
for (k=0;k<MAX_ROBOTSHIPS;k++)
  {
  if (m_p->robotship[FEDERAL][k].status == ACTIVE) /* auf Mission, nicht gedockt */
   {
   /* Meldung durch Satelliten */
   for (i=0;i<MAX_SATS;i++)
     {
     if (m_p->satellite[i].party==IMPERIAL && (abs((m_p->robotship[FEDERAL][k].pos[0]-m_p->satellite[i].pos[0]))) <= SAT_RANGE && (abs((m_p->robotship[FEDERAL][k].pos[1]-m_p->satellite[i].pos[1]))) <= SAT_RANGE)
       {
       sprintf(str1,">F.Robotship detected at %d %d<",
               m_p->robotship[FEDERAL][k].pos[0],m_p->robotship[FEDERAL][k].pos[1]);
       for (j=1;j<MAX_USER;j++)
	 {
	 if (party[j]==IMPERIAL) send_msg(j,str1);
         }
       }
     }
   /* Forts */
   for (i=0;i<MAX_FORTS;i++)
      {
      if (m_p->fort[i].party == IMPERIAL
                       &&
          m_p->fort[i].status == ACTIVE
                       &&
          (abs((m_p->robotship[FEDERAL][k].pos[0]-m_p->fort[i].pos[0])))<=FORT_RANGE
                       &&
          (abs((m_p->robotship[FEDERAL][k].pos[1]-m_p->fort[i].pos[1])))<=FORT_RANGE)
         {              
         /* o.k. Ziel in Reichweite */
         xstart=m_p->fort[i].pos[0]; 
         ystart=m_p->fort[i].pos[1];
         xziel=m_p->robotship[FEDERAL][k].pos[0];
         yziel=m_p->robotship[FEDERAL][k].pos[1];
         /* Freie Schussbahn? */
         if (wegfrei(xstart,ystart,xziel,yziel)==0)
            {
            hit_energy=PH_FORT;
            gauss2(&hit_energy);
            printf("Fort Imp.: feindliches RS Nr. %d bekam %d-Units-Treffer. dam: %d\n",k,hit_energy,m_p->robotship[FEDERAL][k].dam);
            if (m_p->robotship[FEDERAL][k].shield==ON)
              {
              m_p->robotship[FEDERAL][k].sh_energy=m_p->robotship[FEDERAL][k].sh_energy-hit_energy;
              m_p->robotship[FEDERAL][k].dam=m_p->robotship[FEDERAL][k].dam+(hit_energy/10);
              if (m_p->robotship[FEDERAL][k].sh_energy<=0) 
                {
                m_p->robotship[FEDERAL][k].shield=OFF;
                if (m_p->robotship[FEDERAL][k].sh_energy<0) m_p->robotship[FEDERAL][k].sh_energy=0;
                }
              }
            else
              {
              m_p->robotship[FEDERAL][k].dam=m_p->robotship[FEDERAL][k].dam+hit_energy;
              }
	    m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
            if (m_p->robotship[FEDERAL][k].dam>=MAX_DAM_ROBOTSHIP)
	      {
	      m_p->robotship[FEDERAL][k].status=DESTROYED;
	      m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
              }
            } /* if wegfrei */
         } /* if in Reichweite */
      } /* i Forts */
   /* Basen */ 
   for (i=0;i<MAX_BASES;i++)
      {
      if (m_p->base[i].party == IMPERIAL
			&&
          m_p->base[i].status == ACTIVE
			&&
          (abs((m_p->robotship[FEDERAL][k].pos[0]-m_p->base[i].pos[0])))<=BASE_RANGE
			&&
          (abs((m_p->robotship[FEDERAL][k].pos[1]-m_p->base[i].pos[1])))<=BASE_RANGE)
	  {              
	  /* o.k. Ziel in Reichweite */
	  xstart=m_p->base[i].pos[0]; 
	  ystart=m_p->base[i].pos[1];
	  xziel=m_p->robotship[FEDERAL][k].pos[0];
	  yziel=m_p->robotship[FEDERAL][k].pos[1];
	  /* Freie Schussbahn? */
	  if (wegfrei(xstart,ystart,xziel,yziel)==0)
	     {
	     hit_energy=PH_BASE;
	     gauss2(&hit_energy);   
             printf("Basis Imp.: feindliches RS Nr. %d bekam %d-Units-Treffer. dam: %d\n",k,hit_energy,m_p->robotship[FEDERAL][k].dam);             
             if (m_p->robotship[FEDERAL][k].shield==ON)
              {
              m_p->robotship[FEDERAL][k].sh_energy=m_p->robotship[FEDERAL][k].sh_energy-hit_energy;
              m_p->robotship[FEDERAL][k].dam=m_p->robotship[FEDERAL][k].dam+(hit_energy/10);
              if (m_p->robotship[FEDERAL][k].sh_energy<=0) 
                {
                m_p->robotship[FEDERAL][k].shield=OFF;
                if (m_p->robotship[FEDERAL][k].sh_energy<0) m_p->robotship[FEDERAL][k].sh_energy=0;
                }
              }
             else
              {
              m_p->robotship[FEDERAL][k].dam=m_p->robotship[FEDERAL][k].dam+hit_energy;
              }
	     m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
	     if (m_p->robotship[FEDERAL][k].dam>=MAX_DAM_ROBOTSHIP)
	       {
	       m_p->robotship[FEDERAL][k].status=DESTROYED;
	       m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
	       }
	     } /* if wegfrei */
	  } /* if in Reichweite */
       } /* i Basen */
       /* Heimatwelt */
     if (m_p->homeworld[IMPERIAL].status == ACTIVE
			&&
     (abs((m_p->robotship[FEDERAL][k].pos[0]-m_p->homeworld[IMPERIAL].pos[0])))<=HOMEWORLD_RANGE
			&&
     (abs((m_p->robotship[FEDERAL][k].pos[1]-m_p->homeworld[IMPERIAL].pos[1])))<=HOMEWORLD_RANGE)
	{              
	/* o.k. Ziel in Reichweite */
        xstart=m_p->homeworld[IMPERIAL].pos[0]; 
        ystart=m_p->homeworld[IMPERIAL].pos[1];
        xziel=m_p->robotship[FEDERAL][k].pos[0];
        yziel=m_p->robotship[FEDERAL][k].pos[1];
        /* Freie Schussbahn? */
        if (wegfrei(xstart,ystart,xziel,yziel)==0)
           {
           hit_energy=PH_HOMEWORLD;
           gauss2(&hit_energy);   
           if (m_p->robotship[FEDERAL][k].shield==ON)
              {
              m_p->robotship[FEDERAL][k].sh_energy=m_p->robotship[FEDERAL][k].sh_energy-hit_energy;
              m_p->robotship[FEDERAL][k].dam=m_p->robotship[FEDERAL][k].dam+(hit_energy/10);
              if (m_p->robotship[FEDERAL][k].sh_energy<=0) 
                {
                m_p->robotship[FEDERAL][k].shield=OFF;
                if (m_p->robotship[FEDERAL][k].sh_energy<0) m_p->robotship[FEDERAL][k].sh_energy=0;
                }
              }
           else
              {
              m_p->robotship[FEDERAL][k].dam=m_p->robotship[FEDERAL][k].dam+hit_energy;
              }
           printf("HW Imp.: feindliches RS Nr. %d bekam %d-Units-Treffer. dam: %d\n",k,hit_energy,m_p->robotship[FEDERAL][k].dam);
	   m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+100;
	   if (m_p->robotship[FEDERAL][k].dam>=MAX_DAM_ROBOTSHIP)
             {
             m_p->robotship[FEDERAL][k].status=DESTROYED;
             m_p->score[IMPERIAL]=m_p->score[IMPERIAL]+500;
             }
           } /* if wegfrei */
        } /* if in Reichweite */
   } 
  } /* Robotschiffe Fed. */

/* Robotschiffe Imperium */
for (k=0;k<MAX_ROBOTSHIPS;k++)
  {
  if (m_p->robotship[IMPERIAL][k].status == ACTIVE) /* auf Mission, nicht gedockt */
   {
   /* Meldung durch Satelliten */
   for (i=0;i<MAX_SATS;i++)
     {
     if (m_p->satellite[i].party==FEDERAL && (abs((m_p->robotship[IMPERIAL][k].pos[0]-m_p->satellite[i].pos[0]))) <= SAT_RANGE && (abs((m_p->robotship[IMPERIAL][k].pos[1]-m_p->satellite[i].pos[1]))) <= SAT_RANGE)
       {
       sprintf(str1,">I.Robotship detected at %d %d<",
       m_p->robotship[IMPERIAL][k].pos[0],m_p->robotship[IMPERIAL][k].pos[1]);
       for (j=1;j<MAX_USER;j++)
	 {
	 if (party[j]==FEDERAL) send_msg(j,str1);
         }
       }
     }
   /* Forts */
   for (i=0;i<MAX_FORTS;i++)
      {
      if (m_p->fort[i].party == FEDERAL
                       &&
          m_p->fort[i].status == ACTIVE
                       &&
          (abs((m_p->robotship[IMPERIAL][k].pos[0]-m_p->fort[i].pos[0])))<=FORT_RANGE
                       &&
          (abs((m_p->robotship[IMPERIAL][k].pos[1]-m_p->fort[i].pos[1])))<=FORT_RANGE)
         {              
         /* o.k. Ziel in Reichweite */
         xstart=m_p->fort[i].pos[0]; 
         ystart=m_p->fort[i].pos[1];
         xziel=m_p->robotship[IMPERIAL][k].pos[0];
         yziel=m_p->robotship[IMPERIAL][k].pos[1];
         /* Freie Schussbahn? */
         if (wegfrei(xstart,ystart,xziel,yziel)==0)
            {
            hit_energy=PH_FORT;
            gauss2(&hit_energy);
            printf("Fort Fed.: feindliches RS Nr. %d bekam %d-Units-Treffer. dam: %d\n",k,hit_energy,m_p->robotship[IMPERIAL][k].dam);            
            if (m_p->robotship[IMPERIAL][k].shield==ON)
              {
              m_p->robotship[IMPERIAL][k].sh_energy=m_p->robotship[IMPERIAL][k].sh_energy-hit_energy;
              m_p->robotship[IMPERIAL][k].dam=m_p->robotship[IMPERIAL][k].dam+(hit_energy/10);
              if (m_p->robotship[IMPERIAL][k].sh_energy<=0) 
                {
                m_p->robotship[IMPERIAL][k].shield=OFF;
                if (m_p->robotship[IMPERIAL][k].sh_energy<0) m_p->robotship[IMPERIAL][k].sh_energy=0;
                }
              }
            else
              {
              m_p->robotship[IMPERIAL][k].dam=m_p->robotship[IMPERIAL][k].dam+hit_energy;
              }
	    m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
            if (m_p->robotship[IMPERIAL][k].dam>=MAX_DAM_ROBOTSHIP)
	      {
              m_p->robotship[FEDERAL][k].status=DESTROYED;
	      m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
   	     printf("HW Imp.: feindliches RS zerstoert!\n");
              }
            } /* if wegfrei */
         } /* if in Reichweite */
      } /* i Forts */
   /* Basen */ 
   for (i=0;i<MAX_BASES;i++)
      {
      if (m_p->base[i].party == FEDERAL
			&&
          m_p->base[i].status == ACTIVE
			&&
          (abs((m_p->robotship[IMPERIAL][k].pos[0]-m_p->base[i].pos[0])))<=BASE_RANGE
			&&
          (abs((m_p->robotship[IMPERIAL][k].pos[1]-m_p->base[i].pos[1])))<=BASE_RANGE)
	  {              
	  /* o.k. Ziel in Reichweite */
	  xstart=m_p->base[i].pos[0]; 
	  ystart=m_p->base[i].pos[1];
	  xziel=m_p->robotship[IMPERIAL][k].pos[0];
	  yziel=m_p->robotship[IMPERIAL][k].pos[1];
	  /* Freie Schussbahn? */
	  if (wegfrei(xstart,ystart,xziel,yziel)==0)
	     {
	     hit_energy=PH_BASE;
	     gauss2(&hit_energy);   
             printf("Basis Fed.: feindliches RS Nr. %d bekam %d-Units-Treffer. dam: %d\n",k,hit_energy,m_p->robotship[IMPERIAL][k].dam);            
             if (m_p->robotship[IMPERIAL][k].shield==ON)
              {
              m_p->robotship[IMPERIAL][k].sh_energy=m_p->robotship[IMPERIAL][k].sh_energy-hit_energy;
              m_p->robotship[IMPERIAL][k].dam=m_p->robotship[IMPERIAL][k].dam+(hit_energy/10);
              if (m_p->robotship[IMPERIAL][k].sh_energy<=0) 
                {
                m_p->robotship[IMPERIAL][k].shield=OFF;
                if (m_p->robotship[IMPERIAL][k].sh_energy<0) m_p->robotship[IMPERIAL][k].sh_energy=0;
                }
              }
             else
              {
              m_p->robotship[IMPERIAL][k].dam=m_p->robotship[IMPERIAL][k].dam+hit_energy;
              }
	     m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
	     if (m_p->robotship[IMPERIAL][k].dam>=MAX_DAM_ROBOTSHIP)
	       {
	       m_p->robotship[IMPERIAL][k].status=DESTROYED;
	       m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
	       }
	     } /* if wegfrei */
	  } /* if in Reichweite */
       } /* i Basen */
       /* Heimatwelt */
     if (m_p->homeworld[FEDERAL].status == ACTIVE
			&&
     (abs((m_p->robotship[IMPERIAL][k].pos[0]-m_p->homeworld[FEDERAL].pos[0])))<=HOMEWORLD_RANGE
			&&
     (abs((m_p->robotship[IMPERIAL][k].pos[1]-m_p->homeworld[FEDERAL].pos[1])))<=HOMEWORLD_RANGE)
	{              
	/* o.k. Ziel in Reichweite */
        xstart=m_p->homeworld[FEDERAL].pos[0]; 
        ystart=m_p->homeworld[FEDERAL].pos[1];
        xziel=m_p->robotship[IMPERIAL][k].pos[0];
        yziel=m_p->robotship[IMPERIAL][k].pos[1];
        /* Freie Schussbahn? */
        if (wegfrei(xstart,ystart,xziel,yziel)==0)
           {
           hit_energy=PH_HOMEWORLD;
           gauss2(&hit_energy);   
           printf("HW Fed.: feindliches RS Nr. %d bekam %d-Units-Treffer. dam: %d\n",k,hit_energy,m_p->robotship[IMPERIAL][k].dam);
           if (m_p->robotship[IMPERIAL][k].shield==ON)
              {
              m_p->robotship[IMPERIAL][k].sh_energy=m_p->robotship[IMPERIAL][k].sh_energy-hit_energy;
              m_p->robotship[IMPERIAL][k].dam=m_p->robotship[IMPERIAL][k].dam+(hit_energy/10);
              if (m_p->robotship[IMPERIAL][k].sh_energy<=0) 
                {
                m_p->robotship[IMPERIAL][k].shield=OFF;
                if (m_p->robotship[IMPERIAL][k].sh_energy<0) m_p->robotship[IMPERIAL][k].sh_energy=0;
                }
              }
           else
              {
              m_p->robotship[IMPERIAL][k].dam=m_p->robotship[IMPERIAL][k].dam+hit_energy;
              }
	   m_p->score[FEDERAL]=m_p->score[FEDERAL]+100;
	   if (m_p->robotship[IMPERIAL][k].dam>=MAX_DAM_ROBOTSHIP)
             {
             m_p->robotship[IMPERIAL][k].status=DESTROYED;
             m_p->score[FEDERAL]=m_p->score[FEDERAL]+500;
             }
           } /* if wegfrei */
        } /* if in Reichweite */
   } 
  } /* Robotschiffe Emp. */

printf(" DEFEAT: pause!\n");
usleep(DEF_DELAY);
goto start;

/*(void) shmdt(m_p);
exit(0);*/
}

void sig_hnd()
/***********************************************************************/
/*** Fuer signal(); setzt nach pause() signal-handling neu           ***/
/***********************************************************************/
{
signal(SIGALRM,sig_hnd);
return;
}



/*** qgetkey.c         ---    Unterprogramm vom Typ key_t                  ***/

/*****************************************************************************/
/*** Uebersetzung mit "cc qgetkey.c -c"                                    ***/
/*** qgetkey() liefert fuer den symbolischen Namen einer Nachrichtenwarte- ***/
/*** schlange den Wert (key) zurueck. Nur die ersten 4 Zeichen des symb.   ***/
/*** Namens sind signifikant!                                              ***/
/*** Beispiel: aus dem symbolischen Namen "ambit" wird der Wert 0x616d6269 ***/
/*****************************************************************************/

#include <sys/types.h>

key_t qgetkey(p_string)
 register char *p_string;

{

key_t key = (key_t)0;
register int i;

for (i = 0;
 i < sizeof(key_t)   /* Solange i < Zahl der Bytes von key_t */
 &&                  /* UND */
 p_string[i];        /* p_string[i] */
 i++)
    {
    key=(key << 8) | p_string[i]; /*8-Bit ASCII-Wert ODER um 8 Bit geschiftet */
    }
    
return(key);
}
/******************  ENDE qgetkey() ******************************************/

void hit_def(b_party,zflag,energy)
     int b_party,zflag,energy;
/**************************************************************************/
/*** Trefferwirkung auf Schiffe und Romulaner                           ***/
/**************************************************************************/
{
int pntsd,pnts,device;
float random;

/*************/
/* Romulaner */
/*************/
if (zflag==0)
   {
   m_p->score[b_party]=m_p->score[b_party]+50;
   if (m_p->ship[0].shield==ON)
      {
      m_p->ship[0].sh_energy=m_p->ship[0].sh_energy-energy;
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+(energy/10);
      if (m_p->ship[0].sh_energy<=0) 
         {
         m_p->ship[0].shield=OFF;
         if (m_p->ship[0].sh_energy<0) m_p->ship[0].sh_energy=0;
         }
      }
   else
      {
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+energy;
      /* Schadenueberwachung macht R. selber */
      if (m_p->ship[0].dam[0]>=max_dam[0])
	{
        m_p->score[b_party]=m_p->score[b_party]+max_dam[0];
        m_p->l_exit[0]=EXIT;
        printf("DEF: Romulan destroyed!\n");
	return;
        }
      }
   return;
   }       /* Ende Romulaner */   

/***********/
/* Schiffe */
/***********/   
if (zflag>0 && zflag<MAX_USER)
   {
   printf("Basis: Feuer auf Schiff Nr. %d (%s) mit %d Einheiten\n",zflag,ships[zflag],energy);
   pnts=30+(level_of_ship[zflag]*10);
   pntsd=max_dam[zflag];
   m_p->score[b_party]=m_p->score[b_party]+pnts;
   random=10.0*(ran()+0.1); /* Zufallszahl device fuer damages */
   device=(int) random;
   if (device <= 0) device=1;
   if (device > 10) device=10;
   if (level_of_ship[zflag] == DESTROYER && device == 4) device=5;
   if (m_p->ship[zflag].shield==ON)
      {
      m_p->ship[zflag].sh_energy=m_p->ship[zflag].sh_energy-energy;
      if (m_p->ship[zflag].sh_energy<0) m_p->ship[zflag].sh_energy=0;      
      m_p->ship[zflag].dam[device]=m_p->ship[zflag].dam[device]+(energy/10);
      if (m_p->ship[zflag].sh_energy<=0) 
         {
         m_p->ship[zflag].shield=OFF;
         }
      }
   else
      {
      m_p->ship[zflag].dam[device]=m_p->ship[zflag].dam[device]+energy; 
      }
   if (m_p->ship[zflag].dam[0]>=max_dam[zflag])
      {
      m_p->a[m_p->ship[zflag].pos[0]][m_p->ship[zflag].pos[1]]=EXPLOSION;
      m_p->score[b_party]=m_p->score[b_party]+pntsd;
/*      delay(0.3); */
      usleep(300000);
      m_p->a[m_p->ship[zflag].pos[0]][m_p->ship[zflag].pos[1]]=SPACE;
      return; 
      }
   return;
   }       /* Ende Schiffe */   
   
return;
}

void gauss2(energy)
     int *energy;
/***************************************************************************/
/*** Berechnet die Trefferenergie bei Phaser (Gaussverteilung um energy) ***/
/***************************************************************************/
{
float fx,rnd,fenergy;

rnd=ran();
fenergy=(float) (*energy);
fx=exp( ((-0.5)*(rnd-0.5)*(rnd-0.5)*25.0) );
if (rnd<0.5) 
   { 
   *energy=(*energy- (NINT( ((1.0-fx)*0.25*fenergy) ) ));
   }
else
   {
   *energy=(*energy+ (NINT( ((1.0-fx)*0.10*fenergy) ) ));  
   }
   
return;
}

void delay(warten)
/****************************************************************************/
/*** delay: liefert delay von warten Sekunden                             ***/
/****************************************************************************/
float warten;

{
	float zeit,zstat,zfrage;
	
        zeit = 0.0;
        zstat = ( (float) clock() )  / 1000000.0;
        zfrage = zstat + warten;

	while (zeit  < zfrage)
	{
		zeit = ( (float) clock() )  / 1000000.0;
	}
}


float ran()
/*****************************************************************************/
/*** ran() liefert eine Float-Zufallszahl 0.0 bis 1.0                      ***/
/*****************************************************************************/
{
int id,m1,ia1,m2,ia2,ic2,ic1,m3,ia3,ic3,iff,ix1,ix2,ix3,j,dummy,idum,err_cnt;
float r[97],rm1,rm2;

id=seed();	
m1=259200;
ia1=7141;
ic1=54773;
rm1=1.0/(float) m1;
m2=134456;
ia2=8121;
ic2=28411;
rm2=1.0/(float) m2;
m3=243000;
ia3=4561;
ic3=51349;
iff=0;
err_cnt = 0;
ix1=seed();
ix2=seed();
ix3=seed();
idum=seed();

init:
dummy=seed();
idum=idum+dummy/10000;
idum=(idum*10000)-dummy*id;
idum=idum*id; 

/* as above, initailize on first call even if idum is not negative */

if ( (idum<0) || (iff==0) )
   {
   iff=1;
   ix1=((ic1-idum)%m1);   /* seed the first routine */
   ix1=((ia1*ix1+ic1)%m1);
   ix2=ix1%m2;  /* and use it to seed the second */
   ix1=((ia1*ix1+ic1)%m1);
   ix3=ix1%m3;  /* and the third routine */
   /* fill the table with sequ. uniform deviates gen. by the routines 1,2 */
   for (j=0;j<97;j++)
       {
       ix1=((ia1*ix1+ic1)%m1);
       ix2=((ia2*ix2+ic2)%m2);
       r[j]=((rm2*ix2)+ix1)*rm1; /* low and high-order pieces combined here */
       }
   idum=1;
   }
/*  except when initializing,this is where we start generate the next number */
/*  for each sequence */
ix1=(ia1*ix1+ic1)%m1;
ix2=(ia2*ix2+ic2)%m2;
ix3=(ia3*ix3+ic3)%m3;
/* use the third sequ. to get an int between 1 and 97 */;
j=1 + ((float)(97*ix3) / (float)m3);
if (err_cnt>100)
   {
   j = 0;
   err_cnt = 0;
   }
if (j>96 || j<0)
   {
   err_cnt++;
   goto init;
   }
/* return that table entry and refill it */
r[j]= rm1 * (ix1+(ix2 * rm2));
dummy=r[j]*1000;
r[j]=r[j]*1000.0-(float)dummy;
r[j]=sqrt( (r[j]*r[j]) ); 
return(r[j]);
}    /**** Ende ran.c ****/


int seed(void)
/* benoetigt -lrt beim Linken */
{
  int i;
  struct timespec ntime;
  clock_gettime(CLOCK_REALTIME, &ntime);
  i = (int) ntime.tv_nsec%10000;
  return i;
}

void path(xsteps,ysteps,rxdiff,rydiff,steps)
/***************************************************************************/
/*** path: Berechnet fuer vorgegebene x- und y-Flugweite die Schrittzahl ***/
/***       steps und die jeweilige Schrittweite (float) rxdiff u. rydiff ***/
/***************************************************************************/
int xsteps,ysteps,*steps;
float *rxdiff,*rydiff;
{
*steps=(IMAX( (abs(xsteps)) , (abs(ysteps)) ));
*rxdiff= ((float) xsteps)/((float) *steps);   	
*rydiff= ((float) ysteps)/((float) *steps);   	    
return;
}

int wegfrei(x,y,xz,yz)
    int x,y,xz,yz;
/*************************************************************************/
/** Ueberprueft, ob die Schussbahn frei ist                             **/
/*************************************************************************/
{
int xsteps,ysteps,steps,i;
int xalt,yalt,xneu,yneu,cond;
char aneu;

float rxdiff,rydiff;

xsteps=xz-x;
ysteps=yz-y;

path(xsteps,ysteps,&rxdiff,&rydiff,&steps);
cond=0;
if (steps==1) return(cond);
if (steps<=0) return(1);
xalt=x;
yalt=y;
i=1;
while (i<steps && cond==0)
    {
    xneu=xalt+ (NINT( (((float) i)*rxdiff) ));
    yneu=yalt+ (NINT( (((float) i)*rydiff) ));
    aneu=m_p->a[xneu][yneu];
    if (aneu != SPACE && aneu != EXPLOSION) 
       {
       if (aneu==TORPEDO || aneu==MINE) 
          {
          m_p->a[xneu][yneu]=EXPLOSION;
          /* delay(0.5); */
          usleep(500000);
          m_p->a[xneu][yneu]=SPACE;
          }
       else
          {
          cond=1;
          }          	
       }
    i++;       
    } 
     
return(cond);
}  

void send_msg(zflag,new_msg)
/*****************************************************************************/
/***      sort_msg: bereitet die msg-Strings des Users vflag so auf, dass  ***/
/**                 new_msg hinten (im Cockpit-Window unten) steht.        ***/
/*****************************************************************************/
int zflag;
char new_msg[MSG_LENGTH];
{
int i;

for (i=0;i<6;i++)
    { 
    strncpy(m_p->ship[zflag].msg[i],m_p->ship[zflag].msg[(i+1)],MSG_LENGTH);
    }  
strncpy(m_p->ship[zflag].msg[6],new_msg,MSG_LENGTH);
return;
}

float star_date(t1)
     long t1;
/************************************************************************/
/***                    Berechnung der Sternzeit                      ***/
/************************************************************************/
{
double z1;
float z;

z1=(double)t1;

z1=z1/60.0;
while (z1>9000) z1=z1-9000.0;

z=(float)z1;

return(z);
}


void sig_hnd_ctrlc()
/************************************************************************/
/*** setzen des Signalhandlings fuer CTRL-C                           ***/
/************************************************************************/
{
int i,j;

printf("Abbruch def mit CTRL-C: Zuruecksetzen der survey_array-Werte erfolgt...");

m_p->isurvey_area[FEDERAL]=-1;
m_p->isurvey_area[IMPERIAL]=-1;
m_p->fsurvey_area[FEDERAL]=-1.0;
m_p->fsurvey_area[IMPERIAL]=-1.0;

for (i=0;i<MAP_SIZE;i++)
  {
  for (j=0;j<MAP_SIZE;j++)
    {
    m_p->map[0][i][j]='?';
    m_p->map[1][i][j]='?';
    }
  }

printf(" ...Werte auf -1, -1.0\n");
exit(1);
}

int behind(x,y,x0,y0,range)
    int x,y,x0,y0,range;
/**************************************************************************/
/** Ueberprueft, ob die Position x,y im Radarlee von Position x0,y0 liegt */
/**************************************************************************/
{
int xsteps,ysteps,steps,i;
int xalt,yalt,xneu,yneu,cond;
float rxdiff,rydiff;

xsteps=x-x0;
ysteps=y-y0;

steps=(IMAX(abs(xsteps),abs(ysteps)));

if (steps==0) return(0);
if (steps>range) return(1);

rxdiff=(float) xsteps/ (float) steps;
rydiff=(float) ysteps/ (float) steps;

xalt=x0;
yalt=y0;
i=1;
cond=0;

while (i<steps && cond==0)
  {
  xneu=xalt+ (NINT( (((float) i)*rxdiff) ));
  yneu=yalt+ (NINT( (((float) i)*rydiff) ));
  if (m_p->a[xneu][yneu] == CLOUD) cond=1;
  i++;
  }  

return(cond);
}

