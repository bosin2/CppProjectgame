#include <windows.h>
#include "Map.h"
#include "Battle.h"

int main() {
    // 인코딩 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Player player;  // 플레이어 객체 생성
    initMap();       // 맵 무작위 생성
    drawStartMap();  // 시작 위치 선택 화면 출력

    char startInput;
    cin >> startInput;

    player.row = startInput - '1';
    player.col = 0;

    // 메인 게임 루프
    while (true) {
        drawMap(player);

        cout << "\n  입력: ";
        char input;
        cin >> input;

        int newRow = player.row;
        int newCol = player.col + 1; // 무조건 오른쪽으로 한 칸

        // 이동 방향 결정
        if (input == '1') newRow = player.row - 1; // ↗ 우상
        if (input == '2') newRow = player.row;      // → 우
        if (input == '3') newRow = player.row + 1;  // ↘ 우하

        // 맵 경계 체크
        if (newRow < 0 || newRow >= ROWS) {
            cout << "이동 불가!\n";
            continue;
        }

        // 지나온 타일 표시 제거
        gameMap[player.row][player.col] = VISIT;

        // 플레이어 위치 업데이트
        player.row = newRow;
        player.col = newCol;

        // 몬스터 타일
        if (gameMap[player.row][player.col] == MONSTER) {
            int elem  = elementMap[player.row][player.col];
            int count = player.fightCount[elem];

            // 전투 횟수에 따른 등급 결정 (없애셔도 됩니다)
            string grade;
            if (count == 0)
                grade = "하급";
            else if (count <= 2)
                grade = (rand() % 2 == 0) ? "하급" : "중급";
            else if (count <= 3)
                grade = (rand() % 2 == 0) ? "중급" : "상급";
            else
                grade = "상급";

            player.fightCount[elem]++;

            // 속성 정수 → 문자열 변환 (JSON 로드용)
            string elemName;
            switch (elem) {
            case FIRE:  elemName = "fire";  break;
            case WATER: elemName = "water"; break;
            case EARTH: elemName = "earth"; break;
            case WIND:  elemName = "wind";  break;
            }

            Enemy enemy = loadEnemy(elemName, grade);
            drawBattle(player, enemy);

            // 현재는 전투 화면만 보여주고 아무 키나 누르면 넘어가는 상태
            cin.get();
            cin.get();

            // 전투 승리 후 원소 획득 처리 필요
            // - 해당 타일의 속성 원소를 슬롯에 장착 또는 교체
            // - 슬롯이 꽉 찬 경우 교체 선택 UI 호출
        }

        // 우물 타일
        if (gameMap[player.row][player.col] == WELL) {
            // 우물 회복량 수치 구현
 
        }

        // 보스 타일
        if (gameMap[player.row][player.col] == BOSS) {
            drawMap(player);
            cout << "\n  보스 등장!\n";

            // 보스 전투 연결 필요

            cin.get();
            break;
        }
    }

    // 게임 클리어 // 게임오버 화면 구현 필요

    cin.get();
    return 0;
}
