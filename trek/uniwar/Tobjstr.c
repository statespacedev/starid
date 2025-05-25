#include "config.h"
#include "uniwar.h"

char *fleet[];
char *strcpy(), *strcat(), *strncpy();

/*
 * objstr -- obj is uniwar object (SHIP, PORT, etc).  Translate
 * it to a string based on oshort (short or long output).
 * If it's a PORT, return the number of builds in *help.
 * If object is an enemy, set eneflg, else clear it.
 */
objstr(tbuf, obj, pflags, help, eneflg)
char tbuf[];
int obj;
long pflags;
short *help, *eneflg;
{
	short alliance;
	short oshort;
	int type;
	int len;

	oshort = (pflags & OSHORT);

	/*
	 * initialize eneflg to be true if player is a feddie; later
	 * compare to alliance of obj.
	 */
	if (pflags & FEDDIE)
		*eneflg = 1;
	else
		*eneflg = 0;

	type = (obj & 7);
	*help = (obj >> 3) & 037;

	switch(type) {
	case EPORT:
	case FPORT:
	case NPORT:
		if (type == FPORT)
			alliance = 1;
		else if (type == EPORT)
			alliance = 0;
		else
			alliance = *eneflg;

		if (*help >= 0 && oshort == 0) {
			if (type == FPORT)
				(void)strcpy(tbuf, "Federation ");
			else
				(void)strcpy(tbuf, "Empire ");
		} else
			*tbuf = '\0';

		/* neutral has -1 builds */
		if (*help == 037)
			(void)strcpy(tbuf, oshort ? "@" : "Neu Planet");
		else if (*help == 5) {
			if (oshort) {
				if (type == FPORT)
					(void)strcpy(tbuf, "[]");
				else
					(void)strcpy(tbuf, "()");
			} else
				(void)strcat(tbuf, "Base");
		} else {
			if (oshort) {
				if (type == FPORT)
					(void)strcpy(tbuf, "+@");
				else
					(void)strcpy(tbuf, "-@");
			} else
				(void)strcat(tbuf, "Planet");
			if (*help > 0)
				(void)sprintf(&tbuf[strlen(tbuf)]
					, oshort ? "%d" :", %d builds"
					, *help);
		}
		break;

	case SHIP:
		if (type < MAXPL/2)
			alliance = 1;	/* obj is a feddie */
		else
			alliance = 0;	/* obj is empire */

		len = oshort ? 1 : strlen(fleet[*help]);
		(void)strncpy(tbuf, fleet[*help], len);
		tbuf[len] = '\0';
		break;

	case STAR:
		(void)strcpy(tbuf, oshort ? "*" : "Star");
		alliance = *eneflg;
		break;

	case 7:	/* type EOF */
		return(-1);

	default:
		(void)sprintf(tbuf, oshort ? "??" : "Unknown Object (%d)"
								, type);
		break;
	}

	if (*eneflg != alliance)
		*eneflg = 1;
	else
		*eneflg = 0;
	return(type);
}
