#include <curses.h>	/* curses.h automatically includes stdio.h */
#include "config.h"
#include "uniwar.h"

/*
 * global variables for terminal handler.
 */
char	Ififonam[18];

int	Ifd,		/* file descriptor for input FIFO */
	Ofd;		/* file descriptor for output FIFO */

WINDOW	*mapwin,	/* window containing map (short-range scan) */
	*statwin,	/* window containing ship's status */
	*textwin,	/* window containing text messages */
	*cmdwin;	/* window containing user's typed commands */

int	Uerror;		/* stores uniwar error type for error() */
int	Echar, Kchar;	/* erase and kill chars for this user */
short	mypid;		/* term handler's process id */
short	wins_on;	/* flag TRUE when initscr() has been called */
short	playing;	/* flag TRUE when daemon knows about player */
short	active;		/* flag TRUE when daemon has let player in */
short	doprompt;	/* prompt for "--more--" when window is full */
short	cmdstart,	/* first byte to process in cmdbuf circular buffer */
	cmdend;		/* last byte to process in cmdbuf circular buffer */
short	msgstart,	/* first byte to process in msgbuf circular buffer */
	msgend;		/* last byte to process in msgbuf circular buffer */
short	ycorner,	/* col in universe for upper left of mapwin */
	xcorner;	/* col in universe for upper left of mapwin */
short	died;		/* TRUE if player or daemon died */
short	linenum;	/* current line in text window */
int	interrupted;	/* true if interrupted (used in polldaemon()) */
int	Mrows;		/* number of rows in map */
int	Mcols;		/* number of columns in map */

char tmpbuf[256];	/* used for sprintf prior to textwindow prints */
char cmdbuf[CMDBUFLEN];	/* circular buffer for outgoing commands */
char Outbuf[CMDBUFLEN];	/* buffer for outgoing commands */
char msgbuf[MSGBUFLEN];	/* circular buffer for incoming messages */
char Buffer[MSGBUFLEN];	/* buffer for incoming messages */
char *Uerrmsg[] = {
	"",
	"unix error",
	"interrupt",
	"UNIWAROPTS shell variable\nstring too long",
	"game full, try again later",
	"command buffer full,\nhit RETURN again",
	"message buffer full,\nmessage discarded",
	"message was not in\nexpected format",
	"internal error (bug)"
};

/* names of ships for objstr() */
/*
 * some ships canned to avoid running out of file descriptors
 *	"Vulcan",
 *	"Yorktown",
 *	"Panther",
 *	"Wolf",
 */
char *fleet[] = {
	"Excalibur",
	"Farragut",
	"Intrepid",
	"Lexington",
	"Nimitz",
	"Savannah",
	"Trenton",
	"Buzzard",
	"Cobra",
	"Demon",
	"Goblin",
	"Hawk",
	"Jackal",
	"Manta"
};
