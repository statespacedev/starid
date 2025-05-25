#include <signal.h>
#include <curses.h>
#include "config.h"
#include "uniwar.h"

extern WINDOW	*cmdwin;	/* window where commands are entered */
extern int errno;

char *getenv();

/*
 * doshell -- fork a shell
 */
void
doshell()
{
	int (*oldsig1)();
	int (*oldsig2)();
	char *sh;

	if ((sh = getenv("SHELL")) == NULL)
		sh = "/bin/sh";

	/* prevent screwing up parent */
	oldsig1 = signal(SIGINT, (int(*)())(SIG_IGN));
	oldsig2 = signal(SIGQUIT, (int(*)())(SIG_IGN));

	switch(fork()) {
	case 0:
		/* child */
		nodelay(cmdwin, FALSE);
		endwin();		/* cleans up windows */
		if (execl(sh, sh, 0) < 0) {
			printf("can't exec %s\n", sh);
			fflush(stdout);
			exit(1);
		}
		/* no return from exec */

	case -1:
		wmove(cmdwin, 1, 0);
		wrefresh(cmdwin);
		printf("can't fork (errno=%d)\n", errno);
		break;

	default:
		/* parent */
		(void)wait(0);
		break;
	}

	printf("hit any key to continue");
	fflush(stdout);
	/* funny modes were disabled - put them back */
	cbreak();	/* enter CBREAK mode */
	noecho();	/* don't echo typed chars as typed */
	nonl();		/* speed up curses use of <cr> and <lf> */

	(void)getch();
	clearok(cmdwin, TRUE);
	(void)signal(SIGINT, oldsig1);
	(void)signal(SIGQUIT, oldsig2);
	/* screen will be refreshed in getcmdch() */

	return;
}
