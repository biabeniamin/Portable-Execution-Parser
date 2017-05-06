#include "Parser.h"
#include "Types.h"
fileHeader 
parseFileHeader(__in IMAGE_NT_HEADERS *imageNtHeader)
{
	fileHeader fHeader;
	fHeader.Machine = imageNtHeader->FileHeader.Machine;
	fHeader.NumberOfSections = imageNtHeader->FileHeader.NumberOfSections;
	fHeader.Characteristics = imageNtHeader->FileHeader.Characteristics;
	return fHeader;
}
optionalHeader
parseOptionalHeader(__in IMAGE_NT_HEADERS *imageNtHeader)
{
	optionalHeader oHeader;
	oHeader.AddressOfEntryPoint = imageNtHeader->OptionalHeader.AddressOfEntryPoint;
	oHeader.ImageBase = imageNtHeader->OptionalHeader.ImageBase;
	oHeader.SectionAlignment = imageNtHeader->OptionalHeader.SectionAlignment;
	oHeader.FileAlignment = imageNtHeader->OptionalHeader.FileAlignment;
	oHeader.Subsystem = imageNtHeader->OptionalHeader.Subsystem;
	oHeader.NumberOfRvaAndSizes = imageNtHeader->OptionalHeader.NumberOfRvaAndSizes;
	return oHeader;
}
pSectionHeader 
parseSectionHeader(__in IMAGE_NT_HEADERS *imageNtHeader,
	__out LPDWORD lpdSectiosHeaderCount)
{
	pSectionHeader sectionsHeader;
	PIMAGE_SECTION_HEADER imageSectionHeader;
	//get section count
	*lpdSectiosHeaderCount = imageNtHeader->FileHeader.NumberOfSections;
	//allocate space for data
	sectionsHeader = malloc(sizeof(sectionHeader)*(*lpdSectiosHeaderCount));
	//get location of section header,it increment by sizeof(image_nt_header),but you can write +1 because that is how pointer arithmetic works
	imageSectionHeader = imageNtHeader+1;
	for (DWORD i = 0; i < *lpdSectiosHeaderCount; i++)
	{
		_tcscpy(sectionsHeader[i].Name ,imageSectionHeader->Name);
		sectionsHeader[i].Address = imageSectionHeader->VirtualAddress;
		sectionsHeader[i].Size = imageSectionHeader->SizeOfRawData;
		imageSectionHeader=imageSectionHeader+1;
	}
	return sectionsHeader;
}
//find section that owns that location
PIMAGE_SECTION_HEADER
getPointerSectionHeader(__in IMAGE_NT_HEADERS *imageNtHeader,
	__in DWORD dwpointer)
{
	DWORD dwSectiosHeaderCount;
	PIMAGE_SECTION_HEADER imageSectionHeader;
	imageSectionHeader = imageNtHeader + 1;
	//get section count
	dwSectiosHeaderCount = imageNtHeader->FileHeader.NumberOfSections;
	for (DWORD i = 0; i < dwSectiosHeaderCount; i++)
	{
		//check if is in memmory range
		if (imageSectionHeader->VirtualAddress <= dwpointer &&
			imageSectionHeader->VirtualAddress + imageSectionHeader->Misc.VirtualSize >= dwpointer)
		{
			return imageSectionHeader;
		}
		imageSectionHeader++;
	}
	//if it didn't founded it
	return NULL;
}
pImportedEntry
parseDirectoryEntryImport(__in IMAGE_DOS_HEADER *imageDosHeader,
	__in IMAGE_NT_HEADERS *imageNtHeader,
	__out LPDWORD lpdwImportedEntriesCount)
{
	PIMAGE_SECTION_HEADER imageSectionHeader;
	PIMAGE_OPTIONAL_HEADER pOptionalHeader;
	IMAGE_DATA_DIRECTORY importDirectory;
	PIMAGE_IMPORT_DESCRIPTOR importDescriptor;
	PCHAR pcImportName;
	pImportedEntry importedEntries;
	//initializing entries count
	*lpdwImportedEntriesCount = 0;
	//get optional header
	pOptionalHeader = &imageNtHeader->OptionalHeader;
	//get import directory
	importDirectory = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	//getsection header which owns this directory based on pointer location
	imageSectionHeader = getPointerSectionHeader(imageNtHeader, importDirectory.VirtualAddress);
	//if there is no import
	if (imageSectionHeader == NULL)
	{
		*lpdwImportedEntriesCount = 0;
		return NULL;
	}
	//allocate space for entries
	importedEntries = malloc(sizeof(importedEntry) * 1);
	//get first import descriptor
	importDescriptor = (DWORD)imageDosHeader+ imageSectionHeader->PointerToRawData +importDirectory.VirtualAddress - imageSectionHeader->VirtualAddress;
	while (1)
	{
		//get pointer to location of import name
		pcImportName = (PCHAR)(DWORD)imageDosHeader + imageSectionHeader->PointerToRawData + importDescriptor->Name - imageSectionHeader->VirtualAddress;
		//save in structure
		_tcscpy(importedEntries[*lpdwImportedEntriesCount].Name, pcImportName);
		//get next import entry
		importDescriptor++;
		//increment count
		*lpdwImportedEntriesCount= *lpdwImportedEntriesCount+1;
		//check if we are still on import entry
		if (getPointerSectionHeader(imageNtHeader, importDescriptor->Name) != imageSectionHeader)
		{
			break;
		}
		//reallocate space for imported entries
		importedEntries = realloc(importedEntries, sizeof(importedEntry) * (*lpdwImportedEntriesCount + 1));
	}
	return importedEntries;
}
pExportedEntry
parseDirectoryEntryExport(__in IMAGE_DOS_HEADER *imageDosHeader,
	__in IMAGE_NT_HEADERS *imageNtHeader,
	__out LPDWORD lpdwExportedEntriesCount)
{
	PIMAGE_SECTION_HEADER imageSectionHeader;
	PIMAGE_OPTIONAL_HEADER pOptionalHeader;
	IMAGE_DATA_DIRECTORY exportDirectory;
	PIMAGE_EXPORT_DIRECTORY imageExportDirectory;
	PCHAR pcExportName;
	LPDWORD lpdwNamesArray;
	LPWORD lpdwOrdinalArray;
	LPDWORD lpdwFunctionsArray;
	pExportedEntry exportedEntries;
	//initializing entries count
	*lpdwExportedEntriesCount = 0;
	//get optional header
	pOptionalHeader = &imageNtHeader->OptionalHeader;
	//get export directory
	exportDirectory = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	//getsection header which owns this directory based on pointer location
	imageSectionHeader = getPointerSectionHeader(imageNtHeader, exportDirectory.VirtualAddress);
	//if there is no import
	if (imageSectionHeader == NULL)
	{
		*lpdwExportedEntriesCount = 0;
		return NULL;
	}
	//get first import descriptor
	imageExportDirectory = (DWORD)imageDosHeader + imageSectionHeader->PointerToRawData + exportDirectory.VirtualAddress - imageSectionHeader->VirtualAddress;
	//get beginning of functions info
	lpdwNamesArray = (DWORD)imageDosHeader + imageSectionHeader->PointerToRawData + imageExportDirectory->AddressOfNames - imageSectionHeader->VirtualAddress;
	lpdwOrdinalArray = (DWORD)imageDosHeader + imageSectionHeader->PointerToRawData + imageExportDirectory->AddressOfNameOrdinals - imageSectionHeader->VirtualAddress;
	lpdwFunctionsArray = (DWORD)imageDosHeader + imageSectionHeader->PointerToRawData + imageExportDirectory->AddressOfFunctions - imageSectionHeader->VirtualAddress;
	//set functions count in result
	*lpdwExportedEntriesCount = imageExportDirectory->NumberOfNames;
	//allocate space for entries
	exportedEntries = malloc(sizeof(exportedEntry) * imageExportDirectory->NumberOfNames);
	for (DWORD i = 0; i < imageExportDirectory->NumberOfNames; i++)
	{
		//get pointer to location of exported function's name
		pcExportName = (PCHAR)(DWORD)imageDosHeader + lpdwNamesArray[i];
		_tcscpy(exportedEntries[i].Name, pcExportName);
		LPWORD as = (DWORD)imageDosHeader + lpdwOrdinalArray[i];
		exportedEntries[i].Ordinal = lpdwOrdinalArray[i];
		exportedEntries[i].Address = lpdwFunctionsArray[lpdwOrdinalArray[i]];
	}
	return exportedEntries;
}
parseResult
parse(__in PTCHAR ptFilePath)
{
	HANDLE hFile;
	DWORD dwFileSize;
	HANDLE hFileMap;
	PVOID pvFile;
	IMAGE_DOS_HEADER *dHeader;
	IMAGE_NT_HEADERS *ntHeader;
	parseResult result;
	//set flag as failed,it it finished correctly,it will be set as correctly
	result.fHasSucceed = 0;
	//open file
	hFile = CreateFile(ptFilePath,
		GENERIC_READ,
		0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("Cannot open file\n"));
		goto InvalidFile;
	}
	dwFileSize = GetFileSize(hFile,
		NULL);
	//map file
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL);
	if (hFileMap == NULL)
	{
		_tprintf(TEXT("Cannot map file!\n"));
		goto FailedMapping;
	}
	//get location on file
	pvFile = MapViewOfFile(hFileMap,
		FILE_MAP_READ,
		0,
		0,
		0);
	if (pvFile == NULL)
	{
		_tprintf(TEXT("Cannot accesed maped file!\n"));
		goto FailedMapping;
	}
	//convert pvoid to image_dos_header
	dHeader = pvFile;
	//get image_nt_header based on dos header
	ntHeader = (DWORD)dHeader + dHeader->e_lfanew;
	//parse file header
	result.fileHead = parseFileHeader(ntHeader);
	//parse file optional header
	result.optionalHead = parseOptionalHeader(ntHeader);
	//parse file sections header
	result.sectionsHeader=parseSectionHeader(ntHeader,&result.dwSectionslHeaderCount);
	//parse import entries 
	result.importedEntries= parseDirectoryEntryImport(dHeader, ntHeader, &result.dwImportedEntriesCount);
	//parse export entries
	result.exportedEntries= parseDirectoryEntryExport(dHeader, ntHeader, &result.dwExportedEntriesCount);
	//mark as succeed
	result.fHasSucceed = 1;
FailedMapping:
	CloseHandle(hFileMap);
InvalidFile:
	CloseHandle(hFile);
	return result;
}