#include "error.h"

void __AssertWin32(DWORD err, LPCSTR msg, DWORD line)
{
	static char buf[1024];
	if (!FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			err,
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			buf + 512,
			sizeof buf - 512,
			NULL))
	{
		snprintf(buf, 512, "Unknown error-code 0x%lX", err);
	}
	else
	{
		snprintf(buf, 512, "%s:%ld\n%s", msg, line, buf + 512);
	}

	MessageBoxA(NULL, buf, "error", MB_ICONERROR);

	if (err)
	{
		ExitProcess(err);
		// no-return
	}
}
