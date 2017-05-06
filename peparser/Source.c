#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include <winnt.h>
#include "Parser.h"
#include "Dumper.h"
int main()
{
	parseResult result=parse(TEXT("TestedFiles\\Profile.dll.infected"));
	dumpResult(&result);
	
}