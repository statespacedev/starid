#include <curses.h>	/* curses.h automatically includes stdio.h */
#include <signal.h>
#include "config.h"
#include "uniwar.h"

extern short	wins_on;	/* flag TRUE when initscr() has been called */
extern short	playing;	/* flag TRUE when daemon knows about player */
extern short	died;		/* TRUE if player or daemon died */
extern WINDOW	*cmdwin;
extern char	Ififonam[];

void exit(), error(), saygoodbye();

/*
 * Close up shop before exiting.
 *
 * If curses are in action, turn them off.
 * (later may need to detach from shmem).
 */
void
enditall(sig)
register int sig;
{
	if (sig != 0)
		(void)signal(sig, (int(*)())(SIG_IGN));
	/* we can't be screwing up our goodbyes */
	if (sig != SIGINT)
		(void)signal(SIGINT, (int(*)())(SIG_IGN));
	if (sig != SIGQUIT)
		(void)signal(SIGQUIT, (int(*)())(SIG_IGN));
	else if (!died && wins_on)
		error("Fatal error: ");
	if (playing)
		saygoodbye();	/* no more msgs from me */

	if (wins_on == TRUE) {
		/*** not used
		idlok(textwin, FALSE);
		***/
		nodelay(cmdwin, FALSE);
		endwin();		/* cleans up windows */
	}
	unlink(Ififonam);	/* clean up FIFO */
	exit(0);
}

