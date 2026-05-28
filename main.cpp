#include <windows.h>
#include "Map.h"
#include "Battle.h"
#include "Boss.h"
#include "Scene.h"
#include "Player.h"
#include <iostream>
#include <string>

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

    Player player;
    initMap();
    while (true) {
        showTitle();
        int choice;
        cin >> choice;

        if (choice == 1) { showPrologue(); break; }
        else if (choice == 2) { showExplan(); }
    }

    // 스타팅 원소 선택
    while (true) {
        startChoice();
        int startElem;
        cin >> startElem;

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
    char startInput;
    cin >> startInput;
    player.row = startInput - '1';
    player.col = 0;
    while (true) {
        drawMap(player);

        cout << "\n  입력: ";
        char input;
        cin >> input;

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
                cout << "교체할 슬롯 선택 (1~3, 0=포기) >> ";
                int replaceIdx;
                cin >> replaceIdx;
                if (replaceIdx >= 1 && replaceIdx <= 3) {
                    cout << "\n[" << player.slots[replaceIdx - 1] << "] 버리고 [" << dropElem << "] 장착\n";
                    player.slots[replaceIdx - 1] = dropElem;
                }
                else {
                    cout << "\n원소 획득 포기\n";
                }
            }

            cout << "\n엔터를 누르면 맵으로 돌아갑니다...";
            cin.get();
        }

        // 우물 타일
        if (gameMap[player.row][player.col] == WELL) {
            player.hp = min(player.hp + 30, player.maxHp);
            player.mp = min(player.mp + 20, player.maxMp);
            cin.get();
            showHeal(player);
        }

        // 보스 타일
        if (gameMap[player.row][player.col] == BOSS) {
            drawMap(player);
            cout << "\n  [시스템] 보스 등장!!\n";
            cin.get();

            Boss boss = loadBoss();
            bool bossVictory = startBossBattle(player, boss);

            if (!bossVictory)
                break; // 패배 → showDie
            break; // 클리어 → showEnd
        }
    }

    // 엔딩
    cout << CLEAR;
    if (player.hp > 0) {
        cin.get();
        showEnd(player);
    }
    else {
        cin.get();
        showDie();
    }

    cout << "\n엔터를 누르면 게임을 종료합니다...";
    cin.get();
    return 0;
}