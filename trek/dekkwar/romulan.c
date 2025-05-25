/**********************************************************/
/*** romulan.c                                          ***/
/*** Kommentar nach V.1.00                              ***/
/*** 11.02.03: Signalhandling fuer CTRL-C               ***/
/*** 14.02.03: String in send_msg auf 33 begrenzt       ***/
/*** 18.02.03: TMusleep weg wg. Linux, siehe dwmain.c   ***/
/*** 25.02.03: neue Variable xmsg in send_msg, da der   ***/
/***           String new_msg u.U. geaendert wird       ***/
/*** Kommentar nach V.1.01                              ***/
/*** 17.03.03: In find_nr_fort und find_nr_base das     ***/
/***           i++ ans Ende der Schleife gesetzt, da    ***/
/***           sonst der Wert 0 nie durchlaufen wurde   ***/
/***           und der Returnwert nr falsch war.        ***/
/***           Dadurch tauchten Geisterforts auf.       ***/
/*** 29.04.03: Version 1.1.0                            ***/
/*** Kommentar nach V.1.1.0                             ***/
/*** 27.04.04: Umstellung auf strncpy in sort_msg und   ***/
/***           send_msg                                 ***/
/*** 07.05.04: Umstellung struct fort und explosion     ***/ 
/*** 30.06.04: MSG_LENGTH neu in dw_def.h               ***/
/*** 30.06.04: Treffer auf Sektion 4 fuer Zerstoerer    ***/
/***           korrigiert; -> Sektion 5                 ***/
/*** 30.06.04: libdw weg                                ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 22.10.04: in Textausgabe " on x y" durch "at x y " ***/
/***           ersetzt.                                 ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 28.07.07: Central Control-Text weg                 ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 23.01.08: Umstellung struct satellite              ***/
/*** 24.01.08: Umstellung struct planet                 ***/
/*** 25.01.08: Umstellung struct base                   ***/
/*** 06.02.08: neue Elemente struct base                ***/
/*** 08.02.08: Wenn Basis zerstoert wird, werden auch   ***/
/***           alle gedockten Schiffe zerstoert         ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 26.02.08: Symbol fuer DOCKING entfernt             ***/
/*** 27.02.08: struct scout eingefuehrt                 ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 13.03.08: Umstellung auf struct homeworld          ***/
/*** 15.03.08: HOOD_ON/OFF, RLEE_ON/OFF, BEAM_ON/OFF,   ***/
/***           RAMM_ON/OFF durch ON/OFF ersetzt,        ***/
/***           DEFEAT_ON/OFF weg. N_/EXISTING durch     ***/
/***           NOT_/ACTIVE ersetzt                     ***/
/*** 24.03.08: SH_DOWN/UP durch ON/OFF ersetzt          ***/
/*** 26.03.08: Umstellung auf struct ship               ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 05.04.08: Andere Seed fuer ran bei shmid=0         ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/**********************************************************/
/*** 14.10.08: Anpassungen fuer Ubuntu 8.04 (includes)  ***/
/*** 23.10.08: Anpassungen -Wall bei gcc (Ubuntu 8.04)  ***/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 22.11.09: str1 auf MSG_LENGTH gesetzt              ***/
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 02.01.10: seed() neu; ran() mit seed ueberarbeitet ***/
/*** 17.01.10: ROM_DELAY neu                            ***/
/*** 24.01.10: Robotship: Elimination und Explosion in  ***/
/***           robot.c verlagert                        ***/
/*** 24.04.10: neue Konstantendefinitionen aus dw_def.h  ***/
/*** 25.04.10: IMAX,IMIN,IABS ueberall in () gesetzt    ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/shm.h>

/*****************************************************************************/
/*** Includes DECWAR                                                       ***/
/*****************************************************************************/
#include "mathfkt.h"     /* math. Funktionen (incl. <math.h> */
#include "dw_def.h"
#include "dw_shm.h"
#include "ships.h"

extern void sig_hnd();
extern void sig_hnd_ctrlc();
extern int find_nr_fort();
extern int find_nr_base();
extern void eliminate_fort();
extern void eliminate_base();
extern key_t qgetkey();
extern void delay();
extern float ran();
extern int seed();
extern void path();
extern void send_msg();
extern int det_target();
extern int wegfrei();
extern void hit();
extern void init_expl();
extern void gauss2();
extern int find_nr_robotship();
extern int kontrolle();

int main()
/***************************************************************************/
/*** romulan.c : Der Romulaner in DECWAR                                  **/
/***************************************************************************/
{

/***************************************************************************/
/***                    lokale Variablen                                 ***/
/***************************************************************************/
int i,j,evx,evy,xalt,yalt,xneu,yneu,xstep,ystep,try,min_dist;
int xziel,yziel,hit_en,rom_priority;
int l_fire;
float r1,r2;
char obj,str1[MSG_LENGTH];

/****************************************************************************/
/*** sig_hnd setzen fuer SIGKILL, SIGINT und SIGALRM                      ***/
/****************************************************************************/
signal(SIGALRM,sig_hnd);
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
/*** Startpunkt mit Wartezustand fuer den naechsten Einsatz            ***/
/*************************************************************************/
m_p->l_exist[0]=ACTIVE;
xziel=yziel=-99;

start:
/*********************/
/* eigentl. Programm */
/*********************/
printf(" Romulan: action!\n");
/*****************/
/*** if user  ***/
/****************/

if (m_p->anz_user > 0 )  /* Nur wenn ueberhaupt jemand im Spiel ist */
   {
   /*****************/
   /* Neu einsetzen */
   /*****************/
   for (i=0;i<ARRAY_SIZE;i++)
       {
       for (j=0;j<ARRAY_SIZE;j++)
           {
           if (m_p->a[i][j]==ROMULAN) m_p->a[i][j]=SPACE;
           }
       }
   do {
      r1=ran();
      m_p->ship[0].pos[0]=5+((int) (r1*194.0));     	
      r2=ran();
      m_p->ship[0].pos[1]=5+((int) (r2*194.0));
      } while(m_p->a[m_p->ship[0].pos[0]][m_p->ship[0].pos[1]] != SPACE);
   printf("Startpos. gefunden\n");
   m_p->a[m_p->ship[0].pos[0]][m_p->ship[0].pos[1]]=ROMULAN;
   
   if (m_p->ship[0].pos[0] < (ARRAY_SIZE/2))
      {
      evx=1;
      }
   else
      {
      evx=-1;
      }
   if (m_p->ship[0].pos[1] < (ARRAY_SIZE/2))
      {
      evy=1;
      }
   else
      {
      evy=-1;
      }
   printf("Startvektoren gefunden\n");
   m_p->luser[0]=ACTIVE;
   m_p->l_exit[0]=NO_EXIT;
   m_p->ship[0].hood=OFF;
   m_p->ship[0].dock=NOT_DOCKED;
   m_p->ship[0].dam[0]=0;
   try=0;
   m_p->ship[0].sh_energy=max_sh_en[0];  
   printf("Startwerte gesetzt\n");
   /******************/
   /** Lauf des Rom. */
   /******************/
   while (m_p->anz_user>0 && m_p->ship[0].dam[0]<=max_dam[0]
                &&
          m_p->ship[0].pos[0]<(ARRAY_SIZE-5) && m_p->ship[0].pos[0]>5
                &&
          m_p->ship[0].pos[1]<(ARRAY_SIZE-5) && m_p->ship[0].pos[1]>5 
                && 
              try<10
                &&
          m_p->l_exit[0]==NO_EXIT
                &&
          kontrolle()==0) 
       {
	 printf("while gestartet\n");
       xalt=m_p->ship[0].pos[0];
       yalt=m_p->ship[0].pos[1];       
       try=0;
       printf("Beginn try\n");
       do {
          try++;
          r1=ran()*3.0;
          xstep=(NINT(r1)) * evx;
          usleep(((evx*11111)+20000));
          r2=ran()*3.0;
          ystep=(NINT(r2)) * evy;   
          xneu=xalt+xstep;
          yneu=yalt+ystep;       
          } while(m_p->a[xneu][yneu] != SPACE && try<10);
       printf("Try fertig\n");
      
       if (try<10)
          { /* neue Position */
          m_p->a[xalt][yalt]=SPACE;
          m_p->ship[0].pos[0]=xneu;          
          m_p->ship[0].pos[1]=yneu;                    
          m_p->a[xneu][yneu]=ROMULAN; 
          printf("Position: %d %d",xneu,yneu);
          printf(" Status: %d Damages: %d\n",m_p->ship[0].status,m_p->ship[0].dam[0]);
          usleep(ROM_DELAY);
          /************/
          /* Schiffe  */
          /************/
          printf("Schiffe\n");
          do {
             rom_priority=(-99);
             min_dist=99;
             l_fire=0;
             for (i=1;i<MAX_USER;i++)
                 {
                 if (m_p->luser[i]==ACTIVE 
                           &&
                    m_p->ship[i].dock==NOT_DOCKED
                          && 
                    (IABS((m_p->ship[i].pos[0]-xneu))) < 10     
                          &&
                    (IABS((m_p->ship[i].pos[1]-yneu))) < 10 
                          &&
		    kontrolle() == 0)
                    {
                    if (min_dist>(IABS((m_p->ship[i].pos[0]-xneu)))) 
                       {
                       xziel=m_p->ship[i].pos[0];
                       yziel=m_p->ship[i].pos[1];
                       if (wegfrei(xneu,yneu,xziel,yziel)==0)
                          {
                          min_dist=(IABS((m_p->ship[i].pos[0]-xneu)));
                          rom_priority=i;
                          }
                       }  /* ENDIF x-Abstand */
                    if (min_dist>(IABS((m_p->ship[i].pos[1]-yneu)))) 
                       {
                       xziel=m_p->ship[i].pos[0];
                       yziel=m_p->ship[i].pos[1];
                       if (wegfrei(xneu,yneu,xziel,yziel)==0)
                          {
                          min_dist=(IABS((m_p->ship[i].pos[0]-xneu)));
                          rom_priority=i;
                          }   
                       }    /* ENDIF y-Abstand */              
                    } /* ENDIF Schiff in Reichweite */
                 } /* ENDFOR alle Schiffe */
             if (m_p->l_exit[0]==EXIT) rom_priority=-999;
             if (rom_priority>=0)
                {
                send_msg(rom_priority,">> ATTACK by ROMULAN <<");
                hit_en=max_ph[0];
                gauss2(&hit_en);
                l_fire=1;
                printf("Feuer auf S %d\n",rom_priority);
                obj=ships[rom_priority][0];
                hit(obj,xziel,yziel,hit_en);
                sprintf(str1," We got a %d units phaser hit!",hit_en);
                send_msg(rom_priority,str1);
                alarm(3);
                pause();
                } /* ENDIF Feuern */
	  } while (l_fire!=0 && m_p->l_exit[0]!=EXIT && kontrolle()==0);  /* d.h. nochmal, wenn auf Schiff gefeuert wurde und Romulaner noch intakt*/

	/****************/
        /* Robotschiffe */
	/****************/
        printf("Robotschiffe Fed.\n");
        do {
           rom_priority=(-99);
           min_dist=99;
           l_fire=0;
           for (i=0;i<MAX_ROBOTSHIPS;i++)
	     {
             if (m_p->robotship[FEDERAL][i].status==ACTIVE 
                           &&
                (IABS((m_p->robotship[FEDERAL][i].pos[0]-xneu))) <= SHIP_RANGE
                          &&
                (IABS((m_p->robotship[FEDERAL][i].pos[1]-yneu))) <= SHIP_RANGE                           &&
		 kontrolle()==0)
                {
                if (min_dist>(IABS((m_p->robotship[FEDERAL][i].pos[0]-xneu)))) 
                   {
                   xziel=m_p->robotship[FEDERAL][i].pos[0];
                   yziel=m_p->robotship[FEDERAL][i].pos[1];
                   if (wegfrei(xneu,yneu,xziel,yziel)==0)
                      {
                      min_dist=(IABS((m_p->robotship[FEDERAL][i].pos[0]-xneu)));
                      rom_priority=i;
                      }
                   }  /* ENDIF x-Abstand */
                if (min_dist>(IABS((m_p->robotship[FEDERAL][i].pos[1]-yneu)))) 
                   {
                   xziel=m_p->robotship[FEDERAL][i].pos[0];
                   yziel=m_p->robotship[FEDERAL][i].pos[1];
                   if (wegfrei(xneu,yneu,xziel,yziel)==0)
                      { 
                      min_dist=(IABS((m_p->robotship[FEDERAL][i].pos[0]-xneu)));
                      rom_priority=i;
                      }   
                   }    /* ENDIF y-Abstand */              
                } /* ENDIF Schiff in Reichweite */
             }
             if (m_p->l_exit[0]==EXIT) rom_priority=-999;
             if (rom_priority>=0 && rom_priority < MAX_ROBOTSHIPS)
                {
                hit_en=max_ph[0];
                gauss2(&hit_en);
                l_fire=1;
                printf("Feuer auf RS Fed. %d mit %d units\n",rom_priority,hit_en);
                hit(F_ROBOTSHIP,xziel,yziel,hit_en);
                alarm(3);
                pause();
                } /* ENDIF Feuern */
	} while (l_fire!=0 && m_p->l_exit[0]!=EXIT && kontrolle()==0);  /* d.h. nochmal, wenn auf Schiff gefeuert wurde */

        printf("Robotschiffe Imp.\n");
        do {
           rom_priority=(-99);
           min_dist=99;
           l_fire=0;
           for (i=0;i<MAX_ROBOTSHIPS;i++)
	     {
             if (m_p->robotship[IMPERIAL][i].status==ACTIVE 
                           &&
                (IABS((m_p->robotship[IMPERIAL][i].pos[0]-xneu))) <= SHIP_RANGE
                          &&
                (IABS((m_p->robotship[IMPERIAL][i].pos[1]-yneu))) <= SHIP_RANGE                          && 
		 kontrolle()==0)
                {
                if (min_dist>(IABS((m_p->robotship[IMPERIAL][i].pos[0]-xneu)))) 
                   {
                   xziel=m_p->robotship[IMPERIAL][i].pos[0];
                   yziel=m_p->robotship[IMPERIAL][i].pos[1];
                   if (wegfrei(xneu,yneu,xziel,yziel)==0)
                      {
                      min_dist=(IABS((m_p->robotship[IMPERIAL][i].pos[0]-xneu)));
                      rom_priority=i;
                      }
                   }  /* ENDIF x-Abstand */
                if (min_dist>(IABS((m_p->robotship[IMPERIAL][i].pos[1]-yneu)))) 
                   {
                   xziel=m_p->robotship[IMPERIAL][i].pos[0];
                   yziel=m_p->robotship[IMPERIAL][i].pos[1];
                   if (wegfrei(xneu,yneu,xziel,yziel)==0)
                      { 
                      min_dist=(IABS((m_p->robotship[IMPERIAL][i].pos[0]-xneu)));
                      rom_priority=i;
                      }   
                   }    /* ENDIF y-Abstand */              
                } /* ENDIF Schiff in Reichweite */
             }
             if (m_p->l_exit[0]==EXIT) rom_priority=-999;
             if (rom_priority>=0 && rom_priority < MAX_ROBOTSHIPS)
                {
                hit_en=max_ph[0];
                gauss2(&hit_en);
                l_fire=1;
                printf("Feuer auf RS Imp. %d mit %d units\n",rom_priority,hit_en);
                hit(E_ROBOTSHIP,xziel,yziel,hit_en);
                alarm(3);
                pause();
                } /* ENDIF Feuern */
	} while (l_fire!=0 && m_p->l_exit[0]!=EXIT && kontrolle()==0);  /* d.h. nochmal, wenn auf Schiff gefeuert wurde */


        /*******************************************/
        /* Basen, falls noch nicht gefeuert wurde  */
        /*******************************************/
         printf("Basen\n");
          do {
             rom_priority=(-99);
             min_dist=99;
             l_fire=0;
             for (i=0;i<MAX_BASES;i++)
                 {
                 if ( (IABS((m_p->base[i].pos[0]-xneu)))<10     
                         &&
                      (IABS((m_p->base[i].pos[1]-yneu)))<10 
                         &&
                      m_p->base[i].status==ACTIVE
                         &&
                      m_p->base[i].party!=NEUTRAL 
                         &&
                      kontrolle()==0)
                    {
                    if (min_dist>(IABS((m_p->base[i].pos[0]-xneu)))) 
                       {
                       xziel=m_p->base[i].pos[0];
                       yziel=m_p->base[i].pos[1];
                       if (wegfrei(xneu,yneu,xziel,yziel)==0)
                          {
                          min_dist=(IABS((xziel-xneu)));
                          rom_priority=i;
                          }
                       } /* ENDIF x-Abstand */
                    if (min_dist>(IABS((m_p->base[i].pos[1]-yneu)))) 
                       {
                       xziel=m_p->base[i].pos[0];
                       yziel=m_p->base[i].pos[1];
                       if (wegfrei(xneu,yneu,xziel,yziel)==0)
                          {
                          min_dist=(IABS((xziel-xneu)));
                          rom_priority=i;
                          }
                       }   /* ENDIF y-Abstand */                  
                    } /* ENDIF Basis in Reichweite, aktiv und nicht neutral*/
                 } /* ENDFOR Basem */
             if (m_p->l_exit[0]==EXIT) rom_priority=-999;
             if (rom_priority>=0)
                {
                i=rom_priority;
                hit_en=max_ph[0];
                gauss2(&hit_en);
                l_fire=1; 
                printf("Feuer auf B %d",rom_priority);
       printf(" Status: %d Damages: %d\n",m_p->ship[0].status,m_p->ship[0].dam[0]);
                if (m_p->base[i].party==FEDERAL && m_p->base[i].status==ACTIVE)
                   {
                   obj=F_BASE;
                   }
                else
                   {
                   obj=E_BASE;
                   }
                hit(obj,xziel,yziel,hit_en);
                alarm(3);
                pause();
                } /* ENDIF Feuern */
	  } while (l_fire!=0 && m_p->l_exit[0]!=EXIT && kontrolle()==0);  /* d.h. nochmal, wenn auf Schiff gefeuert wurde */

            
         /*******************************************/
         /* Forts, falls nicht schon gefeuert wurde */
         /*******************************************/ 
       printf("Forts\n");
          do {
             rom_priority=(-99);
             min_dist=99;
             l_fire=0;
             for (i=0;i< MAX_FORTS;i++)
                 {
                 if ( (IABS((m_p->fort[i].pos[0]-xneu)))<10     
                         &&
                      (IABS((m_p->fort[i].pos[1]-yneu)))<10 
                         &&
                      kontrolle()==0)
                    {
                    if (min_dist>(IABS((m_p->fort[i].pos[0]-xneu)))) 
                       {
                       xziel=m_p->fort[i].pos[0];
                       yziel=m_p->fort[i].pos[1];
                       if (wegfrei(xneu,yneu,xziel,yziel)==0)
                          {
                          min_dist=(IABS((xziel-xneu)));
                          rom_priority=i;
                          }
                       } /* ENDIF x-Abstand */
                    if (min_dist>(IABS((m_p->fort[i].pos[1]-yneu)))) 
                       {
                       xziel=m_p->fort[i].pos[0];
                       yziel=m_p->fort[i].pos[1];
                       if (wegfrei(xneu,yneu,xziel,yziel)==0)
                          {
                          min_dist=(IABS((xziel-xneu)));
                          rom_priority=i;
                          }
                       }   /* ENDIF y-Abstand */                  
                    } /* ENDIF Fort in Reichweite */
                 } /* ENDFOR Forts */
             if (m_p->l_exit[0]==EXIT) rom_priority=-999;                 
             if (rom_priority>=0)
                {
                i=rom_priority;
                hit_en=max_ph[0];
                gauss2(&hit_en);
                l_fire=1; 
                printf("Feuer auf F %d\n",rom_priority);
                    /* Hier Wirkung programmieren */
                if (m_p->fort[i].party==FEDERAL)
                   {
                   obj=F_FORT;
                   }
                else
                   {
                   obj=E_FORT;
                   }
                hit(obj,xziel,yziel,hit_en);
                alarm(3);
                pause();
                } /* ENDIF Feuern */
	  } while (l_fire!=0 && m_p->l_exit[0]!=EXIT && kontrolle()==0);  /* d.h. nochmal, wenn auf Schiff gefeuert wurde */
          /* Kontrolle */
          if (kontrolle()==1)
             { 
	     m_p->l_exit[0]=EXIT;
             }
          if (l_fire==0 && m_p->l_exit[0]==NO_EXIT && kontrolle()==0)
             { 
             alarm(3);
             pause();
             }
          }  /* ENDIF try<10 */   
       } /* while d>5 usw. */
   m_p->a[m_p->ship[0].pos[0]][m_p->ship[0].pos[1]]=EXPLOSION;
   usleep(EXPL_TIME);
   for (j=1;j<MAX_USER;j++)
     {
     if ( m_p->luser[j]==ACTIVE )
        {
        sprintf(str1,"   Romulan at %d %d ",m_p->ship[0].pos[0] ,m_p->ship[0].pos[1]);
        send_msg(j,str1);
        send_msg(j,"   destroyed or vanished!");
        } 
     }
   m_p->a[m_p->ship[0].pos[0]][m_p->ship[0].pos[1]]=SPACE;
   m_p->ship[0].pos[0]=m_p->ship[0].pos[1]=-99;
   m_p->luser[0]=DESTROYED;
   m_p->l_exit[0]=EXIT;          

   } /* anz_user>0 */
printf("Romulan: Pause\n");
alarm(30);
pause();
goto start;

}

void sig_hnd()
/*************************************************************************/
/*** Steuerung der Signale                                             ***/
/*************************************************************************/
{
signal(SIGALRM,sig_hnd);
return;
}

int wegfrei(x,y,xz,yz)
    int x,y,xz,yz;
/*************************************************************************/
/** Ueberprueft, ob die Schussbahn fuer Romulaner frei ist              **/
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
for (i=1;i<steps;i++)
    {
    xneu=xalt+ (NINT( (((float) i)*rxdiff) ));
    yneu=yalt+ (NINT( (((float) i)*rydiff) ));
    aneu=m_p->a[xneu][yneu];
    if (aneu != SPACE && aneu != EXPLOSION) cond=1;
    }
 
return(cond);
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


void path(xsteps,ysteps,rxdiff,rydiff,steps)
/***************************************************************************/
/*** path: Berechnet fuer vorgegebene x- und y-Flugweite die Schrittzahl ***/
/***       steps und die jeweilige Schrittweite (float) rxdiff u. rydiff ***/
/***************************************************************************/
int xsteps,ysteps,*steps;
float *rxdiff,*rydiff;
{
*steps=(IMAX( (IABS(xsteps)) , (IABS(ysteps)) ));
*rxdiff= ((float) xsteps)/((float) *steps);   	
*rydiff= ((float) ysteps)/((float) *steps);   	    
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

void hit(obj,x,y,energy)
     int x,y,energy;
     char obj;
/**************************************************************************/
/*** Trefferwirkung auf veschiedene Objekte obj                         ***/
/**************************************************************************/
{
int nr,pntsd,pnts,i,zflag,dx,device,nrob;
char print_str[MSG_LENGTH];

/*********/
/* Forts */
/*********/
if (obj==F_FORT || obj==E_FORT)
   {
   nr=find_nr_fort(x,y);
   if (nr == -99)
     {
     printf("Rom: ERROR: invalid nr (fort)\n");
     return;
     }
   m_p->fort[nr].dam=m_p->fort[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->fort[nr].party)
          {
          sprintf(print_str,"Fort at %03d %03d attacked by %c!",x,y,ships[0][0]);
          send_msg(i,print_str);
          }
       }
   m_p->user_score[0]=m_p->user_score[0]+15;       
   if (m_p->fort[nr].dam>=MAX_DAM_F)
      {
      eliminate_fort(nr);
      m_p->user_score[0]=m_p->user_score[0]+150;
      init_expl(x,y,EXPL_TIME);
      return; 
      }
   return;
   }       /* Ende Fort */   


/*********/
/* Basis */
/*********/
if (obj==F_BASE || obj==E_BASE)
   {
   nr=find_nr_base(x,y);   
   m_p->base[nr].condition=RED;
   m_p->base[nr].red_count=MAX_RED_COUNT;
   m_p->user_score[0]=m_p->user_score[0]+40;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->base[nr].party)
          {
          sprintf(print_str,"Base at %03d %03d attacked by %c!",x,y,ships[0][0]);
          send_msg(i,print_str);
          }
       }
   if (m_p->base[nr].sh==ON)
      {
      m_p->base[nr].sh_energy=m_p->base[nr].sh_energy-energy;
      if (m_p->base[nr].sh_energy<0) m_p->base[nr].sh_energy=0;      
      m_p->base[nr].dam=m_p->base[nr].dam+(energy/10);
      if (m_p->base[nr].sh_energy<=0) m_p->base[nr].sh=OFF;
      }
   else
      {
      m_p->base[nr].dam=m_p->base[nr].dam+energy;
      }
   if (m_p->base[nr].dam>=MAX_DAM_BAS)
      {
      eliminate_base(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[0]=m_p->user_score[0]+500;
      return; 
      }
   return;
   }       /* Ende Basis */   

/***********/
/* Schiffe */
/***********/
zflag=det_target(obj);
   
if (zflag>0 && zflag<MAX_USER)
   {
   pnts=30+(level_of_ship[zflag]*10);
   pntsd=max_dam[zflag];
   m_p->user_score[0]=m_p->user_score[0]+pnts;
   device=energy;
   if (device>10)                 /* Endziffer von energy bestimmen, */
      {                           /* liefert das device fuer damages */
      dx=device;
      dx=dx/10;
      dx=dx*10;
      device=device-dx;
      if (device==0) device=1;
      if (level_of_ship[zflag] == DESTROYER && device == 4) device=5;
      }
   if (m_p->ship[zflag].shield==ON)
      {
      m_p->ship[zflag].sh_energy=m_p->ship[zflag].sh_energy-energy;
      if (m_p->ship[zflag].sh_energy<0) m_p->ship[zflag].sh_energy=0;
      m_p->ship[zflag].dam[device]=m_p->ship[zflag].dam[device]+(energy/10);
      if (m_p->ship[zflag].sh_energy<=0) m_p->ship[zflag].shield=OFF;
      }
   else
      {
      m_p->ship[zflag].dam[device]=m_p->ship[zflag].dam[device]+energy; 
      }
   if (m_p->ship[zflag].dam[0]>=max_dam[zflag])
      {
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[0]=m_p->user_score[0]+pntsd;
      return; 
      }
   return;
   }       /* Ende Schiffe */   


/***************/
/* Robotschiff */
/***************/
if (obj==F_ROBOTSHIP)
   {
   nrob=find_nr_robotship(x,y,FEDERAL);   
   if (nrob < 0 || nrob >=MAX_ROBOTSHIPS)
      {
      m_p->a[x][y]=SPACE;
      return;
      }
   pnts=50;
   pntsd=800;
   m_p->score[0]=m_p->score[0]+pnts;
   if (m_p->robotship[FEDERAL][nrob].shield==ON)
      {
      m_p->robotship[FEDERAL][nrob].sh_energy=m_p->robotship[FEDERAL][nrob].sh_energy-energy;
      if (m_p->robotship[FEDERAL][nrob].sh_energy<0) m_p->robotship[FEDERAL][nrob].sh_energy=0;
      m_p->robotship[FEDERAL][nrob].dam=m_p->robotship[FEDERAL][nrob].dam+(energy/10); 
      if (m_p->robotship[FEDERAL][nrob].sh_energy<=0) m_p->robotship[FEDERAL][nrob].shield=OFF;
      }
   else
      {
      m_p->robotship[FEDERAL][nrob].dam=m_p->robotship[FEDERAL][nrob].dam+energy;
      }
   if (m_p->robotship[FEDERAL][nrob].dam>=MAX_DAM_ROBOTSHIP)
      {
      m_p->robotship[FEDERAL][nrob].status=DESTROYED;
      m_p->score[0]=m_p->score[0]+pntsd;
      return; 
      }
   return;
   }
if (obj==E_ROBOTSHIP)
   {
   nrob=find_nr_robotship(x,y,IMPERIAL);   
   if (nrob < 0 || nrob >=MAX_ROBOTSHIPS)
      {
      m_p->a[x][y]=SPACE;
      return;
      }
   pnts=50;
   pntsd=800;
   m_p->score[0]=m_p->score[0]+pnts;
   if (m_p->robotship[IMPERIAL][nrob].shield==ON)
      {
      m_p->robotship[IMPERIAL][nrob].sh_energy=m_p->robotship[IMPERIAL][nrob].sh_energy-energy;
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<0) m_p->robotship[IMPERIAL][nrob].sh_energy=0;
      m_p->robotship[IMPERIAL][nrob].dam=m_p->robotship[IMPERIAL][nrob].dam+(energy/10); 
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<=0) m_p->robotship[IMPERIAL][nrob].shield=OFF;
      }
   else
      {
      m_p->robotship[IMPERIAL][nrob].dam=m_p->robotship[IMPERIAL][nrob].dam+energy;
      }
   if (m_p->robotship[IMPERIAL][nrob].dam>=MAX_DAM_ROBOTSHIP)
      {
      m_p->robotship[IMPERIAL][nrob].status=DESTROYED;
      m_p->score[0]=m_p->score[0]+pntsd;
      return; 
      }
   return;
   }       
/* Ende Robotschiff */   

return;
}

/*****************************************************************************/
/*** find_nr_fort.c                                                        ***/
/*****************************************************************************/
/*** Index-Nr. des Forts auf x,y bestimmen                                 ***/
/*** Fuer struct fort                                                      ***/
/*** Falls keines gefunden wird -99 zurueckgegeben                         ***/
/*****************************************************************************/
int find_nr_fort(x,y)
    int x,y;
{
int i,nrf;

nrf=-99;
i=0;
while (nrf == -99 && i < MAX_FORTS)
    {
    if (m_p->fort[i].pos[0] == x && m_p->fort[i].pos[1] == y && m_p->fort[i].status == ACTIVE)
       {
       nrf=i;
       }
    i++;
    }
return(nrf);
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

void eliminate_base(nrb)
     int nrb;
/**********************************************************************/
/*** Zerstoerung der Basis nrb. Beinhaltet die Vernichtung aller an ***/
/*** diese Basis gedockten Schiffe!                                 ***/
/**********************************************************************/
{
int xbas,ybas,i;

if (nrb < 0 || nrb >= MAX_BASES) printf("Error: Wrong base count!\n"); 

xbas=m_p->base[nrb].pos[0];   
ybas=m_p->base[nrb].pos[1];   

m_p->base[nrb].status=DESTROYED;
m_p->base[nrb].party=NEUTRAL;
m_p->base[nrb].dam=0;
m_p->base[nrb].sh=ON;
m_p->base[nrb].sh_energy=BASE_SH_EN;
m_p->base[nrb].pos[0]=m_p->base[nrb].pos[1]=-99;
m_p->base[nrb].condition=GREEN;
m_p->base[nrb].red_count=0;
m_p->base[nrb].bay[0]=' ';
m_p->base[nrb].bay[1]=' ';
m_p->base[nrb].bay[2]=' ';
m_p->base[nrb].bay[3]=' ';

for (i=0;i<MAX_USER;i++)
  {
  if (m_p->luser[i]==ACTIVE && m_p->ship[i].pos[0]==xbas &&
      m_p->ship[i].pos[1]==ybas && m_p->ship[i].dock==DOCKED) m_p->l_exit[i]=EXIT;
  }

return;
}


/*****************************************************************************/
/*** eliminate_fort.c                                                      ***/
/*****************************************************************************/
/*** Zerstoerung des Forts mit dem Index nrf                               ***/
/*** Neue Version fuer struct fort                                         ***/
/*****************************************************************************/
void eliminate_fort(nrf)
     int nrf;
{

m_p->fort[nrf].status = DESTROYED;
m_p->fort[nrf].party = NEUTRAL;
m_p->a[m_p->fort[nrf].pos[0]][m_p->fort[nrf].pos[1]] = SPACE;
m_p->fort[nrf].pos[0] = -99;
m_p->fort[nrf].pos[1] = -99;
m_p->fort[nrf].dam = 0;


return;                   
}


int det_target(target)
/*****************************************************************************/
/*** det_target() liefert zum Suchcharacter target die Kontroll-Nummer zflag */
/***              falls das Ziel existiert, sonst -1.                      ***/
/*****************************************************************************/
char target;
{
	int zflag;
	
zflag=(-1);
      if (target == 'R' || target == 'r') zflag=0;    /* Romulaner */
      if (target == 'G') zflag=(-1); /* Traeger noch nicht implementiert */
      if (target == 'A') zflag=(-1);
      if (target == 'H') zflag=(-1);
      if (target == 'Z') zflag=(-1);
      if (target == 'E') zflag=5;
      if (target == 'L') zflag=6;
      if (target == 'Y') zflag=7;
      if (target == 'D') zflag=8;
      if (target == 'M') zflag=9;
      if (target == 'P') zflag=10;
      if (target == 'J') zflag=11;
      if (target == 'V') zflag=12;
      if (target == 'C') zflag=13;
      if (target == 'O') zflag=14;
      if (target == 'K') zflag=15;
      if (target == 'T') zflag=16;
      if (target == 'N') zflag=17;
      if (target == 'U') zflag=18;
      if (target == 'F') zflag=19;
      if (target == 'I') zflag=20;
      if (target == 'B') zflag=21;
      if (target == 'W') zflag=22;
      if (target == 'S') zflag=23;
      if (target == 'Q') zflag=24;
      if (target == 'k') zflag=25;
      if (target == 'n') zflag=26;
      if (target == 'd') zflag=27;
      if (target == 'l') zflag=28;
      if (target == 'e') zflag=29;
      if (target == 't') zflag=30;
      if (target == 'c') zflag=31;
      if (target == 'o') zflag=32;
      if (target == 'a') zflag=33;
      if (target == 'y') zflag=34;
      if (target == 'v') zflag=35;
      if (target == 'w') zflag=36;
      if (target == 'b') zflag=37;
      if (target == 'h') zflag=38;
      if (target == 'f') zflag=39;
      if (target == 's') zflag=40;
      if (target == 'm') zflag=41;
      if (target == 'p') zflag=42;
      if (target == 'i') zflag=43;
      if (target == 'j') zflag=44;

return(zflag);
}


void sig_hnd_ctrlc()
/************************************************************************/
/*** setzen des Signalhandlings fuer CTRL-C                           ***/
/************************************************************************/
{
printf("Abbruch rom mit CTRL-C: Zuruecksetzen der Werte erfolgt...");
if (m_p->ship[0].pos[0] < ARRAY_SIZE && m_p->ship[0].pos[0] >= 0 && m_p->ship[0].pos[1] < ARRAY_SIZE && m_p->ship[0].pos[1] >= 0 )
  {
  m_p->a[m_p->ship[0].pos[0]][m_p->ship[0].pos[1]]=SPACE;
  m_p->ship[0].pos[0]=-99;
  m_p->ship[0].pos[1]=-99;
  }
m_p->l_exist[0]=DESTROYED;
m_p->luser[0]=DESTROYED;
m_p->l_exit[0]=EXIT;
printf(" ...Werte rom auf NULL\n");
exit(1);
}

/*****************************************************************************/
/*** init_expl.c                                                           ***/
/*****************************************************************************/
void init_expl(x,y,mysecs) 
     int x,y;
     int mysecs; 

{
int pid;
char dummy1[4], dummy2[4], dummy3[8];

sprintf(dummy1,"%d",x);
sprintf(dummy2,"%d",y);
sprintf(dummy3,"%d",mysecs);

pid = fork();
if (pid==-1) exit(1);
if (pid==0) execl("explosion",dummy1,dummy2,dummy3,(char *)0);

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

int find_nr_robotship(x,y,p)
     int x,y,p;
/******************************************************************/
/***  laufende Nr. des Robotschiffs auf x,y                     ***/
/******************************************************************/
{
        int i,nr;

i=0;
nr=-1;

while (i<MAX_ROBOTSHIPS && nr == -1)
      {
      if (m_p->robotship[p][i].pos[0] == x && m_p->robotship[p][i].pos[1] == y && m_p->robotship[p][i].status != DESTROYED) nr=i;
      i++;
      }

return(nr);
}

int kontrolle()
/******************************************************************/
/* Check, ob Schaeden zu hoch                                     */
/******************************************************************/
{
int check;

check=0;

if (m_p->ship[0].dam[0]>max_dam[0] || m_p->l_exit[0]==EXIT)
   { 
   check=1;
   }
return check;
}
