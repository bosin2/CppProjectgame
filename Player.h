#pragma once
#include "Define.h"

enum Element {
    NONE  = 0, // 무속성
    FIRE  = 1, // 불
    WATER = 2, // 물
    EARTH = 3, // 흙
    WIND  = 4  // 바람
};

// 플레이어 정보 구조체
struct Player {
    int hp    = 100;
    int maxHp = 100;
    int mp    = 50;
    int maxMp = 50;
    int row   = 2; // 맵상 현재 행 위치
    int col   = 0; // 맵상 현재 열 위치

    int slot[3] = { NONE, NONE, NONE }; // 장착한 속성 슬롯

    // slot 배열에 원소 장착/교체 로직 구현 필요
    // - 전투 승리 후 원소 획득 시 slot에 저장
    // - 슬롯이 꽉 찬 경우 교체 선택 UI 구현
    // - 현재는 전부 NONE으로 초기화된 상태

    // 속성별 전투 횟수 기록 (속성 없애고 전투횟수 세는거로 변경해도됨)
    int fightCount[5] = { 0, 0, 0, 0, 0 }; // 0:없음 1:불 2:물 3:흙 4:바람
};

// 슬라임을 전투 화면에 출력하는 함수
void drawSlime(Player& player);
