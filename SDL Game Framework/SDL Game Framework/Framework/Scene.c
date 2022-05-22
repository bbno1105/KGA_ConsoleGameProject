#include "stdafx.h"
#include "Scene.h"

#include "Framework.h"

Scene g_Scene;

static ESceneType s_nextScene = SCENE_NULL;

#pragma region START

#define SOLID 0
#define SHADED 1
#define BLENDED 2


const wchar_t* str1[] = {
	L"▶게임시작",
	L"▶조작방법",
	L"▶게임종료"
};
typedef struct Start_Data
{
	Image		Start_BackGround_Image;
	Image       Start_Front_Image;
	Image       How_To_Operate;
	Image       Icon;
	Image       Operate_Icon;
	bool        How_To_Operate_Open;

	float		Speed;
	int32		Start_Icon_X;
	int32		Start_Icon_Y;
	int32		Alpha;
	Text        Title_Hyacinth;
	Text        StartMenu[3];

	Music		MainBGM;

} Start_Data;

void init_start(void)
{
	g_Scene.Data = malloc(sizeof(Start_Data));
	memset(g_Scene.Data, 0, sizeof(Start_Data));

	Start_Data* data = (Start_Data*)g_Scene.Data;

	//텍스트 만들기
	Text_CreateText(&data->Title_Hyacinth, "HeirofLightBold.ttf", 65, L"히아신스의 신부", wcslen(L"히아신스의 신부"));

	for (int32 i = 0; i < 3; ++i)
	{
		Text_CreateText(&data->StartMenu[i], "HeirofLightBold.ttf", 30, str1[i], wcslen(str1[i]));
	}

	//이미지 로드
	Image_LoadImage(&data->Start_BackGround_Image, "Background.jpg");
	Image_LoadImage(&data->Start_Front_Image, "Main1.png");
	Image_LoadImage(&data->Icon, "ICON.png");
	Image_LoadImage(&data->How_To_Operate, "HowToOperate.png");
	Image_LoadImage(&data->Operate_Icon, "ICON.png");
	data->Start_Icon_X = 810;
	data->Start_Icon_Y = 710;
	data->Alpha = 255;

	// 사운드
	Audio_LoadMusic(&data->MainBGM, "ID_1_Bgm.wav"); // 비쥐엠
	Audio_Play(&data->MainBGM, INFINITY_LOOP);
	Audio_SetVolume(1.0f);
}

void update_start(void)
{
	Start_Data* data = (Start_Data*)g_Scene.Data;

	// 방향키를 눌러 선택할 위치 변경

	if (Input_GetKeyDown(VK_UP) && data->Start_Icon_Y > 710)
	{
		data->Start_Icon_Y -= 60;
	}
	if (Input_GetKeyDown(VK_DOWN) && data->Start_Icon_Y < 780)
	{
		data->Start_Icon_Y += 60;
	}
	if (Input_GetKeyDown(VK_SPACE))
	{
		if (data->Start_Icon_Y == 710)
		{
			Scene_SetNextScene(SCENE_TITLE);
		}
		else if (data->Start_Icon_Y == 770)
		{
			data->How_To_Operate_Open = !data->How_To_Operate_Open;
		}
		else
		{
			exit(0);
		}
	}
}

void render_start(void)
{
	Start_Data* data = (Start_Data*)g_Scene.Data;

	//이미지 사이즈
	data->Start_BackGround_Image.Width = 1920;
	data->Start_BackGround_Image.Height = 1080;
	data->Start_Front_Image.Width = 830;
	data->Start_Front_Image.Height = 390;
	data->Icon.Width = 35;
	data->Icon.Height = 35;
	data->Operate_Icon.Width = 35;
	data->Operate_Icon.Height = 35;

	//이미지 투명도 및 이미지 출력
	Renderer_DrawImage(&data->Start_BackGround_Image, 0, 0);
	Renderer_DrawImage(&data->Start_Front_Image, 520, 125);
	Renderer_DrawImage(&data->Icon, data->Start_Icon_X, data->Start_Icon_Y);


	//텍스트 출력
	SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = data->Alpha };
	Renderer_DrawTextBlended(&data->Title_Hyacinth, 700, 550, color);

	int StartSelectText_Y[3] = { 700, 740, 780 };
	for (int32 i = 0; i < 3; ++i)
	{
		SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = data->Alpha };
		Renderer_DrawTextBlended(&data->StartMenu[i], 850, StartSelectText_Y[i] + 20 * i, color);
	}

	//조작방법 출력
	if (data->How_To_Operate_Open == true)
	{
		Renderer_DrawImage(&data->How_To_Operate, 0, 0);
		Renderer_DrawImage(&data->Operate_Icon, 1195, 795);
	}

}

void release_start(void)
{
	Start_Data* data = (Start_Data*)g_Scene.Data;
	Text_FreeText(&data->Title_Hyacinth);

	for (int32 i = 0; i < 3; ++i)
	{
		Text_FreeText(&data->StartMenu[i]);
	}

	Audio_FreeMusic(&data->MainBGM);

	SafeFree(g_Scene.Data);
}

#pragma endregion

#pragma region TitleScene

#define LOADINGTIME 5;

typedef struct TitleSceneData
{
	// 플레이어 데이터
	int32   ID;
	int32   PlayerDieCount;
	bool    isPlayerReturn;
	Text    PlayerReturnCountText;

	// 메뉴
	Image   MenuImage;
	Text    Escape;
	Text    SelectMenu[3];
	Image   MenuIcon;
	int32   SelectMenuValue;
	bool    isEscapeActive;

	// 로딩바
	float   CountTime;
	Image   LoadingBarFrame;
	Image   LoadingBar;
	bool    isLoading;

    // 텍스트 관련
    Text    GuideLine[50];          // [id][줄바꿈]
    int32   TextLine;               // 텍스트 줄
    int32   TotalLine;              // 총 몇줄인지 체크
    int32   FontSize;
    int32   RenderMode;
    int32   TextEffect;             // 몇번 이펙트인지
    float   TextEffectElapsedTime;
    int32   Text_X;
    int32   Text_Y;
	bool    isSkip;

    // 선택지관련
    int32	Pointer_X;
    int32	Pointer_Y;
    Text    SelectText[3];       // [선택지3개][선택지글자개수]
    int32   SelectId;            // 씬 인덱스
    int32   SelectMovingPage[3]; // 씬 전환값
    bool    MovingPageSelected[200][3];   // 선택지 선택여부를 위한 변수
    int32   selectIDCount;

	// 이미지관련
	Image   BackGroundImage;
	Image   FrontImage;
	Image   Icon;
	int32	Icon_X;
	int32	Icon_Y;
	int32	Alpha;
	float   ImageActiveTime;

	//Fade In/Out 관련
	bool	isFade;
	Image   Black_Image;
	int32   FadeInOut_Alpha;
	bool    FadeInOut_Alpha_bool;
	//Eyes In/Out 관련
	Image   EyesImage_Up;
	int32	EyesImage_Up_Y;
	Image   EyesImage_Down;
	int32	EyesImage_Down_Y;
	bool    EyesImage_bool;

	// 사운드관련
	bool    isBGM;
	Music   BGM;
	char    NowBGM[20];
	float   BGM_Volume;
	bool    isSE;
	SoundEffect   SE;
	float   SE_Volume;
	float   SoundActiveTime;


} TitleSceneData;

int returnPoint2[] = { 4, 7, 9, 12, 14, 18, 21, 27, 33, 38, 42, 45 };
int returnPoint73[] = { 64, 72, 78, 80 };
int returnPoint92[] = { 88, 91, 97, 99, 129 };
int returnPoint128 = 1;

void init_title(void)
{
	// [ 공통 ]
	// 이닛 타이틀 데이터가 들어갈 공간 만들기
	g_Scene.Data = malloc(sizeof(TitleSceneData));
	memset(g_Scene.Data, 0, sizeof(TitleSceneData));

	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	data->ID = 122;               // ID 1부터 시작

	data->isPlayerReturn = false; // 회귀중이냐
	

	// [ 메뉴 ]
	data->isEscapeActive = false;
	Text_CreateText(&data->Escape, "HeirofLightBold.ttf", 25, L"메뉴 : ESC", wcslen(L"메뉴 : ESC"));
	Image_LoadImage(&data->MenuImage, "IngameMenu.png");
	data->SelectMenuValue = 0;
	Text_CreateText(&data->SelectMenu[0], "HeirofLightBold.ttf", 30, L"게임으로 돌아가기", wcslen(L"게임으로 돌아가기"));
	Text_CreateText(&data->SelectMenu[1], "HeirofLightBold.ttf", 30, L"처음 화면으로", wcslen(L"처음 화면으로"));
	Text_CreateText(&data->SelectMenu[2], "HeirofLightBold.ttf", 30, L"게임 종료", wcslen(L"게임 종료"));
	Image_LoadImage(&data->MenuIcon, "ICON.png");

	// [ 로딩바 ]
	data->CountTime = LOADINGTIME;
	Image_LoadImage(&data->LoadingBarFrame, "LoadingBarFrame.png");
	Image_LoadImage(&data->LoadingBar, "LoadingBar.png");
	data->isLoading = false;

	// [ 텍스트 ]
	data->TextLine = 0;         // ID안의 텍스트 줄 0부터 시작
	data->FontSize = 18;        // 데이터 폰트 사이즈 설정
	data->RenderMode = BLENDED;   // 랜더보드 : 글자만 나오게
	data->TotalLine = 0;
	data->isSkip = false;
	memset(data->MovingPageSelected, false, sizeof(data->MovingPageSelected)); // 전부 false로 초기화
    data->TextEffect = 0;

	// testtext에 Test_s 내용추가
	wchar_t* IdText = ParseToUnicode(csvFile.Items[data->ID + 1][Text_s]); // csvFile.Items[ID+1][컬럼명]

	for (int32 i = 0; i < 20; ++i)
	{
		wchar_t stringl[500] = L""; //텍스트 줄 저장
		IdText = StringLine(IdText, stringl); // IdText안에서 널문자 만날 때 마다 스트링 저장
		Text_CreateText(&data->GuideLine[i], "HeirofLightBold.ttf", data->FontSize, stringl, wcslen(stringl));//스트링 출력
		data->TotalLine++;

		if (*IdText == NULL)
		{
			break; // 토탈라인 플러스 되는거 멈춤
		}
	}

	// [ 선택지 ]
	data->SelectId = 0;
	// SelectText 1, 2, 3에 각각의 cvs select1, 2, 3 문자열 넣음
	Text_CreateText(&data->SelectText[0], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s])));
	Text_CreateText(&data->SelectText[1], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s])));
	Text_CreateText(&data->SelectText[2], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s])));
	// SelectMovingPage 1, 2, 3에 각각의 cvs [MovingPage 1, 2, 3 문자열 넣음
	data->SelectMovingPage[0] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage1_i]);
	data->SelectMovingPage[1] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage2_i]);
	data->SelectMovingPage[2] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage3_i]);

	// [ 사운드 ]
	data->isBGM = false;
	// BGM
	strcpy(data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
	Audio_LoadMusic(&data->BGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
	Audio_Play(&data->BGM, INFINITY_LOOP);
	data->BGM_Volume = 1.0f;
	data->SoundActiveTime = 0.0f;
	Audio_SetVolume(data->BGM_Volume);
	// SE
	if (*ParseToAscii(csvFile.Items[data->ID + 1][SE]) != NULL)
	{
		Audio_LoadSoundEffect(&data->SE, ParseToAscii(csvFile.Items[data->ID + 1][SE]));
		if (ParseToInt(csvFile.Items[data->ID + 1][SE_loop]))
		{
			Audio_PlaySoundEffect(&data->SE, INFINITY_LOOP); // 1 : 무한루프
		}
		else
		{
			Audio_PlaySoundEffect(&data->SE, 0); // 0 : 1회 재생
		}
		data->SE_Volume = 1.0f;
		Audio_SetEffectVolume(&data->SE, data->SE_Volume);
	}

	// [ 이미지 ]
	Image_LoadImage(&data->BackGroundImage, "Background.jpg");
	Image_LoadImage(&data->Icon, "ICON.png");
	Image_LoadImage(&data->FrontImage, ParseToAscii(csvFile.Items[data->ID + 1][ImageFile_s]));
	Image_LoadImage(&data->Black_Image, "Black_Image.jpg");
	Image_LoadImage(&data->EyesImage_Up, "up.png");
	Image_LoadImage(&data->EyesImage_Down, "down.png");

	data->Icon_X = 170;
	data->Icon_Y = 855;
	data->ImageActiveTime = 0.0f;
	data->Alpha;

	// [ 페이드 인/아웃 ] 
	data->isFade = false;
	data->EyesImage_Up_Y = -700;
	data->EyesImage_Down_Y = 1080;
	data->FadeInOut_Alpha = 0;
	data->FadeInOut_Alpha_bool = true;
	data->EyesImage_bool = true;

    data->Text_X = 0;
    data->Text_Y = 0;
}

    

void update_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	// [ 페이드 인/아웃 ]
	static float FadeInOutElapsedTime; // 화면
	static float EyesElapsedTime; // 눈

	FadeInOutElapsedTime += Timer_GetDeltaTime();
	EyesElapsedTime += Timer_GetDeltaTime();

	// [ 엔딩 크레딧으로 가기전 연출 ]
	static bool isEnding = false;
	static float endingTime;
	if (isEnding)
	{
		endingTime += Timer_GetDeltaTime();

		data->isFade = true;

		if (FadeInOutElapsedTime >= 0.01f && data->FadeInOut_Alpha_bool)
		{
			data->FadeInOut_Alpha += 4;
			if (data->FadeInOut_Alpha >= 255)
			{
				data->FadeInOut_Alpha = 255;
				data->FadeInOut_Alpha_bool = false;
			}
			FadeInOutElapsedTime = 0.0f;
		}
		if (EyesElapsedTime >= 0.01f && data->EyesImage_bool)
		{
			data->EyesImage_Up_Y += 10;
			data->EyesImage_Down_Y -= 10;
			if (data->EyesImage_Down_Y <= 380)
			{
				data->EyesImage_Up_Y = 0;
				data->EyesImage_Down_Y = 380;
				data->EyesImage_bool = false;
			}
			EyesElapsedTime = 0.0f;
		}

		if (endingTime > 3 && !data->EyesImage_bool && !data->FadeInOut_Alpha_bool)
		{
			endingTime = 0;
			data->isPlayerReturn = false;
			Scene_SetNextScene(SCENE_ENDING_CREDITS);
		}
		return;
	}

	// [ 회귀 ]
	static float fadeTime; // 페이드 시간
	
	if (data->isPlayerReturn)
	{
		fadeTime += Timer_GetDeltaTime();

		data->isFade = true;

		if (FadeInOutElapsedTime >= 0.01f && data->FadeInOut_Alpha_bool)
		{
			data->FadeInOut_Alpha += 4;
			if (data->FadeInOut_Alpha >= 255)
			{
				data->FadeInOut_Alpha = 255;
				data->FadeInOut_Alpha_bool = false;
			}
			FadeInOutElapsedTime = 0.0f;
		}
		if (EyesElapsedTime >= 0.01f && data->EyesImage_bool)
		{
			data->EyesImage_Up_Y += 10;
			data->EyesImage_Down_Y -= 10;
			if (data->EyesImage_Down_Y <= 380)
			{
				data->EyesImage_Up_Y = 0;
				data->EyesImage_Down_Y = 380;
				data->EyesImage_bool = false;
			}
			EyesElapsedTime = 0.0f;
		}
		
		if (fadeTime > 1 && !data->EyesImage_bool && !data->FadeInOut_Alpha_bool)
		{
			data->PlayerDieCount++;
			// 페이드 인 시작 (2.55초)
			fadeTime = 0;
			data->isPlayerReturn = false;
		}
		return;
	}

	if (FadeInOutElapsedTime >= 0.01f && !data->FadeInOut_Alpha_bool)
	{
		data->FadeInOut_Alpha -= 4;
		if (data->FadeInOut_Alpha <= 0)
		{
			data->FadeInOut_Alpha = 0;
			data->FadeInOut_Alpha_bool = true;
		}
		FadeInOutElapsedTime = 0.0f;
	}
	if (EyesElapsedTime >= 0.01f && !data->EyesImage_bool)
	{
		data->EyesImage_Up_Y -= 10;
		data->EyesImage_Down_Y += 10;

		if (data->EyesImage_Down_Y >= 1080)
		{
			data->EyesImage_Up_Y = -700;
			data->EyesImage_Down_Y = 1080;
			data->EyesImage_bool = true;
		}
		EyesElapsedTime = 0.0f;
	}

	wchar_t playerRetrunCountText[50] = L"";
	wchar_t playerReturnCount[10] = L"";
	wcscat(playerRetrunCountText, L"플레이어 회귀 횟수 : ");
	_itow(data->PlayerDieCount, playerReturnCount, 10);
	wcscat(playerRetrunCountText, playerReturnCount);
	Text_CreateText(&data->PlayerReturnCountText, "HeirofLightBold.ttf", 20, playerRetrunCountText, wcslen(playerRetrunCountText));

	bool RefreshScene = false;
	bool isSkip = false;

	// 델타타임 적용
	static float elapsedTime;
	elapsedTime += Timer_GetDeltaTime();

    // 시간이 증가하면 텍스트 줄 값++
    data->TextEffect = ParseToInt(csvFile.Items[data->ID + 1][TextEffect_i]);
    if (data->TextEffect == 4 && data->TextLine >= ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]))
    {
        if (elapsedTime >= 0.1f)
        {
            if (data->TextLine < 20)
            {
                data->TextLine++;
            }
            elapsedTime = 0.0f;
        }
    }
    else
    {
        if (elapsedTime >= 1.0f)
        {
            if (data->TextLine < 20)
            {
                data->TextLine++;
            }
            elapsedTime = 0.0f;
        }
    }
    
    // 이미지 출력을 위한 델타타임
    data->ImageActiveTime += Timer_GetDeltaTime(); 

	// SE 출력을 위한 델타타임
	data->SoundActiveTime += Timer_GetDeltaTime();

	


	// Esc 누르면 메뉴 띄우기
	if (Input_GetKeyDown(VK_ESCAPE))
	{
		data->isEscapeActive = !data->isEscapeActive;
	}

	// 카운트다운 시작
	if (data->TextLine >= data->TotalLine && data->ID == 123) // 123씬에만 쓰임 (핟으)
	{
		data->isLoading = true;

		data->CountTime -= Timer_GetDeltaTime();
		data->LoadingBar.ScaleX = data->CountTime / LOADINGTIME;

		if (data->CountTime <= 0) // 카운트다운 끝나면 첫번째 선택지 자동 선택
		{
			data->isLoading = false;

			//data->ID = 2; // 테스트용 2번으로 보내는거

			data->ID = data->SelectMovingPage[2]; // 3번 선택지 자동 선택
			data->MovingPageSelected[data->ID][2] = true; // 자동 선택된 선택지 저장

			RefreshScene = true;
		}
	}

	// 다음페이지 넘김 or 텍스트 스킵
	if (Input_GetKeyDown(VK_SPACE))
	{
		if (data->isEscapeActive) // 선택지 선택
		{
			switch (data->SelectMenuValue)
			{
			case 0:
				LogInfo("선택1");
				data->isEscapeActive = false;
				break;
			case 1:
				LogInfo("선택2");
				Scene_SetNextScene(SCENE_START);
				break;
			case 2:
				LogInfo("선택3");
				exit(0);
				break;
			}
		}
		else
		{
			if (data->TextLine < data->TotalLine) // 스킵 기능
			{
				data->TextLine = data->TotalLine;
				data->ImageActiveTime = 4;
				data->SoundActiveTime = 4;
				data->isSkip = true;
				// 시간 델타타임도 올려줌
			}
			else // 진행 기능
			{
				data->MovingPageSelected[data->ID][data->SelectId] = true; // 선택한 선택지 저장

				data->isLoading = false; // 로딩바 삭제
				data->isSkip = false;

				// 여기서 회귀장소 하드코딩 해버리자
				switch (data->SelectMovingPage[data->SelectId])
				{
				case 2 :
					for (int i = 0; i < sizeof(returnPoint2); i++)
					{
						if (data->ID == returnPoint2[i])
						{
							data->isPlayerReturn = true;
						}
					}
					break;

				case 73:
					for (int i = 0; i < sizeof(returnPoint73); i++)
					{
						if (data->ID == returnPoint73[i])
						{
							data->isPlayerReturn = true;
						}
					}
					break;

				case 92:
					for (int i = 0; i < sizeof(returnPoint92); i++)
					{
						if (data->ID == returnPoint92[i])
						{
							data->isPlayerReturn = true;
						}
					}
					break;

				case 125:
						if (data->ID == returnPoint128)
						{
							data->isPlayerReturn = true;
						}
					break;

				default:
					break;
				}
				
				data->ID = data->SelectMovingPage[data->SelectId];

				if (data->ID == 0)
				{
					isEnding = true;
					return;
				}

				RefreshScene = true;
			}
		}
	}

	// 방향키를 눌러 선택할 위치 변경
	if (Input_GetKeyDown(VK_UP))
	{
		if (data->isEscapeActive && data->SelectMenuValue > 0)
		{
			data->SelectMenuValue--;
		}
		else if (!data->isEscapeActive && data->SelectId > 0)
		{
			data->SelectId--;
			LogInfo("SelectId : %d", data->SelectMovingPage[data->SelectId]);
			data->Icon_Y -= 40;
		}
	}
	if (Input_GetKeyDown(VK_DOWN))
	{
		if (data->isEscapeActive && data->SelectMenuValue < 2)
		{
			data->SelectMenuValue++;
		}
		else if (!data->isEscapeActive && data->SelectId < data->selectIDCount)
		{
			data->SelectId++;
			LogInfo("SelectId : %d", data->SelectMovingPage[data->SelectId]);
			data->Icon_Y += 40;
		}
	}

	// 다음 씬 정보 가져오기위해 초기화도 시켜주는곳
	if (RefreshScene)
	{
		data->TextLine = 0; // 텍스트줄 0초기화
		data->TotalLine = 0; // 총 몇줄인지 체크
		data->CountTime = LOADINGTIME;

		data->SelectId = 0; // 선택한 선택지 0으로 초기화
		data->Icon_X = 170;
		data->Icon_Y = 855;

		data->ImageActiveTime = 0.0f; // 이미지
		data->SoundActiveTime = 0.0f; // 이미지

		Image_LoadImage(&data->FrontImage, ParseToAscii(csvFile.Items[data->ID + 1][ImageFile_s]));

		wchar_t* IdText = ParseToUnicode(csvFile.Items[data->ID + 1][Text_s]); // csvFile.Items[ID+1][컬럼명]

		for (int32 i = 0; i < 20; ++i)
		{
			wchar_t stringl[500] = L""; //텍스트 줄 저장
			IdText = StringLine(IdText, stringl); // IdText안에서 널문자 만날 때 마다 스트링 저장
			Text_CreateText(&data->GuideLine[i], "HeirofLightBold.ttf", data->FontSize, stringl, wcslen(stringl));//스트링 출력
			data->TotalLine++;

			if (*IdText == NULL)
			{
				break; // 토탈라인 플러스 되는거 멈춤
			}
		}

		// [ 선택지 ]
		Text_CreateText(&data->SelectText[0], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select1_s])));
		Text_CreateText(&data->SelectText[1], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select2_s])));
		Text_CreateText(&data->SelectText[2], "HeirofLightBold.ttf", 25, ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s]), wcslen(ParseToUnicode(csvFile.Items[data->ID + 1][Select3_s])));
		data->SelectMovingPage[0] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage1_i]);
		data->SelectMovingPage[1] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage2_i]);
		data->SelectMovingPage[2] = ParseToInt(csvFile.Items[data->ID + 1][MovingPage3_i]);

		data->selectIDCount = 0; // 0번 선택지 자동선택
		for (int i = 1; i < 3; i++) // 선택지 몇개인지 찾기
		{
			if (data->SelectMovingPage[i] > 0)
			{
				data->selectIDCount++;
			}
		}

		// [ 사운드 ]
		data->isBGM = true;
		data->isSE = true;
		Audio_StopSoundEffect();
	}
        //// [ 사운드 ]
        //if (strcmp(&data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM])))
        //{
        //    strcpy(data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
        //    Audio_LoadMusic(&data->BGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
        //    Audio_Play(&data->BGM, INFINITY_LOOP);
        //}
        //Audio_StopSoundEffect();
        //if (*ParseToAscii(csvFile.Items[data->ID + 1][SE]) != NULL)
        //{
        //    Audio_LoadSoundEffect(&data->SE, ParseToAscii(csvFile.Items[data->ID + 1][SE]));
        //    if (ParseToInt(csvFile.Items[data->ID + 1][SE_loop]))
        //    {
        //        Audio_PlaySoundEffect(&data->SE, INFINITY_LOOP); // 1 : 무한루프
        //    }
        //    else
        //    {
        //        Audio_PlaySoundEffect(&data->SE, 0); // 0 : 1회 재생
        //    }
        //}
        //LogInfo("Now ID Loading... %d", data->ID);

	// 텍스트 효과 123
	switch (data->TextEffect)
	{
	case 1:
	{
		int random = rand() % 5 + 1;
		data->Text_X = random;
		data->Text_Y = -1 * random;
		break;
	}
	case 2:
		data->Text_X = rand() % 5 + 1;
		break;
	case 3:
		data->Text_Y = rand() % 5 + 1;
		break;
	}

}

void render_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	// [ 이미지 ]
	data->BackGroundImage.Width = 1920;
	data->BackGroundImage.Height = 1080;
	Image_SetAlphaValue(&data->BackGroundImage, 255);
	Renderer_DrawImage(&data->BackGroundImage, 0, 0);

	if (data->ImageActiveTime > ParseToInt(csvFile.Items[data->ID + 1][Image_Time_i]))
	{
		Image_SetAlphaValue(&data->FrontImage, 255);
		Renderer_DrawImage(&data->FrontImage, 980, 200);
	}

	// [ 메뉴 ]
	SDL_Color color = { .r = 100, .g = 85, .b = 70,  .a = 255 };
	Renderer_DrawTextBlended(&data->Escape, 1650, 100, color); // 메뉴 : ESC

    // 델타타임이 늘어남에 따라 늘어난 텍스트 줄 만큼 출력
    for (int32 i = 0; i < data->TextLine && i < data->TotalLine; i++)
    {
        //  업데이트에서 지정한 텍스트 라인 = cvs의 텍스트 라인일 때
        if (data->TextLine == ParseToInt(csvFile.Items[data->ID + 1][TextLine_i])) 
        {
            // 일단 텍스트 이펙트에 cvs의 텍스트 이펙트 집어넣어!
            

            if (data->TextEffect == 1)
            {
                // 텍스트 흔들림 1
                if (i != ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1)
                {
                    Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
                }
                else
                {
                    Renderer_DrawTextBlended(&data->GuideLine[ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1], 200 + data->Text_X, 200 + 40 * i + data->Text_Y, color);
                }
            }

            if (data->TextEffect == 2)
            {
                // 텍스트 흔들림 1
                if (i == ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1)
                {
                    Renderer_DrawTextBlended(&data->GuideLine[ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1], 200 + data->Text_X, 200 + 40 * i, color);
                }
                else
                {
                    Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
                }
            }

            if (data->TextEffect == 3)
            {
                // 텍스트 흔들림 1
                if (i == ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1)
                {
                    Renderer_DrawTextBlended(&data->GuideLine[ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1], 200, 200 + 40 * i + data->Text_Y, color);
                }
                else
                {
                    Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
                }
            }

            if (data->TextEffect == 4)
            {
                // 텍스트 속도 빨라짐
                if (i == ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1)
                {
                    Renderer_DrawTextBlended(&data->GuideLine[ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1], 200 + data->Text_X, 200 + 40 * i + data->Text_Y, color);
                }
                else
                {
                    Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
                }
            }

            if (data->TextEffect == 5)
            {
                // 텍스트 속도 빨라짐
                if (i == ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1)
                {
                    Text_SetFontStyle(&data->GuideLine[ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1], FS_BOLD);
                    Renderer_DrawTextBlended(&data->GuideLine[ParseToInt(csvFile.Items[data->ID + 1][TextLine_i]) - 1], 200 + data->Text_X, 200 + 40 * i + data->Text_Y, color);
                }
                else
                {
                    Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
                }
            }

            
        }
        else if (data->ID > 2 && ParseToInt(csvFile.Items[data->ID + 1][MovingPage1_i]) == 2 && i + 1  == data->TotalLine)
        {
            SDL_Color color = { .r = 255, .g = 0, .b = 0, .a = 255 };
            Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
        }
        else 
        {
            SDL_Color color = { .r = 100, .g = 85, .b = 70,  .a = 255 };
            Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
        }
        //asdf
    }
	// [ 플레이어 데이터 ]
	{
		SDL_Color color = { .r = 100, .g = 85, .b = 70, .a = 255 };
		Renderer_DrawTextBlended(&data->PlayerReturnCountText, 1560, 150, color);
	}

	// [ 선택지 ]
	// 텍스트 줄에 아무것도 없으면 선택지 3줄 출력
	if (data->TextLine >= data->TotalLine)
	{
		// 선택지 출력
		int selectText_Y[3] = { 850, 890, 930 };

		for (int i = 0; i < 3; i++)
		{
			// 이미 선택한 텍스트는 빨간색
			if (data->MovingPageSelected[data->ID][i])
			{
				SDL_Color color = { .r = 100, .g = 0, .b = 80, .a = 255 };
				Renderer_DrawTextBlended(&data->SelectText[i], 200, selectText_Y[i], color);
			}
			else
			{
				SDL_Color color = { .r = 0, .g = 0, .b = 0,  .a = 255 };
				Renderer_DrawTextBlended(&data->SelectText[i], 200, selectText_Y[i], color);
			}
		}

		// 아이콘 출력
		data->Icon.Width = 30;
		data->Icon.Height = 30; //아이콘 이미지 사이즈 조절
		Image_SetAlphaValue(&data->Icon, 255);
		Renderer_DrawImage(&data->Icon, data->Icon_X, data->Icon_Y);
	}

	// [ 사운드 ]
	// 시간이 흐르면
	if (data->isBGM)
	{
		if (!strcmp(L"", ParseToAscii(csvFile.Items[data->ID + 1][BGM])))
		{
			strcpy(data->NowBGM, L"");
			Audio_Stop();
		}
		else if (strcmp(&data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM])))
		{
			strcpy(data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
			Audio_LoadMusic(&data->BGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
			Audio_Play(&data->BGM, INFINITY_LOOP);
		}
		data->isBGM = false;
	}

	if (data->SoundActiveTime > ParseToInt(csvFile.Items[data->ID + 1][SoundTime]) && data->isSE)
	{
		if (!data->isSkip)
		{
			if (*ParseToAscii(csvFile.Items[data->ID + 1][SE]) != NULL)
			{
				Audio_LoadSoundEffect(&data->SE, ParseToAscii(csvFile.Items[data->ID + 1][SE]));
				if (ParseToInt(csvFile.Items[data->ID + 1][SE_loop]))
				{
					Audio_PlaySoundEffect(&data->SE, INFINITY_LOOP); // 1 : 무한루프
				}
				else
				{
					Audio_PlaySoundEffect(&data->SE, 0); // 0 : 1회 재생
				}
			}
			LogInfo("Now ID Loading... %d", data->ID);
		}
		data->isSkip = false;
		data->isSE = false;
	}

	// [ 로딩 바 ]
	if (data->isLoading)
	{
		SDL_Color color = { .r = 0, .g = 0, .b = 0,  .a = 200 };
		Renderer_DrawImage(&data->LoadingBar, 660, 1000, color);
		Renderer_DrawImage(&data->LoadingBarFrame, 660, 1000, color);
	}

	// [ 메뉴 ]
	{
		SDL_Color color = { .r = 100, .g = 85, .b = 70, .a = 255 };
		Renderer_DrawTextBlended(&data->Escape, 1650, 100, color); // 메뉴 : ESC
	}

	if (data->isEscapeActive)
	{
		// 메뉴 출력
		Renderer_DrawImage(&data->MenuImage, 0, 0);
		for (int i = 0; i < 3; i++)
		{
			SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = 255 };
			Renderer_DrawTextBlended(&data->SelectMenu[i], 850, 500 + (i * 90), color);
		}
		data->MenuIcon.Width = 30;
		data->MenuIcon.Height = 30;
		Renderer_DrawImage(&data->MenuIcon, 800, 505 + (data->SelectMenuValue * 90));
	}

	// 페이드 인/아웃 이미지, 눈 떴다/감았다 이미지 출력부분
	if (data->isFade)
	{
		data->Black_Image.Width = 1920;
		data->Black_Image.Height = 1080;
		Image_SetAlphaValue(&data->Black_Image, data->FadeInOut_Alpha);
		Renderer_DrawImage(&data->Black_Image, 0, 0);

		//data->EyesImage_Up.Width = 1920;
		//data->EyesImage_Down.Width = 1920;
		//data->Black_Image.Height = 1080;
		Image_SetAlphaValue(&data->EyesImage_Up, 255);
		Image_SetAlphaValue(&data->EyesImage_Down, 255);
		Renderer_DrawImage(&data->EyesImage_Up, 0, data->EyesImage_Up_Y);
		Renderer_DrawImage(&data->EyesImage_Down, 0, data->EyesImage_Down_Y);
	}
}

void release_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	// [ 플레이어 데이터 ]
	Text_FreeText(&data->PlayerReturnCountText);

	// [ 메뉴 ]
	Image_FreeImage(&data->MenuImage);
	Text_FreeText(&data->Escape);
	for (int i = 0; i < 3; i++)
	{
		Text_FreeText(&data->SelectMenu[i]);
	}
	Image_FreeImage(&data->MenuIcon);

	// [ 로딩바 ]
	Image_FreeImage(&data->LoadingBarFrame);
	Image_FreeImage(&data->LoadingBar);

	// [ 텍스트 ]
	for (int32 i = 0; i < 50; ++i)
	{
		Text_FreeText(&data->GuideLine[i]);
	}

	// [ 선택지 ]
	for (int32 i = 0; i < 3; ++i)
	{
		Text_FreeText(&data->SelectText[i]);
	}

	// [ 이미지 ] 
	Image_FreeImage(&data->BackGroundImage);
	Image_FreeImage(&data->FrontImage);
	Image_FreeImage(&data->Icon);

	// [ Fade In/Out ]
	Image_FreeImage(&data->Black_Image);
	Image_FreeImage(&data->EyesImage_Up);
	Image_FreeImage(&data->EyesImage_Down);

	// [ 사운드 ]
	Audio_FreeMusic(&data->BGM);
	Audio_FreeSoundEffect(&data->SE);

	SafeFree(g_Scene.Data);
}
#pragma endregion


#pragma region Ending_Credits_Scene

const wchar_t* str2[] = {
	L"ENDING CREDITS",
	L"",
	L"   히아신스의 신부",
	L"",
	L"             기획",
	L"           김준영",
	L"           박수현",
	L"           홍혁기",
	L"",
	L"             개발",
	L"           안중재",
	L"           문수진",
	L"           이수연"
};

typedef struct Ending_Credits_Data
{
	Text        Ending_Credits_Text[13];
	int32		Ending_Credits_Text_X;
	int32		Ending_Credits_Text_Y;
	int32		Alpha;
	Image		Ending_Credits_BackGround_Image;
	Image		Ending_Credits_Front_Image;

	Music		CreditsBGM;
}Ending_Credits_Data;

void init_credits(void)
{
	g_Scene.Data = malloc(sizeof(Ending_Credits_Data));
	memset(g_Scene.Data, 0, sizeof(Ending_Credits_Data));

	Ending_Credits_Data* data = (Ending_Credits_Data*)g_Scene.Data;

	//텍스트 만들기
	for (int32 i = 0; i < 13; ++i)
	{
		Text_CreateText(&data->Ending_Credits_Text[i], "HeirofLightBold.ttf", 30, str2[i], wcslen(str2[i]));
	}

	//배경이미지 로드
	Image_LoadImage(&data->Ending_Credits_BackGround_Image, "Background.jpg");
	Image_LoadImage(&data->Ending_Credits_Front_Image, "ending_front_image.png");

	data->Ending_Credits_Text_X = 790;
	data->Ending_Credits_Text_Y = 1000;
	data->Alpha = 225;

	// 사운드
	Audio_LoadMusic(&data->CreditsBGM, "ID_1_Bgm.wav"); // 비쥐엠
	Audio_Play(&data->CreditsBGM, INFINITY_LOOP);
	Audio_SetVolume(1.0f);
}

void update_credits(void)
{
	Ending_Credits_Data* data = (Ending_Credits_Data*)g_Scene.Data;


	// 크레딧 올리기
	static float elapsedTime;
	elapsedTime += Timer_GetDeltaTime();

	for (int i = data->Ending_Credits_Text_Y; i > 200; i--)
	{
		if (elapsedTime >= 0.01f)
		{
			data->Ending_Credits_Text_Y = data->Ending_Credits_Text_Y - 1;

			elapsedTime = 0.0f;
		}
	}

	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_START);
	}

}

void render_credits(void)
{
	Ending_Credits_Data* data = (Ending_Credits_Data*)g_Scene.Data;

	//배경 출력
	data->Ending_Credits_BackGround_Image.Width = 1920;
	data->Ending_Credits_BackGround_Image.Height = 1080;
	Renderer_DrawImage(&data->Ending_Credits_BackGround_Image, 0, 0);

	// 텍스트 출력
	for (int32 i = 0; i < 13; ++i)
	{
		SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = data->Alpha };
		Renderer_DrawTextBlended(&data->Ending_Credits_Text[i], data->Ending_Credits_Text_X, data->Ending_Credits_Text_Y + 50 * i, color);
	}

	//data->Ending_Credits_Front_Image.Width = 1920;
	//data->Ending_Credits_Front_Image.Height = 1080;
	Renderer_DrawImage(&data->Ending_Credits_Front_Image, 0, 0);
}

void release_credits(void)
{
	Ending_Credits_Data* data = (Ending_Credits_Data*)g_Scene.Data;

	//텍스트 반환
	for (int32 i = 0; i < 13; ++i)
	{
		Text_FreeText(&data->Ending_Credits_Text[i]);
	}

	Audio_FreeMusic(&data->CreditsBGM);

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
	case SCENE_START:
		g_Scene.Init = init_start;
		g_Scene.Update = update_start;
		g_Scene.Render = render_start;
		g_Scene.Release = release_start;
		break;
	case SCENE_TITLE:
		g_Scene.Init = init_title;
		g_Scene.Update = update_title;
		g_Scene.Render = render_title;
		g_Scene.Release = release_title;
		break;
	case SCENE_ENDING_CREDITS:
		g_Scene.Init = init_credits;
		g_Scene.Update = update_credits;
		g_Scene.Render = render_credits;
		g_Scene.Release = release_credits;
		break;
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}