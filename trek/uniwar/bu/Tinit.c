#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "config.h"
#include "uniwar.h"

extern short	mypid;		/* term handler's process id */
extern int	Uerror;		/* stores uniwar error type for error() */
extern char *optarg;
extern char Ififonam[];
extern char cmdbuf[];
extern int Ifd, Ofd;
extern int errno;

/*
 * Open Daemon's input FIFO.
 * Open our own input FIFO.
 * Catch signals.
 */
init(ac, av)
int ac;
char *av[];
{
	int gamenum;
	int i, c, errflg;
	char ofifonam[14];
	struct stat statbuf;
	char *progname;
	char UNIWARD[128];	/* path of uniward */

	mypid = getpid();	/* set my permanent msg type */
	progname = av[0];
	gamenum = 0;
	errflg = 0;
	while ((c = getopt(ac, av, "g:")) != EOF) {
		switch(c) {
		case 'g':	/* game number */
			if ((gamenum = atoi(optarg)) < 0 ||
				gamenum > 9) {
				fprintf(stderr, "game number range: 1-9\n");
				Uerror = UEUSAGE;
				return(-1);
			}
			break;
		
		case '?':	/* error */
		default:
			errflg++;
			break;
		}
	}

	if (errflg) {
		fprintf(stderr, "usage: %s [-gN]\n", progname);
		Uerror = UEUSAGE;
		return(-1);
	}

	if (gamenum == 0) {
		/* player wants to know about active games */
		for (i = 1; i < 10; i++) {
			sprintf(ofifonam, "/tmp/UnIwAr%d", i);
			if (stat(ofifonam, &statbuf) != -1)
				printf("Game %d: *active*\n", i);
			else
				printf("Game %d: *inactive*\n", i);
		}
		do {
			printf("Enter which game (1-9): ");
			gets(cmdbuf);
		} while ((gamenum = atoi(cmdbuf)) < 1 || gamenum > 9);
	}

	/* get file descriptor for daemon's input FIFO */
	sprintf(ofifonam, "/tmp/UnIwAr%d", gamenum);
	if (stat(ofifonam, &statbuf) == -1) {
		/* need to fork a daemon */
		if (fork() == 0) {
			/* child (will be daemon) */
			sprintf(cmdbuf, "-g%d", gamenum);
			if (execlp("uniward", "uniward", cmdbuf, 0) < 0) {
				/* not in path, try GAMEDIR */
				sprintf(UNIWARD, "%s/uniward", GAMEDIR);
				if (execl(UNIWARD, UNIWARD, cmdbuf, 0) < 0) {
					printf("can't exec daemon errno=%d\n"
							, errno);
					exit(1);
				}
			}
			/* no return from a successful exec */
		}
		/* parent (handler) */
		while (stat(ofifonam, &statbuf) == -1)
			sleep(2);	/* give daemon time to set up FIFO
						and open it */
	}
	if ((Ofd = open(ofifonam, O_WRONLY | O_NDELAY)) < 0) {
		printf("can't open FIFO %s WRONLY\n", ofifonam);
		Uerror = UEUNIX;
		return(-1);
	}

	/* create input FIFO */
	(void)umask(0);
	sprintf(Ififonam, "/tmp/UnIwAr%05d", mypid);
	if (mknod(Ififonam, S_IFIFO | 0777, 0) != 0) {
		printf("Cannot create FIFO %s\n", Ififonam);
		Uerror = UEUNIX;
		return(-1);
	}
	if ((Ifd = open(Ififonam, O_RDONLY | O_NDELAY)) < 0) {
		printf("can't open FIFO %s RDONLY\n", Ififonam);
		Uerror = UEUNIX;
		return(-1);
	}

	/* catch signals */
	if (setsig(SIGHUP) < 0 || setsig(SIGINT) < 0 ||
		setsig(SIGQUIT) < 0 || setsig(SIGTERM) < 0 ||
		setsig(SIGPIPE) < 0)
		return(-1);
	
	return(0);
}

