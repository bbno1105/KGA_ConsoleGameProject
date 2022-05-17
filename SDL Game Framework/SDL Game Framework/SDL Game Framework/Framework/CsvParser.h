#pragma once

#include "Type.h"

struct Datas
{
	// @@@@@@@@@ 원하는 형식으로 바꿀것 @@@@@@@@@
	int ID;
	wchar_t ScenePage[20][500]; // 20줄 500자
	wchar_t Text[20][500];
	int Select1;
	wchar_t MovingPage1[20][500];
	int Select2;
	wchar_t MovingPage2[20][500];
	int Select3;
	wchar_t MovingPage3[20][500];
	wchar_t SoundFile[20][500];
	wchar_t ImageFile[20][500];

}Data[100];
// 100개의 데이터
// 10개의 컬럼에
// 20줄의 글에
// 500자의 글자

// 사용법 : Data[GetCsvData(ID)].Text[줄] << ID가 10인 값의 String을 불러온다.

int GetCsvData(int ID);

void CsvParse_Init(void);
//1665356385