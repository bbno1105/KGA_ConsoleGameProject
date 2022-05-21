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

} Start_Data;

void init_start(void)
{
    g_Scene.Data = malloc(sizeof(Start_Data));
    memset(g_Scene.Data, 0, sizeof(Start_Data));

    Start_Data* data = (Start_Data*)g_Scene.Data;

    //텍스트 만들기
    Text_CreateText(&data->Title_Hyacinth, "HeirofLightBold.ttf", 50, L"히아신스의 신부", wcslen(L"히아신스의 신부"));

    for (int32 i = 0; i < 3; ++i)
    {
        Text_CreateText(&data->StartMenu[i], "HeirofLightBold.ttf", 30, str1[i], wcslen(str1[i]));
    }

    Image_LoadImage(&data->How_To_Operate, "HowToOperate.png");
    Image_LoadImage(&data->Operate_Icon, "ICON.png");

    //이미지 로드
    Image_LoadImage(&data->Start_BackGround_Image, "Background.jpg");
    Image_LoadImage(&data->Start_Front_Image, "Main1.png");
    Image_LoadImage(&data->Icon, "ICON.png");

    data->Start_Icon_X = 810;
    data->Start_Icon_Y = 710;
    data->Alpha = 255;

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
    data->Start_Front_Image.Width = 800;
    data->Start_Front_Image.Height = 360;
    data->Icon.Width = 35;
    data->Icon.Height = 35;
    data->Operate_Icon.Width = 35;
    data->Operate_Icon.Height = 35;
    
    //이미지 투명도 및 이미지 출력
    Renderer_DrawImage(&data->Start_BackGround_Image, 0, 0);
    Renderer_DrawImage(&data->Start_Front_Image, 560, 125);
    Renderer_DrawImage(&data->Icon, data->Start_Icon_X, data->Start_Icon_Y);


    //텍스트 출력
    SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = data->Alpha};
    Renderer_DrawTextBlended(&data->Title_Hyacinth, 760, 490, color);

    int StartSelectText_Y[3] = { 700, 740, 780 };
    for (int32 i = 0; i < 3; ++i)
    {
        SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = data->Alpha};
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

    SafeFree(g_Scene.Data);
}

#pragma endregion

#pragma region TitleScene

#define LOADINGTIME 5;

typedef struct TitleSceneData
{
    // 모두 관련
    int32   ID;
    int32   PlayerDieCount;
    bool    isPlayerReturn;
    int32   PlayerReturnPoint;

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
    COORD   TextCoord;
    int32   Text_X;
    int32   Text_Y;


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

    // 사운드관련
    Music   BGM;
    char    NowBGM[20];
    float   BGM_Volume;
    SoundEffect   SE;
    float   SE_Volume;

} TitleSceneData;

void init_title(void)
{
    // [ 공통 ]
    // 이닛 타이틀 데이터가 들어갈 공간 만들기
    g_Scene.Data = malloc(sizeof(TitleSceneData));
    memset(g_Scene.Data, 0, sizeof(TitleSceneData));

    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    data->ID = 1;               // ID 1부터 시작

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
    data->TextEffect = 0;
    memset(data->MovingPageSelected, false, sizeof(data->MovingPageSelected)); // 전부 false로 초기화

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
    // BGM
    strcpy(data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
    Audio_LoadMusic(&data->BGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
    Audio_Play(&data->BGM, INFINITY_LOOP);
    data->BGM_Volume = 1.0f;
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

    data->Icon_X = 170;
    data->Icon_Y = 855;
    data->Alpha;
    data->ImageActiveTime = 0.0f;

    //Audio_LoadMusic(&data->BGM, "powerful.mp3");
    //Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);
    data->Text_X = 0;
    data->Text_Y = 0;
}

    

void update_title(void)
{
    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    static float fadeTime;
    if (data->ID == data->PlayerReturnPoint && data->isPlayerReturn)
    {
        fadeTime += Timer_GetDeltaTime();
        // 페이드 아웃 시작 (2.55초)
        if (fadeTime > 3)
        {
            data->PlayerDieCount++;
            // 페이드 인 시작 (2.55초)
            fadeTime = 0;
            LogInfo("회귀중...."); // << 잘 들어오네~
            data->isPlayerReturn = false;
        }
        return;
    }

    bool RefreshScene = false;

    // 델타타임 적용
    static float elapsedTime;
    elapsedTime += Timer_GetDeltaTime();

    // 시간이 증가하면 텍스트 줄 값++
    data->TextEffect = ParseToInt(csvFile.Items[data->ID + 1][TextEffect_i]);
    if (data->TextEffect == 4)
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

    // 텍스트 속도조절
    

    // Esc 누르면 메뉴 띄우기
    if (Input_GetKeyDown(VK_ESCAPE))
    {
        data->isEscapeActive = !data->isEscapeActive;
    }

    // 카운트다운 시작
    if (data->TextLine >= data->TotalLine && data->ID == 2) // 123씬에만 쓰임 (핟으)
    {
        data->isLoading = true;

        data->CountTime -= Timer_GetDeltaTime();
        data->LoadingBar.ScaleX = data->CountTime / LOADINGTIME;

        if (data->CountTime <= 0) // 카운트다운 끝나면 첫번째 선택지 자동 선택
        {
            data->isLoading = false;

            data->ID = 2;

            //data->ID = data->SelectMovingPage[2]; // 선택지 자동 선택
            //data->MovingPageSelected[data->ID][2] = true; // 자동 선택된 선택지 저장

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
            case 0 :
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
                data->ImageActiveTime = 3;
            }
            else // 진행 기능
            {
                data->MovingPageSelected[data->ID][data->SelectId] = true; // 선택한 선택지 저장

                data->isLoading = false; // 로딩바 삭제

                data->ID = data->SelectMovingPage[data->SelectId];         
                
                if (data->ID == data->PlayerReturnPoint)
                {
                    data->isPlayerReturn = true;
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
        if (data->ID == 2) // 회귀시점 저장 (핟으콛잉)
        {
            data->PlayerReturnPoint = 2;
        }

        data->TextLine = 0; // 텍스트줄 0초기화
        data->TotalLine = 0; // 총 몇줄인지 체크
        data->CountTime = LOADINGTIME;

        data->SelectId = 0; // 선택한 선택지 0으로 초기화
        data->Icon_X = 170;
        data->Icon_Y = 855;

        data->ImageActiveTime = 0.0f; // 이미지

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

        // [ 사운드 ]
        if (strcmp(&data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM])))
        {
            strcpy(data->NowBGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
            Audio_LoadMusic(&data->BGM, ParseToAscii(csvFile.Items[data->ID + 1][BGM]));
            Audio_Play(&data->BGM, INFINITY_LOOP);
        }
        Audio_StopSoundEffect();
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

        // [ 선택지 ]
        data->selectIDCount = 0; // 0번 선택지 자동선택
        for (int i = 1; i < 3; i++) // 선택지 몇개인지 찾기
        {
            if (data->SelectMovingPage[i] > 0)
            {
                data->selectIDCount++;
            }
        }
    }

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
    Image_SetAlphaValue(&data->BackGroundImage, 125);
    Renderer_DrawImage(&data->BackGroundImage, 0, 0);
    if (data->ImageActiveTime > ParseToInt(csvFile.Items[data->ID + 1][Image_Time_i]))
    {
        Image_SetAlphaValue(&data->FrontImage, 255);
        Renderer_DrawImage(&data->FrontImage, 1100, 200);
    }
    // [ 텍스트 ]
    SDL_Color color = { .r = 90, .g = 85, .b = 70,  .a = 255 };
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
        }
        else if (data->ID > 2 && ParseToInt(csvFile.Items[data->ID + 1][MovingPage1_i]) == 2 && i + 1  == data->TotalLine)
        {
            SDL_Color color = { .r = 255, .g = 0, .b = 0, .a = 255 };
            Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
        }
        else 
        {
            SDL_Color color = { .r = 90, .g = 85, .b = 70,  .a = 255 };
            Renderer_DrawTextBlended(&data->GuideLine[i], 200, 200 + 40 * i, color);
        }

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
        
        data->Icon.Width = 30;
        data->Icon.Height = 30; //아이콘 이미지 사이즈 조절
        Image_SetAlphaValue(&data->Icon, 255);
        Renderer_DrawImage(&data->Icon, data->Icon_X, data->Icon_Y);
    }

    // [ 로딩 바 ]
    if (data->isLoading)
    {
        Renderer_DrawImage(&data->LoadingBar, 660, 1000, color);
        Renderer_DrawImage(&data->LoadingBarFrame, 660, 1000, color);
    }

    // [ 메뉴 ]
    if (data->isEscapeActive)
    {
        // 메뉴 출력
        Renderer_DrawImage(&data->MenuImage, 0, 0);
        for (int i = 0; i < 3; i++)
        {
            Renderer_DrawTextBlended(&data->SelectMenu[i], 850, 500 + (i * 90), color);
        }
        data->MenuIcon.Width = 30;
        data->MenuIcon.Height = 30;
        Renderer_DrawImage(&data->MenuIcon, 800, 505 + (data->SelectMenuValue * 90));
    }
  

    // 페이드 인

    // 페이드 아웃
}

void release_title(void)
{
    TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

    // [ 이미지 ] 
    Image_FreeImage(&data->MenuImage);
    Image_FreeImage(&data->BackGroundImage);
    Image_FreeImage(&data->FrontImage);
    Image_FreeImage(&data->Icon);
    Image_FreeImage(&data->MenuIcon);
    Image_FreeImage(&data->LoadingBar);
    Image_FreeImage(&data->LoadingBarFrame);

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
 
    // [ 사운드 ]
    Audio_FreeMusic(&data->BGM);
    Audio_FreeSoundEffect(&data->SE);

    SafeFree(g_Scene.Data);
}
#pragma endregion


#pragma region Ending_Credits_Scene

void init_credits(void)
{

}

void update_credits(void)
{

}

void render_credits(void)
{

}

void release_credits(void)
{

}
#pragma endregion


#pragma region 참고용

// #################################################### 참고용 데이터 ####################################################
/*
#pragma region MainScene
#define GUIDELINE_COUNT 8


typedef struct MainSceneData
{
    Text      GuideLine[GUIDELINE_COUNT];
    Music      BGM;
    float      Volume;
    SoundEffect Effect;
    Image      Front;
    Image      BackGround;
    float      Speed;
    int32      X;
    int32      Y;
    int32      Alpha;
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

    Image_LoadImage(&data->Front, "main.png");

    Audio_LoadMusic(&data->BGM, "powerful.mp3");
    Audio_HookMusicFinished(logOnFinished);
    Audio_LoadSoundEffect(&data->Effect, "effect2.wav");
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
        Renderer_DrawTextBlended(&data->GuideLine[i], 100, 400 + 25 * i, color);
    }

    Renderer_DrawImage(&data->Front, 0, 0);
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

//#pragma region ImageScene1
//
//const wchar_t* str3[] = {
//   L"GAME START"
//};
//
//typedef struct Scene1_Data
//{
//    Image      Scene1_BackGround;
//    float      Speed;
//    int32      X;
//    int32      Y;
//    int32      Alpha;
//    Text   GuideLine[10];
//    Text   TestText;
//    int32   FontSize;
//    int32   RenderMode;
//    int32      NextText;
//    int32      check;
//
//} Scene1_Data;
//
//void init_scene_1(void)
//{
//    g_Scene.Data = malloc(sizeof(Scene1_Data));
//    memset(g_Scene.Data, 0, sizeof(Scene1_Data));
//
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//    Image_LoadImage(&data->Scene1_BackGround, "Background.png");
//    (data->Scene1_BackGround.ScaleX = 10);
//    (data->Scene1_BackGround.ScaleY = 10);
//
//    data->Speed = 400.0f;
//    data->X = 0;
//    data->Y = 0;
//    data->Alpha = 100;
//
//
//    data->FontSize = 50;
//    Text_CreateText(&data->TestText, "HeirofLightBold.ttf", data->FontSize, Data[GetCsvData(1)].Text, lstrlen(Data[GetCsvData(1)].Text));
//
//    data->RenderMode = SOLID;
//
//    data->NextText = false;
//    data->check = 0;
//}
//
//void update_scene_1(void)
//{
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//    // 나중에 타자치듯이 하나씩 출력될 예정
//
//}
//
//void render_scene_1(void)
//{
//
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//
//    Image_SetAlphaValue(&data->Scene1_BackGround, data->Alpha);
//    Renderer_DrawImage(&data->Scene1_BackGround, 0, 0);
//
//    for (int32 i = 0; i < 10; ++i)
//    {
//        SDL_Color color = { .a = 255 };
//        Renderer_DrawTextBlended(&data->GuideLine[i], 400, 200 * i, color);
//    }
//
//    switch (data->RenderMode)
//    {
//    case SOLID:
//    {
//        SDL_Color color = { .a = 255 };
//        Renderer_DrawTextBlended(&data->TestText, 400, 400, color);
//    }
//    break;
//    case SHADED:
//    {
//        SDL_Color bg = { .a = 255 };
//        SDL_Color fg = { .r = 255, .g = 255, .a = 255 };
//        Renderer_DrawTextShaded(&data->TestText, 400, 400, fg, bg);
//    }
//    break;
//    }
//
//
//}
//
//void release_scene_1(void)
//{
//    Scene1_Data* data = (Scene1_Data*)g_Scene.Data;
//
//    for (int32 i = 0; i < 10; ++i)
//    {
//        Text_FreeText(&data->GuideLine[i]);
//    }
//    Text_FreeText(&data->TestText);
//
//    SafeFree(g_Scene.Data);
//
//}
//
//#pragma endregion
*/
// ###################################################################################################################

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