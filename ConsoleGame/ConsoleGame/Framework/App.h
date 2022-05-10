#pragma once

#include "Type.h"

/// <summary>
/// 애플리케이션을 초기화한다.
/// </summary>
/// <returns>초기화에 성공했으면 true, 아니면 false</returns>
bool	App_Init();

/// <summary>
/// 애플리케이션을 실행한다.
/// </summary>
/// <returns>앱이 정상적으로 종료됐다면 0, 아니라면 0이 아닌값</returns>
int32	App_Run();