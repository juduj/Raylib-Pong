#include <iostream>
#include <raylib.h>
#include <string>
#include <random>

struct Ball {
    int x;
    int y;
    int radius;
    int speedX;
    int speedY;

    Ball(int startX, int startY, int startRadius, int startSpeedX, int startSpeedY) {
        x = startX;
        y = startY;
        radius = startRadius;
        speedX = startSpeedX;
        speedY = startSpeedY;
    }
};

struct Paddle
{
    int x;
    int y;
    int radius;
    int speedX;
    int speedY;

    int width = 15;
    int height = 100;

    float aiDelay;

    Paddle(int startX, int startY, int startRadius, int speedX_, int speedY_) {
        x = startX;
        y = startY;
        radius = startRadius;
        speedX = speedX_;
        speedY = speedY_;

        aiDelay = 0;
    }

    Paddle(int startX, int startY, int startRadius, int speedX_, int speedY_, float aiDelay_) {
        x = startX;
        y = startY;
        radius = startRadius;
        speedX = speedX_;
        speedY = speedY_;
        aiDelay = aiDelay_;
    }
};

void HandleEventInput(Paddle& paddle)
{
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        paddle.y -= 5;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        paddle.y += 5;
    }
}

float AiDelayFactor()
{
    // Use a random device to generate a seed
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister pseudo-random number generator
    
    // Define the range for the floating-point number
    std::uniform_real_distribution<float> dis(0.2f, 0.8f); // the smaller the smarter the ai plays.

    return dis(gen);
}


int main()
{
    //black for retro pong background.
    Color backgroundColor = Color{ 0, 0, 0, 255 };

    const int screenWidth = 800;
    const int screenHeight = 800;

    int score = 0;

    Ball ball(100, 100, 10, 5, 5);

    Paddle paddle0(screenWidth / 2 - 300, screenHeight / 2, 15, 5, 5);
    
    //ai
    Paddle paddle1(screenWidth / 2 + 300, screenHeight / 2, 15, 5, 5, AiDelayFactor());


    // Init audio devices so we can load sounds
    InitAudioDevice();

    Sound sound_ballCollision = LoadSound("..//resources//ball_collision2.wav");
    Sound sound_ballCollisionWall = LoadSound("..//resources//ball_collision.wav");


    std::cout << "Hello, World!" << std::endl;

    InitWindow(screenWidth, screenHeight, "Pong");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(backgroundColor);
        HandleEventInput(paddle0);

        ball.x += ball.speedX;
        ball.y += ball.speedY ;


        // Check if the ball went past paddle0's goal
        if (ball.x - ball.radius <= 0) {
            // Player 2 (paddle1) scores
            // Update player 2's score and reset the ball
            // You can use a variable to keep track of the score for each player
            // For example:
            // player2Score++;
            // Reset ball position
            ball.x = screenWidth / 2;
            ball.y = screenHeight / 2;
            // Reset ball movement direction (choose initial direction)
            // This can be randomized or set to a default direction
            ball.speedX = 5;
            ball.speedY = 5;

            paddle1.aiDelay = AiDelayFactor();
            std::cout << paddle1.aiDelay << std::endl;
        }

        // Check if the ball went past paddle1's goal
        if (ball.x + ball.radius >= screenWidth) {
            // Player 1 (paddle0) scores
            // Update player 1's score and reset the ball
            // For example:
            // player1Score++;
            // Reset ball position
            ball.x = screenWidth / 2;
            ball.y = screenHeight / 2;
            // Reset ball movement direction (choose initial direction)
            // This can be randomized or set to a default direction
            ball.speedX = -5;
            ball.speedY = -5;

            score++;

            paddle1.aiDelay = AiDelayFactor();
            std::cout << paddle1.aiDelay << std::endl;
        }

        
        if (ball.y + ball.radius >= screenHeight || ball.y - ball.radius <= 0)
        {
            ball.speedY *= -1;
            PlaySound(sound_ballCollisionWall);
        }

        // Check collision with paddle 0
        if ((ball.x - ball.radius <= paddle0.x + paddle0.width) &&
            (ball.y >= paddle0.y && ball.y <= paddle0.y + paddle0.height)) {
            ball.speedX *= -1; // Reverse ball's X direction on collision
            PlaySound(sound_ballCollision);
        }

        // Check collision with paddle 1
        if ((ball.x + ball.radius >= paddle1.x) &&
            (ball.y >= paddle1.y && ball.y <= paddle1.y + paddle1.height)) {
            ball.speedX *= -1; // Reverse ball's X direction on collision
            PlaySound(sound_ballCollision);
        }

        //ai code
        if (paddle1.y + paddle1.height / 2 < ball.y - paddle1.aiDelay * paddle1.height) {
            paddle1.y += paddle1.speedY;
        }
        else if (paddle1.y + paddle1.height / 2 > ball.y + paddle0.aiDelay * paddle1.height) {
            paddle1.y -= paddle1.speedY;
        }


        DrawCircle(ball.x, ball.y, ball.radius, WHITE);
        DrawRectangle(paddle0.x, paddle0.y, paddle0.width, paddle0.height, WHITE);
        DrawRectangle(paddle1.x, paddle1.y, paddle1.width, paddle1.height, WHITE);
        DrawText(std::to_string(score).c_str(), screenWidth / 2 - 36, screenHeight / 2 - 380, 36, WHITE);


        EndDrawing();
    }

   
    UnloadSound(sound_ballCollision);     // Unload sound data
    UnloadSound(sound_ballCollisionWall);     // Unload sound data

    CloseAudioDevice();     // Close audio device

    CloseWindow();
    return 0;
}
