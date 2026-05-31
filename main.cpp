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
        flushInput();

        int choice = 0;
        int hovTitle = 0;
        while (choice == 0) {
            bool isClick;
            COORD pos = getMouseEvent(isClick);

            int newHov = 0;
            if (pos.Y == 24) newHov = 1;
            else if (pos.Y == 26) newHov = 2;

            if (newHov != hovTitle) {
                hovTitle = newHov;
                // 시작하기 버튼 (Y=24 → gotoxy row 25)
                gotoxy(25, 11);
                if (hovTitle == 1) cout << "║" << REVERSE << "   ▶  시작하기     " << CLR << "║";
                else               cout << "║" << BOLD   << "   ▶  시작하기     " << CLR << "║";
                // 설명보기 버튼 (Y=26 → gotoxy row 27)
                gotoxy(27, 11);
                if (hovTitle == 2) cout << "║" << REVERSE << "   ▶  설명보기     " << CLR << "║";
                else               cout << "║" << BOLD   << "   ▶  설명보기     " << CLR << "║";
            }

            if (isClick) {
                if (pos.Y == 24) choice = 1;
                else if (pos.Y == 26) choice = 2;
            }
        }

        if (choice == 1) { showPrologue(); break; }
        else if (choice == 2) { showExplan(); }
    }

    // ── 원소 박스 호버 라인 업데이트 (startChoice 이후 gotoxy로 덮어쓰기) ──
    auto drawElemHover = [&](int hovered) {
        // 박스 3줄 전체를 다시 그림 (Y=18~20 → gotoxy row 19~21)
        auto box = [&](int idx, const char* bg, const string& label) {
            if (hovered == idx) cout << WB << BLACK << BOLD << label << CLR;
            else                cout << bg  << BOLD           << label << CLR;
        };

        // 윗 테두리
        gotoxy(19, 1);
        cout << "  ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐";

        // 내용
        gotoxy(20, 1);
        cout << "  │"; box(1, RB, "  불  ██  ");
        cout << "│    │"; box(2, BB, "  물  ██  ");
        cout << "│    │"; box(3, YB, "  흙  ██  ");
        cout << "│    │"; box(4, GB, "  바람 ██ ");
        cout << "│";

        // 아랫 테두리
        gotoxy(21, 1);
        cout << "  └──────────┘    └──────────┘    └──────────┘    └──────────┘";
    };

    // 스타팅 원소 선택
    while (true) {
        startChoice();
        flushInput();

        int startElem = 0;
        int hovElem = 0;
        while (startElem == 0) {
            bool isClick;
            COORD pos = getMouseEvent(isClick);

            int newHov = 0;
            if (pos.Y >= 19 && pos.Y <= 21) {
                if      (pos.X >= 2  && pos.X <= 13) newHov = 1;
                else if (pos.X >= 18 && pos.X <= 29) newHov = 2;
                else if (pos.X >= 34 && pos.X <= 45) newHov = 3;
                else if (pos.X >= 50 && pos.X <= 61) newHov = 4;
            }
            if (newHov != hovElem) { hovElem = newHov; drawElemHover(hovElem); }

            if (isClick && pos.Y >= 19 && pos.Y <= 21) {
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
    flushInput();
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
        cout << "\n  방향 선택:  " << BOLD << "1(↗)" << CLR << "  " << BOLD << "2(→)" << CLR << "  " << BOLD << "3(↘)" << CLR << "  ← 마우스를 올려 미리 보기\n";
        flushInput();

        char input = '0';
        int hovDir = 0;
        while (input == '0') {
            bool isClick;
            COORD pos = getMouseEvent(isClick);
            int playerBaseY = 1 + player.row * 2;

            int newDir = 2;
            if (pos.Y < playerBaseY - 1) newDir = 1;
            else if (pos.Y > playerBaseY + 1) newDir = 3;

            if (newDir != hovDir) {
                hovDir = newDir;
                // 맵 아래쪽 힌트 라인 업데이트 (ROWS*2+2 = row 13 근처)
                gotoxy(ROWS * 2 + 7, 1);
                cout << "  방향 선택:  "
                     << (hovDir == 1 ? REVERSE : BOLD) << "1(↗)" << CLR << "  "
                     << (hovDir == 2 ? REVERSE : BOLD) << "2(→)" << CLR << "  "
                     << (hovDir == 3 ? REVERSE : BOLD) << "3(↘)" << CLR << "  ← 마우스를 올려 미리 보기";
            }

            if (isClick) {
                if (pos.Y < playerBaseY - 1) input = '1';
                else if (pos.Y > playerBaseY + 1) input = '3';
                else input = '2';
            }
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

            // ── 고정 레이아웃으로 전리품 화면 출력 ──
            // Y=1~3: 헤더박스, Y=5: 획득 원소, Y=7~8: 메시지
            // Y=10~13: 슬롯박스(가득일 때), Y=15: 결과, Y=16: 보너스, Y=18: 클릭 안내
            gotoxy(2, 1);
            cout << BOLD << CYAN << "  ╔═══════════════════╗" << CLR;
            gotoxy(3, 1);
            cout << BOLD << CYAN << "  ║  ★ 전리품 획득 ★  ║" << CLR;
            gotoxy(4, 1);
            cout << BOLD << CYAN << "  ╚═══════════════════╝" << CLR;
            gotoxy(6, 1);
            cout << "  획득한 원소 : " << BOLD << elemColor(dropElem) << "【 " << dropElem << " 】" << CLR;

            // ── 메시지 영역 (Y=7~8 고정) ──
            for (int i = 0; i < 3; i++) {
                if (player.slots[i] == plus2) {
                    gotoxy(8, 1);
                    cout << BOLD << YELLOW << "  !! [" << plus2 << "] 속성은 이미 최고 단계입니다." << CLR;
                    gotoxy(9, 1);
                    cout << "     [" << dropElem << "] 원소가 흩어집니다...";
                    handled = true; break;
                }
            }

            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == plus1) {
                        player.slots[i] = plus2;
                        gotoxy(8, 1);
                        cout << BOLD << elemColor(dropElem) << "  == FINAL EVOLUTION! ==" << CLR;
                        gotoxy(9, 1);
                        cout << "  " << BOLD << "[ " << plus1 << " ]" << CLR << "  ->  " << BOLD << elemColor(dropElem) << "[ " << plus2 << " ]" << CLR << "  최종 진화!";
                        handled = true; isAbsorbed = true; break;
                    }
                }
            }

            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == dropElem) {
                        player.slots[i] = plus1;
                        gotoxy(8, 1);
                        cout << BOLD << elemColor(dropElem) << "  >> EVOLUTION!" << CLR;
                        gotoxy(9, 1);
                        cout << "  " << BOLD << "[ " << dropElem << " ]" << CLR << "  ->  " << BOLD << elemColor(dropElem) << "[ " << plus1 << " ]" << CLR << "  진화!";
                        handled = true; isAbsorbed = true; break;
                    }
                }
            }

            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == "") {
                        player.slots[i] = dropElem;
                        gotoxy(8, 1);
                        cout << BOLD << GREEN << "  ★ 새 원소 획득!" << CLR;
                        gotoxy(9, 1);
                        cout << "  " << BOLD << elemColor(dropElem) << "[ " << dropElem << " ]" << CLR << " 원소를 슬롯에 장착!";
                        handled = true; isAbsorbed = true; break;
                    }
                }
            }

            // ── 슬롯 가득: 박스 고정 위치 (Y=10~13) ──
            if (!handled) {
                gotoxy(8, 1);
                cout << BOLD << YELLOW << "  ▶ 슬롯이 가득 찼습니다!  새 원소: "
                     << elemColor(dropElem) << "[ " << dropElem << " ]" << CLR;
                gotoxy(9, 1);
                cout << "  교체할 슬롯을 클릭하세요.  (가장 오른쪽 클릭 = 포기)";

                string s0 = player.slots[0].empty() ? "비어있음" : player.slots[0];
                string s1 = player.slots[1].empty() ? "비어있음" : player.slots[1];
                string s2 = player.slots[2].empty() ? "비어있음" : player.slots[2];

                auto dispw = [](const string& s) {
                    int w = 0;
                    for (unsigned char c : s)
                        if ((c & 0xC0) != 0x80)
                            w += (c >= 0x80) ? 2 : 1;
                    return w;
                };
                auto slotCell = [&](const string& s, const char* color, int innerW) {
                    int pad = innerW - 4 - dispw(s);
                    if (pad < 0) pad = 0;
                    return string("    ") + BOLD + color + s + CLR + string(pad, ' ');
                };

                gotoxy(11, 1);
                cout << "  ┌──────────────────┐┌──────────────────┐┌──────────────────┐┌─────────────┐";
                gotoxy(12, 1);
                cout << "  │   [ 슬롯  1 ]    ││   [ 슬롯  2 ]    ││   [ 슬롯  3 ]    ││  [ 포기 ]   │";
                gotoxy(13, 1);
                cout << "  │" << slotCell(s0, elemColor(s0), 18)
                     << "││" << slotCell(s1, elemColor(s1), 18)
                     << "││" << slotCell(s2, elemColor(s2), 18)
                     << "││    포기     │";
                gotoxy(14, 1);
                cout << "  └──────────────────┘└──────────────────┘└──────────────────┘└─────────────┘";

                int replaceIdx = -1;
                int hovSlot = 0;
                flushInput();
                while (replaceIdx == -1) {
                    bool isClick;
                    COORD pos = getMouseEvent(isClick);

                    int newHov = 0;
                    if      (pos.X <= 21) newHov = 1;
                    else if (pos.X <= 41) newHov = 2;
                    else if (pos.X <= 61) newHov = 3;
                    else                  newHov = 4;

                    if (newHov != hovSlot) {
                        hovSlot = newHov;
                        // 헤더 행(Y=11 → gotoxy 12) 호버 강조
                        gotoxy(12, 1);
                        cout << "  │"
                             << (hovSlot == 1 ? REVERSE : "") << "   [ 슬롯  1 ]    " << CLR
                             << "││"
                             << (hovSlot == 2 ? REVERSE : "") << "   [ 슬롯  2 ]    " << CLR
                             << "││"
                             << (hovSlot == 3 ? REVERSE : "") << "   [ 슬롯  3 ]    " << CLR
                             << "││"
                             << (hovSlot == 4 ? REVERSE : "") << "  [ 포기 ]   " << CLR
                             << "│";
                    }

                    if (isClick) {
                        if      (pos.X <= 21) replaceIdx = 1;
                        else if (pos.X <= 41) replaceIdx = 2;
                        else if (pos.X <= 61) replaceIdx = 3;
                        else                  replaceIdx = 0;
                    }
                }

                // 결과 메시지 고정 위치 (Y=15)
                gotoxy(16, 1);
                if (replaceIdx >= 1 && replaceIdx <= 3) {
                    cout << "  " << BOLD << RED << "[ " << player.slots[replaceIdx - 1] << " ]" << CLR
                         << " 버리고 " << BOLD << elemColor(dropElem) << "[ " << dropElem << " ]" << CLR << " 장착!";
                    player.slots[replaceIdx - 1] = dropElem;
                    isAbsorbed = true;
                }
                else {
                    cout << "  " << BOLD << "원소 획득 포기..." << CLR;
                }
            }

            if (isAbsorbed) {
                gotoxy(17, 1);
                cout << "  " << BOLD << PINK << player.applyElementBonus(dropElem) << CLR;
            }
            gotoxy(19, 1);
            cout << "  아무 곳이나 클릭하면 맵으로 돌아갑니다...";
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
            cout << "\n 탑의 끝에 도달했다. 보스가 나타났다!! (아무 곳이나 클릭하세요)\n";
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