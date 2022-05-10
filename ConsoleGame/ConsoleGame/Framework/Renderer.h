#pragma once

#include "Type.h"

/// <summary>
/// 렌더러를 초기화한다.
/// </summary>
/// <param name=""></param>
/// <returns>초기화 성공하면 true, 아니면 false</returns>
bool   Renderer_Init(void);

/// <summary>
/// 렌더러를 정리한다.
/// </summary>
void   Renderer_Cleanup(void);

/// <summary>
/// 화면을 바꾼다.
/// </summary>
void   Renderer_Flip(void);

/// <summary>
/// 텍스트를 출력한다.
/// </summary>
void Renderer_DrawText(const char* text);