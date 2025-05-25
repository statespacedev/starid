/**********************************************************/
/*** dw_def.h                                           ***/
/*** Kommentar nach V.1.00                              ***/
/*** 26.02.03:    MAX_DAM_HOMEWORLD von 10000 auf 20000 ***/
/*** Kommentar nach V.1.0.1                             ***/
/*** LOADED,EMPTY und RELOADING fuer Torpedo-Tubes neu  ***/
/**********************************************************/
/*** 29.04.03: Version 1.1.0                            ***/
/*** Kommentar nach V.1.1.0                             ***/
/*** CLAIMED neu                                        ***/
/*** EXPL_TIME (Dauer einer Explosion auf dem Schirm)   ***/
/*** 30.06.04: MSG_LENGTH neu                           ***/
/*** 30.06.04: CMD_LENGTH neu                           ***/
/**********************************************************/
/*** 30.06.04: Version 1.2.0                            ***/
/**********************************************************/
/*** 17.07.07: HW_xxx durch DISPLAY_xxx ersetzt         ***/
/**********************************************************/
/*** 24.07.07: Version 1.3.0                            ***/
/**********************************************************/
/*** 11.08.07: MAX_TT auf 3 gesetzt                     ***/
/*** 12.08.07: DISPLAY_MAP eingefuehrt                  ***/ 
/*** 27.08.07: SCANX,Y                                  ***/
/**********************************************************/
/*** 02.09.07: Version 1.3.1                            ***/
/**********************************************************/
/*** 06.02.08: base.conditon: RED,GREEN,YELLOW          ***/
/***                          MAX_RED_COUNT             ***/
/**********************************************************/
/*** 09.02.08: Version 1.3.2                            ***/
/*** 16.02.08: RLEE_ON/OFF eingefuehrt                  ***/
/*** 22.02.08: Sprungtor-Funktion                       ***/
/**********************************************************/
/*** 22.02.08: Version 1.4.0                            ***/
/**********************************************************/
/*** 25.02.08: SPACE und MINE vertauscht                ***/
/*** 26.02.08: Symbol fuer DOCKING entfernt             ***/
/*** 27.02.08: struct scout eingefuehrt                 ***/
/**********************************************************/
/*** 28.02.08: Version 1.4.1                            ***/
/**********************************************************/
/*** 12.03.08: Missile neu                              ***/
/**********************************************************/
/*** 12.03.08: Version 1.4.2                            ***/
/**********************************************************/
/*** 13.03.08: HOMEWORLD und N_HOMEWORLD weg            ***/
/*** 15.03.08: HOOD_ON/OFF, RLEE_ON/OFF, BEAM_ON/OFF,   ***/
/***           RAMM_ON/OFF durch ON/OFF ersetzt,        ***/
/***           DEFEAT_ON/OFF weg. N_/EXISTING durch     ***/
/***           NOT_/IN_GAME ersetzt                     ***/
/*** 21.03.08: N_BUILDING/BUILDING durch ON/OFF ers.    ***/
/*** 23.03.08: STOPPED durch OFF und MOVING durch ON    ***/
/***           ersetzt                                  ***/
/*** 24.03.08: SH_UP/DOWN durch ON/OFF ersetzt          ***/
/***           N/JUMPING durch OFF/ON ersetzt           ***/
/**********************************************************/
/*** 30.03.08: Version 1.4.3                            ***/
/**********************************************************/
/*** 07.04.08: Version 1.4.4                            ***/
/**********************************************************/
/*** 05.05.08: MISSILE_DELAY und SCOUT_DELAY neu        ***/
/*** 25.10.08: Neue Symbole fuer Scouts, Robots, Miss.  **/
/*** 28.10.08: Grundfunktionalitaeten Robotschiffe mit  ***/
/***           Mission P und M                          ***/
/**********************************************************/
/*** 28.10.08: Version 1.5.0                            ***/
/**********************************************************/
/*** 20.10.09: NULLF fuer fopen-Abfragen                ***/
/*** 04.11.09: CMD_LENGTH, MSG_LENGTH angepasst         ***/
/*** 28.11.09: SIDEX,SIDEY,TERM_WIDTH/HEIGHT neu        ***/
/*** 28.11.09: MAX_NAMES neu: Zahl Spieler in user.dat  ***/
/*** 05.12.09: RELOAD_TT_TIME neu                       ***/
/*** 26.12.09: Symbol Romulaner jetzt R                 ***/
/***           Symbol Fed. Robotship jetzt !            ***/
/***           Symbol EXPLOSION jetzt ?                 ***/
/*** 08.01.10: FORT_BUILD_RESS und JUMPGATE_BUILD_RESS  ***/
/***           neu eingefuehrt                          ***/
/*** 09.01.10: BLIND_ und MINE_COL_ENERGY neu           ***/
/*** 13.01.10: MISSILE_COL_ENERGY neu                   ***/
/*** 17.01.10: ROM_DELAY neu                            ***/
/*** 23.01.10: RTORP_DELAY neu                          ***/
/*** 24.01.10: SCOUT_SCAN_INTERVAL neu                  ***/
/*** 20.02.10: DEF_DELAY neu                            ***/
/*** 21.02.10: ROM_DELAY von 6600000 auf 4000000        ***/
/*** 13.03.10: ROBOTSHIP_FUEL von 200 auf 300 gesetzt   ***/
/*** 13.03.10: WAITING neu (fuer Robotship. fuel <= 0)  ***/
/*** 21.03.10: MIN_ROBOTSHIP_ENERGY neu                 ***/
/*** 27.03.10: ROBOTSHIP_BUILD_TIME neu                 ***/
/*** 27.03.10: CLAIMED von 2 auf 5 gesetzt              ***/
/*** 10.04.10: MAX_DAM_HW von 10000 auf 15000 gesetzt   ***/
/*** 10.04.10: MISSILE_EXPL von 4000 auf 3000 gesetzt   ***/
/*** 11.04.10: MAX_MPATHS neu                           ***/
/*** 24.04.10: neue Konstantendefinitionen              ***/
/**********************************************************/
/*** 01.05.10: Version 1.5.1                            ***/
/**********************************************************/
/*** 03.05.10: MIN_HW_DIFF neu, auf 80 gesetzt          ***/
/*** 22.05.10: CANCELED neu                             ***/
/**********************************************************/
/*** 06.06.10: Version 1.5.2                            ***/
/**********************************************************/
/*** 03.10.11: ROBOTSHIP_CAPTURE_DELAY neu              ***/
/*** 23.10.11: ROBOTSHIP_CA_ENERGY neu                  ***/
/**********************************************************/
#define NULLF (FILE *)0
#define ROMULAN    'R'
#define STAR       '*'
#define BLACK_HOLE ' '
#define PLANET     '@'
#define SPACE      '.'
#define F_BASE     ')'
#define E_BASE     ']'
#define F_FORT     '$'
#define E_FORT     '#' 
#define F_SAT      '~'
#define E_SAT      '^'
#define BLIND      '+'
#define TORPEDO    '+'
#define CLOUD      '='
#define F_SCOUT    ':'
#define E_SCOUT    ';'
#define E_HOMEWORLD '«'
#define F_HOMEWORLD '»'
#define JUMPGATE   '§'
#define MISSILE    '-'
#define EXPLOSION  '?'
#define HOOD       ','
#define MINE       '·'
#define F_ROBOTSHIP '!'
#define E_ROBOTSHIP '|'
#define EXPL_TIME  300000
#define MSG_LENGTH  32
#define CMD_LENGTH  34
#define ARRAY_SIZE  200
#define MAP_SIZE    20
#define MAX_USER    45
#define NO_EXIT     0
#define EXIT        1
#define NOT_DOCKED  0
#define FEDERAL     0
#define IMPERIAL    1
#define NEUTRAL     2
#define LEVEL_1     1
#define LEVEL_2     2
#define LEVEL_3     3
#define LEVEL_4     4
#define MAX_PLANETS 499
#define MAX_BASES   99
#define BASE_SH_EN  3000
#define LIEUTENANT  0
#define COMMANDER   1
#define CAPTAIN     2
#define COMMODORE   3
#define ADMIRAL     4
#define LIEUTENANT_SC 7000
#define CAPTAIN_SC   15000
#define COMMODORE_SC 30000
#define ADMIRAL_SC   100000
#define CORVETTE    0
#define DESTROYER   1
#define CRUISER     2
#define BATTLESHIP  3
#define CARRIER     4
#define MAX_DAM_HOMEWORLD 15000
#define MAX_DAM_PL  500
#define MAX_DAM_BAS 2500
#define MAX_DAM_SCOUT 1000
#define MAX_DAM_JG  2000
#define SCOUT_ENERGY 1000
#define PL_PH_RANGE 2
#define PH_PLANET   100
#define PH_BASE     500
#define PH_HOMEWORLD 1510
#define FORT_RANGE  9
#define PH_FORT     250
#define SAT_RANGE   9
#define BASE_RANGE  9
#define SHIP_RANGE  9
#define HOMEWORLD_RANGE  9
#define HOMEWORLD_WRANGE 15
#define ON          1
#define OFF         0
#define MAX_FORTS   199
#define MAX_DAM_F   1200
#define MAX_SATS    199
#define HIT         1
#define NO_HIT      0
#define EXPLODING   1
#define N_EXPLODING 0
#define LOCKED      1
#define UNLOCKED    0
#define RUNNING     1
#define NOT_RUNNING 0
#define DISPLAY_RADAR    0
#define DISPLAY_RECON    1
#define DISPLAY_SCOUT    2
#define DISPLAY_HW       3
#define DISPLAY_FLEET    4
#define DISPLAY_BP       5
#define DISPLAY_CAPTURING   6
#define DISPLAY_HIT      7
#define DISPLAY_MAP      8
#define DISPLAY_BASE     9
#define DISPLAY_JUMP     10
#define DISPLAY_ROBOT    11
#define LOADED      1
#define EMPTY       0
#define RELOADING   2
#define MAX_TT      3
#define TERM_WIDTH 80
#define TERM_HEIGTH 24
#define SCANX      45
#define SIDEX      (TERM_WIDTH-SCANX)
#define SCANY      21
#define SIDEY      (TERM_HEIGTH-SCANY)
#define GREEN       0
#define YELLOW      1
#define RED         2
#define MAX_RED_COUNT 5
#define MAX_JUMPGATES 6
#define JUMP_DURATION 4000000
#define MISSILE_FUEL 200
#define MISSILE_EXPL 3000
#define MAX_DAM_MISSILE 400
#define MAX_ROBOTSHIPS 10
#define MAX_DAM_ROBOTSHIP 2000
#define ROBOTSHIP_ENERGY 2000
#define MIN_ROBOTSHIP_ENERGY 100
#define ROBOTSHIP_FUEL 300
#define ROBOTSHIP_TORPEDOES 20
#define ROBOTSHIP_SH_ENERGY 1800
#define ROBOTSHIP_BUILD_TIME 30000000
#define ROBOTSHIP_CAPTURE_TIME 10000000
#define ROBOTSHIP_CA_ENERGY 400
#define RTORP_POWER 250
#define RELOAD_TT_TIME 2100000000
#define SCOUT_DELAY 2000000
#define ROBOT_DELAY 2200000
#define MISSILE_DELAY 800000
#define ROM_DELAY 400000
#define MAX_NAMES 99
#define BLIND_COL_ENERGY 500
#define MINE_COL_ENERGY 1000
#define MISSILE_COL_ENERGY 100
#define JUMPGATE_BUILD_RESS 3500
#define FORT_BUILD_RESS 1000
#define RTORP_DELAY 130000
#define SCOUT_SCAN_INTERVAL 100000
#define DEF_DELAY 4000000
#define MAX_MPATHS 3
#define DESTROYED     0
#define OPERATIONAL   1
#define ACTIVE        2
#define DOCKED        3
#define HW_DOCKED     4
#define CLAIMED       5
#define WAITING       6
#define UNDER_CONSTRUCTION 7
#define CANCELED      8
#define MIN_HW_DIFF   80
