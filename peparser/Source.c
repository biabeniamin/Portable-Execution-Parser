#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include <winnt.h>
#include "Parser.h"
#include "Dumper.h"
void test()
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	char path[] = "Inputs";
	//szDir = "Inputs\\*";
	szDir[0] = 'T';
	szDir[1] = 'e';
	szDir[2] = 's';
	szDir[3] = 't';
	szDir[4] = 'e';
	szDir[5] = 'd';
	szDir[6] = '\\';
	szDir[7] = '*';
	szDir[8] = '\0';
	hFind = FindFirstFile(szDir, &ffd);
	int count = 0;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//printf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			TCHAR y[200];
			_tcscpy(y, ffd.cFileName);
			TCHAR x[200];
			_tcscpy(x, TEXT("Tested\\"));
			_tcscat(x, y);
			parseResult result = parse(x);
			if (result.fHasSucceed)
				dumpResult(&result);
			deallocateStructure(&result);
			if (++count == -1)
				break;
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	
}
int main()
{
	TCHAR filePath[MAX_PATH];
	_tcscpy(filePath, TEXT("Tested\\Profile.dll.infected"));
	_tprintf(TEXT("File path of file which will be analyzed:"));
	//_tcscanf(TEXT("%s"), filePath);
	//test();
	if (_tcslen(filePath) < 1)
	{
		_tprintf(TEXT("Invalid file path!\n"));
		return;
	}
	parseResult result = parse(filePath);
	dumpResult(&result);
	deallocateStructure(&result);
	
}