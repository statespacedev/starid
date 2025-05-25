/*****************************************************************************/
/*** dw_shm.h                                                              ***/
/*****************************************************************************/
/*** Kommentar nach Version 1.0.1                       ***/
/**********************************************************/
/*** 20.03.03:        used_ttubes neu                   ***/
/*** 25.04.03: m_p->l_torp[vflag] entfernt              ***/
/**********************************************************/
/*** 29.04.03: Version 1.1.0                            ***/
/**********************************************************/
/*** Kommentar nach Version 1.1.0                       ***/
/**********************************************************/
/*** 26.05.04: Neue Fort-struct                         ***/
/*** 27.05.04: Alte Fort-struct entfernt                ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 22.12.06: Anzahl Spieler jeder Partei anz_user_f/e ***/
/*** 17.07.07: mp->l_hw durch mp->display_status ersetzt***/
/*** 24.07.07: recon_str (Aufklaerungsdaten) eingefuehrt***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 28.07.07: m_p->xsurvey_area eingefuehrt            ***/
/*** 05.08.07: m_p->map eingefuehrt                     ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 23.01.08: Umstellung struct satellite              ***/
/*** 24.01.08: Umstellung struct planet                 ***/
/*** 25.01.08: Umstellung struct base                   ***/
/*** 06.02.08: Neue Elemente struct base: bay,condition,***/
/***           red_count                                ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 16.02.08: m_p->radarlee eingefuehrt                ***/
/*** 22.02.08: Sprungtor-Funktion                       ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 27.02.08: struct scout eingefuehrt                 ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 12.03.08: Missile neu                              ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 13.03.08: Umstellung auf struct homeworld          ***/
/*** 21.03.08: l_hit,l_capture weg                      ***/
/*** 26.03.08: l_defeat weg                             ***/
/*** 26.03.08  Umstellung auf struct ship               ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 03.04.08: SV-Flag eingebaut                        ***/
/*** 06.04.08: Missile path eingefuehrt                 ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 12.12.09: tt_lock_time bei struct ship neu         ***/
/*** 13.12.09: used_ttubes entfernt                     ***/
/*** 01.01.10: Laenge recon_str auf SCANX gesetzt       ***/
/*** 11.04.10: mpath und imp neu                        ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
#define SHMEM "DW667"  /* symb. Shared-Memory-Name */
#define MODE 0666            /* Schutzbits */
#define ERROR (-1)          /* Fehlercode bei Shared Memory */
#define DW_SHM struct dw_shm  /* Struktur der Shared-Memory */
DW_SHM 
         {
         int l_game;                /* Spiel an=1 , aus=0 */
         int anz_user;              /* Zahl der Spieler (ohne Rom.) */
         int anz_user_f;            /* Zahl der Spieler Fed. */
         int anz_user_e;            /* Zahl der Spieler Emp. */
         int rw_lock;               /* steuert Schreibzugriff auf shm */
         int shm_id;                /* Shared Memory ID */
         int l_exist[MAX_USER];     /* Existiert=1, vernichtet=0, [0]=Rom. */
         int luser[MAX_USER];       /* 1=im Spiel,0=nicht im Spiel,[0]=Rom.*/
         int l_exit[MAX_USER];      /* 1= raus aus dem Spiel, 0=im Spiel */
         int display_status[MAX_USER]; /* Display-Moeglichk. Radarbereich*/
	 int rank[MAX_USER];          /* Rang des Kommandanten 1-4*/
         char name_of_cmdr[MAX_USER][21]; /* Name des Kommandanten */
         char rank_of_name[MAX_USER][10]; /* Rang des Namens */
         char recon_str[MAX_USER][SCANX]; /* String letzte Sichtung */
         char cmd[MAX_USER][CMD_LENGTH];      /* Kommando-String */
         char mr_cmd[MAX_USER][CMD_LENGTH];   /* Kopie von cmd fuer MOVE */
         char to_cmd[MAX_USER][CMD_LENGTH];   /* Kopie von cmd fuer TORPEDO */
         char ph_cmd[MAX_USER][CMD_LENGTH];   /* Kopie von cmd fuer PHASER */ 
         char a[ARRAY_SIZE][ARRAY_SIZE];      /* Spielfeld */ 
         char map[2][MAP_SIZE][MAP_SIZE]; /* Uebersichtskarte Fed/Emp */
         int isurvey_area[2];         /* Ueberwachtes Gebiet Fed,Emp. */
         float fsurvey_area[2];       /* Ueberwachtes Gebiet Fed,Emp. (%) */
         int radarlee;                /* Flag fuer Radarlee: 0=aus 1=an */
         int mpath[2][MAX_MPATHS][MISSILE_FUEL+1][2]; /* Pfad f. Missiles */
         int imp[2][MAX_MPATHS]; /* Pfad-Laengen */
         struct
         {
	   int status;        /* Status: IN_GAME, NOT_IN_GAME, DESTROYED */
	   int pos[2];        /* Position x und y */
	   int main_energy;   /* Hauptenergie */
	   int sh_energy;     /* Shield-Energie */
           int ph_energy;     /* Phaser-Energie */
           int torp;          /* Zahl der Torpedos */
	   int sat;           /* Satelliten */ 
           int mines;         /* Mines */
           int fuel;          /* Treibstoff fuer Impulsantrieb */
           int status_ttubes[MAX_TT]; /* Status Torpedo-Tubes */
           long long tt_lock_time[MAX_TT]; /* Startzeitpunkt Torpedo */
           int dam[11];       /* Damage-Punkte: [0]=Summe */
                              /* [1]=Warp-Engines [2]=Imp.-Engines */
                              /* [3]=Torp.-Tubes  [4]=Phaser */
                              /* [5]=Shields      [6]=Computer */
                              /* [7]=Life-Support [8]=Radar  */
                              /* [9]=Funk         [10]=Hood */
           int ramm;          /* Rammflag*/
           int build;         /* Build-Flag */
           int hood;          /* Tarn-Flag */
           int phaser;        /* Phaser-Flag */
           int jump;          /* Sprung-Flag */
           int dock;          /* Dock-Flag */
           int move;          /* Flug-Flag */
           int shield;        /* Shield-Flag */
           int sv;            /* Save-Modus-Flag */
	   int mp;            /* Missile-Path-Flag */
           int imp;           /* Index-Nr. des Pfads fuer Missile */
           int mpath[MISSILE_FUEL+1][2]; /* Path fuer Missile, x y */
           char msg[7][33];   /* Message-Strings fuer Funk und Meld.*/
	 } ship[MAX_USER];    /* Schiffe, 0=Romulaner */     

         struct
         {
	   int status;        /* Status: IN_GAME, NOT_IN_GAME, DOCKED */
           char mission;      /* Art der Mission: N,P,M,... */
	   int pos[2];        /* Position x und y */
	   int main_energy;   /* Hauptenergie */
	   int sh_energy;     /* Shield-Energie */
           int torp;          /* Zahl der Torpedos */
	   int dam;           /* Damage-Punkte */	
           int fuel;          /* Treibstoff fuer Impulsantrieb */
           int shield;        /* 1=Shields up, 0=down */
	 } robotship[2][MAX_ROBOTSHIPS];   /* Robot-Schiffe */

         struct
           {
           int status;     /* Status: IN_GAME oder NOT_IN_GAME */
	   int pos[2];     /* Position */
	   int dam;        /* Damage-Punkte */	
	   int condition;  /* Alarmzustand */
	   int red_count;  /* Zaehler fuer roten Alarm */
           } homeworld[2]; /* Je eine Heimatwelt */

         struct
           {
           int status; /* Status: IN_GAME oder NOT_IN_GAME */
	   int pos[2]; /* Position */
	   int energy; /* Energie */
	   int dam;    /* Damage-Punkte */
	   int target[2]; /* Ziel */
	 } scout[2];  /* Ein Scout fuer jede Partei */

         struct
           {
           int status; /* Status: IN_GAME oder NOT_IN_GAME */
	   int pos[2]; /* Position */
           int nr;     /* Nummer */
	   int fuel;   /* Treibstoff */
	   int subtarget[2]; /* Zwischenziel */
	   int target[2]; /* Ziel */
	   int dam;    /* Damage-Punkte */
	   int path[MISSILE_FUEL+1][2]; /* Path fuer Missile, x y */
	 } missile[2];  /* Ein missile fuer jede Partei */

         struct
           {
           int status; /* Status: IN_GAME oder NOT_IN_GAME */
	   int pos[2]; /* Position des Sprungtores */
           int code;   /* Code-Nummer */
	   int dam;    /* Schadenspunkte */
	   int party;  /* Partei: 0=Fed. 1=Emp. 2=neutral */
           } jumpgate[MAX_JUMPGATES]; /* Sprungtore */

         struct
           {
           int status; /* Status der Basis: 0=frei 1=belegt 2=reserviert */
           int party;  /* Partei der Basis: 0=Fed. 1=Emp. 2=neutral */
           int dam;    /* Schadenspunkte des Forts */
	   int sh;     /* Shields of Base: 0=down, 1=up */
	   int sh_energy; /* Energy of Shields of Base */
           int pos[2]; /* Position x,y der Basis */
	   char bay[4]; /* Belegung der 4 Docking-Bays */
	   int condition; /* Alarmzustand */
	   int red_count; /* Zaehler fuer roten Alarm */
           } base[MAX_BASES];

         struct
           {
           int nr;     /* laufende Nummer des Planeten */
           int status; /* Status des Planeten: 0=frei 1=belegt */
           int party;  /* Partei des Planeten: 0=Fed. 1=Emp. 2=neutral */
           int level;  /* Level des Planeten (1-4)*/
           int dam;    /* Schadenspunkte des Planeten */
           int pos[2]; /* Position x,y des Planeten */
           } planet[MAX_PLANETS];

         struct
           {
           int nr;     /* laufende Nummer des Forts */
           int status; /* Status des Forts: 0=frei 1=belegt 2=reserviert */
           int party;  /* Partei des Forts: 0=Fed. 1=Emp. 2=neutral */
           int dam;    /* Schadenspunkte des Forts */
           int pos[2]; /* Position x,y des Forts */
           } fort[MAX_FORTS];

         struct
           {
           int party;  /* Partei des Satelliten: 0=Fed. 1=Emp. 2=neutral */
	   int pos[2]; /* Position x,y des Satelliten */
           } satellite[MAX_SATS];

         int name_score[MAX_USER];    /* Scored Points of name */
         int user_score[MAX_USER];    /* Scored Points, [0]=Rom gesamt */ 
         int score[2];                /* Scored Points 1=Emp. 0=Fed. */
         }; 
      

static DW_SHM *m_p;         /* Puffer-Zeiger */
static int shmid;           /* Shared Memory-ID */

extern void *shmat();
