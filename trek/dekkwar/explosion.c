/**********************************************************/
/*** explosion.c                                        ***/
/**********************************************************/
/*** 29.05.04: explosion neu als Fork NEU!              ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 26.03.08: Umstellung auf struct ship               ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
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
/*** 26.12.09: incident() auf ran() mit seed() umgest.  ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include "dw_def.h"
#include "dw_shm.h"
#include "mathfkt.h"     /* math. Funktionen (incl. <math.h> */

extern key_t qgetkey();
extern void sort_msg();
extern int incident();
extern int seed();
extern float ran();

static int vflag;

int main(argc,argv)    
    int argc;
    char *argv[];
{
int x,y;
int mysecs;
char dummy;

if (argc != 3) exit(0);

sscanf(argv[0],"%d",&x); 
sscanf(argv[1],"%d",&y); 
sscanf(argv[2],"%d",&mysecs); 

if (x < 0 || x >= ARRAY_SIZE || y < 0 || y >= ARRAY_SIZE || mysecs < 10000 || mysecs > 999999) exit(0);

shmid = shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   exit(1); 
   }

if (shmid != m_p->shm_id)
   {
   exit(1);
   }

dummy=m_p->a[x][y];
m_p->a[x][y]=EXPLOSION;       	
usleep(mysecs);
/* Sonderfall: Stern wird mit 25 % Wahrscheinlichkeit ein Schwarzes Loch */
if (dummy == STAR)
   {
   if (incident(25) == 0)
     {
     m_p->a[x][y]=BLACK_HOLE;
     }
   else
     {
     m_p->a[x][y]=SPACE;
     }
   }
/* Sonderfaelle: Gaswolke bleibt Gaswolke, Schwarzes Loch ein Schwarzes Loch */
else if (dummy == CLOUD)
   {
   m_p->a[x][y]=CLOUD;
   }
else if (dummy == BLACK_HOLE)
  {
  m_p->a[x][y]=BLACK_HOLE;
  }
else
  {
  m_p->a[x][y]=SPACE;
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
/******************  ENDE qgetkey() ******************************************/

void sort_msg(new_msg)
/*****************************************************************************/
/***      sort_msg: bereitet die msg-Strings des Users vflag so auf, dass  ***/
/**                 new_msg hinten (im Cockpit-Window unten) steht.        ***/
/*****************************************************************************/
/* 27.04.04: Umstellung auf libdw und strncpy                              ***/
/*****************************************************************************/
char new_msg[MSG_LENGTH];
{
int i;

for (i=0;i<6;i++)
    { 
    strncpy(m_p->ship[vflag].msg[i],m_p->ship[vflag].msg[(i+1)],MSG_LENGTH-1);
    }  
strncpy(m_p->ship[vflag].msg[6],new_msg,MSG_LENGTH-1);  
return;
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

