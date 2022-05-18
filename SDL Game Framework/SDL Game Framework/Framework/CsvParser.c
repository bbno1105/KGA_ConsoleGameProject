#include "stdafx.h"

<<<<<<< HEAD
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
=======
// [csv ���� ��Ģ]
// ��ǥ(,) : �� ����
// �ٹٲ�(\n) : �� ���� 
// ���� ���ڿ��� ��ǥ�� �ٹٲ��� �ִ� ���, ū ����ǥ�� ���� ( "�̷�,��" )
// ���� ���ڿ��� ū����ǥ�� �ִ� ���, (" -> "")  "�̷��� ""������"" ���Ѵ�"

// �޴� ������ ù �ٿ� ������ �ĺ���, �ι�° �� ���� ������
// int�� string Ÿ�����θ� ����

// [�����߻�]
// char Ÿ������ ���� wchar�� �ٲ���� 

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
	fp = fopen("Data.csv", "rt");

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
				// @@@@@@@@@ ���Ŀ� �°� ���⸦ �����ؾ��� @@@@@@@@@
				switch (dataNum)
				{
					case 0:
						Data[idNum].ID = atoi(save);
						break;

					case 1:
					{
						int stringNum = 0;	int i = 0;	int j = 0;
						while (true)
						{
							if (save[i] == L'\n')
							{
								stringNum++; i++; j = 0;
								continue;
							}
							if (save[i] == NULL) break;
							Data[idNum].ScenePage[stringNum][j] = save[i];
							i++; j++;
						}
					}
					break;

					case 2:
					{
						int stringNum = 0;	int i = 0;	int j = 0;
						while (true)
						{
							if (save[i] == L'\n')
							{
								stringNum++; i++; j = 0;
								continue;
							}
							if (save[i] == NULL) break;
							Data[idNum].Text[stringNum][j] = save[i];
							i++; j++;
						}
					}
					break;

					case 3:
						Data[idNum].Select1 = atoi(save);
						break;

					case 4:
					{
						int stringNum = 0;	int i = 0;	int j = 0;
						while (true)
						{
							if (save[i] == L'\n')
							{
								stringNum++; i++; j = 0;
								continue;
							}
							if (save[i] == NULL) break;
							Data[idNum].MovingPage1[stringNum][j] = save[i];
							i++; j++;
						}
					}
					break;

					case 5:
						Data[idNum].Select2 = atoi(save);
						break;

					case 6:
					{
						int stringNum = 0;	int i = 0;	int j = 0;
						while (true)
						{
							if (save[i] == L'\n')
							{
								stringNum++; i++; j = 0;
								continue;
							}
							if (save[i] == NULL) break;
							Data[idNum].MovingPage2[stringNum][j] = save[i];
							i++; j++;
						}
					}
					break;

					case 7:
						Data[idNum].Select3 = atoi(save);
						break;

					case 8:
					{
						int stringNum = 0;	int i = 0;	int j = 0;
						while (true)
						{
							if (save[i] == L'\n')
							{
								stringNum++; i++; j = 0;
								continue;
							}
							if (save[i] == NULL) break;
							Data[idNum].MovingPage3[stringNum][j] = save[i];
							i++; j++;
						}
					}
					break;

					case 9:
					{
						int stringNum = 0;	int i = 0;	int j = 0;
						while (true)
						{
							if (save[i] == L'\n')
							{
								stringNum++; i++; j = 0;
								continue;
							}
							if (save[i] == NULL) break;
							Data[idNum].SoundFile[stringNum][j] = save[i];
							i++; j++;
						}
					}
					break;

					case 10:
					{

						int stringNum = 0;	int i = 0;	int j = 0;
						while (true)
						{
							if (save[i] == L'\n')
							{
								stringNum++; i++; j = 0;
								continue;
							}
							if (save[i] == NULL) break;
							Data[idNum].ImageFile[stringNum][j] = save[i];
							i++; j++;
						}
					}
					break;
				}
			}
			dataNum++;
		}
		dataNum = 0;
		idNum++;
	}

	printf("%d\n", sizeof(Data));

	fclose(fp);
	free(csv);
}

// ���� : Data[GetData(10)].String << ID�� 10�� ���� String�� �ҷ��´�.
int GetCsvData(int ID)
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

>>>>>>> origin/Text_test
