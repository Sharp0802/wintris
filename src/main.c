#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdint.h>
#include <time.h>

#include "winstd.h"
#include "blkctl.h"
#include "error.h"

#define writes(s) write(1, s, strlen(s))


#define GROUND_TIMEOUT (1500)


typedef struct
{
	LPCSTR LT;
	LPCSTR MT;
	LPCSTR RT;

	LPCSTR LM;
	LPCSTR RM;

	LPCSTR LB;
	LPCSTR MB;
	LPCSTR RB;
} BOXINFO;

static BOXINFO GenericBox = {
		"┏", "━", "┓",
		"┃", "┃",
		"┗", "━", "┛"
};

static BOXINFO DoubleBox = {
		"╔", "═", "╗",
		"║", "║",
		"╚", "═", "╝"
};

static BOXINFO SlotBox = {
		"┱", "─", "┐",
		"┃", "│",
		"╂", "─", "┤"
};

static BOXINFO NextBox = {
		"╂", "─", "┤",
		"┃", "│",
		"┠", "─", "┘"
};

LPCSTR DepthMap[] = {
		" ",
		"█",
		"▓",
		"▒",
		"░",
};

BYTE ColorTable[] = {
		C_CYAN,
		C_BLUE,
		C_WHITE,
		C_YELLOW,
		C_GREEN,
		C_MAGENTA,
		C_RED
};

void DrawBox(
		DWORD x,
		DWORD y,
		DWORD cx,
		DWORD cy,
		BOXINFO bi)
{
	DWORD i;
	LPCSTR p;

	cx += 2;
	cy += 2;

	// Render top
	gotoxy(x, y);
	p = bi.LT;
	writes(p);
	p = bi.MT;
	for (i = 1; i < cx - 1; ++i)
		writes(p);
	p = bi.RT;
	writes(p);

	// Render left-side
	p = bi.LM;
	for (i = 1; i < cy - 1; ++i)
	{
		gotoxy(x, y + i);
		writes(p);
	}

	// Render right-side
	p = bi.RM;
	for (i = 1; i < cy - 1; ++i)
	{
		gotoxy(x + cx - 1, y + i);
		writes(p);
	}

	// Render bottom
	gotoxy(x, y + cy - 1);
	p = bi.LB;
	writes(p);
	p = bi.MB;
	for (i = 1; i < cx - 1; ++i)
		writes(p);
	p = bi.RB;
	writes(p);
}

void DrawCriteria(
		DWORD x,
		DWORD y,
		DWORD cx)
{
	cx += 2;

	gotoxy(x, y);
	writes("┠");
	for (DWORD i = 1; i < cx - 1; ++i)
		writes("┄");
	writes("┨");
}

void DrawScore(
		DWORD x,
		DWORD y,
		DWORD cx,
		LPCSTR name,
		DWORD score)
{
	gotoxy(x + 1, y + 1);
	printf(" %-*s %07lu ", (int)(cx - 10), name, score);
	fflush(stdout);
}

void DrawMap(
		DWORD x,
		DWORD y,
		DWORD cx,
		DWORD cy,
		DWORD mx,
		BYTE* map,
		const BYTE* col,
		const LPCSTR* chv)
{
	BYTE c;
	LPCSTR buf;
	for (DWORD i = 0; i < cx * cy; ++i)
	{
		if (!map[i])
			continue;

		gotoxy((i % cx) * mx + x, i / cx + y);

		if (map[i] == CH_RM)
		{
			map[i] = CH_SP;
			buf = " ";
		}
		else
		{
			buf = chv[map[i]];
		}

		c = col[i];
		for (DWORD j = 0; j < mx; ++j)
			printf("\x1b[%hhum%s\x1b[m", c, buf);
	}
	fflush(stdout);
}

DWORD DrawGui(
		DWORD x,
		DWORD y,
		BYTE* map,
		const BYTE* mapCol,
		BYTE* slot,
		const BYTE* slotCol,
		BYTE* next,
		const BYTE* nextCol,
		const LPCSTR* chv,
		DWORD score)
{
	DWORD width = 10;
	DWORD height = 20;

	DWORD wSub = 4 * 2;
	DWORD wMain = width * 2;

	DWORD hSub = 4;
	DWORD hMain = height;

	DrawBox(x, y, wMain + wSub + 1, 1, DoubleBox);

	DrawBox(x, y + 3, wMain, hMain, GenericBox);
	DrawCriteria(x, y + hSub + 4, wMain);
	DrawBox(x + wMain + 1, y + 3, wSub, hSub, SlotBox);
	DrawBox(x + wMain + 1, y + hSub + 4, wSub, hSub, NextBox);

	DrawScore(x, y, wMain + wSub + 1, "Syw0802", score);

	DrawMap(x + 1, y + 4, width, height, 2, map, mapCol, chv);
	DrawMap(x + wMain + 2, y + 4, 4, 4, 2, slot, slotCol, chv);
	DrawMap(x + wMain + 2, y + hSub + 5, 4, 4, 2, next, nextCol, chv);

	return wMain + wSub + 3;
}

int main(void)
{
	srand(time(NULL));

	// Enable UTF8
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// Enable ANSI Sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
		AssertWin32(GetLastError());
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE)
		AssertWin32(GetLastError());

	DWORD outMode = 0;
	DWORD inMode = 0;
	if (!GetConsoleMode(hOut, &outMode))
		AssertWin32(GetLastError());
	if (!GetConsoleMode(hIn, &inMode))
		AssertWin32(GetLastError());

	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	inMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
	if (!SetConsoleMode(hOut, outMode))
		AssertWin32(GetLastError());
	if (!SetConsoleMode(hIn, inMode))
		AssertWin32(GetLastError());

	writes("\x1b[?25l");

	// Allocates buffers
	BYTE* map = calloc(10 * 20, sizeof *map);
	BYTE* mapCol = calloc(10 * 20, sizeof *mapCol);

	BYTE* slot = calloc(4 * 4, sizeof *slot);
	BYTE* slotCol = calloc(4 * 4, sizeof *slotCol);

	BYTE* next = calloc(4 * 4, sizeof *next);
	BYTE* nextCol = calloc(4 * 4, sizeof *nextCol);

	// Start game
	BOOL exit = FALSE;

	DWORD score = 0;

	DWORD curX = 4;
	DWORD curY = 0;
	DWORD curRot = 0;
	BYTE curBlk = BLK_T;

	BYTE slotBlk = BLK_L;
	BOOL slotted = FALSE;

	BYTE nextBlk = BLK_J;

	BYTE ground = FALSE;
	ULONG64 groundTick = 0;

	ULONG64 globalTick = 0;
	ULONG64 downInterval = 500;

	INPUT_RECORD ir;
	DWORD nir;

	BLOCK_CONTROL ctl, sCtl, nCtl;
	while (!exit)
	{
		/* Prologue */
		HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
		if (GetNumberOfConsoleInputEvents(in, &nir) && nir)
		{
			if (!ReadConsoleInputA(in, &ir, 1, &nir))
				AssertWin32(GetLastError());
			if (ir.EventType == KEY_EVENT)
			{
				WORD vkey = ir.Event.KeyEvent.wVirtualKeyCode;
				CHAR ach = ir.Event.KeyEvent.uChar.AsciiChar;

				if (vkey == VK_ESCAPE)
				{
					exit = TRUE;
					break;
				}

				/* MOVEMENT */
				{
					// Open transaction
					int addX = 0, addY = 0;
					if (vkey == VK_LEFT)
						addX--;
					if (vkey == VK_RIGHT)
						addX++;
					if (vkey == VK_DOWN)
					{
						addY++;
					}

					// Check conflicts
					BYTE overlap = FALSE;
					ctl.data = map;
					ctl.color = mapCol;
					ctl.height = 20;
					ctl.width = 10;
					ctl.pb = &overlap;
					BlockControl(curX + addX, curY + addY, curBlk + curRot, CheckBlock, ctl);

					// Flush transaction
					if (!overlap)
					{
						curX += addX;
						curY += addY;
					}
				}

				/* ROTATION */
				if (ir.Event.KeyEvent.bKeyDown)
				{
					// Open transaction
					int addRot = 0;
					switch (ach)
					{
					case 'q': // 270
						addRot += 3;
						break;
					case 'e': // 90
						addRot++;
						break;
					}

					// Check conflicts
					BYTE overlap = FALSE;
					ctl.data = map;
					ctl.color = mapCol;
					ctl.height = 20;
					ctl.width = 10;
					ctl.pb = &overlap;
					BlockControl(curX, curY, curBlk + (curRot + addRot) % 4, CheckBlock, ctl);

					// Flush transaction
					if (!overlap)
					{
						curRot += addRot;
						curRot %= 4;
					}
					/*
					else
					{
						curY++;
					}
					 */
				}

				/* SLOT */
				if (!slotted && vkey == VK_UP)
				{
					curBlk ^= slotBlk;
					slotBlk ^= curBlk;
					curBlk ^= slotBlk;

					curX = 4;
					curY = 0;

					slotted = TRUE;
				}
			}
		}

		/* COLLISION */
		{
			// Check conflicts
			BYTE overlap = FALSE;
			ctl.data = map;
			ctl.color = mapCol;
			ctl.height = 20;
			ctl.width = 10;
			ctl.pb = &overlap;
			BlockControl(curX, curY + 1, curBlk + curRot, CheckBlock, ctl);

			if (ground && overlap)
			{
				ULONG64 elapsed = GetTickCount64() - groundTick;
				if (elapsed > GROUND_TIMEOUT)
				{
					// TODO: BUG
					curBlk = nextBlk;
					nextBlk = (rand() % 7) * 4;
					curX = 4;
					curY = 0;
					slotted = FALSE;
					ground = FALSE;

					score += 50;
					downInterval -= downInterval / 30;
					if (downInterval < 100)
						downInterval = 100;

					goto SKIP;
				}
			}

			if (!ground && overlap)
			{
				groundTick = GetTickCount64();
				ground = TRUE;
			}

			if (ground && !overlap)
			{
				ground = FALSE;
			}
		}

		/* DOWN-STAIR */
		if (GetTickCount64() - globalTick > downInterval)
		{
			globalTick = GetTickCount64();

			// Open transaction
			curY++;

			// Check conflicts
			BYTE overlap = FALSE;
			ctl.data = map;
			ctl.color = mapCol;
			ctl.height = 20;
			ctl.width = 10;
			ctl.pb = &overlap;
			BlockControl(curX, curY, curBlk + curRot, CheckBlock, ctl);

			// Flush transaction
			if (overlap)
				curY--;
			else
				score += 10;
		}

	SKIP:
		/* Frame */
		ctl.data = map;
		ctl.color = mapCol;
		ctl.height = 20;
		ctl.width = 10;
		ctl.ch = CH_B2;
		ctl.fg = C_FG + ColorTable[curBlk / 4];

		sCtl.data = slot;
		sCtl.color = slotCol;
		sCtl.height = 4;
		sCtl.width = 4;
		sCtl.ch = CH_B2;
		sCtl.fg = C_FG + ColorTable[slotBlk / 4];

		nCtl.data = next;
		nCtl.color = nextCol;
		nCtl.height = 4;
		nCtl.width = 4;
		nCtl.ch = CH_B2;
		nCtl.fg = C_FG + ColorTable[nextBlk / 4];

		BlockControl(curX, curY, curBlk + curRot, FillBlock, ctl);
		BlockControl(0, 0, slotBlk, FillBlock, sCtl);
		BlockControl(0, 0, nextBlk, FillBlock, nCtl);

		DrawGui(0, 1, map, mapCol, slot, slotCol, next, nextCol, DepthMap, score);

		BlockControl(curX, curY, curBlk + curRot, RevertBlock, ctl);
		BlockControl(0, 0, slotBlk, RevertBlock, sCtl);
		BlockControl(0, 0, nextBlk, RevertBlock, nCtl);

		/* Epilogue */
	}

	// Finalize game
	free(map);
	free(slot);
	free(next);
	free(mapCol);
	free(slotCol);
	free(nextCol);

	return 0;
}
