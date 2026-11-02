#include "C:/raylib/raylib/src/raylib.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

struct platform{
    int platformType;
    Vector2 position;
    platform(int pT, Vector2 p){
        platformType=pT;position=p;
    }
};
std::vector<platform> platforms={};
void addPlatform(int pT, int x, int y){
    platforms.emplace_back(platform(pT, Vector2{static_cast<float>(x), static_cast<float>(y)}));
};
void openFileAndRead(const std::string& filename) {
    std::ifstream levelFile(filename);
    if (!levelFile.is_open()) {
        return;
    }
    int pT;int x; int y;
    std::string line;
    while (std::getline(levelFile,line)){
        std::replace(line.begin(), line.end(), ',', ' ');
        line.erase(line.length() - 1, 1);
        line.erase(0, 1);
        std::stringstream ss(line);
        std::string smallInt;
        ss >> pT >> x >> y;
        addPlatform(pT,x,y);
    }
    levelFile.close();
}
    double scroll;
    Vector2 playerY;
    bool onGround;
    bool gameOver;
    bool gameWin;
void reset(){
    platforms.clear();
    scroll=0;
    playerY={0,0};
    onGround=false;
    gameOver=false;
    gameWin=false;
}
int main(){
    reset();
    openFileAndRead("level1.txt");
    InitWindow(1024,576,"Platformer");
    SetTargetFPS(60);
    Texture danger=LoadTexture("assets/danger.png");
    Texture block=LoadTexture("assets/block.png");
    Texture player=LoadTexture("assets/player.png");
    Texture end=LoadTexture("assets/end.png");
    double speed=4;
    double g=98.1*2;
    Vector2 levels={1,2};
    while(!WindowShouldClose()){
        ClearBackground(RAYWHITE);
        double dt=GetFrameTime();
        if(!gameOver && !gameWin){
            if(IsKeyPressed(KEY_W) && onGround){playerY.y=120.0*2;onGround=false;}
            playerY.y-=g*dt;
            playerY.x-=playerY.y*dt;
            if(IsKeyDown(KEY_D)){scroll+=speed*dt;}
            if(IsKeyDown(KEY_A)){scroll-=speed*dt;}
        }
        if (!gameWin && playerY.x>576){
            gameOver=true;
        }
        for (const platform& plat : platforms){
            Rectangle playerRect = {8*64, (float)playerY.x, 64, 64};
            Rectangle platformRect = {
                (float)((plat.position.x-scroll)*64),
                (float)(plat.position.y*64),
                64,
                64
            };
            if (plat.platformType == 1) {
                constexpr float dangerInset = 1.0f;
                Rectangle dangerRect = {
                    platformRect.x + dangerInset,
                    platformRect.y + dangerInset,
                    platformRect.width - dangerInset * 2,
                    platformRect.height - dangerInset * 2
                };
                if (CheckCollisionRecs(playerRect, dangerRect)) {
                    gameOver = true;
                    break;
                }
                continue;
            }
            if (plat.platformType == 3) {
                if (CheckCollisionRecs(playerRect, platformRect)) {
                    gameWin = true;
                    break;
                }
                continue;
            }
            if(CheckCollisionRecs(playerRect, platformRect)){
                if((plat.position.y-0.8)*64>playerY.x){
                    playerY.y=0.0;
                    playerY.x=plat.position.y*64-64;
                    onGround=true;
                }
                else{
                    if(IsKeyDown(KEY_D)){scroll-=speed*dt;}
                    if(IsKeyDown(KEY_A)){scroll+=speed*dt;}
                }
            }
        };
        BeginDrawing();
            for(const platform& plat : platforms){
                if(plat.position.x-scroll>(-1) && plat.position.x-scroll<16){
                    DrawTexture(plat.platformType==1?danger:plat.platformType==2?block:end,(plat.position.x-scroll)*64,plat.position.y*64,WHITE);
                };
            };
            DrawTexture(player,8*64,float(playerY.x),WHITE);
            if (gameOver) {
                DrawRectangle(0, 0, 1024, 576, Fade(BLACK, 0.65f));
                DrawText("YOU LOSE", 410, 250, 40, RED);
                DrawText("Press SPACE to continue", 300, 300, 20, WHITE);
                if (IsKeyPressed(KEY_SPACE)) {
                    reset();
                    openFileAndRead("level" + std::to_string(int(levels.x)) + ".txt");
                }
            }
            if (gameWin && levels.x == levels.y) {
                DrawRectangle(0, 0, 1024, 576, Fade(BLACK, 0.65f));
                DrawText("YOU WIN", 410, 250, 40, GREEN);
            }else if (gameWin && levels.x < levels.y) {
                DrawRectangle(0, 0, 1024, 576, Fade(BLACK, 0.65f));
                DrawText("LEVEL COMPLETE", 350, 250, 40, GREEN);
                DrawText("Press SPACE to continue", 300, 300, 20, WHITE);
                if (IsKeyPressed(KEY_SPACE)) {
                    levels.x++;
                    reset();
                    openFileAndRead("level" + std::to_string(int(levels.x)) + ".txt");
                }
            }
        EndDrawing();
    }
    UnloadTexture(danger);
    UnloadTexture(block);
    UnloadTexture(player);
    UnloadTexture(end);
} 