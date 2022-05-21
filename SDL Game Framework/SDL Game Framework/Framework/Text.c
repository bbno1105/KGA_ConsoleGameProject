#include "stdafx.h"
#include "Text.h"
#include "Framework.h"

static char s_path[MAX_PATH];

bool Text_Init(void)
{
	if (0 != TTF_Init())
	{
		return false;
	}

	return true;
}

void Text_Cleanup(void)
{
	TTF_Quit();
}

void Text_CreateText(Text* text, const char* fontFile, int32 fontSize, const wchar_t* str, int32 length)
{
	Text_SetFont(text, fontFile, fontSize);
	
	text->String = malloc(sizeof(wchar_t) * (length + 1));
	for (int32 i = 0; i < length; ++i)
	{
		(text->String)[i] = str[i];
	}
	(text->String)[length] = L'\0';
	
	text->Length = length;
}

void Text_FreeText(Text* text)
{
	TTF_CloseFont(text->Font);
	text->Font = NULL;

	SafeFree(text->String);
	
	text->Length = 0;
}

void Text_SetFont(Text* text, const char* fontFile, int32 fontSize)
{
	if (text->Font)
	{
		TTF_CloseFont(text->Font);
	}

	LogInfo("Loading Font : %s", fontFile);
	sprintf_s(s_path, sizeof(s_path), "%s/%s", FONT_DIRECTORY, fontFile);
	text->Font = TTF_OpenFont(s_path, fontSize);
}

void Text_SetFontStyle(Text* text, EFontStyle style)
{
	TTF_SetFontStyle(text->Font, style);
}

EFontStyle Text_GetFontStyle(const Text* text)
{
	return (EFontStyle)TTF_GetFontStyle(text->Font);
}


void TextEffext_1(Text* text)
{
	text->ElapsedTime += Timer_GetDeltaTime();
	if (text->ElapsedTime >= 0.5f)
	{
		text->ElapsedTime = 0;
		int32 x = Random_GetNumberFromRange(10, 30);
		int32 y = Random_GetNumberFromRange(10, 30);
		SetCoord(text->TextCoord, x, y);
	}
}

void TextEffext_2(Text* text)
{
	text->ElapsedTime += Timer_GetDeltaTime();
	if (text->ElapsedTime >= 0.5f)
	{
		text->ElapsedTime = 0;
		int32 x = Random_GetNumberFromRange(10, 30);
		int32 y = Random_GetNumberFromRange(10, 30);
		SetCoord(text->TextCoord, x, y);
	}
}

void TextEffext_3(Text* text)
{
	text->ElapsedTime += Timer_GetDeltaTime();
	if (text->ElapsedTime >= 0.5f)
	{
		text->ElapsedTime = 0;
		int32 x = Random_GetNumberFromRange(10, 30);
		int32 y = Random_GetNumberFromRange(10, 30);
		SetCoord(text->TextCoord, x, y);
	}
}