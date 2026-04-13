#include "Battle.h"
#include "Player.h"

// 커서 이동 함수
void gotoxy(int row, int col) {
    printf("\x1b[%d;%dH", row, col);
}

// 전투 화면 UI 출력 함수
void drawBattle(Player& player, Enemy& enemy) {
    cout << CLEAR;

    // 좌상단: 플레이어 상태창
    gotoxy(1, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    gotoxy(2, 1); cout << "┃ 슬라임                   ┃";
    gotoxy(3, 1); cout << "┃ HP: ";
    drawHpBar(player.hp, player.maxHp);
    gotoxy(4, 1); cout << "┃ MP: ";
    drawHpBar(player.mp, player.maxMp);
    gotoxy(5, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛";


    // 슬롯 현황 출력 추가 필요

    // 우상단: 몬스터 상태창
    gotoxy(1, 50); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    gotoxy(2, 50); cout << "┃ <" << enemy.grade <<" 정령" << "> " << "             ┃";
    gotoxy(3, 50); cout << "┃ " << enemy.name;
    gotoxy(4, 50); cout << "┃HP: ";
    drawHpBar(enemy.hp, enemy.maxHp);
    gotoxy(5, 50); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    // 적 행동 예고 출력 추가 필요
    // gotoxy(5, 50); cout << "| 다음 행동: [공격 - 15] |";

    // 중앙 좌측: 슬라임 아스키 아트
    // slot[0] 원소에 따라 색상 변경됨 (Player.cpp 참고)
    drawSlime(player);

    // 중앙 우측: 몬스터 아스키 아트
    for (int i = 0; i < (int)enemy.ascii.size(); i++) {
        gotoxy(7 + i, 55);
        if (enemy.element == "fire") {
            cout <<RED << enemy.ascii[i] << CLR;

        }
        else if (enemy.element == "water") {
            cout <<BLUE<< enemy.ascii[i] << CLR;

        }
        else if (enemy.element == "earth") {
            cout <<YELLOW<< enemy.ascii[i] << CLR;

        }
        else if (enemy.element == "wind") {
            cout << GREEN<< enemy.ascii[i] << CLR;

        }
    }

    // 전투 로그 출력 영역 추가 필요

    // 좌하단: 스킬 메뉴
    gotoxy(15, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    gotoxy(16, 1); cout << "┃  1: 원소 공격            ┃";
    gotoxy(17, 1); cout << "┃  2: 조합 공격            ┃";
    gotoxy(18, 1); cout << "┃  3: 방어 하기            ┃";
    gotoxy(19, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    // 원소 공격 선택 시 세부 메뉴 구현 필요
    // 슬롯에 장착된 원소에 따라 기술 3종 표시
    // 조합 공격 선택 시 세부 메뉴 구현 필요
    // 슬롯의 원소 2개 선택 → 조합 기술 발동
    // 조합 목록: 불+물=증기 / 불+흙=용암 / 불+바람=화염
    //            물+흙=진흙 / 물+바람=빙결 / 흙+바람=모래

    // 하단: 입력
    gotoxy(21, 1); cout << "【  1: 원소 공격   2: 조합 공격   3: 방어 하기  】";
    gotoxy(22, 1); cout << "선택 ⇒ ";
    cin.get();
}

