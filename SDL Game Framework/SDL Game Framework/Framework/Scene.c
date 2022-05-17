#include "stdafx.h"
#include "Scene.h"

#include "Framework.h"

Scene g_Scene;

static ESceneType s_nextScene = SCENE_NULL;

#pragma region TitleScene

#define SOLID 0
#define SHADED 1
#define BLENDED 2

const wchar_t* str[] = {
	L"¿©±â´Â Å¸ÀÌÆ²¾ÀÀÔ´Ï´Ù. ÅØ½ºÆ®¿Í °ü·ÃµÈ ¿©·¯°¡Áö¸¦ Å×½ºÆ®ÇØº¾½Ã´Ù.",
	L"BÅ°¸¦ ´©¸£¸é ÆùÆ®°¡ ±½°Ô º¯ÇÕ´Ï´Ù.",
	L"IÅ°¸¦ ´©¸£¸é ÆùÆ®°¡ ÀÌÅÅ¸¯Ã¼·Î º¯ÇÕ´Ï´Ù.",
	L"UÅ°¸¦ ´©¸£¸é ÅØ½ºÆ®¿¡ ¹ØÁÙÀÌ »ı±é´Ï´Ù.",
	L"SÅ°¸¦ ´©¸£¸é ÅØ½ºÆ®¿¡ Ãë¼Ò¼±ÀÌ »ı±é´Ï´Ù.",
	L"NÅ°¸¦ ´©¸£¸é ´Ù½Ã ¿ø·¡´ë·Î µ¹¾Æ¿É´Ï´Ù.",
	L"CÅ°¸¦ ´©¸£¸é ·»´õ ¸ğµå°¡ ¹Ù²ò´Ï´Ù. (Solid -> Shaded -> Blended)",
	L"1Å°¸¦ ´©¸£¸é ÅØ½ºÆ®°¡ ÀÛ¾ÆÁı´Ï´Ù.",
	L"2Å°¸¦ ´©¸£¸é ÅØ½ºÆ®°¡ Ä¿Áı´Ï´Ù.",
	L"½ºÆäÀÌ½º Å°¸¦ ´©¸£¸é ´ÙÀ½ ¾ÀÀ¸·Î ³Ñ¾î°©´Ï´Ù."
};

typedef struct TitleSceneData
{
	Text	GuideLine[20];
	Text	TestText;
	int32	FontSize;
	int32	RenderMode;
	Image	TestImage;
} TitleSceneData;

void init_title(void)
{
	g_Scene.Data = malloc(sizeof(TitleSceneData));
	memset(g_Scene.Data, 0, sizeof(TitleSceneData));

	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	wchar_t* testtext = ParseToUnicode(csvFile.Items[44][Text_s]); // csvFile.Items[ID+1][ÄÃ·³¸í]

	for (int32 i = 0; i < 20; ++i)
	{
		wchar_t stringl[100] = L"";
		testtext = StringLine(testtext, stringl); // ¿©±â¼­ ®c
		Text_CreateText(&data->GuideLine[i], "d2coding.ttf", 16, stringl, wcslen(stringl));
		if (*testtext == NULL) break;
	}

	data->FontSize = 24;
	
	Text_CreateText(&data->TestText, "d2coding.ttf", data->FontSize, testtext, lstrlen(testtext));
	data->RenderMode = SOLID;

	Image_LoadImage(&data->TestImage, "Background.jfif");
}

void update_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	if (Input_GetKeyDown('B'))
	{
		Text_SetFontStyle(&data->TestText, FS_BOLD);
	}

	if (Input_GetKeyDown('I'))
	{
		Text_SetFontStyle(&data->TestText, FS_ITALIC);
	}

	if (Input_GetKeyDown('U'))
	{
		Text_SetFontStyle(&data->TestText, FS_UNDERLINE);
	}

	if (Input_GetKeyDown('S'))
	{
		Text_SetFontStyle(&data->TestText, FS_STRIKETHROUGH);
	}

	if (Input_GetKeyDown('N'))
	{
		Text_SetFontStyle(&data->TestText, FS_NORMAL);
	}

	if (Input_GetKeyDown('C'))
	{
		data->RenderMode = (data->RenderMode + 1) % 3;
	}

	if (Input_GetKey('1'))
	{
		--data->FontSize;
		Text_SetFont(&data->TestText, "d2coding.ttf", data->FontSize);
	}

	if (Input_GetKey('2'))
	{
		++data->FontSize;
		Text_SetFont(&data->TestText, "d2coding.ttf", data->FontSize);
	}

	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_MAIN);
	}
}

void render_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	for (int32 i = 0; i < 20; ++i)
	{
		SDL_Color color = { .a = 255 };
		Renderer_DrawTextSolid(&data->GuideLine[i], 10, 20 * i, color);
	}
	
	switch (data->RenderMode)
	{
	case SOLID:
	{
		SDL_Color color = { .a = 255 };
		Renderer_DrawTextSolid(&data->TestText, 400, 400, color);
	}
	break;
	case SHADED:
	{
		SDL_Color bg = { .a = 255 };
		SDL_Color fg = { .r = 255, .g = 255, .a = 255 };
		Renderer_DrawTextShaded(&data->TestText, 400, 400, fg, bg);
	}
	break;
	case BLENDED:
	{
		Renderer_DrawImage(&data->TestImage, 400, 400);
		SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
		Renderer_DrawTextBlended(&data->TestText, 400, 400, color);
	}
	break;
	}
}

void release_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	for (int32 i = 0; i < 10; ++i)
	{
		Text_FreeText(&data->GuideLine[i]);
	}
	Text_FreeText(&data->TestText);

	SafeFree(g_Scene.Data);
}
#pragma endregion

#pragma region MainScene
const wchar_t* str2[] = {
	L"¿©±â¼­´Â »ç¿îµå¿Í ÀÌ¹ÌÁö ºí·»µù¿¡ ´ëÇØ¼­ ¾Ë¾Æº¾½Ã´Ù.",
	L"È­»ìÇ¥Å°·Î ÀÌ¹ÌÁö¸¦ ÀÌµ¿½ÃÅ³ ¼ö ÀÖ½À´Ï´Ù.",
	L"EÅ°¸¦ ´©¸£¸é ÀÌÆåÆ®¸¦ Àç»ı½ÃÅ³ ¼ö ÀÖ½À´Ï´Ù. ÀÌÆåÆ® ¼Ò¸®°¡ ÀÛÀ¸´Ï º¼·ıÀ» ³·Ãá ÈÄ ÁøÇàÇÏ¼¼¿ä.",
	L"MÅ°·Î À½¾ÇÀ» ²ô°Å³ª ÄÓ ¼ö ÀÖ½À´Ï´Ù.",
	L"PÅ°·Î À½¾ÇÀ» ¸ØÃß°Å³ª Àç°³ÇÒ ¼ö ÀÖ½À´Ï´Ù.",
	L"1¹ø°ú 2¹øÀ¸·Î º¼·ıÀ» Á¶ÀıÇÒ ¼ö ÀÖ½À´Ï´Ù.",
	L"WASD·Î ÀÌ¹ÌÁöÀÇ ½ºÄÉÀÏÀ» Á¶Á¤ÇÒ ¼ö ÀÖ½À´Ï´Ù.",
	L"KLÅ°·Î ÀÌ¹ÌÁöÀÇ Åõ¸íµµ¸¦ Á¶ÀıÇÒ ¼ö ÀÖ½À´Ï´Ù."
};

#define GUIDELINE_COUNT 8

typedef struct MainSceneData
{
	Text		GuideLine[GUIDELINE_COUNT];
	Music		BGM;
	float		Volume;
	SoundEffect Effect;
	Image		BackGround;
	float		Speed;
	int32		X;
	int32		Y;
	int32		Alpha;
} MainSceneData;

void logOnFinished(void)
{
	LogInfo("You can show this log on stopped the music");
}

void log2OnFinished(int32 channel)
{
	LogInfo("You can show this log on stopped the effect");
}

void init_main(void)
{
	g_Scene.Data = malloc(sizeof(MainSceneData));
	memset(g_Scene.Data, 0, sizeof(MainSceneData));

	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
	{
		Text_CreateText(&data->GuideLine[i], "d2coding.ttf", 16, str2[i], wcslen(str2[i]));
	}
	
	Image_LoadImage(&data->BackGround, "background.jfif");

	Audio_LoadMusic(&data->BGM, "powerful.mp3");
	Audio_HookMusicFinished(logOnFinished);
	Audio_LoadSoundEffect(&data->Effect, "effect2.wav"); // Data[GetCsvData(1)].String
	Audio_HookSoundEffectFinished(log2OnFinished);
	Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);

	data->Volume = 1.0f;

	data->Speed = 400.0f;
	data->X = 400;
	data->Y = 400;
	data->Alpha = 255;
}

void update_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	if (Input_GetKeyDown('E'))
	{
		Audio_PlaySoundEffect(&data->Effect, 1);
	}

	if (Input_GetKeyDown('M'))
	{
		if (Audio_IsMusicPlaying())
		{
			Audio_Stop();
		}
		else
		{
			Audio_Play(&data->BGM, INFINITY_LOOP);
		}
	}

	if (Input_GetKeyDown('P'))
	{
		if (Audio_IsMusicPaused())
		{
			Audio_Resume();
		}
		else
		{
			Audio_Pause();
		}
	}

	if (Input_GetKey('1'))
	{
		data->Volume -= 0.01f;
		Audio_SetVolume(data->Volume);
	}

	if (Input_GetKey('2'))
	{
		data->Volume += 0.01f;
		Audio_SetVolume(data->Volume);
	}

	if (Input_GetKey(VK_DOWN))
	{
		data->Y += data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey(VK_UP))
	{
		data->Y -= data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey(VK_LEFT))
	{
		data->X -= data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey(VK_RIGHT))
	{
		data->X += data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey('W'))
	{
		data->BackGround.ScaleY -= 0.05f;
	}

	if (Input_GetKey('S'))
	{
		data->BackGround.ScaleY += 0.05f;
	}

	if (Input_GetKey('A'))
	{
		data->BackGround.ScaleX -= 0.05f;
	}

	if (Input_GetKey('D'))
	{
		data->BackGround.ScaleX += 0.05f;
	}

	if (Input_GetKey('K'))
	{
		data->Alpha = Clamp(0, data->Alpha - 1, 255);
		Image_SetAlphaValue(&data->BackGround, data->Alpha);
	}

	if (Input_GetKey('L'))
	{
		data->Alpha = Clamp(0, data->Alpha + 1, 255);
		Image_SetAlphaValue(&data->BackGround, data->Alpha);
	}
}

void render_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
	{
		SDL_Color color = { .a = 255 };
		Renderer_DrawTextSolid(&data->GuideLine[i], 10, 20 * i, color);
	}

	Renderer_DrawImage(&data->BackGround, data->X, data->Y);
}

void release_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
	{
		Text_FreeText(&data->GuideLine[i]);
	}
	Audio_FreeMusic(&data->BGM);
	Audio_FreeSoundEffect(&data->Effect);

	SafeFree(g_Scene.Data);
}
#pragma endregion

bool Scene_IsSetNextScene(void)
{
	if (SCENE_NULL == s_nextScene)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Scene_SetNextScene(ESceneType scene)
{
	assert(s_nextScene == SCENE_NULL);
	assert(SCENE_NULL < scene&& scene < SCENE_MAX);

	s_nextScene = scene;
}

void Scene_Change(void)
{
	assert(s_nextScene != SCENE_NULL);

	if (g_Scene.Release)
	{
		g_Scene.Release();
	}

	switch (s_nextScene)
	{
	case SCENE_TITLE:
		g_Scene.Init = init_title;
		g_Scene.Update = update_title;
		g_Scene.Render = render_title;
		g_Scene.Release = release_title;
		break;
	case SCENE_MAIN:
		g_Scene.Init = init_main;
		g_Scene.Update = update_main;
		g_Scene.Render = render_main;
		g_Scene.Release = release_main;
		break;
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}