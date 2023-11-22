#ifndef TETRIS_WINSTD_H
#define TETRIS_WINSTD_H

#include <io.h>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>

#define write _write

void gotoxy(DWORD x, DWORD y);
void clrscr(void);

#endif //TETRIS_WINSTD_H
