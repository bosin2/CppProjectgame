#include "MouseInput.h"
#include <windows.h>

void flushInput() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

COORD getMouseEvent(bool& isClick) {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD ir;
    DWORD cNumRead;

    while (true) {
        ReadConsoleInput(hStdin, &ir, 1, &cNumRead);
        if (ir.EventType == MOUSE_EVENT) {
            // 마우스 이동
            if (ir.Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
                isClick = false;
                return ir.Event.MouseEvent.dwMousePosition;
            }
            // 왼쪽 버튼 클릭
            if (ir.Event.MouseEvent.dwEventFlags == 0 &&
                ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                isClick = true;
                return ir.Event.MouseEvent.dwMousePosition;
            }
        }
    }
}

COORD getMouseClick() {
    bool isClick;
    while (true) {
        COORD pos = getMouseEvent(isClick);
        if (isClick) return pos;
    }
}

void waitAnyClick() {
    flushInput(); // 이전에 쌓인 클릭 버퍼 제거
    getMouseClick();
}
