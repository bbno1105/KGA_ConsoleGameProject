#pragma once

#include "Type.h"

struct Datas
{
	// @@@@@@@@@ 원하는 형식으로 바꿀것 @@@@@@@@@
	int ID;
	wchar_t String[500];
}Data[100];

int GetData(int ID);

void CsvParse_Init(void);