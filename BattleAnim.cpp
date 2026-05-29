#include "BattleAnim.h"
#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;
static int displayWidth(const string& text) {
    int width = 0;
    for (size_t i = 0; i < text.size();) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        width++;

        if (c < 0x80) i += 1;
        else if ((c & 0xE0) == 0xC0) i += 2;
        else if ((c & 0xF0) == 0xE0) i += 3;
        else if ((c & 0xF8) == 0xF0) i += 4;
        else i += 1;
    }
    return width;
}

// 지정 영역을 공백으로 지우기
void clearArea(int row, int col, int width, int height) {
    for (int r = 0; r < height; r++) {
        gotoxy(row + r, col);
        for (int c = 0; c < width; c++) {
            cout << " ";
        }
    }
    cout.flush();
}

// 아스키아트를 위치·색상으로 출력
void drawArtAt(int row, int col, vector<string>& ascii, const char* color) {
    for (int i = 0; i < (int)ascii.size(); i++) {
        gotoxy(row + i, col);
        cout << color << ascii[i] << CLR;
    }
    cout.flush();
}

// 아스키아트 좌우 흔들기
void shakeArt(int row, int col, vector<string>& ascii, const char* color,
    vector<int> offsets, int frameDelay) {

    // 지울 영역 너비 계산용
    int maxWidth = 0;
    for (int i = 0; i < (int)ascii.size(); i++) {
        if (displayWidth(ascii[i]) > maxWidth)
            maxWidth = displayWidth(ascii[i]);
    }
    int height = (int)ascii.size();

    // 흔들림 범위 포함한 지울 영역
    int clearCol = max(1, col - 4);
    int clearWidth = maxWidth + 8;

    for (int f = 0; f < (int)offsets.size(); f++) {
        // 이전 프레임 지우기
        clearArea(row, clearCol, clearWidth, height);

        // 흔들린 위치에 다시 그리기
        drawArtAt(row, col + offsets[f], ascii, color);

        Sleep(frameDelay);
    }

    // 원래 위치로 복귀
    clearArea(row, clearCol, clearWidth, height);
    drawArtAt(row, col, ascii, color);
    cout.flush();
}

// 아스키아트 수평 이동 (startCol → endCol)
void moveArt(int row, int startCol, int endCol, vector<string>& ascii,
    const char* color, int speed, const char* color2) {

    int maxWidth = 0;
    for (int i = 0; i < (int)ascii.size(); i++) {
        if (displayWidth(ascii[i]) > maxWidth)
            maxWidth = displayWidth(ascii[i]);
    }
    int height = (int)ascii.size();

    int step = (startCol < endCol) ? 2 : -2;
    int clearLimit = endCol + 2;

    int col = startCol;
    int frame = 0;
    while (true) {
        if (step > 0 && col >= endCol) break;
        if (step < 0 && col <= endCol) break;

        int clearStart = col - abs(step);
        int clearW = maxWidth + abs(step) * 2;

        if (step > 0 && clearStart + clearW > clearLimit) {
            clearW = clearLimit - clearStart;
        }
        if (clearW > 0) {
            clearArea(row, clearStart, clearW, height);
        }

        // ★ color2 있으면 2색 교대, 없으면 단색
        const char* curColor = color;
        if (color2 != nullptr && frame % 2 == 1) {
            curColor = color2;
        }
        drawArtAt(row, col, ascii, curColor);

        Sleep(speed);
        col += step;
        frame++;
    }

    int clearStart = col - abs(step);
    int clearW = maxWidth + abs(step) * 2;
    if (step > 0 && clearStart + clearW > clearLimit) {
        clearW = clearLimit - clearStart;
    }
    if (clearW > 0) {
        clearArea(row, clearStart, clearW, height);
    }
    cout.flush();
}

// 슬롯명으로 원소 색상 반환
const char* getElemColor(string slotName) {
    if (slotName.find("불") != string::npos)  return RED;
    if (slotName.find("물") != string::npos)  return CYAN;
    if (slotName.find("흙") != string::npos)  return YELLOW;
    if (slotName.find("바람") != string::npos) return GREEN;
    return WHITE;
}

// 슬롯명으로 발사체 아트 반환
vector<string> getProjectile(string slotName) {

    // ── 불 ──
    if (slotName == "불") return {
        " ⟩⟩≻",
        "⟩⟩⟩≻≻",
        " ⟩⟩≻"
    };
    if (slotName == "불+") return {
        " ⟩⟩⟩≻",
        "⟩⟩⟩⟩≻≻",
        " ⟩⟩⟩≻"
    };
    if (slotName == "불++") return {
        " ⟩⟩⟩⟩≻",
        "⟩⟩⟩⟩⟩≻≻",
        "⟩⟩⟩⟩⟩≻≻",
        " ⟩⟩⟩⟩≻"
    };

    // ── 물 ──
    if (slotName == "물") return {
        " ≈≈∽",
        "≈≈≈∽∽",
        " ≈≈∽"
    };
    if (slotName == "물+") return {
        " ≈≈≈∽",
        "≈≈≈≈∽∽",
        " ≈≈≈∽"
    };
    if (slotName == "물++") return {
        " ≈≈≈≈∽",
        "≈≈≈≈≈∽∽",
        "≈≈≈≈≈∽∽",
        " ≈≈≈≈∽"
    };

    // ── 바람 ──
    if (slotName == "바람") return {
        " ∼∿⟫",
        "∼∿∼⟫⟫",
        " ∼∿⟫"
    };
    if (slotName == "바람+") return {
        " ∼∿∼∿⟫",
        "∼∿∼∿∼⟫⟫",
        " ∼∿∼∿⟫"
    };
    if (slotName == "바람++") return {
        " ∼∿∼∿∼⟫",
        "∼∿∼∿∼∿⟫⟫",
        "∼∿∼∿∼∿⟫⟫",
        " ∼∿∼∿∼⟫"
    };

    // ── 흙 ──
    if (slotName == "흙") return {
        " ▰▰▶",
        "▰▰▰▶▶",
        " ▰▰▶"
    };
    if (slotName == "흙+") return {
        " ▰▰▰▶",
        "▰▰▰▰▶▶",
        " ▰▰▰▶"
    };
    if (slotName == "흙++") return {
        " ▰▰▰▰▶",
        "▰▰▰▰▰▶▶",
        "▰▰▰▰▰▶▶",
        " ▰▰▰▰▶"
    };

    // 기본
    return { "▶" };
}

// 원소 발사체 공격 애니메이션 (슬라임→몬스터)
void animProjectile(Player& player, string slotName) {
    const char* color = getElemColor(slotName);
    vector<string> bullet = getProjectile(slotName);

    // 슬라임 색 변경
    clearArea(10, 5, 10, 3);
    gotoxy(10, 5); cout << color << "  .--." << CLR;
    gotoxy(11, 5); cout << color << " ( .. )" << CLR;
    gotoxy(12, 5); cout << color << "  '--'" << CLR;
    cout.flush();
    Sleep(150);

    // 발사체 이동
    int projectileRow = 10;
    int startCol = 14;
    int endCol = 52;

    moveArt(projectileRow, startCol, endCol, bullet, color, 25);

    // 착탄 이펙트
    vector<string> impact = { " *  ", "* * *", " *  " };
    drawArtAt(projectileRow, endCol - 2, impact, color);
    Sleep(150);
    clearArea(projectileRow, endCol - 3, 8, 3);
    cout.flush();

    // 슬라임 색 복구
    drawSlime(player);
    cout.flush();
}

// 몬스터 원소로 색상 반환
const char* getEnemyColor(string element) {
    if (element == "fire")  return RED;
    if (element == "water") return BLUE;
    if (element == "earth") return YELLOW;
    if (element == "wind")  return GREEN;
    return PINK; // boss (all)
}

// 몬스터 피격 애니메이션 (표정 교체 + 흔들림)
void animEnemyHit(Enemy& enemy) {
    const char* color = getEnemyColor(enemy.element);
    int row = 8;
    int col = 55;

    // 아스키아트 최대 너비 계산 (지울 영역용)
    int maxWidth = 0;
    for (int i = 0; i < (int)enemy.ascii.size(); i++) {
        if (displayWidth(enemy.ascii[i]) > maxWidth)
            maxWidth = displayWidth(enemy.ascii[i]);
    }
    int height = (int)enemy.ascii.size();

    // ascii_hit 있으면 피격 표정, 없으면 원본
    vector<string>& hitArt = (!enemy.ascii_hit.empty()) ? enemy.ascii_hit : enemy.ascii;

    // 피격 표정 교체 → 흔들기 → 원래 표정 복구
    clearArea(row, col - 4, maxWidth + 8, height);
    drawArtAt(row, col, hitArt, color);
    Sleep(100);
    shakeArt(row, col, hitArt, color, { 3, -3, 2, -2, 1, -1 }, 50);
    clearArea(row, col - 4, maxWidth + 8, height);
    drawArtAt(row, col, enemy.ascii, color);
    cout.flush();
}

// 슬라임 피격 애니메이션 (표정 교체 + 흔들림)
void animSlimeHit(Player& player) {
    const char* color = WHITE;
    int row = 10;
    int col = 5;

    vector<string> hitArt = {
        "  .--.",
        " (x.x)",
        "  '--'"
    };

    // 피격 표정 교체 → 흔들기 → 원래 표정 복구
    clearArea(row, col - 4, 14, 3);
    drawArtAt(row, col, hitArt, color);
    Sleep(100);
    shakeArt(row, col, hitArt, color, { 2, -2, 1, -1 }, 60);
    clearArea(row, col - 4, 14, 3);
    drawSlime(player);
    cout.flush();
}

void animSlimeDefend(Player& player) {
    const char* color = WHITE;

    // 방어 표정 변경
    clearArea(10, 5, 10, 3);
    gotoxy(10, 5); cout << color << "  .--." << CLR;
    gotoxy(11, 5); cout << color << " (>_<)" << CLR;
    gotoxy(12, 5); cout << color << "  '--'" << CLR;
    cout.flush();

    // 방패 표시
    vector<string> shield = {
        " [|] ",
        "[|||]",
        " [|] "
    };
    drawArtAt(10, 13, shield, YELLOW);
    Sleep(500);

    // 방패 제거
    clearArea(10, 13, 6, 3);
    cout.flush();
}

// 몬스터 원소로 공격 발사체 아트 반환
static vector<string> getEnemyProjectile(string element) {
    if (element == "fire") return {
        " ⟩⟩⟩ ≻",
        "⟩⟩⟩⟩ ≻≻",
        " ⟩⟩⟩ ≻"
    };
    if (element == "water") return {
        " ≈≈≈ ∽",
        "≈≈≈≈ ∽∽",
        " ≈≈≈ ∽"
    };
    if (element == "earth") return {
        " ▰▰▰ ▶",
        "▰▰▰▰ ▶▶",
        " ▰▰▰ ▶"
    };
    if (element == "wind") return {
        " ∼∿∼ ⟫",
        "∼∿∼∿ ⟫⟫",
        " ∼∿∼ ⟫"
    };
    // boss (all) - 보라색 전용
    return {
        " ◆◇◆ ►",
        "◆◇◆◇ ►►",
        " ◆◇◆ ►"
    };
}

// 몬스터 공격 애니메이션 (몬스터→슬라임)
void animEnemyAttack(Enemy& enemy) {
    const char* color = getEnemyColor(enemy.element);
    vector<string> bullet = getEnemyProjectile(enemy.element);

    // 몬스터 왼쪽 바깥(col 42)에서 출발 → 슬라임(col 12)
    moveArt(10, 42, 12, bullet, color, 25);

    // 착탄 이펙트
    vector<string> impact = { " *  ", "* * *", " *  " };
    drawArtAt(10, 10, impact, color);
    Sleep(150);
    clearArea(10, 8, 8, 3);
    cout.flush();

    // 몬스터 복구
    drawArtAt(8, 55, enemy.ascii, color);
    cout.flush();
}

// 몬스터 방어 이펙트
void animEnemyDefend(Enemy& enemy) {
    const char* color = getEnemyColor(enemy.element);

    vector<string> shield = {
        " [|] ",
        "[|||]",
        " [|] "
    };

    drawArtAt(9, 48, shield, color);
    Sleep(500);
    clearArea(9, 48, 6, 3);
    cout.flush();
}

// 몬스터 회복 이펙트
void animEnemyHeal(Enemy& enemy) {
    const char* color = getEnemyColor(enemy.element);

    vector<string> heal = {
        " + + ",
        "+ + +",
        " + + "
    };

    int col = 56;

    // + 기호 위에서 아래로 내려오기
    for (int r = 7; r <= 11; r += 2) {
        if (r > 7) clearArea(r - 2, col, 6, 3);
        drawArtAt(r, col, heal, GREEN);
        Sleep(180);
    }
    clearArea(11, col, 6, 3);

    // 몬스터 복구
    drawArtAt(8, 55, enemy.ascii, color);
    cout.flush();
}

// 몬스터 버프 이펙트
void animEnemyBuff(Enemy& enemy) {
    const char* color = getEnemyColor(enemy.element);

    vector<string> buff = {
        " ~ ~ ",
        "~ ~ ",
        " ~ ~ "
    };

    // 버프 표시 (2회 깜빡)
    for (int i = 0; i < 2; i++) {
        drawArtAt(9, 48, buff, color);
        Sleep(200);
        clearArea(9, 48, 6, 3);
        Sleep(100);
    }
    cout.flush();
}
// 몬스터 소멸 애니메이션 (글자 흩어짐 → 줄별 소멸)
void animEnemyDie(Enemy& enemy) {
    const char* color = getEnemyColor(enemy.element);
    int row = 8;
    int col = 55;
    int height = (int)enemy.ascii.size();

    vector<string> art = enemy.ascii;   // 원본 훼손 방지용 복사본

    int maxWidth = 0;
    for (int i = 0; i < height; i++) {
        if (displayWidth(art[i]) > maxWidth)
            maxWidth = displayWidth(art[i]);
    }

    // 글자 흩어짐 (3프레임)
    float percents[] = { 0.3f, 0.6f, 0.9f };

    for (int f = 0; f < 3; f++) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < (int)art[i].size(); j++) {
                if (art[i][j] != ' ' && (rand() % 100) < (int)(percents[f] * 100)) {
                    art[i][j] = (f < 2) ? '*' : ' ';
                }
            }
        }

        clearArea(row, col, maxWidth + 2, height);
        drawArtAt(row, col, art, color);
        Sleep(150);
    }

    // 줄별 소멸
    for (int i = 0; i < height; i++) {
        gotoxy(row + i, col);
        for (int j = 0; j < maxWidth; j++) cout << " ";
        cout.flush();
        Sleep(80);
    }

    cout.flush();
}
// 조합별 발사체 아트 + 2색 데이터
struct ComboData {
    vector<string> art;
    const char* color1;
    const char* color2;
};

static ComboData getComboData(string comboName) {
    if (comboName == "증기 폭발") return {
        { " ≈≈⟩⟩≈≈ ",
          "≈≈⟩⟩⟩⟩≈≈",
          " ≈≈⟩⟩≈≈ " },
        RED, CYAN
    };
    if (comboName == "용암 투척") return {
        { " ▰▰⟩⟩▰▰ ",
          "▰▰⟩⟩⟩⟩▰▰",
          " ▰▰⟩⟩▰▰ " },
        RED, YELLOW
    };
    if (comboName == "화염 돌풍") return {
        { " ∿∿⟩⟩∿∿ ",
          "∿∿⟩⟩⟩⟩∿∿",
          " ∿∿⟩⟩∿∿ " },
        RED, GREEN
    };
    if (comboName == "진흙 늪") return {
        { " ≈≈▰▰≈≈ ",
          "≈≈▰▰▰▰≈≈",
          " ≈≈▰▰≈≈ " },
        CYAN, YELLOW
    };
    if (comboName == "빙결 화살") return {
        { " ≈≈∿∿≈≈ ",
          "≈≈∿∿∿∿≈≈",
          " ≈≈∿∿≈≈ " },
        CYAN, GREEN
    };
    // 모래 폭풍
    return {
        { " ▰▰∿∿▰▰ ",
          "▰▰∿∿∿∿▰▰",
          " ▰▰∿∿▰▰ " },
        YELLOW, GREEN
    };
}

// 조합 공격 애니메이션 (2색 깜빡이며 발사 + 착탄)
void animComboAttack(Player& player, string comboName) {
    ComboData data = getComboData(comboName);

    // 슬라임 색 변경
    clearArea(10, 5, 10, 3);
    gotoxy(10, 5); cout << data.color1 << "  .--." << CLR;
    gotoxy(11, 5); cout << data.color1 << " ( ** )" << CLR;
    gotoxy(12, 5); cout << data.color1 << "  '--'" << CLR;
    cout.flush();
    Sleep(150);

    // ★ 이동 루프 전체를 moveArt 한 줄로 대체
    int row = 10;
    int endCol = 52;
    moveArt(row, 14, endCol, data.art, data.color1, 20, data.color2);

    // 착탄 이펙트
    vector<string> bigImpact = {
        "  \\  |  /  ",
        " -- *** -- ",
        "  /  |  \\  "
    };
    drawArtAt(row, endCol - 5, bigImpact, data.color1);
    Sleep(100);
    drawArtAt(row, endCol - 5, bigImpact, data.color2);
    Sleep(100);
    clearArea(row, endCol - 6, 14, 3);
    cout.flush();

    // 슬라임 복구
    drawSlime(player);
    cout.flush();
}


