#include "Dumper.h"
void
dumpSections(__in pParseResult result)
{
	_tprintf(TEXT("Count of sections:%d\n"), result->dwSectionslHeaderCount);
	for (DWORD i = 0; i < result->dwSectionslHeaderCount; i++)
	{
		printf("Name:%s     Address:%0x     Size:%0x\n", result->sectionsHeader[i].Name, result->sectionsHeader[i].Address, result->sectionsHeader[i].Size);
	}
}
void
dumpResult(__in pParseResult result)
{
	dumpSections(result);
}