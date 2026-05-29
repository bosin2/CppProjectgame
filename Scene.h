#pragma once
#include "Define.h"
#include "Player.h"

void showPrologue();             // 프롤로그 씬 출력
void showTitle();                // 타이틀 화면 출력
void startChoice();              // 원소 선택 화면 출력
void showExplan();               // 게임 설명 화면 출력 (미구현)
void showHeal(Player& player);   // 우물 회복 씬 출력
void showEnd(Player& player);    // 클리어 엔딩 씬 출력
void showDie();                  // 사망 게임오버 씬 출력