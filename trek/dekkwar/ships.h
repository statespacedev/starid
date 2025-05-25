/*************************************************************************/
/***                          ships.h                                  ***/
/*************************************************************************/
/*** Kommentar nach Version 1.0.1                       ***/
/**********************************************************/
/*** 20.03.03: max_speed entfernt, anz_ttubes neu       ***/
/***           ave_torp_pow gleich                      ***/
/**********************************************************/
/*** 29.04.03: Version 1.1.0                            ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 11.08.07: anz_ttubes mit MAX_TT-n besetzt          ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 23.10.07: Einige Schiffsnamen geaendert            ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 11.03.08: Tippfehler Warlard korrigiert            ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/**********************************************************/
/*** 23.10.08: Name Buzzard -> Bloodhunt                ***/
/***           und bludgeon -> buzzard                  ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 27.12.09: Romulaner: Symbol geaendert (R)          ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
static const char ships[MAX_USER][12] = {"Romulan", /* 0 Romulaner  */
                         "GALACTICA",            /* 1 Traegersch. Foed. */
                         "AMERICA",              /* 2 */
                         "HADES",                /* 3 Traegersch. Imp. */
                         "ZYLON",                /* 4 */
                         "ENTERPRISE",           /* 5 Schlachtschiffe Foed. */
                         "LEXINGTON",            /* 6 */
                         "YORKTOWN",             /* 7 */
                         "DEFIANT",              /* 8 */
                         "MANTA",                /* 9 Schlachtschiffe Imp. */
                         "PYTHON",               /* 10 */
                         "JACKAL",               /* 11 */
                         "VIPER",                /* 12 */
                         "California",           /* 13 Kreuzer Foederation */ 
                         "Oklahoma",             /* 14 */
                         "Kansas",               /* 15 */
                         "Tennessee",            /* 16 */
                         "Nevada",               /* 17 */
                         "Utah",                 /* 18 */
                         "Falcon",               /* 19 Kreuzer Imperium */
                         "Iscariot",             /* 20 */
                         "Bloodhunt",            /* 21 */
                         "Warlord",              /* 22 */
                         "Scorpion",             /* 23 */
                         "Quarrel",              /* 24 */
                         "kittyhawk",            /* 25 Zerstoerer Foederation */
                         "nostromo",             /* 26 */
                         "discovery",            /* 27 */
                         "leonow",               /* 28 */
                         "endeavour",            /* 29 */
                         "ticonderoga",          /* 30 */
                         "columbia",             /* 31 */
                         "orion",                /* 32 */
                         "avenger",              /* 33 */
                         "yorikke",              /* 34 */
                         "vulture",              /* 35 Zerstoerer Imperium */
                         "wasp",                 /* 36 */
                         "buzzard",              /* 37 */
                         "hornet",               /* 38 */
                         "falchion",             /* 39 */
                         "shark",                /* 40 */
                         "mamba",                /* 41 */
                         "panther",              /* 42 */  
                         "intruder",             /* 43 */
                         "jaguar",               /* 44 */  };
static const int level_of_ship[MAX_USER] = {BATTLESHIP,  /* Schiffstyp */
                                    CARRIER,CARRIER,CARRIER,CARRIER,
                                    BATTLESHIP,BATTLESHIP,BATTLESHIP,BATTLESHIP,
                                    BATTLESHIP,BATTLESHIP,BATTLESHIP,BATTLESHIP,
                                    CRUISER,CRUISER,CRUISER,CRUISER,CRUISER,
                                                                     CRUISER,
                                    CRUISER,CRUISER,CRUISER,CRUISER,CRUISER,
                                                                     CRUISER,
                                    DESTROYER,DESTROYER,DESTROYER,DESTROYER,
                                     DESTROYER,DESTROYER,DESTROYER,DESTROYER,
                                    DESTROYER,DESTROYER,DESTROYER,DESTROYER,
                                    DESTROYER,DESTROYER,DESTROYER,DESTROYER,
                                     DESTROYER,DESTROYER,DESTROYER,DESTROYER };
static const int party[MAX_USER] =   {ROMULAN,                /* Zugehoerigkeit */
                          FEDERAL,FEDERAL,
                          IMPERIAL,IMPERIAL,
                          FEDERAL,FEDERAL,FEDERAL,FEDERAL,
                          IMPERIAL,IMPERIAL,IMPERIAL,IMPERIAL,
                          FEDERAL,FEDERAL,FEDERAL,FEDERAL,FEDERAL,FEDERAL,
                          IMPERIAL,IMPERIAL,IMPERIAL,IMPERIAL,IMPERIAL,IMPERIAL,
                          FEDERAL,FEDERAL,FEDERAL,FEDERAL,
                              FEDERAL,FEDERAL,FEDERAL,FEDERAL,
                          FEDERAL,FEDERAL,IMPERIAL,IMPERIAL,
                          IMPERIAL,IMPERIAL,IMPERIAL,IMPERIAL,
                              IMPERIAL,IMPERIAL,IMPERIAL,IMPERIAL};
static const char kind_of_ship[MAX_USER][20] = {"ROMULAN Battleship",  /* Schiffstyp */
                                    "Federal Carrier",
                                    "Federal Carrier",
                                    "Imperial Carrier",
                                    "Imperial Carrier",
                                    "Federal Battleship",
                                    "Federal Battleship",
                                    "Federal Battleship",
                                    "Federal Battleship",
                                    "Imperial Battleship",
                                    "Imperial Battleship",
                                    "Imperial Battleship",
                                    "Imperial Battleship",
                                    "Federal Cruiser",
                                    "Federal Cruiser",
                                    "Federal Cruiser",
                                    "Federal Cruiser",
                                    "Federal Cruiser",
                                    "Federal Cruiser",
                                    "Imperial Cruiser",
                                    "Imperial Cruiser",
                                    "Imperial Cruiser",
                                    "Imperial Cruiser",
                                    "Imperial Cruiser",
                                    "Imperial Cruiser",
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Federal Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",	
                                    "Imperial Destroyer",};
static const char type_of_ship[MAX_USER][11] = {"Battleship",  /* Schiffstyp */
                                    "Carrier",
                                    "Carrier",
                                    "Carrier",
                                    "Carrier",
                                    "Battleship",
                                    "Battleship",
                                    "Battleship",
                                    "Battleship",
                                    "Battleship",
                                    "Battleship",
                                    "Battleship",
                                    "Battleship",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Cruiser",
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",	
                                    "Destroyer",};
static const int max_sat[MAX_USER] = { 0,    /* Anzahl der max. mitfuehrbaren Satelliten */
                           5,5,5,5,
                           5,5,5,5,5,5,5,5,
                           3,3,3,3,3,3,3,3,3,3,3,3,
                           2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
static const int max_mines[MAX_USER] = { 0,   /* Anzahl der max. mitfuehrbaren Minen */
                             10,10,10,10,
                             10,10,10,10,10,10,10,10,
                             6,6,6,6,6,6,6,6,6,6,6,6,
                             6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};
static const int max_dam[MAX_USER] = { 2500,  /* max. Schadenskapazitaet */
                           3000,3000,3000,3000,
                           2500,2500,2500,2500,2500,2500,2500,2500,
                           2000,2000,2000,2000,2000,2000, 
                                2000,2000,2000,2000,2000,2000, 
                           1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500,
                                1500,1500,1500,1500,1500,1500,1500,1500};
static const int max_torp[MAX_USER] = { 999,   /* Max. Torpedomenge */
                            25,25,25,25,
                            25,25,25,25,25,25,25,25,
                            20,20,20,20,20,20,20,20,20,20,20,20,
                            15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15};
static const int max_m_en[MAX_USER] = { 9999,  /* max. Hauptenergie */
                            6000,6000,6000,6000,
                            5000,5000,5000,5000,5000,5000,5000,5000,
                            4000,4000,4000,4000,4000,4000,
                                 4000,4000,4000,4000,4000,4000,
                            4000,4000,4000,4000,4000,4000,4000,4000,
                                 4000,4000,4000,4000,
                                 4000,4000,4000,4000,4000,4000,4000,4000};
static const int max_ph_en[MAX_USER] = {9999,  /* max. Phaserenergie */
                            2000,2000,2000,2000,
                            2500,2500,2500,2500,2500,2500,2500,2500,
                            2000,2000,2000,2000,2000,2000,
                                 2000,2000,2000,2000,2000,2000,
                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const int max_sh_en[MAX_USER] = { 2500,  /* max. Schildenergie */
                             3000,3000,3000,3000,
                             2500,2500,2500,2500,2500,2500,2500,2500,
                             2000,2000,2000,2000,2000,2000,
                                  2000,2000,2000,2000,2000,2000,
                             1500,1500,1500,1500,1500,1500,1500,1500,
                                  1500,1500,1500,1500,
                                  1500,1500,1500,1500,1500,1500,1500,1500};
static const int max_ph[MAX_USER] = { 400, /* max. Phaser-Schuss-Staerke */
                          400,400,400,400,
                          500,500,500,500,500,500,500,500,
                          400,400,400,400,400,400,400,400,400,400,400,400,
                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const int ave_torp_pow[MAX_USER] = { 0, /* mittlere Torpedostaerke */
                             250,250,250,250,
			     250,250,250,250,250,250,250,250,
                             250,250,250,250,250,250,250,250,250,250,250,250,
                             250,250,250,250,250,250,250,250,250,250,250,250,
                             250,250,250,250,250,250,250,250};
static const int max_fuel[MAX_USER] = {0,   /* Treibstoff fuer Impulsantrieb */
                                150,150,150,150,
                                130,130,130,130,130,130,130,130,
                                120,120,120,120,120,120,120,120,120,120,120,120,
                                100,100,100,100,100,100,100,100,100,100,100,100,
                                    100,100,100,100,100,100,100,100};
static const int anz_ttubes[MAX_USER] = { MAX_TT-3, /* Anzahl Torpedo-Tubes */
		   MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,
		   MAX_TT,MAX_TT,MAX_TT,MAX_TT,MAX_TT,MAX_TT,MAX_TT,MAX_TT,
		   MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,MAX_TT-1,
                   MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2,MAX_TT-2};
