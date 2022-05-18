#pragma once

#include "Type.h"
#include <wchar.h>

#define MAXIMUM_ROW 10000

typedef struct CsvItem
{
	char*	RawData;
} CsvItem;

typedef struct CsvFile
{
	int			ColumnCount;
	int			RowCount;
	CsvItem*	Items[MAXIMUM_ROW];
} CsvFile;

CsvFile csvFile;

void CreateCsvFile(CsvFile* csvFile, const char* filename);
void csv_Init(void);

int			ParseToInt(const CsvItem item);
char*		ParseToAscii(const CsvItem item);
wchar_t*	ParseToUnicode(const CsvItem item);
wchar_t*	StringLine(wchar_t* string, wchar_t* stringl);

enum column
{
	ID_i,
	ScenePage_s,
	Text_s,
	Selet1_s,
	MovingPage1_i,
	Selet2_s,
	MovingPage2_i,
	Selet3_s,
	MovingPage3_i,
	SoundFile_s,
	ImageFile_s
};