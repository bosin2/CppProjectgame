#include "Battle.h"
#include "BattleAnim.h"
#include "Boss.h"
#include "Player.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
using namespace std;


void enemyDie(Player& player, Enemy& enemy, string& msg) {
    // 스르륵 사라지는 애니메이션
    animEnemyDie(enemy);

    enemy.ascii.clear();
    msg = enemy.name + "(이)가 쓰러졌습니다 ! ⸜(⠀  ᐢ ᵕ ᐢ  )⸝‍";
    drawBattle(player, enemy, msg);
    drawSkillMenu(player, 0);
    gotoxy(29, 1);
    cout << "전투 승리! ヽ(◍˃ᗜ˂◍)ﾉ (엔터를 누르면 계속...)";
}

bool hasElement(Player& p, string name) {
    for (int i = 0; i < 3; i++)
        if (p.slots[i].find(name) != string::npos) return true;
    return false;
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

// ── UI ──
void drawBattle(Player& player, Enemy& enemy, string currentMessage) {
    cout << CLEAR;

    gotoxy(1, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    gotoxy(2, 1); cout << "┃ 슬라임                           ┃";
    gotoxy(3, 1); cout << "┃ HP: "; drawHpBar(player.hp, player.maxHp);
    gotoxy(4, 1); cout << "┃ MP: "; drawHpBar(player.mp, player.maxMp);
    gotoxy(5, 1); cout << "┃ 슬롯: ["
        << (player.slots[0] == "" ? "-" : player.slots[0]) << "] ["
        << (player.slots[1] == "" ? "-" : player.slots[1]) << "] ["
        << (player.slots[2] == "" ? "-" : player.slots[2]) << "]";
    gotoxy(6, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    gotoxy(1, 50); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
    gotoxy(2, 50); cout << "┃ <" << enemy.grade << " 정령> " << enemy.name;
    gotoxy(3, 50); cout << "┃";
    gotoxy(4, 50); cout << "┃ HP: "; drawHpBar(enemy.hp, enemy.maxHp);
    if (!enemy.patterns.empty()) {
        Pattern& nextPat = enemy.patterns[enemy.nextPattern];
        gotoxy(5, 50); cout << "┃ " << nextPat.name << "를 준비하고 있습니다.(" << nextPat.damage << ")";
    }
    gotoxy(6, 50); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    drawSlime(player);

    for (int i = 0; i < (int)enemy.ascii.size(); i++) {
        gotoxy(8 + i, 55);
        if (enemy.element == "fire")  cout << RED << enemy.ascii[i] << CLR;
        else if (enemy.element == "water") cout << BLUE << enemy.ascii[i] << CLR;
        else if (enemy.element == "earth") cout << YELLOW << enemy.ascii[i] << CLR;
        else if (enemy.element == "wind")  cout << GREEN << enemy.ascii[i] << CLR;
        else                               cout << PINK << enemy.ascii[i] << CLR; // 보스(all)
    }

    gotoxy(15, 1); cout << "──────────────────────────────────────────────────────────────────────";
    gotoxy(16, 4); cout << ">> " << currentMessage;
    gotoxy(17, 1); cout << "──────────────────────────────────────────────────────────────────────";
}

void drawSkillMenu(Player& player, int mode) {
    for (int row = 20; row <= 29; row++) {
        gotoxy(row, 1);
        cout << "                                                                        ";
    }
    if (mode == 0) {
        gotoxy(19, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
        gotoxy(20, 1); cout << "┃  1: 원소 공격            ┃";
        gotoxy(21, 1); cout << "┃  2: 조합 공격            ┃";
        gotoxy(22, 1); cout << "┃  3: 방어 하기            ┃";
        gotoxy(23, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
        gotoxy(27, 1); cout << "선택 (1~3)⇒ ";
    }
    else if (mode == 1) {
        gotoxy(19, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
        gotoxy(20, 1); cout << "┃ [ 원소 공격 목록 (MP 1) ]            ┃";
        gotoxy(21, 1); cout << "┃ 1: [" << (player.slots[0] == "" ? "-" : player.slots[0]) << "]";
        gotoxy(22, 1); cout << "┃ 2: [" << (player.slots[1] == "" ? "-" : player.slots[1]) << "]";
        gotoxy(23, 1); cout << "┃ 3: [" << (player.slots[2] == "" ? "-" : player.slots[2]) << "]";
        gotoxy(24, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
        gotoxy(27, 1); cout << "슬롯 번호 선택 (1~3, 0=뒤로) ⇒ ";
    }
    else if (mode == 2) {
        gotoxy(19, 1); cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓";
        gotoxy(20, 1); cout << "┃ [ 조합 공격 목록 (MP 3) ]            ┃";
        gotoxy(21, 1); cout << "┃ 1: " << (hasElement(player, "불") && hasElement(player, "물") ? "증기 폭발 (불+물)  " : "[조합 잠금]        ") << "               ┃";
        gotoxy(22, 1); cout << "┃ 2: " << (hasElement(player, "불") && hasElement(player, "흙") ? "용암 투척 (불+흙)  " : "[조합 잠금]        ") << "               ┃";
        gotoxy(23, 1); cout << "┃ 3: " << (hasElement(player, "불") && hasElement(player, "바람") ? "화염 돌풍 (불+바람)" : "[조합 잠금]        ") << "               ┃";
        gotoxy(24, 1); cout << "┃ 4: " << (hasElement(player, "물") && hasElement(player, "흙") ? "진흙 늪   (물+흙)  " : "[조합 잠금]        ") << "               ┃";
        gotoxy(25, 1); cout << "┃ 5: " << (hasElement(player, "물") && hasElement(player, "바람") ? "빙결 화살 (물+바람)" : "[조합 잠금]        ") << "               ┃";
        gotoxy(26, 1); cout << "┃ 6: " << (hasElement(player, "흙") && hasElement(player, "바람") ? "모래 폭풍 (흙+바람)" : "[조합 잠금]        ") << "               ┃";
        gotoxy(27, 1); cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";
        gotoxy(28, 1); cout << "조합 번호 선택 (1~6, 0=뒤로) ⇒ ";
    }
}

// ── 전투 로직 ──
bool startBattle(Player& player, Enemy& enemy) {
    enemy.nextPattern = rand() % enemy.patterns.size();
    string msg = enemy.name + "(이)가 나타났습니다 !";

    while (player.hp > 0 && enemy.hp > 0) {
        drawBattle(player, enemy, msg);
        drawSkillMenu(player, 0);
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (player.mp >= 1) {
                drawSkillMenu(player, 1);
                int sel;
                cin >> sel;
                if (sel == 0) continue;
                int idx = sel - 1;

                if (idx >= 0 && idx < 3 && player.slots[idx] != "") {
                    player.mp -= 1;
                    string myElem = player.slots[idx];

                    int baseDmg = 20;
                    if (myElem.find("++") != string::npos) baseDmg = 40;
                    else if (myElem.find("+") != string::npos) baseDmg = 30;

                    float multiplier = elemCounter(myElem, enemy.element);
                    int finalDmg = (int)(baseDmg * multiplier);

                    if (enemy.isMiss && rand() % 2 == 0) {
                        msg = enemy.name + "이(가) 공격을 회피했다 !";
                    }
                    else {
                        animProjectile(player, myElem);
                        enemy.takeDamage(finalDmg);
                        animEnemyHit(enemy);
                        msg = "[" + myElem + "] 공격 !";
                        if (multiplier > 1.0f) msg += " 효과가 굉장했다 !! " + to_string(finalDmg) + "의 피해를 입혔다 ! (*>∇<)ﾉ";
                        else if (multiplier < 1.0f) msg += " 효과가 별로인 듯하다...( > ~ < )💦 " + to_string(finalDmg) + " 의 피해를 입혔다...";
                        else                        msg += " " + to_string(finalDmg) + "의 피해를 입혔다 ! ᐠ( ᐢ ᵕ ᐢ )ᐟ";
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
        else if (choice == 2) {
            if (player.mp >= 3) {
                drawSkillMenu(player, 2);
                int ult;
                cin >> ult;
                if (ult == 0) continue;

                bool possible = false;
                int  finalDmg = 0;
                string skillName = "";

                if (ult == 1 && hasElement(player, "불") && hasElement(player, "물")) { possible = true; skillName = "증기 폭발"; finalDmg = 45; }
                else if (ult == 2 && hasElement(player, "불") && hasElement(player, "흙")) { possible = true; skillName = "용암 투척"; finalDmg = 50; }
                else if (ult == 3 && hasElement(player, "불") && hasElement(player, "바람")) { possible = true; skillName = "화염 돌풍"; finalDmg = 55; }
                else if (ult == 4 && hasElement(player, "물") && hasElement(player, "흙")) { possible = true; skillName = "진흙 늪";   finalDmg = 40; }
                else if (ult == 5 && hasElement(player, "물") && hasElement(player, "바람")) { possible = true; skillName = "빙결 화살"; finalDmg = 48; }
                else if (ult == 6 && hasElement(player, "흙") && hasElement(player, "바람")) { possible = true; skillName = "모래 폭풍"; finalDmg = 42; }

                if (possible) {
                    if (enemy.isMiss && rand() % 2 == 0) {
                        msg = enemy.name + "이(가) 공격을 회피했다 ! Σ(； ･`д･´)";
                    }
                    else {
                        animComboAttack(player, skillName);
                        player.mp -= 3;
                        enemy.takeDamage(finalDmg);
                        animEnemyHit(enemy);
                        msg = skillName + " 시전 !! " + enemy.name + "에게 " + to_string(finalDmg) + "의 피해 ! (*>∇<)ﾉ";
                    }
                    enemy.isMiss = false;
                    enemy.isDefending = false;
                }
                else {
                    msg = "원소가 부족하거나 잘못된 선택입니다.";
                }
            }
            else {
                msg = "마나가 부족합니다. (필요 MP: 3)";
            }
        }
        else if (choice == 3) {
            player.defend();
            animSlimeDefend(player);
            msg = "방어 자세를 취했습니다. 입는 데미지가 줄어듭니다. (･`_´･ )";
        }
        else continue;

        if (enemy.hp <= 0) {
            enemy.hp = 0;
            enemyDie(player, enemy, msg);
            cin.ignore(1000, '\n');
            cin.get();
            break;
        }

        drawBattle(player, enemy, msg);
        drawSkillMenu(player, 0);
        Sleep(1500);

        // 몬스터 턴
        Pattern& mTurn = enemy.patterns[enemy.nextPattern];

        if (mTurn.type == "attack") {
            animEnemyAttack(enemy);
            int dmg = mTurn.damage;
            player.takeDamage(dmg);
            animSlimeHit(player);
            if (player.isDefending)
                msg = enemy.name + "이(가) [" + mTurn.name + "]을(를) 사용했습니다 ! (방어 성공 ! " + to_string(dmg / 2) + " 피해)";
            else
                msg = enemy.name + "이(가) [" + mTurn.name + "]을(를) 사용했습니다 ! (" + to_string(dmg) + " 피해)";
        }
        else if (mTurn.type == "defend") {
            animEnemyDefend(enemy);
            enemy.defend();
            msg = enemy.name + "이(가) 방어 자세를 취했습니다 !";
        }
        else if (mTurn.type == "heal") {
            animEnemyHeal(enemy);
            int heal = mTurn.damage;
            enemy.hp = min(enemy.hp + heal, enemy.maxHp);
            msg = enemy.name + "이(가) 체력을 회복했습니다 ! (+" + to_string(heal) + ")";
        }
        else if (mTurn.type == "buff") {
            animEnemyBuff(enemy);
            enemy.isMiss = true;
            msg = enemy.name + "이(가) 몸을 날렵하게 했습니다 !";
        }

        player.isDefending = false;
        enemy.nextPattern = rand() % enemy.patterns.size();

        if (player.hp <= 0) {
            player.hp = 0;
            drawBattle(player, enemy, enemy.name + "에게 쓰러졌습니다...");
            drawSkillMenu(player, 0);
            Sleep(2000);
            break;
        }
    }

    return (player.hp > 0);
}

// 보스 전투 로직 (Boss는 Enemy& 로 drawBattle 재사용)
bool startBossBattle(Player& player, Boss& boss) {
    boss.nextPattern = 0;
    string msg = "";

    while (player.isAlive() && boss.isAlive()) {

        drawBattle(player, boss, msg);
        drawSkillMenu(player, 0);
        int choice;
        cin >> choice;

        // ── 플레이어 턴 ──
        if (choice == 1) {
            if (player.mp >= 1) {
                drawSkillMenu(player, 1);
                int sel; cin >> sel;
                if (sel == 0) continue;
                int idx = sel - 1;

                if (idx >= 0 && idx < 3 && player.slots[idx] != "") {
                    player.mp -= 1;
                    string myElem = player.slots[idx];

                    int baseDmg = 20;
                    if (myElem.find("++") != string::npos) baseDmg = 40;
                    else if (myElem.find("+") != string::npos) baseDmg = 30;

                    boss.takeDamage(baseDmg);
                    msg = "[" + myElem + "] 공격 ! " + to_string(baseDmg) + "의 피해 ! ᐠ( ᐢ ᵕ ᐢ )ᐟ";
                    boss.isDefending = false;
                }
                else {
                    msg = "비어있는 슬롯입니다.";
                }
            }
            else {
                msg = "마나가 부족합니다.";
            }
        }
        else if (choice == 2) {
            if (player.mp >= 3) {
                drawSkillMenu(player, 2);
                int ult; cin >> ult;
                if (ult == 0) continue;

                bool possible = false;
                int  finalDmg = 0;
                string skillName = "";

                if (ult == 1 && hasElement(player, "불") && hasElement(player, "물")) { possible = true; skillName = "증기 폭발"; finalDmg = 45; }
                else if (ult == 2 && hasElement(player, "불") && hasElement(player, "흙")) { possible = true; skillName = "용암 투척"; finalDmg = 50; }
                else if (ult == 3 && hasElement(player, "불") && hasElement(player, "바람")) { possible = true; skillName = "화염 돌풍"; finalDmg = 55; }
                else if (ult == 4 && hasElement(player, "물") && hasElement(player, "흙")) { possible = true; skillName = "진흙 늪";   finalDmg = 40; }
                else if (ult == 5 && hasElement(player, "물") && hasElement(player, "바람")) { possible = true; skillName = "빙결 화살"; finalDmg = 48; }
                else if (ult == 6 && hasElement(player, "흙") && hasElement(player, "바람")) { possible = true; skillName = "모래 폭풍"; finalDmg = 42; }

                if (possible) {
                    player.mp -= 3;
                    boss.takeDamage(finalDmg);
                    msg = skillName + " 시전 !! 보스에게 " + to_string(finalDmg) + "의 피해 ! (*>∇<)ﾉ";
                    boss.isDefending = false;
                }
                else {
                    msg = "원소가 부족하거나 잘못된 선택입니다.";
                }
            }
            else {
                msg = "마나가 부족합니다. (필요 MP: 3)";
            }
        }
        else if (choice == 3) {
            player.defend();
            msg = "방어 자세를 취했습니다. (･`_´･ )";
        }
        else continue;

        // 보스 사망 체크
        if (!boss.isAlive()) {
            boss.hp = 0;
            cin.get();
            break;
        }

        drawBattle(player, boss, msg);
        drawSkillMenu(player, 0);
        Sleep(3000);

        // ── 보스 턴 ──
        Pattern& bTurn = boss.patterns[boss.nextPattern];

        if (bTurn.type == "attack") {
            int dmg = bTurn.damage;
            player.takeDamage(dmg);
            if (player.isDefending)
                msg = "보스 이름 [" + bTurn.name + "] ! (방어 성공 ! " + to_string(dmg / 2) + " 피해)";
            else
                msg = "보스 이름 [" + bTurn.name + "] 을(를) 사용했다 ! (" + to_string(dmg) + " 피해)";
        }
        else if (bTurn.type == "defend") {
            boss.defend();
        }

        player.isDefending = false;
        boss.nextPattern = rand() % boss.patterns.size();

        if (!player.isAlive()) {
            player.hp = 0;
            drawBattle(player, boss, "보스 에게 쓰러졌습니다...");
            Sleep(2000);
            break;
        }
    }

    return player.isAlive();
}