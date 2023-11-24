#include "inits.h"

void InitializeMap(int level, BYTE* map, BYTE* col)
{
	const DWORD width = 10;
	const DWORD height = 20;

	switch (level % 3)
	{
	case 0:
		for (DWORD i = 0; i < width; ++i)
		{
			for (DWORD j = 0; j < i; ++j)
			{
				map[(height - j - 1) * width + i] = CH_BR;
				col[(height - j - 1) * width + i] = C_FG + C_GREEN;
			}
		}
		break;
	case 1:
		for (DWORD i = 0; i < width; ++i)
		{
			DWORD t = (i + 3) % width;
			for (DWORD j = 0; j < t; ++j)
			{
				map[(height - j - 1) * width + i] = CH_BR;
				col[(height - j - 1) * width + i] = C_FG + C_RED;
			}
		}
		break;
	case 2:
		for (DWORD i = 0; i < width; ++i)
		{
			DWORD t = (i + 6) % width;
			for (DWORD j = 0; j < t; ++j)
			{
				map[(height - j - 1) * width + i] = CH_BR;
				col[(height - j - 1) * width + i] = C_FG + C_YELLOW;
			}
		}
		break;
	}
}
