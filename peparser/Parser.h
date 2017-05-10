#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
#include <winnt.h>
#include "Types.h"
//parse file
parseResult
parse(__in PTCHAR ptfilePath);
void
deallocateStructure(__in pParseResult pResult);