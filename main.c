#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

#define MAX_ITEMS 10

typedef struct {
    char name[50];
    int health, maxHealth, strength, lives;
    char items[MAX_ITEMS][50];
    int itemCount, hasJewel, day;
} Player;

Player player = {0};
int screen = 0, character = 0, enemyHealth = 0, combatRound = 0;

void AddItem(const char *item) {
    if (player.itemCount < MAX_ITEMS) strcpy(player.items[player.itemCount++], item);
}

int HasItem(const char *item) {
    for (int i = 0; i < player.itemCount; i++)
        if (strcmp(player.items[i], item) == 0) return 1;
    return 0;
}

void RemoveItem(const char *item) {
    for (int i = 0; i < player.itemCount; i++) {
        if (strcmp(player.items[i], item) == 0) {
            for (int j = i; j < player.itemCount - 1; j++) strcpy(player.items[j], player.items[j + 1]);
            player.itemCount--;
            break;
        }
    }
}

void ShowStatus() {
    DrawRectangle(50, 80, 350, 90, DARKGRAY);
    DrawText(TextFormat("Health: %d/%d", player.health, player.maxHealth), 60, 90, 25, RED);
    DrawText(TextFormat("Lives: %d | Day: %d", player.lives, player.day), 60, 120, 25, GREEN);
    DrawText(TextFormat("Items: %d", player.itemCount), 60, 145, 20, WHITE);
}

int main(void) {
    InitWindow(1000, 600, "Jumanji Adventure");
    SetTargetFPS(60);
    srand(time(NULL));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){20, 60, 20, 255});

        switch (screen) {
            case 0: // Start
                DrawText("JUMANJI", 250, 200, 100, GOLD);
                DrawText("Press SPACE to Start", 250, 290, 40, WHITE);
                if (IsKeyPressed(KEY_SPACE)) screen = 1;
                break;

            case 1: // Character Select
                DrawText("Choose Your Character", 230, 50, 45, WHITE);
                DrawText("1 - Dr. Bravestone (Warrior)", 200, 180, 30, LIGHTGRAY);
                DrawText("2 - Ruby Roundhouse (Fighter)", 200, 240, 30, LIGHTGRAY);
                DrawText("3 - Prof. Oberon (Scholar)", 200, 300, 30, LIGHTGRAY);
                DrawText("4 - Franklin Finbar (Expert)", 200, 360, 30, LIGHTGRAY);
                
                if (IsKeyPressed(KEY_ONE)) {
                    character = 1; strcpy(player.name, "Bravestone");
                    player.health = player.maxHealth = 120; player.strength = 50; player.lives = 3; player.day = 1;
                    screen = 10;
                } else if (IsKeyPressed(KEY_TWO)) {
                    character = 2; strcpy(player.name, "Ruby");
                    player.health = player.maxHealth = 90; player.strength = 40; player.lives = 3; player.day = 1;
                    screen = 10;
                } else if (IsKeyPressed(KEY_THREE)) {
                    character = 3; strcpy(player.name, "Oberon");
                    player.health = player.maxHealth = 70; player.strength = 20; player.lives = 3; player.day = 1;
                    AddItem("Ancient Map"); screen = 10;
                } else if (IsKeyPressed(KEY_FOUR)) {
                    character = 4; strcpy(player.name, "Franklin");
                    player.health = player.maxHealth = 80; player.strength = 25; player.lives = 3; player.day = 1;
                    AddItem("Rope"); AddItem("Knife"); screen = 10;
                }
                break;

            case 10: // Day 1
                DrawText("DAY 1 - JUNGLE ENTRANCE", 200, 20, 40, WHITE);
                ShowStatus();
                DrawText("Three paths lie ahead:", 50, 200, 35, WHITE);
                DrawText("1 - DARK PATH (Dangerous)", 50, 260, 28, LIGHTGRAY);
                DrawText("2 - RIVER PATH (Water)", 50, 310, 28, LIGHTGRAY);
                DrawText("3 - SUNLIT PATH (Peaceful)", 50, 360, 28, LIGHTGRAY);
                DrawText("Choose 1, 2, or 3", 50, 450, 30, YELLOW);
                
                if (IsKeyPressed(KEY_ONE)) { enemyHealth = 60; combatRound = 1; screen = 11; }
                else if (IsKeyPressed(KEY_TWO)) screen = 12;
                else if (IsKeyPressed(KEY_THREE)) screen = 13;
                break;

            case 11: // Combat
                DrawText("JAGUAR ATTACK!", 300, 20, 45, RED);
                ShowStatus();
                DrawText(TextFormat("Enemy Health: %d", enemyHealth), 50, 200, 35, RED);
                DrawText("1 - Strong Attack", 50, 280, 28, WHITE);
                DrawText("2 - Quick Attack", 50, 330, 28, WHITE);
                DrawText("3 - Defend", 50, 380, 28, WHITE);
                
                if (IsKeyPressed(KEY_ONE)) {
                    int dmg = player.strength + rand() % 25;
                    enemyHealth -= dmg;
                    if (enemyHealth > 0) player.health -= 20;
                    if (enemyHealth <= 0) { AddItem("Fang"); screen = 14; }
                    if (player.health <= 0) {
                        player.lives--; player.health = player.maxHealth;
                        screen = (player.lives > 0) ? 10 : 100;
                    }
                } else if (IsKeyPressed(KEY_TWO)) {
                    enemyHealth -= player.strength;
                    if (enemyHealth > 0) player.health -= 10;
                    if (enemyHealth <= 0) { AddItem("Fang"); screen = 14; }
                    if (player.health <= 0) {
                        player.lives--; player.health = player.maxHealth;
                        screen = (player.lives > 0) ? 10 : 100;
                    }
                } else if (IsKeyPressed(KEY_THREE)) {
                    enemyHealth -= 10; player.health -= 5;
                    if (enemyHealth <= 0) { AddItem("Fang"); screen = 14; }
                }
                break;

            case 12: // River
                DrawText("RIVER CROSSING", 300, 20, 45, BLUE);
                ShowStatus();
                DrawText("A blue healing flower on far shore", 50, 220, 30, WHITE);
                DrawText("1 - Jump stones (Risky)", 50, 300, 28, LIGHTGRAY);
                DrawText("2 - Swim (Safe)", 50, 350, 28, LIGHTGRAY);
                
                if (IsKeyPressed(KEY_ONE)) {
                    if (character == 1 || character == 2) {
                        AddItem("Heal Flower"); player.health = player.maxHealth; screen = 14;
                    } else { player.health -= 25; screen = 14; }
                } else if (IsKeyPressed(KEY_TWO)) {
                    player.health -= 15; screen = 14;
                }
                break;

            case 13: // Sunlit
                DrawText("SUNLIT CLEARING", 280, 20, 45, YELLOW);
                ShowStatus();
                if (character == 3) {
                    DrawText("Heat damages you! -30 HP", 150, 250, 35, RED);
                    DrawText("Press SPACE", 350, 400, 30, WHITE);
                    if (IsKeyPressed(KEY_SPACE)) { player.health -= 30; screen = 14; }
                } else {
                    DrawText("Peaceful! +FULL HEALTH", 200, 250, 35, GREEN);
                    DrawText("Press SPACE", 350, 400, 30, WHITE);
                    if (IsKeyPressed(KEY_SPACE)) { player.health = player.maxHealth; AddItem("Fruit"); screen = 14; }
                }
                break;

            case 14: // Night transition
                DrawText("Night falls...", 300, 250, 50, WHITE);
                DrawText("Press SPACE for Day 2", 250, 350, 35, GRAY);
                if (IsKeyPressed(KEY_SPACE)) { player.day = 2; screen = 20; }
                break;

            case 20: // Day 2
                DrawText("DAY 2 - RUINS", 300, 20, 45, WHITE);
                ShowStatus();
                DrawText("Ancient gate with symbols", 50, 220, 35, WHITE);
                DrawText("1 - Solve puzzle (Smart)", 50, 300, 28, LIGHTGRAY);
                DrawText("2 - Force open (Strong)", 50, 350, 28, LIGHTGRAY);
                DrawText("3 - Find passage", 50, 400, 28, LIGHTGRAY);
                
                if (IsKeyPressed(KEY_ONE)) {
                    if (character == 3 || HasItem("Ancient Map")) {
                        AddItem("Compass"); screen = 21;
                    } else { player.health -= 20; screen = 21; }
                } else if (IsKeyPressed(KEY_TWO)) {
                    if (character == 1) { AddItem("Gold Key"); screen = 21; }
                    else { player.health -= 20; screen = 21; }
                } else if (IsKeyPressed(KEY_THREE)) screen = 21;
                break;

            case 21: // Elephant
                DrawText("ELEPHANT BLOCKS PATH", 200, 20, 45, WHITE);
                ShowStatus();
                DrawText("Massive angry elephant!", 50, 220, 35, WHITE);
                DrawText("1 - Attack it", 50, 300, 28, LIGHTGRAY);
                DrawText("2 - Calm it down", 50, 350, 28, LIGHTGRAY);
                DrawText("3 - Go around", 50, 400, 28, LIGHTGRAY);
                
                if (IsKeyPressed(KEY_ONE)) { player.health -= 40; if (player.health <= 0) { player.lives--; player.health = player.maxHealth; screen = (player.lives > 0) ? 21 : 100; } }
                else if (IsKeyPressed(KEY_TWO)) {
                    if (character == 4 || HasItem("Fruit")) {
                        if (HasItem("Fruit")) RemoveItem("Fruit");
                        AddItem("Temple Map"); screen = 22;
                    } else { player.health -= 20; }
                } else if (IsKeyPressed(KEY_THREE)) screen = 22;
                break;

            case 22: // Night 2
                DrawText("Night falls again...", 270, 250, 50, WHITE);
                DrawText("Press SPACE for Day 3", 250, 350, 35, GRAY);
                if (IsKeyPressed(KEY_SPACE)) { player.day = 3; screen = 30; }
                break;

            case 30: // Day 3 Temple
                DrawText("DAY 3 - JAGUAR TEMPLE!", 180, 20, 45, GOLD);
                ShowStatus();
                DrawText("The legendary temple!", 50, 220, 35, WHITE);
                DrawText("Three trials await...", 50, 280, 30, GRAY);
                DrawText("Press SPACE to enter", 50, 400, 35, YELLOW);
                if (IsKeyPressed(KEY_SPACE)) { enemyHealth = 100; screen = 31; }
                break;

            case 31: // Boss Fight
                DrawText("TRIAL: STONE GUARDIAN!", 180, 20, 45, RED);
                ShowStatus();
                DrawText(TextFormat("BOSS HP: %d", enemyHealth), 50, 200, 40, RED);
                DrawText("1 - Power Strike", 50, 280, 28, WHITE);
                DrawText("2 - Fast Combo", 50, 330, 28, WHITE);
                DrawText("3 - Use Item", 50, 380, 28, WHITE);
                
                if (IsKeyPressed(KEY_ONE)) {
                    int dmg = player.strength + 30;
                    enemyHealth -= dmg; if (enemyHealth > 0) player.health -= 25;
                    if (enemyHealth <= 0) screen = 32;
                    if (player.health <= 0) { player.lives--; player.health = player.maxHealth; screen = (player.lives > 0) ? 31 : 100; }
                } else if (IsKeyPressed(KEY_TWO)) {
                    enemyHealth -= player.strength + 15; if (enemyHealth > 0) player.health -= 15;
                    if (enemyHealth <= 0) screen = 32;
                    if (player.health <= 0) { player.lives--; player.health = player.maxHealth; screen = (player.lives > 0) ? 31 : 100; }
                } else if (IsKeyPressed(KEY_THREE)) {
                    if (HasItem("Heal Flower")) { RemoveItem("Heal Flower"); player.health = player.maxHealth; }
                    if (HasItem("Fang")) { RemoveItem("Fang"); enemyHealth -= 60; if (enemyHealth <= 0) screen = 32; }
                }
                break;

            case 32: // Riddle
                DrawText("TRIAL: WISDOM", 300, 20, 45, BLUE);
                ShowStatus();
                DrawText("'I have cities but no houses,", 100, 200, 28, WHITE);
                DrawText("forests but no trees,", 100, 240, 28, WHITE);
                DrawText("water but no fish. What am I?'", 100, 280, 28, WHITE);
                DrawText("1 - Dream", 100, 350, 28, LIGHTGRAY);
                DrawText("2 - MAP", 100, 390, 28, LIGHTGRAY);
                DrawText("3 - Painting", 100, 430, 28, LIGHTGRAY);
                
                if (IsKeyPressed(KEY_ONE)) { player.health -= 30; }
                else if (IsKeyPressed(KEY_TWO)) screen = 33;
                else if (IsKeyPressed(KEY_THREE)) { player.health -= 30; }
                break;

            case 33: // Final
                DrawText("THE JAGUAR'S EYE!", 200, 100, 60, GOLD);
                ShowStatus();
                DrawText("You found the legendary jewel!", 100, 280, 35, WHITE);
                DrawText("As you grab it, the temple shakes!", 80, 340, 30, YELLOW);
                DrawText("Press SPACE to ESCAPE!", 200, 450, 40, RED);
                if (IsKeyPressed(KEY_SPACE)) { player.hasJewel = 1; screen = 90; }
                break;

            case 90: // Victory
                DrawText("VICTORY!", 350, 150, 70, GOLD);
                DrawText(TextFormat("You escaped as %s!", player.name), 200, 260, 35, WHITE);
                DrawText(TextFormat("Days Survived: %d", player.day), 250, 320, 30, GREEN);
                DrawText(TextFormat("Health Remaining: %d", player.health), 230, 370, 30, GREEN);
                DrawText("You conquered Jumanji!", 220, 450, 40, YELLOW);
                DrawText("Press ESC to exit", 300, 520, 25, GRAY);
                break;

            case 100: // Game Over
                DrawText("GAME OVER", 300, 200, 70, RED);
                DrawText("Jumanji claimed another soul...", 180, 320, 35, GRAY);
                DrawText("Press ESC to exit", 300, 420, 30, LIGHTGRAY);
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


