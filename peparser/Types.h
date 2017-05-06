#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#ifndef TYPES
#define TYPES
typedef struct
{
	DWORD Machine;
	DWORD NumberOfSections;
	WORD Characteristics;
} fileHeader,*pFileHeader;
typedef struct
{
	DWORD AddressOfEntryPoint;
	DWORD ImageBase;
	DWORD SectionAlignment;
	DWORD FileAlignment;
	DWORD Subsystem;
	DWORD NumberOfRvaAndSizes;
}optionalHeader,*pOptionalHeader;
typedef struct
{
	BYTE Name[IMAGE_SIZEOF_SHORT_NAME];
	DWORD Address;
	DWORD Size;
} sectionHeader,*pSectionHeader;
typedef struct
{
	BYTE Name[50];
} importedEntry,*pImportedEntry;
typedef struct
{
	BYTE Name[50];
	WORD Ordinal;
	DWORD Address;
} exportedEntry,*pExportedEntry;
typedef struct
{
	fileHeader fileHead;
	optionalHeader optionalHead;
	DWORD dwSectionslHeaderCount;
	pSectionHeader sectionsHeader;
	DWORD dwImportedEntriesCount;
	pImportedEntry importedEntries;
	DWORD dwExportedEntriesCount;
	pExportedEntry exportedEntries;
	WORD fHasSucceed;
} parseResult,*pParseResult;

#endif // !TYPES