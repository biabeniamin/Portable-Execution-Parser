#include "Dumper.h"
void
dumpSections(__in pParseResult result)
{
	_tprintf(TEXT("Count of sections:%d\n"), result->dwSectionslHeaderCount);
	for (DWORD i = 0; i < result->dwSectionslHeaderCount; i++)
	{
		printf("Name:%s     Address:%0x     Size:%0x\n", result->sectionsHeader[i].Name, result->sectionsHeader[i].Address, result->sectionsHeader[i].Size);
	}
	_tprintf(TEXT("\n"));
}
void
dumpImports(__in pParseResult result)
{
	_tprintf(TEXT("Count of exports:%d\n"), result->dwExportedEntriesCount);
	for (DWORD i = 0; i < result->dwExportedEntriesCount; i++)
	{
		printf("Name:%s     Ordinal:%0x     Address:%0x\n", result->exportedEntries[i].Name, result->exportedEntries[i].Ordinal, result->exportedEntries[i].Address);
	}_tprintf(TEXT("\n"));
}
void
dumpExports(__in pParseResult result)
{
	_tprintf(TEXT("Count of imports:%d\n"), result->dwSectionslHeaderCount);
	for (DWORD i = 0; i < result->dwSectionslHeaderCount; i++)
	{
		printf("Name:%s\n", result->importedEntries[i].Name);
	}_tprintf(TEXT("\n"));
}
void
dumpResult(__in pParseResult result)
{
	dumpSections(result);
	dumpExports(result);
	dumpImports(result);
}