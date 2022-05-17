#pragma once

#include "Type.h"

struct Datas
{
	// @@@@@@@@@ 원하는 형식으로 바꿀것 @@@@@@@@@
	int ID;
	wchar_t String[500];
}Data[100];

<<<<<<< Updated upstream
int GetData(int ID);
=======
// 사용법 : Data[GetCsvData(ID)].Text[줄] << ID가 10인 값의 String을 불러온다.

int GetCsvData(int ID);
>>>>>>> Stashed changes

void CsvParse_Init(void);