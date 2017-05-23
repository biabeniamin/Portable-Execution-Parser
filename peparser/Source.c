/*
PE Parser
Bia Beniamin
V1
*/

#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include <winnt.h>
#include "Parser.h"
#include "Dumper.h"
int main()
{
	TCHAR filePath[MAX_PATH];
	_tprintf(TEXT("File path of file which will be analyzed:"));
	tcscanf(TEXT("%s"), filePath);
	test();
	if (_tcslen(filePath) < 1)
	{
		_tprintf(TEXT("Invalid file path!\n"));
		return;
	}
	parseResult result = parse(filePath);
	dumpResult(&result);
	deallocateStructure(&result);
	
}