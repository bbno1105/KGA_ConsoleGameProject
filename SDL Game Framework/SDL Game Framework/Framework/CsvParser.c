#include "stdafx.h"

//int main2(void)
//{
//	CsvFile csvFile;
//	memset(&csvFile, 0, sizeof(CsvFile));
//	CreateCsvFile(&csvFile, "Data.csv");
//
//	// CSV 파일 파싱한 후 텍스트 그려본 다음 제대로 출력 안되면
//	// App_Init()에 아래 구문 추가
//	setlocale(LC_ALL, "kr_KR.utf8");
//
//	wchar_t* str = ParseToUnicode(csvFile.Items[Text][3]);
//	wprintf(L"%s", str);
//	
//	return 0;
//}