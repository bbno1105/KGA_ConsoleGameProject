#include "stdafx.h"
#include "CsvParser.h"

// [csv 저장 규칙]
// 쉼표(,) : 열 구분
// 줄바꿈(\n) : 행 구분 
// 셀의 문자열에 쉼표와 줄바꿈이 있는 경우, 큰 따옴표로 구분 ( "이렇,게" )
// 셀의 문자열에 큰따옴표가 있는 경우, (" -> "")  "이렇게 ""가나다"" 변한다"

// 받는 형식은 첫 줄에 구분할 식별자, 두번째 줄 부터 데이터
// int와 string 타입으로만 구분

// [문제발생]
// char 타입으로 만들어서 wchar로 바꿔야함 

FILE* fp;

char* csv;
wchar_t* wchar_csv;
wchar_t save[500] = L"";

wchar_t* ConvertCtoWC(char* str)
{
	static wchar_t unicode[500] = L"";
	int strLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), unicode, strLen);
	return unicode;
}

wchar_t* ParseCsv(wchar_t* csvStr, wchar_t* saveStr)
{
	bool isString = false;
	while (true)
	{
		if (isString)
		{
			if (*csvStr == L'"' && *(csvStr + 1) == L'"')
			{
				*saveStr = *csvStr;
				saveStr++; csvStr++; csvStr++;
			}
			else if (*csvStr == L'"')
			{
				isString = false;
				csvStr++;
			}
			else if (*csvStr == L'\n')
			{
				*saveStr = *csvStr;
				saveStr++; csvStr++;

				csv = fgets(csv, 500, fp);
				csvStr = ConvertCtoWC(csv);
				continue;
			}
			else
			{
				*saveStr = *csvStr;
				saveStr++; csvStr++;
			}
		}
		else
		{
			if (*csvStr == L'"')
			{
				isString = true;
				csvStr++;
			}
			else if (*csvStr == L',' || *csvStr == L'\n')
			{
				*saveStr = NULL;
				*saveStr++; *csvStr++;
				break;
			}
			else
			{
				*saveStr = *csvStr;

				saveStr++; csvStr++;
			}
		}
	}
	return csvStr;
}

void CsvParse_Init(void)
{
	fp = fopen("test.csv", "rt");

	setlocale(LC_ALL, "kr_KR.utf8");

	csv = malloc(sizeof(int) * 500);
	wchar_csv = malloc(sizeof(int) * 500);

	int idNum = 0;
	int dataNum = 0;

	while (true)
	{
		csv = fgets(csv, 500, fp);
		if (feof(fp)) break;

		wchar_csv = ConvertCtoWC(csv);


		while (*wchar_csv != NULL)
		{
			wchar_csv = ParseCsv(wchar_csv, save);

			if (idNum > 0)
			{
				// @@@@@@@@@ 형식에 맞게 여기를 수정해야함 @@@@@@@@@
				switch (dataNum)
				{
				case 0:
					Data[idNum].ID = atoi(save);
					break;
				case 1:
					wcscpy(Data[idNum].String, save);
					break;
				}
			}
			dataNum++;
		}
		dataNum = 0;
		idNum++;
	}

	fclose(fp);
	free(csv);
}

// 사용법 : Data[GetData(10)].String << ID가 10인 값의 String을 불러온다.
int GetData(int ID)
{
	for (int i = 0; i < 100; i++)
	{
		if (Data[i].ID == ID)
		{
			return i;
		}
	}
	return 0;
}
