#ifndef TETRIS_BLKCTL_H
#define TETRIS_BLKCTL_H

#include "blk.h"
#include "winstd.h"

typedef struct
{
	BYTE* color;
	BYTE* data;
	DWORD width;
	DWORD height;

	/* OVERLAPPED */
	union
	{
		struct
		{
			BYTE ch;
			BYTE fg;
		};
		BYTE* pb;
	};
} BLOCK_CONTROL;


void RevertLine(unsigned char y, BLOCK_CONTROL p);

DWORD CheckLine(void (*cb)(unsigned char y, BLOCK_CONTROL p), BLOCK_CONTROL p);


void FillBlock(unsigned char x, unsigned char y, BLOCK_CONTROL p);

void RevertBlock(unsigned char x, unsigned char y, BLOCK_CONTROL p);

void CheckBlock(unsigned char x, unsigned char y, BLOCK_CONTROL p);


void BlockControl(
		unsigned char x,
		unsigned char y,
		unsigned char blk,
		void (*cb)(unsigned char x, unsigned char y, BLOCK_CONTROL p),
		BLOCK_CONTROL p);

#endif //TETRIS_BLKCTL_H
