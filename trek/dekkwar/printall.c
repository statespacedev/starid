/************  DECWAR ******/

#include <stdio.h>      /* fuer i/o */
#include <sys/types.h>   /* diverse Datentypdefinitionen */
#include <sys/ipc.h>    /* fuer IPC bei IRIX */
#include <sys/shm.h>    /* fuer Shared Memory */
#include "dw_def.h"
#include "dw_shm.h"


extern key_t qgetkey();
extern exit();

main()

/************************************************************************/
/*** Auslesen des Galaxie                                               */
/************************************************************************/
{
int nrf,nrb,nrp,i,ix,iy,nob,nos,nof,nop,nrs;
char aa;

/************************************************************************/
/*** Shared Memory einbinden                                          ***/
/************************************************************************/
shmid=shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
printf("shmid=%d\n",shmid);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Kann Shared Mem. fuer das Kreieren der Galaxie nicht einbinden!---> Abbruch!\n");
   exit(1); 
   }

printf("printall starting...\n");  

nob=nop=nof=nos=nrp=nrs=0;

for (ix=0;ix<ARRAY_SIZE;ix++)
    {
    for (iy=0;iy<ARRAY_SIZE;iy++)
        {
        printf("%c",m_p->a[ix][iy]);
        if (m_p->a[ix][iy]==F_BASE || m_p->a[ix][iy]==E_BASE)
           {
           nob++;
           }
        if (m_p->a[ix][iy]==F_FORT || m_p->a[ix][iy]==E_FORT)
           {
           nof++;
	   }
        if (m_p->a[ix][iy]==F_SAT || m_p->a[ix][iy]==E_SAT)
           {
           nos++;
           }
        if (m_p->a[ix][iy]==PLANET)
           {
           nop++;
           }  
        }
        printf("\n");
    }
 
nrb=0;
for (i=0;i<MAX_BASES;i++)
    { 
    printf("Basis Nr. %i Status %i Partei=%i Dam=%i Sh=%i En=%i x=%i y=%i\n",i,
       	     m_p->base[i].status,
             m_p->base[i].party,m_p->base[i].dam,m_p->base[i].sh,
             m_p->base[i].sh_energy,m_p->base[i].pos[0],m_p->base[i].pos[1]);
    if (m_p->base[i].status==IN_GAME) nrb++;
    }

nrf=0;
for (i=0; i<MAX_FORTS; i++)
    {
    aa = '-';
    if (m_p->fort[i].pos[0] >= 0 && m_p->fort[i].pos[0] < ARRAY_SIZE && m_p->fort[i].pos[1] >= 0 && m_p->fort[i].pos[1] < ARRAY_SIZE) aa = m_p->a[m_p->fort[i].pos[0]][m_p->fort[i].pos[1]];

    printf("Fort Nr. %i: Status=%i Party=%i Dam=%i X=%i Y=%i Sym=%c\n",m_p->fort[i].nr,m_p->fort[i].status,m_p->fort[i].party,m_p->fort[i].dam,m_p->fort[i].pos[0],m_p->fort[i].pos[1],aa); 
    if (m_p->fort[i].status == IN_GAME) nrf++;
    }
for (i=0;i<MAX_SATS;i++)
    { 
    printf("Sat Nr. %i Partei=%i x=%i y=%i\n",i,
             m_p->satellite[i].party,
             m_p->satellite[i].pos[0],m_p->satellite[i].pos[1]);
    if (m_p->satellite[i].party != NEUTRAL) nrs++;
    }
for (i=0;i<MAX_PLANETS;i++)
    { 
    printf("Planet Nr. %i : Status %i Partei=%i Dam=%i Level=%i x=%i y=%i\n",i,
	     m_p->planet[i].status,
             m_p->planet[i].party,m_p->planet[i].dam,
             m_p->planet[i].level,
             m_p->planet[i].pos[0],m_p->planet[i].pos[1]);
    if (m_p->planet[i].status == IN_GAME) nrp++;
    }

printf("gezaehlte Anzahl: Basen=%i Forts=%i Satelliten=%i Planeten=%i\n",nob,nof,nos,nop);
printf("Sollzahl (+1 aF): Basen=%i Forts=%i Satelliten=%i Planeten=%i\n",nrb,nrf,nrs,nrp);


printf("printall ENDE\n");

(void) shmdt(m_p);

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

