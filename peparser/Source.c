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
	//read file path
	_tcscanf(TEXT("%s"), filePath);
	//write on new row
	_tprintf(TEXT("\n"));
	if (_tcslen(filePath) < 1)
	{
		_tprintf(TEXT("Invalid file path!\n"));
		return;
	}
	//parse file
	parseResult result = parse(filePath);
	//dump result
	dumpResult(&result);
	//deallocate result
	deallocateStructure(&result);
	
}