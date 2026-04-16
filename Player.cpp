#include "Player.h"
#include "Define.h"
#include <iostream>

void drawSlime(Player& player) {
    const char* color = CLR;

    if (player.slots[0].find("불") != string::npos)
        color = RED;
    else if (player.slots[0].find("물") != string::npos)
        color = CYAN;
    else if (player.slots[0].find("흙") != string::npos)
        color = YELLOW;
    else if (player.slots[0].find("바람") != string::npos)
        color = GREEN;

    gotoxy(10, 5); cout << color << "  .--." << CLR;
    gotoxy(11, 5); cout << color << " ( .. )" << CLR;
    gotoxy(12, 5); cout << color << "  '--'" << CLR;

}