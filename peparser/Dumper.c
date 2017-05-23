/*
PE Parser
Bia Beniamin
V1
*/
#include "Dumper.h"
void
dumpMachineCode(__in DWORD dwMachine)
{
	_tprintf(TEXT("Machine:0x%0x\t"), dwMachine);
	switch (dwMachine)
	{
	case 0x14d:
		_tprintf(TEXT("Intel i860"));
		break;
	case 0x14c:
		_tprintf(TEXT("Intel i386 or higher"));
		break;
	case 0x162:
		_tprintf(TEXT("MIPS R3000"));
		break;
	case 0x166:
		_tprintf(TEXT("MIPS R4000"));
		break;
	case 0x183:
		_tprintf(TEXT("DEC Alpha AXP"));
		break;
	}
	_tprintf(TEXT("\n"));
}
void
dumpCharacteristics(__in WORD wCharacteristics)
{
	_tprintf(TEXT("Characteristics:0x%0x\n"), wCharacteristics);
}
void
dumpFileHeader(__in pParseResult result)
{
	_tprintf(TEXT("Result for %s file\n"), result->tFilePath);
	_tprintf(TEXT("----------------------------------\n"));
	_tprintf(TEXT("-----Beginning of File header-----\n"));
	_tprintf(TEXT("----------------------------------\n"));
	if (result->fFileHeadReaded == 0)
	{
		_tprintf(TEXT("There was an error when trying to read file header!"));
	}
	else
	{
		dumpMachineCode(result->fileHead.Machine);
		_tprintf(TEXT("Number of sections:%d\n"), result->fileHead.NumberOfSections);
		dumpCharacteristics(result->fileHead.Characteristics);
	}
	_tprintf(TEXT("-----Ending of File header-----\n\n"));
}
void
dumpOptionalHeader(__in pParseResult result)
{
	_tprintf(TEXT("--------------------------------------\n"));
	_tprintf(TEXT("-----Beginning of Optional header-----\n"));
	_tprintf(TEXT("--------------------------------------\n"));
	if (result->fOptionalHeadReaded== 0)
	{
		_tprintf(TEXT("There was an error when trying to read optional file header!"));		
	}
	else
	{
		_tprintf(TEXT("Address of entry point:ox%0x\n"), result->optionalHead.AddressOfEntryPoint);
		_tprintf(TEXT("Image base:0x%0x\n"), result->optionalHead.ImageBase);
		_tprintf(TEXT("Section alignment:0x%0x\n"), result->optionalHead.SectionAlignment);
		_tprintf(TEXT("File alignment:0x%0x\n"), result->optionalHead.FileAlignment);
		_tprintf(TEXT("Subsysyem:0x%0x\n"), result->optionalHead.Subsystem);
		_tprintf(TEXT("Number of Rva and sizes:%d\n"), result->optionalHead.NumberOfRvaAndSizes);
	}
	_tprintf(TEXT("-----Ending of Optional header-----\n\n"));
}
void
dumpSections(__in pParseResult result)
{
	_tprintf(TEXT("----------------------------------    \n"));
	_tprintf(TEXT("-----Beginning of Sections Header-----\n"));
	_tprintf(TEXT("----------------------------------    \n"));
	_tprintf(TEXT("Count of sections:%d\n"), result->dwSectionslHeaderCount);
	if (result->dwSectionslHeaderCount == PARSE_ERROR|| result->sectionsHeader == NULL)
	{
		_tprintf(TEXT("There was a problem when trying to read sections!\n"));
	}
	else
	{
		for (DWORD i = 0; i < result->dwSectionslHeaderCount; i++)
		{
			printf("Name:%s\t", result->sectionsHeader[i].Name);
			_tprintf(TEXT("Address:0x%0x\t"), result->sectionsHeader[i].Address);
			_tprintf(TEXT("Size:%d\n"), result->sectionsHeader[i].Size);
		}
	}
	_tprintf(TEXT("-----Ending of Sections Header-----\n\n"));
}
void
dumpExports(__in pParseResult result)
{
	_tprintf(TEXT("------------------------------\n"));
	_tprintf(TEXT("-----Beginning of Exports-----\n"));
	_tprintf(TEXT("------------------------------\n"));
	_tprintf(TEXT("Count of exports:%d\n"), result->dwExportedEntriesCount);
	if (result->dwExportedEntriesCount == PARSE_ERROR || (result->exportedEntries == NULL && result->dwExportedEntriesCount!=0))
	{
		_tprintf(TEXT("There was a problem when trying to read exports!\n"));
		if (result->exportedEntries == NULL)
		{
			int h;
			_tscanf(TEXT("%d"), &h);
		}
	}
	else
	{
		for (DWORD i = 0; i < result->dwExportedEntriesCount; i++)
		{
			printf("Name:%s\t", result->exportedEntries[i].Name);
			_tprintf(TEXT("Ordinal:0x%0x\t"), result->exportedEntries[i].Ordinal);
			_tprintf(TEXT("Address:0x%0x\n"), result->exportedEntries[i].Address);
		}
	}
	_tprintf(TEXT("-----Ending of Exports-----\n\n"));
}
void
dumpImports(__in pParseResult result)
{
	_tprintf(TEXT("------------------------------\n"));
	_tprintf(TEXT("-----Beginning of Imports-----\n"));
	_tprintf(TEXT("------------------------------\n"));
	_tprintf(TEXT("Count of imports:%d\n"), result->dwImportedEntriesCount);
	if (result->dwImportedEntriesCount == PARSE_ERROR || result->importedEntries == NULL)
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
	_tprintf(TEXT("-----Ending of Imports-----\n\n"));
}
void
dumpResult(__in pParseResult result)
{
	if (result->fHasSucceed)
	{
		dumpFileHeader(result);
		dumpOptionalHeader(result);
		dumpSections(result);
		dumpImports(result);
		dumpExports(result);
	}
	else
	{
		_tprintf(TEXT("There was a problem when trying to parse %s file\n"), result->tFilePath);
	}
}