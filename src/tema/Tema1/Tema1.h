#pragma once

#include "components/simple_scene.h"

// !!! SCHELET FOLOSIT: LABORATOR_3 -> lab3.h
namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        bool CheckCollisionPOBE(float moveX, float moveY);
        bool CheckCollisionEB(int enemyIdx, float moveX, float moveY);
        void EnemyIsMoving(float deltaTime, int enemyIdx);
        void AddNewEnemy();

     protected:
        glm::mat3 modelMatrix;
        float cxPlayer, cyPlayer;
        int scaleX, scaleY;
        float angularStep;
        int backgroundLength;
        int headRadius;
        int armRadius;
        int obstacleLength;
        float backgroundLengthX, backgroundLengthY;
        float backgroundLeftCornerX, backgroundLeftCornerY;
        float startX, startY;
        struct Obstacle {
            float obsX;
            float obsY;
            int scaleObsX;
            int scaleObsY;
        };
        struct Enemy {
            float enmX;
            float enmY;
            float angle;
            float speed;
        };
        float left, right, bottom, top;
        std::vector<Obstacle> obstacles;
        Obstacle obs1, obs2, obs3, obs4, obs5;
        std::vector<Enemy> enemies;
        bool leftClick;
        int count;
        int dist;
        int nrOfEnemies;
        float spamTime;
        float projectilleAngle;
        float xProjectille, yProjectille;
        int xProjLength;
        int yProjLength;
    };
}   // namespace m1
