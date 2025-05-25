#include <curses.h>	/* curses.h automatically includes stdio.h */
#include "config.h"
#include "uniwar.h"

extern short doprompt;	/* give --more-- prompt in textwin */

char *strcat();
char *coordstr();
void textprint(), getmsgsht(), getmsglng();

/*
 * Interpret coded text and add it to win.  Should only be used for
 * textwin, as assumptions are made about line length.  If moveflg
 * TRUE, prompt the user to hit a key when the screen is full, then
 * print a new screenful.
 *
 * Messages have headers of the form: mwwffffrrcc
 *	m is char message type (UmCODE) (already stripped by caller).
 *	ww is short "what happened".  The implementation of the List command
 *		will allow ww to be LISTS in addition to the above.
 *	ffff is long containing player flags.
 *	rr is short row psn of player getting the msg.
 *	cc is short col psn of player getting the msg.
 *
 * ww is what happened:
 *	FASER-- somebody hit by phaser.
 *	TORP  -- somebody hit by torpedo.
 *	NOVA  -- somebody hit by nova.	
 *	RIP|FASER-- somebody killed by phaser.
 *	RIP|TORP  -- somebody killed by torpedo.
 *	RIP|NOVA  -- somebody killed by nova.	
 *	PDIED -- port died (only used to report death to team members).
 *	NDIED -- star blew up (only used when nova happens due to chain
 *				reaction of novas).
 *	PATTK -- port attacked (report attack to team members).
 *	LISTS	-- listing of objects
 *	DISPLAC	-- thing hit was displaced.
 *
 * The other info governs the output.  This routine has to know something
 * about the daemon's work, especially ship names and such.
 */
void
docode(moveflg)
register int moveflg;
{
	short what, oshort, rpos, cpos;
	long pflags;
	short hitter, r, c;
	int obj;
	long hit, shields;
	short builds, eneflg;
	char obuf[CMDBUFLEN];
	char tbuf[64];

	/* read the header info */
	getmsgsht(&what);
	getmsglng(&pflags);
	getmsgsht(&rpos);
	getmsgsht(&cpos);
	if (moveflg == TRUE)
		doprompt = 1;
	oshort = (pflags & OSHORT);

	/*
	 * output format:
	 * (remember, we have to fit this in TEXTCOLS columns (32))
	 *
	 * long:
	 *	Excalibur @22-31 +4,+2 100.0%
	 *	  makes 155.5 units torpedo hit
	 *	  on Buzzard displaced to 20-31
	 *	  +2,+2, +83.6%
	 * short:
	 *	E 22-31 +4,+2, +100  155T  B
	 *	  >20-31 +2,+2, +83
	 *
	 * Star			-->	*
	 * Emp planet(3)	-->	-@3
	 * Fed planet(3)	-->	+@3
	 * Emp base		-->	()
	 * Fed base		-->	[]
	 * etc.
	 */
	hitter = 1;	/* working with info about hitter, not hittee */
	*obuf = '\0';
	while((obj = objstr(tbuf, getmsgch(), pflags, &builds, &eneflg))
								> EMPTY) {
		(void)sprintf(&obuf[strlen(obuf)], "%s%s "
			, (eneflg && what == LISTS) ? "* " : "", tbuf);
		/*
		 * If we're working with the hitter, or the hittee didn't
		 * die, or the hittee wasn't a ship, get row,col coords.
		 */
		if (hitter == 1 || (what & RIP) == 0 || obj != SHIP) {
			getmsgsht(&r);
			getmsgsht(&c);
			if (hitter == 0 && (what & DISPLAC))
				/* displaced */
				(void)strcat(obuf
					, oshort ? ">" : "displaced to ");
			if (r == 0)
				(void)sprintf(&obuf[strlen(obuf)]
					, "out of range");
			else {
				(void)sprintf(&obuf[strlen(obuf)]
					, coordstr(pflags, r, c, rpos, cpos));
			}
		}
		if (hitter == 0 && (what & RIP)) {
			/*
			 * got both hitter and hittee, now see if that's
			 * all she wrote.
			 */
			(void)sprintf(&obuf[strlen(obuf)]
				, " -- %s DESTROYED!\n", tbuf);
			textprint(obuf);
			return;
		}
		if (what == PDIED || what == NDIED || what == PATTK) {
			/* got all we need */
			if (what == PDIED)
				(void)strcat(obuf
					, oshort ? " D\n" : " Destroyed\n");
			else if (what == NDIED)
				(void)strcat(obuf,oshort ? " N\n" : " Nova!\n");
			else	/* PATTK */
				(void)strcat(obuf
					, oshort ? " A\n" : " Attacked!\n");
			textprint(obuf);
			return;
		}

		/*
		 * If we're not listing, then there is more info about
		 * firing ship to read.  If we are listing and "hit"
		 * is non-zero, then we have more info about ship listed
		 * to read.
		 */
		if (hitter)
			getmsglng(&hit);
		else
			hit = 0;
		if (what != LISTS || hit) {
			switch(obj) {
			case EPORT:
			case FPORT:
				if (builds != 5)
					break;
				/* fall through (base -- need shields info) */

			case SHIP:
				getmsglng(&shields);
				if (obj == SHIP)
					shields /= (MAXSHIE/10000);
				else
					shields /= (MAXBSHIE/10000);
				if (oshort)
					(void)sprintf(&obuf[strlen(obuf)],
						" %+d", shields/100);
				else
					(void)sprintf(&obuf[strlen(obuf)]
						, ", %+d.%d%%", shields/100
						, abs((int)(shields%100))/10);
				break;

			case STAR:
			default:
				break;
			}
			if (hitter == 0) {	/* all done */
				(void)strcat(obuf, "\n");
				textprint(obuf);
				return;
			}
		}
		/*
		 * If we're listing, print the buffer.
		 */
		if (what == LISTS) {
			(void)strcat(obuf, "\n");
			textprint(obuf);
			*obuf = '\0';
			continue;
		}

		if (hitter) {
			/* add the hit to the buffer */
			if (oshort)
				(void)sprintf(&obuf[strlen(obuf)],
					"  %+d", hit/100);
			else
				(void)sprintf(&obuf[strlen(obuf)]
					, "  makes %+d.%d unit ", hit/100
					, (hit%100)/10);
			switch(what & WHATMASK) {
			case FASER:	/* somebody hit by phaser. */
				(void)strcat(obuf, oshort ? "P" : "phaser");
				break;

			case TORP:	/* somebody hit by torpedo. */
				(void)strcat(obuf, oshort ? "T" : "torpedo");
				break;

			case NOVA:	/* somebody hit by nova. */
				(void)strcat(obuf, oshort ? "N" : "nova");
				break;

			default:
				break;
			}
			if (oshort)
				(void)strcat(obuf, "  ");
			else
				(void)strcat(obuf, " hit on ");
		}
		hitter = 0;	/* switch from hitter to hittee */
	}

	/* We escaped the loop, we must have been listing */
	doprompt = 0;
}

