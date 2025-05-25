/**********************************************************/
/*** robot.c                                            ***/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 22.11.09: Feldueberschreitung bei ship.dam[nn] und ***/
/***           andere -Wall-Meldungen beseitigt         ***/
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 29.11.09: Missile-Relikte korrigiert               ***/ 
/*** 20.12.09: Fehler bei Symboldarstellung: es wurde   ***/
/***           immer F_ROBOTSHIP genommen. Das Symbol   ***/
/***           wird jetzt in robsym abgelegt. Ferner    ***/
/***           gab es weitere explizite F_ROBOTSHIP-Zu- ***/
/***           weisungen, die ebenfalls korrig. wurden  ***/
/*** 02.01.09: In wegfrei() auf init_expl() umgestellt  ***/
/*** 24.01.10: Selbstueberwachung und Zerstoerung Rob.  ***/
/***           erweitert, neuen Text, Abfrage status,   ***/
/***           EXPLOSION in eliminate_robotship()       ***/
/*** 13.03.10  Feuer nur noch, wenn wegfrei()==0 (wurde ***/
/***           zuvor nur bei RADARLEE=OFF abgefragt)    ***/
/*** 13.03.10  In wegfrei() keine Zerstoerung von Minen ***/
/***           und Blindgaengern mehr.                  ***/
/*** 13.03.10  Minenraeumung umfasst jetzt auch BLIND   ***/
/*** 13.03.10  Feuer auf feindliche HW                  ***/
/*** 13.03.10: Status WAITING neu (fuer fuel <= 0)      ***/
/*** 20.03.10: Feuer auf feindliche Robotschiffe        ***/
/*** 21.03.10: Status->WAITING bei Energie < MIN_ROBOT- ***/
/***           SHIP_ENERGY                              ***/
/*** 03.04.10: Missionabbruch wenn Torpedos verschossen ***/
/*** 16.04.10: Mission J hinzugefuegt                   ***/
/*** 24.04.10: neue Konstantendefinitionen aus dw_def.h ***/
/*** 25.04.10: IMAX,IMIN,IABS ueberall in () gesetzt    ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 09.05.10: fire(): falls BLIND (=TORPEDO) dann nach ***/
/***           Delay nochmal abfragen, um von Torpedo   ***/
/***           zu unterscheiden                         ***/
/*** 09.05.10: Meldung HW-Dock nach Missionsende mit    ***/
/***           Robotschiff-Nr.-Angabe                   ***/
/*** 22.05.10: Status CANCELED (aus CR) eingebaut       ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
/*** 04.09.11: Infotext bei Start geaendert             ***/
/*** 23.10.11: Mission C (Capture Planet) neu           ***/
/*** 05.11.11: Fall beruecksichtigt, dass Planet bei    ***/
/***           Capture zerstoert wird.                  ***/
/**********************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
  
/*****************************************************************************/
/*** Includes                                                              ***/
/*****************************************************************************/
#include "mathfkt.h"     /* math. Funktionen (incl. <math.h> */
#include "dw_def.h"
#include "dw_shm.h"
#include "ships.h"

/****************************************************************************/
/***                         Globale Variablen                            ***/
/****************************************************************************/
static int vflag; 

/***************************************************************************/
/***                         Unterprogramme                              ***/
/***************************************************************************/

extern float ran();
extern int seed();
extern key_t qgetkey();
extern void sort_msg();
extern void send_msg();
extern void sig_hnd();
extern void sig_hnd_ctrlc();
extern void init_expl();
extern void eliminate_robotship();
extern int find_nr_base();
extern int find_nr_sat();
extern int find_nr_fort();
extern int fire();
extern int wegfrei();
extern int behind();
extern void path();
extern int det_target();
extern void capture();
extern int find_nr_pl();
extern void gauss2();

int main(argc,argv)
     /*******************/
     /* Programmbeginn  */
     /*******************/
    int argc;
    char *argv[];
    
{

/***************************************************************************/
/***                    lokale Variablen                                 ***/
/***************************************************************************/
int hflag,i,xziel,yziel,xstart,ystart,cxziel,cyziel;
int xneu,yneu,fac,nrob,fired;
int xvec,yvec,xsteps,ysteps;
float random;
char aneu,action_flag[2],robsym,str1[MSG_LENGTH];

/****************************************************************************/
/***                         Shared Memory einbinden                      ***/
/****************************************************************************/
shmid = shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Error: SHM nicht einzubinden;shmid= %d m_p->shm_id: %d",shmid,m_p->shm_id);
   exit(1); 
   }
if (shmid != m_p->shm_id)
   {
   printf("Error: shmid ungleich m_p->shm_id: %d %d",shmid,m_p->shm_id);
   exit(1);
   }

/****************************************************************************/
/***                    argv[n] konvertieren                              ***/
/****************************************************************************/
sscanf(argv[0],"%d",&vflag); 
sscanf(argv[1],"%d",&nrob); 
sscanf(argv[2],"%s",action_flag);  
sscanf(argv[3],"%d",&xziel); 
sscanf(argv[4],"%d",&yziel);

/****************************************************************************/
/*** sig_hnd setzen fuer SIGKILL und SIGALRM                              ***/
/****************************************************************************/
signal(SIGALRM,sig_hnd);
signal(SIGINT,sig_hnd_ctrlc);
signal(SIGKILL,SIG_DFL);

/**************************/
/* Einleitenden Abfragen  */
/**************************/

if (nrob < 0 || nrob >= MAX_ROBOTSHIPS)
   {
   sort_msg("LR: Error: wrong robotship!");
   (void) shmdt(m_p);
   exit(0);
   }  
else if (m_p->robotship[party[vflag]][nrob].status!=HW_DOCKED)
  {
   sort_msg("LR: Error: wrong robotship!");
   (void) shmdt(m_p);
   exit(0);
   }  

if (action_flag[0] != 'P' && action_flag[0] != 'p' && 
    action_flag[0] != 'M' && action_flag[0] != 'm' &&
    action_flag[0] != 'C' && action_flag[0] != 'c' &&
    action_flag[0] != 'J' && action_flag[0] != 'j')
   {
   sort_msg("LR: Error: invalid mission type!");
   (void) shmdt(m_p);
   exit(0);
   }

if (party[vflag]==FEDERAL)
  {
  robsym=F_ROBOTSHIP;
  }
else if (party[vflag]==IMPERIAL)
  {
  robsym=E_ROBOTSHIP;
  }
else
  {
  sort_msg("LR: Error; should not happen!");
  (void) shmdt(m_p);
  exit(0);
  }

m_p->robotship[party[vflag]][nrob].mission=action_flag[0];
m_p->robotship[party[vflag]][nrob].status=ACTIVE;
m_p->robotship[party[vflag]][nrob].pos[0]=m_p->homeworld[party[vflag]].pos[0];
m_p->robotship[party[vflag]][nrob].pos[1]=m_p->homeworld[party[vflag]].pos[1];
m_p->robotship[party[vflag]][nrob].main_energy=ROBOTSHIP_ENERGY;
m_p->robotship[party[vflag]][nrob].sh_energy=ROBOTSHIP_SH_ENERGY;
m_p->robotship[party[vflag]][nrob].torp=ROBOTSHIP_TORPEDOES;
m_p->robotship[party[vflag]][nrob].fuel=ROBOTSHIP_FUEL;
m_p->robotship[party[vflag]][nrob].dam=0;
m_p->robotship[party[vflag]][nrob].shield=OFF;

xstart=ystart=-99;
xvec=yvec=0;
i=1;
do 
{
   if (xziel >= m_p->homeworld[party[vflag]].pos[0] && 
       yziel >= m_p->homeworld[party[vflag]].pos[1] && 
       m_p->a[m_p->homeworld[party[vflag]].pos[0]+i][m_p->homeworld[party[vflag]].pos[1]+i] == SPACE)
      {
      xstart=m_p->homeworld[party[vflag]].pos[0]+i;
      ystart=m_p->homeworld[party[vflag]].pos[1]+i;
      }
   else if (xziel <= m_p->homeworld[party[vflag]].pos[0] &&
            yziel <= m_p->homeworld[party[vflag]].pos[1] &&
       m_p->a[m_p->homeworld[party[vflag]].pos[0]-i][m_p->homeworld[party[vflag]].pos[1]-i] == SPACE)
      {
      xstart=m_p->homeworld[party[vflag]].pos[0]-i;
      ystart=m_p->homeworld[party[vflag]].pos[1]-i;
      }       
   else if (xziel >= m_p->homeworld[party[vflag]].pos[0] &&
            yziel <= m_p->homeworld[party[vflag]].pos[1] &&
       m_p->a[m_p->homeworld[party[vflag]].pos[0]+i][m_p->homeworld[party[vflag]].pos[1]-i] == SPACE)
      {
      xstart=m_p->homeworld[party[vflag]].pos[0]+i;
      ystart=m_p->homeworld[party[vflag]].pos[1]-i;
      }
   else if (xziel <= m_p->homeworld[party[vflag]].pos[0] &&
            yziel >= m_p->homeworld[party[vflag]].pos[1] &&
       m_p->a[m_p->homeworld[party[vflag]].pos[0]-i][m_p->homeworld[party[vflag]].pos[1]+i] == SPACE)
      {
      xstart=m_p->homeworld[party[vflag]].pos[0]-i;
      ystart=m_p->homeworld[party[vflag]].pos[1]+i;
      }
   else
      {
      i++;
      }
} while ( (xstart == -99 || ystart == -99) && (i < 99) );

if (i>=99)
{
   m_p->robotship[party[vflag]][nrob].status=HW_DOCKED;
   m_p->robotship[party[vflag]][nrob].mission='N';
   sort_msg("LR: Could not start robotship!");
   (void) shmdt(m_p);
   exit(0);
}

sprintf(str1,"Robot %1d launched. Destination:",nrob);
sort_msg(str1);
if (action_flag[0]=='P' || action_flag[0]=='p') sprintf(str1,"   %d %d (mission: patrol)",xziel,yziel);
if (action_flag[0]=='M' || action_flag[0]=='m') sprintf(str1,"   %d %d (mission: minesweep)",xziel,yziel);
if (action_flag[0]=='C' || action_flag[0]=='c') sprintf(str1,"   %d %d (mission: capture)",xziel,yziel);
if (action_flag[0]=='J' || action_flag[0]=='j') sprintf(str1,"   %d %d (mission: jumpgate)",xziel,yziel);
sort_msg(str1);

m_p->a[xstart][ystart]=robsym;
m_p->robotship[party[vflag]][nrob].pos[0]=xstart;
m_p->robotship[party[vflag]][nrob].pos[1]=ystart;
m_p->robotship[party[vflag]][nrob].shield=ON;
usleep(300000);

if (action_flag[0] == 'P' || action_flag[0] == 'p' || 
    action_flag[0] == 'M' || action_flag[0] == 'm' ||
    action_flag[0] == 'C' || action_flag[0] == 'c' ||
    action_flag[0] == 'J' || action_flag[0] == 'j')
/* zur Zeit: 1. P - Patrouille nach x y und zurueck zur HW 
             2. M - Minenraeumung
             3. C - Planeteneroberung
             4. J - Sprungtore vernichten */
/* Fuer diese Faelle: Flugweise von scout! */
   {
   if (action_flag[0] == 'C' || action_flag[0] == 'c')
     {
     cxziel=xziel;
     cyziel=yziel;
     }
   hflag=0;
   fac=1;
   do {
      xsteps=xziel-m_p->robotship[party[vflag]][nrob].pos[0];
      ysteps=yziel-m_p->robotship[party[vflag]][nrob].pos[1];
      if (hflag == 0)  /* alte Richtung beibehalten bei Hindernis */ 
      {
      if (xsteps > 0)
        {
          xvec=fac;
        }
      else if (xsteps == 0)
        {
          xvec=0;
        } 
      else
        {
          xvec=-fac;
        }
      if (ysteps > 0) 
        {
          yvec=fac;
        }
      else if (ysteps == 0)
        {
          yvec=0;
        }
      else
        {
          yvec=-fac;
        }
      }
      else
        {
        hflag=0;
        }
      xneu=m_p->robotship[party[vflag]][nrob].pos[0]+xvec;
      yneu=m_p->robotship[party[vflag]][nrob].pos[1]+yvec;
      aneu=m_p->a[xneu][yneu];
      if (aneu == SPACE)
        {
        fac=1;
        m_p->a[m_p->robotship[party[vflag]][nrob].pos[0]][m_p->robotship[party[vflag]][nrob].pos[1]]=SPACE;
        m_p->a[xneu][yneu]=robsym;
        m_p->robotship[party[vflag]][nrob].pos[0]=xneu;
        m_p->robotship[party[vflag]][nrob].pos[1]=yneu;
        m_p->robotship[party[vflag]][nrob].fuel=m_p->robotship[party[vflag]][nrob].fuel-1;
        m_p->robotship[party[vflag]][nrob].main_energy=m_p->robotship[party[vflag]][nrob].main_energy-5;
        if (m_p->robotship[party[vflag]][nrob].main_energy < 0 || m_p->robotship[party[vflag]][nrob].status == DESTROYED)
	  {
          sprintf(str1,"Robotship destroyed at %03d %03d",m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1]);
          sort_msg(str1);
          eliminate_robotship(nrob,party[vflag]);
          usleep(100000);
          (void) shmdt(m_p);
          exit(0);
          }
	/* Feuer auf feindliche Objekte (falls welche da sind) */
        fired=0;
        if (m_p->robotship[party[vflag]][nrob].torp > 0) fired=fire(action_flag[0],xneu,yneu,nrob); 
        /* fired>0: hat gefeuert; in Position verharren */
        while (fired>0 && m_p->robotship[party[vflag]][nrob].torp>0 && m_p->robotship[party[vflag]][nrob].main_energy>0 && m_p->robotship[party[vflag]][nrob].status == ACTIVE)
	  {
	  m_p->robotship[party[vflag]][nrob].main_energy=m_p->robotship[party[vflag]][nrob].main_energy-25;
	  m_p->robotship[party[vflag]][nrob].torp--;
	  usleep(ROBOT_DELAY);
          fired=fire(action_flag[0],xneu,yneu,nrob); 
          }
        } 
      else if (abs(xsteps) < 2 && abs(ysteps) < 2 )
        {
        xziel = m_p->robotship[party[vflag]][nrob].pos[0];
        yziel = m_p->robotship[party[vflag]][nrob].pos[1];
        }
      else /* Hindernis oder altes Feld , zufaellig andere Ziele anwaehlen */
        {
        random=ran();
        if (random < 0.125) 
          {	
          xvec = -fac;
          yvec =fac;
          }
        else if (random >= 0.125 && random < 0.25)
          {	
          xvec = 0;
          yvec = fac;
          }
        else if (random >= 0.25 && random < 0.375)
          {	
          xvec = fac;
          yvec = fac;
          }
        else if (random >= 0.375 && random < 0.5)
          {	
          xvec = fac;
          yvec = 0;
          }
        else if (random >= 0.5 && random < 0.625)
          {	
          xvec = fac;
          yvec = -fac;
          }
        else if (random >= 0.625 && random < 0.75)
          {	
          xvec = 0;
          yvec = -fac;
          }
        else if (random >= 0.75 && random < 0.875)
          {	
          xvec = -fac;
          yvec = -fac;
          }
        else 
          {	
          xvec = -fac;
          yvec = 0;
          }
        hflag=1;
        fac++;
        }
     if (m_p->robotship[party[vflag]][nrob].fuel <= 0 || m_p->robotship[party[vflag]][nrob].main_energy < MIN_ROBOTSHIP_ENERGY)
       {
       sort_msg("Robot status changed to WAITING");
       m_p->robotship[party[vflag]][nrob].status=WAITING;
       do
        {
        usleep(10000000);
        m_p->robotship[party[vflag]][nrob].main_energy=m_p->robotship[party[vflag]][nrob].main_energy-2;
	if (m_p->robotship[party[vflag]][nrob].main_energy < 0 || m_p->robotship[party[vflag]][nrob].status == DESTROYED) 
          {
          sprintf(str1,"Robotship destroyed at %03d %03d",m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1]);
          sort_msg(str1);
          eliminate_robotship(nrob,party[vflag]);
          usleep(100000);
          (void) shmdt(m_p);
          exit(0);
          }
        } while ( m_p->robotship[party[vflag]][nrob].fuel <= 0 || m_p->robotship[party[vflag]][nrob].main_energy < MIN_ROBOTSHIP_ENERGY);
       m_p->robotship[party[vflag]][nrob].status=ACTIVE;
       sort_msg("Robot status changed to ACTIVE");
       }
     if (abs(xziel-m_p->robotship[party[vflag]][nrob].pos[0]) < 2 && abs(yziel - m_p->robotship[party[vflag]][nrob].pos[1]) < 2 && (action_flag[0] == 'C' || action_flag[0] == 'c'))
        {
        xziel = m_p->robotship[party[vflag]][nrob].pos[0];
        yziel = m_p->robotship[party[vflag]][nrob].pos[1];
        capture(cxziel,cyziel,nrob);
        }
     usleep(ROBOT_DELAY);
     } while ( (m_p->robotship[party[vflag]][nrob].pos[0] != xziel || 
              m_p->robotship[party[vflag]][nrob].pos[1] != yziel) && 
              m_p->robotship[party[vflag]][nrob].status == ACTIVE && 
              m_p->robotship[party[vflag]][nrob].torp > 0 && 
              m_p->robotship[party[vflag]][nrob].fuel > 0);
   if (m_p->robotship[party[vflag]][nrob].status == DESTROYED)
     {
     sprintf(str1,"Robotship destroyed at %03d %03d",m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1]);
     sort_msg(str1);
     eliminate_robotship(nrob,party[vflag]);
     usleep(100000);
     (void) shmdt(m_p);
     exit(0);
     }
   sprintf(str1,"Robot %d: returning to Homeworld",nrob);
   sort_msg(str1);
   /* Rueckflug */
   if (m_p->robotship[party[vflag]][nrob].status == CANCELED) m_p->robotship[party[vflag]][nrob].status=ACTIVE;
   xziel=m_p->homeworld[party[vflag]].pos[0];
   yziel=m_p->homeworld[party[vflag]].pos[1];
   hflag=0;
   do
      {
      xsteps=xziel-m_p->robotship[party[vflag]][nrob].pos[0];
      ysteps=yziel-m_p->robotship[party[vflag]][nrob].pos[1];
      if (hflag == 0)  /* alte Richtung beibehalten bei Hindernis */ 
      {
        if (xsteps > 0)
        {
          xvec=fac;
        }
        else if (xsteps == 0)
        {
          xvec=0;
        } 
        else
        {
          xvec=-fac;
        }
        if (ysteps > 0) 
        {
          yvec=fac;
        }
        else if (ysteps == 0)
        {
          yvec=0;
        }
        else
        {
          yvec=-fac;
        }
      }
      else
      {
        hflag=0;
      }
      xneu=m_p->robotship[party[vflag]][nrob].pos[0]+xvec;
      yneu=m_p->robotship[party[vflag]][nrob].pos[1]+yvec;
      aneu=m_p->a[xneu][yneu];
      if (aneu == SPACE)
        {
        fac=1;
        m_p->a[m_p->robotship[party[vflag]][nrob].pos[0]][m_p->robotship[party[vflag]][nrob].pos[1]]=SPACE;
        m_p->a[xneu][yneu]=robsym;
        m_p->robotship[party[vflag]][nrob].pos[0]=xneu;
        m_p->robotship[party[vflag]][nrob].pos[1]=yneu;
        m_p->robotship[party[vflag]][nrob].fuel=m_p->robotship[party[vflag]][nrob].fuel-1;
        m_p->robotship[party[vflag]][nrob].main_energy=m_p->robotship[party[vflag]][nrob].main_energy-5;
        if (m_p->robotship[party[vflag]][nrob].main_energy < 0 || m_p->robotship[party[vflag]][nrob].status == DESTROYED) 
	  {
          sprintf(str1,"Robotship destroyed at %03d %03d",m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1]);
          sort_msg(str1);
          eliminate_robotship(nrob,party[vflag]);
          usleep(100000);
          (void) shmdt(m_p);
          exit(0);
          }
	/* Feuer auf feindliche Objekte (falls welche da sind) */
        fired=0;
        if (m_p->robotship[party[vflag]][nrob].torp > 0) fired=fire(action_flag[0],xneu,yneu,nrob); 
        /* fired>0: hat gefeuert; in Position verharren */
        if (m_p->robotship[party[vflag]][nrob].status == CANCELED) m_p->robotship[party[vflag]][nrob].status=ACTIVE;
        while (fired>0 && m_p->robotship[party[vflag]][nrob].torp>0 && m_p->robotship[party[vflag]][nrob].main_energy>0 && m_p->robotship[party[vflag]][nrob].status==ACTIVE) 
	  {
	  m_p->robotship[party[vflag]][nrob].main_energy=m_p->robotship[party[vflag]][nrob].main_energy-25;
	  m_p->robotship[party[vflag]][nrob].torp--;
	  usleep(ROBOT_DELAY);
          fired=fire(action_flag[0],xneu,yneu,nrob); 
          }
        } 
      else if (abs(xsteps) < 2 && abs(ysteps) < 2 )
        {
        xziel = m_p->robotship[party[vflag]][nrob].pos[0];
        yziel = m_p->robotship[party[vflag]][nrob].pos[1];
        }
      else /* Hindernis oder altes Feld , zufaellig andere Ziele anwaehlen */
        {
        random=ran();
        if (random < 0.125) 
          {	
          xvec = -fac;
          yvec =fac;
          }
        else if (random >= 0.125 && random < 0.25)
          {	
          xvec = 0;
          yvec = fac;
          }
        else if (random >= 0.25 && random < 0.375)
          {	
          xvec = fac;
          yvec = fac;
          }
        else if (random >= 0.375 && random < 0.5)
          {	
          xvec = fac;
          yvec = 0;
          }
        else if (random >= 0.5 && random < 0.625)
          {	
          xvec = fac;
          yvec = -fac;
          }
        else if (random >= 0.625 && random < 0.75)
          {	
          xvec = 0;
          yvec = -fac;
          }
        else if (random >= 0.75 && random < 0.875)
          {	
          xvec = -fac;
          yvec = -fac;
          }
        else 
          {	
          xvec = -fac;
          yvec = 0;
          }
        hflag=1;
        fac++;    
        }
     if (m_p->robotship[party[vflag]][nrob].fuel <= 0 || m_p->robotship[party[vflag]][nrob].main_energy < MIN_ROBOTSHIP_ENERGY)
       {
       sort_msg("Robot status changed to WAITING");
       m_p->robotship[party[vflag]][nrob].status=WAITING;
       do
        {
        usleep(10000000);
        m_p->robotship[party[vflag]][nrob].main_energy=m_p->robotship[party[vflag]][nrob].main_energy-2;
        if (m_p->robotship[party[vflag]][nrob].main_energy < 0 || m_p->robotship[party[vflag]][nrob].status == DESTROYED) 
          {
          sprintf(str1,"Robotship destroyed at %03d %03d",m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1]);
          sort_msg(str1);
          eliminate_robotship(nrob,party[vflag]);
          usleep(100000);
          (void) shmdt(m_p);
          exit(0);
          }
        if (m_p->robotship[party[vflag]][nrob].status == CANCELED) m_p->robotship[party[vflag]][nrob].status=ACTIVE;
        } while ( m_p->robotship[party[vflag]][nrob].fuel <= 0 || m_p->robotship[party[vflag]][nrob].main_energy < MIN_ROBOTSHIP_ENERGY);
       /* weiter, wenn Treibstoff oder Energie aufgefuellt wurde */
       m_p->robotship[party[vflag]][nrob].status=ACTIVE;
       sort_msg("Robot status changed to ACTIVE");
       }
     usleep(ROBOT_DELAY);
     } while ( (m_p->robotship[party[vflag]][nrob].pos[0] != xziel ||
	       m_p->robotship[party[vflag]][nrob].pos[1] != yziel) &&
               m_p->robotship[party[vflag]][nrob].status == ACTIVE &&
               m_p->robotship[party[vflag]][nrob].torp > 0 && 
 	       m_p->robotship[party[vflag]][nrob].fuel > 0);
  }

if (m_p->robotship[party[vflag]][nrob].main_energy < 0 || m_p->robotship[party[vflag]][nrob].status == DESTROYED)
  {
  sprintf(str1,"Robotship destroyed at %03d %03d",m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1]);
  sort_msg(str1);
  eliminate_robotship(nrob,party[vflag]);
  (void) shmdt(m_p);
  exit(0);
  }

/* HW-Dock nach Patrouillen-Ende */
m_p->a[m_p->robotship[party[vflag]][nrob].pos[0]][m_p->robotship[party[vflag]][nrob].pos[1]]=SPACE;
m_p->robotship[party[vflag]][nrob].pos[0]=m_p->homeworld[party[vflag]].pos[0];
m_p->robotship[party[vflag]][nrob].pos[1]=m_p->homeworld[party[vflag]].pos[1];
m_p->robotship[party[vflag]][nrob].status=HW_DOCKED;
m_p->robotship[party[vflag]][nrob].mission='N';
m_p->robotship[party[vflag]][nrob].fuel=ROBOTSHIP_FUEL;
m_p->robotship[party[vflag]][nrob].torp=ROBOTSHIP_TORPEDOES;
m_p->robotship[party[vflag]][nrob].shield=OFF;
m_p->robotship[party[vflag]][nrob].sh_energy=ROBOTSHIP_SH_ENERGY;
m_p->robotship[party[vflag]][nrob].main_energy=ROBOTSHIP_ENERGY;
m_p->robotship[party[vflag]][nrob].dam=0;
sprintf(str1,"Robot %d: docked at Homeworld",nrob);
sort_msg(str1);
usleep(10000);
(void) shmdt(m_p);
exit(0);
}

int find_nr_sat(x,y)
    int x,y;
/******************************************************************/     
/***  laufende Nr. des Satelliten auf x,y                       ***/
/******************************************************************/
{
	int i,nr;
	
i=0;
nr=-1;
while (i<MAX_SATS && nr == -1)
    {
    if (m_p->satellite[i].pos[0] == x && m_p->satellite[i].pos[1] == y) nr=i;
    i++;
    }
    
return(nr);
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


int incident(w)
    int w;
/************************************************************************/
/*** berechnet, ob das Ereignis mit der Wahrscheinlchkeit w eintrifft  **/
/*** w = 1...100 , incident=0 =nicht eing.                            ***/
/************************************************************************/
{
int diff;
float rnd;

rnd=ran();

rnd=rnd*100.0;
diff=(NINT(rnd));


if (diff<w) 
   {
   return(1);
   }
else
   {
   return(0);
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

void sig_hnd()
/***************************************************************************/
/*** Signalhandling                                                      ***/
/***************************************************************************/
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

void eliminate_robotship(nrob,p)
     int nrob,p;
{

m_p->robotship[p][nrob].status = DESTROYED;
m_p->robotship[p][nrob].mission = 'N';
m_p->a[m_p->robotship[p][nrob].pos[0]][m_p->robotship[p][nrob].pos[1]] = EXPLOSION;
usleep(EXPL_TIME);
m_p->a[m_p->robotship[p][nrob].pos[0]][m_p->robotship[p][nrob].pos[1]] = SPACE;
m_p->robotship[p][nrob].pos[0] = -99;
m_p->robotship[p][nrob].pos[1] = -99;
m_p->robotship[p][nrob].dam = 0;
m_p->robotship[p][nrob].sh_energy = 0;
m_p->robotship[p][nrob].fuel = 0;
m_p->robotship[p][nrob].torp = 0;
m_p->robotship[p][nrob].shield = OFF;
m_p->robotship[p][nrob].main_energy = 10;

return;                   
}

int fire(p,x,y,nrob)
     int x,y,nrob;
     char p;
{
int i,j,k,kship,xstart,ystart,xend,yend,lfire,xd,yd;
int diffship,diffbase,difffort,diffsat,kbase,kfort,ksat,kscout,khw,krob;
int diffrob,robparty;
char obj,xz_str[4],yz_str[4],xs_str[4],ys_str[4];

/* Umgebung um x,y absuchen, ggf. Radarlee */
xstart=x-SHIP_RANGE;
if (xstart<0) xstart=0;
ystart=y-SHIP_RANGE;
if (ystart<0) ystart=0;
xend=x+SHIP_RANGE;
if (xend>=ARRAY_SIZE) xend=ARRAY_SIZE-1;
yend=y+SHIP_RANGE;
if (yend>=ARRAY_SIZE) yend=ARRAY_SIZE-1;

lfire=0;

/*********************/
/* Planeteneroberung */
/*********************/
if (p=='c' || p=='C')
   {
     return lfire; /* z.Z. wird nicht gefeuert, sondern Mission ausgefuehrt */
   } /* Planeteneroberung Ende */       

/*****************/
/* Minenraeumung */
/*****************/
if (p=='m' || p=='M')
   {
   for (i=xstart;i<=xend;i++)
     {
     for (j=ystart;j<=yend;j++)
       {
       obj=m_p->a[i][j];
       if (obj == BLIND) usleep(2*RTORP_DELAY);
       obj=m_p->a[i][j];
       if ( ((m_p->radarlee==ON && behind(i,j,m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],SHIP_RANGE)==0 && wegfrei(m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],i,j)==0) || (m_p->radarlee==OFF && wegfrei(m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],i,j)==0)) && lfire==0 && (obj==MINE || obj==BLIND)) 
        {
	lfire++;
        if ( fork() == 0 )
          {
          sprintf(xs_str,"%d",x);
          sprintf(ys_str,"%d",y);
          sprintf(xz_str,"%d",i);
          sprintf(yz_str,"%d",j);
          execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
          }
	}
       }
     }
   return lfire;
   } /* Minenraeumung Ende */       

/************************/
/* Sprungtorbeseitigung */
/************************/
if (p=='j' || p=='J')
   {
   for (i=xstart;i<=xend;i++)
     {
     for (j=ystart;j<=yend;j++)
       {
       obj=m_p->a[i][j];
       if ( ((m_p->radarlee==ON && behind(i,j,m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],SHIP_RANGE)==0 && wegfrei(m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],i,j)==0) || (m_p->radarlee==OFF && wegfrei(m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],i,j)==0)) && lfire==0 && obj==JUMPGATE) 
        {
	lfire++;
        if ( fork() == 0 )
          {
          sprintf(xs_str,"%d",x);
          sprintf(ys_str,"%d",y);
          sprintf(xz_str,"%d",i);
          sprintf(yz_str,"%d",j);
          execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
          }
	}
       }
     }
   return lfire;
   } /* Sprungtor Ende */       

/**************/
/* Patrouille */
/**************/
if (p=='p' || p=='P')
   {
   kship=kbase=kfort=ksat=kscout=khw=krob=-1;
   diffship=diffbase=difffort=diffsat=diffrob=SHIP_RANGE+1;
   for (i=xstart;i<=xend;i++)
     {
     for (j=ystart;j<=yend;j++)
       {
       if ( ((m_p->radarlee==ON && behind(i,j,m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],SHIP_RANGE)==0 && wegfrei(m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],i,j)==0) || (m_p->radarlee==OFF && wegfrei(m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1],i,j)==0)) && lfire==0 ) 
        /* d.h. sichtbar */
        {
	obj=m_p->a[i][j];
	/* Reihenfolge: Rom.,Schiffe,Rob.,Scout,Basen,HW,Forts,Sats */
        if (obj==ROMULAN && lfire==0) /* Nur ein R.: sofort feuern */
	  {
	  lfire++;
          if ( fork() == 0 )
            {
            sprintf(xs_str,"%d",x);
            sprintf(ys_str,"%d",y);
	    sprintf(xz_str,"%d",i);
	    sprintf(yz_str,"%d",j);
            execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
            }
          }
        if (lfire==0)
          {
	  for (k=1;k<MAX_USER;k++) /* Schiffe: naechstes feindl. finden */
	    {
	      if (party[k]!=party[vflag] && ships[k][0]==obj)
	      {
	      xd=abs(x-i);
	      yd=abs(y-j);
              if ( (IMIN(xd,yd)) < diffship ) 
	        {
	        diffship=(IMIN(xd,yd));
                kship=k;
		}
	      }
	    }
          if (kship==-1 && lfire==0 && 
	      ((party[vflag]==IMPERIAL && obj==F_ROBOTSHIP) ||
	       (party[vflag]==FEDERAL && obj==E_ROBOTSHIP)) )
              /* noch kein Rom. u. Schiff gefunden: Robots checken */
	    {
	    robparty=FEDERAL;
	    if (party[vflag]==FEDERAL) robparty=IMPERIAL; 
            for (k=0;k<MAX_ROBOTSHIPS;k++) /* Rob.: naechsten feindl. finden */
	      {
	      if (m_p->robotship[robparty][k].status==ACTIVE)
	        {
	        xd=abs(x-i);
	        yd=abs(y-j);
                if ( (IMIN(xd,yd)) < diffrob ) 
	          {
	          diffrob=(IMIN(xd,yd));
                  krob=k;
		  }
	        }
	      }
            }
          if (kship==-1 && krob==-1 && lfire==0) 
          /* noch kein Rom., Schiff, Rob. gefunden: Scout checken */
	    {
	    if (obj==F_SCOUT && party[vflag]!=FEDERAL && m_p->scout[FEDERAL].dam>(MAX_DAM_SCOUT/2)) kscout++;
	    if (obj==E_SCOUT && party[vflag]!=IMPERIAL && m_p->scout[IMPERIAL].dam>(MAX_DAM_SCOUT/2)) kscout++;
	    }

          if (kship==-1  && krob==-1 && kscout==-1 && lfire==0) 
          /* noch kein Rom., Schiff, Rob., Scout gefunden: Basen checken */
	    {
	    if ( (obj==F_BASE && party[vflag]!=FEDERAL) || (obj==E_BASE && party[vflag]!=IMPERIAL) )
              {
              xd=abs(x-i);              
	      yd=abs(y-j);
              if ( (IMIN(xd,yd)) < diffbase ) 
		 {
		 diffbase=(IMIN(xd,yd));
                 kbase=find_nr_base(i,j);
		 }
	      }
	    }
          if (kship==-1 && kscout==-1 && kbase==-1  && krob==-1 && lfire==0) 
          /* noch kein Rom., Schiff, Rob., Scout, Basis gefunden: HW checken - es gibt nur eine feindliche */
	    {
            if (party[vflag]==FEDERAL && obj==E_HOMEWORLD) khw=IMPERIAL;
            if (party[vflag]==IMPERIAL && obj==F_HOMEWORLD) khw=FEDERAL;
	    }
          /* FORTS */
          if (kbase==-1 && kship==-1 && kscout==-1 && khw==-1  && krob==-1 && lfire==0)
	    {
	    if ( (obj==F_FORT && party[vflag]!=FEDERAL) || (obj==E_FORT && party[vflag]!=IMPERIAL) )
              {
              xd=abs(x-i);              
	      yd=abs(y-j);
              if ( (IMIN(xd,yd)) < difffort ) 
		 {
		 difffort=(IMIN(xd,yd));
                 kfort=find_nr_fort(i,j);
		 }
	      }
	    }
          /* SAT */
          if (kbase==-1 && kship==-1 && kfort==-1 && kscout==-1 && khw==-1  && krob==-1 && lfire==0)
	    {
	    if ( (obj==F_SAT && party[vflag]!=FEDERAL) || (obj==E_SAT && party[vflag]!=IMPERIAL) )
              {
              xd=abs(x-i);              
	      yd=abs(y-j);
              if ( (IMIN(xd,yd)) < diffsat ) 
		 {
		 diffsat=(IMIN(xd,yd));
                 ksat=find_nr_sat(i,j);
		 }
	      }
	    }
	  }
	}
       }
     }
   if (kship>0 && kship<MAX_USER && lfire==0)
      {
      lfire++;
      if ( fork() == 0 )
         {
         sprintf(xs_str,"%d",x);
         sprintf(ys_str,"%d",y);
	 sprintf(xz_str,"%d",m_p->ship[kship].pos[0]);
	 sprintf(yz_str,"%d",m_p->ship[kship].pos[1]);
         execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
         }
      }
   if (krob>0 && krob<MAX_ROBOTSHIPS && lfire==0)
      {
      lfire++;
      if ( fork() == 0 )
         {
         sprintf(xs_str,"%d",x);
         sprintf(ys_str,"%d",y);
	 sprintf(xz_str,"%d",m_p->robotship[robparty][krob].pos[0]);
	 sprintf(yz_str,"%d",m_p->robotship[robparty][krob].pos[1]);
         execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
         }
      }
   if (kscout>=0 && lfire==0)
      {
      lfire++;
      if ( fork() == 0 )
         {
         sprintf(xs_str,"%d",x);
         sprintf(ys_str,"%d",y);
         if (party[vflag]==FEDERAL)
	   { 
           sprintf(xz_str,"%d",m_p->scout[IMPERIAL].pos[0]);
           sprintf(yz_str,"%d",m_p->scout[IMPERIAL].pos[1]);
           }
         else
           {
           sprintf(xz_str,"%d",m_p->scout[FEDERAL].pos[0]);
           sprintf(yz_str,"%d",m_p->scout[FEDERAL].pos[1]);
           }
         execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
         }
      }
   else if (kbase>=0 && kbase<MAX_BASES && lfire==0)
      {
      lfire++;
      if ( fork() == 0 )
         {
         sprintf(xs_str,"%d",x);
         sprintf(ys_str,"%d",y);
	 sprintf(xz_str,"%d",m_p->base[kbase].pos[0]);
	 sprintf(yz_str,"%d",m_p->base[kbase].pos[1]);
         execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
         }
      }
   else if (khw>=0 && lfire==0) /* HW */
      {
      lfire++;
      if ( fork() == 0 )
         {
         sprintf(xs_str,"%d",x);
         sprintf(ys_str,"%d",y);
	 sprintf(xz_str,"%d",m_p->homeworld[khw].pos[0]);
	 sprintf(yz_str,"%d",m_p->homeworld[khw].pos[1]);
         execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
         }
      }
   else if (kfort>=0 && kfort<MAX_FORTS && lfire==0)
      {
      lfire++;
      if ( fork() == 0 )
         {
         sprintf(xs_str,"%d",x);
         sprintf(ys_str,"%d",y);
	 sprintf(xz_str,"%d",m_p->fort[kfort].pos[0]);
	 sprintf(yz_str,"%d",m_p->fort[kfort].pos[1]);
         execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
         }
      }
   else if (ksat>=0 && ksat<MAX_SATS && lfire==0)
      {
      lfire++;
      if ( fork() == 0 )
         {
         sprintf(xs_str,"%d",x);
         sprintf(ys_str,"%d",y);
	 sprintf(xz_str,"%d",m_p->satellite[ksat].pos[0]);
	 sprintf(yz_str,"%d",m_p->satellite[ksat].pos[1]);
         execl("rtorpedo",xs_str,ys_str,xz_str,yz_str,NULL);
         }
      }
   return lfire;
   } /* Patrouille */

return lfire;
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
    /* BLIND darf nicht als Hindernis wirken wg. Torpedobeschuss durch Feind */
    if (aneu != SPACE && aneu != EXPLOSION && aneu != BLIND) cond=1;
    i++;       
    } 
     
return(cond);
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

void capture(xpos,ypos,nrob)
     int xpos,ypos,nrob;
/**********************************************************************/
/*** Capture eines Planeten                                         ***/
/**********************************************************************/

{
  int i,x,y,nrp,hit_en;
  char str1[MSG_LENGTH];

x=xpos; y=ypos;
/* x und y verifizieren */

if ( m_p->a[x][y] != PLANET)
   {
   sort_msg("Ro: No planet at that coord.!");
   return;
   }
if ( m_p->robotship[party[vflag]][nrob].dam > MAX_DAM_ROBOTSHIP/2)
   {
   sort_msg("Ro: Damages too severe!");
   sort_msg("Ro: Cannot capture planet!");
   return;
   }
if ( m_p->robotship[party[vflag]][nrob].main_energy < ROBOTSHIP_CA_ENERGY)
   {
   sort_msg("Ro: Not enough energy!");
   sort_msg("Ro: Cannot capture planet!");
   return;
   }
/* Abfrage, ob Planet zur eigenen Partei gehoert */
nrp=find_nr_pl(x,y);
if (m_p->planet[nrp].party == party[vflag])
   {
   sort_msg("Ro: Planet already on our side!");
   return;
   }

/* alles o.k. */
sort_msg("Ro: Capturing planet...");
hit_en=50*m_p->planet[nrp].level;
gauss2(&hit_en);
m_p->robotship[party[vflag]][nrob].dam=m_p->robotship[party[vflag]][nrob].dam+hit_en;
m_p->robotship[party[vflag]][nrob].main_energy=(m_p->robotship[party[vflag]][nrob].main_energy-ROBOTSHIP_CA_ENERGY);
if (m_p->robotship[party[vflag]][nrob].dam > MAX_DAM_ROBOTSHIP) m_p->robotship[party[vflag]][nrob].status=DESTROYED;
/* Schleife mit Abfrage, ob Ro oder Planet zerstoert */
for (i=1; i<=m_p->planet[nrp].level; i++)
   {
   if (m_p->robotship[party[vflag]][nrob].main_energy < 0 || m_p->robotship[party[vflag]][nrob].status == DESTROYED)
      {
      sprintf(str1,"Robotship destroyed at %03d %03d",m_p->robotship[party[vflag]][nrob].pos[0],m_p->robotship[party[vflag]][nrob].pos[1]);
      sort_msg(str1);
      eliminate_robotship(nrob,party[vflag]);
      usleep(100000);
      (void) shmdt(m_p);
      exit(0);
      }
   usleep(ROBOTSHIP_CAPTURE_TIME);
   if ( m_p->a[x][y] != PLANET)
      {
      sort_msg("Planet destroyed during capture");
      return;
      }
   }

nrp=find_nr_pl(x,y);  /* neu wegen delay */
m_p->planet[nrp].party=party[vflag];
sort_msg("Ro: Planet captured!");
m_p->score[party[vflag]]=m_p->score[party[vflag]]+(20*m_p->planet[nrp].level);
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

int find_nr_pl(x,y)
    int x,y;
/******************************************************************/     
/***  laufende Nr. des Planeten auf x,y                         ***/
/******************************************************************/
{
int i,nr;
i=0;
nr=-1;
while (i<MAX_PLANETS && nr == -1)
    {
    if (m_p->planet[i].pos[0] == x && m_p->planet[i].pos[1] == y) 
       {
       nr=i;
       if (nr != m_p->planet[i].nr) sort_msg("ERROR: wrong planet count");
       }
    i++;
    }
return(nr);
}

