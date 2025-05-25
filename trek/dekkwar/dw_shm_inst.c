/************  shm_inst ******/
/**********************************************************/
/*** 29.04.03: Version 1.1.0                            ***/
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
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 03.01.09: Segfault bei Eingabe n beseitigt: else   ***/
/***           fragt auf != n ab, dann exit(0); while   ***/
/***           getchar entfernt                         ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
#include <stdlib.h> 
#include <stdio.h>
#include <sys/shm.h>
#include "dw_def.h"
#include "dw_shm.h"

extern key_t qgetkey();

int main()

{
printf("Shared Memory fuer DECWAR kreieren? (j/n) :");
if (getchar() == 'j')
   {
   if ( (shmid = shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT|MODE))
          ==
        ERROR)
      {
      printf(" !!!! Fehler !!! Kann Shared Memory %s nicht kreieren!\n",SHMEM);
      printf("                 Ist wahrscheinlich schon/noch aktiv!\n");
      printf("                 Die Shared-Memory-ID ist: %d\n",shmid);
      exit(1);
      }
   else
      {
      printf("Shared Memory %s = %d kreiert!\n",SHMEM,shmid);
      }
   }
else
   {
   printf("Ende\n");
   exit(0);
   }

if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Error: SHM nicht einzubinden, shmid=%d m_p->shm_id=%d",shmid,m_p->shm_id);
   exit(1);
   }

exit(0);
} /*** Ende main ***/

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
