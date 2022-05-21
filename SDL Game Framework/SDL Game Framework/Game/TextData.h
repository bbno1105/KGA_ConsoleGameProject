#pragma once

#include "Framework.h"
#include "Type.h"

typedef struct TagTextData
{
	int32	ID;
	Text    GuideLine[50];       // [id][줄바꿈]
	int32   TextLine;            // 텍스트 줄
	int32   TotalLine;           // 총 몇줄인지 체크
	int32   FontSize;
	COORD	Coord;

	//선택지 관련인데 아이디 업데이트하고 텍스트 출력해야해서 가져옴. 씬c에있는 선택지 일단 지우면 안됨
	int32   MovingPageNumberIndex;            // 씬 인덱스
	int32   SelectMovingPageNumber[3]; // 씬 전환값
	float	ElapsedTimeForRenderingLineByLine;
} TextData;

void TextData_Init(TextData* textData);
void TextData_Update(TextData* textData);
void TextData_Render(TextData* textData);
void TextData_Release(TextData* textData);

void SaveOneTextLine(TextData* textData);

void SwingText(TextData* textData);