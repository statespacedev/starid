Es sind möglicherweise Pakete nötig, die in der Basisversion der Linux-Dist nicht enthalten sind, z.B. ncurses.


DECWAR kompilieren:
./comp.linux

Spiel ganz neu starten:
1) Shared Memory einrichten
./shm_inst
2) Galaxis kreieren:
./init_gal
3) Basenverteidigung starten (optional, ist sonst aber langweilig, nur einmal starten,)
./def & > /dev/null 2>&1 (oder um die Meldungen zu sehen, in eigenem Terminal-Fenster starten)
4) Romulaner starten (optional, machts interessanter, nur einmal starten)
./rom & > /dev/null 2>&1 (oder um die Meldungen zu sehen, in eigenem Terminal-Fenster starten)
3) Selber einsteigen:
./dwmain

Wenn shm und Galaxis schon existieren, entfällt 1) und 2)

Galaxis und shm löschen:

./shm_rem

Bei mehreren Spielern die Executables am besten in /usr/local/bin kopieren. Ein Master-Spieler muß dann für das shm, die Galaxis und def und rom zuständig sein.
