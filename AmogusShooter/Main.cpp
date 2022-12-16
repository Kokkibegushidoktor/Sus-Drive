#include "raylib.h"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "Car.h"
#include "Roadblock.h"
#include "Bonus.h"
#include <vector>

#define MAX_RBLOCKS 7
#define MAX_BONUSES 5

Camera3D camera = { 0 };

int main(void)
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Sus Drive");
    InitAudioDevice();

    Texture2D background = LoadTexture("assets/backg.png");

    Sound pickup = LoadSound("assets/sfx/pickup.wav");
    Sound collision = LoadSound("assets/sfx/collision.wav");
    
    Music music = LoadMusicStream("assets/sfx/music.xm");
    music.looping = true;
    SetMusicVolume(music, 0.3f);
    PlayMusicStream(music);

	int score = 0;
    bool gameOver = false;
    //unused
    float gameSpeed = 1.0f;

    RenderTexture2D rTarget = LoadRenderTexture(screenWidth, screenHeight);
    Shader shaders[2];
    shaders[0] = LoadShader(0, "assets/shaders/glsl330/pixel.fs");
    shaders[1] = LoadShader(0, "assets/shaders/glsl330/bloom.fs");

    Model carM = LoadModel("assets/models/shelby/shelbyv2.obj");
    Model streetLight = LoadModel("assets/models/streetlight/untitled.obj");
    Model road = LoadModel("assets/models/road/road.obj");
    Model roadBlock1 = LoadModel("assets/models/roadblocks/roadblock1.obj");
    Model bonus = LoadModel("assets/models/bonus/bonus.obj");

    Shader lShader = LoadShader("assets/shaders/glsl330/lighting.vs", "assets/shaders/glsl330/lighting.fs");
    lShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lShader, "viewPos");
    // Ambient light level
    int ambientLoc = GetShaderLocation(lShader, "ambient");
    float tmp[4] = { 1.5f, 1.5f, 1.5f, 1.0f };
    SetShaderValue(lShader, ambientLoc, tmp, SHADER_UNIFORM_VEC4);

    streetLight.materials[0].shader = lShader;
    road.materials[0].shader = lShader;
    //for (int i = 0; i < car.materialCount; i++) car.materials[i].shader = lShader;

	camera.position = Vector3{ 0.0f, 5.0f, 14.0f };
	camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
	camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
	camera.fovy = 50.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	SetCameraMode(camera, CAMERA_CUSTOM);

    // Create lights
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, Vector3{ -10.0f,3.0f,0.0f }, Vector3{ 0.0f,0.0f,-10.0f }, WHITE, lShader);
    lights[1] = CreateLight(LIGHT_POINT, Vector3{ -10.0f,3.0f,-95.0f }, Vector3Zero(), WHITE, lShader);
    lights[2] = CreateLight(LIGHT_POINT, Vector3{ -10.0f,3.0f,-190.0f }, Vector3Zero(), WHITE, lShader);
    lights[3] = CreateLight(LIGHT_POINT, Vector3{ -10.0f,3.0f,-285.0f }, Vector3Zero(), WHITE, lShader);

    SetTargetFPS(60);

    Car car = Car(carM, Vector3Zero());
    car.shader(lShader);

    roadBlock1.materials[0].shader = lShader;
    std::vector<Roadblock> roadblocks;
    roadblocks.reserve(MAX_RBLOCKS);
    int currentBlocks = 0;
    int spawnTimer = 30;

    std::vector<Bonus> bonuses;
    bonuses.reserve(MAX_BONUSES);
    int currentBonuses = 0;
    int spawnTimerBonus = 30;

    Vector3 road1Pos = { 1.5f,-3.3f,0.0f };
    Vector3 road2Pos = { 1.5f,-3.3f,-95.0f };
    Vector3 road3Pos = { 1.5f,-3.3f,-190.0f };
    Vector3 road4Pos = { 1.5f,-3.3f,-285.0f };

    int currentShader = 0;

    // Main loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);
        UpdateMusicStream(music);
        car.Update();

        if (IsKeyPressed(KEY_RIGHT)) {
            currentShader++;
            if (currentShader > 1)
                currentShader = 0;
        }
        
        //update roadblocks
        for (int i = 0; i < currentBlocks; i++) {
            roadblocks[i].Update();

            if (roadblocks[i].position().z > 20) {
                currentBlocks--;
                roadblocks.erase(roadblocks.begin() + i);
            }
            else if (CheckCollisionBoxes(roadblocks[i].boundingBox(), car.boundingBox())) {
                currentBlocks--;
                roadblocks.erase(roadblocks.begin() + i);
                car.damage(20);
                PlaySoundMulti(collision);
                if (car.hitpoints() < 1) gameOver = true;
            }

        }
        //spawn roadblocks
        if (spawnTimer < 1 && currentBlocks < MAX_RBLOCKS) {
            roadblocks.emplace_back(roadBlock1, Vector3{ 0.0f + 4.5f*GetRandomValue(-1,1),0.0f, -200.0f});
            spawnTimer = 40 + GetRandomValue(-10, 40);
            currentBlocks++;
        }
        if (GetRandomValue(0, 1)) spawnTimer--;

        //game over screen
        if (gameOver)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("YOU ARE DEAD", 340, 260, 70, MAROON);
            DrawText(TextFormat("Your Score: %i", score), 400, 330, 40, WHITE);
            EndDrawing();
            continue;
        }
        //update bonuses
        for (int i = 0; i < currentBonuses; i++) {
            bonuses[i].Update();
            if (bonuses[i].position().z > 20) {
                currentBonuses--;
                bonuses.erase(bonuses.begin() + i);
            }
            else if (CheckCollisionBoxes(bonuses[i].boundingBox(), car.boundingBox())) {
                currentBonuses--;
                bonuses.erase(bonuses.begin() + i);
                score += 5;
                SetSoundPitch(pickup, 1.0f + (float)GetRandomValue(-3, 3) / 10.0f);
                PlaySoundMulti(pickup);
            }
        }
        //spawn bonuses
        if (spawnTimerBonus < 1 && currentBonuses < MAX_BONUSES) {
            bonuses.emplace_back(bonus, Vector3{ 0.0f + 4.5f * GetRandomValue(-1,1),2.0f, -250.0f });
            spawnTimerBonus = 40 + GetRandomValue(-10, 40);
            currentBonuses++;
        }
        if (GetRandomValue(0, 1)) spawnTimerBonus--;

        for (int i = 0; i < MAX_LIGHTS; i++) lights[i].position.z += 0.5;

        road1Pos.z += 0.5;
        road2Pos.z += 0.5;
        road3Pos.z += 0.5;
        road4Pos.z += 0.5;
        if (road1Pos.z > 60) { road1Pos.z = road4Pos.z - 95.0f; lights[0].position.z = road4Pos.z - 95.0f; }
        if (road2Pos.z > 60) { road2Pos.z = road1Pos.z - 95.0f; lights[1].position.z = road1Pos.z - 95.0f; }
        if (road3Pos.z > 60) { road3Pos.z = road2Pos.z - 95.0f; lights[2].position.z = road2Pos.z - 95.0f; }
        if (road4Pos.z > 60) { road4Pos.z = road3Pos.z - 95.0f; lights[3].position.z = road3Pos.z - 95.0f; }

        /*carPos.z -= 0.5;
        camera.position.z -= 0.5;
        camera.target = carPos;*/


        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(lShader, lShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        // Update light values
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(lShader, lights[i]);

        // Draw
        //----------------------------------------------------------------------------------
        BeginTextureMode(rTarget);
        ClearBackground(DARKGRAY);
        DrawTexture(background, 0, 0, WHITE);
        BeginMode3D(camera);
     


        DrawModel(road, road1Pos, 3, WHITE);
        DrawModel(road, road2Pos, 3, WHITE);
        DrawModel(road, road3Pos, 3, WHITE);
        DrawModel(road, road4Pos, 3, WHITE);


        //DrawModel(road, Vector3{ 0.0f,-3.3f,-285.0f }, 3, WHITE);
        DrawModel(streetLight, Vector3Subtract(lights[0].position, Vector3{ 0.0f,3.0f,0.0f }), 1, WHITE);
        DrawModel(streetLight, Vector3Subtract(lights[1].position, Vector3{ 0.0f,3.0f,0.0f }), 1, WHITE);
        DrawModel(streetLight, Vector3Subtract(lights[2].position, Vector3{ 0.0f,3.0f,0.0f }), 1, WHITE);
        DrawModel(streetLight, Vector3Subtract(lights[3].position, Vector3{ 0.0f,3.0f,0.0f }), 1, WHITE);
        //draw car
        car.Draw();
        //draw roadblocks
        for (int i = 0; i < currentBlocks; i++) roadblocks[i].Draw();
        //draw bonuses
        for (int i = 0; i < currentBonuses; i++) bonuses[i].Draw();
        //DrawModel(car, Vector3{ -5.0f,0.0f,0.0f }, 1, WHITE);
        EndMode3D();
        EndTextureMode();


        BeginDrawing();
        ClearBackground(DARKGRAY);
        BeginShaderMode(shaders[currentShader]);
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
        DrawTextureRec(rTarget.texture, Rectangle { 0, 0, (float)rTarget.texture.width, (float)-rTarget.texture.height }, Vector2 { 0, 0 }, WHITE);
        EndShaderMode();
        DrawText(TextFormat("Points: %i", score), 10, 10, 40, YELLOW);
        DrawRectangle(25, 50, 100, 15, MAROON);
        DrawRectangle(25, 50, car.hitpoints(), 15, GREEN);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadModel(carM);
    UnloadModel(streetLight);
    UnloadModel(road);
    UnloadModel(roadBlock1);
    UnloadModel(bonus);

    UnloadTexture(background);
    UnloadShader(shaders[0]);
    UnloadShader(shaders[1]);
    UnloadSound(pickup);
    UnloadSound(collision);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
	return 0;
}