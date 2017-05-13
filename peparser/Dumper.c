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
BOOL
checkCharacteristic(__in DWORD dwCharacteristics,
	__in DWORD dwCheckedCharacteristic)
{
	if ((dwCharacteristics&dwCheckedCharacteristic) == dwCheckedCharacteristic)
		return TRUE;
	return FALSE;
}
void
dumpCharacteristics(__in WORD wCharacteristics)
{
	_tprintf(TEXT("Characteristics:0x%0x\n"), wCharacteristics);
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_RELOCS_STRIPPED))
	{
		_tprintf(TEXT("Relocation information was stripped from the file. The file must be loaded at its preferred base address. If the base address is not available, the loader reports an error.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_EXECUTABLE_IMAGE))
	{
		_tprintf(TEXT("The file is executable (there are no unresolved external references).\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_LINE_NUMS_STRIPPED))
	{
		_tprintf(TEXT("COFF line numbers were stripped from the file.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_LOCAL_SYMS_STRIPPED))
	{
		_tprintf(TEXT("COFF symbol table entries were stripped from file.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_AGGRESIVE_WS_TRIM))
	{
		_tprintf(TEXT("Aggressively trim the working set. This value is obsolete.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_LARGE_ADDRESS_AWARE))
	{
		_tprintf(TEXT("The application can handle addresses larger than 2 GB.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_BYTES_REVERSED_LO))
	{
		_tprintf(TEXT("The bytes of the word are reversed. This flag is obsolete\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_32BIT_MACHINE))
	{
		_tprintf(TEXT("The computer supports 32-bit words.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_DEBUG_STRIPPED))
	{
		_tprintf(TEXT("Debugging information was removed and stored separately in another file.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP))
	{
		_tprintf(TEXT("If the image is on removable media, copy it to and run it from the swap file.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_NET_RUN_FROM_SWAP))
	{
		_tprintf(TEXT("If the image is on the network, copy it to and run it from the swap file.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_SYSTEM))
	{
		_tprintf(TEXT("The image is a system file.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_DLL))
	{
		_tprintf(TEXT("The image is a DLL file. While it is an executable file, it cannot be run directly.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_UP_SYSTEM_ONLY))
	{
		_tprintf(TEXT("The file should be run only on a uniprocessor computer.\n"));
	}
	if (checkCharacteristic(wCharacteristics, IMAGE_FILE_BYTES_REVERSED_HI))
	{
		_tprintf(TEXT("The bytes of the word are reversed. This flag is obsolete.\n"));
	}
	_tprintf(TEXT("\n"));
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
dumpSubsystem(__in DWORD dwSubsystem)
{
	_tprintf(TEXT("Subsystem:0x%0x\n"), dwSubsystem);
	switch (dwSubsystem)
	{
	case IMAGE_SUBSYSTEM_UNKNOWN:
		_tprintf(TEXT("Unknown subsystem.\n"));
		break;
	case IMAGE_SUBSYSTEM_NATIVE:
		_tprintf(TEXT("No subsystem required (device drivers and native system processes).\n"));
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		_tprintf(TEXT("Windows graphical user interface (GUI) subsystem.\n"));
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		_tprintf(TEXT("Windows character-mode user interface (CUI) subsystem.\n"));
		break;
	case IMAGE_SUBSYSTEM_OS2_CUI:
		_tprintf(TEXT("OS/2 CUI subsystem.\n"));
		break;
	case IMAGE_SUBSYSTEM_POSIX_CUI:
		_tprintf(TEXT("POSIX CUI subsystem.\n"));
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
		_tprintf(TEXT("Windows CE system.\n"));
		break;
	case IMAGE_SUBSYSTEM_EFI_APPLICATION:
		_tprintf(TEXT("Extensible Firmware Interface (EFI) application.\n"));
		break;
	case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
		_tprintf(TEXT("EFI driver with boot services.\n"));
		break;
	case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
		_tprintf(TEXT("EFI driver with run-time services.\n"));
		break;
	case IMAGE_SUBSYSTEM_EFI_ROM:
		_tprintf(TEXT("EFI ROM image.\n"));
		break;
	case IMAGE_SUBSYSTEM_XBOX:
		_tprintf(TEXT("Xbox system.\n"));
		break;
	case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
		_tprintf(TEXT("Boot application.\n"));
		break;
	}
	_tprintf(TEXT("\n"));
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
		dumpSubsystem(result->optionalHead.Subsystem);
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