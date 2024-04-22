#include "raylib.h"
#include <iostream>
using namespace std;

Color Green = Color{ 38, 185, 154, 255 };
Color DarkGreen = Color{ 20, 160, 133, 255 };
Color LightGreen = Color{ 129, 205, 133, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int playerScore = 0;
int cpuScore = 0;

bool gamePaused = false;

class Ball {
public:
    float x, y;
    float speedX, speedY;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        if (!gamePaused) {
            x += speedX;
            y += speedY;

            if (y + radius >= GetScreenHeight() || y - radius <= 0) {
                speedY *= -1;
            }
            if (x + radius >= GetScreenWidth() || x - radius <= 0) {
                speedX *= -1;
            }

            if (x + radius >= GetScreenWidth()) {
                cpuScore++;
                ResetBall();
            }

            if (x - radius <= 0) {
                playerScore++;
                ResetBall();
            }
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speedChoices[2] = { -1, 1 };
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitedMovement() {
        if (y < 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }
public:
    float x, y;
    float width, height;
    float speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
    }

    void Update() {
        if (!gamePaused) {
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                y -= speed;
            }
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                y += speed;
            }
            LimitedMovement();
        }
    }
};

class CPU_Paddle : public Paddle {
public:
    void Update(int ballY) {
        if (!gamePaused) {
            if (y + height / 2 > ballY) {
                y -= speed;
            }
            if (y + height / 2 < ballY) {
                y += speed;
            }
            LimitedMovement();
        }
    }

};

Ball ball;
Paddle player;
CPU_Paddle cpu;

int main() {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "pong");
    SetTargetFPS(60);

    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.radius = 20;
    ball.speedX = 7;
    ball.speedY = 7;

    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight / 2 - player.height / 2;
    player.speed = 10;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screenHeight / 2 - cpu.height / 2;
    cpu.speed = 10;

    while (!WindowShouldClose()) {
        DrawText("Use W/S or Up/Down arrow keys to move\nEsc for quit and p for pause", 10, 10, 10, Fade(WHITE, 0.5f));

        if (IsKeyPressed(KEY_P)) { // Press 'P' to pause/unpause the game
            gamePaused = !gamePaused;
        }

        BeginDrawing();
        ClearBackground(DarkGreen);
        DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, Green);
        DrawCircle(screenWidth / 2, screenHeight / 2, 150, LightGreen);

        ball.Update();
        player.Update();
        cpu.Update(ball.y);


        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) {
            ball.speedX *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
            ball.speedX *= -1;
        }


        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

        if (gamePaused) {
            DrawText("Paused. Press 'P' to resume.", screenWidth / 2 - MeasureText("Paused. Press 'P' to resume.", 20) / 2, screenHeight / 2, 20, WHITE);
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
    return 0;
}
