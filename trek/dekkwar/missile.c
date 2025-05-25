/**********************************************************/
/*** missile.c                                          ***/
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
/*** 24.03.08: SH_DOWN/UP durch ON/OFF ersetzt          ***/
/*** 26.03.08: Umstellung auf struct ship               ***/
/*** 26.03.08  condition auch fuer Heimatwelten         ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 06.04.08: Missile path eingefuehrt                 ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/**********************************************************/
/*** 05.05.08: MISSILE_DELAY neu                        ***/
/*** 14.10.08: Anpassungen fuer Ubuntu 8.04 (includes)  ***/
/*** 23.10.08: Anpassungen -Wall bei gcc (Ubuntu 8.04)  ***/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 29.11.09: Treffer auf Robotship eingebaut (fehlte) ***/
/*** 31.12.09: Umgestellt auf init_expl()               ***/
/*** 31.12.09: Treffer auf Schw.L. ergab SPACE; korr.   ***/
/*** 31.12.09: seed() neu; ran() mit seed ueberarbeitet ***/
/*** 23.01.10: Keine Explosion des Romulaners mehr; das ***/
/***           macht R. selbst. Scoring bei R.-Vern.    ***/
/***           wie bei torpedo.c                        ***/
/*** 24.01.10: Keine Explosion des Scouts mehr; erfolgt ***/
/***           ausschliesslich durch den Scout selbst   ***/
/*** 24.01.10: Robotship: Elimination und Explosion in  ***/
/***           robot.c verlagert                        ***/
/*** 25.01.10: MISSILE_DELAY jetzt ueberall             ***/
/*** 26.01.10: Explosion bei Ende mit DESTROYED       ***/
/*** 27.01.10: Missile trifft Missile neu eingefuehrt   ***/
/*** 05.04.10: Fehler in for-Schleife in hit() entfernt ***/
/*** 05.04.10: eliminate_hw() um Robotschiffe ergaenzt  ***/
/*** 11.04.10: In smhit() Fall HOOD != Schiff eingebaut ***/
/*** 15.04.10: Umstellung auf m_p->mpath[MAX_MPATHS]    ***/
/*** 24.04.10: neue Konstantendefinitionen aus dw_def.h ***/
/*** 25.04.10: IMAX,IMIN,IABS ueberall in () gesetzt    ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/shm.h>
#include <unistd.h>
  
/*****************************************************************************/
/*** Includes                                                              ***/
/*****************************************************************************/
#include "mathfkt.h"     /* math. Funktionen (incl. <math.h> */
#include "dw_def.h"
#include "dw_shm.h"

/****************************************************************************/
/***                         Globale Variablen                            ***/
/****************************************************************************/
#include "ships.h"
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
extern void path();
extern void mhit();
extern void smhit();
extern void init_expl();
extern void eliminate_base();
extern void eliminate_fort();
extern void eliminate_sat();
extern void eliminate_pl();
extern void eliminate_jumpgate();
extern void eliminate_hw();
extern int find_nr_base();
extern int find_nr_robotship();
extern int find_nr_sat();
extern int find_nr_fort();
extern int find_nr_jumpgate();
extern int find_nr_pl();
extern int det_target();
extern int gauss();

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
int i,p,xziel,yziel,xsziel,ysziel,xstart,ystart;
int xneu,yneu,xsteps,ysteps,pnr;
int xalt,yalt,steps,zflag,hit_energy;
float rxdiff,rydiff;
char aneu,str[MSG_LENGTH];

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
sscanf(argv[1],"%d",&xziel); 
sscanf(argv[2],"%d",&yziel); 
sscanf(argv[3],"%d",&xsziel); 
sscanf(argv[4],"%d",&ysziel); 

/****************************************************************************/
/*** sig_hnd setzen fuer SIGKILL und SIGALRM                              ***/
/****************************************************************************/
signal(SIGALRM,sig_hnd);
signal(SIGINT,sig_hnd_ctrlc);
signal(SIGKILL,SIG_DFL);

p=party[vflag];
m_p->missile[p].status=ACTIVE;
m_p->missile[p].fuel=MISSILE_FUEL;
m_p->missile[p].dam=0;
m_p->missile[p].target[0]=xziel;
m_p->missile[p].target[1]=yziel;
m_p->missile[p].subtarget[0]=xsziel;
m_p->missile[p].subtarget[1]=ysziel;

if (xsziel==-11) /* -11: in dwmain fuer Path-Fall so gesetzt */
  {
  pnr=ysziel; /* hier ist ysziel die Pfad-Nr. */ 
  steps=m_p->imp[p][pnr];
  if (steps < 1 || steps > MISSILE_FUEL)
    {
    m_p->missile[p].status=DESTROYED;
    m_p->missile[p].fuel=MISSILE_FUEL;
    m_p->missile[p].dam=0;
    m_p->missile[p].pos[0]=-99;
    m_p->missile[p].pos[1]=-99;
    m_p->missile[p].target[0]=-99;
    m_p->missile[p].target[1]=-99;
    m_p->missile[p].subtarget[0]=-99;
    m_p->missile[p].subtarget[1]=-99;
    sort_msg("LM: bad path!");
    (void) shmdt(m_p);
    exit(0);
    }
  for (i=0;i<=MISSILE_FUEL;i++)
    {
    m_p->missile[p].path[i][0]=m_p->mpath[p][pnr][i][0];
    m_p->missile[p].path[i][1]=m_p->mpath[p][pnr][i][1];
    }
  xstart=m_p->missile[p].path[0][0];
  ystart=m_p->missile[p].path[0][1];
  steps=m_p->imp[p][pnr];
  sort_msg("Launching missile, destination:");
  sprintf(str,"%d %d via recorded path %d!",xziel,yziel,pnr);
  sort_msg(str);
  }
else
  {
  xstart=ystart=-99;
  i=1;
  do {
    if (xsziel >= m_p->homeworld[p].pos[0] && 
       ysziel >= m_p->homeworld[p].pos[1] && 
       m_p->a[m_p->homeworld[p].pos[0]+i][m_p->homeworld[p].pos[1]+i] == SPACE)
      {
      xstart=m_p->homeworld[p].pos[0]+i;
      ystart=m_p->homeworld[p].pos[1]+i;
      }
    else if (xsziel <= m_p->homeworld[p].pos[0] &&
            ysziel <= m_p->homeworld[p].pos[1] &&
       m_p->a[m_p->homeworld[p].pos[0]-i][m_p->homeworld[p].pos[1]-i] == SPACE)
      {
      xstart=m_p->homeworld[p].pos[0]-i;
      ystart=m_p->homeworld[p].pos[1]-i;
      }       
    else if (xsziel >= m_p->homeworld[p].pos[0] &&
            ysziel <= m_p->homeworld[p].pos[1] &&
       m_p->a[m_p->homeworld[p].pos[0]+i][m_p->homeworld[p].pos[1]-i] == SPACE)
      {
      xstart=m_p->homeworld[p].pos[0]+i;
      ystart=m_p->homeworld[p].pos[1]-i;
      }
    else if (xsziel <= m_p->homeworld[p].pos[0] &&
            ysziel >= m_p->homeworld[p].pos[1] &&
       m_p->a[m_p->homeworld[p].pos[0]-i][m_p->homeworld[p].pos[1]+i] == SPACE)
      {
      xstart=m_p->homeworld[p].pos[0]-i;
      ystart=m_p->homeworld[p].pos[1]+i;
      }
    else
      {
      i++;
      }
    } while ( (xstart == -99 || ystart == -99) &&
            m_p->missile[p].status == ACTIVE);
  sort_msg("Launching missile, destination:");
  sprintf(str,"%d %d via %d %d",xziel,yziel,xsziel,ysziel);
  sort_msg(str);
  }
m_p->missile[p].pos[0]=xstart;
m_p->missile[p].pos[1]=ystart;
m_p->a[xstart][ystart]=MISSILE;
usleep(MISSILE_DELAY/2);

if (xsziel==-11)
  {
  i=1;
  do {
    usleep(MISSILE_DELAY);
    xalt=m_p->missile[p].path[i-1][0];
    yalt=m_p->missile[p].path[i-1][1];
    xneu=m_p->missile[p].path[i][0];
    yneu=m_p->missile[p].path[i][1];
    aneu=m_p->a[xneu][yneu];    

    /* Kollision mit Obj., eigentlich nur BASIS,FORT,SAT,JG,R,",SCOUT,Schiff */
    if (aneu != SPACE)
       {
       zflag=det_target(aneu);
       if (zflag>=0 && m_p->ship[zflag].pos[0]!=xneu)
          {
          aneu=m_p->a[xneu][yneu]=SPACE;
          }
       }
    if (aneu != SPACE)
       {
       if (aneu==F_HOMEWORLD)
	 {
	 sort_msg("Missile hit Federal Homeworld!");
         } 
       else if (aneu==E_HOMEWORLD)
	 {
	 sort_msg("Missile hit Imperial Homeworld!");
         } 
       else if (aneu==F_BASE)
         {
	 sort_msg("Missile hit Federal Base!");
         } 
       else if (aneu==E_BASE)
         {
	 sort_msg("Missile hit Imperial Base!");
         } 
       else if (aneu==BLACK_HOLE)
         {
	 sort_msg("Missile swallowed by Black Hole!");
         } 
       else
         {
	 sprintf(str,"Missile hit %c!",aneu);
	 sort_msg(str);
         } 
       hit_energy=gauss();
       mhit(aneu,xneu,yneu,hit_energy);
       hit_energy=hit_energy/2;
       smhit(xneu,yneu-1,hit_energy);
       smhit(xneu,yneu+1,hit_energy);
       smhit(xneu-1,yneu-1,hit_energy);
       smhit(xneu-1,yneu,hit_energy);
       smhit(xneu-1,yneu+1,hit_energy);
       smhit(xneu+1,yneu-1,hit_energy);
       smhit(xneu+1,yneu,hit_energy);
       smhit(xneu+1,yneu+1,hit_energy);
       m_p->missile[p].status=DESTROYED;
       m_p->a[xalt][yalt]=SPACE;
       }
    if (m_p->missile[p].fuel <= 0 && m_p->missile[p].status==ACTIVE)
       {
       sort_msg("Missile ran out of fuel!");
       sort_msg("Missile destroyed!");
       m_p->missile[p].status=DESTROYED;
       }
    if (m_p->missile[p].status==ACTIVE)
       {
       m_p->a[xneu][yneu]=MISSILE;
       m_p->a[xalt][yalt]=SPACE;
       m_p->missile[p].fuel=m_p->missile[p].fuel-1;
       m_p->missile[p].pos[0]=xneu;
       m_p->missile[p].pos[1]=yneu;
       }
    if (m_p->missile[p].dam >= MAX_DAM_MISSILE) m_p->missile[p].status=DESTROYED; 
    i++;
    } while (i<=steps && m_p->missile[p].status==ACTIVE);
  }
else /* Flug zum Zwischenziel */
  {
  xsteps=xsziel-xstart;
  ysteps=ysziel-ystart;
  path(xsteps,ysteps,&rxdiff,&rydiff,&steps);
  xalt=xstart;
  yalt=ystart;
  i=1;
  do  {
    usleep(MISSILE_DELAY);
    /* Neue Position */
    xneu=xstart+ (NINT( (((float) i)*rxdiff) ));
    yneu=ystart+ (NINT( (((float) i)*rydiff) ));
    aneu=m_p->a[xneu][yneu];
    /* Kollision mit Objekt */
    if (aneu != SPACE) /* Geisterbild */
       {
       zflag=det_target(aneu);
       if (zflag>=0 && m_p->ship[zflag].pos[0]!=xneu)
          {
          aneu=m_p->a[xneu][yneu]=SPACE;
          }
       }
    if (aneu != SPACE)
       {
       if (aneu==F_HOMEWORLD)
	 {
	 sort_msg("Missile hit Federal Homeworld!");
         } 
       else if (aneu==E_HOMEWORLD)
	 {
	 sort_msg("Missile hit Imperial Homeworld!");
         } 
       else if (aneu==F_BASE)
         {
	 sort_msg("Missile hit Federal Base!");
         } 
       else if (aneu==E_BASE)
         {
	 sort_msg("Missile hit Imperial Base!");
         } 
       else if (aneu==BLACK_HOLE)
         {
	 sort_msg("Missile swallowed by Black Hole!");
         } 
       else
         {
	 sprintf(str,"Missile hit %c!",aneu);
	 sort_msg(str);
         } 
       hit_energy=gauss();
       mhit(aneu,xneu,yneu,hit_energy);
       hit_energy=hit_energy/2;
       smhit(xneu,yneu-1,hit_energy);
       smhit(xneu,yneu+1,hit_energy);
       smhit(xneu-1,yneu-1,hit_energy);
       smhit(xneu-1,yneu,hit_energy);
       smhit(xneu-1,yneu+1,hit_energy);
       smhit(xneu+1,yneu-1,hit_energy);
       smhit(xneu+1,yneu,hit_energy);
       smhit(xneu+1,yneu+1,hit_energy);
       m_p->missile[p].status=DESTROYED;
       m_p->a[xalt][yalt]=SPACE;
       }

    /* Umsetzen der Werte */
    if (m_p->missile[p].fuel <= 0 && m_p->missile[p].status==ACTIVE)
       {
       sort_msg("Missile ran out of fuel!");
       sort_msg("Missile destroyed!");
       m_p->missile[p].status=DESTROYED;
       }
    if (m_p->missile[p].status==ACTIVE)
       {
       m_p->a[xneu][yneu]=MISSILE;
       m_p->a[xalt][yalt]=SPACE;
       m_p->missile[p].fuel=m_p->missile[p].fuel-1;
       m_p->missile[p].pos[0]=xneu;
       m_p->missile[p].pos[1]=yneu;
       xalt=xneu;
       yalt=yneu;
       }
    if (m_p->missile[p].dam >= MAX_DAM_MISSILE) m_p->missile[p].status=DESTROYED;
    i++;
    } while (i<=steps && m_p->missile[p].status==ACTIVE);
  }
/* Zwischenziel oder Pfadende erreicht: jetzt von dort Flug zum Ziel */ 

if (m_p->missile[p].status==ACTIVE)
  {
  xalt=xstart=m_p->missile[p].pos[0];
  yalt=ystart=m_p->missile[p].pos[1];

  xsteps=xziel-xalt;
  ysteps=yziel-yalt;
  path(xsteps,ysteps,&rxdiff,&rydiff,&steps);

  i=1;
  do  {
    usleep(MISSILE_DELAY);
    /* Neue Position */
    xneu=xstart+ (NINT( (((float) i)*rxdiff) ));
    yneu=ystart+ (NINT( (((float) i)*rydiff) ));
    aneu=m_p->a[xneu][yneu];
    /* Kollision mit Objekt */
    if (aneu != SPACE) /* Geisterbild */
       {
       zflag=det_target(aneu);
       if (zflag>=0 && m_p->ship[zflag].pos[0]!=xneu)
          {
          aneu=m_p->a[xneu][yneu]=SPACE;
          }
       }
    if (aneu != SPACE)
       {
       if (aneu==F_HOMEWORLD)
	 {
	 sort_msg("Missile hit Federal Homeworld!");
         } 
       else if (aneu==E_HOMEWORLD)
	 {
	 sort_msg("Missile hit Imperial Homeworld!");
         } 
       else if (aneu==F_BASE)
         {
	 sort_msg("Missile hit Federal Base!");
         } 
       else if (aneu==E_BASE)
         {
	 sort_msg("Missile hit Imperial Base!");
         } 
       else if (aneu==BLACK_HOLE)
         {
	 sort_msg("Missile swallowed by Black Hole!");
         } 
       else
         {
	 sprintf(str,"Missile hit %c!",aneu);
	 sort_msg(str);
         } 
       hit_energy=gauss();
       mhit(aneu,xneu,yneu,hit_energy);
       hit_energy=hit_energy/2;
       smhit(xneu,yneu-1,hit_energy);
       smhit(xneu,yneu+1,hit_energy);
       smhit(xneu-1,yneu-1,hit_energy);
       smhit(xneu-1,yneu,hit_energy);
       smhit(xneu-1,yneu+1,hit_energy);
       smhit(xneu+1,yneu-1,hit_energy);
       smhit(xneu+1,yneu,hit_energy);
       smhit(xneu+1,yneu+1,hit_energy);
       m_p->missile[p].status=DESTROYED;
       m_p->a[xalt][yalt]=SPACE;
       }

    /* Umsetzen der Werte */
    if (m_p->missile[p].fuel <= 0 && m_p->missile[p].status==ACTIVE)
       {
       sort_msg("Missile ran out of fuel!");
       sort_msg("Missile destroyed!");
       m_p->missile[p].status=DESTROYED;
       }
    if (m_p->missile[p].status==ACTIVE)
       {
       m_p->a[xneu][yneu]=MISSILE;
       m_p->a[xalt][yalt]=SPACE;
       m_p->missile[p].fuel=m_p->missile[p].fuel-1;
       m_p->missile[p].pos[0]=xneu;
       m_p->missile[p].pos[1]=yneu;
       xalt=xneu;
       yalt=yneu;
       }
   if (m_p->missile[p].dam >= MAX_DAM_MISSILE) m_p->missile[p].status=DESTROYED;
   i++;
   } while (i<=steps && m_p->missile[p].status==ACTIVE);
  }
/* Ziel erreicht ohne auf Objekt getroffen zu sein: => BLIND */
if (m_p->missile[p].status==ACTIVE)
  {
  sort_msg("Missile reached destination!");
  sort_msg("No target hit, mission aborted!");
  m_p->a[xneu][yneu]=BLIND;
  }
 else
  {
  m_p->a[m_p->missile[p].pos[0]][m_p->missile[p].pos[1]]=EXPLOSION;
  usleep(EXPL_TIME);
  m_p->a[m_p->missile[p].pos[0]][m_p->missile[p].pos[1]]=SPACE;
  }

/* Ende */

m_p->missile[p].status=DESTROYED;
m_p->missile[p].fuel=MISSILE_FUEL;
m_p->missile[p].dam=0;
m_p->missile[p].pos[0]=-99;
m_p->missile[p].pos[1]=-99;
m_p->missile[p].target[0]=-99;
m_p->missile[p].target[1]=-99;
m_p->missile[p].subtarget[0]=-99;
m_p->missile[p].subtarget[1]=-99;
usleep(MISSILE_DELAY/10);
(void) shmdt(m_p);
exit(0);
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
      if (target == ships[0][0]) zflag=0; /* Romulaner */
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

int gauss()
/***************************************************************************/
/*** Berechnet die Trefferenergie (Gaussverteilung um MISSILE_EXPL)      ***/
/***************************************************************************/
{
int x1,x2;
float fx,rnd;

rnd=ran(); 
if (rnd>1.0 || rnd<0.0) sort_msg(" Ran falsch!");

fx=exp( ((-0.5)*(rnd-0.5)*(rnd-0.5)*25.0) );
x1=(MISSILE_EXPL-(NINT( (((float) MISSILE_EXPL)*(1.0-fx)))));
x2=(MISSILE_EXPL+(NINT( (((float) MISSILE_EXPL)*(1.0-fx)))));
if (rnd<0.5) 
   { 
   return(x1);
   }
else
   {
   return(x2);
   }

}

void eliminate_pl(nrp)
     int nrp;
/**********************************************************************/
/*** Setzt den Planeten Nr. nrp inaktiv                             ***/
/**********************************************************************/
{

if ( nrp >= MAX_PLANETS || nrp < 0 )
  {
  sort_msg("ERROR: wrong planet number!");
  }
else
  {
  m_p->planet[nrp].pos[0]=m_p->planet[nrp].pos[1]=-99;
  m_p->planet[nrp].party=NEUTRAL;
  m_p->planet[nrp].level=LEVEL_1;
  m_p->planet[nrp].dam=0;
  m_p->planet[nrp].status=DESTROYED;
  }
return;
}

void eliminate_sat(nrs)
     int nrs;
/**********************************************************************/
/*** Entfernt den Satelliten Nr.nrs aus den Satelliten-Werten       ***/
/**********************************************************************/
{

if (nrs<0 || nrs>=MAX_SATS) sort_msg("ERROR: wrong sat count!");
m_p->satellite[nrs].pos[0]=-99;       
m_p->satellite[nrs].pos[1]=-99;             
m_p->satellite[nrs].party=NEUTRAL;

return;
}

void eliminate_base(nrb)
     int nrb;
/**********************************************************************/
/*** Zerstoerung der Basis nrb. Beinhaltet die Vernichtung aller an ***/
/*** diese Basis gedockten Schiffe!                                 ***/
/**********************************************************************/
{
int xbas,ybas,i;

if (nrb < 0 || nrb >= MAX_BASES) sort_msg("Error: Wrong base count!"); 

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
/***   Achtung: clcok-Aufruf im Hauptprogramm noetig!                 ***/
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

void mhit(obj,x,y,energy)
     int x,y,energy;
     char obj;
/**************************************************************************/
/*** Trefferwirkung auf verschiedene Objekte                            ***/
/**************************************************************************/
{
int nr,pntsd,pnts,i,zflag,sdiff,nrob;
char print_str[MSG_LENGTH];
int mparty;

/**********************************/
/* Schwarzes Loch: nichts         */
/**********************************/
if (obj==BLACK_HOLE) return;

/**********************************/
/* STERN, Mine, Torpedo, Gaswolke */
/**********************************/ 
if (obj==STAR || obj==MINE || obj==CLOUD || obj==BLIND || obj==BLACK_HOLE || obj==TORPEDO)
   {
   init_expl(x,y,EXPL_TIME);
   return;
   }

/*********/
/* Forts */
/*********/
if (obj==F_FORT || obj==E_FORT)
   {
   nr=find_nr_fort(x,y);   
   if (nr == -99)
     {
     sort_msg("ERROR: invalid nr (fort)");
     return;
     }
   if (party[vflag]!=m_p->fort[nr].party) 
      {
      pnts=15;
      pntsd=150;
      }
   else
      {
      pnts=-15;
      pntsd=-150;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->fort[nr].dam=m_p->fort[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->fort[nr].party)
          {
          sprintf(print_str,"Fort at %03d %03d hit by missile",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->fort[nr].dam>=MAX_DAM_F)
      {
      eliminate_fort(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende Fort */   


/***********/
/* Missile */
/***********/
/* Da gegenwaertig nur ein Missile jeder Seite fliegen kann, ist mparty */
/* eigentlich ueberfluessig; als Vorgriff fuer mehr Missiles wurde es aber */
/* schon eingebaut */
if (obj==MISSILE)
   {
   if (x==m_p->missile[FEDERAL].pos[0] && y==m_p->missile[FEDERAL].pos[1] && m_p->missile[FEDERAL].status==ACTIVE)
      {
      mparty=FEDERAL;
      }
   else if (x==m_p->missile[IMPERIAL].pos[0] && y==m_p->missile[IMPERIAL].pos[1] && m_p->missile[IMPERIAL].status==ACTIVE)
      {
      mparty=IMPERIAL;
      }
   else
      {
      sort_msg("Hit error missile!");
      return;
      }

   if (party[vflag]==mparty)
      {
      pntsd=-500;
      }
   else
      {
      pntsd=500;
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
   m_p->missile[mparty].dam=m_p->missile[mparty].dam+energy;
   if (m_p->missile[mparty].dam>=MAX_DAM_MISSILE) m_p->missile[mparty].status=DESTROYED; 
   return;
   }       /* Ende missile */   

/*************/
/* Jumpgates */
/*************/
if (obj==JUMPGATE)
   {
   nr=find_nr_jumpgate(x,y);   
   if (nr == -99)
     {
     sort_msg("ERROR: invalid nr (jumpgate)");
     return;
     }
   if (party[vflag]!=m_p->jumpgate[nr].party) 
      {
      pnts=30;
      pntsd=300;
      }
   else
      {
      pnts=-10;
      pntsd=-100;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->jumpgate[nr].dam=m_p->jumpgate[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->jumpgate[nr].party)
          {
          sprintf(print_str,"Jumpg. at %03d %03d hit by missile!",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->jumpgate[nr].dam>=MAX_DAM_JG)
      {
      eliminate_jumpgate(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende Jumpgate */   


/**************/
/* Heimatwelt */
/**************/
if (obj==F_HOMEWORLD)
   {
   m_p->homeworld[FEDERAL].red_count=MAX_RED_COUNT;
   m_p->homeworld[FEDERAL].condition=RED;
   if (party[vflag]==IMPERIAL) 
      {
      pnts=500;
      pntsd=20000;
      }
   else
      {
      pnts=-500;
      pntsd=-20000;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->homeworld[FEDERAL].dam=m_p->homeworld[FEDERAL].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==FEDERAL)
          {
          sprintf(print_str,"> Homeworld hit by missile! <");
          send_msg(i,print_str);
          }
       }
   if (m_p->homeworld[FEDERAL].dam>=MAX_DAM_HOMEWORLD)
      {
      eliminate_hw(FEDERAL);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }
else if (obj==E_HOMEWORLD)
   {
   m_p->homeworld[IMPERIAL].red_count=MAX_RED_COUNT;
   m_p->homeworld[IMPERIAL].condition=RED;
   if (party[vflag]==FEDERAL) 
      {
      pnts=500;
      pntsd=20000;
      }
   else
      {
      pnts=-500;
      pntsd=-20000;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->homeworld[IMPERIAL].dam=m_p->homeworld[IMPERIAL].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==IMPERIAL)
          {
          sprintf(print_str,"> Homeworld hit by missile! <");
          send_msg(i,print_str);
          }
       }
   if (m_p->homeworld[IMPERIAL].dam>=MAX_DAM_HOMEWORLD)
      {
      eliminate_hw(IMPERIAL);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende Heimatwelt */   

/**********/
/* Planet */
/**********/
if (obj==PLANET)
   {
   nr=find_nr_pl(x,y);
   pnts=-15;
   pntsd=-150;
   /* Planetenvernichtung gibt immer Minuspunkte! */      
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->planet[nr].dam=m_p->planet[nr].dam+energy;
   if (m_p->planet[nr].dam>=(MAX_DAM_PL*m_p->planet[nr].level))
      {
      eliminate_pl(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende planet */   

/*********/
/* Scout */
/*********/
if (obj==E_SCOUT && m_p->scout[IMPERIAL].status==DESTROYED)
  {
  m_p->a[x][y]=SPACE;
  return;
  }
if (obj==F_SCOUT && m_p->scout[FEDERAL].status==DESTROYED)
  {
  m_p->a[x][y]=SPACE;
  return;
  }

if (obj==F_SCOUT)
  {
  m_p->scout[FEDERAL].dam=m_p->scout[FEDERAL].dam+energy;
  if (m_p->scout[FEDERAL].dam>=MAX_DAM_SCOUT)
     {
     if (party[vflag]==IMPERIAL)
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]+500;
       }
     else
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]-500;
       }
     m_p->scout[party[vflag]].status=DESTROYED; 
     return;
     }
  }
else if (obj==E_SCOUT)
  {
  m_p->scout[IMPERIAL].dam=m_p->scout[IMPERIAL].dam+energy;
  if (m_p->scout[IMPERIAL].dam>=MAX_DAM_SCOUT)
     {
     if (party[vflag]==FEDERAL)
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]+500;
       }
     else
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]-500;
       }
     m_p->scout[party[vflag]].status=DESTROYED; 
     return;
     }
  }

/*********/
/* Basis */
/*********/
if (obj==F_BASE || obj==E_BASE)
   {
   nr=find_nr_base(x,y);   
   m_p->base[nr].condition=RED;
   m_p->base[nr].red_count=MAX_RED_COUNT;
   if (party[vflag]!=m_p->base[nr].party) 
      {
      pnts=40;
      pntsd=500;
      }
   else
      {
      pnts=-40;
      pntsd=-500;      
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->base[nr].party)
          {
          sprintf(print_str,"Base at %03d %03d hit by missile",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->base[nr].sh==ON)
      {
      m_p->base[nr].dam=m_p->base[nr].dam+(energy/10);
      sdiff=m_p->base[nr].sh_energy-energy;
      if (sdiff<=0) 
         {
         m_p->base[nr].sh_energy=0;   
         m_p->base[nr].sh=OFF;   
         m_p->base[nr].dam=m_p->base[nr].dam-sdiff;
         }
      else
	 {
         m_p->base[nr].sh_energy=sdiff;   
         }      
      }
   else
      {
      m_p->base[nr].dam=m_p->base[nr].dam+energy;
      }
   if (m_p->base[nr].dam>=MAX_DAM_BAS)
      {
      eliminate_base(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      }
   return;
   }       /* Ende Basis */   

/*************/
/* Romulaner */
/*************/
if (obj==ROMULAN)
   {
   pnts=50;
   pntsd=max_dam[0];
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   if (m_p->ship[0].shield==ON)
      {
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+(energy/10);
      sdiff=m_p->ship[0].sh_energy-energy;
      if (sdiff<=0) 
         {
         m_p->ship[0].sh_energy=0;   
         m_p->ship[0].shield=OFF;   
         m_p->ship[0].dam[0]=m_p->ship[0].dam[0]-sdiff;
         }
      else
	 {
         m_p->ship[0].sh_energy=sdiff;
         }      
      }
   else
      {
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+energy;
      }
   if (m_p->ship[0].dam[0]>=max_dam[0])
      {
      m_p->l_exit[0]=EXIT;
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      }
   return;
   }       /* Ende Romulaner */   

/**********************/
/* Schiffe (und HOOD) */
/**********************/
zflag=-1;
if (obj==HOOD)
   {
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->ship[i].pos[0]==x && m_p->ship[i].pos[1]==y) zflag=i;
       }
   }
else
   {
   zflag=det_target(obj);
   }
   
if (zflag>0 && zflag<MAX_USER)
   {
   pnts=30+(level_of_ship[zflag]*10);
   pntsd=max_dam[zflag];
   if (party[vflag] == party[zflag])
      {
      pnts=-pnts;
      pntsd=-pntsd;
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;

   if (m_p->ship[zflag].shield==ON)
      {
      m_p->ship[zflag].dam[1]=m_p->ship[zflag].dam[1]+(energy/100);
      m_p->ship[zflag].dam[2]=m_p->ship[zflag].dam[2]+(energy/100);
      m_p->ship[zflag].dam[3]=m_p->ship[zflag].dam[3]+(energy/100);
      if (level_of_ship[zflag]>DESTROYER) m_p->ship[zflag].dam[4]=m_p->ship[zflag].dam[4]+(energy/100);
      m_p->ship[zflag].dam[5]=m_p->ship[zflag].dam[5]+(energy/100);
      m_p->ship[zflag].dam[6]=m_p->ship[zflag].dam[6]+(energy/100);
      m_p->ship[zflag].dam[7]=m_p->ship[zflag].dam[7]+(energy/100);
      m_p->ship[zflag].dam[8]=m_p->ship[zflag].dam[8]+(energy/100);
      m_p->ship[zflag].dam[9]=m_p->ship[zflag].dam[9]+(energy/100);
      m_p->ship[zflag].dam[10]=m_p->ship[zflag].dam[10]+(energy/100);
      sdiff=m_p->ship[zflag].sh_energy-energy;
      if (sdiff<=0) 
         {
         m_p->ship[zflag].sh_energy=0;   
         m_p->ship[zflag].shield=OFF;
         m_p->ship[zflag].dam[1]=m_p->ship[zflag].dam[1]-(sdiff/10);
         m_p->ship[zflag].dam[2]=m_p->ship[zflag].dam[2]-(sdiff/10);
         m_p->ship[zflag].dam[3]=m_p->ship[zflag].dam[3]-(sdiff/10);
         if (level_of_ship[zflag]>DESTROYER) m_p->ship[zflag].dam[4]=m_p->ship[zflag].dam[4]-(sdiff/10);
         m_p->ship[zflag].dam[5]=m_p->ship[zflag].dam[5]-(sdiff/10);
         m_p->ship[zflag].dam[6]=m_p->ship[zflag].dam[6]-(sdiff/10);
         m_p->ship[zflag].dam[7]=m_p->ship[zflag].dam[7]-(sdiff/10);
         m_p->ship[zflag].dam[8]=m_p->ship[zflag].dam[8]-(sdiff/10);
         m_p->ship[zflag].dam[9]=m_p->ship[zflag].dam[9]-(sdiff/10);
         m_p->ship[zflag].dam[10]=m_p->ship[zflag].dam[10]-(sdiff/10);
         }
      else
	 {
         m_p->ship[zflag].sh_energy=sdiff;
         }      
      }
   else
      {
      m_p->ship[zflag].dam[1]=m_p->ship[zflag].dam[1]+(energy/10);
      m_p->ship[zflag].dam[2]=m_p->ship[zflag].dam[2]+(energy/10);
      m_p->ship[zflag].dam[3]=m_p->ship[zflag].dam[3]+(energy/10);
      if (level_of_ship[zflag]>DESTROYER) m_p->ship[zflag].dam[4]=m_p->ship[zflag].dam[4]+(energy/10);
      m_p->ship[zflag].dam[5]=m_p->ship[zflag].dam[5]+(energy/10);
      m_p->ship[zflag].dam[6]=m_p->ship[zflag].dam[6]+(energy/10);
      m_p->ship[zflag].dam[7]=m_p->ship[zflag].dam[7]+(energy/10);
      m_p->ship[zflag].dam[8]=m_p->ship[zflag].dam[8]+(energy/10);
      m_p->ship[zflag].dam[9]=m_p->ship[zflag].dam[9]+(energy/10);
      m_p->ship[zflag].dam[10]=m_p->ship[zflag].dam[10]+(energy/10);
      }
   if (zflag != vflag)
      {
      sprintf(print_str,"%s hit by missile",ships[zflag]);
      send_msg(zflag,print_str);
      }
   sprintf(print_str,"** We got a %d units hit! **!",energy);
   send_msg(zflag,print_str);
   usleep(110000);
   if (m_p->ship[zflag].dam[0]>=max_dam[zflag])
      {
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
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
      sort_msg(" ** Ghost image, vanished! **");
      m_p->a[x][y]=SPACE;
      return;
      }
   if (party[vflag] != FEDERAL) 
      {
      pnts=50;
      pntsd=800;
      }
   else
      {
      pnts=-50;
      pntsd=-800;      
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==FEDERAL)
          {
          sprintf(print_str,">R.ship at %03d %03d under fire!<",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->robotship[FEDERAL][nrob].shield==ON)
      {
      sort_msg("Hit deflected by shields!");
      m_p->robotship[FEDERAL][nrob].sh_energy=m_p->robotship[FEDERAL][nrob].sh_energy-energy;
      if (m_p->robotship[FEDERAL][nrob].sh_energy<0) m_p->robotship[FEDERAL][nrob].sh_energy=0;
      m_p->robotship[FEDERAL][nrob].dam=m_p->robotship[FEDERAL][nrob].dam+(energy/10); 
      if (m_p->robotship[FEDERAL][nrob].sh_energy<=0) 
         {
         m_p->robotship[FEDERAL][nrob].shield=OFF;
         sort_msg("Shields of robotship broke down!");
         }
      }
   else
      {
      m_p->robotship[FEDERAL][nrob].dam=m_p->robotship[FEDERAL][nrob].dam+energy;
      }
   if (m_p->robotship[FEDERAL][nrob].dam>=MAX_DAM_ROBOTSHIP)
      {
      m_p->robotship[FEDERAL][nrob].status=DESTROYED;
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }
if (obj==E_ROBOTSHIP)
   {
   nrob=find_nr_robotship(x,y,IMPERIAL);   
   if (nrob < 0 || nrob >=MAX_ROBOTSHIPS)
      {
      sort_msg(" ** Ghost image, vanished! **");
      m_p->a[x][y]=SPACE;
      return;
      }
   if (party[vflag] != IMPERIAL) 
      {
      pnts=50;
      pntsd=800;
      }
   else
      {
      pnts=-50;
      pntsd=-800;      
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==IMPERIAL)
          {
          sprintf(print_str,"R.ship at %03d %03d under fire!",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->robotship[IMPERIAL][nrob].shield==ON)
      {
      sort_msg("Hit deflected by shields!");
      m_p->robotship[IMPERIAL][nrob].sh_energy=m_p->robotship[IMPERIAL][nrob].sh_energy-energy;
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<0) m_p->robotship[IMPERIAL][nrob].sh_energy=0;
      m_p->robotship[IMPERIAL][nrob].dam=m_p->robotship[IMPERIAL][nrob].dam+(energy/10); 
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<=0) 
         {
         m_p->robotship[IMPERIAL][nrob].shield=OFF;
         sort_msg("Shields of robotship broke down!");
         }
      }
   else
      {
      m_p->robotship[IMPERIAL][nrob].dam=m_p->robotship[IMPERIAL][nrob].dam+energy;
      }
   if (m_p->robotship[IMPERIAL][nrob].dam>=MAX_DAM_ROBOTSHIP)
      {
      m_p->robotship[IMPERIAL][nrob].status=DESTROYED;
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       
/* Ende Robotschiff */   


/************/
/* Satellit */
/************/
if (obj == F_SAT || obj == E_SAT)
   {
   nr=find_nr_sat(x,y);
   if (m_p->satellite[nr].party==party[vflag])
             {
             pnts=-5;
             }
          else
             {
             pnts=5;
             }
   eliminate_sat(nr);
   init_expl(x,y,EXPL_TIME);
   sprintf(print_str,"Missile hit sat. at %03d %03d",x,y);
   sort_msg(print_str);
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   return;
   }

sort_msg(" ERROR fehlerhaftes Objekt!");
return;
}

void smhit(x,y,energy)
     int x,y,energy;
/**************************************************************************/
/*** Trefferwirkung auf benachbarte Sektoren um Missile-Einschlag       ***/
/**************************************************************************/
{
int nr,pntsd,pnts,i,zflag,sdiff;
char print_str[MSG_LENGTH],obj;

/** Check der Koordinaten **/
if (x < 0 || y < 0 || y >= ARRAY_SIZE || x >= ARRAY_SIZE) return;

obj=m_p->a[x][y];

/** Nichts, wenn... **/
if (obj==SPACE || obj==CLOUD || obj==BLACK_HOLE || obj==MISSILE) return;

/*****************************************/
/* Objekte, die sofort vernichtet werden */
/*****************************************/ 
if (obj==STAR || obj==MINE || obj==BLIND || obj==TORPEDO)
   {
   init_expl(x,y,EXPL_TIME);
   return;
   }

/*********/
/* Forts */
/*********/
if (obj==F_FORT || obj==E_FORT)
   {
   nr=find_nr_fort(x,y);   
   if (nr == -99)
     {
     sort_msg("ERROR: invalid nr (fort)");
     return;
     }
   if (party[vflag]!=m_p->fort[nr].party) 
      {
      pnts=15;
      pntsd=150;
      }
   else
      {
      pnts=-15;
      pntsd=-150;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->fort[nr].dam=m_p->fort[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->fort[nr].party)
          {
          sprintf(print_str,"Fort at %03d %03d hit by explosion",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->fort[nr].dam>=MAX_DAM_F)
      {
      eliminate_fort(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende Fort */   


/*************/
/* Jumpgates */
/*************/
if (obj==JUMPGATE)
   {
   nr=find_nr_jumpgate(x,y);   
   if (nr == -99)
     {
     sort_msg("ERROR: invalid nr (jumpgate)");
     return;
     }
   if (party[vflag]!=m_p->jumpgate[nr].party) 
      {
      pnts=30;
      pntsd=300;
      }
   else
      {
      pnts=-10;
      pntsd=-100;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->jumpgate[nr].dam=m_p->jumpgate[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->jumpgate[nr].party)
          {
          sprintf(print_str,"Jumpg. at %d %d hit by explosion",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->jumpgate[nr].dam>=MAX_DAM_JG)
      {
      eliminate_jumpgate(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende Jumpgate */   


/**************/
/* Heimatwelt */
/**************/
if (obj==F_HOMEWORLD)
   {
   if (party[vflag]==IMPERIAL) 
      {
      pnts=500;
      pntsd=20000;
      }
   else
      {
      pnts=-500;
      pntsd=-20000;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->homeworld[FEDERAL].dam=m_p->homeworld[FEDERAL].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==FEDERAL)
          {
          sprintf(print_str,"> Homeworld hit by explosion! <");
          send_msg(i,print_str);
          }
       }
   if (m_p->homeworld[FEDERAL].dam>=MAX_DAM_HOMEWORLD)
      {
      eliminate_hw(FEDERAL);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }
else if (obj==E_HOMEWORLD)
   {
   if (party[vflag]==FEDERAL) 
      {
      pnts=500;
      pntsd=20000;
      }
   else
      {
      pnts=-500;
      pntsd=-20000;      
      }
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->homeworld[IMPERIAL].dam=m_p->homeworld[IMPERIAL].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==IMPERIAL)
          {
          sprintf(print_str,"> Homeworld hit by explosion! <");
          send_msg(i,print_str);
          }
       }
   if (m_p->homeworld[IMPERIAL].dam>=MAX_DAM_HOMEWORLD)
      {
      eliminate_hw(IMPERIAL);
      init_expl(x,y,EXPL_TIME);
      sprintf(print_str,"Imp.Homew. at %03d %03d destroyed!",x,y); 
      sort_msg(print_str);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende Heimatwelt */   

/**********/
/* Planet */
/**********/
if (obj==PLANET)
   {
   nr=find_nr_pl(x,y);
   pnts=-15;
   pntsd=-150;
   /* Planetenvernichtung gibt immer Minuspunkte! */      
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->planet[nr].dam=m_p->planet[nr].dam+energy;
   if (m_p->planet[nr].dam>=(MAX_DAM_PL*m_p->planet[nr].level))
      {
      eliminate_pl(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }       /* Ende planet */   

/*********/
/* Scout */
/*********/
if (obj==E_SCOUT && m_p->scout[IMPERIAL].status==DESTROYED)
  {
  m_p->a[x][y]=SPACE;
  return;
  }
if (obj==F_SCOUT && m_p->scout[FEDERAL].status==DESTROYED)
  {
  m_p->a[x][y]=SPACE;
  return;
  }

if (obj==F_SCOUT)
  {
  m_p->scout[FEDERAL].dam=m_p->scout[FEDERAL].dam+energy;
  if (m_p->scout[FEDERAL].dam>=MAX_DAM_SCOUT)
     {
     init_expl(x,y,EXPL_TIME);
     if (party[vflag]==IMPERIAL)
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]+500;
       }
     else
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]-500;
       }
     m_p->scout[party[vflag]].target[0]=-99;
     m_p->scout[party[vflag]].target[1]=-99;
     m_p->scout[party[vflag]].pos[0]=-99;
     m_p->scout[party[vflag]].pos[1]=-99;
     m_p->scout[party[vflag]].dam=0;
     m_p->scout[party[vflag]].status=DESTROYED; 
     return;
     }
  }
else if (obj==E_SCOUT)
  {
  m_p->scout[IMPERIAL].dam=m_p->scout[IMPERIAL].dam+energy;
  if (m_p->scout[IMPERIAL].dam>=MAX_DAM_SCOUT)
     {
     init_expl(x,y,EXPL_TIME);
     if (party[vflag]==FEDERAL)
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]+500;
       }
     else
       {
       m_p->score[party[vflag]]=m_p->score[party[vflag]]-500;
       }
     m_p->scout[party[vflag]].target[0]=-99;
     m_p->scout[party[vflag]].target[1]=-99;
     m_p->scout[party[vflag]].pos[0]=-99;
     m_p->scout[party[vflag]].pos[1]=-99;
     m_p->scout[party[vflag]].dam=0;
     m_p->scout[party[vflag]].status=DESTROYED; 
     return;
     }
  }

/*********/
/* Basis */
/*********/
if (obj==F_BASE || obj==E_BASE)
   {
   nr=find_nr_base(x,y);   
   m_p->base[nr].condition=RED;
   m_p->base[nr].red_count=MAX_RED_COUNT;
   if (party[vflag]!=m_p->base[nr].party) 
      {
      pnts=40;
      pntsd=500;
      }
   else
      {
      pnts=-40;
      pntsd=-500;      
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->base[nr].party)
          {
          sprintf(print_str,"Base at %03d %03d hit by explosion",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->base[nr].sh==ON)
      {
      m_p->base[nr].dam=m_p->base[nr].dam+(energy/10);
      sdiff=m_p->base[nr].sh_energy-energy;
      if (sdiff<=0) 
         {
         m_p->base[nr].sh_energy=0;   
         m_p->base[nr].sh=OFF;   
         m_p->base[nr].dam=m_p->base[nr].dam-sdiff;
         }
      else
	 {
         m_p->base[nr].sh_energy=sdiff;   
         }      
      }
   else
      {
      m_p->base[nr].dam=m_p->base[nr].dam+energy;
      }
   if (m_p->base[nr].dam>=MAX_DAM_BAS)
      {
      eliminate_base(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      }
   return;
   }       /* Ende Basis */   

/*************/
/* Romulaner */
/*************/
if (obj==ROMULAN)
   {
   pnts=50;
   pntsd=1000-(level_of_ship[vflag]*150);      
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   if (m_p->ship[0].shield==ON)
      {
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+(energy/10);
      sdiff=m_p->ship[0].sh_energy-energy;
      if (sdiff<=0) 
         {
         m_p->ship[0].sh_energy=0;   
         m_p->ship[0].shield=OFF;   
         m_p->ship[0].dam[0]=m_p->ship[0].dam[0]-sdiff;
         }
      else
	 {
         m_p->ship[0].sh_energy=sdiff;
         }      
      }
   else
      {
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+energy;
      }
   if (m_p->ship[0].dam[0]>=max_dam[0])
      {
      m_p->l_exit[0]=EXIT;
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      }
   return;
   }       /* Ende Romulaner */   

/**********************/
/* Schiffe (und HOOD) */
/**********************/
zflag=-2;
if (obj==HOOD)
   {
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->ship[i].pos[0]==x && m_p->ship[i].pos[1]==y) zflag=i;
       }
   }
else
   {
   zflag=det_target(obj);
   }
   
if (zflag>0 && zflag<MAX_USER)
   {
   pnts=30+(level_of_ship[zflag]*10);
   pntsd=max_dam[zflag];
   if (party[vflag] == party[zflag])
      {
      pnts=-pnts;
      pntsd=-pntsd;
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;

   if (m_p->ship[zflag].shield==ON)
      {
      m_p->ship[zflag].dam[1]=m_p->ship[zflag].dam[1]+(energy/100);
      m_p->ship[zflag].dam[2]=m_p->ship[zflag].dam[2]+(energy/100);
      m_p->ship[zflag].dam[3]=m_p->ship[zflag].dam[3]+(energy/100);
      if (level_of_ship[zflag]>DESTROYER) m_p->ship[zflag].dam[4]=m_p->ship[zflag].dam[4]+(energy/100);
      m_p->ship[zflag].dam[5]=m_p->ship[zflag].dam[5]+(energy/100);
      m_p->ship[zflag].dam[6]=m_p->ship[zflag].dam[6]+(energy/100);
      m_p->ship[zflag].dam[7]=m_p->ship[zflag].dam[7]+(energy/100);
      m_p->ship[zflag].dam[8]=m_p->ship[zflag].dam[8]+(energy/100);
      m_p->ship[zflag].dam[9]=m_p->ship[zflag].dam[9]+(energy/100);
      m_p->ship[zflag].dam[10]=m_p->ship[zflag].dam[10]+(energy/100);
      sdiff=m_p->ship[zflag].sh_energy-energy;
      if (sdiff<=0) 
         {
         m_p->ship[zflag].sh_energy=0;   
         m_p->ship[zflag].shield=OFF;
         m_p->ship[zflag].dam[1]=m_p->ship[zflag].dam[1]-(sdiff/10);
         m_p->ship[zflag].dam[2]=m_p->ship[zflag].dam[2]-(sdiff/10);
         m_p->ship[zflag].dam[3]=m_p->ship[zflag].dam[3]-(sdiff/10);
         if (level_of_ship[zflag]>DESTROYER) m_p->ship[zflag].dam[4]=m_p->ship[zflag].dam[4]-(sdiff/10);
         m_p->ship[zflag].dam[5]=m_p->ship[zflag].dam[5]-(sdiff/10);
         m_p->ship[zflag].dam[6]=m_p->ship[zflag].dam[6]-(sdiff/10);
         m_p->ship[zflag].dam[7]=m_p->ship[zflag].dam[7]-(sdiff/10);
         m_p->ship[zflag].dam[8]=m_p->ship[zflag].dam[8]-(sdiff/10);
         m_p->ship[zflag].dam[9]=m_p->ship[zflag].dam[9]-(sdiff/10);
         m_p->ship[zflag].dam[10]=m_p->ship[zflag].dam[10]-(sdiff/10);
         }
      else
	 {
         m_p->ship[zflag].sh_energy=sdiff;
         }      
      }
   else
      {
      m_p->ship[zflag].dam[1]=m_p->ship[zflag].dam[1]+(energy/10);
      m_p->ship[zflag].dam[2]=m_p->ship[zflag].dam[2]+(energy/10);
      m_p->ship[zflag].dam[3]=m_p->ship[zflag].dam[3]+(energy/10);
      if (level_of_ship[zflag]>DESTROYER) m_p->ship[zflag].dam[4]=m_p->ship[zflag].dam[4]+(energy/10);
      m_p->ship[zflag].dam[5]=m_p->ship[zflag].dam[5]+(energy/10);
      m_p->ship[zflag].dam[6]=m_p->ship[zflag].dam[6]+(energy/10);
      m_p->ship[zflag].dam[7]=m_p->ship[zflag].dam[7]+(energy/10);
      m_p->ship[zflag].dam[8]=m_p->ship[zflag].dam[8]+(energy/10);
      m_p->ship[zflag].dam[9]=m_p->ship[zflag].dam[9]+(energy/10);
      m_p->ship[zflag].dam[10]=m_p->ship[zflag].dam[10]+(energy/10);
      }
   if (zflag != vflag)
      {
      sprintf(print_str,"%s hit by explosion",ships[zflag]);
      send_msg(zflag,print_str);
      }
   sprintf(print_str,"** We got a %d units hit! **!",energy);
   send_msg(zflag,print_str);
   usleep(110000);
   if (m_p->ship[zflag].dam[0]>=max_dam[zflag])
      {
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      }
   return;
   }
 else if (zflag == -2) 
 /* d.h. getarntes Missile oder anderes getarntes Objekt: z.Z. nix */
   {
   return;
   }

/************/
/* Satellit */
/************/
if (obj == F_SAT || obj == E_SAT)
   {
   nr=find_nr_sat(x,y);
   if (m_p->satellite[nr].party==party[vflag])
             {
             pnts=-5;
             }
          else
             {
             pnts=5;
             }
   eliminate_sat(nr);
   init_expl(x,y,EXPL_TIME);
   sprintf(print_str,"Sat. at %03d %03d hit by explosion",x,y);
   sort_msg(print_str);
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   return;
   }

sort_msg(" ERROR fehlerhaftes Objekt!");
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

void eliminate_hw(ef)
     int ef;
/***************************************************/
/* Entfernt die Heimatwelt von ef aus dem Spiel  ***/                  
/***************************************************/
{
int i;

/* Aktive Schiffe HW_DOCKED werden zerstoert */
/* d.h. Schiffe HW_DOCKED aber operational bleiben nutzbar */
/* Robotschiffe werden vernichtet */

if (ef == IMPERIAL && m_p->homeworld[ef].status == ACTIVE)
  {
  m_p->homeworld[ef].status=DESTROYED;
  m_p->a[m_p->homeworld[ef].pos[0]][m_p->homeworld[ef].pos[1]]=SPACE;
  m_p->homeworld[ef].pos[0]=m_p->homeworld[ef].pos[1]=-99;
  for (i=0; i<MAX_ROBOTSHIPS; i++)
    {
    if (m_p->robotship[IMPERIAL][i].status==HW_DOCKED)
      {
      m_p->robotship[IMPERIAL][i].status = DESTROYED;
      m_p->robotship[IMPERIAL][i].mission = 'N';
      m_p->robotship[IMPERIAL][i].pos[0] = -99;
      m_p->robotship[IMPERIAL][i].pos[1] = -99;
      m_p->robotship[IMPERIAL][i].dam = 0;
      m_p->robotship[IMPERIAL][i].sh_energy = 0;
      m_p->robotship[IMPERIAL][i].fuel = 0;
      m_p->robotship[IMPERIAL][i].torp = 0;
      m_p->robotship[IMPERIAL][i].shield = OFF;
      m_p->robotship[IMPERIAL][i].main_energy = 10;
      }
    else
      {
      m_p->robotship[IMPERIAL][i].status = DESTROYED;
      }
    }
  for (i=1;i<MAX_USER;i++)
    {
    send_msg(i,"Imperial Homeworld destroyed!");
    if (party[i]==IMPERIAL && m_p->ship[i].dock==HW_DOCKED) m_p->l_exit[i]=EXIT;
    }
  }
else if (ef == FEDERAL && m_p->homeworld[ef].status == ACTIVE)
  {
  m_p->homeworld[ef].status=DESTROYED;
  m_p->a[m_p->homeworld[ef].pos[0]][m_p->homeworld[ef].pos[1]]=SPACE;
  m_p->homeworld[ef].pos[0]=m_p->homeworld[ef].pos[1]=-99;
  for (i=0; i<MAX_ROBOTSHIPS; i++)
    {
    if (m_p->robotship[FEDERAL][i].status==HW_DOCKED)
      {
      m_p->robotship[FEDERAL][i].status = DESTROYED;
      m_p->robotship[FEDERAL][i].mission = 'N';
      m_p->robotship[FEDERAL][i].pos[0] = -99;
      m_p->robotship[FEDERAL][i].pos[1] = -99;
      m_p->robotship[FEDERAL][i].dam = 0;
      m_p->robotship[FEDERAL][i].sh_energy = 0;
      m_p->robotship[FEDERAL][i].fuel = 0;
      m_p->robotship[FEDERAL][i].torp = 0;
      m_p->robotship[FEDERAL][i].shield = OFF;
      m_p->robotship[FEDERAL][i].main_energy = 10;
      }
    else
      {
      m_p->robotship[FEDERAL][i].status = DESTROYED;
      }
    }
  for (i=1;i<MAX_USER;i++)
    {
    send_msg(i,"Federal Homeworld destroyed!");
    if (party[i]==FEDERAL && m_p->ship[i].dock==HW_DOCKED) m_p->l_exit[i]=EXIT;
    }
  }
else
  {
  sort_msg(" ERROR - Homeworld, Code 1");
  }

return;
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

/*****************************************************************************/
/*** eliminate_jumpgate.c                                                  ***/
/*****************************************************************************/
/*** Zerstoerung des jumpgates mit dem Index nr                            ***/
/*****************************************************************************/
void eliminate_jumpgate(nr)
     int nr;
{
m_p->jumpgate[nr].status = DESTROYED;
m_p->a[m_p->jumpgate[nr].pos[0]][m_p->jumpgate[nr].pos[1]] = SPACE;
m_p->jumpgate[nr].pos[0] = -99;
m_p->jumpgate[nr].pos[1] = -99;
m_p->jumpgate[nr].dam = 0;
m_p->jumpgate[nr].code = 0;
m_p->jumpgate[nr].party = NEUTRAL;

return;                   
}

/*****************************************************************************/
/*** find_nr_jumpgate.c                                                    ***/
/*****************************************************************************/
/*** Index-Nr. des Sprungtores auf x,y bestimmen                           ***/
/*** Falls keines gefunden, wird -99 zurueckgegeben                        ***/
/*****************************************************************************/
int find_nr_jumpgate(x,y)
    int x,y;
{
int i,nr;

nr=-99;
i=0;
while (nr == -99 && i < MAX_JUMPGATES)
    {
    if (m_p->jumpgate[i].pos[0] == x && m_p->jumpgate[i].pos[1] == y && m_p->jumpgate[i].status == ACTIVE)
       {
       nr=i;
       }
    i++;
    }
return(nr);
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


