#include "stdafx.h"
#include "TextData.h"

void TextData_Init(TextData* textData, int32 id)
{
	memset(textData, 0, sizeof(TextData));
	textData->FontSize = 18;
    // idText에 Text_s 내용추가
      wchar_t* idText = ParseToUnicode(csvFile.Items[id + 1][Text_s]); // csvFile.Items[ID+1][컬럼명]

    SaveOneTextLine(textData, idText);
}

void TextData_Update(TextData* textData, int32 id)
{
	// 델타타임 적용
    textData->ElapsedTimeForRenderingLineByLine += Timer_GetDeltaTime();
    
    // 시간이 증가하면 텍스트 줄 값++
    if (textData->ElapsedTimeForRenderingLineByLine >= 1.0f)
    {
        if (textData->TextLine < 20)
        {
            textData->TextLine++;
        }
        textData->ElapsedTimeForRenderingLineByLine = 0.0f;
    }

    if (Input_GetKeyDown(VK_SPACE))
    {
        textData->MovingPageNumberIndex = 0; // 선택한 선택지 0으로 초기화
       
        if (textData->TextLine < textData->TotalLine)
        {
            textData->TextLine = textData->TotalLine;
        }
        else
        {
            textData->TextLine = 0; // 텍스트줄 0초기화
            textData->TotalLine = 0; // 총 몇줄인지 체크
            
            wchar_t* idText = ParseToUnicode(csvFile.Items[id + 1][Text_s]); // csvFile.Items[ID+1][컬럼명]

            SaveOneTextLine(textData, idText);
            
            //SafeFree(idText);           
        }
    }   
}

void TextData_Render(TextData* textData, int32 id)
{
    // 델타타임이 늘어남에 따라 늘어난 텍스트 줄 만큼 출력
    for (int32 i = 0; i < textData->TextLine && i < textData->TotalLine; i++)
    {
        if (id > 2 && ParseToInt(csvFile.Items[id + 1][MovingPage1_i]) == 2 && i + 1 == textData->TotalLine)
        {
            SDL_Color color = { .r = 255, .g = 0, .b = 0, .a = 255 * 0.7 };
            Renderer_DrawTextBlended(&textData->GuideLine[i], 200, 200 + 40 * i, color);
        }
        else
        {
            SDL_Color color = { .r = 90, .g = 85, .b = 70,  .a = 255 };
            Renderer_DrawTextBlended(&textData->GuideLine[i], 200, 200 + 40 * i, color);
        }
    }
}

void TextData_Release(TextData* textData)
{
    for (int32 i = 0; i < 50; ++i)
    {
        Text_FreeText(&textData->GuideLine[i]);
    }
}

void SaveOneTextLine(TextData* textData, wchar_t* idText)
{
    for (int32 i = 0; i < 20; ++i)
    {
        wchar_t stringl[500] = L""; //텍스트 줄 저장
        idText = StringLine(idText, stringl); // IdText안에서 널문자 만날 때 마다 스트링 저장
        Text_CreateText(&textData->GuideLine[i], "HeirofLightBold.ttf", textData->FontSize, stringl, wcslen(stringl));//스트링 출력
        textData->TotalLine++;

        if (*idText == NULL)
        {
            break; // 토탈라인 플러스 되는거 멈춤
        }
    }
}

void SwingText(TextData* textData, int32 id)
{
    int32 x = Random_GetFNumberFromRange(10, 30);
    int32 y = Random_GetFNumberFromRange(0, 15);
    //SetCoord(TextData->Coord, x, y);
}