/**********************************************************/
/*** scout.c                                            ***/
/*** Kommentar nach V.1.00                              ***/
/*** 14.02.03: String in sort_msg auf 33 begrenzt       ***/
/*** 18.02.03: TMusleep weg wg. Linux, siehe dwmain.c   ***/
/*** 25.02.03: neue Variable xmsg in sort_msg, da der   ***/
/***           String new_msg u.U. geaendert wird       ***/
/*** Kommentar nach V.1.0.1                             ***/
/*** 25.04.03: m_p->l_torp[vflag] entfernt              ***/
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
/*** 23.07.07: target area durch destination ersetzt    ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 27.02.08: struct scout eingefuehrt                 ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 13.03.08: Umstellung auf struct homeworld          ***/
/*** 26.03.08: Umstellung auf struct ship               ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/**********************************************************/
/*** 05.05.08: SCOUT_DELAY neu; usleep verschoben       ***/
/*** 14.10.08: Anpassungen fuer Ubuntu 8.04 (includes)  ***/
/*** 23.10.08: Anpassungen -Wall bei gcc (Ubuntu 8.04)  ***/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 02.01.10: seed() neu; ran() mit seed ueberarbeitet ***/
/*** 24.01.10: SCOUT_SCAN_INTERVAL neu                  ***/
/*** 24.01.10: Meldung bei Zerstoerung mit Position     ***/
/***           und Ausgabe ueber send_msg; Explosion    ***/
/*** 24.04.10: neue Konstantendefinitionen aus dw_def.h  ***/
/*** 25.04.10: IMAX,IMIN,IABS ueberall in () gesetzt    ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(argc,argv)
/***************************************************************************/
/*** torpedo  : Feuern eines Torpedos                                    ***/
/***         Aufruf in dwmain: SCout                                     ***/
/***************************************************************************/
    int argc;
    char *argv[];
    
{

/***************************************************************************/
/***                    lokale Variablen                                 ***/
/***************************************************************************/
int hflag,i,j,p,xziel,yziel,xstart,ystart;
int xneu,yneu,fac;
int xvec,yvec,xsteps,ysteps;
float random;
char aneu, str1[MSG_LENGTH];

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

/****************************************************************************/
/*** sig_hnd setzen fuer SIGKILL und SIGALRM                              ***/
/****************************************************************************/
signal(SIGALRM,sig_hnd);
signal(SIGINT,sig_hnd_ctrlc);
signal(SIGKILL,SIG_DFL);

/***********************/
/* Programm fuer SCOUT */
/***********************/

p=party[vflag];
m_p->scout[p].status=ACTIVE;
m_p->scout[p].energy=SCOUT_ENERGY;
m_p->scout[p].target[0]=xziel;
m_p->scout[p].target[1]=yziel;
m_p->scout[p].dam=0;
hflag=0;
xstart=ystart=-99;
xvec=yvec=0;

fac=i=1;
do {
   if (xziel >= m_p->homeworld[p].pos[0] && 
       yziel >= m_p->homeworld[p].pos[1] && 
       m_p->a[m_p->homeworld[p].pos[0]+i][m_p->homeworld[p].pos[1]+i] == SPACE)
      {
      xstart=m_p->homeworld[p].pos[0]+i;
      ystart=m_p->homeworld[p].pos[1]+i;
      }
   else if (xziel <= m_p->homeworld[p].pos[0] &&
            yziel <= m_p->homeworld[p].pos[1] &&
       m_p->a[m_p->homeworld[p].pos[0]-i][m_p->homeworld[p].pos[1]-i] == SPACE)
      {
      xstart=m_p->homeworld[p].pos[0]-i;
      ystart=m_p->homeworld[p].pos[1]-i;
      }       
   else if (xziel >= m_p->homeworld[p].pos[0] &&
            yziel <= m_p->homeworld[p].pos[1] &&
       m_p->a[m_p->homeworld[p].pos[0]+i][m_p->homeworld[p].pos[1]-i] == SPACE)
      {
      xstart=m_p->homeworld[p].pos[0]+i;
      ystart=m_p->homeworld[p].pos[1]-i;
      }
   else if (xziel <= m_p->homeworld[p].pos[0] &&
            yziel >= m_p->homeworld[p].pos[1] &&
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
            m_p->scout[p].status == ACTIVE);

m_p->scout[p].pos[0]=xstart;
m_p->scout[p].pos[1]=ystart;

if (p == FEDERAL)
   {
   m_p->a[xstart][ystart]=F_SCOUT;
   }
else
   {
   m_p->a[xstart][ystart]=E_SCOUT;
   }

usleep(SCOUT_DELAY);

do {
   xsteps=xziel-m_p->scout[p].pos[0];
   ysteps=yziel-m_p->scout[p].pos[1];
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
   xneu=m_p->scout[p].pos[0]+xvec;
   yneu=m_p->scout[p].pos[1]+yvec;
   aneu=m_p->a[xneu][yneu];
   if (aneu == SPACE)
     {
     fac=1;
     m_p->a[m_p->scout[p].pos[0]][m_p->scout[p].pos[1]]=SPACE;
     if (p == FEDERAL)
       {
       m_p->a[xneu][yneu]=F_SCOUT;
       }
     else 
       {
       m_p->a[xneu][yneu]=E_SCOUT;
       }
     m_p->scout[p].pos[0]=xneu;
     m_p->scout[p].pos[1]=yneu;
     m_p->scout[p].energy=m_p->scout[p].energy-5;
     if (m_p->scout[p].energy <= 0) 
        {
        sort_msg("Scout ran out of energy!");
        m_p->scout[p].status=DESTROYED;
        }
     }
   else if (abs(xsteps) < 2 && abs(ysteps) < 2 )
     {
     xziel = m_p->scout[p].pos[0];
     yziel = m_p->scout[p].pos[1];
     m_p->scout[p].target[0]=xziel;
     m_p->scout[p].target[1]=yziel;
     sort_msg("SCOUT: Coordinates adjusted!");
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
   usleep(SCOUT_DELAY);
   } while ( (m_p->scout[p].pos[0] != xziel || m_p->scout[p].pos[1] != yziel)
             && m_p->scout[p].status == ACTIVE);

if (m_p->scout[p].status == ACTIVE)
  {
  j=0;
  sort_msg("SCOUT: destination reached!");
  while (m_p->scout[p].status == ACTIVE)
      {
      usleep(SCOUT_SCAN_INTERVAL);
      j++;
      if (j >= 50)
	{
        m_p->scout[p].energy=m_p->scout[p].energy-1;
        j=0;
        if (m_p->scout[p].energy <= 0) 
          {
          sort_msg("Scout ran out of energy!");
          m_p->scout[p].status=DESTROYED;
	  }
        }
      }
  }

sprintf(str1,">Scout at %03d %03d destroyed<",m_p->scout[p].pos[0],m_p->scout[p].pos[1]);
for (i=1;i<MAX_USER;i++)
  {
    if (m_p->luser[i]==ACTIVE && party[i]==p)
     {
     send_msg(i,str1);
     }
  }

m_p->scout[p].status=DESTROYED;
m_p->a[m_p->scout[p].pos[0]][m_p->scout[p].pos[1]]=EXPLOSION;
usleep(EXPL_TIME);
m_p->a[m_p->scout[p].pos[0]][m_p->scout[p].pos[1]]=SPACE;
m_p->scout[p].pos[0]=-99;
m_p->scout[p].pos[1]=-99;
m_p->scout[p].target[0]=-99;
m_p->scout[p].target[1]=-99;
m_p->scout[p].dam=0;

(void) shmdt(m_p);
exit(0);
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
/******************  ENDE qgetkey() ******************************************/

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
