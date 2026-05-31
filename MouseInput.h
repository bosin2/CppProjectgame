#pragma once
#include <windows.h>

// 입력 버퍼 비우기 (선입력 방지)
void flushInput();

// 마우스 이동(isClick=false) 또는 클릭(isClick=true) 이벤트 반환
COORD getMouseEvent(bool& isClick);

// 클릭만 기다림
COORD getMouseClick();

// 아무 클릭 대기 (버퍼 플러시 포함)
void waitAnyClick();