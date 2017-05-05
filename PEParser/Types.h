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
	fileHeader fileHead;
	optionalHeader optionalHead;
} parseResult,*pParseResult;

#endif // !TYPES