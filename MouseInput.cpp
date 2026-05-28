#include "MouseInput.h"
#include <windows.h>

COORD getMouseClick() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD ir;
    DWORD cNumRead;

    while (true) {
        ReadConsoleInput(hStdin, &ir, 1, &cNumRead);
        if (ir.EventType == MOUSE_EVENT) {
            if (ir.Event.MouseEvent.dwEventFlags == 0 &&
                ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                return ir.Event.MouseEvent.dwMousePosition;
            }
        }
    }
}

void waitAnyClick() {
    getMouseClick();
}