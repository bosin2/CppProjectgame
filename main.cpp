#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime> // ★ 난수 생성을 위해 추가된 헤더
#include "MouseInput.h" 
#include "Player.h"
#include "Map.h"
#include "Battle.h"
#include "Boss.h"
#include "Scene.h"

using namespace std;

// 원소 이름 변환
string translateElement(string engElem) {
    if (engElem == "fire")  return "불";
    if (engElem == "water") return "물";
    if (engElem == "earth") return "흙";
    if (engElem == "wind")  return "바람";
    return "무";
}

int main() {
    // ★ 랜덤 시드 초기화 (단 한 번만 실행되어야 함)
    srand((unsigned int)time(NULL));

    // 콘솔 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hStdin, &mode)) {
        mode |= ENABLE_EXTENDED_FLAGS;
        mode &= ~ENABLE_QUICK_EDIT_MODE;
        mode |= ENABLE_MOUSE_INPUT;
        SetConsoleMode(hStdin, mode);
    }

    Player player;
    initMap();

    // 타이틀 화면
    while (true) {
        showTitle();
        
        int choice = 0;
        while (choice == 0) {
            COORD pos = getMouseClick();
            if (pos.Y == 24) choice = 1;  
            else if (pos.Y == 26) choice = 2;  
        }

        if (choice == 1) { showPrologue(); break; }
        else if (choice == 2) { showExplan(); }
    }

    // 스타팅 원소 선택
    while (true) {
        startChoice();
        
        int startElem = 0;
        while (startElem == 0) {
            COORD pos = getMouseClick();
            if (pos.Y >= 19 && pos.Y <= 21) {
                if      (pos.X >= 2  && pos.X <= 13) startElem = 1;  
                else if (pos.X >= 18 && pos.X <= 29) startElem = 2;  
                else if (pos.X >= 34 && pos.X <= 45) startElem = 3;  
                else if (pos.X >= 50 && pos.X <= 61) startElem = 4;  
            }
        }

        switch (startElem) {
        case 1: player.slots[0] = "불";   break;
        case 2: player.slots[0] = "물";   break;
        case 3: player.slots[0] = "흙";   break;
        case 4: player.slots[0] = "바람"; break;
        default: continue;
        }
        break;
    }

    player.applyElementBonus(player.slots[0]);

    drawStartMap();
    char startInput = '0';
    while (startInput == '0') {
        COORD pos = getMouseClick();
        int clickedRow = pos.Y / 2; 
        if (clickedRow >= 0 && clickedRow < ROWS) {
            startInput = '1' + clickedRow;
        }
    }

    player.row = startInput - '1';
    player.col = 0;

    // 게임 루프
    while (true) {
        drawMap(player);
        cout << "\n  화면의 맵 타일을 눌러 이동하세요.\n";
        
        char input = '0';
        while (input == '0') {
            COORD pos = getMouseClick();
            int playerBaseY = 1 + player.row * 2;
            
            if (pos.Y < playerBaseY - 1) input = '1';
            else if (pos.Y > playerBaseY + 1) input = '3';
            else input = '2';
        }

        int newRow = player.row;
        int newCol = player.col + 1;

        if (input == '1') newRow = player.row - 1;
        if (input == '2') newRow = player.row;
        if (input == '3') newRow = player.row + 1;

        if (newRow < 0 || newRow >= ROWS) {
            cout << "이동 불가!\n";
            continue;
        }

        gameMap[player.row][player.col] = VISIT;
        player.row = newRow;
        player.col = newCol;

        // ── 몬스터 타일 ──
        if (gameMap[player.row][player.col] == MONSTER) {
            int elem = elementMap[player.row][player.col];
            int count = player.fightCount[elem];

            string grade;
            if (count == 0) grade = "하급";
            else if (count <= 1) grade = (rand() % 2 == 0) ? "하급" : "중급";
            else if (count <= 2) grade = (rand() % 2 == 0) ? "중급" : "상급";
            else grade = "상급";

            player.fightCount[elem]++;

            string elemName;
            switch (elem) {
            case FIRE:  elemName = "fire";  break;
            case WATER: elemName = "water"; break;
            case EARTH: elemName = "earth"; break;
            case WIND:  elemName = "wind";  break;
            }
            
            Enemy enemy = loadEnemy(elemName, grade);
            
            // ── 맵 진행도에 따른 몬스터 강화 ──
            int diff = player.col; 
            enemy.maxHp += (diff * 15);
            enemy.hp = enemy.maxHp;
            enemy.attack += (diff * 1);
            enemy.defense += (diff / 3);

            // 몬스터 속성별 스탯 부여
            if (enemy.element == "fire") {
                enemy.bonusDamage += (diff / 2);
                enemy.defense += 2;
            } 
            else if (enemy.element == "water") {
                enemy.lifeSteal = 20; 
            } 
            else if (enemy.element == "earth") {
                enemy.maxHp += 50; 
                enemy.hp += 50;
                enemy.defense += 5; 
            } 
            else if (enemy.element == "wind") {
                enemy.evadeChance = 20; 
            }

            bool isVictory = startBattle(player, enemy);
            if (!isVictory) break;

            // 전리품 획득
            string dropElem = translateElement(enemy.element);
            string plus1 = dropElem + "+";
            string plus2 = dropElem + "++";
            bool handled = false;
            bool isAbsorbed = false; 

            cout << CLEAR;

            auto elemColor = [&](const string& e) -> const char* {
                if (e == "불")   return RED;
                if (e == "물")   return BLUE;
                if (e == "흙")   return GREEN;
                if (e == "바람") return YELLOW;
                return WHITE;
            };

            cout << BOLD << CYAN << "\n  ╔═══════════════════╗\n  ║  ★ 전리품 획득 ★  ║\n  ╚═══════════════════╝\n" << CLR << "\n";
            cout << "  획득한 원소 : " << BOLD << elemColor(dropElem) << "【 " << dropElem << " 】" << CLR << "\n\n";

            for (int i = 0; i < 3; i++) {
                if (player.slots[i] == plus2) {
                    cout << BOLD << YELLOW << "  !! [" << plus2 << "] 속성은 이미 최고 단계입니다.\n" << CLR;
                    cout << "     [" << dropElem << "] 원소가 흩어집니다...\n";
                    handled = true; break;
                }
            }

            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == plus1) {
                        player.slots[i] = plus2;
                        cout << BOLD << elemColor(dropElem) << "  == FINAL EVOLUTION! ==\n" << CLR;
                        cout << "  " << BOLD << "[ " << plus1 << " ]" << CLR << "  ->  " << BOLD << elemColor(dropElem) << "[ " << plus2 << " ]" << CLR << "  최종 진화!\n";
                        handled = true; isAbsorbed = true; break;
                    }
                }
            }

            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == dropElem) {
                        player.slots[i] = plus1;
                        cout << BOLD << elemColor(dropElem) << "  >> EVOLUTION!\n" << CLR;
                        cout << "  " << BOLD << "[ " << dropElem << " ]" << CLR << "  ->  " << BOLD << elemColor(dropElem) << "[ " << plus1 << " ]" << CLR << "  진화!\n";
                        handled = true; isAbsorbed = true; break;
                    }
                }
            }

            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == "") {
                        player.slots[i] = dropElem;
                        cout << BOLD << GREEN << "  ★ 새 원소 획득!\n" << CLR;
                        cout << "  " << BOLD << elemColor(dropElem) << "[ " << dropElem << " ]" << CLR << " 원소를 슬롯에 장착!\n";
                        handled = true; isAbsorbed = true; break;
                    }
                }
            }

            // ── 기존에 완벽하게 정렬되어 있던 슬롯 박스 UI 적용 ──
            if (!handled) {
                cout << BOLD << YELLOW << "  ▶ 슬롯이 가득 찼습니다!" << CLR;
                cout << "  새 원소: " << BOLD << elemColor(dropElem)
                     << "[ " << dropElem << " ]" << CLR << "\n\n";
                cout << "  교체할 슬롯을 클릭하세요.  (가장 오른쪽 클릭 = 포기)\n\n";

                string s0 = player.slots[0].empty() ? "비어있음" : player.slots[0];
                string s1 = player.slots[1].empty() ? "비어있음" : player.slots[1];
                string s2 = player.slots[2].empty() ? "비어있음" : player.slots[2];

                // 표시 너비 계산 (Korean=2, ASCII=1)
                auto dispw = [](const string& s) {
                    int w = 0;
                    for (unsigned char c : s)
                        if ((c & 0xC0) != 0x80)
                            w += (c >= 0x80) ? 2 : 1;
                    return w;
                };
                // 박스 내부 셀 문자열 생성 (4 spaces + 색상 + 내용 + 패딩)
                auto slotCell = [&](const string& s, const char* color, int innerW) {
                    int pad = innerW - 4 - dispw(s);
                    if (pad < 0) pad = 0;
                    return string("    ") + BOLD + color + s + CLR + string(pad, ' ');
                };

                cout << "  ┌──────────────────┐┌──────────────────┐┌──────────────────┐┌─────────────┐\n";
                cout << "  │   [ 슬롯  1 ]    ││   [ 슬롯  2 ]    ││   [ 슬롯  3 ]    ││  [ 포기 ]   │\n";
                cout << "  │" << slotCell(s0, elemColor(s0), 18)
                     << "││" << slotCell(s1, elemColor(s1), 18)
                     << "││" << slotCell(s2, elemColor(s2), 18)
                     << "││    포기     │\n";
                cout << "  └──────────────────┘└──────────────────┘└──────────────────┘└─────────────┘\n";

                int replaceIdx = -1;
                while (replaceIdx == -1) {
                    COORD pos = getMouseClick();
                    if (pos.X <= 21)       replaceIdx = 1;
                    else if (pos.X <= 41)  replaceIdx = 2;
                    else if (pos.X <= 61)  replaceIdx = 3;
                    else                   replaceIdx = 0;
                }

                cout << "\n";
                if (replaceIdx >= 1 && replaceIdx <= 3) {
                    cout << "  " << BOLD << RED << "[ " << player.slots[replaceIdx - 1] << " ]" << CLR
                         << " 버리고 " << BOLD << elemColor(dropElem) << "[ " << dropElem << " ]" << CLR << " 장착!\n";
                    player.slots[replaceIdx - 1] = dropElem;
                    isAbsorbed = true; // 아이템 흡수 플래그 활성화
                }
                else {
                    cout << "  " << BOLD << "원소 획득 포기...\n" << CLR;
                }
            }

            if (isAbsorbed) cout << "\n  " << BOLD << PINK << player.applyElementBonus(dropElem) << CLR << "\n";
            cout << "\n  아무 곳이나 클릭하면 맵으로 돌아갑니다...";
            waitAnyClick();
        }

        // 우물 타일 
        if (gameMap[player.row][player.col] == WELL) {
            player.hp = min(player.hp + 60, player.maxHp);
            player.mp = min(player.mp + 30, player.maxMp);
            showHeal(player);
        }

        // 보스 타일
        if (gameMap[player.row][player.col] == BOSS) {
            drawMap(player);
            cout << "\n  [시스템] 보스 등장!! (아무 곳이나 클릭하세요)\n";
            waitAnyClick();

            Boss boss = loadBoss();
            boss.lifeSteal = 0; boss.evadeChance = 0; boss.bonusDamage = 0; boss.defense = 5;
            
            bool bossVictory = startBossBattle(player, boss);
            if (!bossVictory) break; 
            break; 
        }
    }

    cout << CLEAR;
    if (player.hp > 0) { waitAnyClick(); showEnd(player); }
    else { waitAnyClick(); showDie(); }

    cout << "\n아무 곳이나 클릭하면 게임을 종료합니다...";
    waitAnyClick();
    return 0;
}