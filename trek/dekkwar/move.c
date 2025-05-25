/**********************************************************/
/*** move.c                                             ***/
/*** Kommentar nach V.1.00                              ***/
/*** 10.02.03: Handling fuer Ctrl-C geaendert, da immer ***/
/***           noch das letzte TO-Kommando ausgefuehrt  ***/
/***           wurde wegen Shared Memory                ***/
/*** 14.02.03: String in sort_msg auf 33 begrenzt       ***/
/***           send_msg: dito                           ***/
/*** 18.02.03: TMusleep weg wg. Linux, siehe dwmain.c   ***/
/*** 25.02.03: neue Variable xmsg in sort_msg, da der   ***/
/***           String new_msg u.U. geaendert wird       ***/
/***           dito send_msg                            ***/
/*** Kommentar nach V.1.0.1                             ***/
/*** 20.03.03: jetzt sind beliebig viele Schritte       ***/
/***           erlaubt (max_speed weg).                 ***/
/*** 24.03.03: Schritt-Geschwindigkeiten neu            ***/
/*** 24.03.03: aalt (alter Wert vom m_p->a[][]) ent-    ***/
/***           fernt, da bei Warp immer SPACE, auch     ***/
/***           nach Kollision mit Mine/Blindgaenger     ***/
/***           Schleife etwas vereinfacht               ***/
/*** 04.04.03: Unterprogramm eliminate_hw jetzt in libdw***/
/*** 10.04.03: Stoppen der Bewegung aus dwmain mit STOP ***/
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
/*** 30.06.04: void hit() einheitlich bei move, dwmain, ***/
/***           phaser, torpedo                          ***/
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
/*** 04.08.07: Zerstoerung Schiff in recon_str          ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 23.01.08: Umstellung struct satellite              ***/
/*** 24.01.08: Umstellung struct planet                 ***/
/*** 25.01.08: Umstellung struct base                   ***/
/*** 08.02.08: Wenn Basis zerstoert wird, werden auch   ***/
/***           alle gedockten Schiffe zerstoert         ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 22.02.08: Sprungtor-Funktion                       ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 26.02.08: Symbol fuer DOCKING entfernt             ***/
/*** 27.02.08: struct scout eingefuehrt                 ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
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
/*** 23.03.08: STOPPED durch OFF und MOVING durch ON    ***/
/***           ersetzt                                  ***/
/*** 26.03.08  Umstellung auf struct ship               ***/
/*** 26.03.08  condition auch fuer Heimatwelten         ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 03.04.08: Fehler: return bei missile entfernt      ***/
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
/*** 26.11.09: Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 29.11.09: Treffer Scout in hit umgeschrieben       ***/
/*** 31.12.09: seed() neu; ran() mit seed ueberarbeitet ***/
/*** 09.01.10: umgestellt auf init_expl()               ***/
/*** 09.01.10: BLIND_ und MINE_COL_ENERGY neu           ***/
/*** 10.01.10: init_expl weg bei Kollision mit MINE u.  ***/
/***           BLIND wegen Flug durch Explosion         ***/
/*** 13.01.10: init_expl weg bei Kollision mit MISSILE  ***/
/*** 13.01.10: MISSILE_COL_ENERGY neu                   ***/
/*** 23.01.10: Keine Explosion des Romulaners mehr; das ***/
/***           macht R. selbst. Scoring bei R.-Vern.    ***/
/***           wie bei torpedo.c                        ***/
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
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/shm.h>
#include <unistd.h>

/*****************************************************************************/
/*** Includes DECWAR                                                       ***/
/*****************************************************************************/
#include "mathfkt.h"     /* math. Funktionen (incl. <math.h> */
#include "dw_def.h"
#include "dw_shm.h"
#include "ships.h"

static int vflag; 

extern void sig_hnd();
extern void sig_hnd_ctrlc();
extern key_t qgetkey();
extern void path();
extern void sort_msg();
extern float ran();
extern int seed();
extern void gauss2();
extern int det_target();
extern void hit();
extern void send_msg();
extern int incident();
extern void eliminate_sat();
extern void eliminate_base();
extern void eliminate_fort();
extern void eliminate_jumpgate();
extern void eliminate_pl();
extern void eliminate_hw();
extern void init_expl();
extern int find_nr_pl();
extern int find_nr_sat();
extern int find_nr_robotship();
extern int find_nr_fort();
extern int find_nr_base();
extern int find_nr_jumpgate();

int main(argc,argv)
/***************************************************************************/
/*** move  : Bewegung im Spielfeld, max. je 6 Einheiten                  ***/
/***         Aufruf in dwmain:  MR XSTEPS YSTEPS                         ***/
/***************************************************************************/
    int argc;
    char *argv[];
    
{

/***************************************************************************/
/***                    lokale Variablen                                 ***/
/***************************************************************************/
int xsteps,ysteps, steps,i,zflag;
int xneu,yneu,xalt,yalt,xstart,ystart,lex,hit_energy;
char cm[2],aneu,print_str[MSG_LENGTH];
float rxdiff,rydiff;

/****************************************************************************/
/***                    argv[0] konvertieren                              ***/
/****************************************************************************/
sscanf(argv[0],"%d",&vflag);
/****************************************************************************/
/*** sig_hnd setzen fuer SIGKILL und SIGALRM                              ***/
/****************************************************************************/
signal(SIGALRM,sig_hnd);
signal(SIGINT,sig_hnd_ctrlc);
signal(SIGKILL,SIG_DFL);

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
/*** Startpunkt mit Wartezustand                                       ***/
/*************************************************************************/
clock();
start:
m_p->ship[vflag].move=OFF; 
pause();

/*********************************/
/* Hier das eigentliche Programm */
/*********************************/

/****************************************************************************/
/***                    Commandostring interpretieren (argv[1])           ***/
/****************************************************************************/
if (sscanf(m_p->mr_cmd[vflag],"%s%d%d",cm,&xsteps,&ysteps) != 3)
   {
   sort_msg("     *** BAD COMMAND ***     ");
   goto start;
   }
  
if (xsteps == 0  && ysteps == 0) 
   {
   goto start;
   }
if ((m_p->ship[vflag].pos[0]+xsteps) < 0 || (m_p->ship[vflag].pos[0]+xsteps) >= ARRAY_SIZE ||
    (m_p->ship[vflag].pos[1]+ysteps) < 0 || (m_p->ship[vflag].pos[1]+ysteps) >= ARRAY_SIZE )
   {
   sort_msg("MR: *** Wrong coordinates ***");
   goto start;
   }
if (m_p->ship[vflag].dam[1] > 240) 
   {
   sort_msg("MR: Warp Engines out of order");
   goto start;
   }

/* Kommando o.k.: jetzt Weg berechnen und dann Flug */
path(xsteps,ysteps,&rxdiff,&rydiff,&steps);
if (m_p->ship[vflag].dam[1] > 80 && m_p->ship[vflag].dam[1] < 121 && (IABS(ysteps)) > 4 ) 
   {
   sort_msg("MR: Warp Engines dam., max.4");
   goto start;
   }
if (m_p->ship[vflag].dam[1] > 120 && m_p->ship[vflag].dam[1] < 161 && (IABS(ysteps)) > 3 ) 
   {
   sort_msg("MR: Warp Engines dam., max.3");
   goto start;
   }
if (m_p->ship[vflag].dam[1] > 160 && m_p->ship[vflag].dam[1] < 201 && (IABS(ysteps)) > 2 ) 
   {
   sort_msg("MR: Warp Engines dam., max.2");
   goto start;
   }
if (m_p->ship[vflag].dam[1] > 200 && (IABS(ysteps)) > 1 ) 
   {
   sort_msg("MR: Warp Engines dam., max.1");
   goto start;
   }

/* "Flug" */
xalt=xstart=m_p->ship[vflag].pos[0];
yalt=ystart=m_p->ship[vflag].pos[1];
for (i=1;i<=steps;++i)
    {
    if (m_p->ship[vflag].move==OFF) goto start;
    usleep((150000+level_of_ship[vflag]*100000));
    /* Neue Position */
    xneu=xstart+ (NINT( (((float) i)*rxdiff) ));
    yneu=ystart+ (NINT( (((float) i)*rydiff) ));
    aneu=m_p->a[xneu][yneu];
    if (aneu == ships[vflag][0]) aneu=SPACE;
    /* Abfragen neue Position */
    /* Kollision abwenden bei festliegenden Objekten (und Scout)  und intaktem Computer */
    if (aneu == STAR || aneu==PLANET || aneu==E_BASE || aneu==F_BASE ||
        aneu == F_SAT || aneu==E_SAT || aneu==F_FORT || aneu==E_FORT || 
        aneu==CLOUD || aneu == E_HOMEWORLD || aneu == F_HOMEWORLD ||
        aneu==F_SCOUT || aneu==E_SCOUT || aneu==JUMPGATE ||
        aneu == E_ROBOTSHIP || aneu == F_ROBOTSHIP
         || xneu<0 || xneu>(ARRAY_SIZE-1) || yneu<0 || yneu>(ARRAY_SIZE-1))
       {
       m_p->ship[vflag].pos[0]=xalt;
       m_p->ship[vflag].pos[1]=yalt;
       if (aneu==CLOUD) 
          {
          sort_msg("MR: We can't fly with warp en-");
          sort_msg("    gines through a gas cloud!");
          }
       if (m_p->ship[vflag].dam[6] < 350 && m_p->ship[vflag].ramm==OFF)
          {
          sort_msg("MR: *** Collision averted ***");
          m_p->ship[vflag].main_energy=(m_p->ship[vflag].main_energy-100);
          }
       else
          {
          if ( party[vflag]==IMPERIAL && aneu == F_HOMEWORLD )
       	     {
             sort_msg("MR: *** Collision!!!!! ***");
             sort_msg("MR: ***50000 units hit!***");
             hit_energy=50000;
             hit(ships[vflag][0],xneu,yneu,hit_energy);
             hit_energy=500;
             gauss2(&hit_energy);
             hit(aneu,xneu,yneu,hit_energy);
             }          
          else if ( party[vflag]==FEDERAL && aneu == E_HOMEWORLD )
       	     {
             sort_msg("MR: *** Collision!!!!! ***");
             sort_msg("MR: ***50000 units hit!***");
             hit_energy=50000;
             hit(ships[vflag][0],xneu,yneu,hit_energy);
             hit_energy=500;
             gauss2(&hit_energy);
             hit(aneu,xneu,yneu,hit_energy);
             }          
          else
	     {
             sprintf(print_str,"MR: Collision with %c !!!",aneu);
             sort_msg(print_str);
             hit_energy=100;
             gauss2(&hit_energy);
             hit(ships[vflag][0],xneu,yneu,hit_energy);
             m_p->ship[vflag].main_energy=(m_p->ship[vflag].main_energy-100);
             sort_msg("MR: Emergency stop!");
             if (aneu != CLOUD) hit(aneu,xneu,yneu,hit_energy);
             }
          }      
       goto start;
       }
    /* Fliegende Objekte */
    zflag=det_target(aneu);
    if (zflag>=0 && m_p->ship[zflag].pos[0]!=xneu)
       {
       aneu=m_p->a[xneu][yneu]=SPACE;
       zflag=-1;
       sort_msg(" Ghost image, vanished!");
       }
    if (zflag!=-1 || aneu==HOOD)
       {
       m_p->ship[vflag].pos[0]=xalt;
       m_p->ship[vflag].pos[1]=yalt;
       sort_msg("MR:Collision with flying object");
       hit_energy=100;
       gauss2(&hit_energy);
       hit(aneu,xneu,yneu,hit_energy);
       hit_energy=100;
       gauss2(&hit_energy);
       sprintf(print_str,"MR: We got a %d units hit!",hit_energy);
       sort_msg(print_str);
       hit(ships[vflag][0],xneu,yneu,hit_energy);
       m_p->ship[vflag].main_energy=(m_p->ship[vflag].main_energy-100);
       sort_msg("MR: Emergency stop!");
       goto start;
       }
    else if (aneu==MISSILE)
       {
       if (m_p->missile[FEDERAL].status==DESTROYED && m_p->missile[IMPERIAL].status==DESTROYED)
         {
         m_p->a[xneu][yneu]=SPACE;
         }
       else if (m_p->missile[FEDERAL].pos[0]!=xneu && m_p->missile[FEDERAL].pos[1]!=yneu && m_p->missile[IMPERIAL].pos[0]!=xneu && m_p->missile[IMPERIAL].pos[1]!=yneu)
         {
         m_p->a[xneu][yneu]=SPACE;
         }
       else
         {
         sort_msg("IM: Collision with missile!");
         hit_energy=MISSILE_COL_ENERGY;
         gauss2(&hit_energy);
         sprintf(print_str,"    %d units hit",hit_energy);  
         sort_msg(print_str);
         hit(MISSILE,xneu,yneu,hit_energy);
         hit(ships[vflag][0],xneu,yneu,hit_energy);        
         aneu=m_p->a[xneu][yneu]=SPACE;
         }
       } 

    /* Zusammenstoss mit anderen Objekten (nicht averted) */
    /* init_expl geht nicht wegen Flug durch Expl.: Symbol verschwindet */
    if (aneu==BLIND)
       {
       sort_msg("MR: Collision with Blind!");
       sort_msg("    Blind exploded!");
       aneu=m_p->a[xneu][yneu]=EXPLOSION;
       usleep(EXPL_TIME);
       aneu=m_p->a[xneu][yneu]=SPACE;
       hit_energy=BLIND_COL_ENERGY;
       gauss2(&hit_energy);
       sprintf(print_str,"    %d units hit",hit_energy);
       sort_msg(print_str);
       hit(ships[vflag][0],xneu,yneu,hit_energy); 
       }
    /* Mine , auch kein init_expl moeglich */      
    if (aneu==MINE)
       {
       sort_msg("MR: Collision with Mine!");
       sort_msg("    Mine exploded!");
       aneu=m_p->a[xneu][yneu]=EXPLOSION;
       usleep(EXPL_TIME);
       aneu=m_p->a[xneu][yneu]=SPACE;
       hit_energy=MINE_COL_ENERGY; 
       gauss2(&hit_energy);
       sprintf(print_str,"    %d units hit",hit_energy);
       sort_msg(print_str);
       hit(ships[vflag][0],xneu,yneu,hit_energy); 
       }
    /* Schwarzes Loch */
    if (aneu == BLACK_HOLE)
       {
       lex=incident(20);
       if (lex == 1) 
          {
          sort_msg("-GULP-Swallowed by black hole!");
          m_p->l_exit[vflag]=EXIT;
          m_p->a[xalt][yalt]=SPACE;
          usleep(200000);
          i=999;
          }
       else
          {
          sort_msg("MR: Displaced by black hole!");
          m_p->a[xalt][yalt]=SPACE;
          m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[1]=xneu=yneu=-99;
          do    {
                xneu=(int) (199.0*ran());
                yneu=(int) (199.0*ran());                
                }
          while ((xneu<0 || yneu<0 || xneu>(ARRAY_SIZE-1) || yneu>(ARRAY_SIZE-1))
                                        &&
                             (m_p->a[xneu][yneu]!=SPACE) );
          aneu=SPACE;
          i=999;
          if (m_p->ship[vflag].mp==ON)
	    {
	    m_p->ship[vflag].mp=OFF;
	    m_p->ship[vflag].imp=0;
	    for (i=0;i<(MISSILE_FUEL+1);i++)
		{
		m_p->ship[vflag].mpath[i][0]=-99;
		m_p->ship[vflag].mpath[i][1]=-99;
		}
	    sort_msg(" MP OFF - Path data deleted!");
            }
          }
       }
           
    /* Umsetzen der Werte */
    m_p->ship[vflag].pos[0]=xneu;
    m_p->ship[vflag].pos[1]=yneu;
    m_p->a[xalt][yalt]=SPACE;
    if (m_p->ship[vflag].mp==ON)
      {
      if (m_p->ship[vflag].imp > MISSILE_FUEL)
	{
	sort_msg("MP: ERROR! Maximum step number");
	sort_msg("  exceeded! Recording stopped!");
	}
     else
        {
        m_p->ship[vflag].imp++;
        m_p->ship[vflag].mpath[m_p->ship[vflag].imp][0]=xneu;
        m_p->ship[vflag].mpath[m_p->ship[vflag].imp][1]=yneu;
        }
     }


   if (m_p->ship[vflag].hood==OFF)
     {
     m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
     }
   else
     {
     m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=HOOD;
     }
   xalt=xneu;
   yalt=yneu;
   m_p->ship[vflag].main_energy=(m_p->ship[vflag].main_energy-10);    
   }      

/**********************************/
/* Ruecksprung zu start und pause */
/**********************************/
goto start;

/*(void) shmdt(m_p);
exit(0);*/
}


void sig_hnd()
/*************************************************************************/
/*** Setzen des Handlings fuer SIGALRM; bei bei jedem Aufruf!!         ***/
/*************************************************************************/
{
signal(SIGALRM,sig_hnd);
m_p->ship[vflag].move=ON;
return;
}

void sig_hnd_ctrlc()
/************************************************************************/
/*** setzen des neuen Signalhandlings nach pause                      ***/
/************************************************************************/
{
sort_msg(" *** CTRL-C desaktiviert! ***");
m_p->mr_cmd[vflag][0]='\0';
signal(SIGINT,sig_hnd_ctrlc);
return;
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

void hit(obj,x,y,energy)
     int x,y,energy;
     char obj;
/**************************************************************************/
/*** Trefferwirkung auf veschiedene Objekte obj                         ***/
/**************************************************************************/
{
int nr,pntsd,pnts,i,zflag,j,he,device,mparty,nrob,tparty;
float random;
char print_str[MSG_LENGTH],n_obj;

/**********/
/* Sterne */
/**********/
if (obj==STAR)
   {
   if (incident(50)==0)   /* Stern explodiert nicht ===> Ende */
      {
      m_p->name_score[vflag]=m_p->name_score[vflag]-5;
      m_p->user_score[vflag]=m_p->user_score[vflag]-5;
      m_p->score[party[vflag]]=m_p->score[party[vflag]]-5;      
      sort_msg("  Star unaffected!");
      return;
      }
   /* Stern explodiert */
   m_p->name_score[vflag]=m_p->name_score[vflag]-15;
   m_p->user_score[vflag]=m_p->user_score[vflag]-15;
   m_p->score[party[vflag]]=m_p->score[party[vflag]]-15;      
   init_expl(x,y,EXPL_TIME);
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
                    sprintf(print_str,"NOVA: %c at %03d %03d affected!",n_obj,i,j);
                    sort_msg(print_str); 
                    he=1200;
                    gauss2(&he);
                    hit(n_obj,i,j,he);
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
   sprintf(print_str,"  Mine at %03d %03d destroyed!",x,y);
   sort_msg(print_str);
   return;
   }

/*********/
/* Blind */
/*********/
if (obj==BLIND || obj==TORPEDO)
   {
   init_expl(x,y,EXPL_TIME);
   sprintf(print_str,"  Torpedo at %03d %03d destroyed",x,y);
   sort_msg(print_str);
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
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
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
      sprintf(print_str,"  Fort at %03d %03d destroyed!",x,y); 
      sort_msg(print_str);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
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
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->jumpgate[nr].dam=m_p->jumpgate[nr].dam+energy;
   for (i=1;i<MAX_USER;i++)
       {
       if (m_p->luser[i]==ACTIVE && party[i]==m_p->jumpgate[nr].party)
          {
          sprintf(print_str,"Jumpgate at %03d %03d under fire!",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->jumpgate[nr].dam>=MAX_DAM_JG)
      {
      eliminate_jumpgate(nr);
      init_expl(x,y,EXPL_TIME);
      sprintf(print_str,"Jumpgate at %03d %03d destroyed",x,y);
      sort_msg(print_str);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
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
  sort_msg("Ghosting; corrected!");
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
   usleep(EXPL_TIME); /* Wartezeit, um missile Zeit fuer Treffer zu geben! */
   m_p->missile[mparty].status=DESTROYED; 
   }       /* Ende missile */   


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
          sprintf(print_str,">Robotship at %03d %03d under fire!<",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->robotship[FEDERAL][nrob].shield==ON)
      {
      sort_msg(" Hit deflected by shields!");
      m_p->robotship[FEDERAL][nrob].sh_energy=m_p->robotship[FEDERAL][nrob].sh_energy-energy;
      if (m_p->robotship[FEDERAL][nrob].sh_energy<0) m_p->robotship[FEDERAL][nrob].sh_energy=0;
      m_p->robotship[FEDERAL][nrob].dam=m_p->robotship[FEDERAL][nrob].dam+(energy/10); 
      if (m_p->robotship[FEDERAL][nrob].sh_energy<=0) 
         {
         m_p->robotship[FEDERAL][nrob].shield=OFF;
         sort_msg(" Shields of robotship broke down!");
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
          sprintf(print_str,">Robotship at %03d %03d under fire!<",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->robotship[IMPERIAL][nrob].shield==ON)
      {
      sort_msg(" Hit deflected by shields!");
      m_p->robotship[IMPERIAL][nrob].sh_energy=m_p->robotship[IMPERIAL][nrob].sh_energy-energy;
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<0) m_p->robotship[IMPERIAL][nrob].sh_energy=0;
      m_p->robotship[IMPERIAL][nrob].dam=m_p->robotship[IMPERIAL][nrob].dam+(energy/10); 
      if (m_p->robotship[IMPERIAL][nrob].sh_energy<=0) 
         {
         m_p->robotship[IMPERIAL][nrob].shield=OFF;
         sort_msg(" Shields of robotship broke down!");
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

/**************/
/* Heimatwelt */
/**************/
if (obj==F_HOMEWORLD)
   {
   m_p->homeworld[FEDERAL].red_count=MAX_RED_COUNT;
   m_p->homeworld[FEDERAL].condition=RED;
   if (party[vflag]==IMPERIAL) 
      {
      pnts=50;
      pntsd=10000;
      }
   else
      {
      pnts=-50;
      pntsd=-10000;      
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
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
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }
if (obj==E_HOMEWORLD)
   {
   m_p->homeworld[IMPERIAL].red_count=MAX_RED_COUNT;
   m_p->homeworld[IMPERIAL].condition=RED;
   if (party[vflag]==FEDERAL) 
      {
      pnts=50;
      pntsd=10000;
      }
   else
      {
      pnts=-50;
      pntsd=-10000;      
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
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
      eliminate_hw(IMPERIAL);
      init_expl(x,y,EXPL_TIME);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
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
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   m_p->planet[nr].dam=m_p->planet[nr].dam+energy;
   if (m_p->planet[nr].dam>=(MAX_DAM_PL*m_p->planet[nr].level))
      {
      eliminate_pl(nr);
      init_expl(x,y,EXPL_TIME);
      sprintf(print_str,"  Planet at %03d %03d destroyed!",x,y); 
      sort_msg(print_str);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
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
  sort_msg("Ghost image; vanished!");
  return;
  }
if (obj==F_SCOUT && m_p->scout[FEDERAL].status==DESTROYED)
  {
  m_p->a[x][y]=SPACE;
  sort_msg("Ghost image; vanished!");
  return;
  }

if (obj==E_SCOUT || obj==F_SCOUT)
  {
  tparty=IMPERIAL;
  if (obj == F_SCOUT) tparty=FEDERAL;
  if (party[vflag] != tparty)
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
  m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
  m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
  m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
  if (m_p->scout[tparty].dam>=MAX_DAM_SCOUT)
     {
     m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
     m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
     m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
     m_p->scout[tparty].status=DESTROYED; 
     }
  return;
  }       /* Ende scout */   


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
          sprintf(print_str,">Base at %03d %03d under fire!<",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->base[nr].sh==ON)
      {
      sort_msg(" Hit deflected by shields!");
      m_p->base[nr].sh_energy=m_p->base[nr].sh_energy-energy;
      if (m_p->base[nr].sh_energy<0) m_p->base[nr].sh_energy=0;      
      m_p->base[nr].dam=m_p->base[nr].dam+(energy/10);
      if (m_p->base[nr].sh_energy<=0) 
         {
         m_p->base[nr].sh=OFF;
         sort_msg(" Shields of base broke down!");
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
      sprintf(print_str,"  Base at %03d %03d destroyed!",x,y); 
      sort_msg(print_str);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
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
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   if (m_p->ship[0].shield==ON)
      {
      m_p->ship[0].sh_energy=m_p->ship[0].sh_energy-energy;
      m_p->ship[0].dam[0]=m_p->ship[0].dam[0]+(energy/10);
      if (m_p->ship[0].sh_energy<=0) 
         {
         m_p->ship[0].shield=OFF;
         sort_msg(" Shields of Rom. broke down!");
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
   if (party[vflag] == party[zflag])
      {
      pnts=-pnts;
      pntsd=-pntsd;
      }
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;   
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
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
         sprintf(print_str," Shields of %c broke down!",ships[zflag][0]);     
         sort_msg(print_str);
         }
      }
   else
      {
      m_p->ship[zflag].dam[device]=m_p->ship[zflag].dam[device]+energy; 
      }
   if (zflag != vflag)
      {
      sprintf(print_str,"%s attacked by %s",ships[zflag],ships[vflag]);
      send_msg(zflag,print_str);
      }
   sprintf(print_str,"** We got a %d units hit! **!",energy);
   send_msg(zflag,print_str);
   if (m_p->ship[zflag].dam[0]>=max_dam[zflag])
      {
      init_expl(x,y,EXPL_TIME);
      if (m_p->ship[zflag].hood==OFF)
         {
         sprintf(print_str,"!! %s destroyed !!",ships[zflag]); 
	 sprintf(m_p->recon_str[zflag],"%-12s: destroyed             ",ships[zflag]);
         }
      else
         {
         sprintf(print_str,"UFO on %03d %03d destroyed",x,y);
         }
      sort_msg(print_str);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
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
   sprintf(print_str," Satellite on %d %d destroyed",x,y);
   sort_msg(print_str);
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   return;
   }

sort_msg(" ERROR fehlerhaftes Objekt!");
return;
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


