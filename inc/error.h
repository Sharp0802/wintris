#ifndef WINTRIS_ERROR_H
#define WINTRIS_ERROR_H

#include "winstd.h"

void __AssertWin32(DWORD err, LPCSTR msg, DWORD line);

#define AssertWin32(err) __AssertWin32(err, __FILE__, __LINE__)

#endif //WINTRIS_ERROR_H
