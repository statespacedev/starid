	/****** fun with windows -- kill this code ******/
	if (c == '\r') {
		int i;
		wmove(cmdwin, 1, 0);
		wrefresh(cmdwin);
		for (i = 0; i < 10; i++)
			printf("test line %d\n\r");
		printf("hit any key to continue");
		fflush(stdout);
		(void)getch();
		clearok(cmdwin, TRUE);
		wrefresh(cmdwin);
	}
	/****** end of code to zap ******/


