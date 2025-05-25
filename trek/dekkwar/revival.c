/**********************************************************/
/*** init_gal.c                                         ***/
/**********************************************************/
/*** Kommentar nach V.1.0.1                             ***/
/*** 25.04.03: m_p->l_torp[vflag] entfernt              ***/
/**********************************************************/
/*** 29.04.03: Version 1.1.0                            ***/
/**********************************************************/
/*** 07.05.04: Umstellung struct fort                   ***/
/*** 30.06.04: MSG_LENGTH neu in dw_def.h               ***/
/*** 30.06.04: libdw weg                                ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 17.07.07: m_p->l_hw durch m_p->display_status ers. ***/
/***           HW_xxx durch DISPLAY_xxx ersetzt         ***/
/*** 24.07.07: recon_str neu; vorbesetzt                ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 26.08.07: m_p->map vorbesetzt                      ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 23.01.08: Umstellung struct satellite              ***/
/*** 24.01.08: Umstellung struct planet                 ***/
/*** 25.01.08: Umstellung struct base                   ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 16.02.08: Flag RLEE_ON/OFF                         ***/
/*** 22.02.08: Sprungtor-Funktion                       ***/
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
/*** 15.03.08: HOOD_ON/OFF, RLEE_ON/OFF, BEAM_ON/OFF,   ***/
/***           RAMM_ON/OFF durch ON/OFF ersetzt,        ***/
/***           DEFEAT_ON/OFF weg. N_/EXISTING durch     ***/
/***           NOT_/IN_GAME ersetzt                     ***/
/*** 23.03.08: STOPPED durch OFF und MOVING durch ON    ***/
/***           ersetzt                                  ***/
/*** 24.03.08: SH_DOWN/UP durch ON/OFF ersetzt          ***/
/***           N/JUMPING durch OFF/ON ersetzt           ***/
/*** 26.03.08: l_defeat weg                             ***/
/*** 26.03.08: Umstellung auf struct ship               ***/
/*** 29.03.08: ship.dock=HW_DOCKED als Voreinstellung   ***/
/*** 30.03.08: Umgebung von HW: keine *,=,BL.HOLE       ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 03.04.08: SV-Flag eingebaut                        ***/
/*** 06.04.08: Missile path eingefuehrt                 ***/
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
/*** 07.12.09: Vorbesetzungen ship vervollstaendigt     ***/
/*** 13.12.09: seed() neu; ran() mit seed ueberarbeitet ***/
/*** 20.12.09: in ran2 i,j > 0 wg. floating p. except.  ***/
/*** 23.01.10: base[].sh=ON bei Basen im Spiel; war auf ***/
/***           OFF voreingestellt gesetzt               ***/
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include "dw_def.h"
#include "mathfkt.h"
#include "dw_shm.h"
#include "ships.h"


extern key_t qgetkey();
extern float ran();
extern int ran2();
extern int seed();

int main()

/************************************************************************/
/*** galaxy()   - Kreieren eines neuen Universums                     ***/
/************************************************************************/
{
int i,j,z1,z2,z3,z4,z5,z6,z7,z8,z9,z0,zz,ir,zc,nrp,nrb;
int c1,c2,c1c,c2c,clu,cluc,clux,cluy,cluxc,cluyc,xhe,yhe,xhf,yhf;
float rnd,r;

/************************************************************************/
/*** Shared Memory einbinden                                          ***/
/************************************************************************/
shmid=shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
printf("shmid=%d\n",shmid);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Kann Shared Mem. fuer das Kreieren des Universums nicht einbinden!---> Abbruch!\n");
   exit(1); 
   }
for (i=0;i<MAX_USER;i++)
    {
    strcpy(m_p->name_of_cmdr[i],"                    "); 
    strcpy(m_p->rank_of_name[i],"         ");
    strcpy(m_p->cmd[i],"                   ");   
    strcpy(m_p->mr_cmd[i],"                   ");   
    strcpy(m_p->ph_cmd[i],"                   ");   
    strcpy(m_p->to_cmd[i],"                   ");
    sprintf(m_p->recon_str[i],"%-12s: no data available            ",ships[i]);
    printf("%2d %s\n",i,m_p->recon_str[i]);
    m_p->l_exist[i]=IN_GAME;
    m_p->l_exit[i]=NO_EXIT;
    m_p->luser[i]=NOT_IN_GAME;
    m_p->display_status[i]=DISPLAY_RADAR;      
    m_p->user_score[i]=0;
    m_p->name_score[i]=0;  
    m_p->rank[i]=COMMANDER;
    m_p->ship[i].status=NOT_IN_GAME;    
    m_p->ship[i].pos[0]=-99;
    m_p->ship[i].pos[1]=-99;
    m_p->ship[i].main_energy=0; 
    m_p->ship[i].sh_energy=0;     
    m_p->ship[i].ph_energy=0;  
    m_p->ship[i].torp=0;       
    m_p->ship[i].sat=0;       
    m_p->ship[i].mines=0;        
    m_p->ship[i].fuel=0; 
    for (j=0;j<MAX_TT;j++)
      {
      m_p->ship[i].status_ttubes[j]=LOADED;    
      m_p->ship[i].tt_lock_time[j]=0;    
      }
    for (j=0;j<11;j++)
        {
        m_p->ship[i].dam[j]=0;
        }
    m_p->ship[i].ramm=OFF;      
    m_p->ship[i].build=OFF;                  
    m_p->ship[i].hood=OFF;
    m_p->ship[i].phaser=OFF;    
    m_p->ship[i].jump=OFF;
    m_p->ship[i].dock=HW_DOCKED;          
    m_p->ship[i].move=OFF;    
    m_p->ship[i].shield=OFF;      
    m_p->ship[i].sv=OFF;
    m_p->ship[i].mp=OFF;
    m_p->ship[i].imp=0;
    for (j=0;j<7;j++)
        {
	strncpy(m_p->ship[i].msg[j],"                               ",MSG_LENGTH);  
        }
    for (j=0;j<(MISSILE_FUEL+1);j++)
        {
        m_p->ship[i].mpath[j][0]=-99;
        m_p->ship[i].mpath[j][1]=-99;
        }
    }

(void) shmdt(m_p);
exit(0);
}
/*** Ende galaxy() ***/

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


float ran()
/*****************************************************************************/
/*** ran(idum) liefert eine Integer Zufallszahl 0.0 bis 1.0                ***/
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

int ran2(i,j)
    int i,j;
/***********************************************************************/
/*** ran2 liefert integer-Zufallszahl aus i und j mittels %           **/
/***********************************************************************/

{
int rnd,i2,rnd1,err_cnt;

if (i<=0) i=1;
if (j<=0) j=1;

i2=i;
if (i<j) i2=10*i;

rnd=i2%j; 

while (rnd>10) 
      {
      rnd1=rnd/10;
      rnd1=rnd1*10;
      rnd=rnd-rnd1;
      err_cnt++;
      if (err_cnt>10000)
         {
         rnd = 0;
         err_cnt=0;
         }
      }
      printf("rnd2=%d\n",rnd);
return(rnd);
}

int seed(void)
{
  int i;
  struct timespec ntime;
  int iretval;
  iretval = clock_gettime(CLOCK_REALTIME, &ntime);
  i = (int) ntime.tv_nsec%10000;
  return i;
}

