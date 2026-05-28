#include <windows.h>
#include <iostream>
#include <string>
#include "MouseInput.h" 
#include "Player.h"
#include "Map.h"
#include "Battle.h"
#include "Boss.h"
#include "Scene.h"

using namespace std;

string translateElement(string engElem) {
    if (engElem == "fire")  return "불";
    if (engElem == "water") return "물";
    if (engElem == "earth") return "흙";
    if (engElem == "wind")  return "바람";
    return "무";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE; 
    mode |= ENABLE_MOUSE_INPUT;
    SetConsoleMode(hStdin, mode);

    Player player;
    initMap();

    while (true) {
        showTitle();
        
        int choice = 0;
        while (choice == 0) {
            COORD pos = getMouseClick();
            if (pos.Y == 24 && (pos.X >= 15 && pos.X <= 35)) {
                choice = 1;
            }
            else if (pos.Y == 25 && (pos.X >= 15 && pos.X <= 35)) {
                choice = 2;
            }
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
            if (pos.Y >= 15 && pos.Y <= 18) {
                if (pos.X >= 2 && pos.X <= 12) startElem = 1;
                else if (pos.X >= 15 && pos.X <= 25) startElem = 2;
                else if (pos.X >= 28 && pos.X <= 38) startElem = 3;
                else if (pos.X >= 41 && pos.X <= 53) startElem = 4;
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

    while (true) {
        drawMap(player);

        cout << "\n  화면의 맵 타일이나 이동 방향을 클릭하여 이동하세요.\n";
        
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

        // 몬스터 타일
        if (gameMap[player.row][player.col] == MONSTER) {
            int elem = elementMap[player.row][player.col];
            int count = player.fightCount[elem];

            string grade;
            if (count == 0)
                grade = "하급";
            else if (count <= 1)
                grade = (rand() % 2 == 0) ? "하급" : "중급";
            else if (count <= 2)
                grade = (rand() % 2 == 0) ? "중급" : "상급";
            else
                grade = "상급";

            player.fightCount[elem]++;

            string elemName;
            switch (elem) {
            case FIRE:  elemName = "fire";  break;
            case WATER: elemName = "water"; break;
            case EARTH: elemName = "earth"; break;
            case WIND:  elemName = "wind";  break;
            }
            Enemy enemy = loadEnemy(elemName, grade);
            bool isVictory = startBattle(player, enemy);
            if (!isVictory) break;

            // 전리품 획득
            string dropElem = translateElement(enemy.element);
            string plus1 = dropElem + "+";
            string plus2 = dropElem + "++";
            bool handled = false;

            cout << CLEAR;
            cout << "========= [ 전리품 획득 ] =========\n\n";

            // 1. 이미 ++ 있으면 더 이상 진화 불가
            for (int i = 0; i < 3; i++) {
                if (player.slots[i] == plus2) {
                    cout << ">> [" << plus2 << "] 속성은 더 이상 진화할 수 없습니다.\n";
                    cout << ">> [" << dropElem << "] 원소가 흩어집니다...\n";
                    handled = true;
                    break;
                }
            }

            // 2. + → ++ 업그레이드
            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == plus1) {
                        player.slots[i] = plus2;
                        cout << ">> [" << plus1 << "] 속성이 [" << plus2 << "] 로 최종 진화!\n";
                        handled = true;
                        break;
                    }
                }
            }

            // 3. 기본 → + 업그레이드
            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == dropElem) {
                        player.slots[i] = plus1;
                        cout << ">> [" << dropElem << "] 속성이 [" << plus1 << "] 로 진화!\n";
                        handled = true;
                        break;
                    }
                }
            }

            // 4. 빈 슬롯 채우기
            if (!handled) {
                for (int i = 0; i < 3; i++) {
                    if (player.slots[i] == "") {
                        player.slots[i] = dropElem;
                        cout << ">> [" << dropElem << "] 원소를 새로 획득!\n";
                        handled = true;
                        break;
                    }
                }
            }

            // 5. 슬롯 꽉 찼을 때 교체
            if (!handled) {
                cout << ">> 슬롯이 가득 찼습니다! 새 원소: [" << dropElem << "]\n\n";
                cout << "1: " << player.slots[0] << "  2: " << player.slots[1] << "  3: " << player.slots[2] << "\n";
                cout << "교체할 슬롯 클릭 (화면 우측 클릭 시 원소 포기) >> ";
                
                int replaceIdx = -1;
                while (replaceIdx == -1) {
                    COORD pos = getMouseClick();
                    if (pos.X < 20) replaceIdx = 1;
                    else if (pos.X < 40) replaceIdx = 2;
                    else if (pos.X < 60) replaceIdx = 3;
                    else replaceIdx = 0;
                }
                
                if (replaceIdx >= 1 && replaceIdx <= 3) {
                    cout << "\n[" << player.slots[replaceIdx - 1] << "] 버리고 [" << dropElem << "] 장착\n";
                    player.slots[replaceIdx - 1] = dropElem;
                }
                else {
                    cout << "\n원소 획득 포기\n";
                }
            }

            cout << "\n아무 곳이나 클릭하면 맵으로 돌아갑니다...";
            waitAnyClick();
        }

        // 우물 타일
        if (gameMap[player.row][player.col] == WELL) {
            player.hp = min(player.hp + 30, player.maxHp);
            player.mp = min(player.mp + 20, player.maxMp);
            waitAnyClick();
            showHeal(player);
        }

        // 보스 타일
        if (gameMap[player.row][player.col] == BOSS) {
            drawMap(player);
            cout << "\n  [시스템] 보스 등장!! (아무 곳이나 클릭하세요)\n";
            waitAnyClick();

            Boss boss = loadBoss();
            bool bossVictory = startBossBattle(player, boss);

            if (!bossVictory)
                break; //보스 클리어 못할시 빠져나오기 showDie로   
            break;//클리어시 엔딩 
        }
    }

    // 엔딩
    cout << CLEAR;
    if (player.hp > 0) {
        waitAnyClick();
        showEnd(player);
    }
    else {
        waitAnyClick();
        showDie();
    }

    cout << "\n아무 곳이나 클릭하면 게임을 종료합니다...";
    waitAnyClick();
    return 0;
}