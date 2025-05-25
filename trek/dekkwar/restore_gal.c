/**********************************************************/
/*** restore_gal.c                                      ***/
/**********************************************************/
#include <stdio.h>      /* fuer i/o */
#include <sys/types.h>   /* diverse Datentypdefinitionen */
#include <sys/times.h>   /* Zeitfunktionen */
#include <sys/ipc.h>    /* fuer IPC bei IRIX */
#include <sys/shm.h>    /* fuer Shared Memory */
#include "dw_def.h"
#include "mathfkt.h"
#include "dw_shm.h"
#include "ships.h"

extern key_t qgetkey();

main()
{
int i,j;
FILE *fp,*fopen();

/************************************************************************/
/*** Shared Memory einbinden                                          ***/
/************************************************************************/
shmid=shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
printf("shmid=%d\n",shmid);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Kann Shared Memory des Universums nicht einbinden!---> Abbruch!\n");
   exit(1); 
   }

printf("Restauriere Universum!\n");
fp=fopen("galaxy.dat","r");

for (i=0;i<ARRAY_SIZE;i++)
  {
  for (j=0;j<ARRAY_SIZE;j++)
    {
    fscanf(fp,"%s",m_p->a[i][j]);
    }
  }
fclose(fp);

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
