#pragma once
#include <vector>
#include <cmath>
#include "../scratch/collision/collisionShape.hpp"
extern CollisionPoint debugPoints[COLLISION_POINT_COUNT*2];

class Render{
public:
    
    static void Init();
    static void deInit();
    static void renderSprites();
    static bool appShouldRun();

};

class LoadingScreen{
private:
    struct squareObject{
        float x,y;
        float size;
    };
    std::vector<squareObject> squares;
    void createSquares(int count) {
        for (int i = 0; i < count; i++) {
            squareObject square;
            square.x = rand() % 400;
            square.y = 260;
            square.size = rand() % 20;
            squares.push_back(square);
        }
    }
public:
    //TextObject* text;
    void init();
    void renderLoadingScreen();
    void cleanup();
};
