#pragma once
#include "Define.h"
#include "Player.h"
#include "Enemy.h"
#include <string> 


// 전투 화면 UI를 그리는 함수
void drawBattle(Player& player, Enemy& enemy, string currentMessage = "전투 시작!");
void drawSkillMenu(Player& player, int mode);

// 전투 루프 전체를 관리하는 함수
bool startBattle(Player& player, Enemy& enemy);
