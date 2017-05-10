#include "Dumper.h"
void
dumpFileHeader(__in pParseResult result)
{
	_tprintf(TEXT("Result for %s file\n"), result->tFilePath);
	_tprintf(TEXT("-----Beginning of File header-----\n"));
	_tprintf(TEXT("Machine:%0x\n"), result->fileHead.Machine);
	_tprintf(TEXT("Number of sections:%d\n"), result->fileHead.NumberOfSections);
	_tprintf(TEXT("Characteristics:%0x\n"), result->fileHead.Characteristics);
	_tprintf(TEXT("-----Ending of File header-----\n"));
}
void
dumpOptionalHeader(__in pParseResult result)
{
	_tprintf(TEXT("-----Beginning of Optional header-----\n"));
	_tprintf(TEXT("Address of entry point:%0x\n"), result->optionalHead.AddressOfEntryPoint);
	_tprintf(TEXT("Image base:%0x\n"), result->optionalHead.ImageBase);
	_tprintf(TEXT("Section alignment:%0x\n"), result->optionalHead.SectionAlignment);
	_tprintf(TEXT("File alignment:%0x\n"), result->optionalHead.FileAlignment);
	_tprintf(TEXT("Subsysyem:%0x\n"), result->optionalHead.Subsystem);
	_tprintf(TEXT("Number of Rva and sizes:%d\n"), result->optionalHead.NumberOfRvaAndSizes);
	_tprintf(TEXT("-----Ending of Optional header-----\n"));
}
void
dumpSections(__in pParseResult result)
{
	_tprintf(TEXT("-----Beginning of Sections Header-----\n"));
	_tprintf(TEXT("Count of sections:%d\n"), result->dwSectionslHeaderCount);
	if (result->dwSectionslHeaderCount == -1 || result->sectionsHeader == NULL)
	{
		_tprintf(TEXT("There was a problem when trying to read sections!\n"));
	}
	else
	{
		for (DWORD i = 0; i < result->dwSectionslHeaderCount; i++)
		{
			printf("Name:%s     Address:%0x     Size:%d\n", result->sectionsHeader[i].Name, result->sectionsHeader[i].Address, result->sectionsHeader[i].Size);
		}
	}
	_tprintf(TEXT("-----Ending of Sections Header-----\n"));
}
void
dumpExports(__in pParseResult result)
{
	_tprintf(TEXT("-----Beginning of Exports-----\n"));
	_tprintf(TEXT("Count of exports:%d\n"), result->dwExportedEntriesCount);
	if (result->dwExportedEntriesCount == -1 || (result->exportedEntries == NULL && result->dwExportedEntriesCount!=0))
	{
		_tprintf(TEXT("There was a problem when trying to read exports!\n"));
	}
	else
	{
		for (DWORD i = 0; i < result->dwExportedEntriesCount; i++)
		{
			printf("Name:%s     Ordinal:%0x     Address:%0x\n", result->exportedEntries[i].Name, result->exportedEntries[i].Ordinal, result->exportedEntries[i].Address);
		}
	}
	_tprintf(TEXT("-----Ending of Exports-----\n"));
}
void
dumpImports(__in pParseResult result)
{
	_tprintf(TEXT("-----Beginning of Imports-----\n"));
	_tprintf(TEXT("Count of imports:%d\n"), result->dwImportedEntriesCount);
	if (result->dwImportedEntriesCount == -1 || result->importedEntries == NULL)
	{
		_tprintf(TEXT("There was a problem when trying to read imports!\n"));
	}
	else
	{
		for (DWORD i = 0; i < result->dwImportedEntriesCount; i++)
		{
			printf("Name:%s\n", result->importedEntries[i].Name);
		}
	}
	_tprintf(TEXT("-----Ending of Imports-----\n"));
}
void
dumpResult(__in pParseResult result)
{
	dumpFileHeader(result);
	dumpOptionalHeader(result);
	dumpSections(result);
	dumpImports(result);
	dumpExports(result);
}