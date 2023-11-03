#ifndef TETRIS_BLK_H
#define TETRIS_BLK_H

#define BLK_J 0x00
#define BLK_L 0x04
#define BLK_Z 0x08
#define BLK_R 0x0C
#define BLK_I 0x10
#define BLK_O 0x14

#define CH_SP 0
#define CH_B0 1
#define CH_B1 2
#define CH_B2 3
#define CH_B3 4
#define CH_BR (rand()%4+1)

#define C_FG   30
#define C_BG   40
#define C_NORM 0

#define C_BLACK   0
#define C_RED     1
#define C_GREEN   2
#define C_YELLOW  3
#define C_BLUE    4
#define C_MAGENTA 5
#define C_CYAN    6
#define C_WHITE   7

extern char* chmap[5];

#endif //TETRIS_BLK_H
