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

__attribute__((always_inline))
inline void __gotoxy(int x, int y)
{
    printf("\x1b[%d;%df", y, x);
    fflush(stdout);
}

__attribute__((always_inline))
inline void __clrscr()
{
    printf("\033[H\033[J");
    fflush(stdout);
}

#ifndef _WIN32

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
#define GETCH() __getch()
#define KBHIT() __kbhit()

#else

#define ENABLE_RAW()
#define DISABLE_RAW()

#define GETCH() _getch()
#define KBHIT() _kbhit()

#endif

#define CLEAR() __clrscr()
#define GOTO(x, y) __gotoxy(x+1,y+1)

#endif //TETRIS_WINSTD_H
