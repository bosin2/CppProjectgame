#include "Player.h"
#include "Define.h"
#include <iostream>

void drawSlime(Player& player) {
    const char* color = WHITE;
    gotoxy(10, 5); cout << color << "  .--." << CLR;
    gotoxy(11, 5); cout << color << " ( .. )" << CLR;
    gotoxy(12, 5); cout << color << "  '--'" << CLR;
}
