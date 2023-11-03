#ifndef TETRIS_BLKCTL_H
#define TETRIS_BLKCTL_H

#include "blk.h"

typedef struct
{
	unsigned char* bg;
	unsigned char* fg;
	char* data;
	unsigned char width;
	unsigned char height;
} MAP;

typedef struct
{
	MAP map;

	/* OVERLAPPED */
	union
	{
		struct
		{
			char ch;
			char fg;
			char bg;
		};
		char* pb;
	};
} BLKCTL;


void fillblk(unsigned char x, unsigned char y, BLKCTL p);

void revertblk(unsigned char x, unsigned char y, BLKCTL p);

void chkblk(unsigned char x, unsigned char y, BLKCTL p);


void blkctl(
		unsigned char x,
		unsigned char y,
		unsigned char blk,
		void (*cb)(unsigned char x, unsigned char y, BLKCTL p),
		BLKCTL p);

#endif //TETRIS_BLKCTL_H
