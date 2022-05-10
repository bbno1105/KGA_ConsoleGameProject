#include "App.h"
#include "Common.h"
#include "Renderer.h"
#include "Timer.h"

bool App_Init()
{
	if (false == Renderer_Init())
	{
		return false;
	}
	return true;
}

void processInput()
{

}

float elapsedTime = 0;
bool canShow = false;

void update()
{
	//char str[128] = "";
	//sprintf_s(str, sizeof(str), "현재 FPS : %d", (int32)(1 / Timer_GetDeltaTime()));
	//Renderer_DrawText(str, strlen(str));

	elapsedTime += 1.0f * DELTA_TIME; // 매크로도 써보자

	if (elapsedTime >= 0.5f)
	{
		elapsedTime = 0.0;
		canShow = !canShow;
	}

	if (canShow)
	{

		Renderer_DrawText("Hello Game", sizeof("Hello Game"), 5,5);
	}


}

void render()
{
	Renderer_Flip();
}

void cleanup()
{
	Renderer_Cleanup();
}

int32 App_Run()
{
	atexit(cleanup); // 프로그램이 종료될 때 cleanup이 자동으로 실행

	Timer_Init(60); // 루프 들어가기 전

	// Game Loop : 게임을 동작시키는 루프. 프레임(Frame)이라고 한다.
	while (true)
	{
		if (Timer_Update())
		{
			processInput();
			update();
			render();
		}
	}
	return 0;
}