#pragma once
#include "Define.h"
#include "Player.h"
#include "Enemy.h"
#include <string>
#include <vector>
using namespace std;

// 전투 애니메이션 모듈

void clearArea(int row, int col, int width, int height);                          // 지정 영역 공백으로 지우기
void drawArtAt(int row, int col, vector<string>& ascii, const char* color);       // 아스키아트를 위치·색상으로 출력
void shakeArt(int row, int col, vector<string>& ascii, const char* color,
    vector<int> offsets = { 2, -2, 1, -1 }, int frameDelay = 60);                // 아스키아트 좌우 흔들기
void moveArt(int row, int startCol, int endCol, vector<string>& ascii,
    const char* color, int speed = 30);                                           // 아스키아트 수평 이동

vector<string> getProjectile(string slotName);       // 슬롯명으로 발사체 아트 반환
const char* getElemColor(string slotName);           // 슬롯명으로 원소 색상 반환
void animProjectile(Player& player, string slotName);// 원소 발사체 공격 애니메이션

const char* getEnemyColor(string element);           // 몬스터 원소로 색상 반환
void animEnemyHit(Enemy& enemy);                     // 몬스터 피격 애니메이션
void animSlimeHit(Player& player);                   // 슬라임 피격 애니메이션
void animSlimeDefend(Player& player);                // 슬라임 방어 이펙트
void animEnemyAttack(Enemy& enemy);                  // 몬스터 공격 애니메이션
void animEnemyDefend(Enemy& enemy);                  // 몬스터 방어 이펙트
void animEnemyHeal(Enemy& enemy);                    // 몬스터 회복 이펙트
void animEnemyBuff(Enemy& enemy);                    // 몬스터 버프 이펙트
void animEnemyDie(Enemy& enemy);                     // 몬스터 소멸 애니메이션
void animComboAttack(Player& player, string comboName); // 조합 공격 애니메이션