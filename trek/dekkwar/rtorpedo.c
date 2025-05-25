/**********************************************************/
/*** rtorpedo.c                                         ***/
/*** Torpedo eines Robotschiffs                         ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 29.11.09: Treffer Scout in thit umgeschrieben      ***/
/*** 25.12.09: Treffer auf EXPLOSION anders und init_   ***/
/***           expl in thit() (siehe auch torpedo.c)    ***/
/*** 23.01.10: Keine Explosion des Romulaners mehr; das ***/
/***           macht R. selbst. Scoring bei R.-Vern.    ***/
/***           wie bei torpedo.c                        ***/
/*** 23.01.10: seed() neu; ran() mit seed ueberarbeitet ***/
/*** 23.01.10: RTORP_DELAY neu                          ***/
/*** 24.01.10: Keine Explosion des Scouts mehr; erfolgt ***/
/***           ausschliesslich durch den Scout selbst;  ***/
/***           doppeltes Scoring bei Scout-Zerstoerung  ***/
/***           beseitigt                                ***/
/*** 24.01.10: Robotship: Elimination und Explosion in  ***/
/***           robot.c verlagert                        ***/
/*** 26.01.10: Keine Explosion des Miss. mehr; erfolgt  ***/
/***           ausschliesslich durch Missile selbst;    ***/
/***           Meldungen "under fire" und "destroyed"   ***/
/***           entfernt                                 ***/
/*** 03.04.10: Zu lange Nachrichten gekuerzt            ***/
/*** 05.04.10: Fehler in for-Schleife in hit() entfernt ***/
/*** 05.04.10: eliminate_hw() um Robotschiffe ergaenzt  ***/
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

/****************************************************************************/
/***                         Globale Variablen                            ***/
/****************************************************************************/
#include "ships.h"

/***************************************************************************/
/***                         Unterprogramme                              ***/
/***************************************************************************/

extern float ran();
extern int seed();
extern key_t qgetkey();
extern void path();
extern int det_target();
extern int gauss1();
extern void gauss2();
extern void thit();
extern void send_msg();
extern void eliminate_base();
extern void eliminate_pl();
extern void eliminate_hw();
extern void eliminate_fort();
extern void eliminate_sat();
extern void eliminate_jumpgate();
extern void init_expl();
extern int find_nr_base();
extern int find_nr_pl();
extern int find_nr_fort();
extern int find_nr_sat();
extern int find_nr_robotship();
extern int find_nr_jumpgate();
extern int incident();
extern int behind();
extern int wegfrei();

int main(argc,argv)
/***************************************************************************/
/*** rtorpedo  : Feuern eines Torpedos durch Robotschiff                 ***/
/***************************************************************************/
    int argc;
    char *argv[];
    
{
/***************************************************************************/
/***                    lokale Variablen                                 ***/
/***************************************************************************/
int xsteps,ysteps,steps,i,xziel,yziel,zflag;
int xneu,yneu,xalt,yalt,xstart,ystart,rtparty;
int hit_energy,nrob;
char aneu,aalt;
float rxdiff,rydiff;

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
/***                    argv[0] konvertieren                              ***/
/****************************************************************************/
sscanf(argv[0],"%d",&xstart); 
sscanf(argv[1],"%d",&ystart); 
sscanf(argv[2],"%d",&xziel); 
sscanf(argv[3],"%d",&yziel); 

if (m_p->a[xstart][ystart]==F_ROBOTSHIP)
  {
  rtparty=FEDERAL;
  }
else if (m_p->a[xstart][ystart]==E_ROBOTSHIP)
  {
  rtparty=IMPERIAL;
  }
else
  {
  (void) shmdt(m_p);
  exit(0);
  }

xsteps=xziel-xstart;
ysteps=yziel-ystart;
path(xsteps,ysteps,&rxdiff,&rydiff,&steps);

/***************************************************************************/
/***                         "Flug"                                      ***/
/***************************************************************************/
xalt=xstart;
yalt=ystart;

aalt=SPACE;
for (i=1;i<=SHIP_RANGE;++i)
   {
    usleep(RTORP_DELAY);
    /* Neue Position */
    xneu=xstart+ (NINT( (((float) i)*rxdiff) ));
    xneu=xstart+ (NINT( (((float) i)*rxdiff) ));
    yneu=ystart+ (NINT( (((float) i)*rydiff) ));
    aneu=m_p->a[xneu][yneu];
    if ((xneu<0 || yneu<0 || xneu>=ARRAY_SIZE || yneu>=ARRAY_SIZE) && i > 1)
       {
       m_p->a[xalt][yalt]=aalt;       
       (void) shmdt(m_p);
       exit(0);
       }
    if (aneu != SPACE)
       {
       if (i>1) m_p->a[xalt][yalt]=aalt;       
       zflag=det_target(aneu);
     /* Geisterbild */
       if (zflag>=0 && m_p->ship[zflag].pos[0]!=xneu)
          {
          aneu=m_p->a[xneu][yneu]=SPACE;
          (void) shmdt(m_p);
          exit(0);
          }
       if (aneu==BLACK_HOLE)
	 {
	 (void) shmdt(m_p);
	 exit(0);
         }
       if (aneu==CLOUD)
          {
	  init_expl(xneu,yneu,EXPL_TIME);
          (void) shmdt(m_p);
          exit(0);
          }
       if (aneu == EXPLOSION)   
          {
          (void) shmdt(m_p);
          exit(0);
          }
       if (aneu==STAR || aneu==E_SAT || aneu==F_SAT || aneu==MINE || aneu==BLIND)
          {
          thit(aneu,xneu,yneu,0,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }

      /* Trefferwirkung fuer alle anderen Objekte */
          hit_energy=gauss1();
      /* Homeworld getroffen */
       if (aneu==E_HOMEWORLD || aneu==F_HOMEWORLD)
          {
          thit(aneu,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }
      /* Planet getroffen */
       if (aneu==PLANET)
          {
          thit(PLANET,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }
      /* Robotschiff getroffen */
       if (aneu==E_ROBOTSHIP || aneu==F_ROBOTSHIP)
          {
	  nrob=-1;
	  if (aneu==F_ROBOTSHIP) nrob=find_nr_robotship(xneu,yneu,FEDERAL);   
	  if (aneu==E_ROBOTSHIP) nrob=find_nr_robotship(xneu,yneu,IMPERIAL);
          if (nrob < 0 || nrob >=MAX_ROBOTSHIPS)
            {
            m_p->a[xneu][yneu]=SPACE;
            (void) shmdt(m_p);
            exit(0);
            }
          if (aneu==E_ROBOTSHIP) thit(E_ROBOTSHIP,xneu,yneu,hit_energy,rtparty); 
          if (aneu==F_ROBOTSHIP) thit(F_ROBOTSHIP,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }
      /* Jumpgate getroffen */
       if (aneu==JUMPGATE)
          {
          thit(JUMPGATE,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }
      /* Missile getroffen */
       if (aneu==MISSILE)
          {
          thit(MISSILE,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }
      /* SCOUT getroffen */
       if (aneu==E_SCOUT || aneu==F_SCOUT)
          {
          thit(aneu,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }
       /* Fort getroffen */
       if (aneu==F_FORT || aneu==E_FORT)
          {
          thit(aneu,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }
       /* Basis getroffen */
       if (aneu==F_BASE || aneu==E_BASE)
          {
          thit(aneu,xneu,yneu,hit_energy,rtparty);
          (void) shmdt(m_p);
          exit(0);
          }
       /* Hood getroffen */
       if (aneu==HOOD)
          {
          thit(HOOD,xneu,yneu,hit_energy,rtparty); 
          (void) shmdt(m_p);
          exit(0);
          }          
       /* Schiff getroffen (inkl. Romulaner) */
       if (zflag!=-1)
          {
          thit(aneu,xneu,yneu,hit_energy,rtparty);
          (void) shmdt(m_p);
          exit(0);
          }
       (void) shmdt(m_p);
       exit(0);
       }
    /* Umsetzen der Werte */
    m_p->a[xneu][yneu]=BLIND;
    if (i>1) m_p->a[xalt][yalt]=aalt;       	
    xalt=xneu;
    yalt=yneu;
    aalt=aneu;
    }
    
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

int gauss1()
/***************************************************************************/
/*** Berechnet die Trefferenergie bei Torpedos (Gaussverteilung um 250)  ***/
/***************************************************************************/
{
int x1,x2;
float fx,rnd;

rnd=ran(); 

fx=exp( ((-0.5)*(rnd-0.5)*(rnd-0.5)*25.0) );
x1=(RTORP_POWER-(NINT( (((float) RTORP_POWER)*(1.0-fx)))));
x2=(RTORP_POWER+(NINT( (((float) RTORP_POWER)*(1.0-fx)))));
if (rnd<0.5) 
   { 
   return(x1);
   }
else
   {
   return(x2);
   }
/*return(x1);*/
}

void eliminate_pl(nrp)
     int nrp;
/**********************************************************************/
/*** Setzt den Planeten Nr. nrp inaktiv                             ***/
/**********************************************************************/
{

m_p->planet[nrp].pos[0]=m_p->planet[nrp].pos[1]=-99;
m_p->planet[nrp].party=NEUTRAL;
m_p->planet[nrp].level=LEVEL_1;
m_p->planet[nrp].dam=0;
m_p->planet[nrp].status=DESTROYED;

return;
}

void eliminate_sat(nrs)
     int nrs;
/**********************************************************************/
/*** Entfernt den Satelliten Nr.nrs aus den Satelliten-Werten       ***/
/**********************************************************************/
{

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


void thit(obj,x,y,energy,rtparty)
     int x,y,energy,rtparty;
     char obj;
/**************************************************************************/
/*** Trefferwirkung auf veschiedene Objekte obj                         ***/
/**************************************************************************/
{
int nr,nrob,pntsd,pnts,i,zflag,j,he,device,mparty,tparty;
float random;
char print_str[MSG_LENGTH],n_obj;

/**********/
/* Sterne */
/**********/
if (obj==STAR)
   {
   if (incident(50)==0)   /* Stern explodiert nicht ===> Ende */
      {
      m_p->score[rtparty]=m_p->score[rtparty]-5;      
      return;
      }
   init_expl(x,y,EXPL_TIME);
   m_p->score[rtparty]=m_p->score[rtparty]-15;      
   /* Umgebung absuchen */
   for (i=(x-1);i<(x+2);i++)
       {
       for (j=(y-1);j<(y+2);j++)
           {
           if (i!=x || j!=y)
              {
              if (i>=0 && i<(ARRAY_SIZE-1) && j>=0 && j<(ARRAY_SIZE-1))
                 {
                 n_obj=m_p->a[i][j];
                 if (n_obj!=SPACE && n_obj!=BLACK_HOLE)
                    {
                    he=1200;
                    gauss2(&he);
                    thit(n_obj,i,j,he);
                    }
                 }
              }
           }
       }  
   return;
   }


/********/
/* Mine */
/********/ 
if (obj==MINE)
   {
   init_expl(x,y,EXPL_TIME);
   return;
   }

/*********/
/* Blind */
/*********/
if (obj==BLIND || obj==TORPEDO || obj == EXPLOSION)
   {
   init_expl(x,y,EXPL_TIME);
   return;
   }    


/**********/
/* Clouds */
/**********/
if (obj==CLOUD)
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
   if (rtparty!=m_p->fort[nr].party) 
      {
      pnts=15;
      pntsd=150;
      }
   else
      {
      pnts=-15;
      pntsd=-150;      
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   m_p->fort[nr].dam=m_p->fort[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->fort[nr].party)
          {
          sprintf(print_str,">Fort at %03d %03d under fire!<",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->fort[nr].dam>=MAX_DAM_F)
      {
      eliminate_fort(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
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
   if (rtparty!=m_p->jumpgate[nr].party) 
      {
      pnts=30;
      pntsd=300;
      }
   else
      {
      pnts=-10;
      pntsd=-100;      
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   m_p->jumpgate[nr].dam=m_p->jumpgate[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->jumpgate[nr].party)
          {
          sprintf(print_str,">Jumpg. at %03d %03d under fire!<",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->jumpgate[nr].dam>=MAX_DAM_JG)
      {
      eliminate_jumpgate(nr);
      init_expl(x,y,EXPL_TIME);
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
      return; 
      }
   return;
   }       /* Ende Jumpgate */   

/***********/
/* Missile */
/***********/
if (obj==MISSILE && m_p->missile[IMPERIAL].status==DESTROYED && m_p->missile[FEDERAL].status==DESTROYED)
  {
  m_p->a[x][y]=SPACE;
  return;
  }

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
      return;
      }

   if (rtparty==mparty)
      {
      pntsd=-500;
      }
   else
      {
      pntsd=500;
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
   m_p->missile[mparty].dam=m_p->missile[mparty].dam+energy;
   if (m_p->missile[mparty].dam>=MAX_DAM_MISSILE) m_p->missile[mparty].status=DESTROYED; 
   }       /* Ende missile */   

/**************/
/* Heimatwelt */
/**************/
if (obj==F_HOMEWORLD)
   {
   m_p->homeworld[FEDERAL].red_count=MAX_RED_COUNT;
   m_p->homeworld[FEDERAL].condition=RED;
   if (rtparty==IMPERIAL) 
      {
      pnts=50;
      pntsd=10000;
      }
   else
      {
      pnts=-50;
      pntsd=-10000;      
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   m_p->homeworld[FEDERAL].dam=m_p->homeworld[FEDERAL].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==FEDERAL)
          {
          sprintf(print_str,"> Homeworld under fire! <");
          send_msg(i,print_str);
          }
       }
   if (m_p->homeworld[FEDERAL].dam>=MAX_DAM_HOMEWORLD)
      {
      eliminate_hw(FEDERAL);
      init_expl(x,y,EXPL_TIME);
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
      return; 
      }
   return;
   }
if (obj==E_HOMEWORLD)
   {
   m_p->homeworld[IMPERIAL].red_count=MAX_RED_COUNT;
   m_p->homeworld[IMPERIAL].condition=RED;
   if (rtparty==FEDERAL) 
      {
      pnts=50;
      pntsd=10000;
      }
   else
      {
      pnts=-50;
      pntsd=-10000;      
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   m_p->homeworld[IMPERIAL].dam=m_p->homeworld[IMPERIAL].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==IMPERIAL)
          {
          sprintf(print_str,"> Homeworld under fire! <");
          send_msg(i,print_str);
          }
       }
   if (m_p->homeworld[IMPERIAL].dam>=MAX_DAM_HOMEWORLD)
      {
      init_expl(x,y,EXPL_TIME);
      eliminate_hw(IMPERIAL);
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
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
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   m_p->planet[nr].dam=m_p->planet[nr].dam+energy;
   if (m_p->planet[nr].dam>=(MAX_DAM_PL*m_p->planet[nr].level))
      {
      init_expl(x,y,EXPL_TIME);
      eliminate_pl(nr);
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
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

if (obj==E_SCOUT || obj==F_SCOUT)
  {
  tparty=IMPERIAL;
  if (obj == F_SCOUT) tparty=FEDERAL;
  if (party[rtparty] != tparty)
     {
     pnts=100;
     pntsd=500;      
     }
  else
     {
     pnts=-100;
     pntsd=-500;      
     }
  for (i=1;i<MAX_USER;i++)
    {
    if (m_p->luser[i]==ACTIVE && party[i]==tparty)
       {
       sprintf(print_str,">Scout at %03d %03d under fire!<",x,y);
       send_msg(i,print_str);
       }
    }
  m_p->scout[tparty].dam=m_p->scout[tparty].dam+energy;
  m_p->user_score[rtparty]=m_p->user_score[rtparty]+pnts;
  m_p->name_score[rtparty]=m_p->name_score[rtparty]+pnts;   
  m_p->score[party[rtparty]]=m_p->score[party[rtparty]]+pnts;
  if (m_p->scout[tparty].dam>=MAX_DAM_SCOUT)
     {
     m_p->user_score[rtparty]=m_p->user_score[rtparty]+pntsd;
     m_p->name_score[rtparty]=m_p->name_score[rtparty]+pntsd;   
     m_p->score[party[rtparty]]=m_p->score[party[rtparty]]+pntsd;
     m_p->scout[tparty].status=DESTROYED; 
     }
  return;
  }       /* Ende scout */   


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
   if (rtparty != FEDERAL) 
      {
      pnts=50;
      pntsd=800;
      }
   else
      {
      pnts=-50;
      pntsd=-800;      
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==FEDERAL)
          {
          sprintf(print_str,"R.ship at %d %d under fire!",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->robotship[FEDERAL][nrob].shield==ON)
      {
      m_p->robotship[FEDERAL][nrob].sh_energy=m_p->robotship[FEDERAL][nrob].sh_energy-energy;
      if (m_p->robotship[FEDERAL][nrob].sh_energy<0) m_p->robotship[FEDERAL][nrob].sh_energy=0;
      m_p->robotship[FEDERAL][nrob].dam=m_p->robotship[FEDERAL][nrob].dam+(energy/10); 
      if (m_p->robotship[FEDERAL][nrob].sh_energy<=0) 
         {
         m_p->robotship[FEDERAL][nrob].shield=OFF;
         }
      }
   else
      {
      m_p->robotship[FEDERAL][nrob].dam=m_p->robotship[FEDERAL][nrob].dam+energy;
      }
   if (m_p->robotship[FEDERAL][nrob].dam>=MAX_DAM_ROBOTSHIP)
      {
      m_p->robotship[FEDERAL][nrob].status=DESTROYED;
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
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
   if (rtparty != IMPERIAL) 
      {
      pnts=50;
      pntsd=800;
      }
   else
      {
      pnts=-50;
      pntsd=-800;      
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
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
      m_p->robotship[IMPERIAL][nrob].sh_energy=m_p->robotship[IMPERIAL][nrob].sh_energy-energy;
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<0) m_p->robotship[IMPERIAL][nrob].sh_energy=0;
      m_p->robotship[IMPERIAL][nrob].dam=m_p->robotship[IMPERIAL][nrob].dam+(energy/10); 
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<=0) 
         {
         m_p->robotship[IMPERIAL][nrob].shield=OFF;
         }
      }
   else
      {
      m_p->robotship[IMPERIAL][nrob].dam=m_p->robotship[IMPERIAL][nrob].dam+energy;
      }
   if (m_p->robotship[IMPERIAL][nrob].dam>=MAX_DAM_ROBOTSHIP)
      {
      m_p->robotship[IMPERIAL][nrob].status=DESTROYED;
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
      return; 
      }
   return;
   }       
/* Ende Robotschiff */   

/*********/
/* Basis */
/*********/
if (obj==F_BASE || obj==E_BASE)
   {
   nr=find_nr_base(x,y);   
   m_p->base[nr].condition=RED;
   m_p->base[nr].red_count=MAX_RED_COUNT;
   if (rtparty!=m_p->base[nr].party) 
      {
      pnts=40;
      pntsd=500;
      }
   else
      {
      pnts=-40;
      pntsd=-500;      
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->base[nr].party)
          {
          sprintf(print_str,">Base at %03d %03d under fire!<",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->base[nr].sh==ON)
      {
      m_p->base[nr].sh_energy=m_p->base[nr].sh_energy-energy;
      if (m_p->base[nr].sh_energy<0) m_p->base[nr].sh_energy=0;      
      m_p->base[nr].dam=m_p->base[nr].dam+(energy/10);
      if (m_p->base[nr].sh_energy<=0) 
         {
         m_p->base[nr].sh=OFF;
         }
      }
   else
      {
      m_p->base[nr].dam=m_p->base[nr].dam+energy;
      }
   if (m_p->base[nr].dam>=MAX_DAM_BAS)
      {
      init_expl(x,y,EXPL_TIME);
      eliminate_base(nr);
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
      return; 
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
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   if (m_p->ship[0].shield==ON)
      {
      m_p->ship[0].sh_energy=m_p->ship[0].sh_energy-energy;
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+(energy/10);
      if (m_p->ship[0].sh_energy<=0) 
         {
         m_p->ship[0].shield=OFF;
         }
      }
   else
      {
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+energy;
      }
   if (m_p->ship[0].dam[0]>=max_dam[0])
      {
      m_p->l_exit[0]=EXIT;
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
      return; 
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
   if (rtparty == party[zflag])
      {
      pnts=-pnts;
      pntsd=-pntsd;
      }
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
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
   sprintf(print_str,"%s attacked by robotship",ships[zflag]);
   send_msg(zflag,print_str);
   sprintf(print_str,"** We got a %d units hit! **!",energy);
   send_msg(zflag,print_str);
   if (m_p->ship[zflag].dam[0]>=max_dam[zflag])
      {
      init_expl(x,y,EXPL_TIME);
      m_p->score[rtparty]=m_p->score[rtparty]+pntsd;
      return; 
      }
   return;
   }       /* Ende Schiffe */   


/************/
/* Satellit */
/************/
if (obj == F_SAT || obj == E_SAT)
   {
   nr=find_nr_sat(x,y);
   if (m_p->satellite[nr].party==rtparty)
             {
             pnts=-5;
             }
          else
             {
             pnts=5;
             }
   init_expl(x,y,EXPL_TIME);
   eliminate_sat(nr);
   m_p->score[rtparty]=m_p->score[rtparty]+pnts;
   return;
   }

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

void send_msg(zflag,new_msg)
/*****************************************************************************/
/***      send_msg: bereitet die msg-Strings des Users zflag so auf, dass  ***/
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
i=1;
while (i<steps && cond==0)
    {
    xneu=xalt+ (NINT( (((float) i)*rxdiff) ));
    yneu=yalt+ (NINT( (((float) i)*rydiff) ));
    aneu=m_p->a[xneu][yneu];
    if (aneu != SPACE && aneu != EXPLOSION && aneu != BLIND) cond=1;
    i++;       
    } 
     
return(cond);
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


