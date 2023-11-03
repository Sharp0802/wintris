#ifndef TETRIS_WINSTD_H
#define TETRIS_WINSTD_H

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

#ifndef _WIN32

void __gotoxy(int x, int y)
{
	int w;
	static char buf[64];
	w = sprintf(buf, "\x1b[%d;%df", y, x);
	write(1, buf, w);
}

void __clrscr()
{
	const char *const f = "\033[H\033[J";
	write(1, f, __builtin_strlen(f));
}

int __kbhit()
{
	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);
	return byteswaiting > 0;
}

char __getch()
{
	char buf = 0;
	if (read(0, &buf, 1) < 0)
		perror ("read()");
	return buf;
}

void __enable_raw_mode()
{
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &term);
}

void __disable_raw_mode()
{
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &term);
}

#define ENABLE_RAW() __enable_raw_mode()
#define DISABLE_RAW() __disable_raw_mode()

#define CLEAR() __clrscr()
#define GOTO(x, y) __gotoxy(x+1,y+1)
#define GETCH() __getch()
#define KBHIT() __kbhit()

#else

#define ENABLE_RAW()
#define DISABLE_RAW()

#define CLEAR() clrscr()
#define GOTO(x, y) gotoxy(x+1,y+1)
#define GETCH() getch()
#define KBHIT() kbhit()

#endif

#endif //TETRIS_WINSTD_H
