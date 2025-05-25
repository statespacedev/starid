/**********************************************************/
/*** cockpit.c                                          ***/
/*** Kommentar nach V.1.00                              ***/
/*** 14.02.03: String in sort_msg auf 33 begrenzt       ***/
/***           dito sort_exit_msg                       ***/
/*** 18.02.03: TMusleep weg wg. Linux, siehe dwmain.c   ***/
/*** 19.02.03: Window-Initialisierung in zwei Stufen    ***/
/***           mit pause wegen Empty-Screen-Effekt      ***/
/*** 20.02.03: Aederung vom 19.2. entfernt, stattdessen ***/
/***           warten, bis luser und xpos,ypos fuer     ***/
/***           vflag in dwmain gesetzt worden sind.     ***/
/*** 20.02.03: Stringlaenge des Schiffnamens auf 14 im  ***/
/***           head-Fenster                             ***/
/*** 20.02.03: strlen in sort_msg, sort_exit_msg nach-  ***/
/***           getragen, war sonst schon drin           ***/
/*** 25.02.03: neue Variable xmsg in sort_msg, da der   ***/
/***           String new_msg u.U. geaendert wird       ***/
/***           dito sort_exit_msg                       ***/
/*** 25.02.03: endwin nur bei normalem Ende wg. nicht   ***/
/***           mehr angezeigtem R.I.P. bei Linux        ***/
/*** Kommentar nach V.1.0.1                             ***/
/*** 11.04.03: Anzeige Torpedo-Tube-Status eingebaut    ***/
/*** 25.04.03: m_p->l_torp[vflag] entfernt              ***/
/*** 25.04.03: Torpedo-Tube(s)-Anzeige geaendert        ***/
/**********************************************************/
/*** 29.04.03: Version 1.1.0                            ***/
/**********************************************************/
/*** 27.04.04: Umstellung auf strncpy in sort_msg und   ***/
/***           send_msg                                 ***/
/*** 07.05.04: Umstellung struct fort                   ***/
/*** 29.05.04: explosion neu als Fork                   ***/
/*** 30.06.04: MSG_LENGTH neu in dw_def.h               ***/
/*** 30.06.04: Ende bei Zerstoerung berichtigt          ***/
/*** 30.06.04: Umstellung auf strncpy in sort_exit_msg  ***/
/*** 30.06.04: kein =0 in bei Zerstoerer Damage Sektion ***/
/***           4 in Ueberpruefung Schadensumme          ***/
/*** 30.06.04: libdw weg                                ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 17.07.07: m_p->l_hw durch mp->display_status ers.  ***/
/***           HW_nnn durch DISPLAY_nnn ersetzt         ***/
/*** 23.07.07: head-Window HW geaendert: stardate+dam.  ***/
/***           scan-Window HW: SD bp realisiert         ***/
/*** 24.07.07: recon_str bei SD re eingefuehrt (scan)   ***/
/*** 24.07.07: Neue Startposition: Meldung (message)    ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 28.07.07: head: HW: Position HW                    ***/
/***           sstatus: HW: Position durch survey_area- ***/
/***           Anzeige ersetzt; Anz. Spieler jetzt hier.***/
/***           Central Text weg                         ***/
/*** 04.08.07: Zerstoerung Schiff in recon_str          ***/
/*** 12.08.07: Anzeige DISPLAY_MAP                      ***/
/*** 27.08.07: SCANX,Y                                  ***/
/*** 01.09.07: Aenderung der SD BP-Anzeige: alle Rubri- ***/
/***           ken werden angezeigt, auch wenn leer     ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 23.01.08: Umstellung struct satellite              ***/
/*** 24.01.08: Umstellung struct planet                 ***/
/*** 25.01.08: Umstellung struct base                   ***/
/*** 26.01.08: Anzeige von SD HW (Homeworld base + HQ)  ***/
/*** 01.02.08: Radarlee (testweise)                     ***/
/*** 06.02.08: Neues Dockbild                           ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 12.02.08: Kosmetik am HW-Dockbild                  ***/
/*** 16.02.08: m_p->radarlee u. RLEE_ON/OFF eingefuehrt ***/
/*** 17.02.08: Radarlee fuer scout                      ***/
/*** 22.02.08: Sprungtor-Funktion                       ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 25.02.08: Jumpgates in DISPLAY_BP integriert       ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 12.03.08: Missile neu                              ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 13.03.08: Umstellung auf struct homeworld          ***/
/*** 15.03.08: HOOD_ON/OFF, RLEE_ON/OFF, BEAM_ON/OFF,   ***/
/***           RAMM_ON/OFF durch ON/OFF ersetzt,        ***/
/***           DEFEAT_ON/OFF weg. N_/EXISTING durch     ***/
/***           NOT_/ACTIVE ersetzt                     ***/
/*** 23.03.08: MOVING durch ON ersetzt                  ***/
/*** 24.03.08: SH_DOWN/UP durch ON/OFF ersetzt          ***/
/***           N/JUMPING durch OFF/ON ersetzt           ***/
/*** 26.03.08: Umstellung auf struct ship               ***/
/*** 27.03.08: Neue HW-Darstellung mit condition        ***/
/*** 28.03.08: Dockbild Basis an HW angepasst           ***/
/*** 29.03.08: Dockbild HW um Scout und Missile erweit. ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 31.03.08: Format %4.1f durch %06.1f ersetzt        ***/
/*** 03.04.08: SV-Flag eingebaut                        ***/
/*** 06.04.08: Missile path eingefuehrt                 ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/*** 14.10.08: Anpassungen fuer Ubuntu 8.04 (includes)  ***/
/*** 14.10.08: Korrektur strb1/2 Stringlaenge 22->23    ***/
/***           wegen Fehler bei SD bp-Anzeige           ***/
/*** 23.10.08: Anpassungen -Wall bei gcc (Ubuntu 8.04)  ***/
/*** 23.10.08: str2 auf 12 (Fehler bei Anzeige MP on)   ***/
/*** 26.10.08: Radarbild: Leerzeichen bei Werten ausser-***/
/***           halb des Spielfeldes                     ***/
/*** 27.10.08: Neues HW-Dockbild; Compiler-Warnungen    ***/
/***           bzgl. jcnt und k bearbeitet              ***/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 21.10.09: Anpassungen wg. buffer overflow mit gcc  ***/
/***           4.3.3: str1 auf Laenge 24,etliche strcpy ***/
/***           und strcat durch strncpy,strncat ersetzt ***/
/***           sowie Radarbildanzeige optimiert         ***/
/*** 24.10.09: Anpassungen wg. buffer overflow: strs4   ***/
/***           und Strings fuer SD BP                   ***/ 
/*** 26.10.09: Bei HW-Docking Player-Anzeige fuer beide ***/
/***           Seiten                                   ***/
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 27.11.09: msgw-Window: Strings mit Leerzeichen     ***/
/***           aufgefuellt                              ***/
/*** 28.11.09: SIDEX/SIDEY bei Window-Def. neu          ***/
/*** 28.11.09: Scan-Window: nicht gefuellte scan_row    ***/
/***           mit Leerzeichen aufgefuellt              ***/
/*** 12.12.09: TT-Reload jetzt hier mit Zeitstempel     ***/
/*** 13.12.09: used_ttubes entfernt                     ***/
/*** 20.11.09: msgw-Window: Strings mit Leerzeichen     ***/
/***           auffuellen: aber nur falls notwendig!    ***/
/***           Ansonsten leeres msgw-Window bei HW-Dock ***/
/***           wenn ohne Optimierung compiliert wird    ***/
/***           (gcc 4.3.3)                              ***/
/*** 26.12.09: Index-Ueberschreitung scan_row[] bei     ***/
/***           Explosion beseitigt. Fuehrte bei Compil. ***/
/***           ohne Optimierung zum Absturz von cockpit ***/
/*** 03.01.10: QUIT/EXIT: DESTROYED zuerst, Aenderun-   ***/
/***           gen im Ende-Text                         ***/
/*** 03.01.10: String bei Area-Anzeige verlaengert      ***/
/*** 14.01.10: Letztes Zeichen in Radarbild auf ' ' ge- ***/
/***           setzt, weil dort Explosionsreste auf-    ***/
/***           tauchten. Besser: line neu befuellen     ***/
/*** 23.01.10: Stringlaenge bei Area (-def?-) korr.     ***/
/*** 30.01.10: Ausgabe % von Area geaendert (>10,<10)   ***/
/*** 21.03.10: Status WAITING bei Robotschiffanzeige    ***/
/*** 27.03.10: Status CLAIMED bei Robotschiffanzeige    ***/
/*** 02.04.10: Kosmetik                                 ***/
/*** 24.04.10: neue Konstantendefinitionen aus dw_def.h ***/
/*** 25.04.10: IMAX,IMIN,IABS ueberall in () gesetzt    ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 03.05.10: Statusanzeige "under construction" RS    ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
/*** 08.09.11: Korrekturen bei Anzeige SD RA und SD BP  ***/
/*** 21.01.12: Korrektur bei Anzeige SD MA              ***/
/**********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include "mathfkt.h"
/*************************************************************************/
/***   Includes DECWAR                                                 ***/
/*************************************************************************/
#include "dw_def.h"
#include "dw_shm.h"
#include "ships.h"

static int vflag;
static long rest,start_zeit;

extern key_t qgetkey();
extern void sig_hnd();
extern float star_date();
extern void status_ctrl();
extern void sort_exit_msg();
extern void sort_msg();
extern void sig_hnd_ctrlc();
extern int find_nr_base();
extern int behind();
extern int iproz();

/****************************************************************************/
/****                            main COCKPIT                             ***/
/****************************************************************************/
int main(argc,argv)
    int argc;
    char *argv[];

                         /* Uebergabewert vflag als String in argv[0] */

/****************************************************************************/
/***                             Lokale Deklarationen                     ***/
/****************************************************************************/
{
    /*   SCREEN *cockpit; */
    WINDOW *scan, *com, *sstatus, *head, *msgw;
    int i,ip,nrb;
    int anz_bas,anz_pl,anz_sat,anz_fort;
    int anz_pl1,anz_pl2,anz_pl3,anz_pl4;
    int startx,anzx,endx,jcnt;
    int starty,endy;
    int xscout,yscout;
    int ibattle,icruiser,idest;
    int j,k,n,anz_row;
    int expl[SCANY][SCANX],expl_flag,flag;
    float stdt;
    long t1,t2;
    char a1,line[SCANX],nr_of_col[5],nr_of_row[4],scan_row[SCANY][SCANX];
    char str1[24],strb1[23],strb2[23],str2[9],rstr[3],s0dummy[SCANX+10];
    char strs1[13],strs2[13],strs3[13],strs4[9],print_str[SCANX+1];
    char strf1[(SCANX/3)+1],strf2[(SCANX/3)+1],strf3[(SCANX/3)+2];

    struct timespec ntime;
    long long elapstime;
/****************************************************************************/
/***                          Uebergabeparameter auslesen                 ***/
/****************************************************************************/  
sscanf(*argv,"%d",&vflag); 

/****************************************************************************/
/***                          Startbelegungen                             ***/
/****************************************************************************/
start_zeit=time(0);
rest=999L;
expl_flag=N_EXPLODING;
signal(SIGKILL,SIG_DFL);
signal(SIGALRM,sig_hnd);
signal(SIGINT,sig_hnd_ctrlc);

/****************************************************************************/
/***                        Shared Memory einbinden                       ***/
/****************************************************************************/
/* Einbinden von Shared Memory */
shmid = shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Kann Shared Memory nicht einbinden!---> Abbruch!\n");
   exit(1); 
   }
if (shmid != m_p->shm_id)
   {
   printf("Error, shmid=%d m_p->shm_id=%d\n",shmid,m_p->shm_id);
   exit(1);
   }	
/* Statt pause wg. leerem Bildschirm */
do
  {
  usleep(200000);
  } while (m_p->luser[vflag] != ACTIVE ||
           m_p->ship[vflag].pos[0] < 0 ||
           m_p->ship[vflag].pos[1] < 0 ||
           m_p->ship[vflag].pos[0] >= ARRAY_SIZE ||
           m_p->ship[vflag].pos[0] >= ARRAY_SIZE);

/****************************************************************************/
/***                       Window-Definitionen                            ***/
/****************************************************************************/
initscr();
/* cockpit=newterm(getenv("TERM"),stdout,stdin);
set_term(cockpit); */

scan=newwin(SCANY,SCANX,0,0);
wclear(scan);
head=newwin(2,SIDEX,0,SCANX);
wclear(head);
sstatus=newwin(14,SIDEX,2,SCANX);
wclear(sstatus);
msgw=newwin(9,SIDEX,15,SCANX);
wclear(msgw);
com=newwin(SIDEY,SCANX,SCANY,0);
wclear(com);
box(sstatus,'|','-');
box(msgw,'|','-');
box(com,'|','-');

wrefresh(scan);
wrefresh(head);
wrefresh(sstatus);
wrefresh(com);
wrefresh(msgw);

/*****************************************************************************/
/***                    Hauptschleife mit Window-Update                    ***/
/*****************************************************************************/

jcnt=0;

do
   {
   clock_gettime(CLOCK_REALTIME, &ntime);
   elapstime=((long long)ntime.tv_sec*1000000000) + ntime.tv_nsec;
   for (i=0;i<MAX_TT;i++) /* gibt es noch nicht geladene Torpedorohre? */
     {
     if (m_p->ship[vflag].status_ttubes[i]==RELOADING 
                             && 
         (elapstime - m_p->ship[vflag].tt_lock_time[i] > RELOAD_TT_TIME) )
       {
       m_p->ship[vflag].status_ttubes[i]=LOADED;
       }
     }
   if (m_p->display_status[vflag] == DISPLAY_SCOUT && m_p->scout[party[vflag]].status == DESTROYED)
     {
     sort_msg("Display reset to RADAR!");
     m_p->display_status[vflag]=DISPLAY_RADAR;
     }
   t1=time(0);
   /****************************************************/
   /***                Radarbild scan                ***/
   /****************************************************/
   /* Moeglichkeiten                             flag  */
   /* 1) Gedockt an Basis:Voreinst. (DISPLAY_BASIS) 0  */
   /*                     Radarbild (DISPLAY_RADAR) 3  */
   /* 2) Gedockt an HW:                                */
   /*    2a) HW-Bild (Voreinstellung, DISPLAY_HW)   1  */
   /*    2b) Flottenstatus (DISPLAY_FLEET)          2  */
   /*    2c) Radarbild (DISPLAY_RADAR)              3  */
   /*    2d) Scout-Radarbild (DISPLAY_SCOUT)        4  */
   /*    2e) Aufklaerungsdaten (DISPLAY_RECON)      5  */
   /*    2f) Gesamtstatus (DISPLAY_BP)              6  */
   /*    2g) Gesamtstatus (DISPLAY_MAP)             7  */
   /*    2h) Status Robotschiffe (DISPLAY_ROBOT)    8  */
   /* 3) Flug (nicht gedockt):                         */
   /*    3a) Radarbild (Radar ok & DISPLAY_RADAR)  10 (STANDARTANZEIGE!) */
   /*    3b) nichts (DISPLAY_RADAR & Radar defekt) 11   */
   /*    3c) Scout-Radarbild (DISPLAY_SCOUT)       12   */
   /*    3d) Aufklaerungsdaten (DISPLAY_RECON)     13   */
   /*    3e) Planetenbesetzung (CAPTURING)         14   */
   /*    3f) Schiff wird getroffen (HIT)           15   */
   /*    3g) Sprung (jump=ON, DISPLAY_JUMP)        16   */
   /*****************************************************/

   /*****************/
   /* flag besetzen */
   /*****************/
   if (m_p->ship[vflag].dock == DOCKED) 
     {
     flag=0;
     if (m_p->display_status[vflag] == DISPLAY_RADAR) flag=3;
     }
   else if (m_p->ship[vflag].dock == HW_DOCKED)
     {
     if (m_p->display_status[vflag] == DISPLAY_FLEET)      flag=2;
     else if (m_p->display_status[vflag] == DISPLAY_RADAR) flag=3;
     else if (m_p->display_status[vflag] == DISPLAY_SCOUT) flag=4;
     else if (m_p->display_status[vflag] == DISPLAY_RECON) flag=5;
     else if (m_p->display_status[vflag] == DISPLAY_BP)    flag=6;
     else if (m_p->display_status[vflag] == DISPLAY_MAP)   flag=7;
     else if (m_p->display_status[vflag] == DISPLAY_ROBOT) flag=8;
     else flag=1; /* u.a. == DISPLAY_HW */
     }
   else
     {
     if (m_p->display_status[vflag] == DISPLAY_RADAR && m_p->ship[vflag].dam[8] > 250) flag=11;
     else if (m_p->display_status[vflag] == DISPLAY_SCOUT)     flag=12;
     else if (m_p->display_status[vflag] == DISPLAY_RECON)     flag=13;
     else if (m_p->display_status[vflag] == DISPLAY_CAPTURING) flag=14;
     else if (m_p->display_status[vflag] == DISPLAY_HIT)       flag=15;
     else if (m_p->display_status[vflag] == DISPLAY_JUMP)      flag=16;
     else flag=10; /* u.a. == DISPLAY_RADAR und ship[vflag].dam[8] < 250 */
     }

   /*********************************/
   /* Window-Ausgaben (5 Stueck):   */
   /* scan                          */
   /* head                          */
   /* sstatus                       */
   /* com                           */
   /* msgw                          */
   /*********************************/

   /****************************************************/
   /* Window fuer alle Flagwerte gleich: com msgw      */
   /****************************************************/
   /* msgw */
   mvwprintw(msgw,0,12,"-Messages-");
   for (i=1;i<8;i++)
     {
     strncat(m_p->ship[vflag].msg[i-1],"                                 ",(MSG_LENGTH-strlen(m_p->ship[vflag].msg[i-1])));
     mvwprintw(msgw,i,1,"%-s",m_p->ship[vflag].msg[i-1]);      
     } 

   /* com */
   mvwprintw(com,1,1,"COMMAND: %s",m_p->cmd[vflag]); 
   if ( strncmp(m_p->cmd[vflag],"   ",3) == 0) wmove(com,1,10);

   /*********************************************************/
   /* Windows, die nur zwei Zustaende haben: HW und Flug:   */
   /*          head und sstatus                             */
   /*********************************************************/
   if (flag > 0 && flag < 10) /* HW */
      {
      /* head */
      stdt=star_date(t1);
      if (party[vflag]==IMPERIAL)
         {
         mvwprintw(head,0,0,"|     -- Imperial HOMEWORLD --    |");
         }
      else
         {
         mvwprintw(head,0,0,"|     -- Federal HOMEWORLD --     |");
         }
      mvwprintw(head,1,0,"| Starfleet Base and Head Quarter |");
      /* sstatus */
      mvwprintw(sstatus,0,10,"-Player-Status-");
      mvwprintw(sstatus,1,3,"Name......:  %-10s       ",
                   m_p->name_of_cmdr[vflag]);
      mvwprintw(sstatus,2,3,"Rank......:  %-9s        ",
                   m_p->rank_of_name[vflag]);
      mvwprintw(sstatus,3,3,"Score.....:  %-7d          ",
		m_p->name_score[vflag]);
      mvwprintw(sstatus,4,3,"Ship......:  %-10s       ",
                   ships[vflag]);
      mvwprintw(sstatus,5,3,"Class.....:  %-10s       ",
                   type_of_ship[vflag]);

      anz_bas=anz_pl=anz_sat=anz_fort=anz_pl1=anz_pl2=anz_pl3=anz_pl4=0;
      for (i=0;i<MAX_BASES;i++)
          {
          if (party[vflag]==m_p->base[i].party) anz_bas++;
          } 
      for (i=0;i<MAX_FORTS;i++)
	  {
          if (party[vflag]==m_p->fort[i].party 
                          &&
              m_p->fort[i].status == ACTIVE ) anz_fort++;
          } 
      for (i=0;i<MAX_PLANETS;i++)
          {
          if (party[vflag]==m_p->planet[i].party) 
             {
             anz_pl++;
             if (m_p->planet[i].level==1) anz_pl1++;
             else if (m_p->planet[i].level==2) anz_pl2++;
             else if (m_p->planet[i].level==3) anz_pl3++;
             else if (m_p->planet[i].level==4) anz_pl4++;
	     }
          } 
      for (i=0;i<MAX_SATS;i++)
          {
          if (party[vflag]==m_p->satellite[i].party) anz_sat++;
          } 
      if (party[vflag]==IMPERIAL)
         {
         mvwprintw(sstatus,6,1, "-------Match-Status-Empire-------"); 
         }
      else
	 {
         mvwprintw(sstatus,6,1, "-----Match-Status-Federation-----"); 
         }
      mvwprintw(sstatus,7,3, "Bases......: %3d Players: %2d/%-2d",anz_bas,m_p->anz_user_f,m_p->anz_user_e);
      mvwprintw(sstatus,8,3, "Planets....: %3d %3d %3d %3d",anz_pl1,anz_pl2,anz_pl3,anz_pl4);
      mvwprintw(sstatus,9,3,"Forts......: %3d            ",anz_fort);
      mvwprintw(sstatus,10,3,"Satellites.: %3d            ",anz_sat);
      mvwprintw(sstatus,11,3,"Score......:   %-7d         ",m_p->score[party[vflag]]);
      if (m_p->isurvey_area[party[vflag]] < 0 )
	{
        mvwprintw(sstatus,12,3,"Area.......:   ? (def?)        ");
        }
      else
        {
	if (m_p->fsurvey_area[party[vflag]] < 10.0)
	   {
           mvwprintw(sstatus,12,3,"Area.......:   %-5d (%2.1f %%)   ",m_p->isurvey_area[party[vflag]],m_p->fsurvey_area[party[vflag]]);
           }
        else
	   {
           mvwprintw(sstatus,12,3,"Area.......:   %-5d (%2.1f %%)  ",m_p->isurvey_area[party[vflag]],m_p->fsurvey_area[party[vflag]]);
           }
        }
      usleep(100000);
      }   
   else /* nicht HW */
      {
      /* head */
      mvwprintw(head,0,0," %-15s  %-14s  ",kind_of_ship[vflag],ships[vflag]);
      mvwprintw(head,1,0," Comm. Off.: %-10s %-9s  ",m_p->rank_of_name[vflag],
                                            m_p->name_of_cmdr[vflag]);

      /* sstatus */
      mvwprintw(sstatus,0,10,"-State-of-Ship-");
      mvwprintw(sstatus,0,10,"----Status-----");
      mvwprintw(sstatus,1,3,"Position.....:     %03d  %03d",m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1]); 
      ip=iproz(m_p->ship[vflag].main_energy,max_m_en[vflag]);
      mvwprintw(sstatus,2,3,"Main Energy..:    %4d  %3d%%",m_p->ship[vflag].main_energy,ip);
      ip=iproz(m_p->ship[vflag].sh_energy,max_sh_en[vflag]);
      mvwprintw(sstatus,3,3,"Shield Energy:    %4d  %3d%%",m_p->ship[vflag].sh_energy,ip);
      if (vflag<25)
         { 
         ip=iproz(m_p->ship[vflag].ph_energy,max_ph_en[vflag]);
         mvwprintw(sstatus,4,3,"Phaser Energy:    %4d  %3d%%",m_p->ship[vflag].ph_energy,ip);
         }
      else
         {
         mvwprintw(sstatus,4,3,"Phaser Energy:    ----  ---");
         }
      ip=iproz(m_p->ship[vflag].fuel,max_fuel[vflag]);         
      mvwprintw(sstatus,5,3,"Fuel (tons)..:     %3d  %3d%%",m_p->ship[vflag].fuel,ip);
      ip=iproz(m_p->ship[vflag].torp,max_torp[vflag]);         
      mvwprintw(sstatus,6,1,"  Torpedoes....:      %2d  %3d%%   ",m_p->ship[vflag].torp,ip);
      ip=iproz(m_p->ship[vflag].mines,max_mines[vflag]);         
      mvwprintw(sstatus,7,1,"  Mines........:      %2d  %3d%%   ",m_p->ship[vflag].mines,ip);
      ip=iproz(m_p->ship[vflag].sat,max_sat[vflag]);         
      mvwprintw(sstatus,8,3,"Satellites...:       %1d  %3d%%",m_p->ship[vflag].sat,ip);
      ip=iproz(m_p->ship[vflag].dam[0],max_dam[vflag]);         
      mvwprintw(sstatus,9,3,"Damages......:    %4d  %3d%%",m_p->ship[vflag].dam[0],ip);
      mvwprintw(sstatus,10,3,"Score........: %7d",m_p->user_score[vflag]);
      stdt=star_date(t1);
      mvwprintw(sstatus,11,3,"Stardate.....:  %06.1f",stdt);            
      if (m_p->ship[vflag].shield == ON)
         {         
	   /*          strcpy(str1,"Shields UP  ");*/
         strcpy(str1,"SH:+ ");
         }
      else
         {
	   /*         strcpy(str1,"Shields DOWN"); */
         strcpy(str1,"SH:- ");
         }
      if (m_p->ship[vflag].ramm==ON)
         {
         strcat(str1,"RA:+ ");
         }
      else
         {
         strcat(str1,"RA:- ");
         }
      if (m_p->ship[vflag].sv==ON)
         {
         strcat(str1,"SV:+ ");
         }
      else
         {
         strcat(str1,"SV:- ");
         }
      if (m_p->ship[vflag].mp==ON)
         {
	 sprintf(str2,"MP:+%03d ",m_p->ship[vflag].imp);
         strcat(str1,str2);
         }
      else
         {
	 if (m_p->ship[vflag].imp==0)
	   {
           strcat(str1,"MP:-    ");
           }
         else
           {
	   sprintf(str2,"MP:-%03d ",m_p->ship[vflag].imp);
           strcat(str1,str2);
           }
         }

      mvwprintw(sstatus,12,3,str1);    
      if (m_p->ship[vflag].build==ON)
         {
         mvwprintw(sstatus,12,26,"B");
         }
      else if (m_p->ship[vflag].dock==DOCKED)
	 {
	 mvwprintw(sstatus,12,26,"D");
         }
      else if (m_p->ship[vflag].jump==ON)
	 {
	 mvwprintw(sstatus,12,26,"J");
         }
      else if (m_p->ship[vflag].move==ON)
	 {
         mvwprintw(sstatus,12,26,"M");
         }
      else
         {
         mvwprintw(sstatus,12,26," ");
         }
      if (m_p->ship[vflag].phaser==ON)
         {
         mvwprintw(sstatus,12,27,"P");
         }
      else
         {
         mvwprintw(sstatus,12,27," ");
         }
      mvwprintw(sstatus,12,28,"TT:");
      for (i=1;i<=anz_ttubes[vflag];i++)
         {
         if (m_p->ship[vflag].torp<=0 || m_p->ship[vflag].status_ttubes[i-1]==EMPTY)
           {
           mvwprintw(sstatus,12,(30+i),"-");
           }
         else if (m_p->ship[vflag].status_ttubes[i-1]==RELOADING)
           {
           mvwprintw(sstatus,12,(30+i),"R");
           }
         else if (m_p->ship[vflag].status_ttubes[i-1]==LOADED) 
           {
           if (m_p->ship[vflag].torp >= i)
             {
             mvwprintw(sstatus,12,(30+i),"+");
             }
           else
	     {
             mvwprintw(sstatus,12,(30+i),"-");
             }
           }
         else 
	   {
           mvwprintw(sstatus,12,(30+i),"E");
           }
         }
      }

   /****************************************************/
   /* Window scan: viele Faelle moeglich               */
   /****************************************************/
   if (flag == 0) /* Docking an Basis */
     {
     nrb=find_nr_base(m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1]);
     strncpy(scan_row[0], "###################    ###################",SCANX);
     strncpy(scan_row[1], "#######                            #######",SCANX);
     strncpy(scan_row[2], "#######                            #######",SCANX);
     sprintf(scan_row[3],"#######----<%c                %c>----#######",m_p->base[nrb].bay[0],m_p->base[nrb].bay[1]);
     strncpy(scan_row[4], "#######                            #######",SCANX);
     strncpy(scan_row[5], "#######                            #######",SCANX);
     sprintf(scan_row[6],"#######----<%c                %c>----#######",m_p->base[nrb].bay[2],m_p->base[nrb].bay[3]);
     strncpy(scan_row[7], "#######                            #######",SCANX);
     strncpy(scan_row[8], "#######                            #######",SCANX);
     strncpy(scan_row[9], "##########################################",SCANX);
     strncpy(scan_row[10], "##########################################",SCANX);
     if (party[vflag]==FEDERAL)
        {
        strncpy(scan_row[11],"#+-----------Federal_Starbase-----------+#",SCANX);
        }
     else
        {
        strncpy(scan_row[11],"#+----------Imperial_Starbase-----------+#",SCANX);
        }
     if (m_p->base[nrb].condition==RED)
       {
       sprintf(scan_row[12],"#| Position: %03d %03d  Condition: RED    |#",m_p->base[nrb].pos[0],m_p->base[nrb].pos[1]);
       }
     else if (m_p->base[nrb].condition==YELLOW)
       {
       sprintf(scan_row[12],"#| Position: %03d %03d  Condition: YELLOW |#",m_p->base[nrb].pos[0],m_p->base[nrb].pos[1]);
       }
     else
       {
       sprintf(scan_row[12],"#| Position: %03d %03d  Condition: Green  |#",m_p->base[nrb].pos[0],m_p->base[nrb].pos[1]);
       }
     if (m_p->base[nrb].sh==ON) sprintf(scan_row[13],"#| Shields : %-4d (%-3d %%) UP            |#",m_p->base[nrb].sh_energy,iproz(m_p->base[nrb].sh_energy,BASE_SH_EN));
     if (m_p->base[nrb].sh==OFF) sprintf(scan_row[13],"#| Shields : %-4d (%-3d %%) DOWN          |#",m_p->base[nrb].sh_energy,iproz(m_p->base[nrb].sh_energy,BASE_SH_EN));
     sprintf(scan_row[14],"#| Damages : %-4d (%-3d %%)               |#",m_p->base[nrb].dam,iproz(m_p->base[nrb].dam,MAX_DAM_BAS)); 
     strncpy(scan_row[15],"#|                                      |#",SCANX);
     strncpy(scan_row[16],"#|                                      |#",SCANX);
     strncpy(scan_row[17],"#|                                      |#",SCANX);
     strncpy(scan_row[18],"#|                                      |#",SCANX);
     strncpy(scan_row[19],"#+--------------------------------------+#",SCANX);
     strncpy(scan_row[20],"##########################################",SCANX);
     }
   else if (flag == 1) /* HW-Dock-Standardbild */
     {
     strncpy(scan_row[0], " #################        ################## ",SCANX);
     strncpy(scan_row[1], " #                                         # ",SCANX);
     strncpy(scan_row[2], " #         #####################           # ",SCANX);
     strncpy(scan_row[3], " #-<                                     >-# ",SCANX);
     strncpy(scan_row[4], " #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[5], " #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[6], " #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[7], " #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[8], " #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[9], " #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[10]," #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[11]," #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[12]," #-<         >-#-<       >-#-<           >-# ",SCANX);
     strncpy(scan_row[13]," ########################################### ",SCANX);

     sprintf(print_str," # Position: %03d %03d  Stardate: %06.1f     # ",m_p->homeworld[party[vflag]].pos[0],m_p->homeworld[party[vflag]].pos[1],stdt);
     strncpy(scan_row[14],print_str,SCANX);
     if (m_p->homeworld[party[vflag]].condition==RED)
       {
       sprintf(print_str," # Condition: RED     Damages:%5d (%2d %%) # ",m_p->homeworld[party[vflag]].dam,iproz(m_p->homeworld[party[vflag]].dam,MAX_DAM_HOMEWORLD));
       }
     else if (m_p->homeworld[party[vflag]].condition==YELLOW)
       {
       sprintf(print_str," # Condition: YELLOW  Damages:%5d (%2d %%) # ",m_p->homeworld[party[vflag]].dam,iproz(m_p->homeworld[party[vflag]].dam,MAX_DAM_HOMEWORLD));
       }
     else
       {
       sprintf(print_str," # Condition: GREEN   Damages:%5d (%2d %%) # ",m_p->homeworld[party[vflag]].dam,iproz(m_p->homeworld[party[vflag]].dam,MAX_DAM_HOMEWORLD));
       }
     strncpy(scan_row[15],print_str,SCANX);
     /* xxx */
     for (i=0;i<MAX_MPATHS;i++)
       {
       if (m_p->imp[party[vflag]][i] < 1 )
         {
         sprintf(print_str," # MP no. %1d: empty                         # ",i);
         }
       else
         {
         sprintf(print_str," # MP no. %1d: length=%03d, end point %03d %03d # ",i,m_p->imp[party[vflag]][i],m_p->mpath[party[vflag]][i][m_p->imp[party[vflag]][i]][0],m_p->mpath[party[vflag]][i][m_p->imp[party[vflag]][i]][1]);
         }
       strncpy(scan_row[16+i],print_str,SCANX);
       }
     for (i=(16+MAX_MPATHS);i<(SCANY-1);i++)
	{
        strncpy(scan_row[i]," #                                         # ",SCANX);
        }
     strncpy(scan_row[SCANY-1]," ########################################### ",SCANX);
     if (m_p->missile[party[vflag]].status==OFF) scan_row[4][12]=MISSILE;
     if (m_p->scout[party[vflag]].status==OFF && party[vflag]==FEDERAL) scan_row[5][12]=F_SCOUT;
     if (m_p->scout[party[vflag]].status==OFF && party[vflag]==IMPERIAL) scan_row[5][12]=E_SCOUT;
     /* Robotschiffe */
     for (i=0;i<MAX_ROBOTSHIPS;i++)
       {
       if (m_p->robotship[party[vflag]][i].status==HW_DOCKED) 
         {
         sprintf(rstr,"%1d",i);
	 scan_row[i+3][40]=rstr[0];
         }
       }
     /* Schiffe */
     ibattle=icruiser=idest=0;
     for (i=5; i<MAX_USER; i++)
       {
       if (party[vflag]==party[i] && m_p->l_exist[i]==ACTIVE && m_p->ship[i].dock==HW_DOCKED && level_of_ship[i]==BATTLESHIP) 
	 {
	 ibattle++;
         scan_row[8+ibattle][12]=ships[i][0];
         }
       else if (party[vflag]==party[i] && m_p->l_exist[i]==ACTIVE && m_p->ship[i].dock==HW_DOCKED && level_of_ship[i]==CRUISER)
	 {
	 icruiser++;
         scan_row[6+icruiser][18]=ships[i][0];
         }
       else if (party[vflag]==party[i] && m_p->l_exist[i]==ACTIVE && m_p->ship[i].dock==HW_DOCKED && level_of_ship[i]==DESTROYER)
 	 {
	 idest++;
         scan_row[2+idest][4]=ships[i][0];
         }
       }
     }
   else if (flag == 2) /* HW: Flottenstatus */
     {
     if (party[vflag]==FEDERAL)
       {
       strcpy(scan_row[0], "    Overview of the Federal star fleet    ");
       }
     else
       {
       strcpy(scan_row[0], "    Overview of the Imperial star fleet   ");
       }
     n=0;
     for (i=5;i<MAX_USER;i++) /* Beginn bei 5 da 1-4 Traegerschiffe */
	 {                       /* und noch nicht implementiert! */
         if (party[vflag]==party[i])
	    {
            n++;
	    if (m_p->luser[i]==ACTIVE) 
	       {
               sprintf(scan_row[n],"%-10s %-11s - %-9s %-9s",
                                    type_of_ship[i],
                                    ships[i],m_p->rank_of_name[i],
                                    m_p->name_of_cmdr[i]);
               }
            else if (m_p->l_exist[i]==DESTROYED)
               {
               sprintf(scan_row[n],"%-10s %-11s - destroyed        ",
                                       type_of_ship[i],ships[i]);
	       }
            else
	       {
               sprintf(scan_row[n],"%-10s %-11s - operational      ",
                                    type_of_ship[i],ships[i]);
               }
	    }
         } /* das sollten jetzt 21 Zeilen sein! */
     }
   else if (flag == 3 || flag == 10 || flag == 0)
     /* Radarbild, 3: bei HW-Docking, 0: Docking an Basis */
     {
      if ( m_p->ship[vflag].pos[0] < 9 )
         {
         startx=0;
         anzx=m_p->ship[vflag].pos[0]+SHIP_RANGE;
         }
      else 
         {
         startx=m_p->ship[vflag].pos[0]-SHIP_RANGE;
         anzx=19;
         }
      if ( m_p->ship[vflag].pos[0] > (ARRAY_SIZE-10) )
         {
         endx=(ARRAY_SIZE-1);
         anzx=(ARRAY_SIZE-1)-m_p->ship[vflag].pos[0]+SHIP_RANGE+1;
         }
      else
         {
         endx=m_p->ship[vflag].pos[0]+SHIP_RANGE;
         }
   
      if ( m_p->ship[vflag].pos[1] < SHIP_RANGE )
         {
         starty=0;
         }
      else
         {
         starty=m_p->ship[vflag].pos[1]-SHIP_RANGE;
         }
     if ( m_p->ship[vflag].pos[1] > (ARRAY_SIZE-SHIP_RANGE-1) )
        {
        endy=(ARRAY_SIZE-1);
        } 
     else
        {
        endy=m_p->ship[vflag].pos[1]+SHIP_RANGE;
        }
     if (expl_flag==EXPLODING) /* Feld EXPL besetzen */
        {
        for (i=0;i<SCANY;i++)   
            {
            for (j=0;j<SCANX;j++)
                {
                expl[i][j]=N_EXPLODING;
                }
            }
        expl_flag=N_EXPLODING;  
        }
     for (n=0;n<SCANY;++n)
         {
	 strncpy(scan_row[n],"                                             ",SCANX); 
         }
     strcpy(s0dummy,"  "); 
     for (k=startx;k<=startx+anzx;k=k+2) /* Kopfzeile mit x-Werten */
         {
         sprintf(nr_of_col," %03d",k);
         strcat(s0dummy,nr_of_col);
         }
     strcat(s0dummy,"   ");
     strncpy(scan_row[0],s0dummy,SCANX);
     n=0;
     for (i=endy;i>=starty;--i) /* y-Wert (Zeilenzahl) fuer Scanzeile setzen */
        {  	
        ++n;
        sprintf(nr_of_row,"%03d",i);
        strncpy(scan_row[n],nr_of_row,SCANX);
        }
     anz_row=0;
     for (i=endy;i>=starty;--i)
         {
         n=0;
         ++anz_row;
         line[0]=' ';
         for (j=startx;j<=endx;++j)
             {
             a1=m_p->a[j][i];
             if (m_p->radarlee==ON)
                {
                if (behind(j,i,m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1],SHIP_RANGE)==1) a1=' '; 
                }
             if ((a1==E_SCOUT && m_p->scout[IMPERIAL].dam < MAX_DAM_SCOUT/2)
                                    || 
                 (a1==F_SCOUT && m_p->scout[FEDERAL].dam < MAX_DAM_SCOUT/2)) a1=HOOD;  /* Scout als HOOD sichtbar wenn wenig Schaeden, missile ebenfalls */
             if (a1==MISSILE && m_p->missile[FEDERAL].pos[0]==j &&
                 m_p->missile[FEDERAL].pos[1]==i && 
                 m_p->missile[FEDERAL].dam < MAX_DAM_MISSILE/2) a1=HOOD;
             if (a1==MISSILE && m_p->missile[IMPERIAL].pos[0]==j &&
                 m_p->missile[IMPERIAL].pos[1]==i && 
                 m_p->missile[IMPERIAL].dam < MAX_DAM_MISSILE/2) a1=HOOD;
             if (a1==E_HOMEWORLD)
	       {
               line[n]=']';
               ++n;
               line[n]='E';
               if (n<36) line[n+1]='[';
               ++n;
               }
             else if (a1==F_HOMEWORLD)
	       {
               line[n]=')';
               ++n;
               line[n]='F';
               if (n<36) line[n+1]='(';
               ++n;
               }
             else
               {
               if (line[n]!='(' && line[n]!='[') line[n]=' ';
               if (a1==E_BASE) line[n]='[';
               if (a1==F_BASE) line[n]='(';
               if (a1==EXPLOSION)
                  {
                  expl_flag=EXPLODING;
                  a1=BLIND;
                  expl[(anz_row-1)][n]=EXPLODING;
                  }  
               ++n;       
               line[n]=a1;
	       line[n+1]='\0';
               ++n;       
               }
             }
         strncat(scan_row[anz_row],line,SCANX);
         }
     n=0;
     for (i=endy;i>=starty;--i) /* y-Wert (Zeilenzahl) Ende Scanzeile */
        {  	
        ++n;
        sprintf(nr_of_row,"%03d",i);
        strcat(scan_row[n],nr_of_row);
        } 
     strncpy(scan_row[n+1],scan_row[0],SCANX);
     for (k=n+2;k<SCANY;k++)
       {
       strncpy(scan_row[k],"                                                          ",SCANX);
       }
  /* Explosionsflag auswerten */
     if (expl_flag==EXPLODING)
        {
        for (i=0;i<SCANY;i++)
         {
         for (j=0;j<SCANX;j++)
             {
             if (expl[i][j]==EXPLODING)
                {
		if (((i+2) < SCANY) && ((j+3) < SCANX)) scan_row[i+2][j+3]=BLIND;
                if (((i+2) < SCANY) && ((j+5) < SCANX)) scan_row[i+2][j+5]=BLIND;
                if (((j+3) < SCANX)) scan_row[i][j+3]=BLIND;
                if (((j+5) < SCANX)) scan_row[i][j+5]=BLIND;
                }
             }
          }
        }
     for (i=0;i<SCANY;i++) 
     /* Letztes Zeichen auf ' ' weil bei Explosion am Rand dort ein + 
        auftauchte. Bessere Loesung: line oben besser fuellen */
        {
        scan_row[i][(SCANX-1)]=' ';
        }
     }
   else if (flag == 4 || flag == 12) /* Scout-Display (HW und Flug) */ 
     {
     stdt=star_date(t1);
     sprintf(scan_row[0],"SCOUT: energy: %4d destination: %3d %3d   ",
                     m_p->scout[party[vflag]].energy,
                     m_p->scout[party[vflag]].target[0],
                     m_p->scout[party[vflag]].target[1]);
     /* Radarbild des Scouts */
     xscout=m_p->scout[party[vflag]].pos[0];
     yscout=m_p->scout[party[vflag]].pos[1];
     if ( xscout < SHIP_RANGE )
        {
        startx=0;
        anzx=xscout+SHIP_RANGE;
        }
     else 
        {
        startx=xscout-SHIP_RANGE;
        anzx=19;
        }
     if ( xscout > (ARRAY_SIZE-SHIP_RANGE-1) )
       {
       endx=(ARRAY_SIZE-1);
       anzx=(ARRAY_SIZE-1)-xscout+SHIP_RANGE+1;
       }
     else
       {
       endx=xscout+SHIP_RANGE;
       }
     if (yscout < SHIP_RANGE )
       {
       starty=0;
       }
     else
       {
       starty=yscout-SHIP_RANGE;
       }
     if ( yscout > (ARRAY_SIZE-SHIP_RANGE-1) )
       { 
       endy=(ARRAY_SIZE-1);
       } 
     else
       {
       endy=yscout+SHIP_RANGE;
       }
     if (expl_flag==EXPLODING) /* Feld EXPL besetzen */
       {
       for (i=0;i<SCANY;i++)   
         {
         for (j=0;j<SCANX;j++)
          {
          expl[i][j]=N_EXPLODING;
          }
         }
       expl_flag=N_EXPLODING;  
       }
     n=0;
     for (i=endy;i>=starty;--i) /* y-Wert (Zeilenzahl) fuer Scanzeile setzen */
        {  	
        ++n;
	/*        sprintf(nr_of_row,"%03d\0",i); */
		  sprintf(nr_of_row,"%03d",i); 
        strcpy(scan_row[n],nr_of_row);
        }
     anz_row=0;
     for (i=endy;i>=starty;--i)
       {
       n=0;
       ++anz_row;
       line[0]=' ';
       for (j=startx;j<=endx;++j)
          {
          a1=m_p->a[j][i];
          if (m_p->radarlee==ON)
             {
             if (behind(j,i,m_p->scout[party[vflag]].pos[0],m_p->scout[party[vflag]].pos[1],SHIP_RANGE)==1) a1=' '; 
             }
          if (a1==E_HOMEWORLD)
            {
            line[n]=']';
            ++n;
            line[n]='E';
            if (n<36) line[n+1]='[';
            ++n;
	    line[n+1]='\0';
            }
          else if (a1==F_HOMEWORLD)
            {
            line[n]=')';
            ++n;
            line[n]='F';
            if (n<36) line[n+1]='(';
            ++n;
	    line[n+1]='\0';
            }
          else
            {
            if (line[n]!='(' && line[n]!='[') line[n]=' ';
            if (a1==E_BASE) line[n]='[';
            if (a1==F_BASE) line[n]='(';
            if (a1==EXPLOSION)
               {
               expl_flag=EXPLODING;
               a1=BLIND;
               expl[(anz_row-1)][n]=EXPLODING;
               }  
            ++n;       
            line[n]=a1;
	    line[n+1]='\0';
            ++n;       
            }
         }
       strcat(scan_row[anz_row],line);
       }
    n=0;
    for (i=endy;i>=starty;--i) /* y-Wert (Zeilenzahl) Ende Scanzeile */
      {  	
      ++n;
      sprintf(nr_of_row,"%03d",i);
      /*      sprintf(nr_of_row,"%03d\0",i); */
      strcat(scan_row[n],nr_of_row);
      } 
    strcpy(scan_row[n+1],"  ");
    for (k=startx;k<=startx+anzx;k=k+2) /* Fusszeile mit x-Werten,n von oben */
      {
      sprintf(nr_of_col,"%4d",k);
      if (nr_of_col[1]==' ') nr_of_col[1]='0';
      if (nr_of_col[2]==' ') nr_of_col[2]='0';
      strcat(scan_row[n+1],nr_of_col); 
      }
  /* Explosionsflag auswerten */
    if (expl_flag==EXPLODING)
      {
      for (i=0;i<SCANY;i++)
       {
       for (j=0;j<SCANX;j++)
          {
          if (expl[i][j]==EXPLODING)
             {
             scan_row[i+2][j+3]=BLIND;
             scan_row[i+2][j+5]=BLIND;
             scan_row[i][j+3]=BLIND;
             scan_row[i][j+5]=BLIND;
             }
          }
       }
      }
     }
   else if (flag == 5 || flag == 13) /* Recon-Daten Flug und HW */
     {
     n=0;
     sprintf(scan_row[n],"%-s",m_p->recon_str[0]); /* Romul. */
     for (i=5;i<MAX_USER;i++)
       {
       if (party[vflag]!=party[i])
	  {
	  n++;
	  sprintf(scan_row[n],"%-s",m_p->recon_str[i]);
	  }
       }
     }
   else if (flag == 6) /* HW: Basen, Planeten, Forts, Satelliten */
     {
     n=j=0;
     strcpy(scan_row[n],"__________________Star_Bases_________________");
     for (i=0;i<MAX_BASES;i++)
	 {                  
         if (party[vflag]==m_p->base[i].party && n < 20)
	    {
            if (j < 1)
	      {
              sprintf(strb1,"%03d %03d S=%-4d D=%-4d  ",
                                                    m_p->base[i].pos[0],
		                                    m_p->base[i].pos[1],
                                                    m_p->base[i].sh_energy,
                                                    m_p->base[i].dam);
   	      j++;
              }
            else
              {
	      sprintf(strb2," %03d %03d S=%-4d D=%-4d",
                                                    m_p->base[i].pos[0],
		                                    m_p->base[i].pos[1],
                                                    m_p->base[i].sh_energy,
                                                    m_p->base[i].dam); 
              j=0;
              n++;
              strcpy(scan_row[n],strb1);
	      strcat(scan_row[n],strb2);
              }
           }
         }
         if (j==1 && n < 20) 
         {
         n++;
         strcpy(scan_row[n],strb1);
         strcat(scan_row[n],"                      ");
         }
 
     k=0;
     if (n < 16)
       {
       n++;
       strcpy(scan_row[n],"______________Occupied_Planets_______________");
       for (i=0;i<MAX_PLANETS;i++) 
	 {                  
         if (party[vflag]==m_p->planet[i].party && n < 20)
	    {
	    k++;
            if (k==1)
	       {
               sprintf(strb1,"%03d %03d L=%1d D=%-4d     ",m_p->planet[i].pos[0],
                                                    m_p->planet[i].pos[1],
                                                    m_p->planet[i].level,
                                                    m_p->planet[i].dam);
	       }
            else
               { 
               sprintf(strb2," %03d %03d L=%1d D=%-4d   ",m_p->planet[i].pos[0],
                                                    m_p->planet[i].pos[1],
                                                    m_p->planet[i].level,
                                                    m_p->planet[i].dam);
	       k=0;
               n++;
               strcpy(scan_row[n],strb1);
	       strcat(scan_row[n],strb2); 
	       }
	    }
	 }
       }
     if (k==1 && n < 20) 
       {
       n++;
       strcpy(scan_row[n],strb1);
       strcat(scan_row[n],"                      ");
       }
     /* Forts */
     strncpy(strf1,"                  ",(SCANX/3)+1);
     strncpy(strf2,"                  ",(SCANX/3)+1);
     strncpy(strf3,"                  ",(SCANX/3)+2);
     if (n < 16)
       {
       n++;
       k=0;
       strncpy(scan_row[n],"___________________Forts_____________________",SCANX);
       for (i=0;i<MAX_FORTS;i++) 
	 {                  
         if (party[vflag]==m_p->fort[i].party && n < 20)
	    {
	    k++;
            if (k==1) sprintf(strf1,"%03d %03d D=%-4d",m_p->fort[i].pos[0],m_p->fort[i].pos[1],m_p->fort[i].dam);
            if (k==2) sprintf(strf2," %03d %03d D=%-4d",m_p->fort[i].pos[0],m_p->fort[i].pos[1],m_p->fort[i].dam);
	    if (k==3) 
	       {
               sprintf(strf3," %03d %03d D=%-4d ",m_p->fort[i].pos[0],m_p->fort[i].pos[1],m_p->fort[i].dam);
	       k=0;
               n++;
               strncpy(scan_row[n],strf1,(SCANX/3)+1);
	       strncat(scan_row[n],strf2,(SCANX/3)+1); 
	       strncat(scan_row[n],strf3,(SCANX/3)+2);
	       }
	    }
	 }
      }
     
     if (k==2 && n < 20) 
       {
       n++;
       strncpy(scan_row[n],strf1,(SCANX/3)+1);
       strncat(scan_row[n],strf2,(SCANX/3)+1);
       strncat(scan_row[n],"                 ",(SCANX/3)+2);
       }
     if (k==1 && n < 20) 
       {
       n++;
       strncpy(scan_row[n],strf1,SCANX/3);
       strncat(scan_row[n],"                                  ",2*(SCANX/3)+2);
       } 

     /* Sprungtore */
     if (n < 16)
       {
       n++;
       k=0;
       strncpy(scan_row[n],"_________________Jumpgates___________________",SCANX);
       for (i=0;i<MAX_JUMPGATES;i++) 
	 {                  
         if (party[vflag]==m_p->jumpgate[i].party && n < 20)
	    {
	    k++;
            if (k==1) 
               {
               sprintf(strb1,"%03d %03d C=%2d D=%-4d    ",m_p->jumpgate[i].pos[0],m_p->jumpgate[i].pos[1],m_p->jumpgate[i].code,m_p->jumpgate[i].dam); 
               }
            else
	       {
	       sprintf(strb2," %03d %03d C=%2d D=%-4d  ",m_p->jumpgate[i].pos[0],m_p->jumpgate[i].pos[1],m_p->jumpgate[i].code,m_p->jumpgate[i].dam);
	       k=0;
               n++;
               strcpy(scan_row[n],strb1);
	       strcat(scan_row[n],strb2); 
	       }
	    }
	 }
       }
     if (k==1 && n < 20) 
       {
       n++;
       strcpy(scan_row[n],strb1);
       strcat(scan_row[n],"                      ");
       }

     if (n < 18)
       {
       n++;
       k=0;
       strncpy(scan_row[n],"_________________Satellites__________________",SCANX);
       for (i=0;i<MAX_SATS;i++) 
	 {                  
         if (party[vflag]==m_p->satellite[i].party && n < 20)
	    {
	    k++;
            if (k==1) sprintf(strs1,"%03d %03d     ",m_p->satellite[i].pos[0],m_p->satellite[i].pos[1]);
            if (k==2) sprintf(strs2,"%03d %03d     ",m_p->satellite[i].pos[0],m_p->satellite[i].pos[1]);
            if (k==3) sprintf(strs3,"%03d %03d    ",m_p->satellite[i].pos[0],m_p->satellite[i].pos[1]);
            if (k==4) 
               {
               sprintf(strs4,"%03d %03d   ",m_p->satellite[i].pos[0],m_p->satellite[i].pos[1]);
	       k=0;
               n++;
               strcpy(scan_row[n],strs1);
	       strcat(scan_row[n],strs2); 
	       strcat(scan_row[n],strs3); 
	       strcat(scan_row[n],strs4);
	       }
	    }
	 }
        if (k==3 && n < 20) 
          {
          n++;
          strcpy(scan_row[n],strs1);
          strcat(scan_row[n],strs2); 
          strcat(scan_row[n],strs3); 
          strcat(scan_row[n],"          ");
          }
        if (k==2 && n < 20) 
          {
          n++;
          strcpy(scan_row[n],strs1);
          strcat(scan_row[n],strs2); 
          strcat(scan_row[n],"                     ");
          }
        if (k==1 && n < 20) 
          {
          n++;
          strcpy(scan_row[n],strs1);
          strcat(scan_row[n],"                                 ");
          }
       }
     for (i=n+1;i<SCANY;i++)
       {
	 strncpy(scan_row[i],"                                             ",SCANX);
       }
     }
   else if (flag == 7)
     {
     for (j=0;j<SCANY;j++)
       {
       if (j<MAP_SIZE)
	 {
	 k=0;
	 for (i=0;i<SCANX-MAP_SIZE;i++)
	   {
	   if (i<MAP_SIZE)
             {
	     line[k]=m_p->map[party[vflag]][i][j]; 
	     line[k+1]=' ';
             k++;
             k++;
             }
           else
             {
	     line[k]='#'; k++;
             }
           }
         strcpy(scan_row[j],line);
	 }
       else
         {
         strcpy(scan_row[j],"#############################################");
         } 
       }
     }
   else if (flag == 8) /* Robotschiffe */
     {
     strcpy(scan_row[0],"_________________Robotships_________________");
     j=1;
     for (i=0;i<MAX_ROBOTSHIPS;i++) 
	 {
	 if (m_p->robotship[party[vflag]][i].status==DESTROYED) 
            {
            sprintf(scan_row[j],"RS %1d: DESTROYED                              ",i);
            }
         else if (m_p->robotship[party[vflag]][i].status==HW_DOCKED)
            {
            sprintf(scan_row[j],"RS %1d: docked at Homeworld, operational       ",i);
            }
         else if (m_p->robotship[party[vflag]][i].status==UNDER_CONSTRUCTION)
            {
            sprintf(scan_row[j],"RS %1d: under construction                     ",i);
            }
	 else if (m_p->robotship[party[vflag]][i].status==ACTIVE) 
            {
            sprintf(scan_row[j],"RS %1d: active, current position: %3d %3d       ",i,m_p->robotship[party[vflag]][i].pos[0],m_p->robotship[party[vflag]][i].pos[1]);
	    j++;
	    sprintf(scan_row[j],"       D=%04d E=%04d SE=%04d F=%03d T=%02d        ",m_p->robotship[party[vflag]][i].dam,m_p->robotship[party[vflag]][i].main_energy,m_p->robotship[party[vflag]][i].sh_energy,m_p->robotship[party[vflag]][i].fuel,m_p->robotship[party[vflag]][i].torp);
	    }
	 else if (m_p->robotship[party[vflag]][i].status==WAITING) 
            {
            sprintf(scan_row[j],"RS %1d: waiting, current position: %3d %3d       ",i,m_p->robotship[party[vflag]][i].pos[0],m_p->robotship[party[vflag]][i].pos[1]);
	    j++;
	    sprintf(scan_row[j],"       D=%04d E=%04d SE=%04d F=%03d T=%02d        ",m_p->robotship[party[vflag]][i].dam,m_p->robotship[party[vflag]][i].main_energy,m_p->robotship[party[vflag]][i].sh_energy,m_p->robotship[party[vflag]][i].fuel,m_p->robotship[party[vflag]][i].torp);
	    }
         else
           {
           sprintf(scan_row[j],"RS ???");
           }
         j++;
	 }
     strcpy(scan_row[j],"--------------------------------------------");
     for (i=j+1;i<=21;i++) 
	 {
         strcpy(scan_row[i],"                                            ");
         }
     }
   else if (flag == 11) /* Radarbild, aber Radar defekt */
     {
     strcpy(scan_row[0], "                                          ");
     strcpy(scan_row[1], "                                          ");
     strcpy(scan_row[2], "                                          ");
     strcpy(scan_row[3], "                                          ");
     strcpy(scan_row[4], "                                          ");
     strcpy(scan_row[5], "                                          ");
     strcpy(scan_row[6], "                                          ");
     strcpy(scan_row[7], "                                          ");
     strcpy(scan_row[8], "          RADAR OUT OF ORDER              ");
     strcpy(scan_row[9], "                                          ");
     strcpy(scan_row[10],"                                          ");
     strcpy(scan_row[11],"                                          ");
     strcpy(scan_row[12],"                                          ");
     strcpy(scan_row[13],"                                          ");
     strcpy(scan_row[14],"                                          ");
     strcpy(scan_row[15],"                                          ");
     strcpy(scan_row[16],"                                          ");
     strcpy(scan_row[17],"                                          ");
     strcpy(scan_row[18],"                                          ");
     strcpy(scan_row[19],"                                          ");
     strcpy(scan_row[20],"                                          ");
     } 
   else if (flag == 14) /* Capturing planet */
     {
     strcpy(scan_row[0], "                        *                 ");      
     strcpy(scan_row[1], "  *                    * *             *  ");
     strcpy(scan_row[2], "          *                               ");
     strcpy(scan_row[3], "                               *          ");
     strcpy(scan_row[4], "             *                            ");
     strcpy(scan_row[5], "                         @@@@@@@@@@@@     ");
     strcpy(scan_row[6], "    *     *         @@@@@@@@%@@@@         ");
     strcpy(scan_row[7], "               @@@@@@@@@@@@%@%%           ");
     strcpy(scan_row[8], "           @@@@@@@@@%@@@@@@%@             ");
     strcpy(scan_row[9], "  *      @@@@@@%%@@%@%@@@@@               ");
     strcpy(scan_row[10],"        @@%%@@@@@@@@%@@@@                 ");
     strcpy(scan_row[11],"     @@@@%@@%@@@@@@%@@@@                  ");
     strcpy(scan_row[12],"   @@@@@@@%%@@@@%%%@@@                    ");
     strcpy(scan_row[13],"  @@@@@@@@@@@@@%@@@%                      ");
     strcpy(scan_row[14]," @@@%%%@@@@@@@%@@@@                       ");
     strcpy(scan_row[15],"@@@%@@@%@@@@@@@%%%                        ");
     strcpy(scan_row[16],"@@@@%%%@@@@@@@@@@                         ");
     strcpy(scan_row[17],"@@@@@@@@@@@@@%%%                          ");
     strcpy(scan_row[18],"%%@@@@@@@@@@%@@@                          ");
     strcpy(scan_row[19],"@@%@@@@@@@@%@@@                           ");
     strcpy(scan_row[20],"@@@@@@@@@@@@@@@                           ");
     } 
   else if (flag == 15) /* Treffer im Flug */
     {
     strcpy(scan_row[0], "******************************************");
     strcpy(scan_row[1], "******************************************");
     strcpy(scan_row[2], "******************************************");
     strcpy(scan_row[3], "******************************************");
     strcpy(scan_row[4], "******************************************");
     strcpy(scan_row[5], "******************************************");
     strcpy(scan_row[6], "******************************************");
     strcpy(scan_row[7], "******************************************");
     strcpy(scan_row[8], "******************************************");
     strcpy(scan_row[9], "******************************************");
     strcpy(scan_row[10],"******************************************");
     strcpy(scan_row[11],"******************************************");
     strcpy(scan_row[12],"******************************************");
     strcpy(scan_row[13],"******************************************");
     strcpy(scan_row[14],"******************************************");
     strcpy(scan_row[15],"******************************************");
     strcpy(scan_row[16],"******************************************");
     strcpy(scan_row[17],"******************************************");
     strcpy(scan_row[18],"******************************************");
     strcpy(scan_row[19],"******************************************");
     strcpy(scan_row[20],"******************************************");
     } 
   else if (flag == 16) /* Sprung */
     {
     if (jcnt==0)
       {
       strcpy(scan_row[0], "                                            ");
       strcpy(scan_row[1], "                                            ");
       strcpy(scan_row[2], "                                            ");
       strcpy(scan_row[3], "                                            ");
       strcpy(scan_row[4], "                                            ");
       strcpy(scan_row[5], "                                            ");
       strcpy(scan_row[6], "                                            ");
       strcpy(scan_row[7], "                                            ");
       strcpy(scan_row[8], "                                            ");
       strcpy(scan_row[9], "                      ###                   ");
       strcpy(scan_row[10],"                      # #                   ");
       strcpy(scan_row[11],"                      ###                   ");
       strcpy(scan_row[12],"                                            ");
       strcpy(scan_row[13],"                                            ");
       strcpy(scan_row[14],"                                            ");
       strcpy(scan_row[15],"                                            ");
       strcpy(scan_row[16],"                                            ");
       strcpy(scan_row[17],"                                            ");
       strcpy(scan_row[18],"                                            ");
       strcpy(scan_row[19],"                                            ");
       strcpy(scan_row[20],"                                            ");
       }
     else if (jcnt==1)
       {
       strcpy(scan_row[0], "                                            ");
       strcpy(scan_row[1], "                                            ");
       strcpy(scan_row[2], "                                            ");
       strcpy(scan_row[3], "                                            ");
       strcpy(scan_row[4], "                                            ");
       strcpy(scan_row[5], "                                            ");
       strcpy(scan_row[6], "                                            ");
       strcpy(scan_row[7], "                   #########                ");
       strcpy(scan_row[8], "                   #       #                ");
       strcpy(scan_row[9], "                   #  ###  #                ");
       strcpy(scan_row[10],"                   #  # #  #                ");
       strcpy(scan_row[11],"                   #  ###  #                ");
       strcpy(scan_row[12],"                   #       #                ");
       strcpy(scan_row[13],"                   #########                ");
       strcpy(scan_row[14],"                                            ");
       strcpy(scan_row[15],"                                            ");
       strcpy(scan_row[16],"                                            ");
       strcpy(scan_row[17],"                                            ");
       strcpy(scan_row[18],"                                            ");
       strcpy(scan_row[19],"                                            ");
       strcpy(scan_row[20],"                                            ");
       }
     else if (jcnt==2)
       {
       strcpy(scan_row[0], "                                            ");
       strcpy(scan_row[1], "                                            ");
       strcpy(scan_row[2], "                                            ");
       strcpy(scan_row[3], "                                            ");
       strcpy(scan_row[4], "                                            ");
       strcpy(scan_row[5], "                ###############             ");
       strcpy(scan_row[6], "                #             #             ");
       strcpy(scan_row[7], "                #  #########  #             ");
       strcpy(scan_row[8], "                #  #       #  #             ");
       strcpy(scan_row[9], "                #  #  ###  #  #             ");
       strcpy(scan_row[10],"                #  #  # #  #  #             ");
       strcpy(scan_row[11],"                #  #  ###  #  #             ");
       strcpy(scan_row[12],"                #  #       #  #             ");
       strcpy(scan_row[13],"                #  #########  #             ");
       strcpy(scan_row[14],"                #             #             ");
       strcpy(scan_row[15],"                ###############             ");
       strcpy(scan_row[16],"                                            ");
       strcpy(scan_row[17],"                                            ");
       strcpy(scan_row[18],"                                            ");
       strcpy(scan_row[19],"                                            ");
       strcpy(scan_row[20],"                                            ");
       }
     else if (jcnt==3)
       {
       strcpy(scan_row[0], "                                            ");
       strcpy(scan_row[1], "                                            ");
       strcpy(scan_row[2], "                                            ");
       strcpy(scan_row[3], "            #######################         ");
       strcpy(scan_row[4], "            #                     #         ");
       strcpy(scan_row[5], "            #   ###############   #         ");
       strcpy(scan_row[6], "            #   #             #   #         ");
       strcpy(scan_row[7], "            #   #  #########  #   #         ");
       strcpy(scan_row[8], "            #   #  #       #  #   #         ");
       strcpy(scan_row[9], "            #   #  #  ###  #  #   #         ");
       strcpy(scan_row[10],"            #   #  #  # #  #  #   #         ");
       strcpy(scan_row[11],"            #   #  #  ###  #  #   #         ");
       strcpy(scan_row[12],"            #   #  #       #  #   #         ");
       strcpy(scan_row[13],"            #   #  #########  #   #         ");
       strcpy(scan_row[14],"            #   #             #   #         ");
       strcpy(scan_row[15],"            #   ###############   #         ");
       strcpy(scan_row[16],"            #                     #         ");
       strcpy(scan_row[17],"            #######################         ");
       strcpy(scan_row[18],"                                            ");
       strcpy(scan_row[19],"                                            ");
       strcpy(scan_row[20],"                                            ");
       }
     else if (jcnt==4)
       {
       strcpy(scan_row[0], "                                            ");
       strcpy(scan_row[1], "        ###############################     ");
       strcpy(scan_row[2], "        #                             #     ");
       strcpy(scan_row[3], "        #   #######################   #     ");
       strcpy(scan_row[4], "        #   #                     #   #     ");
       strcpy(scan_row[5], "        #   #   ###############   #   #     ");
       strcpy(scan_row[6], "        #   #   #             #   #   #     ");
       strcpy(scan_row[7], "        #   #   #  #########  #   #   #     ");
       strcpy(scan_row[8], "        #   #   #  #       #  #   #   #     ");
       strcpy(scan_row[9], "        #   #   #  #  ###  #  #   #   #     ");
       strcpy(scan_row[10],"        #   #   #  #  # #  #  #   #   #     ");
       strcpy(scan_row[11],"        #   #   #  #  ###  #  #   #   #     ");
       strcpy(scan_row[12],"        #   #   #  #       #  #   #   #     ");
       strcpy(scan_row[13],"        #   #   #  #########  #   #   #     ");
       strcpy(scan_row[14],"        #   #   #             #   #   #     ");
       strcpy(scan_row[15],"        #   #   ###############   #   #     ");
       strcpy(scan_row[16],"        #   #                     #   #     ");
       strcpy(scan_row[17],"        #   #######################   #     ");
       strcpy(scan_row[18],"        #                             #     ");
       strcpy(scan_row[19],"        ###############################     ");
       strcpy(scan_row[20],"                                            ");
       }
     else if (jcnt==5)
       {
       strcpy(scan_row[0], "    #                                     # ");
       strcpy(scan_row[1], "    #   ###############################   # ");
       strcpy(scan_row[2], "    #   #                             #   # ");
       strcpy(scan_row[3], "    #   #   #######################   #   # ");
       strcpy(scan_row[4], "    #   #   #                     #   #   # ");
       strcpy(scan_row[5], "    #   #   #   ###############   #   #   # ");
       strcpy(scan_row[6], "    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[7], "    #   #   #   #  #########  #   #   #   # ");
       strcpy(scan_row[8], "    #   #   #   #  #       #  #   #   #   # ");
       strcpy(scan_row[9], "    #   #   #   #  #  ###  #  #   #   #   # ");
       strcpy(scan_row[10],"    #   #   #   #  #  # #  #  #   #   #   # ");
       strcpy(scan_row[11],"    #   #   #   #  #  ###  #  #   #   #   # ");
       strcpy(scan_row[12],"    #   #   #   #  #       #  #   #   #   # ");
       strcpy(scan_row[13],"    #   #   #   #  #########  #   #   #   # ");
       strcpy(scan_row[14],"    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[15],"    #   #   #   ###############   #   #   # ");
       strcpy(scan_row[16],"    #   #   #                     #   #   # ");
       strcpy(scan_row[17],"    #   #   #######################   #   # ");
       strcpy(scan_row[18],"    #   #                             #   # ");
       strcpy(scan_row[19],"    #   ###############################   # ");
       strcpy(scan_row[20],"    #                                     # ");
       }
     else if (jcnt==6)
       {
       strcpy(scan_row[0], "    #                                     # ");
       strcpy(scan_row[1], "    #   ###############################   # ");
       strcpy(scan_row[2], "    #   #                             #   # ");
       strcpy(scan_row[3], "    #   #   #######################   #   # ");
       strcpy(scan_row[4], "    #   #   #                     #   #   # ");
       strcpy(scan_row[5], "    #   #   #   ###############   #   #   # ");
       strcpy(scan_row[6], "    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[7], "    #   #   #   #  #########  #   #   #   # ");
       strcpy(scan_row[8], "    #   #   #   #  #       #  #   #   #   # ");
       strcpy(scan_row[9], "    #   #   #   #  #       #  #   #   #   # ");
       strcpy(scan_row[10],"    #   #   #   #  #       #  #   #   #   # ");
       strcpy(scan_row[11],"    #   #   #   #  #       #  #   #   #   # ");
       strcpy(scan_row[12],"    #   #   #   #  #       #  #   #   #   # ");
       strcpy(scan_row[13],"    #   #   #   #  #########  #   #   #   # ");
       strcpy(scan_row[14],"    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[15],"    #   #   #   ###############   #   #   # ");
       strcpy(scan_row[16],"    #   #   #                     #   #   # ");
       strcpy(scan_row[17],"    #   #   #######################   #   # ");
       strcpy(scan_row[18],"    #   #                             #   # ");
       strcpy(scan_row[19],"    #   ###############################   # ");
       strcpy(scan_row[20],"    #                                     # ");
       }
     else if (jcnt==7)
       {
       strcpy(scan_row[0], "    #                                     # ");
       strcpy(scan_row[1], "    #   ###############################   # ");
       strcpy(scan_row[2], "    #   #                             #   # ");
       strcpy(scan_row[3], "    #   #   #######################   #   # ");
       strcpy(scan_row[4], "    #   #   #                     #   #   # ");
       strcpy(scan_row[5], "    #   #   #   ###############   #   #   # ");
       strcpy(scan_row[6], "    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[7], "    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[8], "    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[9], "    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[10],"    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[11],"    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[12],"    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[13],"    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[14],"    #   #   #   #             #   #   #   # ");
       strcpy(scan_row[15],"    #   #   #   ###############   #   #   # ");
       strcpy(scan_row[16],"    #   #   #                     #   #   # ");
       strcpy(scan_row[17],"    #   #   #######################   #   # ");
       strcpy(scan_row[18],"    #   #                             #   # ");
       strcpy(scan_row[19],"    #   ###############################   # ");
       strcpy(scan_row[20],"    #                                     # ");
       }
     else if (jcnt==8)
       {
       strcpy(scan_row[0], "    #                                     # ");
       strcpy(scan_row[1], "    #   ###############################   # ");
       strcpy(scan_row[2], "    #   #                             #   # ");
       strcpy(scan_row[3], "    #   #   #######################   #   # ");
       strcpy(scan_row[4], "    #   #   #                     #   #   # ");
       strcpy(scan_row[5], "    #   #   #                     #   #   # ");
       strcpy(scan_row[6], "    #   #   #                     #   #   # ");
       strcpy(scan_row[7], "    #   #   #                     #   #   # ");
       strcpy(scan_row[8], "    #   #   #                     #   #   # ");
       strcpy(scan_row[9], "    #   #   #                     #   #   # ");
       strcpy(scan_row[10],"    #   #   #                     #   #   # ");
       strcpy(scan_row[11],"    #   #   #                     #   #   # ");
       strcpy(scan_row[12],"    #   #   #                     #   #   # ");
       strcpy(scan_row[13],"    #   #   #                     #   #   # ");
       strcpy(scan_row[14],"    #   #   #                     #   #   # ");
       strcpy(scan_row[15],"    #   #   #                     #   #   # ");
       strcpy(scan_row[16],"    #   #   #                     #   #   # ");
       strcpy(scan_row[17],"    #   #   #######################   #   # ");
       strcpy(scan_row[18],"    #   #                             #   # ");
       strcpy(scan_row[19],"    #   ###############################   # ");
       strcpy(scan_row[20],"    #                                     # ");
       }
     else if (jcnt==9)
       {
       strcpy(scan_row[0], "    #                                     # ");
       strcpy(scan_row[1], "    #   ###############################   # ");
       strcpy(scan_row[2], "    #   #                             #   # ");
       strcpy(scan_row[3], "    #   #                             #   # ");
       strcpy(scan_row[4], "    #   #                             #   # ");
       strcpy(scan_row[5], "    #   #                             #   # ");
       strcpy(scan_row[6], "    #   #                             #   # ");
       strcpy(scan_row[7], "    #   #                             #   # ");
       strcpy(scan_row[8], "    #   #                             #   # ");
       strcpy(scan_row[9], "    #   #                             #   # ");
       strcpy(scan_row[10],"    #   #                             #   # ");
       strcpy(scan_row[11],"    #   #                             #   # ");
       strcpy(scan_row[12],"    #   #                             #   # ");
       strcpy(scan_row[13],"    #   #                             #   # ");
       strcpy(scan_row[14],"    #   #                             #   # ");
       strcpy(scan_row[15],"    #   #                             #   # ");
       strcpy(scan_row[16],"    #   #                             #   # ");
       strcpy(scan_row[17],"    #   #                             #   # ");
       strcpy(scan_row[18],"    #   #                             #   # ");
       strcpy(scan_row[19],"    #   ###############################   # ");
       strcpy(scan_row[20],"    #                                     # ");
       }
     else if (jcnt==10)
       {
       strcpy(scan_row[0], "    #                                     # ");
       strcpy(scan_row[1], "    #                                     # ");
       strcpy(scan_row[2], "    #                                     # ");
       strcpy(scan_row[3], "    #                                     # ");
       strcpy(scan_row[4], "    #                                     # ");
       strcpy(scan_row[5], "    #                                     # ");
       strcpy(scan_row[6], "    #                                     # ");
       strcpy(scan_row[7], "    #                                     # ");
       strcpy(scan_row[8], "    #                                     # ");
       strcpy(scan_row[9], "    #                                     # ");
       strcpy(scan_row[10],"    #                                     # ");
       strcpy(scan_row[11],"    #                                     # ");
       strcpy(scan_row[12],"    #                                     # ");
       strcpy(scan_row[13],"    #                                     # ");
       strcpy(scan_row[14],"    #                                     # ");
       strcpy(scan_row[15],"    #                                     # ");
       strcpy(scan_row[16],"    #                                     # ");
       strcpy(scan_row[17],"    #                                     # ");
       strcpy(scan_row[18],"    #                                     # ");
       strcpy(scan_row[19],"    #                                     # ");
       strcpy(scan_row[20],"    #                                     # ");
       }
     else
       {
       strcpy(scan_row[0], "                                            ");
       strcpy(scan_row[1], "                                            ");
       strcpy(scan_row[2], "                                            ");
       strcpy(scan_row[3], "                                            ");
       strcpy(scan_row[4], "                                            ");
       strcpy(scan_row[5], "                                            ");
       strcpy(scan_row[6], "                                            ");
       strcpy(scan_row[7], "                                            ");
       strcpy(scan_row[8], "                                            ");
       strcpy(scan_row[9], "                                            ");
       strcpy(scan_row[10],"                                            ");
       strcpy(scan_row[11],"                                            ");
       strcpy(scan_row[12],"                                            ");
       strcpy(scan_row[13],"                                            ");
       strcpy(scan_row[14],"                                            ");
       strcpy(scan_row[15],"                                            ");
       strcpy(scan_row[16],"                                            ");
       strcpy(scan_row[17],"                                            ");
       strcpy(scan_row[18],"                                            ");
       strcpy(scan_row[19],"                                            ");
       strcpy(scan_row[20],"                                            ");
       }
     } 
  else 
     {
     strcpy(scan_row[0], "##########################################");
     strcpy(scan_row[1], "##########################################");
     strcpy(scan_row[2], "##########################################");
     strcpy(scan_row[3], "###      ###     ###     ###    ###     ##");
     strcpy(scan_row[4], "### ######## ### ### ### ### ## ### ### ##");
     strcpy(scan_row[5], "### ######## ### ### ### ### ## ### ### ##");
     strcpy(scan_row[6], "###      ###     ###     ### ## ###     ##");
     strcpy(scan_row[7], "### ######## # ##### # ##### ## ### # ####");
     strcpy(scan_row[8], "### ######## ## #### ## #### ## ### ## ###");
     strcpy(scan_row[9], "###      ### ### ### ### ###    ### ### ##");
     strcpy(scan_row[10],"##########################################");
     strcpy(scan_row[11],"##########################################");
     strcpy(scan_row[12],"##########################################");
     strcpy(scan_row[13],"##########################################");
     strcpy(scan_row[14],"##########################################");
     strcpy(scan_row[15],"##########################################");
     strcpy(scan_row[16],"##########################################");
     strcpy(scan_row[17],"##########################################");
     strcpy(scan_row[18],"##########################################");
     strcpy(scan_row[19],"##########################################");
     strcpy(scan_row[20],"##########################################");
     } 
  for (n=0;n<SCANY;n++) /* Ausgabe aller fertigen Zeilen */
      {
      if (strlen(scan_row[n]) < (SCANX-1))
	 {
	 strncat(scan_row[n],"                                                                       ",(SCANX-1-strlen(scan_row[n])));
	 }
      mvwprintw(scan,n,0,"%s",scan_row[n]);
      }

  /***************************************************************/
  /***                       Window-Update                     ***/
  /***************************************************************/
    wnoutrefresh(scan);
    wnoutrefresh(sstatus);
    wnoutrefresh(msgw);
    wnoutrefresh(head);
    wnoutrefresh(com);
    doupdate(); 
    if (flag==16)
      {
      jcnt++;
      if (jcnt>10) jcnt=0;
      }
    usleep(150000);
    t2=time(0);
    if ( (t2-start_zeit) > 2 )
       {
       if (m_p->ship[vflag].dock==NOT_DOCKED) m_p->ship[vflag].main_energy--;
       if (m_p->ship[vflag].hood==ON) m_p->ship[vflag].main_energy--;
       status_ctrl();
       start_zeit=t2;
       }
    } while (strncmp(m_p->cmd[vflag],"QUIT1234",8) != 0 && m_p->l_exit[vflag] == NO_EXIT);

/**************************************************************************/
/***                        Zuruecksetzen des Terminals                 ***/
/**************************************************************************/
wclear(scan);
wclear(com);
wclear(head);
wclear(msgw);
wclear(sstatus);
wnoutrefresh(scan);
wnoutrefresh(sstatus);
wnoutrefresh(com);
wnoutrefresh(head);
wnoutrefresh(msgw);
doupdate();
endwin();

m_p->l_exist[vflag]=DESTROYED;
if (m_p->ship[vflag].pos[0]!=(-99))
   {
   if (m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]!=E_HOMEWORLD &&
       m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]!=F_HOMEWORLD)
       m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=SPACE;
   m_p->ship[vflag].pos[0]=-99;
   m_p->ship[vflag].pos[1]=-99;   	
   }
if (m_p->l_exit[vflag]==EXIT)
   {
   sprintf(m_p->recon_str[vflag],"%-12s: destroyed                     ",ships[vflag]);
   printf("Game over for %s! Enter RETURN\n\n\n",ships[vflag]); 
   }
(void) shmdt(m_p);
exit(0);
}

/*** qgetkey.c         ---    Unterprogramm vom Typ key_t                  ***/

/*****************************************************************************/
/*** Uebersetzung mit "cc qgetkey.c -c"                                    ***/
/*** qgetkey() liefert fuer den symbolischen Namen einer Nachrichtenwarte- ***/
/*** schlange den Wert (key) zurueck. Nur die ersten 4 Zeichen des symb.   ***/
/*** Namens sind signifikant!                                              ***/
/*** Beispiel: aus dem symbolischen Namen "ambit" wird der Wert 0x616d6269 ***/
/*****************************************************************************/

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


void status_ctrl()
/***********************************************************************/
/** Ueberwachung der Schaeden, bei Erreichen von max_dam[vflag] wird ***/
/** l_exit[vflag]=EXIT gesetzt, was der Zerstoerung des Schiffs ent- ***/
/** spricht und den Spieler aus dem Spiel wirft. Genauso fuer main_en.**/
/** Ferner: Schilde ueberwachen, Life-Support                         **/
/***********************************************************************/
{
int i,j;
char m_str[MSG_LENGTH];

/* Life Support System */

if (m_p->ship[vflag].dam[7] > 300 && rest==999)
   {
   rest=360L;   /* 5 min Zeit zur Reparatur! */
   sort_msg("**Life Support System damaged!**");
   sort_msg("**      5 min time left!      **");
   }
else
   {
   if (m_p->ship[vflag].dam[7] <= 300 && rest != 999)
      {
      sort_msg("LS: Life Support System repaired");
      rest=999L;
      }
   if (m_p->ship[vflag].dam[7] > 300 && rest != 999)
      {
      rest=rest-(2L);
      if ( rest % 10L == 0)
         {
         sprintf(m_str,"LS: %ld seconds left",rest);
         sort_msg(m_str);
         }
      }
   }
   
/* Energie<200 */
if (m_p->ship[vflag].main_energy<200) sort_msg("!!!!!!  LOW ENERGY LEVEL  !!!!!!");
  
/* Schilde */
if (m_p->ship[vflag].dam[5] >= 350 && m_p->ship[vflag].shield==ON)
   {
   m_p->ship[vflag].shield=OFF;
   sort_msg("    ** Shields down, damaged! **");
   }
if (m_p->ship[vflag].sh_energy <= 0 && m_p->ship[vflag].shield==ON)
   {
   m_p->ship[vflag].shield=OFF;   	
   sort_msg("** Shields down,Shield Energy=0");
   m_p->ship[vflag].sh_energy=0;
   }
  
/* Hood */
if (m_p->ship[vflag].dam[10] >= 250 && m_p->ship[vflag].hood==ON)
   {
   m_p->ship[vflag].hood=OFF;
   m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
   sort_msg("    ** Hood off, damaged! **");
   }


/*** Aktuelle Summe bilden ***/
m_p->ship[vflag].dam[0]=0;
if (vflag>=25) m_p->ship[vflag].dam[4]=0;
for (i=1;i<11;++i)
    {
    m_p->ship[vflag].dam[0]=m_p->ship[vflag].dam[0]+m_p->ship[vflag].dam[i];
    }
    
/* Ueberpruefung */
if ((m_p->ship[vflag].dock==NOT_DOCKED) &&
    (m_p->ship[vflag].dam[0] > max_dam[vflag] 
                || 
    m_p->ship[vflag].main_energy <= 0
                ||
               rest<0
                ||
         m_p->l_exit[vflag]==EXIT) /* externer Rauswurf,z.B. Black Hole */ )
   {
   usleep(300000);
   m_p->name_score[vflag]=m_p->name_score[vflag]-(max_dam[vflag]*m_p->rank[vflag]);
   m_p->user_score[vflag]=m_p->user_score[vflag]-(max_dam[vflag]*m_p->rank[vflag]);
   m_p->score[party[vflag]]=m_p->score[party[vflag]]-(max_dam[vflag]*m_p->rank[vflag]);
   for (j=1;j<42;j++)
       {
       if ( m_p->luser[j] == ACTIVE)
          {
          sprintf(m_str,"%s %s",kind_of_ship[vflag],ships[vflag]);
          sort_exit_msg(j,m_str);
          sprintf(m_str,"was destroyed at %03d %03d !!!",m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1]);
          sort_exit_msg(j,m_str);
          }
       }
   m_p->l_exit[vflag]=EXIT;
   }
   if (m_p->l_exit[vflag]==EXIT && m_p->ship[vflag].dock==DOCKED) m_p->l_exit[vflag]=NO_EXIT; 
return;
}


void sort_exit_msg(zflag,new_msg)
/*****************************************************************************/
/***      sort_msg: bereitet die msg-Strings des Users vflag so auf, dass  ***/
/**                 new_msg hinten (im Cockpit-Window unten) steht.        ***/
/*****************************************************************************/
int zflag;
char new_msg[MSG_LENGTH];
{
int i;
char xmsg[MSG_LENGTH];

strncpy(xmsg,new_msg,MSG_LENGTH);

for (i=0;i<6;i++)
    { 
    strncpy(m_p->ship[zflag].msg[i],m_p->ship[zflag].msg[(i+1)],MSG_LENGTH);
    }  
strncpy(m_p->ship[zflag].msg[6],xmsg,MSG_LENGTH);  
return;
}


int iproz(i1,i2)
    int i1,i2;
/****************************************************************************/
/***      Berechnet, wie % von i2 i1 entspricht                           ***/
/****************************************************************************/
{
int i3;
float x,y;

x=(float) i1;
y=(float) i2;
if (i2 == 0) y=0.00000001;
   
i3 = (NINT( ((x/y)*100.0) ));
return(i3);
}

void sig_hnd()
/*******************************************************************/
/*** Signalhandling fuer pause()                                 ***/
/*******************************************************************/
{
signal(SIGALRM,sig_hnd);
return;
}

void sig_hnd_ctrlc()
/************************************************************************/
/*** setzen des neuen Signalhandlings nach pause                      ***/
/************************************************************************/
{
sort_msg(" *** CTRL-C desaktiviert! ***");
signal(SIGINT,sig_hnd_ctrlc);
return;
}

void sort_msg(new_msg)
/*****************************************************************************/
/***      sort_msg: bereitet die msg-Strings des Users vflag so auf, dass  ***/
/**                 new_msg hinten (im Cockpit-Window unten) steht.        ***/
/*****************************************************************************/
char new_msg[MSG_LENGTH];
{
int i;

for (i=0;i<6;i++)
    { 
    strncpy(m_p->ship[vflag].msg[i],m_p->ship[vflag].msg[(i+1)],MSG_LENGTH);
    }  
strncpy(m_p->ship[vflag].msg[6],new_msg,MSG_LENGTH);  
return;
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

int find_nr_base(x,y)
    int x,y;
/******************************************************************/     
/***  laufende Nr. der Basis auf x,y                            ***/
/******************************************************************/
{
	int i,nr;
i=0;
nr=-1;
while (i<MAX_BASES && nr == -1)
    {
    if (m_p->base[i].pos[0] == x && m_p->base[i].pos[1] == y) nr=i;
    i++;
    }
    
return(nr);
}
