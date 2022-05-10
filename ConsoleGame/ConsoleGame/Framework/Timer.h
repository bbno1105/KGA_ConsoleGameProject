#pragma once

/// <summary>
/// 타이머를 초기화 한다.
/// </summary>
/// <param name="fps">고정할 FPS</param>
void Timer_Init(int32 fps);

/// <summary>
/// 타이머를 업데이트 한다.
/// </summary>
/// <return>업데이트를 성공했다면 true, 아니면 false</return>
bool Timer_Update(void);

/// <summary>
/// 델타타임을 구한다.
/// </summary>
/// <param name="">DetaTime : 프레임간의 시간. 이전 프레임으로부터 현재 프레임을 시작할 때까지 걸린 시간</param>
float Timer_GetDeltaTime(void);

#define DELTA_TIME Timer_GetDeltaTime()