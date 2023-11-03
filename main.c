#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "winstd.h"
#include "blkctl.h"

void drawui(
	unsigned char ox,
	unsigned char oy,
	unsigned char lw,
	unsigned char lh)
{
    char* p;
    
    CLEAR();
    GOTO(ox, oy);
    
    /*         HORIZONTAL LINE         */
    p = "┏";
    write(1, p, __builtin_strlen(p));
    p = "━━━━━━━━━━━";
    for (int i = 0; i < lw; ++i)
	write(1, p, __builtin_strlen(p));
    p = "┱";
    write(1, p, __builtin_strlen(p));
    p = "────";
    for (int i = 0; i < lw; ++i)
	write(1, p, __builtin_strlen(p));
    
    GOTO(ox, oy + 1);
    
    /*          VERTICAL LINE          */
    for (int y = 0; y < 20 * lh; ++y)
    {
	GOTO(ox, oy + 1 + y);
	
	p = "┃";
	write(1, p, __builtin_strlen(p));
	p = y / lh == 3 && !(y & 1)
	    ? "\x1b[31;40m╌╌╌╌╌╌╌╌╌╌╌\x1b[0m"
	    : "           ";
	for (int i = 0; i < lw; ++i)
	    write(1, p, __builtin_strlen(p));
	
	char shelve = (y / lh == 3 && !(y % lh)) + (y / lh == 6 && y % lh == 1) * 2;
	p = shelve ? "┠" : "┃";
	write(1, p, __builtin_strlen(p));
	if (shelve)
	{
	    p = shelve == 1 ? "slot" : "next";
	    write(1, p, __builtin_strlen(p));
	    p = "────";
	    for (int i = 1; i < lw; ++i)
		write(1, p, __builtin_strlen(p));
	}
    }
    
    GOTO(ox + 16 * lw - 2, oy);
    p = "┐";
    write(1, p, __builtin_strlen(p));
    p = "│";
    for (int i = 0; i < lh * 3; ++i)
    {
	GOTO(ox + 16 * lw - 2, oy + i + 1);
	write(1, p, __builtin_strlen(p));
    }
    GOTO(ox + 16 * lw - 2, oy + lh * 3 + 1);
    p = "┤";
    write(1, p, __builtin_strlen(p));
    p = "│";
    for (int i = 0; i < lh * 3; ++i)
    {
	GOTO(ox + 16 * lw - 2, oy + lh * 3 + 2 + i);
	write(1, p, __builtin_strlen(p));
    }
    GOTO(ox + 16 * lw - 2, oy + lh * 6 + 2);
    p = "┘";
    write(1, p, __builtin_strlen(p));
    
    GOTO(ox, oy + 20 * lh + 1);
    
    /*         HORIZONTAL LINE         */
    p = "┗";
    write(1, p, __builtin_strlen(p));
    p = "━━━━━━━━━━━";
    for (int i = 0; i < lw; ++i)
	write(1, p, __builtin_strlen(p));
    p = "┛";
    write(1, p, __builtin_strlen(p));
}

void drawmap(
	unsigned char ox,
	unsigned char oy,
	MAP map,
	unsigned char lw,
	unsigned char lh)
{
    int y, idx;
    char ch;
    unsigned char fg, bg;
    static char buf[64];
    for (int sy = 0; sy < map.height * lh; ++sy)
    {
	y = sy / lh;
	for (int x = 0; x < map.width; ++x)
	{
	    idx = y * map.width + x;
	    ch = map.data[idx];
	    fg = map.fg[idx];
	    bg = map.bg[idx];
	    
	    GOTO(ox + x * lw + 1, oy + sy + 1);
	    
	    for (int i = 0; i < lw; ++i)
	    {
		/* do NOT use printf directly */
		char* p;
		if (!ch && sy == 6)
		{
		    sprintf(buf, "\x1b[31;40m╌\x1b[m");
		    write(1, buf, strlen(buf));
		}
		else
		{
		    if (ch > 4 || ch < 0)
		    	printf("%d", ch);
		    p = chmap[ch];
		    sprintf(buf, "\x1b[%hhu;%hhum%s\x1b[m", C_FG + fg, C_BG + bg, p);
		    write(1, buf, strlen(buf));
		}
	    }
	}
    }
}

int main(void)
{
#ifdef _WIN32
    /*                ENABLE SGR                */
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
if (hOut == INVALID_HANDLE_VALUE)
    return GetLastError();
DWORD dwMode = 0;
if (!GetConsoleMode(hOut, &dwMode))
    return GetLastError();
dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
if (!SetConsoleMode(hOut, dwMode))
    return GetLastError();
#endif
    
    unsigned char lw = 4;
    unsigned char lh = 2;
    
    /* INITIALIZATION */
    
    drawui(1, 1, lw, lh);
    
    MAP map;
    map.width = 11;
    map.height = 20;
    map.data = malloc(map.width * map.height * 3);
    map.fg = (unsigned char*)map.data + map.width * map.height;
    map.bg = (unsigned char*)map.data + map.width * map.height * 2;
    
    MAP slot;
    slot.width = 4;
    slot.height = 3;
    slot.data = malloc(slot.width * slot.height * 3);
    slot.fg = (unsigned char*)slot.data + slot.width * slot.height;
    slot.bg = (unsigned char*)slot.data + slot.width * slot.height * 2;
    
    MAP next;
    next.width = 4;
    next.height = 3;
    next.data = malloc(next.width * next.height * 3);
    next.fg = (unsigned char*)next.data + next.width * next.height;
    next.bg = (unsigned char*)next.data + next.width * next.height * 2;
    
    /* BASEMENT */
    
    int x = 0;
    int y = 0;
    
    /* RUNTIME */
    
    ENABLE_RAW();
    char ch = '0';
    
    char chk;
    char cell = CH_BR;
    char blk = BLK_J;
    
    char sblk = -1;
    char nblk = BLK_O;
    
    BLKCTL arg;
    while (ch != '/')
    {
	arg.map = map;
	
	chk = 0;
	
	/*  */
	
	/* INPUT */
	if (KBHIT())
	{
	    arg.pb = &chk;
	    
	    ch = GETCH();
	    switch (ch)
	    {
	    case 'q':
		blk = (blk / 4 * 4) + (blk + 3) % 4;
		break;
	    case 'e':
		blk = (blk / 4 * 4) + (blk + 1) % 4;
		break;
	    
	    case '4' /* LEFT */:
		if (x < -1) break;
		x--;
		blkctl(x, y, blk, chkblk, arg);
		if (chk /* CONFLICTS */) x++;
		break;
	    case '6' /* RIGHT */:
		x++;
		blkctl(x, y, blk, chkblk, arg);
		if (chk /* CONFLICTS */) x--;
		break;
	    case '8' /* UP */:
		if (!y) break;
		y--;
		blkctl(x, y, blk, chkblk, arg);
		if (chk /* CONFLICTS */) y++;
		break;
	    case '2' /* DOWN */:
		y++;
		blkctl(x, y, blk, chkblk, arg);
		if (chk /* CONFLICTS */) y--;
		break;
	    
	    case 'w' /* SLOT */:
		char tmp = sblk == -1 ? nblk : sblk;
		
		sblk = blk;
		if (sblk / 4 * 4 == BLK_I)
		    sblk = BLK_I + 1;
		
		blk = tmp;
		x = 0;
		y = 0;
		
		arg.map = slot;
		memset(slot.data, CH_SP, slot.width * slot.height);
		blkctl(0, 0, sblk, fillblk, arg);
		drawmap(11 * lw + 2, 1, slot, lw, lh);
		arg.map = map;
		
		break;
	    }
	}
	
	arg.ch = cell;
	arg.fg = C_RED;
	arg.bg = C_BLACK;
	
	blkctl(x, y, blk, fillblk, arg);
	drawmap(1, 1, map, lw, lh);
	blkctl(x, y, blk, revertblk, arg);
    }
    DISABLE_RAW();
    
    /* FINALIZE */
    free(map.data);
    free(slot.data);
    free(next.data);
    
    return 0;
}
