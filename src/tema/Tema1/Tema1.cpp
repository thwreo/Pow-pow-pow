#include "tema/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "./transform.h"
#include "./Obj2D.h"

using namespace std;
using namespace m1;
// !!! SCHELET FOLOSIT: LABORATOR_3 -> lab3.cpp
Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    left = 0.2f * (float)resolution.x;
    right = 0.8f * (float)resolution.x;
    bottom = 0.2f * (float)resolution.y;
    top = 0.8f * (float)resolution.y;
    auto camera = GetSceneCamera();
    camera->SetOrthographic(left, right, bottom, top, 0.01f, 400);
    //camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    startX = resolution.x / 2;
    startY = resolution.y / 2;
    
    backgroundLengthX = 1.45f * resolution.x;
    backgroundLengthY = 1.45f * resolution.y;
    backgroundLeftCornerX = -0.45f * resolution.x / 2.f;
    backgroundLeftCornerY = -0.45f * resolution.y / 2.f;

    headRadius = 20;
    armRadius = 5;

    cxPlayer = 0;
    cyPlayer = 0;

    angularStep = 0;

    scaleX = 20;
    scaleY = 20;
    
    leftClick = false;

    obs1.obsX = 20;
    obs1.obsY = 20;
    obs1.scaleObsX = 100;
    obs1.scaleObsY = 200;
    obstacles.push_back(obs1);

    obs2.obsX = 400;
    obs2.obsY = 470;
    obs2.scaleObsX = 50;
    obs2.scaleObsY = 400;
    obstacles.push_back(obs2);
    
    obs3.obsX = 400;
    obs3.obsY = 450;
    obs3.scaleObsX = 250;
    obs3.scaleObsY = 20;
    obstacles.push_back(obs3);

    obs4.obsX = resolution.x - 30;
    obs4.obsY = -50;
    obs4.scaleObsX = 300;
    obs4.scaleObsY = 200;
    obstacles.push_back(obs4);

    obs5.obsX = 400;
    obs5.obsY = 20;
    obs5.scaleObsX = 150;
    obs5.scaleObsY = 200;
    obstacles.push_back(obs5);

    obstacleLength = 1;

    nrOfEnemies = 3;
    spamTime = 0;

    // FOR TESTING
    //enm1.enmX = 175;
    //enm1.enmY = 300;
    //enm1.angle = 0;
    //enm1.speed = 112;
    //enemies.push_back(enm1);

    //enm2.enmX = resolution.x - 100;
    //enm2.enmY = resolution.y - 160;
    //enm2.angle = 0;
    //enm2.speed = 250;
    //enemies.push_back(enm2);

    //enm3.enmX = resolution.x / 2 + 50;
    //enm3.enmY = 30;
    //enm3.angle = 0;
    //enm3.speed = 300;
    //enemies.push_back(enm3);

    for (int i = 0; i < nrOfEnemies; i++) {
        Enemy enm;
        enm.enmX = startX;
        enm.enmY = startY;
        while (enm.enmX == startX || enm.enmY == startY) {
            enm.enmX = (rand() % (resolution.x - headRadius));
            enm.enmY = (rand() % (resolution.y - headRadius));
        }
        enm.angle = 0;
        enm.speed = 112 + (rand() % 50) * i; // 112 = playerSpeed
        enemies.push_back(enm);
    }

    xProjLength = 20;
    yProjLength = 30;
    dist = 50;

    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 backgroundCorner = glm::vec3(backgroundLeftCornerX, backgroundLeftCornerY, 0);

    Mesh* playerHead = Obj2D::CreateCirlce("playerHead", corner, headRadius, glm::vec3(0, 0, 1), true);
    AddMeshToList(playerHead);

    Mesh* playerLeftArm = Obj2D::CreateCirlce("playerLeftArm", corner, armRadius, glm::vec3(0, 1, 0), true);
    AddMeshToList(playerLeftArm);

    Mesh* playerRightArm = Obj2D::CreateCirlce("playerRightArm", corner, armRadius, glm::vec3(0, 1, 0), true);
    AddMeshToList(playerRightArm);

    Mesh* enemyHead = Obj2D::CreateCirlce("enemyHead", corner, headRadius, glm::vec3(1, 0, 0), true);
    AddMeshToList(enemyHead);

    Mesh* enemyLeftArm = Obj2D::CreateCirlce("enemyLeftArm", corner, armRadius, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemyLeftArm);

    Mesh* enemyRightArm = Obj2D::CreateCirlce("enemyRightArm", corner, armRadius, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemyRightArm);

    Mesh* obstacle = Obj2D::CreateSquare("obstacle", corner, 1, glm::vec3(0.90, 0.1, 0.4), true);
    AddMeshToList(obstacle);

    Mesh* background = Obj2D::CreateSquare("background", corner, 1, glm::vec3(0.33, 0.45, 0.1111), true);
    AddMeshToList(background);

    Mesh* projectille = Obj2D::CreateSquare("projectille", corner, 1, glm::vec3(0.9, 0.9, 0.9), true);
    AddMeshToList(projectille);
}



void Tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    auto camera = GetSceneCamera();
    camera->SetOrthographic(left, right, bottom, top, 0.01f, 400);

    float xLen = sin(M_PI / 4) * headRadius;
    float yLen = cos(M_PI / 4) * headRadius;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(startX - xLen, startY - yLen);
    modelMatrix *= transform::Translate(cxPlayer, cyPlayer);
    modelMatrix *= transform::Translate(xLen, yLen);
    modelMatrix *= transform::Rotate(angularStep);
    modelMatrix *= transform::Translate(-xLen, -yLen);
    RenderMesh2D(meshes["playerLeftArm"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(startX + xLen, startY - yLen);
    modelMatrix *= transform::Translate(cxPlayer, cyPlayer);
    modelMatrix *= transform::Translate(-xLen, yLen);
    modelMatrix *= transform::Rotate(angularStep);
    modelMatrix *= transform::Translate(xLen, -yLen);
    RenderMesh2D(meshes["playerRightArm"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(startX, startY);
    modelMatrix *= transform::Translate(cxPlayer, cyPlayer);
    modelMatrix *= transform::Rotate(angularStep);
    RenderMesh2D(meshes["playerHead"], shaders["VertexColor"], modelMatrix);

    spamTime += deltaTimeSeconds;
    if (spamTime > 25) {
        spamTime = 0;
        AddNewEnemy();
    }
    for (int i = 0; i < nrOfEnemies; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(enemies[i].enmX - xLen, enemies[i].enmY - yLen);
        modelMatrix *= transform::Translate(xLen, yLen);
        modelMatrix *= transform::Rotate(enemies[i].angle);
        modelMatrix *= transform::Translate(-xLen, -yLen);
        RenderMesh2D(meshes["enemyLeftArm"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(enemies[i].enmX + xLen, enemies[i].enmY - yLen);
        modelMatrix *= transform::Translate(-xLen, yLen);
        modelMatrix *= transform::Rotate(enemies[i].angle);
        modelMatrix *= transform::Translate(xLen, -yLen);
        RenderMesh2D(meshes["enemyRightArm"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(enemies[i].enmX, enemies[i].enmY);
        modelMatrix *= transform::Rotate(enemies[i].angle);
        RenderMesh2D(meshes["enemyHead"], shaders["VertexColor"], modelMatrix);
        if ((i * (rand() + 50) + 1) % 2) {
            EnemyIsMoving(deltaTimeSeconds, i);
        }
    }
    
    for (int i = 0; i < 5; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(obstacles[i].obsX, obstacles[i].obsY);
        modelMatrix *= transform::Scale(obstacles[i].scaleObsX, obstacles[i].scaleObsY);
        RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    }
    if (leftClick) {
        if (count >= 120) {
            leftClick = false;
        }
        else {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform::Translate(xProjectille - xProjLength / 2 + count * sin(projectilleAngle) * 2.05,
                yProjectille - dist - count * cos(projectilleAngle) * 2.05);
            modelMatrix *= transform::Translate(xProjLength / 2, yProjLength + headRadius);
            modelMatrix *= transform::Rotate(projectilleAngle);
            modelMatrix *= transform::Translate(-xProjLength / 2, -yProjLength - headRadius);
            modelMatrix *= transform::Scale(xProjLength, yProjLength);
            RenderMesh2D(meshes["projectille"], shaders["VertexColor"], modelMatrix);
            count += 1;
        }
    }
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Translate(backgroundLeftCornerX, backgroundLeftCornerY);
    modelMatrix *= transform::Scale(backgroundLengthX, backgroundLengthY);
    RenderMesh2D(meshes["background"], shaders["VertexColor"], modelMatrix);
    
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W) && (!CheckCollisionPOBE(0, 112 * deltaTime))) {
        cyPlayer += 112 * deltaTime;
        top += 112 * deltaTime;
        bottom += 112 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S) && (!CheckCollisionPOBE(0, -112 * deltaTime))) {
        cyPlayer -= 112 * deltaTime;
        top -= 112 * deltaTime;
        bottom -= 112 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D) && (!CheckCollisionPOBE(112 * deltaTime, 0))) {
        cxPlayer += 112 * deltaTime;
        left += 112 * deltaTime;
        right += 112 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A) && (!CheckCollisionPOBE(-112 * deltaTime, 0))) {
        cxPlayer -= 112 * deltaTime;
        left -= 112 * deltaTime;
        right -= 112 * deltaTime;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    
    glm::ivec2 resolution = window->GetResolution();
    float markX = resolution.x / 2;
    float markY = resolution.y / 2;
    mouseX = mouseX;
    mouseY = resolution.y - mouseY;
    float x = mouseX - markX;
    float y = mouseY - markY;
    if (mouseX > markX && mouseY > markY) {
        angularStep = M_PI - asin(x / sqrt(pow(x, 2) + pow(y, 2)));
    }
    if (mouseX < markX && mouseY > markY) {
        angularStep = 1.5f * M_PI - asin(y / sqrt(pow(-x, 2) + pow(y, 2)));
    }
    if (mouseX < markX && mouseY < markY) {
        angularStep = 2 * M_PI - asin(-x / sqrt(pow(-x, 2) + pow(-y, 2)));
    }
    if (mouseX > markX && mouseY < markY) {
        angularStep = asin(x / sqrt(pow(x, 2) + pow(-y, 2)));
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        count = 0;
        xProjectille = startX + cxPlayer;
        yProjectille = startY + cyPlayer;
        projectilleAngle = angularStep;
        leftClick = true;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

// POB - player - obstacles - borders - enemies
// COLLISION SQUARE - CIRCLE: https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
bool Tema1::CheckCollisionPOBE(float moveX, float moveY) {
    bool check = false;
    glm::vec2 center(cxPlayer + startX + moveX, cyPlayer + startY + moveY);
    for (int i = 0; i < 5; i++) {
        glm::vec2 aabb_half_extents(obstacles[i].scaleObsX / 2, obstacles[i].scaleObsY / 2);
        glm::vec2 aabb_center(
            obstacles[i].obsX + aabb_half_extents.x,
            obstacles[i].obsY + aabb_half_extents.y
        );
        glm::vec2 difference = center - aabb_center;
        glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents,
            aabb_half_extents);
        glm::vec2 closest = aabb_center + clamped;
        difference = closest - center;
        check = glm::length(difference) < headRadius;
        if (check == true) {
            break;
        }
    }
    if (backgroundLeftCornerX > center.x - headRadius || backgroundLeftCornerY > center.y - headRadius || 
        backgroundLengthX + backgroundLeftCornerX < center.x + headRadius || backgroundLengthY + backgroundLeftCornerY < center.y + headRadius) {
        check = true;
    }
    // COLLISION CIRCLE - CIRCLE: https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
    for (int i = 0; i < 3; i++) {
        float dx = center.x - enemies[i].enmX;
        float dy = center.y - enemies[i].enmY;
        float distance = sqrt(pow(dx, 2) + pow(dy, 2));
        if (distance < 2 * headRadius) {
            check = true;
        }
        if (check == true) {
            break;
        }
    }
    return check;
}
// EB - enemy - borders
bool Tema1::CheckCollisionEB(int enemyIdx, float moveX, float moveY) {
    bool check = false;
    if (backgroundLeftCornerX > enemies[enemyIdx].enmX + moveX - headRadius || 
        backgroundLeftCornerY > enemies[enemyIdx].enmY + moveY - headRadius ||
        backgroundLengthX + backgroundLeftCornerX < enemies[enemyIdx].enmX + moveX + headRadius ||
        backgroundLengthY + backgroundLeftCornerY < enemies[enemyIdx].enmY + moveY + headRadius) {
        check = true;
    }
    glm::vec2 center(enemies[enemyIdx].enmX + moveX, enemies[enemyIdx].enmY + moveY);
    glm::vec2 centerPlayer(cxPlayer + startX, cyPlayer + startY);
    float dx = center.x - centerPlayer.x;
    float dy = center.y - centerPlayer.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    if (distance < 2 * headRadius) {
        check = true;
    }

    return check;
}
void Tema1::EnemyIsMoving(float deltaTime, int enemyIdx) {
    int followX = 0;
    int followY = 0;
    float playerX = startX + cxPlayer;
    float playerY = startY + cyPlayer;
    float speed = enemies[enemyIdx].speed;
    float radians = 0;
    //if (!((rand() + 100) % 30)) {
    //    enemies[enemyIdx].angle += rand() + 361;
    //}
    if (enemies[enemyIdx].enmX < playerX) {
        followX = 1;
        radians = M_PI / 2.f;
        if (enemies[enemyIdx].enmY < playerY) {
            followY = 1;
            radians += M_PI / 4.f;
        }
        else if (enemies[enemyIdx].enmY > playerY) {
            followY = -1;
            radians -= M_PI / 4.f;
        }
    }
    else if (enemies[enemyIdx].enmX > playerX) {
        followX = -1;
        radians = M_PI * 1.5f;
        if (enemies[enemyIdx].enmY < playerY) {
            followY = 1;
            radians -= M_PI / 4.f;
        }
        else if (enemies[enemyIdx].enmY > playerY) {
            followY = -1;
            radians += M_PI / 4.f;
        }
    }
    enemies[enemyIdx].angle = radians; // angle != radians..ik, ik
    if (!CheckCollisionEB(enemyIdx, 
        followX * speed * deltaTime, 
        followY * speed * deltaTime) && 
        (!((rand() + 100) % 10))) {
        enemies[enemyIdx].enmX += followX * speed * deltaTime;
        enemies[enemyIdx].enmY += followY * speed * deltaTime;
    }
}
void Tema1::AddNewEnemy() {

    Enemy enm;
    enm.enmX = (rand() % (int)(startX * 2 - headRadius));
    enm.enmY = (rand() % (int)(startY * 2 - headRadius));
    enm.angle = 0;
    enm.speed = 112 + (rand() % 50) * nrOfEnemies; // 112 = playerSpeed
    enemies.push_back(enm);
    nrOfEnemies += 1;
}
