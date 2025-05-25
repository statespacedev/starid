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
/***           NOT_/ACTIVE ersetzt                     ***/
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
/*** 11.04.10: m_p->mpath vorbesetzt                    ***/
/*** 24.04.10: neue Konstantendefinitionen aus dw_def.h  ***/
/*** 25.04.10: IMAX,IMIN,IABS ueberall in () gesetzt    ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 03.05.10: MIN_HW_DIFF neu                          ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
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
printf("Kreiere neues Universum!\n");
printf("Anfangswerte werden gesetzt.\n");
m_p->shm_id=shmid;
m_p->l_game = 0; 
m_p->radarlee = ON;
m_p->scout[IMPERIAL].status = DESTROYED; 
m_p->scout[FEDERAL].status = DESTROYED; 
m_p->scout[IMPERIAL].pos[0]=-99;
m_p->scout[IMPERIAL].pos[1]=-99;
m_p->scout[FEDERAL].pos[0]=-99;
m_p->scout[FEDERAL].pos[1]=-99;
nrp=nrb=0;
m_p->score[0] = 0;      
m_p->score[1] = 0;
m_p->homeworld[IMPERIAL].status = DESTROYED;
m_p->homeworld[FEDERAL].status = DESTROYED;
m_p->anz_user = 0;
m_p->anz_user_f = 0;
m_p->anz_user_e = 0;
m_p->rw_lock=UNLOCKED;
m_p->isurvey_area[0]=m_p->isurvey_area[1]=-1;
m_p->fsurvey_area[0]=m_p->fsurvey_area[1]=-1.0;

for (i=0;i<(MISSILE_FUEL+1);i++)
   {
   m_p->missile[FEDERAL].path[i][0]=-99;
   m_p->missile[FEDERAL].path[i][1]=-99;
   m_p->missile[IMPERIAL].path[i][0]=-99;
   m_p->missile[IMPERIAL].path[i][1]=-99;
   }

for (i=0;i<MAX_JUMPGATES;i++)
    {
    m_p->jumpgate[i].status=DESTROYED;
    m_p->jumpgate[i].pos[0]=-99;
    m_p->jumpgate[i].pos[1]=-99;
    m_p->jumpgate[i].code=0;
    m_p->jumpgate[i].dam=0;
    m_p->jumpgate[i].party=NEUTRAL;
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
    m_p->l_exist[i]=ACTIVE;
    m_p->l_exit[i]=NO_EXIT;
    m_p->luser[i]=DESTROYED;
    m_p->display_status[i]=DISPLAY_RADAR;      
    m_p->user_score[i]=0;
    m_p->name_score[i]=0;  
    m_p->rank[i]=COMMANDER;
    m_p->ship[i].status=DESTROYED;    
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
for (i=0;i < MAX_MPATHS;i++)
    {
    m_p->imp[FEDERAL][i]=0;
    m_p->imp[IMPERIAL][i]=0;
    for (j=0;j<(MISSILE_FUEL+1);j++)
        {
        m_p->mpath[FEDERAL][i][j][0]=-99;
        m_p->mpath[FEDERAL][i][j][1]=-99;
        m_p->mpath[IMPERIAL][i][j][0]=-99;
        m_p->mpath[IMPERIAL][i][j][1]=-99;
        }
     }
for (i=0;i < MAX_BASES;i++)
     {
     m_p->base[i].party=NEUTRAL;
     m_p->base[i].dam = 0;
     m_p->base[i].sh = OFF;
     m_p->base[i].status = DESTROYED;
     m_p->base[i].sh_energy = BASE_SH_EN;
     m_p->base[i].pos[0]=-99;
     m_p->base[i].pos[1]=-99;
     m_p->base[i].bay[0]=' ';
     m_p->base[i].bay[1]=' ';
     m_p->base[i].bay[2]=' ';
     m_p->base[i].bay[3]=' ';
     m_p->base[i].condition=GREEN;
     m_p->base[i].red_count=0;
     }

for (i=0;i < MAX_FORTS;i++)
     {
     m_p->fort[i].nr=i;
     m_p->fort[i].status=DESTROYED;
     m_p->fort[i].party=NEUTRAL;
     m_p->fort[i].dam=0;
     m_p->fort[i].pos[0]=-99;
     m_p->fort[i].pos[1]=-99;
     }

for (i=0;i < MAX_SATS;i++)
     {
     m_p->satellite[i].party=NEUTRAL;
     m_p->satellite[i].pos[0]=-99;
     m_p->satellite[i].pos[1]=-99;
     }
     

for (i=0;i < MAX_PLANETS;i++)
     {
     m_p->planet[i].nr=i;
     m_p->planet[i].status=DESTROYED;
     m_p->planet[i].party=NEUTRAL;
     m_p->planet[i].level=LEVEL_1;
     m_p->planet[i].dam=0;
     m_p->planet[i].pos[0]=-99;
     m_p->planet[i].pos[1]=-99;
     }

c1=c2=c1c=c2c=clu=cluc=clux=cluy=cluxc=cluyc=0;
z1=z2=z3=z4=z5=z6=z7=z8=z9=z0=zc=0;
for (i=0;i<200;i++)
    {
        printf("stufe: %d \n",i);    	
    for (j=0;j<200;j++)
        {
        m_p->a[i][j]=SPACE;
        rnd=ran();
        if (rnd>0.3013 && rnd<=0.3019)
           {
	   c1c=(NINT((10.0*ran())))+4;
	   c2c=(NINT((10.0*ran())))+4;           
           cluxc=i;
           cluyc=j;
           cluc=c1c*c2c;
           printf("Cloud: %d %d groesse= %d %d\n",i,j,c1c,c2c);
           }
        if (cluc>0 && (i-cluxc)<c1 && (j-cluyc)<c2 && (i-cluxc)>=0 && (j-cluyc)>=0)
           {
	   r=ran();
           if (r>0.3 && r<0.95) m_p->a[i][j]=CLOUD; 
           cluc--;
           }
        r=ran();
        if (r>0.5013 && r<=0.5019)
           {
	     c1=(NINT((10.0*ran())))+5;
	     c2=(NINT((10.0*ran())))+5;           
           clux=i;
           cluy=j;
           clu=c1*c2;
           printf("Cluster: %d %d groesse= %d %d\n",i,j,c1,c2);           
           }
        if (clu>0 && (i-clux)<c1 && (j-cluy)<c2 && (i-clux)>=0 && (j-cluy)>=0)
           {
	   ir=ran2((i*cluy),j);           
           if (ir<7) m_p->a[i][j]=SPACE;
           if (ir>6 && ir<9) m_p->a[i][j]=STAR;           
           if (ir==1) m_p->a[i][j]=CLOUD;           
           if (ir>=9 && nrp < MAX_PLANETS) 
              {
              m_p->a[i][j]=PLANET;
              m_p->planet[nrp].status=ACTIVE;
              m_p->planet[nrp].pos[0]=i;
              m_p->planet[nrp].pos[1]=j;
              nrp++;
              }	
           clu--;
           zc++;
           }
        else
	  {
          if ( rnd < 0.1 ) z1++;
          if ( rnd >= 0.1 && rnd<0.2 ) z2++;
          if ( rnd >= 0.2 && rnd<0.3 ) z3++;
          if ( rnd >= 0.3 && rnd<0.4 ) z4++;
          if ( rnd >= 0.4 && rnd<0.5 ) z5++;
          if ( rnd >= 0.5 && rnd<0.6 ) z6++;
          if ( rnd >= 0.6 && rnd<0.7 ) z7++;
          if ( rnd >= 0.7 && rnd<0.8 ) z8++;
          if ( rnd >= 0.8 && rnd<0.9 ) z9++;
          if ( rnd >= 0.9 ) z0++;
          if ( rnd>0.91 && rnd<=0.95) m_p->a[i][j]=STAR;
	  
	  if ( rnd>0.95 && rnd<=0.9525 && nrp < MAX_PLANETS)
             {
             m_p->a[i][j]=PLANET;
             m_p->planet[nrp].status=ACTIVE;
             m_p->planet[nrp].pos[0]=i;
             m_p->planet[nrp].pos[1]=j;
             nrp++;
             }
	  if ( rnd>0.96075 && rnd<=0.96100 &&  nrb < MAX_BASES) 
             {
             m_p->a[i][j]=F_BASE;
             m_p->base[nrb].party=FEDERAL;
             m_p->base[nrb].status=ACTIVE;
             m_p->base[nrb].sh=ON;
             m_p->base[nrb].pos[0]=i;
             m_p->base[nrb].pos[1]=j;
             nrb++;
             } 
          if ( rnd>0.96100 && rnd<=0.96125 && nrb < MAX_BASES)
             {
             m_p->a[i][j]=E_BASE;
             m_p->base[nrb].party=IMPERIAL;
             m_p->base[nrb].status=ACTIVE;
             m_p->base[nrb].sh=ON;
             m_p->base[nrb].pos[0]=i;
             m_p->base[nrb].pos[1]=j;
             nrb++;
             }
          if ( rnd>0.964 && rnd<=0.970) m_p->a[i][j]=BLACK_HOLE;
          }
      }
    }

/* HOMEWORLD */

i=-1;
j=1;
do {
  r=ran();
   j++;
   xhf= NINT(199.0*r);
   r=ran();
   j++;
   yhf= NINT(199.0*r);
   r=ran();
   j++;
   xhe= NINT(199.0*r);
   r=ran();
   j++;
   yhe= NINT(199.0*r);
   } while (m_p->a[xhe][yhe] != SPACE || 
            m_p->a[xhf][yhf] != SPACE || 
            xhe < 14  || yhe < 14  || xhf < 14  || yhf < 14  ||
            xhe > 185 || yhe > 185 || xhf > 185 || yhf > 185 ||
	    (IMAX(abs(yhe-yhf),abs(xhe-xhf))) < MIN_HW_DIFF);

m_p->homeworld[FEDERAL].status=ACTIVE;
m_p->homeworld[FEDERAL].dam=0;
m_p->homeworld[FEDERAL].pos[0]=xhf;
m_p->homeworld[FEDERAL].pos[1]=yhf;
m_p->homeworld[FEDERAL].condition=GREEN;
m_p->homeworld[FEDERAL].red_count=0;
m_p->a[xhf][yhf]=F_HOMEWORLD;
printf("Homeworld Fed. on %d %d\n",xhf,yhf);
if (m_p->a[xhf][yhf-1]==STAR || m_p->a[xhf][yhf-1]==CLOUD || m_p->a[xhf][yhf-1]==BLACK_HOLE) m_p->a[xhf][yhf-1]=SPACE;
if (m_p->a[xhf][yhf+1]==STAR || m_p->a[xhf][yhf+1]==CLOUD || m_p->a[xhf][yhf+1]==BLACK_HOLE) m_p->a[xhf][yhf+1]=SPACE;
if (m_p->a[xhf-1][yhf-1]==STAR || m_p->a[xhf-1][yhf-1]==CLOUD || m_p->a[xhf-1][yhf-1]==BLACK_HOLE) m_p->a[xhf-1][yhf-1]=SPACE;
if (m_p->a[xhf-1][yhf+1]==STAR || m_p->a[xhf-1][yhf+1]==CLOUD || m_p->a[xhf-1][yhf+1]==BLACK_HOLE) m_p->a[xhf-1][yhf+1]=SPACE;
if (m_p->a[xhf-1][yhf]==STAR || m_p->a[xhf-1][yhf]==CLOUD || m_p->a[xhf-1][yhf]==BLACK_HOLE) m_p->a[xhf-1][yhf]=SPACE;
if (m_p->a[xhf+1][yhf-1]==STAR || m_p->a[xhf+1][yhf-1]==CLOUD || m_p->a[xhf+1][yhf-1]==BLACK_HOLE) m_p->a[xhf+1][yhf-1]=SPACE;
if (m_p->a[xhf+1][yhf+1]==STAR || m_p->a[xhf+1][yhf+1]==CLOUD || m_p->a[xhf+1][yhf+1]==BLACK_HOLE) m_p->a[xhf+1][yhf+1]=SPACE;
if (m_p->a[xhf+1][yhf]==STAR || m_p->a[xhf+1][yhf]==CLOUD || m_p->a[xhf+1][yhf]==BLACK_HOLE) m_p->a[xhf+1][yhf]=SPACE;

m_p->homeworld[IMPERIAL].status=ACTIVE;
m_p->homeworld[IMPERIAL].dam=0;
m_p->homeworld[IMPERIAL].pos[0]=xhe;
m_p->homeworld[IMPERIAL].pos[1]=yhe;
m_p->homeworld[IMPERIAL].condition=GREEN;
m_p->homeworld[IMPERIAL].red_count=0;
m_p->a[xhe][yhe]=E_HOMEWORLD;
printf("Homeworld Emp. on %d %d\n",xhe,yhe);
if (m_p->a[xhe][yhe-1]==STAR || m_p->a[xhe][yhe-1]==CLOUD || m_p->a[xhe][yhe-1]==BLACK_HOLE) m_p->a[xhe][yhe-1]=SPACE;
if (m_p->a[xhe][yhe+1]==STAR || m_p->a[xhe][yhe+1]==CLOUD || m_p->a[xhe][yhe+1]==BLACK_HOLE) m_p->a[xhe][yhe+1]=SPACE;
if (m_p->a[xhe-1][yhe-1]==STAR || m_p->a[xhe-1][yhe-1]==CLOUD || m_p->a[xhe-1][yhe-1]==BLACK_HOLE) m_p->a[xhe-1][yhe-1]=SPACE;
if (m_p->a[xhe-1][yhe+1]==STAR || m_p->a[xhe-1][yhe+1]==CLOUD || m_p->a[xhe-1][yhe+1]==BLACK_HOLE) m_p->a[xhe-1][yhe+1]=SPACE;
if (m_p->a[xhe-1][yhe]==STAR || m_p->a[xhe-1][yhe]==CLOUD || m_p->a[xhe-1][yhe]==BLACK_HOLE) m_p->a[xhe-1][yhe]=SPACE;
if (m_p->a[xhe+1][yhe-1]==STAR || m_p->a[xhe+1][yhe-1]==CLOUD || m_p->a[xhe+1][yhe-1]==BLACK_HOLE) m_p->a[xhe+1][yhe-1]=SPACE;
if (m_p->a[xhe+1][yhe+1]==STAR || m_p->a[xhe+1][yhe+1]==CLOUD || m_p->a[xhe+1][yhe+1]==BLACK_HOLE) m_p->a[xhe+1][yhe+1]=SPACE;
if (m_p->a[xhe+1][yhe]==STAR || m_p->a[xhe+1][yhe]==CLOUD || m_p->a[xhe+1][yhe]==BLACK_HOLE) m_p->a[xhe+1][yhe]=SPACE;

for (i=0;i<MAP_SIZE;i++)
  {
  for (j=0;j<MAP_SIZE;j++)
    {
    m_p->map[0][i][j]='?';
    m_p->map[1][i][j]='?';
    }
  }

for (i=0;i<MAX_ROBOTSHIPS;i++)
    {
    m_p->robotship[FEDERAL][i].torp=ROBOTSHIP_TORPEDOES;
    m_p->robotship[FEDERAL][i].fuel=ROBOTSHIP_FUEL;
    m_p->robotship[FEDERAL][i].main_energy=ROBOTSHIP_ENERGY;
    m_p->robotship[FEDERAL][i].sh_energy=ROBOTSHIP_SH_ENERGY;
    m_p->robotship[FEDERAL][i].shield=OFF;
    m_p->robotship[FEDERAL][i].dam=0;
    m_p->robotship[FEDERAL][i].status=HW_DOCKED;
    m_p->robotship[FEDERAL][i].mission='N';
    m_p->robotship[FEDERAL][i].pos[0]=m_p->homeworld[FEDERAL].pos[0];
    m_p->robotship[FEDERAL][i].pos[1]=m_p->homeworld[FEDERAL].pos[1];
    m_p->robotship[IMPERIAL][i].torp=ROBOTSHIP_TORPEDOES;
    m_p->robotship[IMPERIAL][i].fuel=ROBOTSHIP_FUEL;
    m_p->robotship[IMPERIAL][i].main_energy=ROBOTSHIP_ENERGY;
    m_p->robotship[IMPERIAL][i].sh_energy=ROBOTSHIP_SH_ENERGY;
    m_p->robotship[IMPERIAL][i].shield=OFF;
    m_p->robotship[IMPERIAL][i].dam=0;
    m_p->robotship[IMPERIAL][i].status=HW_DOCKED;
    m_p->robotship[IMPERIAL][i].mission='N';
    m_p->robotship[IMPERIAL][i].pos[0]=m_p->homeworld[IMPERIAL].pos[0];
    m_p->robotship[IMPERIAL][i].pos[1]=m_p->homeworld[IMPERIAL].pos[1];
    }

printf("Neues Universum kreiert!\n");
 printf("z1=%d z2=%d z3=%d z4=%d z5=%d z6=%d z7=%d z8=%d z9=%d z0=%d zc=%d\n",z1,z2,z3,z4,z5,z6,z7,z8,z9,z0,zc);
zz=z1+z2+z3+z4+z5+z6+z7+z8+z9+z0+zc;
printf("Summe=%d\n",zz);
printf("Basen: %d Planeten: %d\n",nrb,nrp);

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
  clock_gettime(CLOCK_REALTIME, &ntime);
  i = (int) ntime.tv_nsec%10000;
  return i;
}

