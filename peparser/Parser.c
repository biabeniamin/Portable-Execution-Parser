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
	IMAGE_SECTION_HEADER *imageSectionHeader;
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
	IMAGE_OPTIONAL_HEADER *pOptionalHeader = &imageNtHeader->OptionalHeader;
	IMAGE_DATA_DIRECTORY asd = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	asd = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	IMAGE_IMPORT_DESCRIPTOR *asdf = (DWORD)imageDosHeader + asd.VirtualAddress;
	return sectionsHeader;
}
void
parseDirectoryEntryExport(__in IMAGE_DOS_HEADER *imageDosHeader,
	__in IMAGE_NT_HEADERS *imageNtHeader)
{
	IMAGE_OPTIONAL_HEADER *pOptionalHeader = &imageNtHeader->OptionalHeader;
	IMAGE_DATA_DIRECTORY asd = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	asd = pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	IMAGE_IMPORT_DESCRIPTOR *asdf = (DWORD)imageDosHeader + asd.VirtualAddress;
}
void parse(__in PTCHAR ptFilePath)
{
	HANDLE hFile;
	DWORD dwFileSize;
	HANDLE hFileMap;
	PVOID pvFile;
	IMAGE_DOS_HEADER *dHeader;
	IMAGE_NT_HEADERS *ntHeader;
	parseResult result;
	//open file
	hFile = CreateFile(ptFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		goto InvalidFile;
	dwFileSize = GetFileSize(hFile,
		NULL);
	//map file
	hFileMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READWRITE,
		0,
		dwFileSize + sizeof(WCHAR),
		NULL);
	if (hFileMap == NULL)
		goto FailedMapping;
	//get location on file
	pvFile = MapViewOfFile(hFileMap,
		FILE_MAP_WRITE,
		0,
		0,
		0);
	if (pvFile == NULL)
		goto FailedMapping;
	//convert pvoid to image_dos_header
	dHeader = pvFile;
	//get image_nt_header based on dos header
	ntHeader = (DWORD)dHeader + dHeader->e_lfanew;
	//parse file header
	result.fileHead = parseFileHeader(ntHeader);
	//parse file optional header
	result.optionalHead = parseOptionalHeader(ntHeader);
	//parse file sections header
	result.sectionsHeader=parseSectionHeader(ntHeader,&result.dSectionslHeaderCount);
	//parse export 
	parseDirectoryEntryExport(dHeader,ntHeader);
FailedMapping:
	CloseHandle(hFileMap);
InvalidFile:
	CloseHandle(hFile);
}