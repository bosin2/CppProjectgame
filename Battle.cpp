#include "Battle.h"
#include "BattleAnim.h"
#include "Boss.h"
#include "Player.h"
#include "MouseInput.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <stdexcept>
#include <windows.h>
using namespace std;

// 전방 선언
bool hasElement(Player& p, string name);

// ── 액션 메뉴(mode 0) 호버 라인 업데이트 ──
void updateActionHover(int hovered) {
    gotoxy(20, 1);
    if (hovered == 1) cout << "┃  " << REVERSE << "▶  원소 공격   (MP:  2)          " << CLR << "┃";
    else              cout << "┃  " << YELLOW << BOLD << "▶  원소 공격   (MP:  2)" << CLR << "          ┃";

    gotoxy(21, 1);
    if (hovered == 2) cout << "┃  " << REVERSE << "▶  조합 공격   (MP:  5)          " << CLR << "┃";
    else              cout << "┃  " << CYAN << BOLD << "▶  조합 공격   (MP:  5)" << CLR << "          ┃";

    gotoxy(22, 1);
    if (hovered == 3) cout << "┃  " << REVERSE << "▶  방어 하기   (MP +10)          " << CLR << "┃";
    else              cout << "┃  " << GREEN << BOLD << "▶  방어 하기   (MP +10)" << CLR << "          ┃";
}

// ── 원소 공격 슬롯(mode 1) 호버 라인 업데이트 ──
void updateSlotHover(Player& player, int hovered) {
    string slots[3];
    for (int i = 0; i < 3; i++)
        slots[i] = player.slots[i].empty() ? "비어있음" : player.slots[i];

    for (int i = 0; i < 3; i++) {
        gotoxy(21 + i, 1);
        // 라인 전체를 공백으로 초기화 후 내용 덮어쓰기
        cout << "                                                                    ";
        gotoxy(21 + i, 1);
        if (hovered == i + 1)
            cout << "┃  " << REVERSE << " ▶ [" << (i+1) << "] " << slots[i] << " " << CLR;
        else
            cout << "┃  " << YELLOW << "▶ [" << (i+1) << "] " << CLR << slots[i];
    }
}

// ── 조합 공격(mode 2) 호버 라인 업데이트 ──
void updateComboHover(Player& player, int hovered) {
    struct ComboEntry { int idx; const char* label; string e1; string e2; };
    ComboEntry combos[] = {
        {1, "증기 폭발 (불+물)  ", "불", "물"},
        {2, "용암 투척 (불+흙)  ", "불", "흙"},
        {3, "화염 돌풍 (불+바람)", "불", "바람"},
        {4, "진흙 늪   (물+흙)  ", "물", "흙"},
        {5, "빙결 화살 (물+바람)", "물", "바람"},
        {6, "모래 폭풍 (흙+바람)", "흙", "바람"},
    };
    for (auto& c : combos) {
        gotoxy(20 + c.idx, 1);
        bool avail = hasElement(player, c.e1) && hasElement(player, c.e2);
        if (hovered == c.idx && avail)
            cout << "┃  " << REVERSE << "▶ [" << c.idx << "] " << c.label << "          " << CLR;
        else if (avail)
            cout << "┃  " << CYAN << "▶ [" << c.idx << "] " << c.label << CLR << "          ";
        else
            cout << "┃  " << WHITE << "  [" << c.idx << "] [잠금]          " << CLR;
    }
}

void enemyDie(Player& player, Enemy& enemy, string& msg) {
    animEnemyDie(enemy);
    enemy.ascii.clear();
    msg = enemy.name + "이(가) 쓰러졌습니다.";
    drawBattle(player, enemy, msg);
    drawSkillMenu(player, 0);
    gotoxy(29, 1);
    cout << "전투 승리! (아무 곳이나 클릭하면 계속...)"; 
}

bool hasElement(Player& p, string name) {
    for (int i = 0; i < 3; i++)
        if (p.slots[i].find(name) != string::npos) return true;
    return false;
}

int getElementDamage(Player& p, string baseElem) {
    int maxDmg = 0;
    for (int i = 0; i < 3; i++) {
        if (p.slots[i].find(baseElem) != string::npos) {
            int d = 20; 
            if (p.slots[i].find("++") != string::npos) d = 40;
            else if (p.slots[i].find("+") != string::npos) d = 30;
            if (d > maxDmg) maxDmg = d; 
        }
    }
    return maxDmg;
}

float elemCounter(string pSlot, string eElem) {
    bool pFire = (pSlot.find("불") != string::npos);
    bool pWater = (pSlot.find("물") != string::npos);
    bool pEarth = (pSlot.find("흙") != string::npos);
    bool pWind = (pSlot.find("바람") != string::npos);

    if ((pFire && eElem == "earth") || (pEarth && eElem == "wind") ||
        (pWind && eElem == "water") || (pWater && eElem == "fire")) return 2.0f;
    if ((pEarth && eElem == "fire") || (pWind && eElem == "earth") ||
        (pWater && eElem == "wind") || (pFire && eElem == "water")) return 0.5f;
    return 1.0f;
}

// ── UI (절대 좌표 강제 고정으로 깨짐 완벽 방지) ──
void drawBattle(Player& player, Enemy& enemy, string currentMessage) {
    cout << CLEAR;
    
    int pCol = 2;  
    int eCol = 50; 
    string clearLine = "                                                "; 

    // ─── [플레이어 박스] ───
    gotoxy(1, pCol); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓"; 
    
    gotoxy(2, pCol); cout << "┃" << clearLine; 
    gotoxy(2, pCol); cout << "┃ 슬라임"; 
    gotoxy(2, pCol + 43); cout << "┃"; 
    
    gotoxy(3, pCol); cout << "┃" << clearLine; 
    gotoxy(3, pCol); cout << "┃ HP: "; drawHpBar(player.hp, player.maxHp); 
    gotoxy(3, pCol + 43); cout << "┃";
    
    gotoxy(4, pCol); cout << "┃" << clearLine; 
    gotoxy(4, pCol); cout << "┃ MP: "; drawHpBar(player.mp, player.maxMp); 
    gotoxy(4, pCol + 43); cout << "┃";
    
    gotoxy(5, pCol); cout << "┃" << clearLine; 
    gotoxy(5, pCol); cout << "┃ 슬롯: [" << (player.slots[0]==""?"-":player.slots[0]) << "] [" 
                                  << (player.slots[1]==""?"-":player.slots[1]) << "] [" 
                                  << (player.slots[2]==""?"-":player.slots[2]) << "]"; 
    gotoxy(5, pCol + 43); cout << "┃";
    
    gotoxy(6, pCol); cout << "┃" << clearLine; 
    gotoxy(6, pCol); cout << "┃ 방어:" << player.defense << " 딜:" << player.bonusDamage 
                       << " 흡:" << player.lifeSteal << "% 회피:" << player.evadeChance << "%"; 
    gotoxy(6, pCol + 43); cout << "┃";
    
    gotoxy(7, pCol); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    // ─── [몬스터 박스] ───
    gotoxy(1, eCol); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    
    gotoxy(2, eCol); cout << "┃" << clearLine; 
    gotoxy(2, eCol); cout << "┃ <" << enemy.grade << " 정령> " << enemy.name; 
    gotoxy(2, eCol + 43); cout << "┃";
    
    gotoxy(3, eCol); cout << "┃" << clearLine; 
    gotoxy(3, eCol); cout << "┃ HP: "; drawHpBar(enemy.hp, enemy.maxHp); 
    gotoxy(3, eCol + 43); cout << "┃";
    
    gotoxy(4, eCol); cout << "┃" << clearLine; 
    gotoxy(4, eCol); cout << "┃ 방어:" << enemy.defense << " 딜:" << enemy.bonusDamage 
                       << " 흡:" << enemy.lifeSteal << "% 회피:" << enemy.evadeChance << "%"; 
    gotoxy(4, eCol + 43); cout << "┃";
    
    gotoxy(5, eCol); cout << "┃" << clearLine; 
    gotoxy(5, eCol); cout << "┃ ";
    if (!enemy.patterns.empty()) {
        Pattern& nextPat = enemy.patterns[enemy.nextPattern];
        cout << nextPat.name << " 준비 중 (" << nextPat.damage + enemy.bonusDamage << ")";
    }
    gotoxy(5, eCol + 43); cout << "┃";
    
    gotoxy(6, eCol); cout << "┃" << clearLine; 
    gotoxy(6, eCol + 43); cout << "┃";
    
    gotoxy(7, eCol); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    drawSlime(player);

    for (int i = 0; i < (int)enemy.ascii.size(); i++) {
        gotoxy(8 + i, 55);
        if (enemy.element == "fire")  cout << RED << enemy.ascii[i] << CLR;
        else if (enemy.element == "water") cout << BLUE << enemy.ascii[i] << CLR;
        else if (enemy.element == "earth") cout << YELLOW << enemy.ascii[i] << CLR;
        else if (enemy.element == "wind")  cout << GREEN << enemy.ascii[i] << CLR;
        else                               cout << PINK << enemy.ascii[i] << CLR; 
    }

    gotoxy(15, 1); cout << "──────────────────────────────────────────────────────────────────────────────────────────";
    gotoxy(16, 4); cout << ">> " << currentMessage << clearLine << clearLine; 
    gotoxy(17, 1); cout << "──────────────────────────────────────────────────────────────────────────────────────────";
}

void drawSkillMenu(Player& player, int mode) {
    for (int row = 20; row <= 29; row++) {
        gotoxy(row, 1);
        cout << "                                                                                ";
    }
    if (mode == 0) {
        gotoxy(19, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
        gotoxy(20, 1); cout << "┃  " << YELLOW << BOLD << "▶  원소 공격   (MP:  2)" << CLR << "          ┃";
        gotoxy(21, 1); cout << "┃  " << CYAN   << BOLD << "▶  조합 공격   (MP:  5)" << CLR << "          ┃"; 
        gotoxy(22, 1); cout << "┃  " << GREEN  << BOLD << "▶  방어 하기   (MP +10)  " << CLR << "        ┃";
        gotoxy(23, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
        gotoxy(25, 1); cout << "  [ 원하는 행동을 클릭하세요 ]          ";
    }
    else if (mode == 1) {
        gotoxy(19, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
        gotoxy(20, 1); cout << "┃   " << BOLD << "[ 원소 공격 목록  (MP: 2) ]" << CLR << "        ┃";
        gotoxy(21, 1); cout << "┃  " << YELLOW << "▶ [1] " << CLR << (player.slots[0] == "" ? "비어있음" : player.slots[0]) << "                                  ";
        gotoxy(22, 1); cout << "┃  " << YELLOW << "▶ [2] " << CLR << (player.slots[1] == "" ? "비어있음" : player.slots[1]) << "                                  ";
        gotoxy(23, 1); cout << "┃  " << YELLOW << "▶ [3] " << CLR << (player.slots[2] == "" ? "비어있음" : player.slots[2]) << "                                  ";
        gotoxy(24, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
        gotoxy(25, 1); cout << "  [ 슬롯 클릭 / 박스 밖 클릭 = 뒤로 ]   ";
    }
    else if (mode == 2) {
        gotoxy(19, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
        gotoxy(20, 1); cout << "┃   " << BOLD << "[ 조합 공격 목록  (MP: 5) ]" << CLR << "           ┃"; 
        gotoxy(21, 1); cout << "┃  " << (hasElement(player, "불") && hasElement(player, "물") ? string(CYAN) + "▶ [1] 증기 폭발 (불+물)  " + CLR : string(WHITE) + "  [1] [잠금]          " + CLR);
        gotoxy(22, 1); cout << "┃  " << (hasElement(player, "불") && hasElement(player, "흙") ? string(CYAN) + "▶ [2] 용암 투척 (불+흙)  " + CLR : string(WHITE) + "  [2] [잠금]          " + CLR);
        gotoxy(23, 1); cout << "┃  " << (hasElement(player, "불") && hasElement(player, "바람") ? string(CYAN) + "▶ [3] 화염 돌풍 (불+바람)" + CLR : string(WHITE) + "  [3] [잠금]          " + CLR);
        gotoxy(24, 1); cout << "┃  " << (hasElement(player, "물") && hasElement(player, "흙") ? string(CYAN) + "▶ [4] 진흙 늪   (물+흙)  " + CLR : string(WHITE) + "  [4] [잠금]          " + CLR);
        gotoxy(25, 1); cout << "┃  " << (hasElement(player, "물") && hasElement(player, "바람") ? string(CYAN) + "▶ [5] 빙결 화살 (물+바람)" + CLR : string(WHITE) + "  [5] [잠금]          " + CLR);
        gotoxy(26, 1); cout << "┃  " << (hasElement(player, "흙") && hasElement(player, "바람") ? string(CYAN) + "▶ [6] 모래 폭풍 (흙+바람)" + CLR : string(WHITE) + "  [6] [잠금]          " + CLR);
        gotoxy(27, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
        gotoxy(28, 1); cout << "  [ 번호 클릭 / 박스 밖 클릭 = 뒤로 ]     ";
    }
}

// ── 일반 전투 로직 ──
bool startBattle(Player& player, Enemy& enemy) {
    if (enemy.patterns.empty()) {
        throw runtime_error("전투를 시작할 수 없습니다: 몬스터 패턴이 비어 있습니다.");
    }
    enemy.nextPattern = rand() % enemy.patterns.size();
    string msg = enemy.name + "이(가) 나타났습니다.";

    while (player.hp > 0 && enemy.hp > 0) {
        drawBattle(player, enemy, msg);
        drawSkillMenu(player, 0);
        flushInput();

        int choice = 0;
        int hovAct = 0;
        while (choice == 0) {
            bool isClick;
            COORD pos = getMouseEvent(isClick);
            int newHov = 0;
            if (pos.Y == 19) newHov = 1;
            else if (pos.Y == 20) newHov = 2;
            else if (pos.Y == 21) newHov = 3;
            if (newHov != hovAct) { hovAct = newHov; updateActionHover(hovAct); }
            if (isClick) {
                if (pos.Y == 19) choice = 1;
                else if (pos.Y == 20) choice = 2;
                else if (pos.Y == 21) choice = 3;
            }
        }

        if (choice == 1) { // 원소 공격
            if (player.mp >= 2) {
                drawSkillMenu(player, 1);
                flushInput();

                int sel = -1;
                int hovSlot = 0;
                while (sel == -1) {
                    bool isClick;
                    COORD pos = getMouseEvent(isClick);
                    int newHov = 0;
                    if (pos.Y == 20) newHov = 1;
                    else if (pos.Y == 21) newHov = 2;
                    else if (pos.Y == 22) newHov = 3;
                    if (newHov != hovSlot) { hovSlot = newHov; updateSlotHover(player, hovSlot); }
                    if (isClick) {
                        if (pos.Y == 20) sel = 1;
                        else if (pos.Y == 21) sel = 2;
                        else if (pos.Y == 22) sel = 3;
                        else sel = 0;
                    }
                }

                if (sel == 0) continue;
                int idx = sel - 1;

                if (idx >= 0 && idx < 3 && player.slots[idx] != "") {
                    player.mp -= 2;
                    string myElem = player.slots[idx];

                    int baseDmg = 20;
                    if (myElem.find("++") != string::npos) baseDmg = 40;
                    else if (myElem.find("+") != string::npos) baseDmg = 30;

                    float multiplier = elemCounter(myElem, enemy.element);
                    int finalDmg = (int)(baseDmg * multiplier);
                    
                    finalDmg += player.bonusDamage;

                    if ((enemy.isMiss && rand() % 2 == 0) || (rand() % 100 < enemy.evadeChance)) {
                        msg = enemy.name + "이(가) 공격을 회피했습니다.";
                    }
                    else {
                        animProjectile(player, myElem);
                        bool enemyWasDefending = enemy.isDefending;
                        enemy.takeDamage(finalDmg);
                        int dealtDmg = enemyWasDefending ? finalDmg / 2 : finalDmg;
                        animEnemyHit(enemy);
                        
                        int healAmt = 0;
                        if (player.lifeSteal > 0 && dealtDmg > 0) {
                            healAmt = (dealtDmg * player.lifeSteal) / 100;
                            if (healAmt < 1) healAmt = 1;
                            player.hp = min(player.hp + healAmt, player.maxHp);
                        }

                        string effectMsg = "";
                        if (multiplier > 1.0f) effectMsg = " 효과가 굉장했다. ";
                        else if (multiplier < 1.0f) effectMsg = " 효과가 별로인 듯하다. ";

                        msg = "[" + myElem + "] 공격! " + effectMsg + to_string(dealtDmg) + "의 피해를 입혔습니다.";
                        if (healAmt > 0) msg += " (체력 " + to_string(healAmt) + " 회복)";
                    }
                    enemy.isMiss = false;
                    enemy.isDefending = false;
                }
                else {
                    msg = "비어있는 슬롯입니다.";
                }
            }
            else {
                msg = "마나가 부족합니다.";
            }
        }
        else if (choice == 2) { // 조합 공격
            if (player.mp >= 6) {
                drawSkillMenu(player, 2);
                flushInput();

                int ult = -1;
                int hovCombo = 0;
                while (ult == -1) {
                    bool isClick;
                    COORD pos = getMouseEvent(isClick);
                    int newHov = 0;
                    if (pos.Y >= 20 && pos.Y <= 25) newHov = pos.Y - 19;
                    if (newHov != hovCombo) { hovCombo = newHov; updateComboHover(player, hovCombo); }
                    if (isClick) {
                        if (pos.Y == 20) ult = 1;
                        else if (pos.Y == 21) ult = 2;
                        else if (pos.Y == 22) ult = 3;
                        else if (pos.Y == 23) ult = 4;
                        else if (pos.Y == 24) ult = 5;
                        else if (pos.Y == 25) ult = 6;
                        else ult = 0;
                    }
                }

                if (ult == 0) continue;

                bool possible = false;
                int  finalDmg = 0;
                string skillName = "";
                string elem1 = "", elem2 = "";

                if (ult == 1 && hasElement(player, "불") && hasElement(player, "물")) { possible = true; skillName = "증기 폭발"; elem1 = "불"; elem2 = "물"; }
                else if (ult == 2 && hasElement(player, "불") && hasElement(player, "흙")) { possible = true; skillName = "용암 투척"; elem1 = "불"; elem2 = "흙"; }
                else if (ult == 3 && hasElement(player, "불") && hasElement(player, "바람")) { possible = true; skillName = "화염 돌풍"; elem1 = "불"; elem2 = "바람"; }
                else if (ult == 4 && hasElement(player, "물") && hasElement(player, "흙")) { possible = true; skillName = "진흙 늪";   elem1 = "물"; elem2 = "흙"; }
                else if (ult == 5 && hasElement(player, "물") && hasElement(player, "바람")) { possible = true; skillName = "빙결 화살"; elem1 = "물"; elem2 = "바람"; }
                else if (ult == 6 && hasElement(player, "흙") && hasElement(player, "바람")) { possible = true; skillName = "모래 폭풍"; elem1 = "흙"; elem2 = "바람"; }

                if (possible) {
                    player.mp -= 6; 
                    
                    int dmg1 = getElementDamage(player, elem1);
                    int dmg2 = getElementDamage(player, elem2);
                    
                    float mult1 = elemCounter(elem1, enemy.element);
                    float mult2 = elemCounter(elem2, enemy.element);
                    
                    finalDmg = (int)(((dmg1 * mult1) + (dmg2 * mult2)) * 1.5f);
                    finalDmg += player.bonusDamage;

                    if ((enemy.isMiss && rand() % 2 == 0) || (rand() % 100 < enemy.evadeChance)) {
                        msg = enemy.name + "이(가) 공격을 회피했습니다.";
                    }
                    else {
                        animComboAttack(player, skillName);
                        bool enemyWasDefending = enemy.isDefending;
                        enemy.takeDamage(finalDmg);
                        int dealtDmg = enemyWasDefending ? finalDmg / 2 : finalDmg;
                        animEnemyHit(enemy);
                        
                        int healAmt = 0;
                        if (player.lifeSteal > 0 && dealtDmg > 0) {
                            healAmt = (dealtDmg * player.lifeSteal) / 100;
                            if (healAmt < 1) healAmt = 1;
                            player.hp = min(player.hp + healAmt, player.maxHp);
                        }

                        string effectMsg = "";
                        if (mult1 > 1.0f || mult2 > 1.0f) effectMsg = " 효과가 굉장했다. ";
                        else if (mult1 < 1.0f || mult2 < 1.0f) effectMsg = " 효과가 별로인 듯하다. ";

                        msg = skillName + " 시전! " + effectMsg + to_string(dealtDmg) + "의 피해를 입혔습니다.";
                        if (healAmt > 0) msg += " (체력 " + to_string(healAmt) + " 회복)";
                    }
                    enemy.isMiss = false;
                    enemy.isDefending = false;
                }
                else {
                    msg = "원소가 부족하거나 잘못된 선택입니다.";
                }
            }
            else {
                msg = "마나가 부족합니다.";
            }
        }
        else if (choice == 3) {
            player.defend();
            int mpGain = min(10, player.maxMp - player.mp);
            player.mp += mpGain;
            animSlimeDefend(player);
            msg = "방어 자세를 취했습니다. (MP +" + to_string(mpGain) + ")";
        }
        else continue;

        if (enemy.hp <= 0) {
            enemy.hp = 0;
            enemyDie(player, enemy, msg);
            waitAnyClick(); 
            break;
        }

        drawBattle(player, enemy, msg);
        drawSkillMenu(player, 0);
        Sleep(1500);

        // ── 몬스터 턴 ──
        Pattern& mTurn = enemy.patterns[enemy.nextPattern];

        if (mTurn.type == "attack") {
            if (rand() % 100 < player.evadeChance) {
                msg = enemy.name + "의 공격을 회피했습니다.";
            } else {
                animEnemyAttack(enemy);
                int dmg = mTurn.damage + (enemy.attack / 2); 
                
                int actualDmg = dmg - player.defense;
                if (actualDmg < 1) actualDmg = 1;
                if (player.isDefending) actualDmg /= 2;

                player.takeDamage(dmg);
                animSlimeHit(player);

                if (player.isDefending)
                    msg = enemy.name + "의 공격! (방어 성공, " + to_string(actualDmg) + " 피해)";
                else
                    msg = enemy.name + "의 공격! (" + to_string(actualDmg) + " 피해)";
                
                if (enemy.lifeSteal > 0 && actualDmg > 0) {
                    int healAmt = (actualDmg * enemy.lifeSteal) / 100;
                    if (healAmt < 1) healAmt = 1;
                    enemy.hp = min(enemy.hp + healAmt, enemy.maxHp);
                    msg += " (적 체력 " + to_string(healAmt) + " 회복)";
                }
            }
        }
        else if (mTurn.type == "defend") {
            animEnemyDefend(enemy);
            enemy.defend();
            msg = enemy.name + "이(가) 방어 자세를 취했습니다.";
        }
        else if (mTurn.type == "heal") {
            animEnemyHeal(enemy);
            int heal = mTurn.damage;
            enemy.hp = min(enemy.hp + heal, enemy.maxHp);
            msg = enemy.name + "이(가) 체력을 회복했습니다. (+" + to_string(heal) + ")";
        }
        else if (mTurn.type == "buff") {
            animEnemyBuff(enemy);
            enemy.isMiss = true;
            msg = enemy.name + "이(가) 회피를 준비합니다.";
        }

        player.isDefending = false;

        // 패턴 무한 중복 방지 보정 시스템
        if (enemy.patterns.size() > 1) {
            int lastPattern = enemy.nextPattern;
            while (enemy.nextPattern == lastPattern) {
                enemy.nextPattern = rand() % enemy.patterns.size();
            }
        }

        if (player.hp <= 0) {
            player.hp = 0;
            drawBattle(player, enemy, enemy.name + "에게 쓰러졌습니다.");
            drawSkillMenu(player, 0);
            Sleep(2000);
            break;
        }
    }

    return (player.hp > 0);
}

// ── 보스 전투 (3페이즈) ──
bool startBossBattle(Player& player, Boss& boss) {
    if (boss.patterns.empty()) {
        throw runtime_error("보스전을 시작할 수 없습니다: 보스 패턴이 비어 있습니다.");
    }
    
    boss.elemIndex = (boss.elemIndex + 1) % 4;
    boss.defense = 5;
    boss.bonusDamage = 0;
    boss.lifeSteal = 0;
    boss.evadeChance = 0;

    string koName = "";
    if (boss.elemIndex == 0) { boss.element = "fire"; boss.patterns = boss.firePatterns; boss.defense += 2; boss.bonusDamage = 5; koName = "불"; }
    else if (boss.elemIndex == 1) { boss.element = "water"; boss.patterns = boss.waterPatterns; boss.lifeSteal = 20; koName = "물"; }
    else if (boss.elemIndex == 2) { boss.element = "earth"; boss.patterns = boss.earthPatterns; boss.defense += 10; koName = "흙"; }
    else if (boss.elemIndex == 3) { boss.element = "wind"; boss.patterns = boss.windPatterns; boss.evadeChance = 20; koName = "바람"; }

    string msg = "보스가 [" + koName + "] 속성으로 나타났습니다. (페이즈 1)";

    while (player.isAlive() && boss.isAlive()) {

        drawBattle(player, boss, msg);
        drawSkillMenu(player, 0);
        flushInput();

        int choice = 0;
        int hovAct = 0;
        while (choice == 0) {
            bool isClick;
            COORD pos = getMouseEvent(isClick);
            int newHov = 0;
            if (pos.Y == 19) newHov = 1;
            else if (pos.Y == 20) newHov = 2;
            else if (pos.Y == 21) newHov = 3;
            if (newHov != hovAct) { hovAct = newHov; updateActionHover(hovAct); }
            if (isClick) {
                if (pos.Y == 19) choice = 1;
                else if (pos.Y == 20) choice = 2;
                else if (pos.Y == 21) choice = 3;
            }
        }

        // ── 플레이어 턴 ──
        if (choice == 1) { // 원소 공격
            if (player.mp >= 2) {
                drawSkillMenu(player, 1);
                flushInput();
                int sel = -1;
                int hovSlot = 0;
                while (sel == -1) {
                    bool isClick;
                    COORD pos = getMouseEvent(isClick);
                    int newHov = 0;
                    if (pos.Y == 20) newHov = 1;
                    else if (pos.Y == 21) newHov = 2;
                    else if (pos.Y == 22) newHov = 3;
                    if (newHov != hovSlot) { hovSlot = newHov; updateSlotHover(player, hovSlot); }
                    if (isClick) {
                        if (pos.Y == 20) sel = 1;
                        else if (pos.Y == 21) sel = 2;
                        else if (pos.Y == 22) sel = 3;
                        else sel = 0;
                    }
                }
                if (sel == 0) continue;
                int idx = sel - 1;

                if (idx >= 0 && idx < 3 && player.slots[idx] != "") {
                    player.mp -= 2;
                    string myElem = player.slots[idx];
                    int baseDmg = 20;
                    if (myElem.find("++") != string::npos) baseDmg = 40;
                    else if (myElem.find("+") != string::npos) baseDmg = 30;

                    float multiplier = elemCounter(myElem, boss.element);
                    int finalDmg = (int)(baseDmg * multiplier);
                    finalDmg += player.bonusDamage;

                    if ((boss.isMiss && rand() % 2 == 0) || (rand() % 100 < boss.evadeChance)) {
                        msg = boss.name + "이(가) 공격을 회피했습니다.";
                    }
                    else {
                        animProjectile(player, myElem);
                        bool bossWasDefending = boss.isDefending;
                        boss.takeDamage(finalDmg);
                        int dealtDmg = bossWasDefending ? finalDmg / 2 : finalDmg;
                        animEnemyHit(boss);

                        int healAmt = 0;
                        if (player.lifeSteal > 0 && dealtDmg > 0) {
                            healAmt = (dealtDmg * player.lifeSteal) / 100;
                            if (healAmt < 1) healAmt = 1;
                            player.hp = min(player.hp + healAmt, player.maxHp);
                        }

                        string effectMsg = "";
                        if (multiplier > 1.0f) effectMsg = " 효과가 굉장했다. ";
                        else if (multiplier < 1.0f) effectMsg = " 효과가 별로인 듯하다. ";

                        msg = "[" + myElem + "] 공격! " + effectMsg + to_string(dealtDmg) + " 피해를 입혔습니다.";
                        if (healAmt > 0) msg += " (체력 " + to_string(healAmt) + " 회복)";
                    }
                    boss.isDefending = false;
                    boss.isMiss = false;
                } else msg = "비어있는 슬롯입니다.";
            } else msg = "마나가 부족합니다.";
        }
        else if (choice == 2) { // 조합 공격
            if (player.mp >= 6) {
                drawSkillMenu(player, 2);
                flushInput();
                int ult = -1;
                int hovCombo = 0;
                while (ult == -1) {
                    bool isClick;
                    COORD pos = getMouseEvent(isClick);
                    int newHov = 0;
                    if (pos.Y >= 20 && pos.Y <= 25) newHov = pos.Y - 19;
                    if (newHov != hovCombo) { hovCombo = newHov; updateComboHover(player, hovCombo); }
                    if (isClick) {
                        if (pos.Y >= 20 && pos.Y <= 25) ult = pos.Y - 19;
                        else ult = 0;
                    }
                }
                if (ult == 0) continue;

                bool possible = false;
                string skillName = "", elem1 = "", elem2 = "";

                if (ult == 1 && hasElement(player, "불") && hasElement(player, "물")) { possible = true; skillName = "증기 폭발"; elem1 = "불"; elem2 = "물"; }
                else if (ult == 2 && hasElement(player, "불") && hasElement(player, "흙")) { possible = true; skillName = "용암 투척"; elem1 = "불"; elem2 = "흙"; }
                else if (ult == 3 && hasElement(player, "불") && hasElement(player, "바람")) { possible = true; skillName = "화염 돌풍"; elem1 = "불"; elem2 = "바람"; }
                else if (ult == 4 && hasElement(player, "물") && hasElement(player, "흙")) { possible = true; skillName = "진흙 늪";   elem1 = "물"; elem2 = "흙"; }
                else if (ult == 5 && hasElement(player, "물") && hasElement(player, "바람")) { possible = true; skillName = "빙결 화살"; elem1 = "물"; elem2 = "바람"; }
                else if (ult == 6 && hasElement(player, "흙") && hasElement(player, "바람")) { possible = true; skillName = "모래 폭풍"; elem1 = "흙"; elem2 = "바람"; }

                if (possible) {
                    player.mp -= 6; 
                    int dmg1 = getElementDamage(player, elem1);
                    int dmg2 = getElementDamage(player, elem2);
                    float mult1 = elemCounter(elem1, boss.element);
                    float mult2 = elemCounter(elem2, boss.element);
                    
                    int finalDmg = (int)(((dmg1 * mult1) + (dmg2 * mult2)) * 1.2f);
                    finalDmg += player.bonusDamage;

                    if ((boss.isMiss && rand() % 2 == 0) || (rand() % 100 < boss.evadeChance)) {
                        msg = boss.name + "이(가) 공격을 회피했습니다.";
                    }
                    else {
                        animComboAttack(player, skillName);
                        bool bossWasDefending = boss.isDefending;
                        boss.takeDamage(finalDmg);
                        int dealtDmg = bossWasDefending ? finalDmg / 2 : finalDmg;
                        animEnemyHit(boss);
                        
                        int healAmt = 0;
                        if (player.lifeSteal > 0 && dealtDmg > 0) {
                            healAmt = (dealtDmg * player.lifeSteal) / 100;
                            if (healAmt < 1) healAmt = 1; 
                            player.hp = min(player.hp + healAmt, player.maxHp);
                        }
                        string effectMsg = "";
                        if (mult1 > 1.0f || mult2 > 1.0f) effectMsg = " 효과가 굉장했다. ";
                        else if (mult1 < 1.0f || mult2 < 1.0f) effectMsg = " 효과가 별로인 듯하다. ";

                        msg = skillName + " 시전! " + effectMsg + to_string(dealtDmg) + " 피해를 입혔습니다.";
                        if (healAmt > 0) msg += " (체력 " + to_string(healAmt) + " 회복)";
                    }
                    boss.isDefending = false;
                    boss.isMiss = false;
                } else msg = "원소가 부족하거나 잘못된 선택입니다.";
            } else msg = "마나가 부족합니다.";
        }
        else if (choice == 3) {
            player.defend();
            int mpGain = min(10, player.maxMp - player.mp);
            player.mp += mpGain;
            animSlimeDefend(player);
            msg = "방어 자세를 취했습니다. (MP +" + to_string(mpGain) + ")";
        }
        else continue;

        // ── 페이즈 전환 제어 및 체력/마나 회복량 상향 부분 ──
        if (boss.hp <= 0) {
            if (boss.phase < 3) {
                int currentTurn = boss.turnCount;
                int currentElemIdx = boss.elemIndex;
                string currentElem = boss.element;
                vector<Pattern> currentPat = boss.patterns;
                int currentDef = boss.defense;
                int currentDmg = boss.bonusDamage;
                int currentLife = boss.lifeSteal;
                int currentEvade = boss.evadeChance;

                boss.nextPhase();
                if (!boss.phaseAscii.empty()) {
                    int artIdx = boss.phase - 1;
                    if (artIdx >= 0 && artIdx < (int)boss.phaseAscii.size())
                        boss.ascii = boss.phaseAscii[artIdx];
                }                
                boss.turnCount = 0;
                boss.elemIndex = currentElemIdx;
                boss.element = currentElem;
                boss.patterns = currentPat;
                boss.defense = currentDef;
                boss.bonusDamage = currentDmg;
                boss.lifeSteal = currentLife;
                boss.evadeChance = currentEvade;

                player.hp = min(player.hp + 80, player.maxHp);
                player.mp = min(player.mp + 30, player.maxMp);
                
                msg = "보스가 다음 페이즈로 돌입합니다! 체력(+80)과 마나(+30)가 회복되었습니다.";
                drawBattle(player, boss, msg);
                Sleep(2000);
                continue; 
            } else {
                boss.hp = 0;
                animEnemyDie(boss);
                boss.ascii.clear();
                msg = boss.name + "이(가) 완전히 쓰러졌습니다.";
                drawBattle(player, boss, msg);
                drawSkillMenu(player, 0);
                gotoxy(29, 1);
                cout << "보스전 승리! (아무 곳이나 클릭하면 계속...)";
                waitAnyClick();
                break;
            }
        }

        drawBattle(player, boss, msg);
        drawSkillMenu(player, 0);
        Sleep(1500);

        // ── 보스 턴 ──
        Pattern& bTurn = boss.patterns[boss.nextPattern];
        if (bTurn.type == "attack") {
            if (rand() % 100 < player.evadeChance) {
                msg = boss.name + "의 공격을 회피했습니다.";
            } else {
                animEnemyAttack(boss);
                int dmg = bTurn.damage + (boss.attack / 2);
                int actualDmg = dmg - player.defense;
                if (actualDmg < 1) actualDmg = 1;
                if (player.isDefending) actualDmg /= 2;

                player.takeDamage(dmg);
                animSlimeHit(player);
                
                if (player.isDefending) msg = boss.name + "의 공격! (방어 성공, " + to_string(actualDmg) + " 피해)";
                else msg = boss.name + "의 공격! (" + to_string(actualDmg) + " 피해)";
                
                if (boss.lifeSteal > 0 && actualDmg > 0) {
                    int healAmt = (actualDmg * boss.lifeSteal) / 100;
                    if (healAmt < 1) healAmt = 1;
                    boss.hp = min(boss.hp + healAmt, boss.maxHp);
                    msg += " (적 체력 " + to_string(healAmt) + " 회복)";
                }
            }
        }
        else if (bTurn.type == "defend") {
            animEnemyDefend(boss);
            boss.defend();
            msg = boss.name + "이(가) 방어 자세를 취했습니다.";
        }
        else if (bTurn.type == "heal") {
            animEnemyHeal(boss);
            int heal = bTurn.damage;
            boss.hp = min(boss.hp + heal, boss.maxHp);
            msg = boss.name + "이(가) 체력을 회복했습니다. (+" + to_string(heal) + ")";
        }
        else if (bTurn.type == "buff") {
            animEnemyBuff(boss);
            boss.isMiss = true;
            msg = boss.name + "이(가) 회피를 준비합니다.";
        }

        player.isDefending = false;

        // 보스 패턴 무한 중복 방지 시스템
        if (boss.patterns.size() > 1) {
            int lastPattern = boss.nextPattern;
            while (boss.nextPattern == lastPattern) {
                boss.nextPattern = rand() % boss.patterns.size();
            }
        }
        
        // ── 3턴마다 속성 변환 로직 ──
        boss.turnCount++;
        boss.elemIndex = (boss.elemIndex + 1) % 4;
        
        if (player.isAlive()) {
            string koName = "";
            boss.defense = 5; 
            boss.lifeSteal = 0; 
            boss.evadeChance = 0;
            boss.bonusDamage = 0;
            
            if (boss.elemIndex == 0) { boss.element = "fire"; boss.patterns = boss.firePatterns; boss.defense += 2; boss.bonusDamage = 5; koName = "불"; }
            else if (boss.elemIndex == 1) { boss.element = "water"; boss.patterns = boss.waterPatterns; boss.lifeSteal = 20; koName = "물"; }
            else if (boss.elemIndex == 2) { boss.element = "earth"; boss.patterns = boss.earthPatterns; boss.defense += 10; koName = "흙"; }
            else if (boss.elemIndex == 3) { boss.element = "wind"; boss.patterns = boss.windPatterns; boss.evadeChance = 20; koName = "바람"; }

            msg += " 보스가 [" + koName + "] 속성으로 변환했습니다.";
        }

        if (!player.isAlive()) {
            player.hp = 0;
            drawBattle(player, boss, boss.name + "에게 쓰러졌습니다.");
            drawSkillMenu(player, 0);
            Sleep(2000);
            break;
        }
    }

    return player.isAlive();
}


