#include "Player.h"
#include "Define.h"
#include "Battle.h" // gotoxy 사용 위함

// 전투 화면에서 슬라임 아스키 아트를 출력하는 함수
// slot[0]에 장착된 원소에 따라 슬라임 색상이 달라짐
// 원소 시스템 등록 후에 사용
void drawSlime(Player& player) {
    const char* color;
    switch (player.slot[0]) {
    case FIRE:  color = RED;    break;
    case WATER: color = CYAN;   break;
    case EARTH: color = YELLOW; break;
    case WIND:  color = GREEN;  break;
    default:    color = CLR;    break;
    }

    gotoxy(8,  5); cout << color << "  .--."  << CLR;
    gotoxy(9,  5); cout << color << " ( .. )" << CLR;
    gotoxy(10, 5); cout << color << "  '--'"  << CLR;

}
