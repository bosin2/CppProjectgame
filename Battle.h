#pragma once
#include "Define.h"
#include "Player.h"
#include "Enemy.h"

// 콘솔 커서를 특정 위치(행, 열)로 이동시키는 함수
void gotoxy(int row, int col);

// 전투 화면 UI를 그리는 함수
void drawBattle(Player& player, Enemy& enemy);

// 전투 루프 전체를 관리하는 함수
// - 턴 반복 / 플레이어 입력 / 적 행동 / 승패 판정

// 플레이어 행동 처리 함수
// - 원소 공격 / 조합 공격 / 방어 선택

// 적 행동 처리 함수
// - 예고된 패턴 실행

// 적 행동 예고를 화면에 출력하는 함수

// 전투 결과 메시지 출력 함수 (승리 / 패배)
