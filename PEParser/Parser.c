#include "Parser.h"
#include "Types.h"
fileHeader parseFileHeader(IMAGE_NT_HEADERS *imageNtHeader)
{
	fileHeader fHeader;
	fHeader.Machine = imageNtHeader->FileHeader.Machine;
	fHeader.NumberOfSections = imageNtHeader->FileHeader.NumberOfSections;
	fHeader.Characteristics = imageNtHeader->FileHeader.Characteristics;
	return fHeader;
}
optionalHeader parseOptionalHeader(IMAGE_NT_HEADERS *imageNtHeader)
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
void parseSectionHeader(IMAGE_DOS_HEADER *imageHeader)
{
	IMAGE_SECTION_HEADER *imageSectionHeader;
	imageSectionHeader = imageHeader + imageHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS);
	/*dosHeader = Memory mapped base address
		ntHeader = (IMAGE_NT_HEADER)((DWORD)dosHeader + dosHeader->e_lfanew)
		sectionHeader = (IMAGE_SECTION_HEADER)((DWORD)ntHeader + OFFSET(OptionalHeader) + sizeof(OptionalHeader))
		each section = (char *)((DWORD)dosHeader + sectionHeader.PointerToRawData)*/

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
	ntHeader = dHeader + dHeader->e_lfanew;
	result.fileHead = parseFileHeader(ntHeader);
	result.optionalHead = parseOptionalHeader(ntHeader);
	parseSectionHeader(dHeader);
FailedMapping:
	CloseHandle(hFileMap);
InvalidFile:
	CloseHandle(hFile);
}