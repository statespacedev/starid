/*
 * uniwar error types
 */
#define UEUNIX	1	/* UNIX error */
#define UEINTR	2	/* Interrupt */
#define	UEOPTSLEN 3	/* UNIWAROPTS shell variable string too long */
#define	UEGAMFULL 4	/* Game full, try again later */
#define	UECMDFULL 5	/* Command buffer full, not sent */
#define UEMSGFULL 6	/* Message buffer full, message discarded */
#define UEBADMSG 7	/* Message was not in expected format */
#define UEINTL	8	/* Internal error (bug) */
#define UEUSAGE	9	/* bad args passed when handler invoked */

/* object types */
#define	EMPTY	0
#define	STAR	1
#define	SHIP	2
#define	PORT	3	/* Base is a 5-built port, neutral is -1 built */
#define	FPORT	3	/* Federation port */
#define	EPORT	4	/* Empire port */
#define	NPORT	5	/* neutral port */

/* miscellaneous stuff */
#define CMDBUFLEN 1024	/* length of (circular) command buffer */
#define MSGBUFLEN 10240	/* arbitrary length of (circular) message buffer */
#define CMDSEP	'/'	/* command separator in cmdbuf[] */
#define CMDREDRAW '\14'	/* redraw the screen */
#define CMDSHELL '!'	/* fork a shell */
#define CMDREPEAT '\33'	/* hitting this character alone on input line repeats
			   last command; with other text terminates command.
			   hitting RETURN always terminates command */

/*
 * message types from daemon
 */
#define	UmSCAN	1	/* update scan window (see comment for doscan()) */
#define	UmSTATUS 2	/* Null terminated string for status window */
#define	UmTEXT	3	/* Null terminated string for text window */
#define	UmPDIED	4	/* Player died due to poor play :-) */
#define	UmCODE	5	/* encoded info for text window */
#define	UmLIST	6	/* encoded info for text window need paging */
#define	UmINIT 7	/* Daemon is initializing player */
#define	UmSLEEP 8	/* Player must wait (take time) */
#define	UmPOINT 9	/* encoded info for point printing */
#define	UmROWCOL 10	/* row and col dimensions of galaxy follow */

/*
 * typedefs and structures
 */
struct msgb {
	long	mtype;		/* message type */
	char	mtext[MAXMSG];	/* message text */
};

#define NSCORE 8	/* also defined in daemon/u.h */
/*
 * what happened to an object
 */
#define	FASER	1	/* somebody hit by phaser. */
#define	TORP	2	/* somebody hit by torpedo. */
#define	NOVA	3	/* somebody hit by nova. */
#define	PDIED	4	/* port died (used to report death to team members). */
#define	NDIED	5	/* star blew up (used when nova happens due to chain
				reaction of novas). */
#define	LISTS	6	/* listing of objects */
#define	PATTK	8	/* port attacked (report attack to team members). */
#define	WHATMASK 017	/* mask off the status of "what" */
#define	RIP	020	/* thing that was hit died */
#define	DISPLAC	040	/* thing that was hit displaced */

/*
 * Player flags (some are needed by terminal handler).
 * These are also in daemon/u.h.
 * don't change EMPIRE and FEDDIE (used as array elements!!)
 */
/* alliance */
#define	EMPIRE	1	/* 1 in this bit means Empire. */
#define	FEDDIE	2	/* 1 in this bit means Federation. */
#define ALIMASK 3	/* mask to determine alliance */
/* player state */
#define	GETSIDE 4	/* player is picking a side */
#define	GETSHIP 010	/* player is picking a ship */
#define	ACTIVE	020	/* 0 in this bit means pregame player */
#define	DIED	040	/* 1 in this bit means player dead (needs cleanup) */
/* ship status */
#define	WANTSOUT 0100	/* 1 in this bit means player just tried to quit */
#define RADIO 0200	/* 1 in this bit means player's radio is on */
#define SHIELDED 0400	/* 1 in this bit means shields are up */
#define DOCKED 01000	/* 1 in this bit means player is docked */
/* player settings */
#define	OSHORT	02000	/* 1 in this bit means player wants short output */
#define WARNMASK 014000	/* four warning styles: no warning, */
#define WARN1	004000	/* warn with exclamation points in corners, */
#define WARN2	010000	/* warn with exclamation point fences, */
#define WARN3	014000	/* warn with exclamation point area. */
#define ABSOIN	020000	/* 1 in this bit means absolute coords on input */
#define ABSOOUT 040000	/* 1 in this bit means absolute coords on output */
#define RELOUT	0100000	/* 1 in this bit means relative coords on output */
#define SCANON	0200000 /* 1 in this bit means scans should be done */
