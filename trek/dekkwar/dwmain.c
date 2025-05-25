/*****************************************************************************/
/*** dwmain.c                                                              ***/
/*****************************************************************************/
/*** 10.02.03: torpedo.c:     Bei Treffer auf Homeworld nicht laenger  ***/
/***                          das Symbol gezeigt, sondern Text         ***/
/*** 10.02.03: phaser.c:      Bei Treffer auf Homeworld nicht laenger  ***/
/***                          das Symbol gezeigt, sondern Text         ***/
/*** 10.02.03: dwmain.c:      Handling fuer Ctrl-C geaendert:          ***/
/***                          es wurde immer der letzte Befehl fuer das***/
/***                          Kommando wiederholt, da Shared-Memory    ***/
/*** 10.02.03: move.c:        Handling fuer Ctrl-C geaendert (s.o.)    ***/
/*** 10.02.03: phaser.c:      Handling fuer Ctrl-C geaendert (s.o.)    ***/
/*** 10.02.03: torpedo.c:     Handling fuer Ctrl-C geaendert (s.o.)    ***/
/*** 10.02.03: dwmain.c:      QUit unmoegl. in Reichw. der feindl. HW  ***/ 
/*** 10.02.03: dwmain.c:      String We -> You bei Meldungen           ***/
/*** 11.02.03: defeat.c:      Einbau Meldung HW Warnung und Schuss     ***/
/*** 11.02.03: romulan.c:     Signalhandling fuer CTRL-C               ***/
/*** 12.02.03: defeat.c:      Nachricht an alle eigenen Schiffe bei    ***/
/***                          Annaeherung eines Feindes an die eig. HW ***/
/*** 13.02.03: dwmain.c:      Fehlerhaften Einsprung bei HW-Start korr.***/
/*** 14.02.03: ueberall:      sort_msg: String auf 33 begrenzt         ***/
/***                          send_msg: dito                           ***/
/*** 18.02.03: ueberall:      Funktion TMusleep auskommentiert wegen   ***/
/***                          Portierung auf Linux; ist auf IRIX       ***/
/***                          ueberfl.. Nur noch dwmain.c, sonst weg   ***/
/*** 19.02.03: dwmain.c:      Kleine Aenderungen im Zeitablauf bei der ***/
/***                          Initialisierung des Terminals (initscr)  ***/
/*** 19.02.03: cockpit.c:     Window-Initialisierung in zwei Stufen    ***/
/***                          mit pause wegen Empty-Screen-Effekt      ***/
/*** 20.02.03: cockpit.c:     Aederung vom 19.2. entfernt, stattdessen ***/
/***                          warten, bis luser und xpos,ypos fuer     ***/
/***                          vflag in dwmain gesetzt worden sind.     ***/
/*** 20.02.03: cockpit.c:     Stringlaenge des Schiffnamens auf 14 im  ***/
/***                          head-Fenster                             ***/
/*** 20.02.03: cockpit.c:     strlen in sort_msg, sort_exit_msg nach-  ***/
/***                          getragen, war sonst schon drin           ***/
/*** 20.02.03: dwmain.c:      restart_procs fuer Neustart cockpit usw. ***/
/*** 25.02.03: ueberall:      neue Variable xmsg in sort_msg, da der   ***/
/***                          String new_msg u.U. geaendert wird       ***/
/***                          dito send_msg, sort_exit_msg             ***/
/*** 25.02.03: cockpit.c:     endwin nur bei normalem Ende wg. nicht   ***/
/***                          mehr angezeigtem R.I.P. bei Linux        ***/
/*** 26.02.03: dw_def.h:      MAX_DAM_HOMEWORLD von 10000 auf 20000    ***/
/*** 26.02.03: dwmain.c:      Behandlung fuer neuen User in user.dat,  ***/
/***                          bislang wurde dieser nicht eingetragen   ***/
/***                          bei QUit, RIP und HW_DOCKING             ***/
/*************************************************************************/
/*** 27.02.03: Version 1.01                                            ***/
/*************************************************************************/
/*** 17.03.03: romulan.c:     In find_nr_fort und find_nr_base das     ***/
/***                          i++ ans Ende der Schleife gesetzt, da    ***/
/***                          sonst der Wert 0 nie durchlaufen wurde   ***/
/***                          und der Returnwert nr falsch war.        ***/
/***                          Dadurch tauchten Geisterforts auf.       ***/
/*************************************************************************/
/*** 20.03.03: Version 1.0.1                                           ***/
/*************************************************************************/
/*** 20.03.03: dwmain.c       torpedo jetzt nicht mehr als permanenter ***/
/***                          Prozess mit Signalabfrage. Stattdessen   ***/
/***                          ein Prozess fuer jeden fliegenden T.     ***/
/*** 20.03.03: ships.h        max_speed entfernt, anz_ttubes neu,      ***/
/***                          ave_torp_pow gleich                      ***/
/*** 20.03.03: dw_shm.h       used_ttubes neu, ave_torp_pow gleich     ***/
/*** 20.03.03: dwmain.c       used_ttubes=0 bei Initialisierung,       ***/
/***                          Docking und HW                           ***/
/*** 20.03.03: move.c         jetzt sind beliebig viele Schritte       ***/
/***                          erlaubt (max_speed weg).                 ***/
/*** 20.03.03: torpedo.c      Neue Version, kein Dauerprozess mehr.    ***/
/***                          anz_ttubes und used_ttubes eingefuehrt.  ***/
/*** 24.03.03: move.c         Schritt-Geschwindigkeiten neu            ***/
/*** 24.03.03: move.c         aalt (alter Wert vom m_p->a[][]) ent-    ***/
/***                          fernt, da bei Warp immer SPACE, auch     ***/
/***                          nach Kollision mit Mine/Blindgaenger.    ***/
/***                          Schleife etwas vereinfacht.              ***/
/*** 25.03.03: torpedo.c      Dauer des Nachladens vom Schiffstyp abh. ***/
/*** 04.04.03: defeat.c       Falsches if bei HW geaendert: Statt      ***/
/***                          m_p->l_homeworld[FEDERAL] == ACTIVE nun ***/
/***                          m_p->l_homeworld[IMPERIAL] == EXISTING   ***/
/***                          und umgekehrt. Frueher feuerten zerst.HW ***/
/*** 04.04.03: dwmain.c       hw_docking: neue Abfrage ob HW existiert ***/
/*** 04.04.03: torpedo.c      Pause beim Nachladen jetzt in dwmain.c   ***/
/*** 04.04.03: ueberall       Unterprogramm eliminate_hw jetzt in libdw***/
/*** 04.04.03: eliminate_hw.c Position HW nach Vernichtung auf -99,-99 ***/
/*** 09.04.03: phaser.c       Treffer auf HW, hat bislang gefehlt      ***/
/*** 10.04.03: move.c,dwmain.c Stoppen der Bewegung aus dwmain mit STOP***/
/*** 11.04.03: torpedo.c      m_p->torp[vflag]-- vorverlegt wegen des  ***/
/***                          Problems mit negativer Torpedoanzahl.    ***/
/*** 11.04.03: cockpit.c      Anzeige Torpedo-Tube-Status eingebaut    ***/
/*** 11.04.03: torpedo.c      Status Torpedo-Tubes hinzugefuegt        ***/
/*** 11.04.03: dwmain.c       Setzen des Torpedo-Tube-Status bei Init. ***/
/*** 25.04.03: ueberall       m_p->l_torp[vflag] entfernt              ***/
/*** 25.04.03: cockpit.c      Torpedo-Tube(s)-Anzeige geaendert        ***/
/*** 29.04.03: dwmain.c       get_score: ueberfluessiges printf entf.  ***/
/*************************************************************************/
/*** 29.04.03: Version 1.1.0                                           ***/
/*************************************************************************/
/*** 20.02.04: dwmain.c       Kommando ST (STop) wenn kein HW_DOCKED   ***/
/***                          und com=" " nach Start von HW (Bef. 27a) ***/
/*** 20.02.04: dwmain.c       vflag_str neu nach SElect-Befehl damit   ***/
/***                          torpedo.c den neuen vflag bekommt        ***/ 
/*** 23.03.04: dwmain.c       void fort: Begrenzung auf MAX_FORTS-1,   ***/
/***                          war auf MAX_BASES-1.                     ***/
/*** 23.03.04: dwmain.c       void docking: Docken an Basis nur wenn   ***/
/***                          Warpantrieb gestoppt ist.                ***/
/*** 27.04.04: ueberall       Umstellung auf strncpy in sort_msg und   ***/
/***                          send_msg                                 ***/
/*** 07.05.04: ueberall       Umstellung struct fort                   ***/
/*** 29.05.04: ueberall       explosion neu als Fork                   ***/
/*** 30.06.04: ueberall       MSG_LENGTH neu in dw_def.h               ***/
/*** 30.06.04: dwmain.c       Ende bei Zerstoerung berichtigt          ***/
/*** 30.06.04: cockpit.c      Ende bei Zerstoerung berichtigt          ***/
/*** 30.06.04: cockpit.c      Umstellung auf strncpy in sort_exit_msg  ***/
/*** 30.06.04: dwmain.c       Umstellung auf strncpy in sort_send_msg  ***/
/*** 30.06.04: ueberall       Treffer auf Sektion 4 fuer Zerstoerer    ***/
/***                          korrigiert; -> Sektion 5                 ***/
/*** 30.06.04: cockpit.c      kein =0 in bei Zerstoerer Damage Sektion ***/
/***                          4 in Ueberpruefung Schadensumme          ***/
/*** 30.06.04: dwmain.c       void hit() einheitlich bei move, dwmain, ***/
/***                          phaser, torpedo                          ***/
/*** 30.06.04: move.c         void hit() einheitlich bei move, dwmain, ***/
/***                          phaser, torpedo                          ***/
/*** 30.06.04: phaser.c       void hit() einheitlich bei move, dwmain, ***/
/***                          phaser, torpedo                          ***/
/*** 30.06.04: torpedo.c      void hit() einheitlich bei move, dwmain, ***/
/***                          phaser, torpedo                          ***/
/*** 30.06.04: ueberall       libdw weg                                ***/
/*** 30.06.04: dw_def.h       CMD_LENGTH neu                           ***/
/*** 30.06.04: dwmain.c       CMD_LENGTH in static char xcmd           ***/
/*************************************************************************/
/*** 30.06.04: Version 1.2.0                                           ***/
/*************************************************************************/
/*** 24.08.04: dwmain.c       torpedo_pid weg                          ***/
/*** 22.10.04: ueberall       in Textausgabe " on x y" durch "at x y " ***/
/***                          ersetzt.                                 ***/
/*** 08.03.05: dwmain.c       strcpy u. strcat bei xcmd durch strn...  ***/
/***                          ersetzt.                                 ***/
/*** 06.05.05: dwmain.c       strcpy(m_p->cmd[nflag],m_p->cmd[vflag])  ***/
/***                          durch strncpy ersetzt.                   ***/
/*** 21.12.06: dwmain.c       Hilfstexte fuer Homeworld-Aktionen       ***/
/*** 10.01.07: dwmain.c       Ausgabe von ID geaendert (void identify) ***/
/*** 06.07.07: dwmain.c       in reach durch within range ersetzt      ***/
/*** 17.07.07: ueberall       m_p->l_hw durch m_p->display_status ers. ***/
/***                          HW_xxx durch DISPLAY_xxx ersetzt         ***/
/*** 21.07.07: dwmain.c       Hilfstexte fuer Homeworld-Aktionen erw.  ***/
/*** 22.07.07: dwmain.c       Neues Kommando SD (set display), DS weg  ***/
/*** 24.07.07: ueberall       recon_str (Aufklaerungsdaten) eingefuehrt***/
/*** 24.07.07: dwmain.c       Neue Startposition: HW (wenn existent),  ***/
/***                          sonst wie vorher Zufallswert             ***/
/*** 24.07.07: cockpit.c      Neue Startposition: Meldung (message)    ***/
/*************************************************************************/
/*** 24.07.07: Version 1.3.0                                           ***/
/*************************************************************************/
/*** 28.07.07: ueberall       Central Control-Text weg                 ***/
/*** 04.08.07: ueberall       Zerstoerung Schiff in recon_str          ***/
/*** 11.08.07: dwmain.c       Bei select jetzt auch Status der TTubes  ***/
/***                          gesetzt - vorher falsche Anzeige TT nach ***/
/***                          Schiffwechsel an HW im Cockpit           ***/
/*** 11.08.07: dw_def.h       MAX_TT auf 3 gesetzt                     ***/
/*** 11.08.07: ships.h        anz_ttubes mit MAX_TT-n besetzt          ***/
/*** 12.08.07: ueberall       DISPLAY_MAP eingefuehrt                  ***/
/*** 12.08.07: dwmain.c       Kommando SD M eingefuehrt                ***/
/*** 27.08.07: ueberall       SCANX,Y                                  ***/
/*** 29.08.07: dwmain.c       HE SD .. ergaenzt                        ***/
/*** 01.09.07: cockpit.c      Aenderung der SD BP-Anzeige: alle Rubri- ***/
/***                          ken werden angezeigt, auch wenn leer     ***/
/*************************************************************************/
/*** 02.09.07: Version 1.3.1                                           ***/
/*************************************************************************/
/*** 23.10.07: ueberall       Einige Schiffsnamen geaendert            ***/
/*** 23.01.08: ueberall       Umstellung struct satellite              ***/
/*** 24.01.08: ueberall       Umstellung struct planet                 ***/
/*** 25.01.08: ueberall       Umstellung struct base                   ***/
/*** 27.01.08: dwmain.c       Kommando SD HW (Homeworld base + HQ)     ***/
/*** 26.01.08: cockpit.c      Anzeige von SD HW (Homeworld base + HQ)  ***/
/*** 27.01.08: dwmain.c       HE SD HW eingebaut                       ***/
/*** 01.02.08: cockpit.c      Radarlee (testweise)                     ***/
/*** 01.02.08: dwmain.c       Radarlee (testweise, bei Kommando IN)    ***/
/*** 02.02.08: dwmain.c       Abfragen auf "10" auf SHIP_RANGE usw.    ***/
/*** 05.02.08: cockpit.c      Neues Dockbild bei Basis                 ***/
/*** 06.02.08: dwmain.c       Neues Docking an Basis                   ***/
/*** 06.02.08: dw_shm.h       Neue Elemente struct base: bay,condition,***/
/***                          red_count                                ***/
/*** 06.02.08: dw_def.h       base.conditon: RED,GREEN,YELLOW          ***/
/***                          MAX_RED_COUNT                            ***/
/*** 06.02.08: ueberall       Anpassungen auf neues Basen-Docking und  ***/
/***                          neue Elemente struct base                ***/
/*** 07.02.08: dwmain.c       Fehler bei Berechnung der zweitnaechsten ***/
/***                          Basis in void info behoben               ***/
/*** 08.02.08: ueberall       Wenn Basis zerstoert wird, werden auch   ***/
/***                          alle gedockten Schiffe zerstoert         ***/
/*** 09.02.08: dwmain.c       Ausdock-Position bei Basis jetzt wie HW  ***/
/*************************************************************************/
/*** 09.02.08: Version 1.3.2                                           ***/
/*************************************************************************/
/*** 11.02.08: torpedo.c      Wenn T. auf EXPLOSION trifft, wird T.    ***/
/***                          vernichtet. (Stillgelegt am 21.02.08)    ***/
/*** 12.02.08: cockpit.c      Kosmetik am HW-Dockbild                  ***/
/*** 12.02.08: dwmain.c       Infotexte SD angepasst                   ***/
/*** 16.02.08: ueberall       m_p->radarlee u. RLEE_ON/OFF eingefuehrt ***/
/*** 17.02.08: cockpit.c      Radarlee fuer scout                      ***/
/*** 17.02.08: defeat.c       Radarlee fuer Basen, Forts, Satelliten   ***/
/*** 21.02.08: dwmain.c       LE Y= ..., static char ycmd, void learnY ***/
/*** 21.02.08: dwmain.c       Radarlee bei quit_control                ***/
/*** 22.02.08: ueberall       Sprungtor-Funktion                       ***/
/*************************************************************************/
/*** 22.02.08: Version 1.4.0                                           ***/
/*************************************************************************/
/*** 23.02.08: dwmain.c       HE SD um HW ergaenzt                     ***/
/*** 25.02.08: defeat.c       Sprungtore in DISPLAY_MAP integriert     ***/
/*** 25.02.08: cockpit.c      Jumpgates in DISPLAY_BP integriert       ***/
/*** 25.02.08: dw_def.h       SPACE und MINE vertauscht                ***/
/*** 26.02.08: ueberall       Symbol fuer DOCKING entfernt             ***/
/*** 27.02.08: ueberall       struct scout eingefuehrt                 ***/
/*** 28.02.08: dwmain.c       Fehler in void build (==CLAIMED) und     ***/
/***                          Aenderung Abfrage Zerstoerung des Pl.    ***/
/*************************************************************************/
/*** 28.02.08: Version 1.4.1                                           ***/
/*************************************************************************/
/*** 29.02.08: torpedo.c      Neue Scout-Behandlung und hit ==> thit   ***/
/*** 05.03.08: defeat.c       Fehler bei Homeworld-Warnung Foed. gefixt***/
/*** 05.03.08: dwmain.c       Fehler bei Dock an B oder HW: CLOUD wurde***/
/***                          migenommen bis Ausdocken => CLOUD "wan-  ***/
/***                          derte" mit. a_alt auf SPACE bei Dock.    ***/
/*** 11.03.08: ships.h        Tippfehler Warlard korrigiert            ***/
/*** 11.03.08: defeat.c       Scout wird jetzt beschossen, wenn Tarnung***/
/***                          weg.                                     ***/
/*** 12.03.08: dwmain.c       IN s mit Prozentangaben (iproz neu)      ***/
/*** 12.03.08: ueberall       Missile neu (missile, LM, KM, IN m usw.) ***/
/*************************************************************************/
/*** 12.03.08: Version 1.4.2                                           ***/
/*************************************************************************/
/*** 12.03.08: dwmain.c       Kosmetik IN m                            ***/
/*** 13.03.08: defeat.c       Fehler bei Abfrage Homeworld<->Scout und ***/
/***                          bei Homeworld<->Missile beseitigt        ***/
/*** 13.03.08: dw_def.h       HOMEWORLD und N_HOMEWORLD weg            ***/
/*** 13.03.08: ueberall       Umstellung auf struct homeworld          ***/
/*** 15.03.08: ueberall       HOOD_ON/OFF, RLEE_ON/OFF, BEAM_ON/OFF,   ***/
/***                          RAMM_ON/OFF durch ON/OFF ersetzt,        ***/
/***                          DEFEAT_ON/OFF weg. N_/EXISTING durch     ***/
/***                          NOT_/ACTIVE ersetzt                     ***/
/*** 21.03.08: ueberall       l_hit,l_capture weg                      ***/
/*** 23.03.08: dwmain.c       Homeworld/Basis: Schaeden > 90%: Dock u. ***/
/***                          Undock unmoeglich; > 50%: diverse Infos  ***/
/***                          bei HW nicht abrufbar (SD RE,BP,M)       ***/
/*** 23.03.08: ueberall       STOPPED durch OFF und MOVING durch ON    ***/
/***                          ersetzt                                  ***/
/*** 23.03.08: dwmain.c       SD ra unmoeglich bei Dock an beschaedig. ***/
/***                          Basis oder Heimatwelt                    ***/
/*** 24.03.08: ueberall       SH_DOWN/UP durch ON/OFF ersetzt          ***/
/***                          N/JUMPING durch OFF/ON ersetzt           ***/
/*** 26.03.08: ueberall       l_defeat weg                             ***/
/*** 26.03.08  ueberall       Umstellung auf struct ship               ***/
/*** 26.03.08  ueberall       condition auch fuer Heimatwelten         ***/
/*** 27.03.08: cockpit.c      Neue HW-Darstellung mit condition        ***/
/*** 28.03.08: cockpit.c      Dockbild Basis an HW angepasst           ***/
/*** 29.03.08: dwmain.c       Symbol MISSILE in SY                     ***/
/*** 29.03.08: cockpit.c      Dockbild HW um Scout und Missile erweit. ***/
/*** 29.03.08: init_gal.c     ship.dock=HW_DOCKED als Voreinstellung   ***/
/*** 30.03.08: init_gal.c     Umgebung von HW: keine *,=,' '           ***/
/*************************************************************************/
/*** 30.03.08: Version 1.4.3                                           ***/
/*************************************************************************/
/*** 31.03.08: ueberall       Format %4.1f durch %06.1f ersetzt        ***/
/*** 31.03.08: phaser.c/torpedo.c 9 durch SHIP_RANGE ersetzt           ***/
/*** 01.04.08: phaser.c       Radarlee bei PH c ?                      ***/
/*** 01.04.08: torpedo.c      Radarlee bei TO c ?                      ***/
/*** 02.04.08: phaser.c       BLACK_HOLE getroffen: Meldung und Ende   ***/
/*** 02.04.08: torpedo.c      BLACK_HOLE getroffen: Meldung und Ende   ***/
/*** 03.04.08: phaser.c       Fehler aneu bei CLOUD-Abfrage beseitigt  ***/
/*** 03.04.08: ueberall       SV-Flag eingebaut                        ***/
/*** 03.04.08: move.c         Fehler: return bei missile entfernt      ***/
/*** 05.04.08: romulan.c      Andere Seed fuer ran bei shmid=0         ***/
/*** 06.04.08: ueberall       Missile path eingefuehrt                 ***/
/*************************************************************************/
/*** 07.04.08: Version 1.4.4                                           ***/
/*************************************************************************/
/*** 04.05.08: defeat.c       Fehler bei score scout/missile beseitigt ***/
/*** 05.05.08: scout.c        SCOUT_DELAY neu; usleep verschoben       ***/
/*** 05.05.08: missile.c      MISSILE_DELAY neu                        ***/
/*** 14.10.08: *.c            Anpassungen fuer Ubuntu 8.04 (includes)  ***/
/*** 14.10.08: cockpit.c      Korrektur strb1/2 Stringlaenge 22->23    ***/
/***                          wegen Fehler bei SD bp-Anzeige           ***/
/*** 16.10.08: dwmain.c       HE-Text LM um Path ergaenzt              ***/
/*** 23.10.08: ueberall       Anpassungen -Wall bei gcc (Ubuntu 8.04)  ***/
/*** 23.10.08: cockpit.c      str2 auf 12 (Fehler bei Anzeige MP on)   ***/
/*** 23.10.08: dwmain.c, ships.h          Name Buzzard -> Bloodhunt    ***/
/***                                      und bludgeon -> buzzard      ***/
/*** 23.10.08: dwmain.c       Einige Hilfstexte geaendert              ***/
/*** 25.10.08: dw_def.h       Neue Symbole fuer Scouts, Robots, Miss.  ***/
/*** 26.10.08: cockpit.c      Radarbild: Leerzeichen bei Werten ausser-***/
/***                          halb des Spielfeldes                     ***/
/*** 27.10.08: cockpit.c      Neues HW-Dockbild; Compiler-Warnungen    ***/
/***                          bzgl. jcnt und k bearbeitet              ***/
/*** 28.10.08: ueberall       Grundfunktionalitaeten Robotschiffe mit  ***/
/***                          Mission P und M                          ***/ 
/*************************************************************************/
/*** 28.10.08: Version 1.5.0                                           ***/
/*************************************************************************/
/*** 18.10.09: dwmain.c       Fehler QU bei Dock an Basis: leeres Feld ***/
/***                          anstelle der Basis; beseitigt, jetzt wie ***/
/***                          Homeworld                                ***/
/*** 18.10.09: dwmain.c       Fehler QU bei Dock an Basis: Schiffs-    ***/
/***                          symbol blieb an Docking-Bay              ***/
/*** 19.10.09: dwmain.c       SD RA: Fehler bei Dock an Basis besei-   ***/
/***                          tigt. Kommando wurde nicht ausgefuehrt   ***/
/*** 20.10.09: dwmain.c       int retval wg. Warning bei gcc4.3.3 bei  ***/
/***                          Funktionsaufrufen, z.B. fscanf,system    ***/
/*** 20.10.09: dwmain.c       Meldung bei fehlender oder inkorrekter   ***/
/***           dw_def.h       user.dat; Algorithmus Erstabfrage user.  ***/
/***                          dat geaendert                            ***/
/*** 21.10.09: dwmain.c       Status HW_DOCKED bei QU ausserhalb HW,   ***/
/***                          ansonsten nicht in HW zu sehen           ***/
/*** 21.10.09: cockpit.c      Anpassungen wg. buffer overflow mit gcc  ***/
/***                          4.3.3: str1 auf Laenge 24,etliche strcpy ***/
/***                          und strcat durch strncpy,strncat ersetzt ***/
/***                          sowie Radarbildanzeige optimiert         ***/
/*** 24.10.09: cockpit.c      Anpassungen wg. buffer overflow: strs4   ***/
/***                          und Strings fuer SD BP                   ***/
/*** 26.10.09: dwmain.c       DO und HW nicht bei MP==ON               ***/
/*** 26.10.09: cockpit.c      Bei HW-Docking Player-Anzeige fuer beide ***/
/***                          Seiten                                   ***/
/*** 31.10.09: dwmain.c       HW-Dock nur bei gestopptem Warpantrieb   ***/
/*** 04.11.09: dw_def.h       CMD_LENGTH, MSG_LENGTH angepasst         ***/
/*** 04.11.09: dwmain.c       CMD_LENGTH bei Kommandostringerstellung  ***/
/***                          und bei send_msg, com wg. Bufferoverfl.  ***/
/*** 08.11.09: dwmain.c       Diverse Stringlaengen bei Textausgaben   ***/
/***                          korrigiert (Buffer Overflow)             ***/
/*** 21.11.09: defeat.c       p_party, zaehler waren nicht deklariert, ***/
/***                          str1 auf MSG_LENGTH, f_zaehler war de-   ***/
/***                          klariert, aber nicht genutzt             ***/
/*** 22.11.09: romulan.c      str1 auf MSG_LENGTH gesetzt              ***/
/*** 22.11.09: dwmain.c       In info() ungenutzte Var. anz_rob entf.  ***/
/*** 22.11.09: dwmain.c       build() wg. Warning (-Wall) zu nrb umge- ***/
/***                          gebaut; kleine Textanpassung             ***/
/*** 26.11.09: ueberall       Anpassungen -Wall bei Ubuntu 9.04        ***/
/*** 27.11.09: cockpit.c      msgw-Window: Strings mit Leerzeichen     ***/
/***                          aufgefuellt                              ***/
/*** 28.11.09: dw_def.h       SIDEX,SIDEY,TERM_WIDTH/HEIGHT neu        ***/
/*** 28.11.09: cockpit.c      SIDEX/SIDEY bei Window-Def. neu          ***/
/*** 28.11.09: dwmain.c       Setzen von Minen u. Satelliten: Position ***/
/***                          muss im Spielfeld sein                   ***/
/*** 28.11.09: dw_def.h       MAX_NAMES neu                            ***/
/*** 28.11.09: dwmain.c       MAX_NAMES fuer Anzahl der Namen in der   ***/
/***                          Datei user.dat                           ***/
/*** 28.11.09: cockpit.c      Scan-Window: nicht gefuellte scan_row    ***/
/***                          mit Leerzeichen aufgefuellt              ***/
/*** 29.11.09: ueberall       Treffer Scout in (t)hit umgeschrieben,   ***/
/***                          nicht bei missile.c                      ***/
/*** 29.11.09: robot.c        Missile-Relikte korrigiert               ***/
/*** 29.11.09: missile.c      Treffer auf Robotship eingebaut (fehlte) ***/
/*** 03.12.09: torpedo.c      Checks (Dam,Anz.T.,Hood) -> dwmain.c     ***/
/*** 03.12.09: phaser.c       Einige Abfragen verschoben               ***/
/*** 04.12.09: torpedo.c      Delay bei Schaden jetzt schadensabh.     ***/
/*** 05.12.09: dw_def.h       RELOAD_TT_TIME neu                       ***/
/*** 07.12.09: init_gal.c     Vorbesetzungen ship vervollstaendigt     ***/
/*** 12.12.09: dw_shm.h       tt_lock_time bei struct ship neu         ***/
/*** 12.12.09: torpedo.c      Hauptschleife ueberarbeitet; Meldungen   ***/
/***                          geaendert; TT-Reload komplett neu mit    ***/
/***                          Zeitstempel und Nanosekundenzeit         ***/
/*** 12.12.09: cockpit.c      TT-Reload jetzt hier mit Zeitstempel     ***/
/*** 13.12.09: init_gal.c     seed() neu; ran() mit seed ueberarbeitet ***/
/*** 13.12.09: torpedo.c      seed() neu; ran() mit seed ueberarbeitet ***/
/*** 13.12.09: ueberall       used_ttubes entfernt                     ***/ 
/*** 13.12.09: dwmain.c       free_ttubes neu                          ***/
/*** 16.12.09: dwmain.c       Bufferoverflow in identify beseitigt     ***/
/*** 18.12.09: dwmain.c       Bufferoverflow in show_score beseitigt   ***/
/*** 19.12.09: dwmain.c       Bufferoverflow bei IN h beseitigt        ***/
/*** 20.12.09: torpedo.c      Bufferoverflow Textausgabe beseitigt     ***/
/*** 20.12.09: robot.c        Fehler bei Symboldarstellung: es wurde   ***/
/***                          immer F_ROBOTSHIP genommen. Das Symbol   ***/
/***                          wird jetzt in robsym abgelegt. Ferner    ***/
/***                          gab es weitere explizite F_ROBOTSHIP-Zu- ***/
/***                          weisungen, die ebenfalls korrig. wurden  ***/
/*** 20.12.09: cockpit.c      msgw-Window: Strings mit Leerzeichen     ***/
/***                          auffuellen: aber nur falls notwendig!    ***/
/***                          Ansonsten leeres msgw-Window bei HW-Dock ***/
/***                          wenn ohne Optimierung compiliert wird    ***/
/***                          (gcc 4.3.3)                              ***/
/*** 20.12.09: init_gal.c     in ran2 i,j > 0 wg. floating p. except.  ***/
/*** 24.12.09: torpedo.c      In thit Abfrage auf EXPLOSION eingebaut; ***/
/***                          sonst konnten zwei schnelle Torpedo-     ***/
/***                          treffer auf CLOUD hintereinander zum     ***/
/***                          zum Verschwinden der CLOUD fuehren.      ***/
/***                          Trifft ein Torpedo auf EXPLOSION, wird   ***/
/***                          der Torpedo ohne Explosion zerstoert     ***/
/*** 25.12.09: torpedo.c      In thit umgestellt auf init_expl()       ***/
/*** 25.12.09: rtorpedo.c     Treffer auf EXPLOSION anders und init_   ***/
/***                          expl in thit() (siehe auch torpedo.c)    ***/
/*** 26.12.09: cockpit.c      Index-Ueberschreitung scan_row[] bei     ***/
/***                          Explosion beseitigt. Fuehrte beim Comp.  ***/
/***                          ohne Optimierung zum Absturz von cockpit ***/
/*** 26.12.09: explosion.c    incident() auf ran() mit seed() umgest.  ***/
/*** 26.12.09: dw_def.h       Symbol Romulaner jetzt R                 ***/
/***                          Symbol Fed. Robotship jetzt !            ***/
/***                          Symbol EXPLOSION jetzt ?                 ***/
/*** 27.12.09: ships.h        Romulaner: Symbol geaendert (R)          ***/
/*** 27.12.09: dwmain.c       HW: Select  : C  M  T  P: Torpedo weg    ***/
/*** 30.12.09: phaser.c       seed() neu; ran() mit seed ueberarbeitet ***/
/*** 31.12.09: move.c         seed() neu; ran() mit seed ueberarbeitet ***/
/*** 31.12.09: missile.c      Umgestellt auf init_expl()               ***/
/*** 31.12.09: missile.c      Treffer auf Schw.L. ergab SPACE; korr.   ***/
/*** 31.12.09: missile.c      seed() neu; ran() mit seed ueberarbeitet ***/
/*** 01.01.10: dw_shm.h       Laenge recon_str auf SCANX gesetzt       ***/
/*** 01.01.10: defeat.c       Format sprintf fuer recon_str geaendert  ***/
/*** 02.01.10: defeat.c       Anzeige Forts, die DESTROYED, aber im    ***/
/***                          Spielfeld sind                           ***/
/*** 02.01.10: phaser.c       Umstellung auf init_expl()               ***/
/*** 02.01.10: dwmain.c       Umstellung auf init_expl()               ***/
/*** 02.01.09: robot.c        In wegfrei() auf init_expl() umgestellt  ***/
/*** 02.01.10: scout.c        seed() neu; ran() mit seed ueberarbeitet ***/
/*** 03.01.10: cockpit.c      QUIT/EXIT: DESTROYED zuerst, Aenderun-   ***/
/***                          gen im Ende-Text                         ***/
/*** 03.01.10: defeat.c       Survey-Array Sat: Nur wenn nicht NEUTRAL ***/
/***                          und Position nicht -99/-99               ***/
/*** 03.01.10: cockpit.c      String bei Area-Anzeige verlaengert      ***/
/*** 03.10.10: defeat.c       Sat-Umgebung checken: Bedingungen erwei- ***/
/***                          tert: nur wenn Sat nicht NEUTRAL und im  ***/
/***                          Spielfeld                                ***/
/*** 03.01.09: dw_shm_inst.c  Segfault bei Eingabe n beseitigt: else   ***/
/***                          fragt auf != n ab, dann exit(0); while   ***/
/***                          getchar entfernt                         ***/
/*** 08.01.10: dwmain.c       Fehler in build_jumpgate() beseitigt;    ***/
/***                          statt jumpgate.status war fort.status ge-***/
/***                          setzt; fuehrte zu Forts mit permanentem  ***/
/***                          Status CLAIMED nach Jumpgatebau          ***/
/*** 08.01.10: ueberall       FORT_BUILD_RESS und JUMPGATE_BUILD_RESS  ***/
/***                          neu eingefuehrt                          ***/
/*** 09.01.10: ueberall       BLIND_- und MINE_COL_ENERGY neu          ***/
/*** 09.01.10: move.c         Umstellung auf init_expl()               ***/
/*** 10.01.10: move.c         init_expl weg bei Kollision mit MINE u.  ***/
/***                          BLIND wegen Flug durch Explosion         ***/
/*** 10.01.10: dwmain.c       dito bei impuls()                        ***/
/*** 10.01.10: dwmain.c       seed() neu; ran() mit seed ueberarbeitet ***/
/*** 10.01.10: defeat.c       seed() neu; ran() mit seed ueberarbeitet ***/
/*** 13.01.10: move.c         init_expl weg bei Kollision mit MISSILE  ***/
/*** 13.01.10: dwmain.c       init_expl weg bei Kollision mit MISSILE  ***/
/*** 13.01.10: ueberall       MISSILE_COL_ENERGY neu                   ***/
/*** 13.01.10: defeat.c       Zwei Formatfehler bei printf beseitigt   ***/
/*** 14.01.10: cockpit.c      Letztes Zeichen in Radarbild auf ' ' ge- ***/
/***                          setzt, weil dort Explosionsreste auf-    ***/
/***                          tauchten. Besser: line neu befuellen     ***/
/*** 16.01.10: defeat.c       usleep bei Romulaner-Expl. auf EXPL_TIME ***/
/*** 17.01.10: defeat.c       Kein Eliminieren des Rom. mehr; das er-  ***/
/***                          folgt durch den Rom. selbst              ***/
/*** 17.01.10: dw_def.h       ROM_DELAY neu                            ***/
/*** 17.01.10: romulan.c      ROM_DELAY neu                            ***/
/*** 23.01.10: defeat.c       Scoring bei Rom.-Zerst. fehlte           ***/
/*** 23.01.10: torpedo.c      Keine Explosion des Romulaners mehr;     ***/
/***                          erfolgt durch den Rom. selbst            ***/
/*** 23.01.10: phaser.c       Keine Explosion des Romulaners mehr; das ***/
/***                          macht R. selbst. Scoring bei R.-Vern.    ***/
/***                          wie bei torpedo.c                        ***/
/*** 23.01.10: move.c         Keine Explosion des Romulaners mehr; das ***/
/***                          macht R. selbst. Scoring bei R.-Vern.    ***/
/***                          wie bei torpedo.c                        ***/
/*** 23.01.10: missile.c      Keine Explosion des Romulaners mehr; das ***/
/***                          macht R. selbst. Scoring bei R.-Vern.    ***/
/***                          wie bei torpedo.c                        ***/
/*** 23.01.10: dwmain.c       dito                                     ***/
/*** 23.01.10: rtorpedo.c     dito                                     ***/
/*** 23.01.10: init_gal.c     base[].sh=ON bei Basen im Spiel; war auf ***/
/***                          OFF voreingestellt gesetzt               ***/
/*** 23.01.10: cockpit.c      Stringlaenge bei Area (-def?-) korr.     ***/
/*** 23.01.10: rtorpedo.c     seed() neu; ran() mit seed ueberarbeitet ***/
/*** 23.01.10: dw_def.h       RTORP_DELAY neu                          ***/
/*** 23.01.10: rtorpedo.c     RTORP_DELAY neu                          ***/
/*** 24.01.10: dw_def.h       SCOUT_SCAN_INTERVAL neu                  ***/
/*** 24.01.10: scout.c        SCOUT_SCAN_INTERVAL neu                  ***/
/*** 24.01.10: scout.c        Meldung bei Zerstoerung mit Position     ***/
/***                          und Ausgabe ueber send_msg; Explosion    ***/
/*** 24.01.10: torpedo.c      Keine Explosion des Scouts mehr; erfolgt ***/
/***                          ausschliesslich durch den Scout selbst;  ***/
/***                          doppeltes Scoring bei Scout-Zerstoerung  ***/
/***                          beseitigt                                ***/
/*** 24.01.10: phaser.c       dito                                     ***/
/*** 24.01.10: move.c         dito                                     ***/
/*** 24.01.10: missile.c      dito (dort gab es kein doppeltes Scor.)  ***/
/*** 24.01.10: rtorpedo.c     dito                                     ***/
/*** 24.01.10: dwmain.c       dito                                     ***/
/*** 24.01.10: robot.c        Selbstueberwachung und Zerstoerung Rob.  ***/
/***                          erweitert, neuen Text, Abfrage status,   ***/
/***                          EXPLOSION in eliminate_robotship()       ***/
/*** 24.01.10: dwmain.c       Robotship: Elimination und Explosion in  ***/
/***                          robot.c verlagert                        ***/
/*** 24.01.10: torpedo.c      dito                                     ***/
/*** 24.01.10: rtorpedo.c     dito                                     ***/
/*** 24.01.10: move.c         dito                                     ***/
/*** 24.01.10: defeat.c       dito                                     ***/
/*** 24.01.10: phaser.c       dito                                     ***/
/*** 24.01.10: missile.c      dito                                     ***/
/*** 24.01.10: romulan.c      dito                                     ***/
/*** 25.01.10: missile.c      MISSILE_DELAY jetzt ueberall             ***/
/*** 26.01.10: missile.c      Explosion bei Ende mit DESTROYED       ***/
/*** 26.01.10: defeat.c       Fehler bei Missile-Meldung korrigiert    ***/
/*** 26.01.10: dwmain.c       Keine Explosion des Miss. mehr; erfolgt  ***/
/***                          ausschliesslich durch Missile selbst;    ***/
/***                          Meldungen "under fire" und "destroyed"   ***/
/***                          entfernt                                 ***/
/*** 26.01.10: torpedo.c      dito                                     ***/
/*** 26.01.10: rtorpedo.c     dito                                     ***/
/*** 26.01.10: move.c         dito                                     ***/
/*** 26.01.10: phaser.c       dito                                     ***/
/*** 27.01.10: missile.c      Missile trifft Missile neu eingefuehrt   ***/
/*** 30.01.10: cockpit.c      Ausgabe % von Area geaendert (>10,<10)   ***/
/*** 05.02.10: dwmain.c       ASCII-Art-Infos (void info...) entfernt  ***/
/*** 06.02.10: dwmain.c       Neue Schiffsinfo infoall() bei Intro     ***/
/*** 06.02.10: dwmain.c       Backspace-Nr. von 8 auf 127 (Charset??)  ***/
/*** 06.02.10: dwmain.c       Sprungtornutzung nur bei MP = OFF        ***/
/*** 13.02.10: dwmain.c       Kommandostring bei CMD_LENGTH-1 beenden  ***/
/***                          wegen Stack-Overflow (bei -O2)           ***/
/*** 20.02.10: dw_def.h       DEF_DELAY neu                            ***/
/*** 20.02.10: defeat.c       DEF_DELAY neu, usleep statt pause        ***/
/*** 21.02.10: dw_def.h       ROM_DELAY von 6600000 auf 4000000        ***/
/*** 27.02.10: defeat.c       etliche printf-Ausgaben entfernt         ***/
/*** 13.03.10: dw_def.h       ROBOTSHIP_FUEL von 200 auf 300 gesetzt   ***/
/*** 13.03.10  robot.c        Feuer nur noch, wenn wegfrei()==0 (wurde ***/
/***                          zuvor nur bei RADARLEE==OFF abgefragt)   ***/
/*** 13.03.10  robot.c        In wegfrei() keine Zerstoerung von Minen ***/
/***                          und Blindgaengern mehr.                  ***/
/*** 13.03.10  robot.c        Minenraeumung umfasst jetzt auch BLIND   ***/
/*** 13.03.10  robot.c        Feuer auf feindliche HW                  ***/
/*** 13.03.10: robot.c        Status WAITING neu (fuer fuel <= 0)      ***/
/*** 13.03.10: dw_def.h       WAITING neu (fuer Robotship fuel <= 0)   ***/
/*** 20.03.10: robot.c        Feuer auf feindliche Robotschiffe        ***/
/*** 21.03.10: dw_def.h       MIN_ROBOTSHIP_ENERGY neu                 ***/
/*** 21.03.10: robot.c        Status->WAITING bei Energie < MIN_ROBOT- ***/
/***                          SHIP_ENERGY                              ***/
/*** 21.03.10: cockpit.c      Status WAITING bei Robotschiffanzeige    ***/
/*** 21.03.10: dwmain.c       Kommando TP in TE umbenannt und auf      ***/
/***                          Robotschiffe erweitert                   ***/
/*** 21.03.10: dwmain.c       IN R um WAITING (W) erweitert            ***/
/*** 21.03.10: dwmain.c       Kommando TF neu (void transport_fuel())  ***/
/*** 22.03.10: dwmain.c       In transport_fuel() Abfrage auf Treib-   ***/
/***                          stoffvorrat des Schiffs eingefuehrt      ***/
/*** 22.03.10: dwmain.c       IN r n mit n=0..9 fuer Robotschiffe      ***/
/***                          und HE IN aktualisiert                   ***/
/*** 27.03.10: dw_def.h       ROBOTSHIP_BUILD_TIME neu                 ***/
/*** 27.03.10: dw_def.h       CLAIMED von 2 auf 5 gesetzt              ***/
/*** 27.03.10: dwmain.c       Kommando BR (void build_robotship()) neu ***/
/***                          inkl. Anpassungen bei IN r               ***/
/*** 27.03.10: cockpit.c      Status CLAIMED bei Robotschiffanzeige    ***/
/*** 28.03.10: dwmain.c       Kommando TT neu (void transport_torp())  ***/
/*** 02.04.10: cockpit.c      Kosmetik                                 ***/
/*** 03.04.10: torpedo.c      Zu lange Nachrichten gekuerzt            ***/
/*** 03.04.10: rtorpedo.c     Zu lange Nachrichten gekuerzt            ***/
/*** 03.04.10: robot.c        Missionsabbruch wenn Torp. verschossen   ***/
/*** 05.04.10: Ueberall       Fehler in for-Schleife in hit() entfernt ***/
/*** 05.04.10: Ueberall       eliminate_hw() um Robotschiffe ergaenzt  ***/
/*** 09.04.10: dwmain.c       IN m fuer Path korrigiert                ***/
/*** 10.04.10: dwmain.c       IN h um Infos ueber feindl. HW erweitert ***/
/*** 11.04.10: missile.c      In smhit() Fall HOOD != Schiff eingebaut ***/
/*** 11.04.10: dw_shm.h       mpath und imp neu                        ***/
/*** 11.04.10: dw_def.h       MAX_MPATHS neu                           ***/
/*** 11.04.10: init_gal.c     m_p->mpath vorbesetzt                    ***/
/*** 15.04.10: missile.c      Umstellung auf m_p->mpath[MAX_MPATHS]    ***/
/*** 15.04.10: dwmain.c       Umstellung auf m_p->mpath[MAX_MPATHS]    ***/
/*** 15.04.10: dwmain.c       neue Kommandos CP und SP. HE angepasst   ***/
/*** 16.04.10: dwmain.c       Anpassungen IN m                         ***/
/*** 16.04.10: robot.c        Mission J hinzugefuegt                   ***/
/*** 16.04.10: dwmain.c       Anpassung LR fuer Mission J              ***/
/*** 17.04.10: dwmain.c       QU nicht in Reichweite feindl. Robotsch. ***/
/*** 18.04.10: dwmain.c       IN r n ueberarbeitet                     ***/
/*** 24.04.10: Ueberall       neue Konstantendefinitionen in dw_def.h  ***/
/*** 25.04.10: Ueberall       IMAX,IMIN,IABS ueberall in () gesetzt    ***/ 
/*************************************************************************/ 
/*** 01.05.10: Version 1.5.1                                           ***/
/*************************************************************************/
/*** 03.05.10: cockpit.c      Statusanzeige "under construction" RS    ***/
/*** 03.05.10: dw_def.h       MIN_HW_DIFF neu, auf 80 gesetzt          ***/
/*** 03.05.10: init_gal.c     MIN_HW_DIFF neu                          ***/
/*** 09.05.10: robot.c        fire(): falls BLIND (=TORPEDO) dann nach ***/
/***                          Delay nochmal abfragen, um von Torpedo   ***/
/***                          zu unterscheiden                         ***/
/*** 09.05.10: robot.c        Meldung HW-Dock nach Missionsende mit    ***/
/***                          Robotschiff-Nr.-Angabe                   ***/
/*** 13.05.10: dwmain.c       IN R ueberarbeitet                       ***/
/*** 15.05.10: dwmain.c       Kommando KR n neu                        ***/
/*** 22.05.10: dw_def.h       CANCELED neu                             ***/
/*** 22.05.10: dwmain.c       Kommando CR n neu                        ***/
/*** 22.05.10: robot.c        Status CANCELED (aus CR) eingebaut       ***/
/*************************************************************************/
/*** 06.06.10: Version 1.5.2                                           ***/
/*************************************************************************/
/*** 04.09.11: robot.c        Infotext bei Start geaendert             ***/
/*** 04.09.11: dwmain.c       Formatfehler Infoanzeige bei IN R korr.  ***/
/*** 08.09.11: cockpit.c      Korrekturen bei Anzeige SD RA und SD BP  ***/
/*** 03.10.11: dw_def.h       ROBOTSHIP_CAPTURE_DELAY neu              ***/
/*** 23.10.11: dw_def.h       ROBOTSHIP_CA_ENERGY neu                  ***/
/*** 23.10.11: robot.c, dwmain.c Mission C (Capture Planet) neu        ***/
/*** 23.10.11: dwmain.c       IN R n um Anzeige Damages erweitert      ***/
/*** 05.11.11: dwmain.c, robot.c Fall beruecksichtigt, dass Planet     ***/
/***                          bei Capture zerstoert wird.              ***/
/*** 15.01.12: dwmain.c       HE LR: Missionsliste aktualisiert: PMCJ  ***/
/*** 21.01.12: cockpit.c      Korrektur bei Anzeige SD MA              ***/
/*************************************************************************/
/*** Nach 1.5.2                                                        ***/
/*** 15.05.15: ueberall       iretval, retval u.a. entfernt wegen      ***/
/***                          Compilerwarning gcc 4.8.2                ***/
/*** 10.10.19: user.dat in /tmp verlegt und wird bei Fehlen angelegt   ***/ 
/*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <termio.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h> 
#include <time.h> 

/****************************************************************************/
/***                   Globale Definitionen                               ***/
/****************************************************************************/

/*****************************************************************************/
/*** Include fuer DEKKWAR                                                  ***/
/*****************************************************************************/
#include "mathfkt.h"
#include "dw_def.h"
#include "dw_shm.h"
#include "ships.h"

/************************************************************************/
/***        Globale Variable                                          ***/
/************************************************************************/
static int vflag;
static char a_alt,xcmd[CMD_LENGTH],ycmd[CMD_LENGTH];

/*************************************************************************/
/***               Externe Funktionen                                  ***/
/*************************************************************************/
extern key_t qgetkey();
extern void hit();
extern int toupper();
extern void sort_msg();
extern void send_msg();
extern void dw_logo();
extern int det_target();
extern float ran();
extern int seed();
extern void docking();
extern void hw_docking();
extern void shields_up();
extern void shields_down();
extern void set_mine();
extern void set_sat();
extern void send();
extern void sort_send_msg();
extern void show_damages();
extern void show_score();
extern void identify();
extern void info();
extern void build();
extern void capture();
extern int find_nr_pl();
extern int find_nr_sat();
extern int find_nr_fort();
extern int find_nr_jumpgate();
extern int find_nr_base();
extern int find_nr_robotship();
extern void eliminate_pl();
extern void eliminate_hw();
extern void eliminate_base();
extern void eliminate_fort();
extern void eliminate_jumpgate();
extern void eliminate_sat();
extern void transfer();
extern void repair();
extern int incident();
extern void symbol();
extern void build_fort();
extern void build_jumpgate();
extern void transport_energy();
extern void transport_fuel();
extern void transport_torp();
extern void learnX();
extern void learnY();
extern void impuls();
extern void gauss2();
extern int get_score();
extern void sig_hnd();
extern void sig_hnd_ctrlc();
extern void hood();
extern int quit_control();
extern long clock();
extern void infoall();
extern void select_ship();
extern void start_ship();
extern void restart_procs();
extern void init_expl();
extern int behind();
extern void jump();
extern int iproz();
extern void build_robotship();
extern void save_path();
extern void clear_path();
extern void kill_robotship();
extern void call_back_robotship();

/**************************************************************************/
/**************************************************************************/
/***                              Beginn main                           ***/
/**************************************************************************/
/**************************************************************************/
int main()

{
/**************************************************************************/
/***                              Lokale Deklarationen                  ***/
/**************************************************************************/
int pid1,pid2,pid4,pid5,ppid1,ppid2,ppid4,ppid5,pid6,pid7,i,ii;
int i_rank,i_user,ia,isp,old_score,i_sc[MAX_NAMES],sc,recog,nrb,nrob,ip;
float random;
char cc,cp[2],vflag_string[4],com[CMD_LENGTH],s_line[20][80];
char cmdr_name[21],i_nam[MAX_NAMES][10];
char scout_cm[10],scout_x[10],scout_y[10],scout_msg[MSG_LENGTH];
char print_str[MSG_LENGTH];
char missile_cm[4],c0[4],c1[4],c2[4],c3[4],c4[4],robotship_cm[4];
int x_scout,y_scout,x_missile,y_missile,xs_missile,ys_missile;
int x_robotship,y_robotship;
int werr, free_ttubes;
FILE *fp,*fopen();

/**************************************************************************/
/***                   Eingangslogo und Startabfrage                    ***/
/**************************************************************************/
signal(SIGINT,sig_hnd_ctrlc);

clock();
dw_logo();
printf("         Do you want to play DEKKWAR (y/n)?");
cc=getchar();
printf("\n");
if (cc != 'y' && cc != 'Y')
   {
   printf("\n");
   system("tput clear");
   exit(0);
   }
while (getchar() != '\n');

/**************************************************************************/
/***                             Startwerte setzen                      ***/
/**************************************************************************/

signal(SIGALRM,sig_hnd);

/**************************************************************************/
/***                     Shared Memory einbinden                        ***/
/**************************************************************************/

shmid = shmget(qgetkey(SHMEM),sizeof(DW_SHM),IPC_CREAT);
if ( (m_p = (DW_SHM *)shmat(shmid,(char *)0,0)) == (DW_SHM *) ERROR )
   {
   printf("Kann Shared Memory nicht einbinden!---> Abbruch!\n");
   exit(1); 
   }
if (shmid!=m_p->shm_id)
   {
   printf("Error: falsche ID");
   exit(1);
   }

/**************************************************************************/
/***                       Eingangsabfragen: Name + Schiff              ***/
/**************************************************************************/

/********/
/* Name */
/********/
/* ============= */
system ("tput clear");
printf("\n\n\n                           DEKKWAR                         ");
printf(    "\n-----------------------------------------------------------");
printf(    "\n            List of commanders and their score             \n\n");
fp=fopen("/tmp/dwuser.dat","r");
if (fp == NULLF)
   {
   printf("/tmp/dwuser.dat not found! Created /tmp/dwuser.dat\n");
   fp=fopen("/tmp/dwuser.dat","w+");
   }

while (!feof(fp))
  {
  fscanf(fp,"%s %d",cmdr_name,&sc);
  if (!feof(fp))
    {
    printf("  %-10s  %6d ",cmdr_name,sc);
    for (ii=1; ii < MAX_USER; ii++)
      {
      if (m_p->luser[ii]==ACTIVE && strcmp(m_p->name_of_cmdr[ii],cmdr_name)==0)
	 {
         printf("    in game: Rank %-s",m_p->rank_of_name[ii]);
         }
      }
    }
  printf("\n");
  }
fclose(fp);
printf(    "-----------------------------------------------------------");
/* ============= */
printf("\n\n\n");
printf(" Enter your name (max. 21 Ch.): ");
scanf("%s",cmdr_name);
while (getchar() != '\n');
old_score=get_score(cmdr_name);
if (old_score==(-1))
   {
   i_rank=COMMANDER;
   printf("\n\n Name %s not in database; you are a beginner!\n",cmdr_name);
   printf(" A beginner starts with 0 points, so your rank is\n");
   printf("                COMMANDER!\n");
   printf(" You cannot choose a cruiser or battleship until you scored\n");
   printf(" 15000 points (cruiser) or 30000 points (battleship) at least!\n\n");
   printf(" Enter RETURN to continue!\n");
   while (getchar()!='\n');   
   }
else
   {
   printf("\n\n %s identified!\n",cmdr_name);
   printf("\n\n %d points scored up today!\n",old_score);   
   if (old_score<CAPTAIN_SC) 
      {
      printf(" Rank: Commander, choose a destroyer!\n");
      i_rank=COMMANDER;
      }
   if (old_score>=CAPTAIN_SC && old_score<COMMODORE_SC)
      {
      i_rank=CAPTAIN;      	
      printf(" Rank: Captain, choose a destroyer or cruiser!\n");
      }
   if (old_score>=COMMODORE_SC && old_score<ADMIRAL_SC)
      {
      i_rank=COMMODORE;
      printf(" Rank: Commodore, choose a destroyer, cruiser, or battleship!\n");
      }
   if (old_score>=ADMIRAL_SC) 
      {
      printf(" Rank: Admiral, choose a destroyer, cruiser, or battleship!\n");      	
      i_rank=ADMIRAL;
      }
   printf(" Enter RETURN to continue!\n");
   while (getchar()!='\n');   
   }

system ("tput clear");

/***********/
/* Schiffe */
/***********/
strcpy(s_line[0],"    Federal Battleship ENTERPRISE (E) |");
strcat(s_line[0],"    Imperial Battleship MANTA      (M)");
if (m_p->luser[5] == ACTIVE) s_line[0][2]='!';
if (m_p->l_exist[5] == DESTROYED) s_line[0][2]='+'; 
if (m_p->luser[9] == ACTIVE) s_line[0][41]='!';
if (m_p->l_exist[9] == DESTROYED) s_line[0][41]='+';
strcpy(s_line[1],"    Federal Battleship LEXINGTON  (L) |");
strcat(s_line[1],"    Imperial Battleship PYTHON     (P)");
if (m_p->luser[6] == ACTIVE) s_line[1][2]='!';
if (m_p->l_exist[6] == DESTROYED) s_line[1][2]='+';
if (m_p->luser[10] == ACTIVE) s_line[1][41]='!';
if (m_p->l_exist[10] == DESTROYED) s_line[1][41]='+';
strcpy(s_line[2],"    Federal Battleship YORKTOWN   (Y) |");
strcat(s_line[2],"    Imperial Battleship JACKAL     (J)");
if (m_p->luser[7] == ACTIVE) s_line[2][2]='!';
if (m_p->l_exist[7] == DESTROYED) s_line[2][2]='+';
if (m_p->luser[11] == ACTIVE) s_line[2][41]='!';
if (m_p->l_exist[11] == DESTROYED) s_line[2][41]='+';
strcpy(s_line[3],"    Federal Battleship DEFIANT    (D) |");
strcat(s_line[3],"    Imperial Battleship VIPER      (V)");
if (m_p->luser[8] == ACTIVE) s_line[3][2]='!';
if (m_p->l_exist[8] == DESTROYED) s_line[3][2]='+';
if (m_p->luser[12] == ACTIVE) s_line[3][41]='!';
if (m_p->l_exist[12] == DESTROYED) s_line[3][41]='+';
strcpy(s_line[4],"    Federal Cruiser    California (C) |");
strcat(s_line[4],"    Imperial Cruiser    Falcon     (F)");
if (m_p->luser[13] == ACTIVE) s_line[4][2]='!';
if (m_p->l_exist[13] == DESTROYED) s_line[4][2]='+';
if (m_p->luser[19] == ACTIVE) s_line[4][41]='!';
if (m_p->l_exist[19] == DESTROYED) s_line[4][41]='+';
strcpy(s_line[5],"    Federal Cruiser    Oklahoma   (O) |");
strcat(s_line[5],"    Imperial Cruiser    Iscariot   (I)");
if (m_p->luser[14] == ACTIVE) s_line[5][2]='!';
if (m_p->l_exist[14] == DESTROYED) s_line[5][2]='+';
if (m_p->luser[20] == ACTIVE) s_line[5][41]='!';
if (m_p->l_exist[20] == DESTROYED) s_line[5][41]='+';
strcpy(s_line[6],"    Federal Cruiser    Kansas     (K) |");
strcat(s_line[6],"    Imperial Cruiser    Bloodhunt  (B)");
if (m_p->luser[15] == ACTIVE) s_line[6][2]='!';
if (m_p->l_exist[15] == DESTROYED) s_line[6][2]='+';
if (m_p->luser[21] == ACTIVE) s_line[6][41]='!';
if (m_p->l_exist[21] == DESTROYED) s_line[6][41]='+';
strcpy(s_line[7],"    Federal Cruiser    Tennessee  (T) |");
strcat(s_line[7],"    Imperial Cruiser    Warlord    (W)");
if (m_p->luser[16] == ACTIVE) s_line[7][2]='!';
if (m_p->l_exist[16] == DESTROYED) s_line[7][2]='+';
if (m_p->luser[22] == ACTIVE) s_line[7][41]='!';
if (m_p->l_exist[22] == DESTROYED) s_line[7][41]='+';
strcpy(s_line[8],"    Federal Cruiser    Nevada     (N) |");
strcat(s_line[8],"    Imperial Cruiser    Scorpion   (S)");
if (m_p->luser[17] == ACTIVE) s_line[8][2]='!';
if (m_p->l_exist[17] == DESTROYED) s_line[8][2]='+';
if (m_p->luser[23] == ACTIVE) s_line[8][41]='!'; 
if (m_p->l_exist[23] == DESTROYED) s_line[8][41]='+';
strcpy(s_line[9],"    Federal Cruiser    Utah       (U) |");
strcat(s_line[9],"    Imperial Cruiser    Quarrel    (Q)");
if (m_p->luser[18] == ACTIVE) s_line[9][2]='!';
if (m_p->l_exist[18] == DESTROYED) s_line[9][2]='+';
if (m_p->luser[24] == ACTIVE) s_line[9][41]='!';
if (m_p->l_exist[24] == DESTROYED) s_line[9][41]='+';
strcpy(s_line[10],"    Federal Destroyer  kittyhawk  (k) |");
strcat(s_line[10],"    Imperial Destroyer  vulture    (v)");
if (m_p->luser[25] == ACTIVE) s_line[10][2]='!';
if (m_p->l_exist[25] == DESTROYED) s_line[10][2]='+';
if (m_p->luser[35] == ACTIVE) s_line[10][41]='!';
if (m_p->l_exist[35] == DESTROYED) s_line[10][41]='+';
strcpy(s_line[11],"    Federal Destroyer  nostromo   (n) |");
strcat(s_line[11],"    Imperial Destroyer  wasp       (w)");
if (m_p->luser[26] == ACTIVE) s_line[11][2]='!';
if (m_p->l_exist[26] == DESTROYED) s_line[11][2]='+';
if (m_p->luser[36] == ACTIVE) s_line[11][41]='!';
if (m_p->l_exist[36] == DESTROYED) s_line[11][41]='+';
strcpy(s_line[12],"    Federal Destroyer  discovery  (d) |");
strcat(s_line[12],"    Imperial Destroyer  buzzard    (b)");
if (m_p->luser[27] == ACTIVE) s_line[12][2]='!';
if (m_p->l_exist[27] == DESTROYED) s_line[12][2]='+';
if (m_p->luser[37] == ACTIVE) s_line[12][41]='!';
if (m_p->l_exist[37] == DESTROYED) s_line[12][41]='+';
strcpy(s_line[13],"    Federal Destroyer  leonow     (l) |");
strcat(s_line[13],"    Imperial Destroyer  hornet     (h)");
if (m_p->luser[28] == ACTIVE) s_line[13][2]='!';
if (m_p->l_exist[28] == DESTROYED) s_line[13][2]='+';
if (m_p->luser[38] == ACTIVE) s_line[13][41]='!';
if (m_p->l_exist[38] == DESTROYED) s_line[13][41]='+';
strcpy(s_line[14],"    Federal Destroyer  endeavour  (e) |");
strcat(s_line[14],"    Imperial Destroyer  falchion   (f)");
if (m_p->luser[29] == ACTIVE) s_line[14][2]='!';
if (m_p->l_exist[29] == DESTROYED) s_line[14][2]='+';
if (m_p->luser[39] == ACTIVE) s_line[14][41]='!';
if (m_p->l_exist[39] == DESTROYED) s_line[14][41]='+';
strcpy(s_line[15],"    Federal Destroyer  ticonderoga(t) |");
strcat(s_line[15],"    Imperial Destroyer  shark      (s)");
if (m_p->luser[30] == ACTIVE) s_line[15][2]='!';
if (m_p->l_exist[30] == DESTROYED) s_line[15][2]='+';
if (m_p->luser[40] == ACTIVE) s_line[15][41]='!';
if (m_p->l_exist[40] == DESTROYED) s_line[15][41]='+';
strcpy(s_line[16],"    Federal Destroyer  columbia   (c) |");
strcat(s_line[16],"    Imperial Destroyer  mamba      (m)");
if (m_p->luser[31] == ACTIVE) s_line[16][2]='!';
if (m_p->l_exist[31] == DESTROYED) s_line[16][2]='+';
if (m_p->luser[41] == ACTIVE) s_line[16][41]='!';
if (m_p->l_exist[41] == DESTROYED) s_line[16][41]='+';
strcpy(s_line[17],"    Federal Destroyer  orion      (o) |");
strcat(s_line[17],"    Imperial Destroyer  panther    (p)");
if (m_p->luser[32] == ACTIVE) s_line[17][2]='!';
if (m_p->l_exist[32] == DESTROYED) s_line[17][2]='+';
if (m_p->luser[42] == ACTIVE) s_line[17][41]='!';
if (m_p->l_exist[42] == DESTROYED) s_line[17][41]='+';
strcpy(s_line[18],"    Federal Destroyer  avenger    (a) |");
strcat(s_line[18],"    Imperial Destroyer  intruder   (i)");
if (m_p->luser[33] == ACTIVE) s_line[18][2]='!';
if (m_p->l_exist[33] == DESTROYED) s_line[18][2]='+';
if (m_p->luser[43] == ACTIVE) s_line[18][41]='!';
if (m_p->l_exist[43] == DESTROYED) s_line[18][41]='+';
strcpy(s_line[19],"    Federal Destroyer  yorikke    (y) |");
strcat(s_line[19],"    Imperial Destroyer  jaguar     (j)");
if (m_p->luser[34] == ACTIVE) s_line[19][2]='!';
if (m_p->l_exist[34] == DESTROYED) s_line[19][2]='+';
if (m_p->luser[44] == ACTIVE) s_line[19][41]='!';
if (m_p->l_exist[44] == DESTROYED) s_line[19][41]='+';
again: system("tput clear"); 
printf("    Choose from one of the follwing ships ('+' = destroyed,  '!' = in use)\n");
printf("_______________FEDERATION_____________|________________EMPIRE_________________\n");
for (i=0;i<20;i++)
    {
    printf("%s\n",s_line[i]);
    }
printf("------------------------------------------------------------------------------\n");
printf("  Enter character (not marked by '!' or '+') or '?' for some information: ");
do {
   cc = getchar();
   while (getchar()!='\n');
   if (cc == '?') 
     {
     infoall();
     printf("Enter RETURN to continue\n");
     getchar();
     goto again;
     } 
   vflag=det_target(cc);
   if (vflag<5 || vflag>=MAX_USER) 
      {
      printf(" Wrong Ship; reenter :");
      }
   else
      {
      if (m_p->l_exist[vflag]==DESTROYED || m_p->luser[vflag]==ACTIVE
                      ||
          (old_score<15000 && vflag<25)
                      ||
          (old_score<30000 && vflag<13) )                      
         {
         printf(" Incompatible choice; reenter :");
         }
      }
   } while (vflag <5 || vflag >=MAX_USER || m_p->l_exist[vflag]==DESTROYED 
                 || 
            m_p->luser[vflag]==ACTIVE
                      ||
          (old_score<15000 && vflag<25)
                      ||
          (old_score<30000 && vflag<13) );
system ("tput clear");
/* Neu Feb. 1996: Logos fuer Empire/Fed. */
printf("\n");
if (party[vflag] == FEDERAL) 
  {
  printf("    You joined the FEDERAL side!\n\n\n");
  printf("             ______\n");
  printf("          _-' .   .`-_\n");
  printf("      |/ /  .. . '   .\\ \\|\n");
  printf("     |/ /            ..\\ \\|\n");
  printf("   \\|/ |: .   ._|_ .. . | \\|/\n");
  printf("    \\/ |   _|_ .| . .:  | \\/\n");
  printf("   \\ / |.   |  .  .    .| \\ /                 (Logo by Julian Visch)\n");
  printf("    \\||| .  . .  _|_   .|||/\n");
  printf("   \\__| \\  . :.  .|.  ./ |__/\n");
  printf("     __| \\_  .    .. _/ |__\n");
  printf("      __|  `-______-'  |__\n");
  printf("         -,____  ____,-\n");
  printf("           ---'  `---\n");
  printf("  UNITED FEDERATION OF PLANETS\n\n\n");
  }
if (party[vflag] == IMPERIAL) 
  {
  printf("        You joined the IMPERIAL side!\n\n");
  printf("     &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&& &&&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&& &&&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&& &&&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&   &&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&   &&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&   &&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&   &&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&     &&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&##     ##&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&####       ####&&&&&&&&&&&\n");
  printf("     &&&&&#####           #####&&&&&&&&\n");
  printf("     &&&&#### ###       ### ####&&&&&&&\n");
  printf("     &&&&&##    ###   ###    ##&&&&&&&&\n");
  printf("     &&&&&&      #######      &&&&&&&&&\n");
  printf("     &&&&&    &&#########&&       &&&&&\n");
  printf("     &&&&   &&&&&&&&&&&&&&&&&&     &&&&\n");
  printf("     &&&&  &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
  printf("     &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
  }

printf("*** Initializing %s %s ***\n",kind_of_ship[vflag],ships[vflag]);

alarm(2);
pause();
system("tput clear");
initscr();
/***************************************************************************/
/***                   Startwerte bei Shared-Memory-Variablen           ***/
/***************************************************************************/
sprintf(vflag_string,"%d",vflag);
m_p->luser[vflag]=ACTIVE;
m_p->l_exit[vflag]=NO_EXIT;
strncpy(m_p->cmd[vflag],"                   ",CMD_LENGTH-1);
strncpy(xcmd,"EMPTY          ",CMD_LENGTH-1);
strncpy(ycmd,"EMPTY          ",CMD_LENGTH-1);

/* Startposition */
/* In Homeworld wenn vorhanden */
if (m_p->homeworld[party[vflag]].status==ACTIVE)
   {
   m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0];
   m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1];
   m_p->ship[vflag].dock=HW_DOCKED;
   m_p->display_status[vflag]=DISPLAY_HW;
   sort_msg("                                 ");
   sort_msg("             Welcome             ");
   sort_msg("                                 ");
   sort_msg("   Initial position: Homeworld   ");
   sort_msg("         Status: docked          ");
   sort_msg("  Enter STart for an new mission ");
   sort_msg("                                 ");
   }
else
   {
   do {
      random=ran();
      m_p->ship[vflag].pos[0]=10+((int) (180.0*random));
      random=ran();
      m_p->ship[vflag].pos[1]=10+((int) (180.0*random));
      } while (m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]] != SPACE);
   m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
   m_p->ship[vflag].dock=NOT_DOCKED;
   m_p->display_status[vflag]=DISPLAY_RADAR;
   sort_msg("                                 ");
   sort_msg("                                 ");
   sort_msg("      HOMEWORLD DESTROYED!!      ");
   sort_msg("    Initial position: Random     ");
   sort_msg("                                 ");
   sort_msg("                                 ");
   sort_msg("                                 ");
   }

m_p->ship[vflag].torp=max_torp[vflag];
m_p->ship[vflag].sat=max_sat[vflag];
m_p->ship[vflag].mines=max_mines[vflag];
m_p->ship[vflag].fuel=max_fuel[vflag];
m_p->ship[vflag].hood=OFF;
m_p->ship[vflag].move=OFF;
m_p->ship[vflag].jump=OFF;
m_p->ship[vflag].phaser=OFF;
m_p->ship[vflag].build=OFF;
m_p->ship[vflag].ramm=OFF;

m_p->ship[vflag].main_energy=max_m_en[vflag];
m_p->ship[vflag].sh_energy=max_sh_en[vflag];
m_p->ship[vflag].ph_energy=max_ph_en[vflag];
m_p->ship[vflag].dam[0]=0;
m_p->ship[vflag].dam[1]=0;
m_p->ship[vflag].dam[2]=0;
m_p->ship[vflag].dam[3]=0;
m_p->ship[vflag].dam[4]=0;
m_p->ship[vflag].dam[5]=0;
m_p->ship[vflag].dam[6]=0;
m_p->ship[vflag].dam[7]=0;
m_p->ship[vflag].dam[8]=0;
m_p->ship[vflag].dam[9]=0;
m_p->ship[vflag].dam[10]=0;
m_p->ship[vflag].shield=OFF;
m_p->ship[vflag].sv=OFF;
m_p->ship[vflag].mp=OFF;
m_p->ship[vflag].imp=0;
for (i=0;i<=MISSILE_FUEL;i++)
  {
  m_p->ship[vflag].mpath[i][0]=-99; 
  m_p->ship[vflag].mpath[i][1]=-99; 
  }

m_p->user_score[vflag]=0;
for (i=0;i<anz_ttubes[vflag];i++)
   {
   m_p->ship[vflag].status_ttubes[i]=LOADED;
   }
if (old_score==(-1) )
   {
   m_p->name_score[vflag]=0;
   }
else
   {
   m_p->name_score[vflag]=old_score;
   }
strcpy(m_p->name_of_cmdr[vflag],cmdr_name);         
/*sprintf(m_p->recon_str[vflag],"%-12s: no data available",ships[vflag]); */
m_p->rank[vflag]=i_rank;
if (i_rank==COMMANDER) strcpy(m_p->rank_of_name[vflag],"Commander");
if (i_rank==CAPTAIN) strcpy(m_p->rank_of_name[vflag],"Captain");
if (i_rank==COMMODORE) strcpy(m_p->rank_of_name[vflag],"Commodore");
if (i_rank==ADMIRAL) strcpy(m_p->rank_of_name[vflag],"Admiral");         
printf(" Rank: %s\n",m_p->rank_of_name[vflag]);
usleep(1000000);
a_alt=SPACE;
/**************************************************************************/
/***                fork fuer Eingabe/Cockpit                           ***/
/**************************************************************************/
ppid5=-99;
m_p->anz_user++;
if (party[vflag] == FEDERAL) m_p->anz_user_f++;
if (party[vflag] == IMPERIAL) m_p->anz_user_e++;

pid1 = fork();
if (pid1==-1)
   {
   printf("ERROR: fork fuer COCKPIT fehlgeschlagen ===> EXIT!");
   m_p->luser[vflag]=DESTROYED;
   m_p->anz_user--;
   if (party[vflag] == FEDERAL) m_p->anz_user_f--;
   if (party[vflag] == IMPERIAL) m_p->anz_user_e--;
   (void) shmdt(m_p);
   exit(1);
   }
/***************************************************************************/
/***                     Hauptschleife: pid=0 ---> Cockpit               ***/
/***                                    pid=1 ---> Eingabe/Ausf. Befehle ***/
/***************************************************************************/
if (pid1==0)
   {
   execl("cockpit",vflag_string,NULL);
   }
else /* weiter in dwmain */
   {
   ppid1=pid1;   /* pid von fork fuer Cockpit */
   /**************************************************************************/
   /***                fork fuer move                                      ***/
   /**************************************************************************/
   pid2 = fork();
   if (pid1==-1)
      {
      printf("ERROR: fork fuer MOVE fehlgeschlagen ===> EXIT!");
      m_p->luser[vflag]=DESTROYED;      
      (void) shmdt(m_p);
      exit(1);
      }
   if (pid2==0)
      {
      execl("move",vflag_string,NULL);
      }  
   ppid2=pid2; /* pid von move */
   /* pid3: frueher torpedo! */
   /**************************************************************************/
   /***                fork fuer PHASER (nicht bei Zerstoerern)            ***/
   /**************************************************************************/
   if (level_of_ship[vflag] != DESTROYER)
      { 
      pid4 = fork();
      if (pid1==-1)
         {
         printf("ERROR: fork fuer PHASER fehlgeschlagen ===> EXIT!");
         m_p->luser[vflag]=DESTROYED;      
         (void) shmdt(m_p);
         exit(1);
         }
      if (pid4==0)
         {
         execl("phaser",vflag_string,NULL);
         }  
      ppid4=pid4; /* pid von phaser */
      }
/*********************************************************************/
/** Hauptschleife mit Eingabe des Befehls                          ***/
/*********************************************************************/
   noecho();
   do
      {
      i=0;
      isp=0;  /* Zaehler fuer Leerzeichen */
      strcpy(m_p->cmd[vflag],"                                 ");
      do {
         cc=getch();
         ia=(int)cc;
         if (ia == 32 && i==0) isp=2;
         if (ia == 32 && i>0) ++isp;
         if (isp>0 && ia != 32) isp=0;
         if (isp < 2 && ia != 10) /*Leerzei. weg(max. eins, nicht am Anfang) */
            {
            if (ia == 127 && i > 0)  /* Backspace-Zeichen interpretieren */
	      {
              i--;
              m_p->cmd[vflag][i]=' ';
              usleep(120000);
              m_p->cmd[vflag][i]='\0';
              }
            else
              {
	      if (i<2) cc=(char) toupper(cc);
              m_p->cmd[vflag][i]=cc;  
              m_p->cmd[vflag][i+1]='\0';  
              i++;
	      }
            }
         
         if (cc=='X' && i==1) /* X-Befehl ausfuehren */
            {
            usleep(200000);
            i=(CMD_LENGTH-1);
            sort_msg(xcmd);
            strncpy(m_p->cmd[vflag],xcmd,CMD_LENGTH-1);
            }
         if (cc=='Y' && i==1) /* Y-Befehl ausfuehren */
            {
            usleep(200000);
            i=CMD_LENGTH;
            sort_msg(ycmd);
            strncpy(m_p->cmd[vflag],ycmd,CMD_LENGTH-1);
            }
         if (ia == '!' && i==1) /*  Befehlswiederholung  */
            {
	    i=(CMD_LENGTH-1);
            strncpy(m_p->cmd[vflag],com,CMD_LENGTH-1);
            usleep(200000);
            } 
         }  while (i < (CMD_LENGTH-1)  &&  ia != 10 && m_p->l_exit[vflag] == NO_EXIT); 

      /* Hier Befehle ausfuehren! */
      strncpy(com,m_p->cmd[vflag],CMD_LENGTH);   

   /**********************************************************************/
   /****               Befehl 001: MR                                  ***/
   /**********************************************************************/
      if (strncmp(com,"MR",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         if (a_alt==CLOUD)
            {
            sort_msg("MR: You are inside a gas cloud!");
            sort_msg("    You cannot use warp engines!");
            }
         else
            {
            strcpy(m_p->mr_cmd[vflag],m_p->cmd[vflag]);   
	    while (m_p->ship[vflag].move==ON) usleep(100000);
            kill(ppid2,SIGALRM);
            }
         }
   /**********************************************************************/
   /****               Befehl 002: TO                                  ***/
   /**********************************************************************/
      if (strncmp(com,"TO",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
	 free_ttubes=0;
	 for (i=0;i<anz_ttubes[vflag];i++)
	   {
	   if (m_p->ship[vflag].status_ttubes[i]==LOADED) free_ttubes++;
           }    
	 if (free_ttubes==0)
	   {
           sort_msg("TO: Cannot fire torpedo.");
           sort_msg("    Still reloading tube(s).");
           }
         else if (m_p->ship[vflag].torp < 1)
           {
           sort_msg("TO: *** No torpedo left!! ***");
           }
         else if (m_p->ship[vflag].hood==ON)
           {
           sort_msg("TO: Hood must be off!");
           } 
         else if (m_p->ship[vflag].dam[3] > 300)
           {
           sort_msg("TO: Torpedo tube(s) out of order");
           }
         else
	   {
           strcpy(m_p->to_cmd[vflag],m_p->cmd[vflag]);   
           if ( fork() == 0 )
              {
              execl("torpedo",vflag_string,NULL);
              }
           }
         } 
   /**********************************************************************/
   /****               Befehl 003: DO                                  ***/
   /**********************************************************************/
      if (strncmp(com,"DO",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
	 {
         docking();
         }
   /**********************************************************************/
   /****               Befehl 004: SH                                  ***/
   /**********************************************************************/
      if ( (strncmp(com,"SH U",4) == 0 || strncmp(com,"SH u",4) == 0) && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         shields_up();
         }
      if ( (strncmp(com,"SH D",4) == 0 || strncmp(com,"SH d",4) == 0) && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         shields_down();
         }
   /**********************************************************************/
   /****               Befehl 005: MI                                  ***/
   /**********************************************************************/
      if (strncmp(com,"MI",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         set_mine(com);
         }
   /**********************************************************************/
   /****               Befehl 006: SA                                  ***/
   /**********************************************************************/
      if (strncmp(com,"SA",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         set_sat(com);
         }
   /**********************************************************************/
   /****               Befehl 007: SE                                  ***/
   /**********************************************************************/
      if (strncmp(com,"SE",2) == 0 && strncmp(com,"SEl",3) != 0
          && strncmp(com,"SEL",3) != 0)
         {
         send(com);
         }
   /**********************************************************************/
   /****               Befehl 008: DA                                  ***/
   /**********************************************************************/
      if (strncmp(com,"DA",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         show_damages();
         }
   /**********************************************************************/
   /****               Befehl 009: SC                                  ***/
   /**********************************************************************/
      if (strncmp(com,"SC",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED)
         {
         show_score();
         }
   /**********************************************************************/
   /****               Befehl 010: ID                                  ***/
   /**********************************************************************/
      if (strncmp(com,"ID",2) == 0)
         {
         identify(com[3]);
         }
   /**********************************************************************/
   /****               Befehl 011: IN                                  ***/
   /**********************************************************************/
      if (strncmp(com,"IN",2) == 0)
         {
         info(com);
         }
   /**********************************************************************/
   /****               Befehl 012: BU                                  ***/
   /**********************************************************************/
      if (strncmp(com,"BU",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         build(com);
         }
   /**********************************************************************/
   /****               Befehl 013: CA                                  ***/
   /**********************************************************************/
      if (strncmp(com,"CA",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         capture(com);
         }
   /**********************************************************************/
   /****               Befehl 014: TR                                  ***/
   /**********************************************************************/
      if (strncmp(com,"TR",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         transfer(com);
         }
   /**********************************************************************/
   /****               Befehl 015: RE                                  ***/
   /**********************************************************************/
      if (strncmp(com,"RE",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         repair(com);
         }
   /**********************************************************************/
   /****               Befehl 016: SY                                  ***/
   /**********************************************************************/
      if (strncmp(com,"SY",2) == 0)
         {
         symbol();
         }
   /**********************************************************************/
   /****               Befehl 017: FO                                  ***/
   /**********************************************************************/
      if (strncmp(com,"FO",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         build_fort(com);
         }
   /**********************************************************************/
   /****               Befehl 018: TE                                  ***/
   /**********************************************************************/
      if (strncmp(com,"TE",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         transport_energy(com);
         }
   /**********************************************************************/
   /****               Befehl 019: LE                                  ***/
   /**********************************************************************/
      if (strncmp(com,"LE X=",5) == 0)
         {
         learnX(com);
         }
      if (strncmp(com,"LE Y=",5) == 0)
         {
         learnY(com);
         }
   /**********************************************************************/
   /****               Befehl 020: HO                                  ***/
   /**********************************************************************/
      if (strncmp(com,"HO",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         hood(com);
         }
   /**********************************************************************/
   /****               Befehl 021: IM                                  ***/
   /**********************************************************************/
      if (strncmp(com,"IM",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         impuls(com);
         }
   /**********************************************************************/
   /****               Befehl 022: PH                                  ***/
   /**********************************************************************/
      if (strncmp(com,"PH",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         if (level_of_ship[vflag] != DESTROYER)
            {
	    while (m_p->ship[vflag].phaser==ON)
                  {
                  alarm(1);
                  pause();
                  }
            strcpy(m_p->ph_cmd[vflag],m_p->cmd[vflag]);            	
            kill(ppid4,SIGALRM); 
            }
         else
            {
            sort_msg("PH:Unknown command for DESTROYER");
            }
         } 
   /**********************************************************************/
   /****               Befehl 023: RA                                  ***/
   /**********************************************************************/
      if (strncmp(com,"RA",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         if (strncmp(com,"RA on",5)==0 || strncmp(com,"RA ON",5)==0)
            {
            alarm(1);            	
            pause();
            m_p->ship[vflag].ramm=ON;
            sort_msg("RA: Computer set to RA ON!");
            }
         else
            {
            if (strncmp(com,"RA of",5)==0 || strncmp(com,"RA OF",5)==0)
               {
               alarm(1);               	
               pause();
               m_p->ship[vflag].ramm=OFF;
               sort_msg("RA: Computer set to RA OFF!");
               }
            else
               {
               sort_msg("   *** BAD COMMAND ***");
               }
            }
         }
   /**********************************************************************/
   /****               Befehl 024: HE                                  ***/
   /**********************************************************************/
      if (strncmp(com,"HE",2) == 0)
         {
         if (strncmp(com,"HE MR",5) == 0)
	    {
            sort_msg("HE: MR X Y -   Move relative X  ");
            sort_msg("HE: steps vertically and Y steps");
            sort_msg("HE: horizontally.               ");
	    }
         else if (strncmp(com,"HE TO",5) == 0)
	    {
            sort_msg("HE: TO X Y - Fire torpedo to the");
            sort_msg("HE:       coordinates X Y       ");
            sort_msg("HE:            or ");
            sort_msg("HE: TO c ? -Fire torpedo the the");
            sort_msg("HE: ship located by computer?");
	    }
         else if (strncmp(com,"HE DO",5) == 0)
	    {
            sort_msg("HE: DO - Docking at an own basis");
	    }
         else if (strncmp(com,"HE SH",5) == 0)
	    {
            sort_msg("HE: SH up/down - Raise or lower ");
            sort_msg("HE: shields ");
	    }
         else if (strncmp(com,"HE MI",5) == 0)
	    {
            sort_msg("HE: MI X Y - Put mine at   ");
            sort_msg("HE: coordinates X Y ");
	    }
         else if (strncmp(com,"HE SA",5) == 0)
	    {
            sort_msg("HE: SA X Y - Put satellite at   ");
            sort_msg("HE:       coordinates X Y       ");
	    }
         else if (strncmp(com,"HE SE",5) == 0)
	    {
            sort_msg("HE: SE ? text - Send text to the");
            sort_msg("HE: ship ?");
	    }
         else if (strncmp(com,"HE DA",5) == 0)
	    {
            sort_msg("HE: DA - Report damages        ");
	    }
         else if (strncmp(com,"HE SC",5) == 0)
	    {
            sort_msg("HE: SC - Show score            ");
	    }
         else if (strncmp(com,"HE ID",5) == 0)
	    {
            sort_msg("HE: ID ? - Identify ship with  ");
            sort_msg("HE: symbol ?                   ");
	    }
         else if (strncmp(com,"HE IN",5) == 0)
	    {
            sort_msg("HE: IN ? - Display information ");
            sort_msg("HE: about planets (p), bases (b),");
            sort_msg("HE: forts (f), jumpgates (j),");
            sort_msg("HE: missile (m), scout (s), ro-");
            sort_msg("HE: botships (r)");
	    }
         else if (strncmp(com,"HE BU",5) == 0)
	    {
            sort_msg("HE: BU x y - Build planet at x y");
            sort_msg("HE: to a higher level or base ");
	    }
         else if (strncmp(com,"HE CA",5) == 0)
	    {
            sort_msg("HE: CA X Y - Capture planet at ");
            sort_msg("HE: coordinates X Y");
	    }
         else if (strncmp(com,"HE TR",5) == 0)
	    {
            sort_msg("HE: TR n a b - Transfer n energy");
            sort_msg("HE: units from a to b; s=shields,");
            sort_msg("HE: p=phaser, m=main");
	    }
         else if (strncmp(com,"HE TE",5) == 0)
	    {
            sort_msg("HE: TE n ? - Transfer n energy ");
            sort_msg("HE: units to ship ?");
	    }
         else if (strncmp(com,"HE TF",5) == 0)
	    {
            sort_msg("HE: TF n ? - Transfer n fuel ");
            sort_msg("HE: units to ship ?");
	    }
         else if (strncmp(com,"HE TT",5) == 0)
	    {
            sort_msg("HE: TT n ? - Transfer n tor-");
            sort_msg("HE: pedoes to ship ?");
	    }
         else if (strncmp(com,"HE RE",5) == 0)
	    {
            sort_msg("HE: RE n - Repair 20 damage ");
            sort_msg("HE: points in section n");
	    }
         else if (strncmp(com,"HE SY",5) == 0)
	    {
            sort_msg("HE: SY - List the meaning of ");
            sort_msg("HE: symbols in the universe");
	    }
         else if (strncmp(com,"HE FO",5) == 0)
	    {
            sort_msg("HE: FO X Y - Build fort at the ");
            sort_msg("HE: coordinates X Y");
	    }
         else if (strncmp(com,"HE LE",5) == 0)
	    {
            sort_msg("HE: LE X=<cmd> or LE Y=<cmd> ");
            sort_msg("    Shortcut for <cmd> on X  ");
            sort_msg("    or Y button.");
	    }
         else if (strncmp(com,"HE HO",5) == 0)
	    {
            sort_msg("HE: HO on/off - Hood on/off");
	    }
         else if (strncmp(com,"HE IM",5) == 0)
	    {
            sort_msg("HE: IM X Y - Move relative by im-");
            sort_msg("HE: pulse engines, max. 1 step");
	    }
         else if (strncmp(com,"HE PH",5) == 0)
	    {
            sort_msg("HE: PH X Y n- Fire n units by");
            sort_msg("HE: phaser to X Y");
            sort_msg("HE:            or ");
            sort_msg("HE: PH c ? n - Fire n units by");
            sort_msg("HE: phaser to the ship located ");
            sort_msg("HE: by computer ");
	    }
         else if (strncmp(com,"HE HW",5) == 0 || strncmp(com,"HE hw",5) == 0)
	    {
            sort_msg("HE: HW - Dock at homeworld");
            sort_msg("    Subcommands available:");
            sort_msg(" START - SELECT -  ");
            sort_msg(" DS FL/RE/BP - display fleet");
            sort_msg(" reconnais., planets&bases");
	    }
         else if (strncmp(com,"HE LS",5) == 0 || strncmp(com,"HE ls",5) == 0)
	    {
            sort_msg("HE: LS x y: Launch scout vessel");
            sort_msg("HE: to x y as target coord.");
            sort_msg("HE: You must at least be a");
            sort_msg("HE: Commander to launch a scout!");
	    }
         else if (strncmp(com,"HE LM",5) == 0 || strncmp(com,"HE lm",5) == 0)
	    {
            sort_msg("HE: LM x1 y1 x2 y2 or LM x y P n:");
            sort_msg("  launch missile to x1 y1 via x2");
	    sort_msg("  y2, or via recorded path n. You");
            sort_msg("  must be Admiral and HW-docked!");
            sort_msg("  See also: HE MP, HE KM, HE IN.");
	    }
         else if (strncmp(com,"HE LR",5) == 0 || strncmp(com,"HE lr",5) == 0)
	    {
            sort_msg("HE: LR n m x y: lauch robotship");
            sort_msg("  n=number of rob., m=mission,");
	    sort_msg("  x y = target coordinates.");
            sort_msg("  Missions: M,P,C,J");
	    }
         else if (strncmp(com,"HE SELECT",9) == 0 || strncmp(com,"HE select",9) == 0)
	    {
            sort_msg("HE: select <ship>");
            sort_msg("HE: change the ship");
	    }
         else if (strncmp(com,"HE START",8) == 0 || strncmp(com,"HE start",8) == 0)
	    {
            sort_msg("HE: Start new mission");
            sort_msg("HE: (when docked at HW only!)");
	    }
         else if (strncmp(com,"HE RA",5) == 0)
	    {
            sort_msg("HE: RA on/off - Enables or dis-");
            sort_msg("HE: ables ramming");
	    }
         else if (strncmp(com,"HE RS",5) == 0)
	    {
            sort_msg("HE: RS - restart of forked");
            sort_msg("    processes like cockpit");
	    }
         else if (strncmp(com,"HE KS",5) == 0)
	    {
            sort_msg("HE: KS - kill (own) scout");
            sort_msg("    (see also HE LS)");
	    }
         else if (strncmp(com,"HE KM",5) == 0)
	    {
            sort_msg("HE: KS - kill (own) missile");
            sort_msg("    (see also HE LM)");
	    }
         else if (strncmp(com,"HE QU",5) == 0)
	    {
            sort_msg("HE: QU - Quit the game");
	    }
         else if (strncmp(com,"HE BR",5) == 0)
	    {
            sort_msg("HE: BR - Build Robotship");
	    }
         else if (strncmp(com,"HE MP",5) == 0 || strncmp(com,"HE mp",5) == 0)
	    {
            sort_msg("HE: MP on: start missile path");
            sort_msg("    recording.");
            sort_msg("    MP off: stop missile path");
            sort_msg("    recording.");
	    }
         else if (strncmp(com,"HE SV",5) == 0 || strncmp(com,"HE sv",5) == 0)
	    {
            sort_msg("HE: SV - Save modus ON or OFF");
            sort_msg("    ON: Torp. or phaser not  ");
            sort_msg("    fired when path to target");
            sort_msg("    obstructed; Default: OFF");
	    }
         else if (strncmp(com,"HE sd ra",8) == 0 || strncmp(com,"HE SD RA",8) == 0 )
	    {
            sort_msg("HE: SD RA - Set display to");
            sort_msg("radar image (default except HW)");
	    }
         else if (strncmp(com,"HE sd ro",8) == 0 || strncmp(com,"HE SD RO",8) == 0 )
	    {
            sort_msg("HE: SD RO - Set display to");
            sort_msg("status of all own robotships");
	    }
         else if (strncmp(com,"HE sd sc",8) == 0 || strncmp(com,"HE SD SC",8) == 0 )
	    {
            sort_msg("HE: SD SC - Set display to");
            sort_msg("scout radar image (when scout");
            sort_msg("is active only!)");
	    }
         else if (strncmp(com,"HE sd fl",8) == 0 || strncmp(com,"HE SD FL",8) == 0 )
	    {
            sort_msg("HE: SD FL - Set display to");
            sort_msg("fleet status (when docked at");
            sort_msg("at homeworld only!)");
	    }
         else if (strncmp(com,"HE sd re",8) == 0 || strncmp(com,"HE SD RE",8) == 0 )
	    {
            sort_msg("HE: SD RE - Set display to");
            sort_msg("reconnaissance data (when");
            sort_msg("docked at homeworld only,");
            sort_msg("rank Commodore or higher!)");
	    }
         else if (strncmp(com,"HE sd bp",8) == 0 || strncmp(com,"HE SD BP",8) == 0 )
	    {
            sort_msg("HE: SD BP - Displaying");
            sort_msg("position and status of bases,");
            sort_msg("planets, forts, and satellites");
            sort_msg("(when docked at homeworld and");
            sort_msg("rank Commodore or higher!)");
	    }
         else if (strncmp(com,"HE sd hw",8) == 0 || strncmp(com,"HE SD HW",8) == 0 )
	    {
            sort_msg("HE: SD BP - Displaying");
            sort_msg("Homeword base and HQ.");
            sort_msg("(when docked at homeworld only!)");
	    }
         else if (strncmp(com,"HE sd ba",8) == 0 || strncmp(com,"HE SD BA",8) == 0 )
	    {
            sort_msg("HE: SD BP - Displaying");
            sort_msg("base and base status.");
	    }
         else if (strncmp(com,"HE sd m",7) == 0 || strncmp(com,"HE SD M",7) == 0 )
	    {
            sort_msg("HE: SD M - Set display to");
            sort_msg("overview map of installations");
            sort_msg("of your party in the universe");
            sort_msg("(when docked at homeworld and");
            sort_msg("rank Commodore or higher!)");
	    }
         else if (strncmp(com,"HE SD",5) == 0 || strncmp(com,"HE sd",5) == 0)
	    {
            sort_msg("HE: SD (set display)");
            sort_msg(" SD RA/SC: radar/scout image");
            sort_msg(" when docked at HW only:");
            sort_msg(" SD FL/RE/BP/M/HW/RO");
            sort_msg(" SD BA: base (docked)");
	    }
         else if (strncmp(com,"HE JG",5) == 0 || strncmp(com,"HE jg",5) == 0)
	    {
            sort_msg("HE: JG (build jumpgate)");
            sort_msg("    JG x y code");
	    }
         else if (strncmp(com,"HE JU",5) == 0 || strncmp(com,"HE ju",5) == 0)
	    {
            sort_msg("HE: JU (use jumpgate)");
            sort_msg("    JU a b");
            sort_msg("  a b are jumpgate codes");
	    }
         else if (strncmp(com,"HE CP",5) == 0 || strncmp(com,"HE cp",5) == 0)
	    {
            sort_msg("HE: CP: Clear (missile) Path n");
            sort_msg("  CP n");
	    }
         else if (strncmp(com,"HE SP",5) == 0 || strncmp(com,"HE sp",5) == 0)
	    {
            sort_msg("HE: CP: Save (missile) Path n");
            sort_msg("  SP n");
	    }
         else if (strncmp(com,"HE !",4) == 0)
	    {
            sort_msg("HE: ! - Repeat last command");
	    }
         else if (strncmp(com,"HE KR",5) == 0 || strncmp(com,"HE kr",5) == 0)
	    {
            sort_msg("HE: KR n - Kill robotship n");
            sort_msg("           (own party only) if");
            sort_msg("           status is ACTIVE or");
            sort_msg("           WAITING");
            sort_msg("    KR a - all own robotships");
	    }
         else if (strncmp(com,"HE CR",5) == 0 || strncmp(com,"HE cr",5) == 0)
	    {
            sort_msg("HE: CR n - Call back robotship n");
            sort_msg("           (own party only) if");
            sort_msg("           status is ACTIVE");
            sort_msg("    CR a - all own robotships");
	    }
         else
	    {
            sort_msg("HE: Commands: ! BR BU CA CP CR");
            sort_msg(" DA DO FO HE HO HE ID IM IN JG");
            sort_msg(" JU KR KS KM LE LS LM MI MP MR");
            sort_msg(" PH QU RA RE RS SA SC SD SE SH");
            sort_msg(" SP SV SY TO TE TF TR TT HW");
            sort_msg(" Details: HE <cmd>");
	    }
         }
   /**********************************************************************/
   /****               Befehl 025: QU                                  ***/
   /**********************************************************************/
      if (strncmp(com,"QU",2) == 0)
         {
         if (quit_control()==0) strcpy(m_p->cmd[vflag],"QUIT1234");
         }

   /**********************************************************************/
   /****               Befehl 026: HW                                  ***/
   /**********************************************************************/
      if (strncmp(com,"HW",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         hw_docking();
         }

   /**********************************************************************/
   /****               Befehl 027a: HW-Befehl SElect                   ***/
   /**********************************************************************/
      if (strncmp(com,"SElect",6) == 0 || strncmp(com,"SELECT",6) == 0)
         {
         select_ship(&ppid1,&ppid2,&ppid4,com);
         sprintf(vflag_string,"%d",vflag);
         }

   /**********************************************************************/
   /****               Befehl 027b: Befehl STart                       ***/
   /**********************************************************************/
      if (strncmp(com,"STart",5) == 0 || strncmp(com,"START",5) == 0)
	 {
         if (m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
	    {
            sort_msg("  You must be docked at");
            sort_msg("    BASE or HOMEWORLD");
            sort_msg("  to use this command!");
            }
         else
            {
            start_ship();
            }
         strcpy(com," "); /* Damit spaeter nicht Kommando ST ausgef. wird */
	 }
  
   /**********************************************************************/
   /****               Befehl 027g: HW-Befehl LS                       ***/
   /**********************************************************************/
      if (strncmp(com,"LS",2) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("in order to lauch a scout!");
            }
         else if (m_p->rank[vflag] < COMMODORE)
            {
            sort_msg("You must be commodore or");
            sort_msg("admiral to launch a scout");
            }
         else if (m_p->scout[party[vflag]].status==ACTIVE)
	    {
            sort_msg("Scout already launched!");
            }
         else
	   {
           if (sscanf(com,"%s %s %s",scout_cm,scout_x,scout_y) == 3)
              {
	      if (sscanf(scout_x,"%d",&x_scout)==1 && sscanf(scout_y,"%d",&y_scout)==1)
                {
                if ( x_scout < 0 || x_scout > ARRAY_SIZE-1 ||
                   y_scout < 0 || y_scout > ARRAY_SIZE-1 )
		  {
                  sort_msg("     *** BAD COMMAND ***     ");                 
                  }
                else if (abs(x_scout-m_p->homeworld[party[vflag]].pos[0])<20
                                          && 
                         abs(y_scout-m_p->homeworld[party[vflag]].pos[1])<20 )
                  {
                  sort_msg("Target too close to Homeworld!");
                  sort_msg("Scout vessel was not launched!");
                  }
                else
		  {
                  sort_msg("Launching scout vessel...");
                  strcpy(scout_msg,"Destination ");
                  strcat(scout_msg,scout_x);
                  strcat(scout_msg," ");
                  strcat(scout_msg,scout_y);
                  sort_msg(scout_msg);
                  usleep(250000);
                  pid5 = fork();
                  if (pid5 < 0 || pid5 == 1)
                    {
                    sort_msg("ERROR: fork for SCOUT failed!");
                     }
                  else if (pid5==0)
                    {
		    execl("scout",vflag_string,scout_x,scout_y,NULL);
                    }
                  else
		    {
                    ppid5=pid5;
                    usleep(200000);
                    }
		  }
                }
	      else
                {
                sort_msg("     *** BAD COMMAND ***    ");            
                }
              }
           else
              {
              sort_msg("     *** BAD COMMAND ***     ");
              }
	   }
	 }
   /**********************************************************************/
   /****               Befehl 027h: HW-Befehl LM                       ***/
   /**********************************************************************/
      if (strncmp(com,"LM",2) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("in order to lauch a missile!");
            }
         else if (m_p->rank[vflag] < ADMIRAL)
            {
            sort_msg("You must be Admiral in");
            sort_msg("oder to launch a missile!");
            }
         else if (m_p->missile[party[vflag]].status==ACTIVE)
	    {
            sort_msg("Missile already running!");
            }
         else
	   {
	   if (sscanf(com,"%s %d %d %d %d",missile_cm,&x_missile,&y_missile,&xs_missile,&ys_missile) == 5) /* kein Pfad */
              {
              if ( x_missile < 0 || x_missile > ARRAY_SIZE-1 ||
                   y_missile < 0 || y_missile > ARRAY_SIZE-1 ||
                   ys_missile < 0 || ys_missile > ARRAY_SIZE-1 ||
                   xs_missile < 0 || xs_missile > ARRAY_SIZE-1 ||
                  (xs_missile == x_missile && ys_missile==y_missile) )
		{
                sort_msg("     *** BAD COMMAND ***     ");                 
                }
              else
		{
                sprintf(c1,"%d",x_missile);
                sprintf(c2,"%d",y_missile);
                sprintf(c3,"%d",xs_missile);
                sprintf(c4,"%d",ys_missile);
                m_p->user_score[vflag]=m_p->user_score[vflag]+100;
	        m_p->name_score[vflag]=m_p->name_score[vflag]+100;
	        m_p->score[party[vflag]]=m_p->score[party[vflag]]+100;
                usleep(250000);
                pid6 = fork();
                if (pid6 < 0 || pid6 == 1)
                   {
                   sort_msg("ERROR: fork for Missile failed!");
                   }
                else if (pid6==0)
                   {
	           execl("missile",vflag_string,c1,c2,c3,c4,NULL);
                   }
                else
	           {
                   usleep(200000);
                   }
         	}
              } 
	   else if (sscanf(com,"%s %d %d %s %d",missile_cm,&x_missile,&y_missile,cp,&ip) == 5)
              {
              if (m_p->ship[vflag].mp==ON)
                {
		sort_msg("LM: MP on! Must be off!");
                }
              else if ( x_missile < 0 || x_missile > ARRAY_SIZE-1 ||
                   y_missile < 0 || y_missile > ARRAY_SIZE-1 ||
		   (cp[0] != 'P' && cp[0] != 'p') || ip < 0 || 
		    ip >= MAX_MPATHS || strlen(cp) > 1)
		{
                sort_msg("LM: bad parameters");                 
                }
              else
		{
                sprintf(c1,"%d",x_missile);
                sprintf(c2,"%d",y_missile);
                sprintf(c3,"%d",ip);
		m_p->user_score[vflag]=m_p->user_score[vflag]+100;
	        m_p->name_score[vflag]=m_p->name_score[vflag]+100;
	        m_p->score[party[vflag]]=m_p->score[party[vflag]]+100;
                usleep(250000);
                pid6 = fork();
                if (pid6 < 0 || pid6 == 1)
                  {
                  sort_msg("ERROR: fork for Missile failed!");
                  }
                else if (pid6==0)
                  {
	          execl("missile",vflag_string,c1,c2,"-11",c3,NULL);
                  } 
                else
		  {
                  usleep(200000);
                  }
                }
              }
           else
              {
              sort_msg("     *** BAD COMMAND ***    ");
              }
	   }
	 }
   /**********************************************************************/
   /****               Befehl 027i: HW-Befehl LR                       ***/
   /**********************************************************************/
      if (strncmp(com,"LR",2) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("in order to lauch a robotship!");
            }
         else if (m_p->rank[vflag] < ADMIRAL)
            {
            sort_msg("You must be Admiral in");
            sort_msg("oder to launch a robotship!");
            }
         else
	   {
           if (sscanf(com,"%s %d %s %d %d",robotship_cm,&nrob,c3,&x_robotship,&y_robotship) == 5)
              {
              if ( x_robotship < 0 || x_robotship > ARRAY_SIZE-1 ||
                   y_robotship < 0 || y_robotship > ARRAY_SIZE-1)
		{
                sort_msg("     *** BAD COMMAND ***    ");                 
                }
              else if (nrob >= MAX_ROBOTSHIPS || nrob < 0 )
                {
		sort_msg("LR: invalid number!");   
                }
	      else if (m_p->robotship[party[vflag]][nrob].status!=HW_DOCKED)
                {
		sort_msg("LR: invalid number!");   
                }
              else
		{
		sort_msg(com);
                sprintf(c0,"%d",nrob);
                sprintf(c1,"%d",x_robotship);
                sprintf(c2,"%d",y_robotship);
		m_p->user_score[vflag]=m_p->user_score[vflag]+100;
	        m_p->name_score[vflag]=m_p->name_score[vflag]+100;
	        m_p->score[party[vflag]]=m_p->score[party[vflag]]+100;
                usleep(250000);
                pid7 = fork();
                if (pid7 < 0 || pid7 == 1)
                  {
                  sort_msg("ERROR: fork for Robotship failed!");
                  }
                else if (pid7==0)
                  {
		  execl("robot",vflag_string,c0,c3,c1,c2,NULL);
                  } 
                else
		  {
                  usleep(200000);
                  }
                }
              }
           else
              {
              sort_msg("     *** BAD COMMAND ***   ");
              }
	   }
	 }

   /**********************************************************************/
   /****               Befehl 028: Befehl KS                           ***/
   /**********************************************************************/
      if (strncmp(com,"KS",2) == 0)
         {
         if (m_p->rank[vflag] < COMMODORE)
            {
            sort_msg("You must be commodore or");
            sort_msg("admiral to kill a scout");
            }
         else if (m_p->scout[party[vflag]].status==DESTROYED)
	    {
            sort_msg("No scout active!");
            }
         else
	    {
            sort_msg("Self destruction initiated");
            if (m_p->display_status[vflag] == DISPLAY_SCOUT)
               {
               if (m_p->ship[vflag].dock == HW_DOCKED) 
		 {
                 m_p->display_status[vflag]=DISPLAY_HW;
                 }
               else
		 {
                 m_p->display_status[vflag]=DISPLAY_RADAR;
                 }
               }
            m_p->scout[party[vflag]].status=DESTROYED;
            m_p->a[m_p->scout[party[vflag]].pos[0]][m_p->scout[party[vflag]].pos[1]]=SPACE;
            m_p->scout[party[vflag]].pos[0]=-99;
            m_p->scout[party[vflag]].pos[1]=-99;
            m_p->scout[party[vflag]].target[0]=-99;
            m_p->scout[party[vflag]].target[1]=-99;
            ppid5 = -99;
            sort_msg("Scout destroyed!");
            }
         }
   /**********************************************************************/
   /****               Befehl 029: Befehle SD ..  (z.T. HW-Kommandos   ***/
   /**********************************************************************/
      if (strncmp(com,"SD SC",5) == 0 || strncmp(com,"SD sc",5) == 0)
         {
         if (m_p->scout[party[vflag]].status == DESTROYED)
            {
            sort_msg("SD sc: No scout active!");
            }
         else if (m_p->ship[vflag].dam[6] > 150)
	    {
            sort_msg("SD sc: Computer damaged! Cannot");
            sort_msg("    display scout radar image!");
            }
         else if (m_p->ship[vflag].dam[9] > 150)
	    {
            sort_msg("SD sc: Radio damaged! Cannot");
            sort_msg("    receive scout radar data!");
            }
         else
            {
            sort_msg("SD: Displaying scout radar image!");
            m_p->display_status[vflag]=DISPLAY_SCOUT;
            }
         }

      if (strncmp(com,"SD FL",5) == 0 || strncmp(com,"SD fl",5) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("SD: You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("    to use this command!");
            }
         else if (m_p->homeworld[party[vflag]].dam > (MAX_DAM_HOMEWORLD/2))
	   {
	   sort_msg("SD fl: Data access failed!");
	   sort_msg("Homeworld severely damaged!");
	   }
         else
	    {
            sort_msg("SD: Display set to fleet status");
            m_p->display_status[vflag]=DISPLAY_FLEET;
	    }
         }

      if (strncmp(com,"SD RA",5) == 0 || strncmp(com,"SD ra",5) == 0)
         {
	 if (m_p->ship[vflag].dock == HW_DOCKED && m_p->homeworld[party[vflag]].dam > (MAX_DAM_HOMEWORLD/2))
	   {
	   sort_msg("SD ra: Cannot display radar!");
           sort_msg("Homeworld is severely damaged!");
           }
	 else if (m_p->ship[vflag].dock == DOCKED)
           {
           nrb=find_nr_base(m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1]);
           if (m_p->base[nrb].dam > ((MAX_DAM_BAS/10)*5))
	     {
	     sort_msg("SD ra: Cannot display radar!");
             sort_msg("   Base is severely damaged!");
             }
           else
             {
             sort_msg("SD: Display set to radar scan");
             m_p->display_status[vflag]=DISPLAY_RADAR;
             }
           }
         else
           {
           sort_msg("SD: Display set to radar scan");
           m_p->display_status[vflag]=DISPLAY_RADAR;
           }
         }

      if (strncmp(com,"SD RE",5) == 0 || strncmp(com,"SD re",5) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("SD: You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("    to use this command!");
            }
         else if (m_p->rank[vflag] < COMMODORE)
            {
            sort_msg("SD: Access denied!");
            sort_msg("    You must be Commodore or");
            sort_msg("    Admiral to get access to");
            sort_msg("    confidential information!");
            }
         else if (m_p->homeworld[party[vflag]].dam > (MAX_DAM_HOMEWORLD/2))
	   {
	   sort_msg("SD re: Data access failed!");
	   sort_msg("Homeworld severely damaged!");
	   }
         else
	    {
            sort_msg("SD: Displaying reconnaiss. data");
            m_p->display_status[vflag]=DISPLAY_RECON;
            }
         }

      if (strncmp(com,"SD BP",5) == 0 || strncmp(com,"SD bp",5) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("SD: You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("    to use this command!");
            }
         else if (m_p->homeworld[party[vflag]].dam > (MAX_DAM_HOMEWORLD/2))
	   {
	   sort_msg("SD bp: Data access failed!");
	   sort_msg("Homeworld severely damaged!");
	   }
         else
            {
            sort_msg("SD: Displaying bases and planets");
            m_p->display_status[vflag]=DISPLAY_BP;
            }
         }

      if (strncmp(com,"SD HW",5) == 0 || strncmp(com,"SD hw",5) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("SD: You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("    to use this command!");
            }
         else
            {
            sort_msg("SD: Displaying Homeworld Base");
            m_p->display_status[vflag]=DISPLAY_HW;
            }
         }
      if (strncmp(com,"SD BA",5) == 0 || strncmp(com,"SD ba",5) == 0)
         {
         if (m_p->ship[vflag].dock != DOCKED)
            {
            sort_msg("SD: You must be docked at a");
            sort_msg("    BASE to use this command!");
            }
         else
            {
            sort_msg("SD ba: Displaying Base");
            m_p->display_status[vflag]=DISPLAY_BASE;
            }
         }

      if (strncmp(com,"SD M",4) == 0 || strncmp(com,"SD m",4) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("SD: You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("    to use this command!");
            }
         else if (m_p->homeworld[party[vflag]].dam > (MAX_DAM_HOMEWORLD/2))
	   {
	   sort_msg("SD m: Data access failed!");
	   sort_msg("Homeworld severely damaged!");
	   }
         else
            {
            sort_msg("SD m: Display set to map");
            m_p->display_status[vflag]=DISPLAY_MAP;
            }
         }

      if (strncmp(com,"SD RO",5) == 0 || strncmp(com,"SD ro",5) == 0)
         {
         if (m_p->ship[vflag].dock != HW_DOCKED)
            {
            sort_msg("SD: You must be docked at your");
            sort_msg("        HOMEWORLD");
            sort_msg("    to use this command!");
            }
         else if (m_p->homeworld[party[vflag]].dam > (MAX_DAM_HOMEWORLD/2))
	   {
	   sort_msg("SD ro: Data access failed!");
	   sort_msg("Homeworld severely damaged!");
	   }
         else
            {
            sort_msg("SD ro: Displaying robotship data");
            m_p->display_status[vflag]=DISPLAY_ROBOT;
            }
         }

   /**********************************************************************/
   /****               Befehl 030: Befehl RS                           ***/
   /**********************************************************************/
      if (strncmp(com,"RS",2) == 0 || strncmp(com,"rs",2) == 0)
         {
         restart_procs(&ppid1,&ppid2,&ppid4);
         }
   /**********************************************************************/
   /****               Befehl 031: Befehl ST                          ***/
   /**********************************************************************/
      if ( (strncmp(com,"ST",2) == 0 || strncmp(com,"st",2) == 0) &&
            m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         if (m_p->ship[vflag].move==OFF)
	    {
            sort_msg("MR: Warp engines already stopped!");
            }
         else
	    {
            m_p->ship[vflag].move=OFF;
            sort_msg("MR: Warp engines stopped!");
            }
         }
   /**********************************************************************/
   /****               Befehl 032: Befehl JG                           ***/
   /**********************************************************************/
      if (strncmp(com,"JG",2) == 0 || strncmp(com,"jg",2) == 0)
         {
         build_jumpgate(com);
         }
   /**********************************************************************/
   /****               Befehl 033: Befehl JU                           ***/
   /**********************************************************************/
      if (strncmp(com,"JU",2) == 0 || strncmp(com,"ju",2) == 0)
         {
         jump(com);
         }
   /**********************************************************************/
   /****               Befehl 034: Befehl KM                           ***/
   /**********************************************************************/
      if (strncmp(com,"KM",2) == 0)
         {
         if (m_p->rank[vflag] < ADMIRAL)
            {
            sort_msg("You must be Admiral!");
            }
         else if (m_p->missile[party[vflag]].status==DESTROYED)
	    {
            sort_msg("No missile running!");
            }
         else
	    {
            sort_msg("Self destruction initiated...");
            m_p->missile[party[vflag]].status=DESTROYED;
            m_p->a[m_p->missile[party[vflag]].pos[0]][m_p->missile[party[vflag]].pos[1]]=SPACE;
            m_p->missile[party[vflag]].pos[0]=-99;
            m_p->missile[party[vflag]].pos[1]=-99;
            m_p->missile[party[vflag]].target[0]=-99;
            m_p->missile[party[vflag]].target[1]=-99;
            m_p->missile[party[vflag]].subtarget[0]=-99;
            m_p->missile[party[vflag]].subtarget[1]=-99;
            }
         }
   /**********************************************************************/
   /****               Befehl 035: Befehl SV                           ***/
   /**********************************************************************/
      if (strncmp(com,"SV ON",5) == 0 || strncmp(com,"SV on",5) == 0)
         {
	 m_p->ship[vflag].sv=ON;
         sort_msg("SV set to ON (SV:+)!");
         }
      else if (strncmp(com,"SV OFF",6) == 0 || strncmp(com,"SV off",6) == 0)
         {
	 m_p->ship[vflag].sv=OFF;
         sort_msg("SV set to OFF (SV:-)!");
         }
      else if (strncmp(com,"SV",2) == 0 || strncmp(com,"sv",2) == 0)
	 {
	   sort_msg(" *** BAD COMMAND ***");
         }
   /**********************************************************************/
   /****               Befehl 036: Befehl MP                           ***/
   /**********************************************************************/
      if (strncmp(com,"MP ON",5) == 0 || strncmp(com,"MP on",5) == 0)
         {
	 if (m_p->ship[vflag].mp==ON)
	   {
	   sort_msg("MP: Already ON!");
           }
         else
           {
	   if ((abs(m_p->ship[vflag].pos[0]-m_p->homeworld[party[vflag]].pos[0]))>1 || (abs(m_p->ship[vflag].pos[1]-m_p->homeworld[party[vflag]].pos[1]))>1)
              {
              sort_msg("MP on: Distance to HW must NOT");
              sort_msg("   be > 1! MP not set to ON!");
              }
           else if (m_p->ship[vflag].dock!=NOT_DOCKED) 
             {
             sort_msg("MP on: Must not be docked!");
             sort_msg("       MP not set to ON!");
             }
           else 
	     {
	     m_p->ship[vflag].mp=ON;
             for (i=0;i<=MISSILE_FUEL;i++)
               {
               m_p->ship[vflag].mpath[i][0]=-99; 
               m_p->ship[vflag].mpath[i][1]=-99; 
               }
	     m_p->ship[vflag].imp=0;
             sort_msg("MP: Starting Path recording");
             m_p->ship[vflag].mpath[0][0]=m_p->ship[vflag].pos[0];
             m_p->ship[vflag].mpath[0][1]=m_p->ship[vflag].pos[1];
             }
           }
         }
      else if (strncmp(com,"MP OFF",6) == 0 || strncmp(com,"MP off",6) == 0)
         {
	 if (m_p->ship[vflag].mp==OFF)
	   {
	   sort_msg("MP: Already OFF!");
           }
         else
           {
	   m_p->ship[vflag].mp=OFF;
           sort_msg("MP: Finishing Path recording!");
           sprintf(print_str,"Recorded steps: %3d",m_p->ship[vflag].imp);
           sort_msg(print_str);
           }
         }
      else if (strncmp(com,"MP",2) == 0 || strncmp(com,"mp",2) == 0)
	 {
	 sort_msg(" *** BAD COMMAND ***");
         }
   /**********************************************************************/
   /****               Befehl 037: Befehl BR (HW)                      ***/
   /**********************************************************************/
      if (strncmp(com,"BR",2) == 0 && m_p->homeworld[party[vflag]].status==DESTROYED)
         {
         sort_msg("BR: error; no homeworld!");
         }
      else if (strncmp(com,"BR",2) == 0 && m_p->ship[vflag].dock!=HW_DOCKED)
         {
         sort_msg("BR: error; must be docked at HW");
         }
      else if (strncmp(com,"BR",2) == 0 && m_p->ship[vflag].dock==HW_DOCKED)
         {
	 if (m_p->rank[vflag] < ADMIRAL)
           {
	   sort_msg("BR: error; you must be Admiral!");
	   }
         else
           {
	   build_robotship();
           }
         }
      else if (strncmp(com,"BR",2) == 0)
	 {
	 sort_msg(" *** BAD COMMAND ***");
         }
   /**********************************************************************/
   /****               Befehl 038: TF                                  ***/
   /**********************************************************************/
      if (strncmp(com,"TF",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         transport_fuel(com);
         }
   /**********************************************************************/
   /****               Befehl 039: TT                                  ***/
   /**********************************************************************/
      if (strncmp(com,"TT",2) == 0 && m_p->ship[vflag].dock != HW_DOCKED && m_p->ship[vflag].dock != DOCKED)
         {
         transport_torp(com);
         }
   /**********************************************************************/
   /****               Befehl 040: SP                                  ***/
   /**********************************************************************/
      if (strncmp(com,"SP",2) == 0)
         {
         save_path(com);
         }
   /**********************************************************************/
   /****               Befehl 041: CP                                  ***/
   /**********************************************************************/
      if (strncmp(com,"CP",2) == 0)
         {
         clear_path(com);
         }
   /**********************************************************************/
   /****               Befehl 042: KR                                  ***/
   /**********************************************************************/
      if (strncmp(com,"KR",2) == 0)
         {
         if (m_p->rank[vflag] < ADMIRAL)
            {
            sort_msg("KR: must be Admiral!");
            }
         else if (m_p->ship[vflag].dock != HW_DOCKED)
	    {
	    sort_msg("KR: must be docked at Homeworld!");
            }
         else
	    {
            kill_robotship(com);
            }
         }
   /**********************************************************************/
   /****               Befehl 043: CR                                  ***/
   /**********************************************************************/
      if (strncmp(com,"CR",2) == 0)
         {
         if (m_p->rank[vflag] < ADMIRAL)
            {
            sort_msg("CR: must be Admiral!");
            }
         else if (m_p->ship[vflag].dock != HW_DOCKED)
	    {
	    sort_msg("CR: must be docked at Homeworld!");
            }
         else
	    {
            call_back_robotship(com);
            }
         }

      }   while ( strncmp(m_p->cmd[vflag],"QUIT1234",8) != 0 && m_p->l_exit[vflag] == NO_EXIT); 
   }
   
/**************************************************************************/
/***                       Terminal zuruecksetzen, ENDE                 ***/
/**************************************************************************/

m_p->luser[vflag]=DESTROYED;
strcpy(m_p->name_of_cmdr[vflag],"                    ");
if (m_p->ship[vflag].pos[0]!=(-99))
   {
   if (m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]!=E_HOMEWORLD &&
       m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]!=F_HOMEWORLD &&
       m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]!=E_BASE &&
       m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]!=F_BASE)
       m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=SPACE;
   if (m_p->ship[vflag].dock == DOCKED)
     {
     nrb=find_nr_base(m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1]);
     if (m_p->base[nrb].bay[0] == ships[vflag][0]) m_p->base[nrb].bay[0]=' ';
     else if (m_p->base[nrb].bay[1] == ships[vflag][0]) m_p->base[nrb].bay[1]=' ';
     else if (m_p->base[nrb].bay[2] == ships[vflag][0]) m_p->base[nrb].bay[2]=' ';
     else if (m_p->base[nrb].bay[3] == ships[vflag][0]) m_p->base[nrb].bay[3]=' ';
     }
   m_p->ship[vflag].pos[0]=-99;
   m_p->ship[vflag].pos[1]=-99;   	
   }

m_p->ship[vflag].dock=HW_DOCKED;
m_p->anz_user--;
if (party[vflag] == FEDERAL) m_p->anz_user_f--;
if (party[vflag] == IMPERIAL) m_p->anz_user_e--;
kill(ppid1,SIGKILL);    /* Cockpit */
kill(ppid2,SIGKILL);    /* Move */
if (level_of_ship[vflag] != DESTROYER) kill(ppid4,SIGKILL); /* Phaser */

if (ppid5 > 1 && m_p->scout[party[vflag]].status==ACTIVE) 
   {
   kill(ppid5,SIGKILL); /* Scout */
   m_p->scout[party[vflag]].status=DESTROYED;
   m_p->a[m_p->scout[party[vflag]].pos[0]][m_p->scout[party[vflag]].pos[1]]=SPACE;
   m_p->scout[party[vflag]].pos[0]=-99;
   m_p->scout[party[vflag]].pos[1]=-99;
   m_p->scout[party[vflag]].target[0]=-99;
   m_p->scout[party[vflag]].target[1]=-99;
   }
fp=fopen("/tmp/dwuser.dat","r");
werr=0;
i_user=(-1);
recog=0;

if (fp == NULLF)
   {
   werr++;
   }
else
   {
   while ( !feof(fp) && i_user<MAX_NAMES)
      {
      i_user++;      	
      fscanf(fp,"%s %d",i_nam[i_user],&i_sc[i_user]);
      if (strcmp(cmdr_name,i_nam[i_user])==0)
	 {
         i_sc[i_user]=m_p->name_score[vflag];
         recog=1;
         }
      }
   fclose(fp);
   }

fp=fopen("/tmp/dwuser.dat","w");
if (fp == NULLF)
   {
   werr++;
   }
else
   {
   for (i=0;i<i_user;i++)
      {
      fprintf(fp,"%s %d\n",i_nam[i],i_sc[i]);
      }
/* Neuer User, noch nicht in der Datei */
   if (recog == 0)
      {
      fprintf(fp,"%s %d\n",cmdr_name,m_p->name_score[vflag]);
      }
   fclose(fp);
   }

echo();
endwin();
system("tput clear");
usleep(10000);
/* Schiff zerstoert? */
if (m_p->l_exit[vflag]==EXIT)
   {
   m_p->l_exist[vflag]=DESTROYED;
   printf("\n\n\n\n\n\n");
   printf("     ######            ###         ######\n");
   printf("     #     #            #          #     #\n");
   printf("     #     #            #          #     #\n");
   printf("     ######             #          ######\n");
   printf("     #   #              #          #\n");
   printf("     #    #    ##       #    ##    #     ##\n");
   printf("     #     #   ##      ###   ##    #     ##\n");
   printf("\n\n\n");
   printf("            Game over for %s!\n\n\n",ships[vflag]); 
   usleep(100000);
   }
(void) shmdt(m_p);
 if (werr == 0)
   {
   printf("\n\n Database updated!\n");   
   }
 else
   {
   printf("\n\n Could not update Database! user.dat does not exist or has wrong permissions!\n");   
   }
printf("\n\n\n DEKKWAR - finished! \n");
exit(0);
}
/***************************************************************************/
/***  ENDE dwmain.c                                                      ***/
/***************************************************************************/



void dw_logo()
/************************************************************************/
/***  Schreiben des Eingangslogos                                     ***/
/************************************************************************/
{
	
system("tput clear");
printf("\n");
printf("\n");
printf("\n");
printf("\n");
printf("\n");
printf("                            -------------------\n");
printf("             --------------------------------------------------\n");
printf("-----------------------------------------------------------------------------\n");
printf("             DDDDDDDD    EEEEEEEEE  KK    KK  KK    KK      \n");
printf("             DD      D   EE         KK   K    KK   K        \n");
printf("             DD       D  EE         KK  K     KK  K         \n");
printf("             DD       D  EE         KK K      KK K          \n");
printf("             DD       D  EEEEEEEE   KKK       KKK           \n");
printf("             DD       D  EE         KK K      KK K          \n");
printf("             DD       D  EE         KK  K     KK  K         \n");
printf("             DD      D   EE         KK   K    KK   K        \n");
printf("             DDDDDDDD    EEEEEEEEE  KK    KK  KK    KK    W A R    \n");
printf("-----------------------------------------------------------------------------\n");
printf("             --------------------------------------------------\n");
printf("                            -------------------\n");
printf("                    Copyright 1992-2015 by Thies Meincke\n");
printf("              (Entwicklungs-Version nach 1.5.2 fuer Ubuntu 10.04)\n");
printf("\n");

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

void hw_docking()
/*******************************************************/
/***           Docken an der Heimatwelt              ***/
/*******************************************************/
{
int i,i_sc[MAX_NAMES],i_user,recog;
char i_nam[MAX_NAMES][10];
int xd,yd;
FILE *fp, *fopen();

if (m_p->homeworld[party[vflag]].status==DESTROYED)
  {
  sort_msg("HW: ** Homeworld destroyed **");
  return;
  }

if (m_p->ship[vflag].move!=OFF)
   {
   sort_msg("HW: Warp engines must be stopped");
   return;
   }

/*** Abfrage, ob in Kontakt mit Heimatwelt **/
xd=abs(m_p->ship[vflag].pos[0]-m_p->homeworld[party[vflag]].pos[0]);
yd=abs(m_p->ship[vflag].pos[1]-m_p->homeworld[party[vflag]].pos[1]);


if ( xd > 1 || yd > 1 )
   {
   sort_msg("HW: ** Distance > 1 **");
   return;
   }
if (m_p->ship[vflag].hood==ON) 
   {
   sort_msg("HW: Hood must be OFF!");
   return;
   }

if (m_p->ship[vflag].mp==ON) 
   {
   sort_msg("HW: MP must be OFF!");
   return;
   }

if (m_p->homeworld[party[vflag]].dam > ((MAX_DAM_HOMEWORLD/10)*9))
  {
  sort_msg("HW: Homeworld severely damaged!");
  sort_msg("HW: Docking impossible!");
  return;
  }

sort_msg("HW:  DOCKING at HOMEWORLD!");
m_p->ship[vflag].dock=HW_DOCKED;
m_p->ship[vflag].shield=OFF;
m_p->ship[vflag].mp=OFF;
m_p->display_status[vflag]=DISPLAY_HW;
/* Vorraete auffuellen */
m_p->ship[vflag].torp=max_torp[vflag];
m_p->ship[vflag].sat=max_sat[vflag];
m_p->ship[vflag].mines=max_mines[vflag];
m_p->ship[vflag].main_energy=max_m_en[vflag];
m_p->ship[vflag].sh_energy=max_sh_en[vflag];
m_p->ship[vflag].ph_energy=max_ph_en[vflag];
m_p->ship[vflag].fuel=max_fuel[vflag];
m_p->ship[vflag].dam[0]=0; 
m_p->ship[vflag].dam[1]=0;
m_p->ship[vflag].dam[2]=0;
m_p->ship[vflag].dam[3]=0;
m_p->ship[vflag].dam[4]=0;
m_p->ship[vflag].dam[5]=0;
m_p->ship[vflag].dam[6]=0;
m_p->ship[vflag].dam[7]=0;
m_p->ship[vflag].dam[8]=0;
m_p->ship[vflag].dam[9]=0;
m_p->ship[vflag].dam[10]=0;
m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=a_alt;
a_alt=SPACE;
m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0];
m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1];


/* Scoring speichern */
fp=fopen("/usr/local/DEKKWAR/user.dat","r");
recog=0;
i_user=(-1); 
if (fp == NULLF)
   {
   sort_msg("Error user.dat!");
   }
else
   {
   while ( !feof(fp) && i_user<MAX_NAMES)
      {
      i_user++;
      fscanf(fp,"%s %d",i_nam[i_user],&i_sc[i_user]);
      if (strcmp(m_p->name_of_cmdr[vflag],i_nam[i_user])==0) 
	 { 
         i_sc[i_user]=i_sc[i_user]+m_p->user_score[vflag];
         m_p->user_score[vflag]=0;
         recog=1;
	 }
      }
   fclose(fp);
   }
fp=fopen("/usr/local/DEKKWAR/user.dat","w");
 if (fp == NULLF)
   {
     sort_msg("Error user.dat!");
   }
 else
   {
   for (i=0;i<i_user;i++)
       {
       fprintf(fp,"%s %d\n",i_nam[i],i_sc[i]);
       }
/* Neuer User, noch nicht in der Datei */
   if (recog == 0)
      {
      fprintf(fp,"%s %d\n",m_p->name_of_cmdr[vflag],m_p->name_score[vflag]);
      m_p->user_score[vflag]=0;
      }
   fclose(fp);
   sort_msg("HW: scored points saved!");
   }
alarm(2);
pause();

/* Befoerderung? */
if (m_p->rank[vflag]==COMMANDER && m_p->name_score[vflag] >= CAPTAIN_SC)
  {
  sort_msg("HW: Congratulation! You are"); 
  sort_msg("HW: promoted to the rank");
  sort_msg("HW:         Captain     ");
  m_p->rank[vflag]=CAPTAIN;
  strcpy(m_p->rank_of_name[vflag],"Captain");
  alarm(2);
  pause();
  }
if (m_p->rank[vflag]==CAPTAIN && m_p->name_score[vflag] >= COMMODORE_SC)
  {
  sort_msg("HW: Congratulation! You are"); 
  sort_msg("HW: promoted to the rank");
  sort_msg("HW:         Commodore     ");
  m_p->rank[vflag]=COMMODORE;
  strcpy(m_p->rank_of_name[vflag],"Commodore");
  alarm(2);
  pause();
  }
if (m_p->rank[vflag]==COMMODORE && m_p->name_score[vflag] >= ADMIRAL_SC)
  {
  sort_msg("HW: Congratulation! You are"); 
  sort_msg("HW: promoted to the rank");
  sort_msg("HW:         Admiral     ");
  m_p->rank[vflag]=ADMIRAL;
  strcpy(m_p->rank_of_name[vflag],"Admiral");
  alarm(2);
  pause();
  }

/* Degradierung? */
if (m_p->rank[vflag]==ADMIRAL && m_p->name_score[vflag] < ADMIRAL_SC)
  {
  sort_msg("HW: You are demoted to the rank");
  sort_msg("HW:         Commodore     ");
  m_p->rank[vflag]=COMMODORE;
  strcpy(m_p->rank_of_name[vflag],"Commodore");
  alarm(2);
  pause();
  }
if (m_p->rank[vflag]==COMMODORE && m_p->name_score[vflag] < COMMODORE_SC)
  {
  sort_msg("HW: You are demoted to the rank");
  sort_msg("HW:         Captain     ");
  m_p->rank[vflag]=CAPTAIN;
  strcpy(m_p->rank_of_name[vflag],"Captain");
  alarm(2);
  pause();
  }
if (m_p->rank[vflag]==CAPTAIN && m_p->name_score[vflag] < CAPTAIN_SC)
  {
  sort_msg("HW: You are demoted to the rank");
  sort_msg("HW:         Commander     ");
  m_p->rank[vflag]=COMMANDER;
  strcpy(m_p->rank_of_name[vflag],"Commander");
  alarm(2);
  pause();
  }

return;
}

void docking()
/**********************************************************/
/***              Docken an eine Basis                  ***/
/**********************************************************/
{
int del_time,nrb;

del_time=0;
/* Abfrage, ob in Kontakt mit Basis */
nrb=find_nr_base(m_p->ship[vflag].pos[0]-1,m_p->ship[vflag].pos[1]);
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;
if (nrb < 0) nrb=find_nr_base(m_p->ship[vflag].pos[0],  m_p->ship[vflag].pos[1]-1);
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;
if (nrb < 0) nrb=find_nr_base(m_p->ship[vflag].pos[0]+1,m_p->ship[vflag].pos[1]  );
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;
if (nrb < 0) nrb=find_nr_base(m_p->ship[vflag].pos[0],  m_p->ship[vflag].pos[1]+1);
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;
if (nrb < 0) nrb=find_nr_base(m_p->ship[vflag].pos[0]-1,m_p->ship[vflag].pos[1]-1);
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;
if (nrb < 0) nrb=find_nr_base(m_p->ship[vflag].pos[0]+1,m_p->ship[vflag].pos[1]+1);
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;
if (nrb < 0) nrb=find_nr_base(m_p->ship[vflag].pos[0]-1,m_p->ship[vflag].pos[1]+1);
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;
if (nrb < 0) nrb=find_nr_base(m_p->ship[vflag].pos[0]+1,m_p->ship[vflag].pos[1]-1);
if (nrb>=0 && m_p->base[nrb].party != party[vflag]) nrb=-1;

if (nrb < 0 || nrb >= MAX_BASES || m_p->base[nrb].party != party[vflag])
  {
  sort_msg("DO: *** no base! ***");
  return;
  }

if (m_p->ship[vflag].hood==ON) 
   {
   sort_msg("DO: Hood must be OFF!");
   return;
   }
if (m_p->ship[vflag].move!=OFF) 
   {
   sort_msg("DO: Warp eng. must be stopped!");
   return;
   }

if (m_p->ship[vflag].mp==ON) 
   {
   sort_msg("DO: MP must be OFF!");
   return;
   }

if (m_p->base[nrb].dam > ((MAX_DAM_BAS/10)*9))
   {
   sort_msg("DO: Base is severely damaged!");
   sort_msg("    Docking impossible!");
   return;
   }

/* Bay frei? */
if (m_p->base[nrb].bay[0]==' ') 
  {   
  m_p->base[nrb].bay[0]=ships[vflag][0];
  }
else if (m_p->base[nrb].bay[1]==' ')
  {   
  m_p->base[nrb].bay[1]=ships[vflag][0];
  }
else if (m_p->base[nrb].bay[2]==' ')
  {   
  m_p->base[nrb].bay[2]=ships[vflag][0];
  }
else if (m_p->base[nrb].bay[3]==' ')
  {   
  m_p->base[nrb].bay[3]=ships[vflag][0];
  }
else 
  {
  sort_msg("DO: Failed! All docking");
  sort_msg("bays occupied or damaged!");
  return;
  }

/*** Docken ***/
sort_msg("DO:  DOCKING at BASE!");
m_p->base[nrb].sh=OFF;
m_p->ship[vflag].dock=DOCKED;
m_p->display_status[vflag]=DISPLAY_BASE;
/* m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=DOCKING; */
m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=a_alt;
m_p->ship[vflag].shield=OFF;
if (m_p->ship[vflag].mp==ON)
  {
  sort_msg("DO: Warning: MP set to OFF!");
  sort_msg("    Path recording finished!");
  sort_msg("    Path most likely useless!");
  m_p->ship[vflag].imp=0;
  m_p->ship[vflag].mp=OFF;
  }
m_p->ship[vflag].pos[0]=m_p->base[nrb].pos[0];
m_p->ship[vflag].pos[1]=m_p->base[nrb].pos[1];

/** Werte auffuellen */
alarm(1);
pause();
m_p->ship[vflag].torp=max_torp[vflag];
m_p->ship[vflag].sat=max_sat[vflag];
m_p->ship[vflag].mines=max_mines[vflag];
m_p->ship[vflag].main_energy=max_m_en[vflag];
m_p->ship[vflag].sh_energy=max_sh_en[vflag];
m_p->ship[vflag].ph_energy=max_ph_en[vflag];
m_p->ship[vflag].fuel=max_fuel[vflag];
if (m_p->base[nrb].sh_energy > 0) m_p->base[nrb].sh=ON;
sort_msg("DO:Supplies filled up:E,T,M,S,F");

/** Eventuelle Schaeden reparieren **/
if (m_p->ship[vflag].dam[0] > 100)
   {
   del_time=m_p->ship[vflag].dam[0]/100;
   sort_msg("DO:   Repairing damages...  ");
   alarm(del_time);
   pause();
   sort_msg("DO:     Damages repaired!!  ");
   }
m_p->ship[vflag].dam[0]=0; 
m_p->ship[vflag].dam[1]=0;
m_p->ship[vflag].dam[2]=0;
m_p->ship[vflag].dam[3]=0;
m_p->ship[vflag].dam[4]=0;
m_p->ship[vflag].dam[5]=0;
m_p->ship[vflag].dam[6]=0;
m_p->ship[vflag].dam[7]=0;
m_p->ship[vflag].dam[8]=0;
m_p->ship[vflag].dam[9]=0;
m_p->ship[vflag].dam[10]=0;

a_alt=SPACE;

alarm(2);
pause();

return;
}

void jump(com)
     char com[];
/**********************************************************/
/***          Flug durch ein Sprungtor                  ***/
/**********************************************************/
{
int zae,i,nrjs,nrjd,codenr_start,codenr_dest,dx,dy,xalt,yalt;
int display_status_old;
float random;
char cm[2];

/* Kommandostringauswertung: JU code_start code_dest */
if ( sscanf(com,"%s%d%d",cm,&codenr_start,&codenr_dest) != 3)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }

nrjs=nrjd=-1;
for (i=0;i<MAX_JUMPGATES;i++)
   {
   if (m_p->jumpgate[i].code==codenr_start) nrjs=i;
   if (m_p->jumpgate[i].code==codenr_dest) nrjd=i;
   }

if (nrjs==-1 || nrjd==-1 || nrjs==nrjd)
   {
   sort_msg("JU: Wrong jumpgate code(s)!");
   return;
   }

/* Abfrage, ob in Kontakt mit Sprungtor */
dx=(IABS((m_p->ship[vflag].pos[0]-m_p->jumpgate[nrjs].pos[0])));
dy=(IABS((m_p->ship[vflag].pos[1]-m_p->jumpgate[nrjs].pos[1])));
if ( dx > 1 || dy > 1 )
   {
   sort_msg("JU: Jumpgate too far away!");
   return;
   }

if (m_p->ship[vflag].hood==ON) 
   {
   sort_msg("JU: Hood must be OFF!");
   return;
   }

if (m_p->ship[vflag].mp==ON) 
   {
   sort_msg("JU: MP must be set to OFF!");
   return;
   }

if (m_p->ship[vflag].move!=OFF) 
   {
   sort_msg("JU: Warp eng. must be stopped!");
   return;
   }

if (m_p->jumpgate[nrjs].dam>1000 || m_p->jumpgate[nrjd].dam>1000)
  {
  sort_msg("JU: Jumpgate(s) damaged!");
  return; /* Moeglichkeit: etwas Besonderes geschieht bei def. Jumpgates! */
  }

if (m_p->ship[vflag].main_energy < 500)
   {
   sort_msg("JU: Too less main energy!");
   return;
   }

/* Sprung */

sort_msg("JU: jump sequence initiated!");
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-500;
m_p->ship[vflag].jump=ON;
display_status_old=m_p->display_status[vflag];
m_p->display_status[vflag]=DISPLAY_JUMP;
m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=SPACE;
xalt=m_p->ship[vflag].pos[0];
yalt=m_p->ship[vflag].pos[1];
m_p->ship[vflag].pos[0]=-99;
m_p->ship[vflag].pos[1]=-99;
sort_msg("   ... jumping ...   ");
usleep(JUMP_DURATION);
/* In der Wartezeit erzeugt das Cockpit das Jump-Bild! */
/* Startposition */
zae=0;
i=-1;
do {
   random=ran();
   i--;
   /* Im Uhrzeigersinn um die Homeworld herum, 8 Moeglichkeiten! */
   zae++;
   if (random < 0.125) 
     {	
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]-1][m_p->jumpgate[nrjd].pos[1]+1] == SPACE) /* l.o. */
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1]+1;
        }
     }
   else if (random >= 0.125 && random < 0.25)
     {	
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]-1][m_p->jumpgate[nrjd].pos[1]] == SPACE) /* l. */
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1];
        }
     }
   else if (random >= 0.25 && random < 0.375)
     {	
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]-1][m_p->jumpgate[nrjd].pos[1]-1] == SPACE) /* l.u. */ 
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1]-1;
        }
     }
   else if (random >= 0.375 && random < 0.5)
     {	
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]][m_p->jumpgate[nrjd].pos[1]-1] == SPACE) /* u. */
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0];
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1]-1;
        }
     }
   else if (random >= 0.5 && random < 0.625)
     {
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]+1][m_p->jumpgate[nrjd].pos[1]-1] == SPACE) /* r.u. */
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1]-1;
        }	
     }
   else if (random >= 0.625 && random < 0.75)
     {	
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]+1][m_p->jumpgate[nrjd].pos[1]] == SPACE) /* r. */
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1];
        }
     }
   else if (random >= 0.75 && random < 0.875)
     {	
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]+1][m_p->jumpgate[nrjd].pos[1]+1] == SPACE) /* r.o. */
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1]+1;
        }
     }
   else
     {	
     if (m_p->a[m_p->jumpgate[nrjd].pos[0]][m_p->jumpgate[nrjd].pos[1]+1] == SPACE) /* o. */
	{
        m_p->ship[vflag].pos[0]=m_p->jumpgate[nrjd].pos[0];
        m_p->ship[vflag].pos[1]=m_p->jumpgate[nrjd].pos[1]+1;
        }
     }
  usleep(100000);
  } while (m_p->ship[vflag].pos[0]==-99 && m_p->ship[vflag].pos[1]==-99 && zae <= 99);

if (zae > 99 || m_p->jumpgate[nrjd].status != ACTIVE) 
  {
  sort_msg("Cannot use exit jumpgate!");
  sort_msg("Jump sequence aborted!");
  sort_msg("Check condition of destination");
  sort_msg("jumpgate (destroyed,circled?)");
  m_p->ship[vflag].pos[0]=xalt;
  m_p->ship[vflag].pos[1]=yalt;
  }
else
  {
  sort_msg("JU: Destination Jumpgate left!");
  }

m_p->ship[vflag].jump=OFF;
m_p->display_status[vflag]=display_status_old;
m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];

return; 
}

void shields_up()
/*********************************************************************/
/**  Heben der Schilde                                             ***/
/*********************************************************************/
{
	
if (m_p->ship[vflag].shield == ON)
   {
   sort_msg("SH: Shields already raised!");
   return;
   }
if (m_p->ship[vflag].hood == ON)
   {
   sort_msg("SH: Hood must be off!");
   return;
   }
if (m_p->ship[vflag].main_energy < 101)
   {
   sort_msg("SH:  Too less Main Energy!");
   return;
   }
if (m_p->ship[vflag].sh_energy <= 0)
   {
   sort_msg("SH: Too less Shield Energy!");
   return;
   }
if (m_p->ship[vflag].dam[5] > 200)
   {
   sort_msg("SH: *** Shields damaged ***");
   return;
   }

alarm(2);
pause();
sort_msg("SH:  *** Shields raised ***");  
m_p->ship[vflag].shield=ON;
m_p->ship[vflag].main_energy=(m_p->ship[vflag].main_energy-100);
return;
}


void shields_down()
/*********************************************************************/
/**  Heben der Schilde                                             ***/
/*********************************************************************/
{
	
if (m_p->ship[vflag].shield == OFF)
   {
   sort_msg("SH:   Shields already down!");
   }
else
   {
   m_p->ship[vflag].shield=OFF;
   sort_msg("SH:   *** Shields down! ***");
   }
return;
}


void set_mine(com)
     char com[];
/************************************************************************/
/***      Setzen einer Mine                                           ***/
/************************************************************************/
{
int x,y;
char cm[2],pos_str[10],print_str[MSG_LENGTH];

/** Ueberpruefung Kommandozeile **/
if (sscanf(com,"%s%d%d",cm,&x,&y) != 3)
   {
   sort_msg("     *** BAD COMMAND ***");
   return;
   }

if (m_p->ship[vflag].hood == ON)
   {
   sort_msg("MI: Hood must be off!");
   return;
   }

/** Ueberpreufung, ob noch Minen an Bord */
if (m_p->ship[vflag].mines<=0)
   {
   sort_msg("MI: No more mines on board!");
   return;
   }

/* Position innerhalb Spielfeld? */
if (x<0 || y<0 || x >= ARRAY_SIZE || y >= ARRAY_SIZE)
   {
   sort_msg("MI: Wrong coordinates!");
   return;
   }

/** Ueberpuefung Abstand=1 **/
if ( (IABS((x - m_p->ship[vflag].pos[0]))) == 0 && (IABS((y - m_p->ship[vflag].pos[1]))) == 0)
   {
   sort_msg("MI: Our own position!");
   return;
   }
if ( (IABS((x - m_p->ship[vflag].pos[0]))) > 1 || (IABS((y - m_p->ship[vflag].pos[1]))) > 1)
   {
   sort_msg("MI: Distance=1 required!");
   return;
   }

/** Uberpruefung, ob x,y leerer Raum **/
sprintf(pos_str," %d %d ",x,y);
if (m_p->a[x][y] != SPACE)
   {
   strcpy(print_str,"MI: No free space at");
   strcat(print_str,pos_str);
   sort_msg(print_str);
   return;
   }

/** alles o.k. **/
alarm(2);
pause();
m_p->ship[vflag].mines--;
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-5;
m_p->user_score[vflag]=m_p->user_score[vflag]+5;
m_p->score[party[vflag]]=m_p->score[party[vflag]]+5; 
m_p->name_score[vflag]=m_p->name_score[vflag]+5; 
m_p->a[x][y]=MINE;
strcpy(print_str,"MI: Mine placed at");
strcat(print_str,pos_str);
sort_msg(print_str);
return;
}


void set_sat(com)
     char com[];
/******************************************************************************/
/***      Setzen eines Satelliten                                           ***/
/******************************************************************************/
{
int x,y,nrs,i;
char cm[2],pos_str[10],print_str[MSG_LENGTH];

/** Ueberpruefung Kommandozeile **/
if (sscanf(com,"%s%d%d",cm,&x,&y) != 3)
   {
   sort_msg("     *** BAD COMMAND ***");
   return;
   }

/** Ueberpruefung ob Schilde unten **/
if (m_p->ship[vflag].shield == ON)
   {
   sort_msg("SA: Shields must be down!");
   return;
   }
if (m_p->ship[vflag].hood == ON)
   {
   sort_msg("SA: Hood must be off!");
   return;
   }

/** Ueberpreufung, ob noch Satelliten an Bord */
if (m_p->ship[vflag].sat<=0)
   {
   sort_msg("SA: No more satellites !");
   return;
   }

/* Position innerhalb Spielfeld? */
if (x<0 || y<0 || x >= ARRAY_SIZE || y >= ARRAY_SIZE)
   {
   sort_msg("SA: Wrong coordinates!");
   return;
   }

/** Ueberpuefung Abstand=1 **/
if ( (IABS((x - m_p->ship[vflag].pos[0]))) == 0 && (IABS((y - m_p->ship[vflag].pos[1]))) == 0)
   {
   sort_msg("SA: Our own position!");
   return;
   }
if ( (IABS((x - m_p->ship[vflag].pos[0]))) > 1 || (IABS((y - m_p->ship[vflag].pos[1]))) > 1)
   {
   sort_msg("SA: Distance=1 required!");
   return;
   }

/** Uberpruefung, ob x,y leerer Raum **/
sprintf(pos_str," %d %d ",x,y);
if (m_p->a[x][y] != SPACE)
   {
   strcpy(print_str,"SA: No free space at");
   strcat(print_str,pos_str);
   sort_msg(print_str);
   return;
   }

nrs=MAX_SATS;
for (i=0; i<MAX_SATS; i++)
   {
   if (m_p->satellite[i].party==NEUTRAL && m_p->satellite[i].pos[0]==-99 && m_p->satellite[i].pos[1]==-99 ) nrs=i;
   }
if (nrs == MAX_SATS)
   {
   sort_msg("SA: Already 199 sats in game!");
   return;
   }

/** alles o.k. **/

m_p->ship[vflag].sat--;
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-5;
m_p->user_score[vflag]=m_p->user_score[vflag]+5;
m_p->score[party[vflag]]=m_p->score[party[vflag]]+5; 
m_p->name_score[vflag]=m_p->name_score[vflag]+5; 

m_p->satellite[nrs].pos[0]=x;
m_p->satellite[nrs].pos[1]=y;

if (party[vflag]==FEDERAL)
   {
   m_p->a[x][y]=F_SAT;
   }
else
   {
   m_p->a[x][y]=E_SAT;
   }
m_p->satellite[nrs].party=party[vflag];

alarm(2);
pause();

strcpy(print_str,"SA: Satellite placed at");
sprintf(pos_str," %d %d",x,y);
strcat(print_str,pos_str);
sort_msg(print_str);

return;
}

void send(com)
     char com[];
/**********************************************************************/
/***            Senden einer Nachricht                              ***/
/**********************************************************************/

{
char cc,m_str[MSG_LENGTH];
int i,zflag;
	
if (m_p->ship[vflag].dam[9] > 250)
   {
   sort_msg("SE:  *** Radio out of order ***");
   return;
   }
else
   {
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-6;
   }
   
strncpy(m_str,com,MSG_LENGTH);
m_str[0]='M';
m_str[1]='S';
m_str[2]='G';
m_str[3]='>';

/** Interpretieren des Kommandostrings **/
if (strncmp(com,"SE *",4) == 0)   /* An alle */
   {
   for (i=1;i<MAX_USER;i++)
       {
       if (i != vflag && m_p->luser[i] == ACTIVE) sort_send_msg(i,m_str); 
       }
   sort_msg("SE: Message submitted (to all)!");
   return;
   }
if (strncmp(com,"SE &",4) == 0)   /* An eigene Partei */
   {
   for (i=1;i<MAX_USER;i++)
       {
       if (   i != vflag 
                && 
              m_p->luser[i] == ACTIVE
                && 
              party[i] == party[vflag] ) sort_send_msg(i,m_str); 
       }
   sort_msg("SE:Message submitted (our party)");
   return;
   }
cc=com[3];   /* 4. Buchstabe entscheidet ueber Ziel */
zflag=det_target(cc);
if (zflag<0 || zflag>=MAX_USER || zflag==vflag) 
   {
   sort_msg("SE: *** Wrong address ***");
   return;
   }
if ( m_p->luser[zflag] == ACTIVE)
   {
   sort_send_msg(zflag,m_str);
   sort_msg("SE: Message submitted!");
   }
else
   {
   sort_msg("SE:   Not in game!");   	
   }
return;
}

void sort_send_msg(zflag,new_msg)
/*****************************************************************************/
/***      sort_msg: bereitet die msg-Strings des Users vflag so auf, dass  ***/
/**                 new_msg hinten (im Cockpit-Window unten) steht.        ***/
/*****************************************************************************/
int zflag;
char new_msg[MSG_LENGTH];
{
int i;
char origin_str[MSG_LENGTH];
char xmsg[MSG_LENGTH];

strcpy(xmsg,new_msg);

for (i=0;i<6;i++)
    { 
    strncpy(m_p->ship[zflag].msg[i],m_p->ship[zflag].msg[(i+1)],MSG_LENGTH);
    }  
sprintf(origin_str,">>> Message received from %c <<<",ships[vflag][0]);
strncpy(m_p->ship[zflag].msg[6],origin_str,MSG_LENGTH);  

for (i=0;i<6;i++)
    { 
    strncpy(m_p->ship[zflag].msg[i],m_p->ship[zflag].msg[(i+1)],MSG_LENGTH);
    }  
strncpy(m_p->ship[zflag].msg[6],xmsg,MSG_LENGTH);  
return;
}


void show_damages()
/*************************************************************************/
/** Auflistung der Schaeden in allen Abteilungen                        **/
/*************************************************************************/
{
char dam_str[MSG_LENGTH];

sort_msg("============DAMAGES=============");
sprintf(dam_str,"1:W-Eng. : %04d 2:I-Eng. : %04d",m_p->ship[vflag].dam[1],m_p->ship[vflag].dam[2]);
sort_msg(dam_str);
if (vflag>=25)
   {
   sprintf(dam_str,"3:Torp.T.: %04d",m_p->ship[vflag].dam[3]);
   sort_msg(dam_str);
   }
else
   {
   sprintf(dam_str,"3:Torp.T.: %04d 4:Phaser : %04d",m_p->ship[vflag].dam[3],m_p->ship[vflag].dam[4]);
   sort_msg(dam_str);
   }
sprintf(dam_str,"5:Shields: %04d 6:Comp.  : %04d",m_p->ship[vflag].dam[5],m_p->ship[vflag].dam[6]);
sort_msg(dam_str);
sprintf(dam_str,"7:Life-S.: %04d 8:Radar  : %04d",m_p->ship[vflag].dam[7],m_p->ship[vflag].dam[8]);
sort_msg(dam_str);
sprintf(dam_str,"9:Radio  : %04d 10:Hood  : %04d",m_p->ship[vflag].dam[9],m_p->ship[vflag].dam[10]);
sort_msg(dam_str);
sort_msg("================================");
return;
}


void show_score()
/*************************************************************************/
/** Auflistung der Schaeden in allen Abteilungen                        **/
/*************************************************************************/
{
char score_str[MSG_LENGTH];

sort_msg("=============SCORE==============");
sprintf(score_str,"Player :%7d Ship  : %7d",m_p->name_score[vflag],m_p->user_score[vflag]);
sort_msg(score_str);
sprintf(score_str,"Fed.   :%7d Empire: %7d",m_p->score[FEDERAL],m_p->score[IMPERIAL]);
sort_msg(score_str);
sprintf(score_str,"Romulan:%7d",m_p->user_score[0]);
sort_msg(score_str);
sort_msg("================================");
return;
}

void identify(object)
     char object;
/**************************************************************************/
/***            Identifiziert object und gibt Informationen aus         ***/
/**************************************************************************/
{
int zflag;
char print_str[MSG_LENGTH];
	
zflag=det_target(object);
/* Kein Schiff */
if (zflag < 0  || zflag >= MAX_USER)
   {
   sprintf(print_str,"ID: %c is unknown!",object);
   sort_msg(print_str);
   return;
   }
/* Romulaner */
if (zflag == 0) 
   {
   sprintf(print_str,"ID: %c: Romulan Battleship",object);
   sort_msg(print_str);
   return;
   }
/* "normale" Schiffe */
sprintf(print_str,"ID: %c successfully identified:",object);
sort_msg(print_str);
sprintf(print_str,"%s %s",kind_of_ship[zflag],ships[zflag]);
sort_msg(print_str);

/* nicht im Spiel */
 if ( party[vflag]==party[zflag] ) /* gleiche Partei: weitere Infos */
  {
  /* vernichtet */
  if (m_p->l_exist[zflag]==DESTROYED)
     {
     sort_msg("destroyed!");
     return;
     }
  if (m_p->luser[zflag]==DESTROYED) /* nicht im Spiel */
     {
     sort_msg("operational");
     return;
     }
  if (m_p->luser[zflag]==ACTIVE) 
     {
     sprintf(print_str,"active: %s %s",m_p->rank_of_name[zflag],m_p->name_of_cmdr[zflag]);
     sort_msg(print_str);
     if (m_p->rank[vflag]==COMMODORE || m_p->rank[vflag]==ADMIRAL)
        {
	sprintf(print_str,"current position: %03d %03d",m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1]);
        sort_msg(print_str);
        }
     }

  }

return;
}

void info(com)
     char com[];
/****************************************************************************/
/*** Information ueber Basen, Planeten, Forts,Heimatwelt,Scout,Sprungtore ***/
/****************************************************************************/
{
int i,nbas,nbas_r,npl,nfort,bxd,byd,njg,ir;
int next_base,nr_next_base_1,nr_next_base_2,enemy;
char cc,pr_str[MSG_LENGTH],sum_str[MSG_LENGTH];
char str1[MSG_LENGTH],str2[MSG_LENGTH/2];

/*************/
/* Homeworld */
/*************/

if (com[3] == 'h' || com[3] == 'H')
   {
   enemy=IMPERIAL;
   if (party[vflag]==IMPERIAL) enemy=FEDERAL;
   if (m_p->homeworld[party[vflag]].status==ACTIVE)
     {
     sprintf(pr_str,"Homeworld at %03d %03d,D=%5d",
             m_p->homeworld[party[vflag]].pos[0],
             m_p->homeworld[party[vflag]].pos[1],
             m_p->homeworld[party[vflag]].dam);
     sort_msg(pr_str);
     }
   else
     {
     sort_msg("IN: HOMEWORLD DESTROYED!");
     }
   if (m_p->homeworld[enemy].status==ACTIVE)
     {
     if (enemy==FEDERAL)
       {  
       sprintf(pr_str,"Federal Homeworld at %03d %03d",
               m_p->homeworld[enemy].pos[0],
	       m_p->homeworld[enemy].pos[1]);
       }
     else
       {
       sprintf(pr_str,"Imperial Homeworld at %03d %03d",
               m_p->homeworld[enemy].pos[0],
	       m_p->homeworld[enemy].pos[1]);
       }
     sort_msg(pr_str);
     }
   else
     {
     if (enemy==FEDERAL)
       {
       sort_msg("Federal Homeworld destroyed!");
       }
     else
       {
       sort_msg("Imperial Homeworld destroyed!");
       }
     }
   return;
   }

/****************/
/* Robotschiffe */
/****************/
if (strlen(com) == 4 && (com[3] == 'r' || com[3] == 'R'))
   {
   for (i=0;i<(MAX_ROBOTSHIPS-1);i=i+2)
      {
      if (m_p->robotship[party[vflag]][i].status==HW_DOCKED) /* ungerades i */
        {
        sprintf(str1,"R %d:  o   | ",i);
        }
      else if (m_p->robotship[party[vflag]][i].status==ACTIVE)
        {
	sprintf(str1,"R %d:  %c   | ",i,m_p->robotship[party[vflag]][i].mission);
        }
      else if (m_p->robotship[party[vflag]][i].status==WAITING)
        {
	sprintf(str1,"R %d:  w   | ",i);
        }
      else if (m_p->robotship[party[vflag]][i].status==UNDER_CONSTRUCTION)
        {
	sprintf(str1,"R %d: uc   | ",i);
        }
      else if (m_p->robotship[party[vflag]][i].status==DESTROYED)
        {
        sprintf(str1,"R %d:  d   | ",i);
        }
      else
        {
        sprintf(str1,"R %d:  ?   | ",i);
        }
      if (m_p->robotship[party[vflag]][i+1].status==HW_DOCKED)  /* gerades i */
        {
        sprintf(str2," R %d:  o",i+1);
        }
      else if (m_p->robotship[party[vflag]][i+1].status==ACTIVE)
        {
	sprintf(str2," R %d:  %c ",i+1,m_p->robotship[party[vflag]][i+1].mission);
        }
      else if (m_p->robotship[party[vflag]][i+1].status==WAITING)
        {
	sprintf(str2," R %d:  w",i+1);
        }
      else if (m_p->robotship[party[vflag]][i+1].status==UNDER_CONSTRUCTION)
        {
	sprintf(str2," R %d: uc",i+1);
        }
      else if (m_p->robotship[party[vflag]][i+1].status==DESTROYED)
        {
        sprintf(str2," R %d:  d",i+1);
        }
      else
        {
        sprintf(str2," R %d:  ?",i+1);
        }
      strcat(str1,str2);
      sort_msg(str1);
      }
   return;
   }
else if (strlen(com) == 6 && (com[3] == 'r' || com[3] == 'R'))
   {
   ir=-1;
   if (com[5] == '0') ir=0;
   if (com[5] == '1') ir=1;
   if (com[5] == '2') ir=2;
   if (com[5] == '3') ir=3;
   if (com[5] == '4') ir=4;
   if (com[5] == '5') ir=5;
   if (com[5] == '6') ir=6;
   if (com[5] == '7') ir=7;
   if (com[5] == '8') ir=8;
   if (com[5] == '9') ir=9;
   if (ir != -1)
     {
     if (m_p->robotship[party[vflag]][ir].status==HW_DOCKED) 
       {
       sprintf(str1,"IN: RS No. %d: Operational",ir);
       sort_msg(str1);
       }
     else if (m_p->robotship[party[vflag]][ir].status==DESTROYED) 
       {
       sprintf(str1,"IN: RS No. %d: Destroyed",ir);
       sort_msg(str1);
       }
     else if (m_p->robotship[party[vflag]][ir].status==UNDER_CONSTRUCTION) 
       {
       sprintf(str1,"IN: RS No. %d: Under construction",ir);
       sort_msg(str1);
       }
     else if (m_p->robotship[party[vflag]][ir].status==WAITING) 
       {
       sprintf(str1,"IN: RS No. %d: Pos. %03d %03d",ir,m_p->robotship[party[vflag]][ir].pos[0],m_p->robotship[party[vflag]][ir].pos[1]);
       sort_msg(str1);
       sprintf(str1,"WAIT., E=%d D=%d F=%d T=%d",m_p->robotship[party[vflag]][ir].main_energy,m_p->robotship[party[vflag]][ir].dam,m_p->robotship[party[vflag]][ir].fuel,m_p->robotship[party[vflag]][ir].torp);
       sort_msg(str1);
       }
     else if (m_p->robotship[party[vflag]][ir].status==ACTIVE) 
       {
       sprintf(str1,"IN: RS No. %d: Pos. %03d %03d",ir,m_p->robotship[party[vflag]][ir].pos[0],m_p->robotship[party[vflag]][ir].pos[1]);
       sort_msg(str1);
       sprintf(str1,"active, E=%d D=%d F=%d T=%d",m_p->robotship[party[vflag]][ir].main_energy,m_p->robotship[party[vflag]][ir].dam,m_p->robotship[party[vflag]][ir].fuel,m_p->robotship[party[vflag]][ir].torp);
       sort_msg(str1);
       }
     else
       {
       sort_msg("IN r: status error");
       }
     }
   else
     {
     sort_msg("IN: Error: wrong input");
     }
   return;
   }
/*********/
/* Basen */
/*********/

if (com[3] == 'b' || com[3] == 'B')
   {
   nr_next_base_1=nr_next_base_2=-1;
   next_base=ARRAY_SIZE+1;
   nbas_r=nbas=0;
   for (i=0;i<MAX_BASES;i++)
       {
       if (m_p->base[i].party == party[vflag] 
              && 
           m_p->base[i].status==ACTIVE)
          {
          bxd=(IABS((m_p->base[i].pos[0] - m_p->ship[vflag].pos[0]))); 
          byd=(IABS((m_p->base[i].pos[1] - m_p->ship[vflag].pos[1])));           
          if (bxd<next_base && byd<next_base)
             {
             next_base=(IMAX(bxd,byd));
             nr_next_base_1=i;      	
             }
          nbas++;
          if ((IABS((m_p->base[i].pos[0] - m_p->ship[vflag].pos[0]))) <= SHIP_RANGE
                               && 
              (IABS((m_p->base[i].pos[1] - m_p->ship[vflag].pos[1]))) <= SHIP_RANGE)
             {
             if ((m_p->radarlee==ON && behind(m_p->base[i].pos[0],m_p->base[i].pos[1],m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1],SHIP_RANGE)==0) 
                 || 
		 (m_p->radarlee==OFF))
	        {
                nbas_r++;
                sprintf(pr_str,"Base at %03d %03d, S=%4d D=%4d",
                                                        m_p->base[i].pos[0],
                                                        m_p->base[i].pos[1],
                                                        m_p->base[i].sh_energy,
                                                        m_p->base[i].dam);
                sort_msg(pr_str);
                if ((nbas_r%7) == 0) 
                   {
                   alarm(2);
                   pause();
                   }
                }
	     }                
          }
       }
   sprintf(sum_str,"IN: Total: %3d bases detected",nbas);
   sort_msg(sum_str);
   if (nbas > 0) sprintf(sum_str,"IN: Next base at %03d %03d",m_p->base[nr_next_base_1].pos[0],m_p->base[nr_next_base_1].pos[1]);
   sort_msg(sum_str);
   if (nbas > 1)
      {
      next_base=ARRAY_SIZE+1;
      for (i=0;i<MAX_BASES;i++)
         {
         if (m_p->base[i].party == party[vflag] 
              && 
             i != nr_next_base_1
              &&
             m_p->base[i].status==ACTIVE)
            {
            bxd=(IABS((m_p->base[i].pos[0] - m_p->ship[vflag].pos[0]))); 
            byd=(IABS((m_p->base[i].pos[1] - m_p->ship[vflag].pos[1])));           
            if (bxd<next_base && byd<next_base)
               {
               next_base=(IMAX(bxd,byd));
               nr_next_base_2=i;      	
               }
            }                
         }
      sprintf(sum_str,"IN: 2nd next base at %03d %03d",
               m_p->base[nr_next_base_2].pos[0],
               m_p->base[nr_next_base_2].pos[1]);
      sort_msg(sum_str);
      }
   return;
   }
   
/***************************************************/
/* Sprungtore                                      */
/***************************************************/
if (com[3] == 'j' || com[3] == 'J')
   {
   njg=0;
   if (party[vflag]==IMPERIAL)
     {
     sort_msg("IN J: Imperial Jumpgate(s):");
     }     
   else
     {
     sort_msg("IN J: Federal Jumpgate(s):");
     }
   for (i=0;i<MAX_JUMPGATES;i++)
       {
       if (m_p->jumpgate[i].party==party[vflag] 
           && m_p->jumpgate[i].status==ACTIVE)
	 {
	 njg++;
	 sprintf(sum_str,"Pos.: %03d %03d,Code=%d,Dam.=%d",m_p->jumpgate[i].pos[0],
                 m_p->jumpgate[i].pos[1],m_p->jumpgate[i].code,
                 m_p->jumpgate[i].dam);
         sort_msg(sum_str);
         }
       }
   sprintf(sum_str,"IN J: Total: %d jumpgate(s)",njg);
   sort_msg(sum_str);
   return;
   }

/***************************************************/
/* Planeten, genaue Inf. nur fuer Pl.in Reichweite */
/***************************************************/
if (com[3] == 'p' || com[3] == 'P')
   {
   npl=0;
   for (i=0;i<MAX_PLANETS;i++)
       {
       if ( (IABS((m_p->planet[i].pos[0]-m_p->ship[vflag].pos[0]))) <= SHIP_RANGE
                               &&
            (IABS((m_p->planet[i].pos[1]-m_p->ship[vflag].pos[1]))) <= SHIP_RANGE
                               &&
            m_p->planet[i].status==ACTIVE)
          {
          if ((m_p->radarlee==ON && behind(m_p->planet[i].pos[0],m_p->planet[i].pos[1],m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1],SHIP_RANGE)==0) 
               || 
	    (m_p->radarlee==OFF))
	     {
             cc='?';
             npl++;
             if (m_p->planet[i].party == FEDERAL) cc='F';          
             if (m_p->planet[i].party == NEUTRAL) cc='N';
             if (m_p->planet[i].party == IMPERIAL) cc='E';
             sprintf(pr_str,"IN: Planet at %03d %03d, L=%1d P=%c",
                                                      m_p->planet[i].pos[0],
                                                      m_p->planet[i].pos[1],
                                                      m_p->planet[i].level,
                                                      cc);
             sort_msg(pr_str);
             if ((npl%7) == 0) 
                {
                alarm(1);
                pause();
                }
             }
          }
       }
   /* +1 wegen Start bei 0 */
   sprintf(sum_str,"IN: Total: %3d planets detected",npl);
   sort_msg(sum_str);
   return;
   }

/***********************************************/
/* Forts, genaue Inf. ueber Forts in Reichweite */
/***********************************************/
if (com[3] == 'f' || com[3] == 'F')
   {
   nfort=0;
   for (i=0;i<MAX_FORTS;i++)
       {
       if ( m_p->fort[i].party == party[vflag])
          {
          nfort++;
          if (  (IABS((m_p->fort[i].pos[0]-m_p->ship[vflag].pos[0]))) <= SHIP_RANGE
                        &&
                (IABS((m_p->fort[i].pos[1]-m_p->ship[vflag].pos[1]))) <= SHIP_RANGE
                        &&
		m_p->fort[i].status == ACTIVE)
             {
             if ((m_p->radarlee==ON && behind(m_p->fort[i].pos[0],m_p->fort[i].pos[1],m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1],SHIP_RANGE)==0) 
                || 
	     (m_p->radarlee==OFF))
	        {
                sprintf(pr_str,"IN: Fort at %03d %03d, Damage=%4d",
                                                        m_p->fort[i].pos[0],
                                                        m_p->fort[i].pos[1],
                                                        m_p->fort[i].dam);
                sort_msg(pr_str);
                if ((nfort%7) == 0) 
                   {
                   alarm(2);
                   pause();
                   }
                }
             }
          }
       }
   sprintf(sum_str,"IN: Total: %3d forts detected",nfort);
   sort_msg(sum_str);
   return;
   }

/*********/
/* Scout */
/*********/
if (com[3] == 's' || com[3] == 'S')
   {
   if (m_p->scout[party[vflag]].status == DESTROYED)
     {
     sort_msg("IN: Scout is not active!");
     }
   else
     {
     sort_msg("IN: Scout is active:");
     sprintf(sum_str,"  Position %03d %03d",m_p->scout[party[vflag]].pos[0]
                                         ,m_p->scout[party[vflag]].pos[1]);
     sort_msg(sum_str);
     sprintf(sum_str,"  Target   %03d %03d",m_p->scout[party[vflag]].target[0]
                                         ,m_p->scout[party[vflag]].target[1]);
     sort_msg(sum_str);
     sprintf(sum_str,"  Energy   %4d (%03d %%)",m_p->scout[party[vflag]].energy,iproz(m_p->scout[party[vflag]].energy,SCOUT_ENERGY));
     sort_msg(sum_str);
     sprintf(sum_str,"  Damages  %4d (%03d %%)",m_p->scout[party[vflag]].dam,iproz(m_p->scout[party[vflag]].dam,MAX_DAM_SCOUT));
     sort_msg(sum_str);
     }
   return;
   }

/***********/
/* Missile */
/***********/
if (com[3] == 'm' || com[3] == 'M')
   {
   if (m_p->missile[party[vflag]].status == DESTROYED)
     {
     sort_msg("IN: no missile active!");
     }
   else
     {
     sprintf(sum_str,"IN m: active, pos.: %03d %03d",
          m_p->missile[party[vflag]].pos[0],m_p->missile[party[vflag]].pos[1]);
     sort_msg(sum_str);
     if (m_p->missile[party[vflag]].subtarget[0]==-11)
        {
        sprintf(sum_str," Target...: %03d %03d (path)",
               m_p->missile[party[vflag]].target[0],
               m_p->missile[party[vflag]].target[1]);
        }
     else
        {
        sprintf(sum_str," Target...: %03d %03d via %03d %03d",
               m_p->missile[party[vflag]].target[0],
               m_p->missile[party[vflag]].target[1],
               m_p->missile[party[vflag]].subtarget[0],
               m_p->missile[party[vflag]].subtarget[1]);
        }
     sort_msg(sum_str);
     sprintf(sum_str," Fuel.....: %03d  (%03d %%)",
          m_p->missile[party[vflag]].fuel,
          iproz(m_p->missile[party[vflag]].fuel,MISSILE_FUEL));
     sort_msg(sum_str);
     sprintf(sum_str," Damages..: %03d  (%03d %%)",
          m_p->missile[party[vflag]].dam,
          iproz(m_p->missile[party[vflag]].dam,MAX_DAM_MISSILE));
     sort_msg(sum_str);
     }
   for (i=0;i<MAX_MPATHS;i++)
     {
     if (m_p->imp[party[vflag]][i]==0)
       {
       sprintf(pr_str," Missile path %d: empty",i);
       sort_msg(pr_str);
       }
     else
       {
       sprintf(pr_str," Missile path %d: defined",i);
       sort_msg(pr_str);
       }
     }
   return;
   }

sort_msg("      *** BAD COMMAND ***");
return;
}


void build(com)
     char com[];
/**********************************************************************/
/*** Ausbau eines Planeten                                          ***/
/**********************************************************************/
{
	int x,y,nrp,nrb,i,del_time;
	char cm[2],print_str[MSG_LENGTH];

/* Kommandostringauswertung */
if ( sscanf(com,"%s%d%d",cm,&x,&y) != 3)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }

if (m_p->ship[vflag].move==ON)
  {
  sort_msg("BU: Cannot build planet");
  sort_msg("    while flying with warp.");
  return;
  }

/* x und y verifizieren */
if ( x<0 || x>=ARRAY_SIZE || y<0 || y>=ARRAY_SIZE)
   {
   sort_msg("BU: Wrong Coordinates!");
   return;
   }
if ( m_p->a[x][y] != PLANET)
   {
   sort_msg("BU: No planet at those coord.!");
   return;
   }
if ( (m_p->ship[vflag].pos[0]-x)  == 0 && (m_p->ship[vflag].pos[1]-y) == 0)
   {
   sort_msg("BU: Wrong coordinates (our)!");
   return;
   }   	
if ( (IABS( (m_p->ship[vflag].pos[0]-x) )) > 1 || (IABS( (m_p->ship[vflag].pos[1]-y) )) > 1)
   {
   sort_msg("BU: Planet too far away (D>1)!");
   return;
   }
/* Abfrage , ob Schilde unten */
if (m_p->ship[vflag].shield==ON)
   {
   sort_msg("BU: Shields must be down!");
   return;
   }
if (m_p->ship[vflag].hood==ON)
   {
   sort_msg("BU: Hood must be off!");
   return;
   }
/* Abfrage, ob Planet zur eigenen Partei gehoert */
nrp=find_nr_pl(x,y);
if (m_p->planet[nrp].party != party[vflag])
   {
   sort_msg("BU: Not a planet of our party!");
   sort_msg("BU: You must capture it first!");
   return;
   }
/* Abfrage Energie */
if ( m_p->ship[vflag].main_energy < m_p->planet[nrp].level*80 )
   {
   sort_msg("BU: Too less main energy!");
   return;
   }

/* Alles o.k.- Ausbau */
if (m_p->planet[nrp].level >= LEVEL_4)
   {
   nrb=-1;
   i=0;
   while (i<MAX_BASES && nrb == -1)
       {
       if (m_p->base[i].status==DESTROYED) nrb=i;
       i++;
       }
   if (nrb == -1) 
      {
      sort_msg("BU: failed; too many bases!");
      return;
      }
   else
     {
     m_p->base[nrb].status=CLAIMED;
     }
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-(m_p->planet[nrp].level*80);
   sort_msg("BU: Building base!");
   m_p->ship[vflag].build=ON;
   del_time=(NINT( (3.0+(2.0 *((float) (m_p->planet[nrp].level+4-level_of_ship[vflag])))))); 
   alarm(del_time);
   pause();
   if (m_p->planet[nrp].status == DESTROYED)
     {
     sort_msg("BU: Planet recently destoyed!");
     sort_msg("    Watch for hostile ship!") ;
     return;
     }
   /* Planet wird Basis */
   eliminate_pl(nrp);
   m_p->base[nrb].pos[0]=x;
   m_p->base[nrb].pos[1]=y;
   m_p->base[nrb].status=ACTIVE;
   m_p->base[nrb].party=party[vflag];
   m_p->base[nrb].dam=0;
   m_p->base[nrb].sh=ON;
   m_p->base[nrb].sh_energy=BASE_SH_EN;
   m_p->base[nrb].bay[0]=' ';
   m_p->base[nrb].bay[1]=' ';
   m_p->base[nrb].bay[2]=' ';
   m_p->base[nrb].bay[3]=' ';
   m_p->base[nrb].condition=GREEN;
   m_p->base[nrb].red_count=0;
   if (party[vflag] == FEDERAL) 
      {
      m_p->a[x][y]=F_BASE;
      }
   else
      { 
      m_p->a[x][y]=E_BASE;
      }
   sort_msg("BU: Planet rebuilt to a base!");
   m_p->user_score[vflag]=m_p->user_score[vflag]+75;
   m_p->name_score[vflag]=m_p->name_score[vflag]+75;
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+75;
   m_p->ship[vflag].build=OFF;   
   return;
   }
else
   { 
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-(m_p->planet[nrp].level*80);
   sort_msg("BU: Building next level!");
   m_p->ship[vflag].build=ON;
   sprintf(print_str,"BU:        Old level=%1d",m_p->planet[nrp].level);
   sort_msg(print_str);
   del_time=(NINT( (3.0+(2.0 *((float) (m_p->planet[nrp].level+4-level_of_ship[vflag])))))); 
   alarm(del_time);
   pause();
   if (m_p->planet[nrp].status == DESTROYED)
     {
     sort_msg("BU: Planet recently destoyed!");
     sort_msg("    Watch for hostile ship!") ;
     return;
     }
   m_p->planet[nrp].level++;
   sprintf(print_str,"BU: Finished: new level=%1d",m_p->planet[nrp].level);
   sort_msg(print_str);
   m_p->ship[vflag].build=OFF;
   m_p->user_score[vflag]=m_p->user_score[vflag]+(10*m_p->planet[nrp].level);
   m_p->name_score[vflag]=m_p->name_score[vflag]+(10*m_p->planet[nrp].level);
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+(10*m_p->planet[nrp].level);
   return;
   }
return;
}

void capture(com)
     char com[];
/**********************************************************************/
/*** Capture eines Planeten                                         ***/
/**********************************************************************/

{
	int x,y,nrp,hit_en,device,del_time,ids;
	char cm[2],print_str[MSG_LENGTH];
	
/* Kommandostringauswertung */
if ( sscanf(com,"%s%d%d",cm,&x,&y) != 3)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }
   
if (m_p->ship[vflag].move==ON)
  {
  sort_msg("CA: Cannot capture planet");
  sort_msg("    while flying with warp.");
  return;
  }
/* x und y verifizieren */
if ( x<0 || x>=ARRAY_SIZE || y<0 || y>=ARRAY_SIZE)
   {
   sort_msg("CA: Wrong Coordinates!");
   return;
   }
if ( m_p->a[x][y] != PLANET)
   {
   sort_msg("CA: No planet at that coord.!");
   return;
   }
if ( (m_p->ship[vflag].pos[0]-x)  == 0 && (m_p->ship[vflag].pos[1]-y) == 0)
   {
   sort_msg("CA: Wrong coordinates (ours)!");
   return;
   }   	
if ( (IABS( (m_p->ship[vflag].pos[0]-x) )) > 1 || (IABS( (m_p->ship[vflag].pos[1]-y) )) > 1)
   {
   sort_msg("CA: Planet too far away (D>1)!");
   return;
   }
/* Abfrage , ob Schilde unten */
if (m_p->ship[vflag].shield==ON)
   {
   sort_msg("CA: Shields must be down!");
   return;
   }
if (m_p->ship[vflag].hood==ON)
   {
   sort_msg("CA: Hood must be off!");
   return;
   }
/* Abfrage, ob Planet zur eigenen Partei gehoert */
nrp=find_nr_pl(x,y);
if (m_p->planet[nrp].party == party[vflag])
   {
   sort_msg("CA: Planet already on our side!");
   alarm(1);
   pause(); 
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-20;
   return;
   }
/* Abfrage, ob Schiff den Planeten capturen kann */
if ( (level_of_ship[vflag]+1) < m_p->planet[nrp].level )
   {
   sort_msg("CA: You can't capture that planet");
   sprintf(print_str,"CA: Level ship=%1d,level planet=%1d",level_of_ship[vflag],
                                                         m_p->planet[nrp].level);
   sort_msg("CA: Lev.ship>=lev.pl.-1 necess.");
   return;
   }
/* Abfrage Energie */
if ( m_p->ship[vflag].main_energy < m_p->planet[nrp].level*50 )
   {
   sort_msg("CA: Too less main energy!");
   return;
   }

/* alles o.k. */
ids=m_p->display_status[vflag];
m_p->display_status[vflag]=DISPLAY_CAPTURING;
sort_msg("CA: Capturing planet...");
sort_msg("CA: Planet activ. defense system");

hit_en=50*m_p->planet[nrp].level;
gauss2(&hit_en);
sprintf(print_str,"Got a %4d units hit by planet",hit_en);
sort_msg(print_str);
device=hit_en;
if (device>10)
   {
   device=device-((device/10)*10);
   if (device==0) device=1;
   }
m_p->ship[vflag].dam[device]=m_p->ship[vflag].dam[device]+hit_en;   
del_time=5+(2*m_p->planet[nrp].level-level_of_ship[vflag]);
alarm(del_time);
pause();

if ( m_p->a[x][y] != PLANET)
   {
   sort_msg("Planet destroyed during capture");
   m_p->display_status[vflag]=ids;
   return;
   }

nrp=find_nr_pl(x,y);  /* neu wegen delay */
m_p->planet[nrp].party=party[vflag];
sort_msg("CA: Planet captured!");
m_p->user_score[vflag]=m_p->user_score[vflag]+(20*m_p->planet[nrp].level);
m_p->name_score[vflag]=m_p->name_score[vflag]+(20*m_p->planet[nrp].level);   
m_p->score[party[vflag]]=m_p->score[party[vflag]]+(20*m_p->planet[nrp].level);
m_p->display_status[vflag]=ids;
return;
}


void transfer(com)
     char com[];
/***********************************************************************/
/*** transfer: Transferieren von Energie innerhalb des Schiffs       ***/
/***********************************************************************/
{
int energy;
char a1[CMD_LENGTH],a2[6];
	
strncpy(a1,com,CMD_LENGTH);
a1[0]=a1[1]=a1[2]=a1[3]=a1[4]=a1[5]='X';

if (sscanf(a1,"%s%d",a2,&energy) != 2)
   {
   sort_msg("        *** BAD COMMAND ***");
   return;
   }
else
   {
   if (energy>999 || energy<0)
      {
      sort_msg("TR:  Wrong amount of energy!");
      return;
      }
   }
   
/* von S zu M */
if (strncmp(com,"TR S M",6)==0 || strncmp(com,"TR s m",6)==0)
   {
   if (energy>m_p->ship[vflag].sh_energy)
      {
      sort_msg("TR:    Not enough Shield Energy!");
      return;
      }
   alarm((1+(energy/100)));
   pause();
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy+energy-10;
   m_p->ship[vflag].sh_energy=m_p->ship[vflag].sh_energy-energy;   
   if (m_p->ship[vflag].main_energy > max_m_en[vflag])
      {
      sort_msg("TR: Too much energy transferred!");
      sort_msg("    Overflow caused damages!");      
      m_p->ship[vflag].dam[6]=m_p->ship[vflag].dam[6]+25;
      m_p->ship[vflag].main_energy=max_m_en[vflag];
      }
   sort_msg("TR:  S->M: Energy transferred!");
   return;
   }
   
/* von P zu M */
if (strncmp(com,"TR P M",6)==0 || strncmp(com,"TR p m",6)==0)
   {
   if (vflag >=25)
      {
      sort_msg("TR:  You have no Phasers!");
      return;
      }
   if (energy>m_p->ship[vflag].ph_energy)
      {
      sort_msg("TR:    Not enough Phaser Energy!");
      return;
      }
   alarm((1+(energy/100)));
   pause();
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy+energy-10;
   m_p->ship[vflag].ph_energy=m_p->ship[vflag].ph_energy-energy;   
   if (m_p->ship[vflag].main_energy > max_m_en[vflag])
      {
      sort_msg("TR: Too much energy transferred!");
      sort_msg("    Overflow caused damages!");      
      m_p->ship[vflag].dam[6]=m_p->ship[vflag].dam[6]+25;
      m_p->ship[vflag].main_energy=max_m_en[vflag];
      }
   sort_msg("TR:  P->M: Energy transferred!");
   return;
   }
   
/* von P zu S */
if (strncmp(com,"TR P S",6)==0 || strncmp(com,"TR p s",6)==0)
   {
   if (vflag >=25)
      {
      sort_msg("TR:  You have no Phasers!");
      return;
      }
   if (energy>m_p->ship[vflag].ph_energy)
      {
      sort_msg("TR:    Not enough Phaser Energy!");
      return;
      }
   alarm((1+(energy/100)));
   pause();
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-10;
   m_p->ship[vflag].sh_energy=m_p->ship[vflag].sh_energy+energy;   
   m_p->ship[vflag].ph_energy=m_p->ship[vflag].ph_energy-energy;   
   if (m_p->ship[vflag].sh_energy > max_sh_en[vflag])
      {
      sort_msg("TR: Too much energy transferred!");
      sort_msg("    Overflow caused damages!");      
      m_p->ship[vflag].dam[5]=m_p->ship[vflag].dam[5]+25;
      m_p->ship[vflag].sh_energy=max_sh_en[vflag];
      }
   sort_msg("TR:  P->S: Energy transferred!");
   return;
   }
   
/* von S zu P */
if (strncmp(com,"TR S P",6)==0 || strncmp(com,"TR s p",6)==0)
   {
   if (vflag >=25)
      {
      sort_msg("TR:  You have no Phasers!");
      return;
      }
   if (energy>m_p->ship[vflag].sh_energy)
      {
      sort_msg("TR:    Not enough Shield Energy!");
      return;
      }
   alarm((1+(energy/100)));
   pause();
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-10;
   m_p->ship[vflag].sh_energy=m_p->ship[vflag].sh_energy-energy;   
   m_p->ship[vflag].ph_energy=m_p->ship[vflag].ph_energy+energy;   
   if (m_p->ship[vflag].ph_energy > max_ph_en[vflag])
      {
      sort_msg("TR: Too much energy transferred!");
      sort_msg("    Overflow caused damages!");      
      m_p->ship[vflag].dam[4]=m_p->ship[vflag].dam[4]+25;
      m_p->ship[vflag].ph_energy=max_ph_en[vflag];
      }
   sort_msg("TR:  S->P: Energy transferred!");
   return;
   }
   
/* von M zu P */
if (strncmp(com,"TR M P",6)==0 || strncmp(com,"TR m p",6)==0)
   {
   if (vflag >=25)
      {
      sort_msg("TR:  You have no Phasers!");
      return;
      }
   if (energy>m_p->ship[vflag].main_energy)
      {
      sort_msg("TR:    Not enough Main Energy!");
      return;
      }
   alarm((1+(energy/100)));
   pause();
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-10-energy;
   m_p->ship[vflag].ph_energy=m_p->ship[vflag].ph_energy+energy;   
   if (m_p->ship[vflag].ph_energy > max_ph_en[vflag])
      {
      sort_msg("TR: Too much energy transferred!");
      sort_msg("    Overflow caused damages!");      
      m_p->ship[vflag].dam[4]=m_p->ship[vflag].dam[4]+25;
      m_p->ship[vflag].ph_energy=max_ph_en[vflag];
      }
   sort_msg("TR:  M->P: Energy transferred!");
   return;
   }
   
/* von M zu S */
if (strncmp(com,"TR M S",6)==0 || strncmp(com,"TR m s",6)==0)
   {
   if (energy>m_p->ship[vflag].main_energy)
      {
      sort_msg("TR:    Not enough Main Energy!");
      return;
      }
   alarm((1+(energy/100)));
   pause();
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-(10+energy);
   m_p->ship[vflag].sh_energy=m_p->ship[vflag].sh_energy+energy;   
   if (m_p->ship[vflag].sh_energy > max_sh_en[vflag])
      {
      sort_msg("TR: Too much energy transferred!");
      sort_msg("    Overflow caused damages!");      
      m_p->ship[vflag].dam[5]=m_p->ship[vflag].dam[5]+25;
      m_p->ship[vflag].sh_energy=max_sh_en[vflag];
      }
   sort_msg("TR:  M->S: Energy transferred!");
   return;
   }

sort_msg("       *** BAD COMMAND ***");

return;
}


void repair(com)
     char com[];
/*************************************************************************/
/***  repair : Reparieren von Schaeden                                 ***/
/*************************************************************************/
{
int nr;
char a1[2];

if (sscanf(com,"%s%d",a1,&nr) != 2)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }
if (nr < 1 || nr > 10)
   {
   sort_msg("RE: Wrong device (1-10 only)!");
   return;
   }
if (vflag>=25 && nr==4)
   {
   sort_msg("RE: Destroyer got no phaser!");
   sort_msg("RE: *** Wrong device ***");
   return;
   }
if (m_p->ship[vflag].dam[nr]==0)
   {
   sort_msg("RE:Device o.k.,nothing repaired!");
   return;
   }
   
/* alles o.k., Rubrik nr um 20 damage-p. red. */
sort_msg("RE:      Repairing... ");
alarm(5);
pause();
m_p->ship[vflag].dam[nr]=m_p->ship[vflag].dam[nr]-20;
if (m_p->ship[vflag].dam[nr]<0) m_p->ship[vflag].dam[nr]=0;
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-40;
sort_msg("RE: 20 Damage Points repaired!");

return;
}


void symbol()
/**************************************************************************/
/*** Darstellung der Symbole im Spiel                                   ***/
/**************************************************************************/
{
char print_str[MSG_LENGTH];

strcpy(print_str,"SY: Meaning of symbols in game:");
sort_msg(print_str);
sprintf(print_str,"Star=%c Planet=%c Space=%c Hood=%c",STAR,PLANET,SPACE,HOOD);
sort_msg(print_str);
sprintf(print_str,"Bl.Hole=%c Mine=%c Blind/Torp.=%c",BLACK_HOLE,MINE,BLIND);
sort_msg(print_str);
sprintf(print_str,"Cloud=%c Fed.Base=(%c Imp.Base=[%c",CLOUD,F_BASE,E_BASE);
sort_msg(print_str);
sprintf(print_str,"Fed.Sat.=%c Imp.Sat.=%c Romulan=%c",F_SAT,E_SAT,ROMULAN);
sort_msg(print_str);
sprintf(print_str,"Fed.Fort=%c Imp.Fort=%c Jumpg.=%c",F_FORT,E_FORT,JUMPGATE);
sort_msg(print_str);
sprintf(print_str,"Fed.Scout=%c Imp.Scout=%c Miss.=%c",F_SCOUT,E_SCOUT,MISSILE);
sort_msg(print_str);
sprintf(print_str,"Fed.Homew.=%c Imp.Homew.=%c",F_HOMEWORLD,E_HOMEWORLD);
sort_msg(print_str);

return;
}

void transport_energy(com)
     char com[];
/**************************************************************************/
/*** Transport einer Energiemenge von Schiff A zu Schiff B              ***/
/**************************************************************************/
{
  int energy,zflag;
  char cm[2],ship1;
	
/* Kommandostring interpretieren */
if (sscanf(com,"%s %c %d",cm,&ship1,&energy) != 3)
   {
   sort_msg("    *** BAD COMMAND ***");
   return;
   }

if (m_p->ship[vflag].shield==ON)
   {
   sort_msg("TE: Our shields must be down!");
   return;
   }
if (m_p->ship[vflag].hood==ON)
   {
   sort_msg("TE: Our hood must be off!");
   return;
   }
if (energy > 500 || energy <1)
   {
   sort_msg("TE: Wrong energy val. (1...500)!");
   return;
   }

zflag=-1;
/* Robotschiffe */
if (ship1 == '1') zflag=1;
if (ship1 == '2') zflag=2;
if (ship1 == '3') zflag=3;
if (ship1 == '4') zflag=4;
if (ship1 == '5') zflag=5;
if (ship1 == '6') zflag=6;
if (ship1 == '7') zflag=7;
if (ship1 == '8') zflag=8;
if (ship1 == '9') zflag=9;
if (ship1 == '0') zflag=0;
if (zflag != -1) /* d.h. Robotschiff */
  {
  if (m_p->robotship[party[vflag]][zflag].status != WAITING)
    {
    sort_msg("TE: Wrong Robotship status:");
    sort_msg("    Must bei WAITING!");
    return;
    }
  if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->robotship[party[vflag]][zflag].pos[0]) )) > 1
                       ||
       (IABS( (m_p->ship[vflag].pos[1]-m_p->robotship[party[vflag]][zflag].pos[1]) )) > 1 )
    {
    sort_msg("TE: Target must be in contact!");
    return;
    }
  sort_msg("TE: Starting energy transport...");
  alarm((1+(energy/100)));
  pause();
  m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-(energy+50);
  m_p->robotship[party[vflag]][zflag].main_energy=m_p->robotship[party[vflag]][zflag].main_energy+energy;
  if (m_p->robotship[party[vflag]][zflag].main_energy > ROBOTSHIP_ENERGY) m_p->robotship[party[vflag]][zflag].main_energy=ROBOTSHIP_ENERGY;
  m_p->user_score[vflag]=m_p->user_score[vflag]+10;
  m_p->name_score[vflag]=m_p->name_score[vflag]+10;
  m_p->score[party[vflag]]=m_p->score[party[vflag]]+10;
  sort_msg("TE: Energy transport finished!");
  return;
  }

/* Schiffe */
zflag=det_target(ship1);
if (zflag == vflag)
   {
   sort_msg("TE: Target is our ship!");
   return;
   }
if (zflag<=4 || m_p->luser[zflag]==DESTROYED || m_p->l_exist[zflag]==DESTROYED)
   {
   sort_msg("TE: Wrong target or not in game!");
   return;
   }
if (m_p->ship[zflag].shield==ON)
   {
   sort_msg("TE: Target shields must be down!");
   return;
   }
if (m_p->ship[zflag].hood==ON)
   {
   sort_msg("TE: Target hood must be off!");
   return;
   }

if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->ship[zflag].pos[0]) )) > 1
                        ||
     (IABS( (m_p->ship[vflag].pos[1]-m_p->ship[zflag].pos[1]) )) > 1 )
   {
   sort_msg("TE: Target must be in contact!");
   return;
   }
   
sort_msg("TE: Starting energy transport...");
alarm((1+(energy/100)));
pause();
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-(energy+50);
m_p->ship[zflag].main_energy=m_p->ship[zflag].main_energy+energy;
m_p->user_score[vflag]=m_p->user_score[vflag]+10;
m_p->name_score[vflag]=m_p->name_score[vflag]+10;
m_p->score[party[vflag]]=m_p->score[party[vflag]]+10;
sort_msg("TE: Energy transport finished!");

return;
}

void transport_fuel(com)
     char com[];
/**************************************************************************/
/*** Transport einer Treibstoffmenge von Schiff A zu Schiff B           ***/
/**************************************************************************/
{
  int fuel,zflag;
  char cm[2],ship1;
	
/* Kommandostring interpretieren */
if (sscanf(com,"%s %c %d",cm,&ship1,&fuel) != 3)
   {
   sort_msg("    *** BAD COMMAND ***");
   return;
   }

if (m_p->ship[vflag].shield==ON)
   {
   sort_msg("TF: Our shields must be down!");
   return;
   }
if (m_p->ship[vflag].hood==ON)
   {
   sort_msg("TF: Our hood must be off!");
   return;
   }
if (fuel <1)
   {
   sort_msg("TF: Minimum 1 t");
   return;
   }
if (fuel > m_p->ship[vflag].fuel)
   {
   sort_msg("TF: Not enough fuel!");
   return;
   }

zflag=-1;
/* Robotschiffe */
if (ship1 == '1') zflag=1;
if (ship1 == '2') zflag=2;
if (ship1 == '3') zflag=3;
if (ship1 == '4') zflag=4;
if (ship1 == '5') zflag=5;
if (ship1 == '6') zflag=6;
if (ship1 == '7') zflag=7;
if (ship1 == '8') zflag=8;
if (ship1 == '9') zflag=9;
if (ship1 == '0') zflag=0;
if (zflag != -1) /* d.h. Robotschiff */
  {
  if (m_p->robotship[party[vflag]][zflag].status != WAITING)
    {
    sort_msg("TF: Wrong Robotship status:");
    sort_msg("    Must bei WAITING!");
    return;
    }
  if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->robotship[party[vflag]][zflag].pos[0]) )) > 1
                       ||
       (IABS( (m_p->ship[vflag].pos[1]-m_p->robotship[party[vflag]][zflag].pos[1]) )) > 1 )
    {
    sort_msg("TF: Target must be in contact!");
    return;
    }
  if ( (m_p->robotship[party[vflag]][zflag].fuel + fuel) > ROBOTSHIP_FUEL)
    {
    sort_msg("TF: Too much fuel!");
    return;
    }
  sort_msg("TF: Starting fuel transport...");
  alarm((1+(fuel/20)));
  pause();
  m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-50;
  m_p->ship[vflag].fuel=m_p->ship[vflag].fuel-fuel;
  m_p->robotship[party[vflag]][zflag].fuel=m_p->robotship[party[vflag]][zflag].fuel+fuel;
  if (m_p->robotship[party[vflag]][zflag].fuel > ROBOTSHIP_FUEL) m_p->robotship[party[vflag]][zflag].fuel=ROBOTSHIP_FUEL;
  m_p->user_score[vflag]=m_p->user_score[vflag]+10;
  m_p->name_score[vflag]=m_p->name_score[vflag]+10;
  m_p->score[party[vflag]]=m_p->score[party[vflag]]+10;
  sort_msg("TF: Fuel transport finished!");
  return;
  }

/* Schiffe */
zflag=det_target(ship1);
if (zflag == vflag)
   {
   sort_msg("TF: Target is our ship!");
   return;
   }
if (zflag<=4 || m_p->luser[zflag]==DESTROYED || m_p->l_exist[zflag]==DESTROYED)
   {
   sort_msg("TF: Wrong target or not in game!");
   return;
   }
if (m_p->ship[zflag].shield==ON)
   {
   sort_msg("TF: Target shields must be down!");
   return;
   }
if (m_p->ship[zflag].hood==ON)
   {
   sort_msg("TF: Target hood must be off!");
   return;
   }

if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->ship[zflag].pos[0]) )) > 1
                        ||
     (IABS( (m_p->ship[vflag].pos[1]-m_p->ship[zflag].pos[1]) )) > 1 )
   {
   sort_msg("TF: Target must be in contact!");
   return;
   }
if ( (m_p->ship[zflag].fuel + fuel) > max_fuel[zflag])
   {
   sort_msg("TF: Too much fuel!");
   return;
   }
   
sort_msg("TF: Starting fuel transport...");
alarm((1+(fuel/20)));
pause();
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-50;
m_p->ship[zflag].fuel=m_p->ship[zflag].fuel+fuel;
m_p->ship[vflag].fuel=m_p->ship[vflag].fuel-fuel;
m_p->user_score[vflag]=m_p->user_score[vflag]+10;
m_p->name_score[vflag]=m_p->name_score[vflag]+10;
m_p->score[party[vflag]]=m_p->score[party[vflag]]+10;
sort_msg("TF: Fuel transport finished!");

return;
}

void transport_torp(com)
     char com[];
/**************************************************************************/
/*** Transport einer Anzahl Torpedoes von Schiff A zu Schiff B          ***/
/**************************************************************************/
{
  int torp,zflag;
  char cm[2],ship1;
	
/* Kommandostring interpretieren */
if (sscanf(com,"%s %c %d",cm,&ship1,&torp) != 3)
   {
   sort_msg("    *** BAD COMMAND ***");
   return;
   }

if (m_p->ship[vflag].shield==ON)
   {
   sort_msg("TT: Our shields must be down!");
   return;
   }
if (m_p->ship[vflag].hood==ON)
   {
   sort_msg("TT: Our hood must be off!");
   return;
   }
if (torp > m_p->ship[vflag].torp)
   {
   sort_msg("TT: Not enough torpedoes!");
   return;
   }
if (torp < 1)
   {
   sort_msg("TT: Minimum 1 torpedo!");
   return;
   }

zflag=-1;
/* Robotschiffe */
if (ship1 == '1') zflag=1;
if (ship1 == '2') zflag=2;
if (ship1 == '3') zflag=3;
if (ship1 == '4') zflag=4;
if (ship1 == '5') zflag=5;
if (ship1 == '6') zflag=6;
if (ship1 == '7') zflag=7;
if (ship1 == '8') zflag=8;
if (ship1 == '9') zflag=9;
if (ship1 == '0') zflag=0;
if (zflag != -1) /* d.h. Robotschiff */
  {
  if (m_p->robotship[party[vflag]][zflag].status != WAITING)
    {
    sort_msg("TT: Wrong Robotship status:");
    sort_msg("    Must bei WAITING!");
    return;
    }
  if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->robotship[party[vflag]][zflag].pos[0]) )) > 1
                       ||
       (IABS( (m_p->ship[vflag].pos[1]-m_p->robotship[party[vflag]][zflag].pos[1]) )) > 1 )
    {
    sort_msg("TT: Target must be in contact!");
    return;
    }
  if ( (m_p->robotship[party[vflag]][zflag].torp + torp) > ROBOTSHIP_TORPEDOES)
    {
    sort_msg("TT: Too many torpedoes!");
    return;
    }
  sort_msg("TT: Starting torpedo transfer...");
  alarm((3*torp));
  pause();
  m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-50;
  m_p->ship[vflag].torp=m_p->ship[vflag].torp-torp;
  m_p->robotship[party[vflag]][zflag].torp=m_p->robotship[party[vflag]][zflag].torp+torp;
  m_p->user_score[vflag]=m_p->user_score[vflag]+10;
  m_p->name_score[vflag]=m_p->name_score[vflag]+10;
  m_p->score[party[vflag]]=m_p->score[party[vflag]]+10;
  sort_msg("TT: Torpedo transfer finished!");
  return;
  }

/* Schiffe */
zflag=det_target(ship1);
if (zflag<=4 || m_p->luser[zflag]==DESTROYED || m_p->l_exist[zflag]==DESTROYED || zflag == vflag)
   {
   sort_msg("TT: Wrong target!");
   return;
   }
if (m_p->ship[zflag].shield==ON)
   {
   sort_msg("TT: Target shields must be down!");
   return;
   }
if (m_p->ship[zflag].hood==ON)
   {
   sort_msg("TT: Target hood must be off!");
   return;
   }
if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->ship[zflag].pos[0]) )) > 1
                        ||
     (IABS( (m_p->ship[vflag].pos[1]-m_p->ship[zflag].pos[1]) )) > 1 )
   {
   sort_msg("TT: Target must be in contact!");
   return;
   }
 if ( (m_p->ship[zflag].torp + torp) > max_torp[zflag])
   {
   sort_msg("TT: Too many torpedoes!");
   return;
   }
   
sort_msg("TT: Starting torpedo transfer...");
alarm((3*torp));
pause();
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-50;
m_p->ship[zflag].torp=m_p->ship[zflag].torp+torp;
m_p->ship[vflag].torp=m_p->ship[vflag].torp-torp;
m_p->user_score[vflag]=m_p->user_score[vflag]+10;
m_p->name_score[vflag]=m_p->name_score[vflag]+10;
m_p->score[party[vflag]]=m_p->score[party[vflag]]+10;
sort_msg("TT: Torpedo transfer finished!");

return;
}

void learnX(com)
    char com[];
/**********************************************************************/
/*** Belegen von X mit einem Befehl (d.h. Abkuerzung)               ***/
/**********************************************************************/
{
	int i;
        char print_str[MSG_LENGTH];

i=5;	
do {
   xcmd[(i-5)] = com[i];
   i++;
 }  while ( i<(CMD_LENGTH-1) && com[(i-1)] != '\0');
strcpy(print_str,"LE: X set to:");
sort_msg(print_str);
strncpy(print_str,xcmd,MSG_LENGTH);
sort_msg(print_str);

return; 
}

void learnY(com)
    char com[];
/**********************************************************************/
/*** Belegen von Y mit einem Befehl (d.h. Abkuerzung)               ***/
/**********************************************************************/
{
	int i;
        char print_str[MSG_LENGTH];

i=5;	
do {
   ycmd[(i-5)] = com[i];
   i++;
 }  while ( i<(CMD_LENGTH-1) && com[(i-1)] != '\0');
strcpy(print_str,"LE: Y set to:");
sort_msg(print_str);
strncpy(print_str,ycmd,MSG_LENGTH);
sort_msg(print_str);
   
return; 
}

void hood(com)
     char com[];
/*************************************************************************/
/*** Tarnvorrichtung                                                   ***/
/*************************************************************************/
{

if (strncmp(com,"HO on",5)==0 || strncmp(com,"HO ON",5)==0) /* Tarnv. an */
   {
   if (m_p->ship[vflag].shield==ON)
      {
      sort_msg("HO: Shields must be down!");
      return;
      }
    if (m_p->ship[vflag].hood==ON)
      {
      sort_msg("HO: Hood already on!");
      return;
      }
   if (m_p->ship[vflag].dam[10] > 250 )
      {
      sort_msg("HO: Hood damaged, out of order!");
      return;
      }
   alarm(1);
   pause();
   m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-55;
   m_p->ship[vflag].hood=ON;
   m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=HOOD;
   sort_msg("HO: Hood on!");
   return;
   }

if (strncmp(com,"HO of",5)==0 || strncmp(com,"HO OF",5)==0) /* Tarnv. aus */
   {
   if (m_p->ship[vflag].hood==OFF)
      {
      sort_msg("HO: Hood already off!");
      return;
      }
   alarm(1);
   pause();
   m_p->ship[vflag].hood=OFF;
   m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
   sort_msg("HO: Hood off!");
   return;
   }
   
sort_msg("       *** BAD COMMAND ***");
return;
}


void impuls(com)
     char com[];
/**********************************************************************/
/***  Impulsantrieb                                                 ***/
/**********************************************************************/
{
     int x,y,xneu,yneu,xalt,yalt,zflag,lex,hit_energy,cl_dam,dev;
     char cm[2],aneu,print_str[MSG_LENGTH];
     
if (sscanf(com,"%s%d%d",cm,&x,&y) != 3)
   {
   sort_msg("     *** BAD COMMAND ***");
   return;
   }
   
if ( (IABS(x)) > 1 || (IABS(y)) > 1 )
   {
   sort_msg("IM: max. 1 step in each direct.!");
   return;
   }
if (m_p->ship[vflag].dam[2]>300)
   {
   sort_msg("IM: Impulse engines out of order");
   return;
   } 
if (m_p->ship[vflag].fuel<=0)
   {
   sort_msg("IM: Out of fuel!");
   return;
   } 

xalt=m_p->ship[vflag].pos[0];
yalt=m_p->ship[vflag].pos[1];
xneu=xalt+x;
yneu=yalt+y;
if (xneu<0 || yneu<0 || xneu>=ARRAY_SIZE || yneu>=ARRAY_SIZE)
   {
   sort_msg("PH:  *** Wrong coordinates ***");
   return;
   }
aneu=m_p->a[xneu][yneu];
usleep((300000+level_of_ship[vflag]*40000));
m_p->ship[vflag].fuel--;

/* kein Flug, wenn Stoss gegen feststehende Objekte
   Sat. werden vernichtet, Flug wird aber gestoppt. Man koennte auch weiter-
   fliegen lassen. Dann stoert jedoch init_expl in hit, siehe MINE */
if (aneu == STAR || aneu==PLANET || aneu==E_BASE || aneu==F_BASE ||
    aneu == F_SAT || aneu==E_SAT || aneu==F_FORT || aneu==E_FORT ||
    aneu == E_HOMEWORLD || aneu == F_HOMEWORLD || aneu==E_SCOUT ||
    aneu == F_SCOUT || aneu==JUMPGATE || aneu == E_ROBOTSHIP || 
    aneu == F_ROBOTSHIP || xneu<0 || xneu>(ARRAY_SIZE-1) || 
    yneu<0 || yneu>(ARRAY_SIZE-1))
    {
    m_p->ship[vflag].pos[0]=xalt;
    m_p->ship[vflag].pos[1]=yalt;
    if (m_p->ship[vflag].hood==OFF)
      {
      m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
      }
    else
      {
      m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=HOOD;
      }
    if (m_p->ship[vflag].ramm==OFF && m_p->ship[vflag].dam[6]<350)
      {
      sort_msg("IM: *** Collision averted ***");
      m_p->ship[vflag].fuel--;
      }
    else
      {
      if ( party[vflag]==IMPERIAL && aneu == F_HOMEWORLD )
       	 {
         sort_msg("IM: *** Collision!!!!! ***");
         sort_msg("    *** emergency stop ***");
         sort_msg("    ***50000 units hit!***");
         hit_energy=50000;
         hit(ships[vflag][0],xneu,yneu,hit_energy);
         hit_energy=500;
         gauss2(&hit_energy);
         hit(aneu,xneu,yneu,hit_energy);
         }          
      else if ( party[vflag]==FEDERAL && aneu == E_HOMEWORLD )
       	 {
         sort_msg("IM: *** Collision!!!!! ***");
         sort_msg("    *** emergency stop ***");
         hit_energy=50000;
         hit(ships[vflag][0],xneu,yneu,hit_energy);
         hit_energy=500;
	 gauss2(&hit_energy);
         hit(aneu,xneu,yneu,hit_energy);
         }          
      else
	 {
         hit_energy=100;
         gauss2(&hit_energy);
         hit(ships[vflag][0],xneu,yneu,hit_energy);                     
         sort_msg("Collision *** emergency stop ***");
         m_p->ship[vflag].fuel--;          
         hit_energy=100;
         gauss2(&hit_energy);
         hit(aneu,xneu,yneu,hit_energy);           
         }
    }
    m_p->ship[vflag].move=OFF;          
    return;
  } /* Ende Stoss gegen ... mit Stopp */


/* Stoss gegen fliegende Objekte: ebenfalls Stopp */
zflag=det_target(aneu);
if (zflag!=-1 || aneu==HOOD)
   {
   m_p->ship[vflag].pos[0]=xalt;
   m_p->ship[vflag].pos[1]=yalt;
   if (m_p->ship[vflag].hood==OFF)
      {
      m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
      }
   else
      {
      m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=HOOD;
      }
   sort_msg("IM: *** Collision ***");
   sort_msg("    *** Emergency stop ****");
   hit_energy=100;
   gauss2(&hit_energy);
   hit(aneu,xneu,yneu,hit_energy); 
   hit_energy=100;
   gauss2(&hit_energy);
   sprintf(print_str,"IM: We got a %d units hit!",hit_energy);
   sort_msg(print_str);
   hit(ships[vflag][0],xneu,yneu,hit_energy);
   m_p->ship[vflag].fuel--;
   m_p->ship[vflag].move=OFF;
   return;
   }

/* Zusammenstoss mit anderen Objekten (Weiterflug) */
if (aneu==MISSILE)
   {
   sort_msg("IM: Collision with missile!");
   hit_energy=MISSILE_COL_ENERGY;
   gauss2(&hit_energy);
   hit(MISSILE,xneu,yneu,hit_energy);
   hit(ships[vflag][0],xneu,yneu,hit_energy);
   aneu=m_p->a[xneu][yneu]=SPACE;
   }

if (aneu==BLIND)
   {
   sort_msg("IM: Collision with Blind!");
   /* init_expl geht hier nicht wegen Flug durch Explosion: Symbol verschw. */
   aneu=m_p->a[xneu][yneu]=EXPLOSION;
   usleep(EXPL_TIME);
   aneu=m_p->a[xneu][yneu]=SPACE;
   hit_energy=BLIND_COL_ENERGY;
   gauss2(&hit_energy);
   hit(ships[vflag][0],xneu,yneu,hit_energy);
   }
if (aneu==MINE)
   {
   sort_msg("IM: Collision with Mine!");
   /* init_expl geht hier nicht wegen Flug durch Explosion: Symbol verschw. */
   aneu=m_p->a[xneu][yneu]=EXPLOSION;
   usleep(EXPL_TIME);
   aneu=m_p->a[xneu][yneu]=SPACE;
   hit_energy=MINE_COL_ENERGY;
   gauss2(&hit_energy);
   hit(ships[vflag][0],xneu,yneu,hit_energy); 
   }

if (aneu == BLACK_HOLE)
   {
   lex=incident(20);
   if (lex == 1) 
      {
      sort_msg("-GULP-Swallowed by Black Hole!");
      m_p->l_exit[vflag]=EXIT;
      m_p->a[xalt][yalt]=SPACE;
      usleep(200000);
      }
   else
      {
      sort_msg("IM: Displaced by Black Hole!");
      m_p->a[xalt][yalt]=SPACE;
      m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[1]=yneu=xneu=-99;
      do {
         xneu=(int) (199.0*ran());
         yneu=(int) (199.0*ran());                
         }
      while ((xneu<0 || yneu<0 || xneu>(ARRAY_SIZE-1) || yneu>(ARRAY_SIZE-1))
                                        &&
                             (m_p->a[xneu][yneu]!=SPACE) );
      a_alt=aneu=SPACE;
      }
   }
if (aneu==CLOUD)
   {
   cl_dam=50;
   gauss2(&cl_dam);
   dev=cl_dam/10;
   if (dev==0) dev=1;
   if (dev>9) dev=9;
   if (m_p->ship[vflag].shield==OFF)
      {
      sort_msg("We are inside a gass cloud!");
      sort_msg("We are damaged because SH DOWN!");
      m_p->ship[vflag].dam[dev]=m_p->ship[vflag].dam[dev]+cl_dam;
      }
   else
      {
      m_p->ship[vflag].sh_energy=m_p->ship[vflag].sh_energy-cl_dam;
      }
   }
/* Ende aller Kollisionen und Abfragen */

m_p->ship[vflag].pos[0]=xneu;
m_p->ship[vflag].pos[1]=yneu;
m_p->a[xalt][yalt]=a_alt;
a_alt=aneu;
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
           
return;
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

void sig_hnd()
/************************************************************************/
/*** setzen des neuen Signalhandlings nach pause                      ***/
/************************************************************************/
{
signal(SIGALRM,sig_hnd);
return;
}

void sig_hnd_ctrlc()
/************************************************************************/
/*** setzen des neuen Signalhandlings nach pause                      ***/
/************************************************************************/
{
sort_msg(" *** CTRL-C desaktiviert! ***");
m_p->cmd[vflag][0]='\0';
signal(SIGINT,sig_hnd_ctrlc);
return;
}


int quit_control()
/************************************************************************/
/*** Kontrolliert, ob QU erlaubt, d.h. ausser Reichweite feindlicher  ***/
/*** Basen, Forts, Schiffe und der feindlichen Heimatwelt             ***/
/*** Immer erlaubt bei Docking an Heimatwelt!                         ***/ 
/************************************************************************/
{
int i,enemy;

enemy=FEDERAL;
if (party[vflag]==FEDERAL) enemy=IMPERIAL;

if (m_p->ship[vflag].dock != HW_DOCKED )
  {
  /* Schiffe */
  for (i=0;i<MAX_USER;i++)
    {
    if ( m_p->luser[i]==ACTIVE && party[i] != party[vflag])
       {
       if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->ship[i].pos[0]))) <= SHIP_RANGE
                           &&
            (IABS( (m_p->ship[vflag].pos[1]-m_p->ship[i].pos[1]))) <= SHIP_RANGE )
          {
          sort_msg("QU: You cannot quit the game");
          sort_msg("    in range of hostile ship!");
          return(1);
          }
       }
    }
  /* Robotschiffe */
  for (i=0;i<MAX_ROBOTSHIPS;i++)
    {
    if (m_p->robotship[enemy][i].status == ACTIVE)
       {
       if ( (IABS( (m_p->robotship[enemy][i].pos[0]-m_p->ship[vflag].pos[0]))) <= SHIP_RANGE
                           &&
	    (IABS( (m_p->robotship[enemy][i].pos[1]-m_p->ship[vflag].pos[1]))) <= SHIP_RANGE )
          {
          sort_msg("QU: You cannot quit the game");
          sort_msg("in range of hostile robotship!");
          return(1);
          }
       }
    }
    /* Basen */
  for (i=0;i<MAX_BASES;i++)
    {
    if ( m_p->base[i].party != party[vflag])
       {
       if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->base[i].pos[0]))) <= BASE_RANGE
                           &&
            (IABS( (m_p->ship[vflag].pos[1]-m_p->base[i].pos[1]))) <= BASE_RANGE )
          {
	  if (m_p->radarlee==ON && behind(m_p->base[i].pos[0],
	  m_p->base[i].pos[1],m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1],BASE_RANGE)==0)
	    {
            sort_msg("QU: You cannot quit the game");
            sort_msg("    within range of enemy base!");
            return(2);
            }
          }
       }
    }    
  /* Forts */
  for (i=0;i<MAX_FORTS;i++)
    {
    if (m_p->fort[i].party != party[vflag] && m_p->fort[i].status == ACTIVE)
       {
       if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->fort[i].pos[0]))) < FORT_RANGE
                           &&
            (IABS( (m_p->ship[vflag].pos[1]-m_p->fort[i].pos[1]))) < FORT_RANGE)
          {
	  if (m_p->radarlee==ON && behind(m_p->fort[i].pos[0],
	  m_p->fort[i].pos[1],m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1],FORT_RANGE)==0)
	    {
            sort_msg("QU: You cannot quit the game");
            sort_msg("    within range of enemy fort!");
            return(2);
            }
          }
       }
    }
  /* Planeten (fehlt noch) */
  /* Heimatwelt */
  if (party[vflag] == FEDERAL)
     {
     if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->homeworld[IMPERIAL].pos[0]))) <= HOMEWORLD_RANGE
                           &&
          (IABS( (m_p->ship[vflag].pos[1]-m_p->homeworld[IMPERIAL].pos[1]))) <= HOMEWORLD_RANGE )
        {
        sort_msg("QU: You cannot quit the game");
        sort_msg("    within range of the Imperial");
        sort_msg("    Homeworld");
        return(2);
        }
    }
  else
     {
     if ( (IABS( (m_p->ship[vflag].pos[0]-m_p->homeworld[FEDERAL].pos[0]))) <= HOMEWORLD_RANGE
                           &&
          (IABS( (m_p->ship[vflag].pos[1]-m_p->homeworld[FEDERAL].pos[1]))) <= HOMEWORLD_RANGE )
        {
        sort_msg("QU: You cannot quit the game");
        sort_msg("    within range of the Federal");
        sort_msg("    Homeworld");
        return(2);
        }
     }
  }
return(0);
}


int get_score(name)
    char name[];
/***************************************************************************/
/*** Auslesen der bislang erzielten Punkte des Spielern mit Namen name zur */
/*** Bestimmung des Ranges und damit des erlaubten Schiffstyps!            */
/***************************************************************************/

{
int sc;
char nam[10];
FILE *fp,*fopen();

fp=fopen("/usr/local/DEKKWAR/user.dat","r");
if (fp == NULLF)
   {
   sort_msg("Error user.dat!");
   }
else
   {
   while ( !feof(fp) )
      {
      fscanf(fp,"%s %d",nam,&sc);
      if (strcmp(nam,name)==0) 
         {
         fclose(fp);   
         return(sc);
         }
      } 
   fclose(fp);
   }   
return((-1));
}
/*************************************************************/
/* Verwendung des nanosleep von IRIX fuer das usleep von AIX */
/* Unter AIX ist usleep ein Systemaufruf und dieses Unter-   */
/* programm muss entfernt werden!                            */
/* #include <time.h> und <errno.h> im Hauptprogramm noetig!  */
/* ACHTUNG: fuer IRIX 6.1, fuer 6.2/IDO7.1 wie AIX!          */
/*          Dann TM entfernen!                                */ 
/*************************************************************/
/* Auskommentiert wegen Portierung auf Linux 
void TMusleep(msecs)
int msecs;
{
 int ierr;
 timespec_t rqtp, rmtp; 

 rqtp.tv_sec = 0;
 rqtp.tv_nsec = (long) msecs*1000;
 rmtp.tv_sec = 0;
 rmtp.tv_nsec = 0;

 ierr = nanosleep(&rqtp,&rmtp);
 if (ierr != 0)
   {
   printf("Fehler bei nanosleep: errno=%d (siehe <sys/errno.h>)\n",errno);
   }
}
*/

void infoall()
{
int i,treffer;

treffer=0;
printf("-------------------------------------------------------------------------------\n");
printf("BATTLESHIPS:\n");
printf("Names: ");
 for (i=1;i<MAX_USER;i++) /* 0 = Rom. */
   {
   if (strcmp(type_of_ship[i],"Battleship")==0) 
     { 
     printf("%s ",ships[i]);
     treffer=i;
     }
   }
printf("\n");
printf("Technical data: main energy=%d, shield energy=%d, damage capacity=%d\n",max_m_en[treffer],max_sh_en[treffer],max_dam[treffer]);
printf("                impulse fuel=%d, satellites=%d, mines=%d\n",max_fuel[treffer],max_sat[treffer],max_mines[treffer]);
 printf("Armament: torpedoes (%d, %d tubes), phaser (phaser energy=%d, max. %d p.shot)\n",max_torp[treffer],anz_ttubes[treffer],max_ph_en[treffer],max_ph[treffer]);
printf("-------------------------------------------------------------------------------\n");
treffer=0;
printf("CRUISERS:\n");
printf("Names: ");
 for (i=1;i<MAX_USER;i++) /* 0 = Rom. */
   {
   if (strcmp(type_of_ship[i],"Cruiser")==0) 
     { 
     printf("%s ",ships[i]);
     treffer=i;
     }
   }
printf("\n");
printf("Technical data: main energy=%d, shield energy=%d, damage capacity=%d\n",max_m_en[treffer],max_sh_en[treffer],max_dam[treffer]);
printf("                impulse fuel=%d, satellites=%d, mines=%d\n",max_fuel[treffer],max_sat[treffer],max_mines[treffer]);
 printf("Armament: torpedoes (%d, %d tubes), phaser (phaser energy=%d, max. %d p.shot)\n",max_torp[treffer],anz_ttubes[treffer],max_ph_en[treffer],max_ph[treffer]);
printf("-------------------------------------------------------------------------------\n");
treffer=0;
printf("DESTROYERS:\n");
printf("Names: ");
 for (i=1;i<MAX_USER;i++) /* 0 = Rom. */
   {
   if (strcmp(type_of_ship[i],"Destroyer")==0) 
     { 
     printf("%s ",ships[i]);
     treffer=i;
     }
   }
printf("\n");
printf("Technical data: main energy=%d, shield energy=%d, damage capacity=%d\n",max_m_en[treffer],max_sh_en[treffer],max_dam[treffer]);
printf("                impulse fuel=%d, satellites=%d, mines=%d\n",max_fuel[treffer],max_sat[treffer],max_mines[treffer]);
printf("Armament: torpedoes (%d, %d tube)\n",max_torp[treffer],anz_ttubes[treffer]);
printf("-------------------------------------------------------------------------------\n");
}

void select_ship(ppid1,ppid2,ppid4,com)
     char com[];
     int *ppid1,*ppid2,*ppid4;
     /* pid3 frueher torpedo */
/******************************************************/
/* Auswahl eines neuen Schiffs bei Heimatwelt-Docking */
/******************************************************/
{
int i,nflag,aflag,pid1,pid2,pid4;
char cs,cm[6],c1,c2,c4,pos_str[MSG_LENGTH],vflag_string[4];

if ( sscanf(com,"%s %c",cm,&cs) != 2 || m_p->ship[vflag].dock != HW_DOCKED)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }

nflag=det_target(cs);
  
/* Test: richtige Seite */
if (party[vflag]==FEDERAL)
  {
  if (nflag != 1 && nflag != 2 &&
      nflag != 5 && nflag != 6 && nflag != 7 && nflag != 8 &&
      nflag != 13 && nflag != 14 && nflag != 15 && nflag != 16 &&
      nflag != 17 && nflag != 18 && nflag != 25 && nflag != 26 &&
      nflag != 27 && nflag != 28 && nflag != 29 && nflag != 30 &&
      nflag != 31 && nflag != 32 && nflag != 33 && nflag != 34)
      {
      sort_msg("You must choose a Federal ship");
      return;
      }
  }
if (party[vflag]==IMPERIAL)
  {
  if (nflag == 1 || nflag == 2 ||
      nflag == 5 || nflag == 6 || nflag == 7 || nflag == 8 ||
      nflag == 13 || nflag == 14 || nflag == 15 || nflag == 16 ||
      nflag == 17 || nflag == 18 || nflag == 25 || nflag == 26 ||
      nflag == 27 || nflag == 28 || nflag == 29 || nflag == 30 ||
      nflag == 31 || nflag == 32 || nflag == 33 || nflag == 34)
      {
      sort_msg("You must choose an Imperial ship");
      return;
      }
  }

if (m_p->rank[vflag]==COMMANDER && nflag < 25)
  {
  sort_msg("     You are a Commander!");
  sort_msg("     Choose a destroyer!");
  return;
  }
if (m_p->rank[vflag]==CAPTAIN && nflag < 13)
  {
  sort_msg("     You are a Captain!");
  sort_msg("Choose a destroyer or cruiser!");
  return;
  }
if (m_p->rank[vflag]==COMMODORE && nflag < 5)
  {
  sort_msg("    You are a Commodore!");
  sort_msg("Choose a destroyer, cruiser,");
  sort_msg("or battleship!");
  return;
  }

if (m_p->luser[nflag]==ACTIVE) 
  {
  sprintf(pos_str," %s already in game!",ships[nflag]);
  sort_msg(pos_str);
  return;
  }
if (m_p->l_exist[nflag]==DESTROYED) 
  {
  sprintf(pos_str,"   %s destroyed!",ships[nflag]);
  sort_msg(pos_str);
  return;
  }


aflag=vflag;

m_p->luser[nflag]=ACTIVE;

m_p->l_exit[vflag]=NO_EXIT;
m_p->l_exit[nflag]=NO_EXIT;

strcpy(m_p->cmd[vflag],"                   ");
strcpy(m_p->cmd[nflag],"                   ");

m_p->ship[nflag].pos[0]=m_p->ship[vflag].pos[0];
m_p->ship[nflag].pos[1]=m_p->ship[vflag].pos[1];

m_p->ship[vflag].pos[0]=-99;
m_p->ship[vflag].pos[1]=-99;

m_p->ship[vflag].torp=max_torp[vflag];
m_p->ship[nflag].torp=max_torp[nflag];

m_p->ship[vflag].sat=max_sat[vflag];
m_p->ship[nflag].sat=max_sat[nflag];

m_p->ship[vflag].mines=max_mines[vflag];
m_p->ship[nflag].mines=max_mines[nflag];

m_p->ship[vflag].fuel=max_fuel[vflag];
m_p->ship[nflag].fuel=max_fuel[nflag];

m_p->ship[vflag].hood=OFF;
m_p->ship[nflag].hood=OFF;

m_p->ship[vflag].move=OFF;
m_p->ship[nflag].move=OFF;

m_p->ship[vflag].phaser=OFF;
m_p->ship[nflag].phaser=OFF;

m_p->display_status[vflag]=DISPLAY_HW;
m_p->display_status[nflag]=DISPLAY_RADAR;

m_p->ship[vflag].build=OFF;
m_p->ship[nflag].build=OFF;

m_p->ship[vflag].ramm=OFF;
m_p->ship[nflag].ramm=OFF;

m_p->ship[vflag].main_energy=max_m_en[vflag];
m_p->ship[nflag].main_energy=max_m_en[nflag];

m_p->ship[vflag].sh_energy=max_sh_en[vflag];
m_p->ship[nflag].sh_energy=max_sh_en[nflag];

m_p->ship[vflag].ph_energy=max_ph_en[vflag];
m_p->ship[nflag].ph_energy=max_ph_en[nflag];

m_p->ship[nflag].dam[0]=0;
m_p->ship[nflag].dam[1]=0;
m_p->ship[nflag].dam[2]=0;
m_p->ship[nflag].dam[3]=0;
m_p->ship[nflag].dam[4]=0;
m_p->ship[nflag].dam[5]=0;
m_p->ship[nflag].dam[6]=0;
m_p->ship[nflag].dam[7]=0;
m_p->ship[nflag].dam[8]=0;
m_p->ship[nflag].dam[9]=0;
m_p->ship[nflag].dam[10]=0;

m_p->ship[vflag].shield=OFF;
m_p->ship[nflag].shield=OFF;

m_p->ship[vflag].sv=OFF;
m_p->ship[nflag].sv=OFF;

m_p->ship[vflag].mp=OFF;
m_p->ship[nflag].mp=OFF;

m_p->ship[vflag].imp=0;
m_p->ship[nflag].imp=0;

m_p->rank[nflag]=m_p->rank[vflag];

m_p->ship[vflag].dock=HW_DOCKED;  /* Wichtig!!! */
m_p->ship[nflag].dock=HW_DOCKED;

m_p->display_status[nflag]=m_p->display_status[vflag];
m_p->name_score[nflag]=m_p->name_score[vflag];
m_p->user_score[nflag]=0;

strcpy(m_p->name_of_cmdr[nflag],m_p->name_of_cmdr[vflag]);

strcpy(m_p->rank_of_name[nflag],m_p->rank_of_name[vflag]);
strncpy(m_p->cmd[nflag],m_p->cmd[vflag],33);

strcpy(m_p->ship[nflag].msg[0],m_p->ship[vflag].msg[0]);
strcpy(m_p->ship[nflag].msg[1],m_p->ship[vflag].msg[1]);
strcpy(m_p->ship[nflag].msg[2],m_p->ship[vflag].msg[2]);
strcpy(m_p->ship[nflag].msg[3],m_p->ship[vflag].msg[3]);
strcpy(m_p->ship[nflag].msg[4],m_p->ship[vflag].msg[4]);
strcpy(m_p->ship[nflag].msg[5],m_p->ship[vflag].msg[5]);
strcpy(m_p->ship[nflag].msg[6],m_p->ship[vflag].msg[6]);

for (i=0;i<anz_ttubes[nflag];i++)
   {
   m_p->ship[nflag].status_ttubes[i]=LOADED;
   }

vflag=nflag;

/* alte Prozesse killen */
kill(*ppid1,SIGKILL); /* cockpit */
kill(*ppid2,SIGKILL); /* move */
if (level_of_ship[aflag] != DESTROYER) kill(*ppid4,SIGKILL);  /* Phaser */

sprintf(vflag_string,"%d",vflag);
c1=c2=c4='-';

/* neue Prozesse starten */

pid1 = fork();
if (pid1==0)
   {
   execl("cockpit",vflag_string,NULL);
   } 
else if (pid1 == (-1))
   {
   sort_msg("Fork fuer COCKPIT fehlgeschlagen!");
   }
else
   {
   c1='+';
   *ppid1=pid1;
   }

pid2 = fork();
if (pid2==0)
   {
   execl("move",vflag_string,NULL);
   } 
else if (pid2 == (-1))
   {
   sort_msg("Fork fuer MOVE fehlgeschlagen!");
   }
else
   {
   c2='+';
   *ppid2=pid2;
   }


if (level_of_ship[vflag] != DESTROYER)  /* Phaser */
  {
  pid4 = fork();
  if (pid4==0)
     {
     execl("phaser",vflag_string,NULL);
     } 
  else if (pid4 == (-1))
     {
     sort_msg("Fork fuer PHASER fehlgeschlagen!");
     }
  else
    {
    c4='+';
    *ppid4=pid4;
    }
  }

strcpy(pos_str,"HW: Select  : C  M  P   ");
pos_str[11]=cs;
pos_str[15]=c1;
pos_str[18]=c2;
pos_str[21]=c4;
sort_msg(pos_str);
sort_msg("HW: Congratulation, you are now");
sort_msg("  the commanding officer of the");
strcpy(pos_str,kind_of_ship[vflag]);
strcat(pos_str," ");
strcat(pos_str,ships[vflag]);
sort_msg(pos_str);
m_p->luser[aflag]=DESTROYED;
m_p->name_score[aflag]=0;
m_p->user_score[aflag]=0;
strcpy(m_p->name_of_cmdr[aflag],"?");
strcpy(m_p->rank_of_name[aflag],"Commander");
strcpy(m_p->cmd[aflag],"  ");
m_p->rank[aflag]=COMMANDER;
strcpy(m_p->ship[aflag].msg[0],"   ");
strcpy(m_p->ship[aflag].msg[1],"   ");
strcpy(m_p->ship[aflag].msg[2],"   ");
strcpy(m_p->ship[aflag].msg[3],"   ");
strcpy(m_p->ship[aflag].msg[4],"   ");
strcpy(m_p->ship[aflag].msg[5],"   ");
strcpy(m_p->ship[aflag].msg[6],"   ");

alarm(2);
pause();

}
void start_ship()
/***************************************************/
/*** Starten von der Heimatwelt oder einer Basis ***/
/***************************************************/
{
int i, zae, nrb;
float random;

/* Basis */
if (m_p->ship[vflag].dock == DOCKED)
  {
  nrb=find_nr_base(m_p->ship[vflag].pos[0],m_p->ship[vflag].pos[1]);
  if (m_p->base[nrb].dam > ((MAX_DAM_BAS/10)*9) )
    {
    sort_msg("Cannot undock from base!");
    sort_msg("Base severely damaged!");
    return;
    }
  zae=0;
  i=-1;
  do {
   random=ran();
   i--;
   /* Im Uhrzeigersinn um die Homeworld/Basis herum, 8 Moeglichkeiten! */
   zae++;
   if (random < 0.125) 
     {	
     if (m_p->a[m_p->ship[vflag].pos[0]-1][m_p->ship[vflag].pos[1]+1] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1]+1;
        }
     }
   else if (random >= 0.125 && random < 0.25)
     {	
     if (m_p->a[m_p->ship[vflag].pos[0]-1][m_p->ship[vflag].pos[1]] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1];
        }
     }
   else if (random >= 0.25 && random < 0.375)
     {	
       if (m_p->a[m_p->ship[vflag].pos[0]-1][m_p->ship[vflag].pos[1]-1] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1]-1;
        }
     }
   else if (random >= 0.375 && random < 0.5)
     {	
     if (m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]-1] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0];
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1]-1;
        }
     }
   else if (random >= 0.5 && random < 0.625)
     {
     if (m_p->a[m_p->ship[vflag].pos[0]+1][m_p->ship[vflag].pos[1]-1] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1]-1;
        }	
     }
   else if (random >= 0.625 && random < 0.75)
     {	
     if (m_p->a[m_p->ship[vflag].pos[0]+1][m_p->ship[vflag].pos[1]] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1];
        }
     }
   else if (random >= 0.75 && random < 0.875)
     {	
     if (m_p->a[m_p->ship[vflag].pos[0]+1][m_p->ship[vflag].pos[1]+1] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1]+1;
        }
     }
   else
     {	
     if (m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]+1] == SPACE)
	{
        m_p->ship[vflag].pos[0]=m_p->ship[vflag].pos[0];
        m_p->ship[vflag].pos[1]=m_p->ship[vflag].pos[1]+1;
        }
     }
   } while (m_p->ship[vflag].pos[0]==m_p->base[nrb].pos[0] &&
            m_p->ship[vflag].pos[1]==m_p->base[nrb].pos[1] && zae <= 99);

  if (zae > 99) 
     {
     sort_msg("Cannot undock! We are trapped");
     sort_msg("in base! Call for help!");
     return;
     }

  m_p->base[nrb].sh=OFF;
  m_p->ship[vflag].dock=NOT_DOCKED;
  m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
  m_p->display_status[vflag]=DISPLAY_RADAR;
  if (m_p->base[nrb].bay[0]==ships[vflag][0])
     {
     m_p->base[nrb].bay[0]=' ';
     }
  else if (m_p->base[nrb].bay[1]==ships[vflag][0])
     {
     m_p->base[nrb].bay[1]=' ';
     }
  else if (m_p->base[nrb].bay[2]==ships[vflag][0])
     {
     m_p->base[nrb].bay[2]=' ';
     }
  else if (m_p->base[nrb].bay[3]==ships[vflag][0])
     {
     m_p->base[nrb].bay[3]=' ';
     }
  else
    {
    sort_msg("DO: Error! Bay flag wrong!");
    }
  usleep(10000);
  sort_msg("DO: Docking finished!");
  if (m_p->base[nrb].sh_energy > 0) m_p->base[nrb].sh=ON;
  return;
  }

/* Heimatwelt */

if (m_p->ship[vflag].dock != HW_DOCKED)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }

if (m_p->homeworld[party[vflag]].dam > ((MAX_DAM_HOMEWORLD/10)*9) )
  {
  sort_msg("Cannot undock from Homeworld!");
  sort_msg("Homeworld is severely damaged!");
  return;
  }

zae=0;
i=-1;
do {
   random=ran();
   i--;
   /* Im Uhrzeigersinn um die Heimatwelt herum, 8 Moeglichkeiten! */
   zae++;
   if (random < 0.125) 
     {	
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]-1][m_p->homeworld[party[vflag]].pos[1]+1] == SPACE) /* l.o. */
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1]+1;
        }
     }
   else if (random >= 0.125 && random < 0.25)
     {	
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]-1][m_p->homeworld[party[vflag]].pos[1]] == SPACE) /* l. */
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1];
        }
     }
   else if (random >= 0.25 && random < 0.375)
     {	
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]-1][m_p->homeworld[party[vflag]].pos[1]-1] == SPACE) /* l.u. */ 
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0]-1;
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1]-1;
        }
     }
   else if (random >= 0.375 && random < 0.5)
     {	
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]][m_p->homeworld[party[vflag]].pos[1]-1] == SPACE) /* u. */
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0];
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1]-1;
        }
     }
   else if (random >= 0.5 && random < 0.625)
     {
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]+1][m_p->homeworld[party[vflag]].pos[1]-1] == SPACE) /* r.u. */
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1]-1;
        }	
     }
   else if (random >= 0.625 && random < 0.75)
     {	
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]+1][m_p->homeworld[party[vflag]].pos[1]] == SPACE) /* r. */
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1];
        }
     }
   else if (random >= 0.75 && random < 0.875)
     {	
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]+1][m_p->homeworld[party[vflag]].pos[1]+1] == SPACE) /* r.o. */
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0]+1;
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1]+1;
        }
     }
   else
     {	
     if (m_p->a[m_p->homeworld[party[vflag]].pos[0]][m_p->homeworld[party[vflag]].pos[1]+1] == SPACE) /* o. */
	{
        m_p->ship[vflag].pos[0]=m_p->homeworld[party[vflag]].pos[0];
        m_p->ship[vflag].pos[1]=m_p->homeworld[party[vflag]].pos[1]+1;
        }
     }
   } while (m_p->ship[vflag].pos[0]==m_p->homeworld[party[vflag]].pos[0] &&
           m_p->ship[vflag].pos[1]==m_p->homeworld[party[vflag]].pos[1] && zae <= 99);
/* Position ist HW waehrend HW-Docking */

if (zae > 99) 
   {
   sort_msg("Cannot undock! We are trapped");
   sort_msg("in homeworld! Call for help!");
   return;
   }

usleep(10000);
m_p->ship[vflag].dock=NOT_DOCKED;
m_p->display_status[vflag]=DISPLAY_RADAR;
m_p->a[m_p->ship[vflag].pos[0]][m_p->ship[vflag].pos[1]]=ships[vflag][0];
return;
}

void restart_procs(ppid1,ppid2,ppid4)
/*************************************************************************/
/*** restart_procs: Cockpit,Move und ggf. Phaser restarten     ***/
/*************************************************************************/
int *ppid1,*ppid2,*ppid4;
/* ppid3 frueher torpedo */
{
int pid1,pid2,pid4;
char vflag_string[10]; 

sprintf(vflag_string,"%d",vflag);

/* alte Prozesse killen */
kill(*ppid1,SIGKILL); /* cockpit */
kill(*ppid2,SIGKILL); /* move */
if (level_of_ship[vflag] != DESTROYER) kill(*ppid4,SIGKILL);  /* Phaser */

/* neue Prozesse starten */
pid1 = fork();
if (pid1==0)
   {
   execl("cockpit",vflag_string,NULL);
   } 
else if (pid1 == (-1))
   {
   sort_msg("Fork for COCKPIT failed!");
   }
else
   {
   sort_msg("COCKPIT restarted!");
   *ppid1=pid1;
   }

pid2 = fork();
if (pid2==0)
   {
   execl("move",vflag_string,NULL);
   } 
else if (pid2 == (-1))
   {
   sort_msg("Fork for MOVE failed!");
   }
else
   {
   sort_msg("MOVE restarted!");
   *ppid2=pid2;
   }

if (level_of_ship[vflag] != DESTROYER)  /* Phaser */
  {
  pid4 = fork();
  if (pid4==0)
     {
     execl("phaser",vflag_string,NULL);
     } 
  else if (pid4 == (-1))
     {
     sort_msg("Fork for PHASER failed!");
     }
  else
    {
     sort_msg("PHASER restarted!");
    *ppid4=pid4;
    }
  }
return;
}
/*****************************************************************************/
/*** build_fort.c                                                          ***/
/*****************************************************************************/
/*** Bau eines Forts                                                       ***/
/*** Neue Version fuer struct fort                                         ***/
/*****************************************************************************/
void build_fort(com)
     char com[];
{
int x,y,nrf,i,del_time;
char cm[2];

/* Kommandostringauswertung */
if ( sscanf(com,"%s%d%d",cm,&x,&y) != 3)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }

/* x und y verifizieren */
if ( x<0 || x>=ARRAY_SIZE || y<0 || y>=ARRAY_SIZE ||
     ((m_p->ship[vflag].pos[0]-x)  == 0 && (m_p->ship[vflag].pos[1]-y) == 0) ||
     (IABS((x-m_p->ship[vflag].pos[0]))) > 1 || (IABS((y-m_p->ship[vflag].pos[1]))) > 1 ||
     m_p->a[x][y] != SPACE )
   {
   sort_msg("FO: Wrong coordinates!");
   return;
   }


if (m_p->ship[vflag].shield==ON)
   {
   sort_msg("FO: Shields must be down!");
   return;
   }
if (m_p->ship[vflag].hood==ON)
   {
   sort_msg("FO: Hood must be off!");
   return;
   }

/* Suchen eines "freien" Forts */
nrf=-99;
i=0;
while (nrf < 0 && i < MAX_FORTS)
    {
    if (m_p->fort[i].status == DESTROYED)
       {
       m_p->fort[i].status = CLAIMED;
       nrf=i;
       }
    i++;
    }
if (nrf < 0)
   {
   sort_msg("FO: Unable to build fort!");
   sort_msg("FO: Maximum number exceeded!");
   return;
   }
/* Alles o.k.- Ausbau in Index Nr. nrf */
sort_msg("FO: Building fort...");
del_time=(NINT( (4.0+(2.0 * ((float) (5-level_of_ship[vflag])))) )); 
alarm(del_time);
pause();
m_p->fort[nrf].party = party[vflag];
m_p->fort[nrf].pos[0] = x;
m_p->fort[nrf].pos[1] = y;
m_p->fort[nrf].dam = 0;
m_p->fort[nrf].status = ACTIVE;
if (party[vflag] == FEDERAL) 
   {
   m_p->a[x][y]=F_FORT;
   }
else
   { 
   m_p->a[x][y]=E_FORT;
   }
sort_msg("FO: New fort built!");
m_p->user_score[vflag]=m_p->user_score[vflag]+40;
m_p->name_score[vflag]=m_p->name_score[vflag]+40;
m_p->score[party[vflag]]=m_p->score[party[vflag]]+40;
m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-FORT_BUILD_RESS;

return;                   
}

/*****************************************************************************/
/*** build_jumpgate.c                                                      ***/
/*****************************************************************************/
/*** Bau eines Sprungtores                                                 ***/
/*****************************************************************************/
void build_jumpgate(com)
     char com[];
{
int x,y,nrjg,codenr,i,del_time;
char cm[2];

/* Kommandostringauswertung */
if ( sscanf(com,"%s%d%d%d",cm,&x,&y,&codenr) != 4)
   {
   sort_msg("       *** BAD COMMAND ***");
   return;
   }

/* x und y verifizieren */
if ( x<1 || x>=ARRAY_SIZE-1 || y<1 || y>=ARRAY_SIZE-1 ||
     ((m_p->ship[vflag].pos[0]-x)  == 0 && (m_p->ship[vflag].pos[1]-y) == 0) ||
     (IABS((x-m_p->ship[vflag].pos[0]))) > 1 || (IABS((y-m_p->ship[vflag].pos[1]))) > 1 || m_p->a[x][y] != SPACE )
   {
   sort_msg("JG: Wrong coordinates!");
   return;
   }

if (m_p->ship[vflag].shield==ON)
   {
   sort_msg("JG: Shields must be down!");
   return;
   }

if (m_p->ship[vflag].main_energy <= 3500)
   {
   sort_msg("JG: Cannot build jumpgate!");
   sort_msg("    Too less energy!");
   return;
   }

if (level_of_ship[vflag]<BATTLESHIP)
   {
   sort_msg("JG: Only a battleship can");
   sort_msg("    build a jumpgate!");
   return;
   }

if (m_p->rank[vflag]<ADMIRAL)
   {
   sort_msg("JG: Cannot build jumpgate!");
   sort_msg("    You must be Admiral!");
   return;
   }

if (m_p->ship[vflag].hood==ON)
   {
   sort_msg("JG: Hood must be off!");
   return;
   }

/* Suchen eines "freien" Sprungtores */
for (i=0;i<MAX_JUMPGATES;i++)
  {
  if (m_p->jumpgate[i].code == codenr || codenr < 10 || codenr > 99)
     {
     sort_msg("JG: Invalid jumpgate code!");
     sort_msg("    valid code: 10 to 99");
     return;
     }
  }

nrjg=-1;
i=0;
while (nrjg < 0 && i < MAX_JUMPGATES)
    {
    if (m_p->jumpgate[i].status == DESTROYED)
       {
       m_p->jumpgate[i].status = CLAIMED;
       nrjg=i;
       }
    i++;
    }
if (nrjg < 0)
   {
   sort_msg("JG: Cannot build jumpgate!");
   sort_msg("JG: Maximum number exceeded!");
   return;
   }
/* Alles o.k.- Ausbau in Index Nr. nrjg */
sort_msg("JG: Building jumpgate ...");
m_p->ship[vflag].build=ON;
del_time=2;
for (i=0;i<20;i++)
   {
     m_p->ship[vflag].main_energy=m_p->ship[vflag].main_energy-(JUMPGATE_BUILD_RESS/20);
   alarm(del_time);
   pause();
   }
m_p->jumpgate[nrjg].pos[0] = x;
m_p->jumpgate[nrjg].pos[1] = y;
m_p->jumpgate[nrjg].dam = 0;
m_p->jumpgate[nrjg].status = ACTIVE;
m_p->jumpgate[nrjg].code = codenr;
m_p->jumpgate[nrjg].party = party[vflag];

m_p->a[x][y]=JUMPGATE;

m_p->ship[vflag].build=OFF;

sort_msg("JG: New jumpgate built!");
m_p->user_score[vflag]=m_p->user_score[vflag]+100;
m_p->name_score[vflag]=m_p->name_score[vflag]+100;
m_p->score[party[vflag]]=m_p->score[party[vflag]]+100;


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
/*** Entfernt den Satelliten Nr.nrs aus den Satelliten-Werten, nicht***/
/*** das Symbol aus a[][]. Setzt die Werte der Satelliten > nrs um  ***/
/**********************************************************************/
{

if (nrs<0 || nrs>=MAX_SATS) sort_msg("ERROR: wrong sat count!");
m_p->satellite[nrs].pos[0]=-99;       
m_p->satellite[nrs].pos[1]=-99;             
m_p->satellite[nrs].party=NEUTRAL;

return;
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
   sort_msg("  Star exploded; NOVA!");
   if (incident(25)==0)
      {
      m_p->a[x][y]=SPACE;
      }
   else
      {
      m_p->a[x][y]=BLACK_HOLE;
      sort_msg("Star turned into black hole!");
      }
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
          sprintf(print_str,"Collision: jumpgate at %03d %03d",x,y);
          send_msg(i,print_str);
          }
       }
   if (m_p->jumpgate[nr].dam>=MAX_DAM_JG)
      {
      eliminate_jumpgate(nr);
      init_expl(x,y,EXPL_TIME);
      sprintf(print_str,"Jumpgate at %03d %03d destroyed!",x,y);
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
      sort_msg("Error hit (missile)!");
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
   m_p->missile[mparty].dam=m_p->missile[mparty].dam+energy;
   usleep(EXPL_TIME); /* Wartezeit, damit Missile explodieren kann */
   m_p->missile[mparty].status=DESTROYED; 
   }       /* Ende missile */   

/**************/
/* Heimatwelt */
/**************/
if (obj==F_HOMEWORLD)
   {
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
   m_p->homeworld[FEDERAL].condition=RED;
   m_p->homeworld[FEDERAL].red_count=MAX_RED_COUNT;
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
      sprintf(print_str,"Fed.HW at %03d %03d destroyed!",x,y); 
      sort_msg(print_str);
      m_p->user_score[vflag]=m_p->user_score[vflag]+pntsd;
      m_p->name_score[vflag]=m_p->name_score[vflag]+pntsd;   
      m_p->score[party[vflag]]=m_p->score[party[vflag]]+pntsd;
      return; 
      }
   return;
   }
if (obj==E_HOMEWORLD)
   {
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
   m_p->homeworld[IMPERIAL].condition=RED;
   m_p->homeworld[IMPERIAL].red_count=MAX_RED_COUNT;
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
      sprintf(print_str,"Imp.HW at %03d %03d destroyed!",x,y); 
      sort_msg(print_str);
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
          sprintf(print_str,"Robotship at %03d %03d under fire",x,y);
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
          sprintf(print_str,"Robotship at %03d %03d under fire",x,y);
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
zflag=(-1);
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
      sprintf(print_str,"%s attacked by %c",ships[zflag],ships[vflag][0]);
      send_msg(zflag,print_str);
      }
   sprintf(print_str,"** We got a %d units hit! **",energy);
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
         sprintf(print_str,"UFO at %03d %03d destroyed",x,y);
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
   sprintf(print_str,"Satellite on %03d %03d destroyed",x,y);
   sort_msg(print_str);
   m_p->user_score[vflag]=m_p->user_score[vflag]+pnts;
   m_p->name_score[vflag]=m_p->name_score[vflag]+pnts;
   m_p->score[party[vflag]]=m_p->score[party[vflag]]+pnts;
   return;
   }

sort_msg(" ERROR fehlerhaftes Objekt!");
return;
}

int incident(w)
    int w;
/************************************************************************/
/*** berechnet, ob das Ereignis mit der Wahrscheinlchkeit w eintrifft  **/
/*** w = 1...100 , incident=0 =nicht eing.                            ***/
/***   Achtung: clock-Aufruf im Hauptprogramm noetig!                 ***/
/************************************************************************/
{
int diff;
long time_ms, time_d;

time_ms=clock();
time_d=time_ms/100;
time_d=time_d*100;
diff= (int) (time_ms-time_d);

if (diff<w) 
   {
   return(1);
   }
else
   {
   return(0);
   }

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

int behind(x,y,x0,y0,range)
    int x,y,x0,y0,range;
/**************************************************************************/
/** Ueberprueft, ob die Position x,y im Radarlee von Position x0,y0 liegt */
/**************************************************************************/
{
int xsteps,ysteps,steps,i;
int xalt,yalt,xneu,yneu,cond;
float rxdiff,rydiff;

xsteps=x-x0;
ysteps=y-y0;

 steps=(IMAX(abs(xsteps),abs(ysteps)));

if (steps==0) return(0);
if (steps>range) return(1);

rxdiff=(float) xsteps/ (float) steps;
rydiff=(float) ysteps/ (float) steps;

xalt=x0;
yalt=y0;
i=1;
cond=0;

while (i<steps && cond==0)
  {
  xneu=xalt+ (NINT( (((float) i)*rxdiff) ));
  yneu=yalt+ (NINT( (((float) i)*rydiff) ));
  if (m_p->a[xneu][yneu] == CLOUD) cond=1;
  i++;
  }  

return(cond);
}

int iproz(i1,i2)
     int i1,i2;
/****************************************************************************/
/***      Berechnet, wie % von i2 i1 entspricht                           ***/
/****************************************************************************/
{
  int i3;
  float x,y;

  x=(float) i1;
  y=(float) i2;
  if (i2 == 0) y=0.00000001;

  i3 = (NINT( ((x/y)*100.0) ));
  return(i3);
}

void build_robotship()
/************************************************/
/* Bau eines Robotschiffs                       */
/************************************************/
{  
int i,nrob;
char str[MSG_LENGTH];

nrob=-1;

for (i=0;i<MAX_ROBOTSHIPS;i++)
   {
   if (m_p->robotship[party[vflag]][i].status == DESTROYED) nrob=i;
   }

if (nrob<0)
  {
  sort_msg("BR: Could not build robotship,");
  sprintf(str," %d already operational!",MAX_ROBOTSHIPS);
  sort_msg(str);
  return;
  }

m_p->robotship[party[vflag]][nrob].status=UNDER_CONSTRUCTION;
m_p->robotship[party[vflag]][nrob].mission='N';
m_p->robotship[party[vflag]][nrob].pos[0]=m_p->homeworld[party[vflag]].pos[0];
m_p->robotship[party[vflag]][nrob].pos[1]=m_p->homeworld[party[vflag]].pos[1];
m_p->robotship[party[vflag]][nrob].torp=ROBOTSHIP_TORPEDOES;
m_p->robotship[party[vflag]][nrob].fuel=ROBOTSHIP_FUEL;
m_p->robotship[party[vflag]][nrob].main_energy=ROBOTSHIP_ENERGY;
m_p->robotship[party[vflag]][nrob].sh_energy=ROBOTSHIP_SH_ENERGY;
m_p->robotship[party[vflag]][nrob].shield=OFF;
m_p->robotship[party[vflag]][nrob].dam=0;
sort_msg("BR: Building robotship...");
usleep(ROBOTSHIP_BUILD_TIME);
m_p->robotship[party[vflag]][nrob].status=HW_DOCKED;
sprintf(str,"BR: robotship built; no. %d",nrob);
sort_msg(str);
return;
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

void save_path(com)
     char com[];
/**************************************************************************/
/*** Abspeichern einer lokalen Missile-Pfadaufzeichnung                 ***/
/**************************************************************************/
{
  int i,pnr;
  char cm[2];
	
/* Kommandostring interpretieren */
if (sscanf(com,"%s %d",cm,&pnr) != 2)
   {
   sort_msg("    *** BAD COMMAND ***");
   return;
   }

if (pnr < 0 || pnr >= MAX_MPATHS)
   {
   sort_msg("SP: bad path!");
   return;
   }

if (m_p->ship[vflag].imp < 1 || m_p->ship[vflag].imp > MISSILE_FUEL)
  {
  sort_msg("SP: bad path!");
  return;
  }

for (i=0;i<=MISSILE_FUEL;i++)
  {
  m_p->mpath[party[vflag]][pnr][i][0]=m_p->ship[vflag].mpath[i][0];
  m_p->mpath[party[vflag]][pnr][i][1]=m_p->ship[vflag].mpath[i][1];
  }
m_p->imp[party[vflag]][pnr]=m_p->ship[vflag].imp;
sort_msg("SP: recorded path data saved!");
return;
}

void clear_path(com)
     char com[];
/**************************************************************************/
/*** Loeschen einer lokalen Missile-Pfadaufzeichnung                 ***/
/**************************************************************************/
{
  int i,pnr;
  char cm[2];
	
/* Kommandostring interpretieren */
if (sscanf(com,"%s %d",cm,&pnr) != 2)
   {
   sort_msg("    *** BAD COMMAND ***");
   return;
   }

if (pnr < 0 || pnr >= MAX_MPATHS)
   {
   sort_msg("CP: bad path!");
   return;
   }

if (m_p->imp[party[vflag]][pnr] == 0)
  {
  sort_msg("CP: path already empty!");
  return;
  }

for (i=0;i<=MISSILE_FUEL;i++)
  {
  m_p->mpath[party[vflag]][pnr][i][0]=-99;
  m_p->mpath[party[vflag]][pnr][i][1]=-99;
  }
m_p->imp[party[vflag]][pnr]=0;
sort_msg("CP: path data deleted!");
return;
}


void kill_robotship(com)
     char com[];
/**************************************************************************/
/*** Selbstzerstoerung eines Robotschiffs                               ***/
/**************************************************************************/
{
  int i,nr,cnt;
  char cm[2],cms[2],str1[MSG_LENGTH];
	
/* Kommandostring interpretieren */
if (sscanf(com,"%s %s",cm,cms) != 2)
   {
   sort_msg("    *** BAD COMMAND ***");
   return;
   }
cnt=0;
nr=-1;
if (cms[0]=='0') nr=0;
if (cms[0]=='1') nr=1;
if (cms[0]=='2') nr=2;
if (cms[0]=='3') nr=3;
if (cms[0]=='4') nr=4;
if (cms[0]=='5') nr=5;
if (cms[0]=='6') nr=6;
if (cms[0]=='7') nr=7;
if (cms[0]=='8') nr=8;
if (cms[0]=='9') nr=9;
if (cms[0]=='a' || cms[0]=='A') nr=-2;

if (nr==-1)
   {
   sort_msg("KR: bad command");
   }
else if (nr==-2) /* Alle RS im Status ACTIVE oder WAITING zerstoeren */
   {
   for (i=0;i<MAX_ROBOTSHIPS;i++)
     {
     if (m_p->robotship[party[vflag]][i].status==ACTIVE
                         ||
         m_p->robotship[party[vflag]][i].status==WAITING)
	{
        m_p->robotship[party[vflag]][i].status=DESTROYED;
        cnt++;
        }
     }
   if (cnt>0) 
     {
     sprintf(str1,"KR: %d active RSs destroyed!",cnt);
     sort_msg(str1);
     }
   else
     {
     sort_msg("KR: No active Robotships!");
     }
   }
else
   {
   if (m_p->robotship[party[vflag]][nr].status==ACTIVE
                         ||
       m_p->robotship[party[vflag]][nr].status==WAITING)
      {
      m_p->robotship[party[vflag]][nr].status=DESTROYED;
      sprintf(str1,"KR: Robotship %d destroyed!",nr);
      sort_msg(str1);
      }
   else
      {
      sprintf(str1,"KR: Robotship %d not active!",nr);
      sort_msg(str1);
      }
   }
return;
}

void call_back_robotship(com)
     char com[];
/**************************************************************************/
/*** Rueckruf eines Robotschiffs                                        ***/
/**************************************************************************/
{
  int i,nr,cnt;
  char cm[2],cms[2],str1[MSG_LENGTH];
	
/* Kommandostring interpretieren */
if (sscanf(com,"%s %s",cm,cms) != 2)
   {
   sort_msg("    *** BAD COMMAND ***");
   return;
   }
cnt=0;
nr=-1;
if (cms[0]=='0') nr=0;
if (cms[0]=='1') nr=1;
if (cms[0]=='2') nr=2;
if (cms[0]=='3') nr=3;
if (cms[0]=='4') nr=4;
if (cms[0]=='5') nr=5;
if (cms[0]=='6') nr=6;
if (cms[0]=='7') nr=7;
if (cms[0]=='8') nr=8;
if (cms[0]=='9') nr=9;
if (cms[0]=='a' || cms[0]=='A') nr=-2;

if (nr==-1)
   {
   sort_msg("CR: bad command");
   }
else if (nr==-2) /* Alle RS im Status ACTIVE auf CANCELED setzen */
   {
   for (i=0;i<MAX_ROBOTSHIPS;i++)
     {
     if (m_p->robotship[party[vflag]][i].status==ACTIVE)
	{
        m_p->robotship[party[vflag]][i].status=CANCELED;
        cnt++;
        }
     }
   if (cnt>0) 
     {
     sprintf(str1,"CR: %d active RSs called back!",cnt);
     sort_msg(str1);
     }
   else
     {
     sort_msg("CR: No active Robotships!");
     }
   }
else
   {
   if (m_p->robotship[party[vflag]][nr].status==ACTIVE)
      {
      m_p->robotship[party[vflag]][nr].status=CANCELED;
      sprintf(str1,"CR: Robotship %d called back!",nr);
      sort_msg(str1);
      }
   else
      {
      sprintf(str1,"CR: Robotship %d not active!",nr);
      sort_msg(str1);
      }
   }
return;
}
