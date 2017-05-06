#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include <winnt.h>
#include "Parser.h"
#include "Dumper.h"
int main()
{
	TCHAR filePath[500];
	_tcscpy(filePath, TEXT("TestedFiles\\Profile.dll.infected"));
	_tprintf(TEXT("File path of file which will be analyzed:"));
	//_tcscanf(TEXT("%s"), filePath);
	if (_tcslen(filePath) < 1)
	{
		_tprintf(TEXT("Invalid file path!\n"));
		return;
	}
	parseResult result = parse(filePath);
	if(result.fHasSucceed)
		dumpResult(&result);
	
}