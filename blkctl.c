#include "blkctl.h"


void fillblk(unsigned char x, unsigned char y, BLKCTL p)
{
	int idx = y * p.map.width + x;
	p.map.data[idx] = p.ch;
	p.map.fg[idx] = p.fg;
	p.map.bg[idx] = p.bg;
}

void revertblk(unsigned char x, unsigned char y, BLKCTL p)
{
	int idx = y * p.map.width + x;
	p.map.data[idx] = CH_SP;
}

void chkblk(unsigned char x, unsigned char y, BLKCTL p)
{
	if (x >= p.map.width || y >= p.map.height)
		*p.pb = 0xFF;
	if (p.map.data[y * p.map.width + x] != CH_SP)
		*p.pb = 0xFF;
}

void blkctl(
		unsigned char x,
		unsigned char y,
		unsigned char blk,
		void (*cb)(unsigned char x, unsigned char y, BLKCTL p),
		BLKCTL p)
{
	unsigned char cx, cy;
	for (int i = 0; i < 4; ++i)
	{
		switch (blk)
		{
		case BLK_J + 0:
			cx = (i % 3) + (i / 3) * 2;
			cy = 1 + i / 3;
			break;
		case BLK_J + 1:
			cx = i > 0;
			cy = (i + 2) % 3;
			break;
		case BLK_J + 2:
			cx = i % 3;
			cy = i > 0;
			break;
		case BLK_J + 3:
			cx = 1 + i / 3;
			cy = i % 3;
			break;

		case BLK_L + 0:
			cx = i % 3;
			cy = 1 + i / 3;
			break;
		case BLK_L + 1:
			cx = 1 - i / 3;
			cy = i % 3;
			break;
		case BLK_L + 2:
			cx = (i + 2) % 3;
			cy = i > 0;
			break;
		case BLK_L + 3:
			cx = 1 + (i < 1);
			cy = (i + 2) % 3;
			break;

		case BLK_Z + 0:
			cx = i - i / 2;
			cy = 1 + i % 2;
			break;
		case BLK_Z + 1:
			cx = i % 2;
			cy = (i + 2) % 3 - (i / 3);
			break;
		case BLK_Z + 2:
			cx = i - i / 2;
			cy = i / 2;
			break;
		case BLK_Z + 3:
			cx = 1 + i % 2;
			cy = (i + 2) % 3 - (i / 3);
			break;

		case BLK_R + 0:
			cx = (i + 1) % 3;
			cy = 1 + i / 2;
			break;
		case BLK_R + 1:
			cx = i / 2;
			cy = i - i / 2;
			break;
		case BLK_R + 2:
			cx = (i + 1) % 3;
			cy = i / 2;
			break;
		case BLK_R + 3:
			cx = 1 + i / 2;
			cy = i - i / 2;
			break;

		case BLK_I + 0:
			cx = 1;
			cy = i;
			break;
		case BLK_I + 1:
			cx = i;
			cy = 1;
			break;
		case BLK_I + 2:
			cx = 1;
			cy = i - 1;
			break;
		case BLK_I + 3:
			cx = i - 1;
			cy = 1;
			break;

		case BLK_O + 0:
		case BLK_O + 1:
		case BLK_O + 2:
		case BLK_O + 3:
			cx = i & 1;
			cy = i / 2;
			break;
		}

		cb(x + cx, y + cy, p);
	}
}
