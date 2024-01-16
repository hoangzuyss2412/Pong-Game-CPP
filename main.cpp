#include <iostream>
#include <raylib.h>


Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

using namespace std;

int player_score = 0;
int computer_score = 0;

class Ball{
    public:
        float x;
        float y;
        int speed_x;
        int speed_y;
        int radius;

        Ball(float x, float y, float speed_x, float speed_y, int radius){
            this->x = x;
            this->y = y;
            this->speed_x = speed_x;
            this->speed_y = speed_y;
            this->radius = radius;
        }
        void Draw(){
            DrawCircle(x, y, radius, Yellow);
        }

        void Update(){
            x += speed_x;
            y += speed_y;

            // Change Y-direction if the ball exceeds Y-boundary 
            if(y >= GetScreenHeight() - radius || y <= radius){
                speed_y *= -1;
            }

            // Change X-direction if the ball exceeds X-boundary 
            if(x >= GetScreenWidth() - radius ){
                computer_score++;
                ResetBall();
            }

            if(x <= radius){
                player_score++;
                ResetBall();
            }
        }

        void ResetBall(){
            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;
            int speed_choices[2] = {-1, 1};
            speed_x *= speed_choices[GetRandomValue(0,1)];
            speed_y *= speed_choices[GetRandomValue(0,1)];
        }
};

class Paddle{
    public:
        float x;
        float y;
        float width;
        float height;
        int speed;

        Paddle(float x, float y, float width, float height, int speed){
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->speed = speed;
        }

        void Draw(){
            DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 12, WHITE);
        }
        
        void Update(){
            if(IsKeyDown(KEY_UP)){
                y -= speed;
            }

            if(IsKeyDown(KEY_DOWN)){
                y += speed;
            }

            // Make sure the paddle doesn't go outside the window
            LimitMovement();
        }

    protected:
        void LimitMovement(){
            if(y <= 0){
                y = 0;
            }
            if(y >= GetScreenHeight() - height){
                y = GetScreenHeight() - height;
            }
        }

};

class ComputerPaddle : public Paddle{
    public:
        ComputerPaddle(float x, float y, float width, float height, int speed) 
        : Paddle(x, y, width, height, speed) {

        }

        void Update(int ball_y){
            if(y > ball_y - height/2){
                y -= speed;
            }

            if(y <= ball_y - height/2){
                y += speed;
            }

            LimitMovement();
        }
};

int main () {

    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    const int paddle_width = 25;
    const int paddle_height = 120;
    const int paddle_speed = 6;
    const int offset = 10;
    const int fontsize = 80;
    
    // Initilize ball object, player and computer's paddle 
    Ball ball(screen_width/2, screen_height/2, 7, 7, 20);
    Paddle player(screen_width - offset - paddle_width, screen_height/2 - paddle_height/2, paddle_width, paddle_height, paddle_speed);
    ComputerPaddle computer(offset, screen_height/2 - paddle_height/2, paddle_width, paddle_height, paddle_speed);

    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);

    while(WindowShouldClose() == false){
        BeginDrawing();
        // Updating 
        ball.Update();
        player.Update();
        computer.Update(ball.y);

        // Checking for collision 
            if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
                ball.speed_x *= -1;
            }

            if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{computer.x, computer.y, computer.width, computer.height})){
                ball.speed_x *= -1;
            }

        // Drawing 
        ClearBackground(Dark_Green);
        DrawRectangle(0, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, screen_width/10, Light_Green);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();           
        computer.Draw();       // Computer's Paddle
        player.Draw();         // Player's Paddle
        DrawText(TextFormat("%i", computer_score), screen_width/4 - 20, 20, fontsize, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width/4 - 20, 20, fontsize, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
