#include "C:/raylib/raylib/src/raylib.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

struct platform{
    int platformType;
    Vector2 position;
    platform(int pT, Vector2 p){
        platformType=pT;position=p;
    }
};
std::vector<platform> platforms={};
void addPlatform(int pT, int x, int y){
    platforms.emplace_back(platform(pT, Vector2{x, y}));
};
int main(){
    std::ifstream levelFile("level.txt");
    if (!levelFile.is_open()) {
        return 1;
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
    InitWindow(1024,576,"Platformer");
    SetTargetFPS(60);
    Texture danger=LoadTexture("assets/danger.png");
    Texture block=LoadTexture("assets/block.png");
    Texture player=LoadTexture("assets/player.png");
    int scroll=0;
    double speed=64;
    while(!WindowShouldClose()){
        ClearBackground(RAYWHITE);
        double dt=GetFrameTime();
        if(IsKeyDown(KEY_D)){scroll+=speed*dt;}
        if(IsKeyDown(KEY_A)){scroll-=speed*dt;}
        scroll=std::max(0,scroll);
        BeginDrawing();
            for(const platform& plat : platforms){
                if(plat.position.x-scroll/4>(-1) && plat.position.x-scroll<16){
                    DrawTexture(plat.platformType==1?danger:block,(plat.position.x-scroll)*64,plat.position.y*64,WHITE);
                };
            };
        EndDrawing();
    }
    UnloadTexture(danger);
    UnloadTexture(block);
    UnloadTexture(player);
    levelFile.close();
}