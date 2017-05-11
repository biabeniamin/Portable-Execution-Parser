#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#ifndef TYPES
#define TYPES
#define PARSE_ERROR 0xFFFFFFFF 
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
	TCHAR tFilePath[MAX_PATH];
	//owns file header information
	fileHeader fileHead;
	//is 1 if it readed succesfully
	INT fFileHeadReaded;
	//owns optional header informations
	optionalHeader optionalHead;
	//is 1 if it readed succesfully
	INT fOptionalHeadReaded;
	//counter is PARSE_ERROr if failed
	DWORD dwSectionslHeaderCount;
	pSectionHeader sectionsHeader;
	//counter is PARSE_ERROR if failed
	DWORD dwImportedEntriesCount;
	//owns imported entries
	pImportedEntry importedEntries;
	//counter is PARSE_ERROR if failed
	DWORD dwExportedEntriesCount;
	//owns exported entries
	pExportedEntry exportedEntries;
	//is 1 if there wasn't any problem which wasn't handled
	WORD fHasSucceed;
} parseResult,*pParseResult;

#endif // !TYPES