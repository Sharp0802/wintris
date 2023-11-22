#include "winstd.h"

void gotoxy(DWORD x, DWORD y)
{
	static char buf[512];
	int n = sprintf(buf, "\x1b[%ld;%ldf", y + 1, x + 1);
	write(1, buf, n);
}

void clrscr(void)
{
#define CLRSCR "\x1b[H\x1b[J"
	write(1, CLRSCR, strlen(CLRSCR));
#undef CLRSCR
}
