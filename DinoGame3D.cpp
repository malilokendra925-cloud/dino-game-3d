#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string>

// 8-बिट ऑडियो जनरेटर
struct AudioState {
    double phase = 0.0;
    double freq = 0.0;
    int samplesLeft = 0;
};
AudioState gAudio;

void AudioCallback(void* userdata, Uint8* stream, int len) {
    Sint16* buffer = (Sint16*)stream;
    int length = len / 2;
    for (int i = 0; i < length; i++) {
        if (gAudio.samplesLeft > 0) {
            buffer[i] = (sin(gAudio.phase) >= 0 ? 3500 : -3500);
            gAudio.phase += 2.0 * M_PI * gAudio.freq / 44100.0;
            gAudio.samplesLeft--;
        } else {
            buffer[i] = 0;
            gAudio.phase = 0;
        }
    }
}

void PlayTone(double freq, int durationMs) {
    gAudio.freq = freq;
    gAudio.samplesLeft = (44100 * durationMs) / 1000;
}

// 3D क्यूब ड्रा फंक्शन
void DrawCube(float x, float y, float z, float sx, float sy, float sz, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);

    GLfloat vertices[] = {
        -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,  -0.5f,  0.5f,  0.5f,   0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f
    };

    GLubyte indices[] = {
        0, 1, 2,  2, 3, 0,
        4, 5, 6,  6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    glColor4f(r, g, b, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

// 3D T-Rex
void Draw3DDino(float x, float y, float z, bool isNight, int score) {
    float bodyR = isNight ? 0.8f : 0.92f;
    float bodyG = isNight ? 0.8f : 0.48f;
    float bodyB = isNight ? 0.9f : 0.12f;
    if (score >= 2100) { bodyR = 0.2f; bodyG = 0.8f; bodyB = 0.9f; }
    else if (score >= 1200) { bodyR = 0.8f; bodyG = 0.2f; bodyB = 0.8f; }

    DrawCube(x, y + 0.6f, z, 0.8f, 0.9f, 0.6f, bodyR, bodyG, bodyB);
    DrawCube(x + 0.45f, y + 1.1f, z, 0.7f, 0.6f, 0.55f, bodyR, bodyG, bodyB);
    DrawCube(x + 0.85f, y + 0.95f, z, 0.4f, 0.35f, 0.5f, bodyR, bodyG, bodyB);
    
    if (score < 1200) {
        DrawCube(x + 0.60f, y + 1.2f, z, 0.5f, 0.18f, 0.58f, 0.05f, 0.05f, 0.05f);
    }
    
    DrawCube(x - 0.6f, y + 0.6f, z, 0.6f, 0.35f, 0.35f, bodyR, bodyG, bodyB);
    DrawCube(x - 0.2f, y + 0.15f, z + 0.2f, 0.22f, 0.4f, 0.2f, bodyR * 0.8f, bodyG * 0.8f, bodyB * 0.8f);
    DrawCube(x + 0.2f, y + 0.15f, z - 0.2f, 0.22f, 0.4f, 0.2f, bodyR * 0.8f, bodyG * 0.8f, bodyB * 0.8f);
}

// 3D Cactus
void Draw3DCactus(float x, float y, float z) {
    DrawCube(x, y + 0.8f, z, 0.35f, 1.6f, 0.35f, 0.15f, 0.65f, 0.2f);
    DrawCube(x, y + 0.7f, z - 0.35f, 0.25f, 0.25f, 0.5f, 0.15f, 0.65f, 0.2f);
    DrawCube(x, y + 1.0f, z - 0.5f, 0.25f, 0.6f, 0.25f, 0.15f, 0.65f, 0.2f);
}

// 3D Pterodactyl Bird
void Draw3DBird(float x, float y, float z) {
    DrawCube(x, y, z, 0.7f, 0.3f, 0.5f, 0.5f, 0.5f, 0.6f);
    DrawCube(x, y + 0.2f, z - 0.9f, 0.3f, 0.1f, 1.4f, 0.6f, 0.6f, 0.65f);
}

// 3D Cloud
void Draw3DCloud(float x, float y, float z, bool isNight) {
    float cR = isNight ? 0.3f : 1.0f;
    float cG = isNight ? 0.3f : 1.0f;
    float cB = isNight ? 0.35f : 1.0f;
    DrawCube(x, y, z, 2.6f, 0.6f, 0.8f, cR, cG, cB);
}

// 2D Font System
const char fontDigits[12][5][4] = {
    {"XXX","X.X","X.X","X.X","XXX"},
    {"..X","..X","..X","..X","..X"},
    {"XXX","..X","XXX","X..","XXX"},
    {"XXX","..X","XXX","..X","XXX"},
    {"X.X","X.X","XXX","..X","..X"},
    {"XXX","X..","XXX","..X","XXX"},
    {"XXX","X..","XXX","X.X","XXX"},
    {"XXX","..X","..X","..X","..X"},
    {"XXX","X.X","XXX","X.X","XXX"},
    {"XXX","X.X","XXX","..X","XXX"},
    {"X.X","X.X","XXX","X.X","X.X"},
    {"XXX",".X.",".X.",".X.","XXX"}
};

void Draw2DRect(float x, float y, float w, float h, float r, float g, float b) {
    GLfloat vertices[] = { x, y, 0.0f, x + w, y, 0.0f, x + w, y + h, 0.0f, x, y + h, 0.0f };
    glColor4f(r, g, b, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawScoreHUD(const std::string& text, float startX, float startY, float pSize, float r, float g, float b) {
    float curX = startX;
    for (char ch : text) {
        if (ch >= '0' && ch <= '9') {
            int d = ch - '0';
            for (int r_idx = 0; r_idx < 5; r_idx++) {
                for (int c_idx = 0; c_idx < 3; c_idx++) {
                    if (fontDigits[d][r_idx][c_idx] == 'X')
                        Draw2DRect(curX + c_idx * pSize, startY + r_idx * pSize, pSize, pSize, r, g, b);
                }
            }
            curX += 4 * pSize;
        } else if (ch == 'H' || ch == 'I') {
            int d = (ch == 'H') ? 10 : 11;
            for (int r_idx = 0; r_idx < 5; r_idx++) {
                for (int c_idx = 0; c_idx < 3; c_idx++) {
                    if (fontDigits[d][r_idx][c_idx] == 'X')
                        Draw2DRect(curX + c_idx * pSize, startY + r_idx * pSize, pSize, pSize, r, g, b);
                }
            }
            curX += 4 * pSize;
        } else if (ch == ' ') {
            curX += 3 * pSize;
        }
    }
}

struct Obstacle {
    float x;
    bool isBird;
};

struct Cloud {
    float x, y, z, speed;
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 512;
    desiredSpec.callback = AudioCallback;

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &desiredSpec, NULL, 0);
    SDL_PauseAudioDevice(dev, 0);

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int screenWidth = dm.w, screenHeight = dm.h;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    SDL_Window* window = SDL_CreateWindow("3D Dino Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    float dinoX = -2.5f, dinoY = 0.0f, velocityY = 0.0f;
    float gravity = -32.0f, jumpForce = 12.5f;
    bool isGrounded = true, running = true;

    float score = 0.0f;
    int highScore = 906;
    int lastMilestone = 0;

    std::vector<Obstacle> obstacles;
    std::vector<Cloud> clouds = { { 12.0f, 6.5f, -4.0f, 2.5f }, { 24.0f, 8.0f, -6.0f, 1.8f } };

    Uint32 lastSpawn = SDL_GetTicks();
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        score += dt * 10.0f;
        if ((int)score / 100 > lastMilestone) {
            lastMilestone = (int)score / 100;
            PlayTone(850.0, 90);
        }

        bool isNight = ((int)score / 400) % 2 == 1;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_FINGERDOWN) {
                if (isGrounded) {
                    velocityY = jumpForce;
                    isGrounded = false;
                    PlayTone(650.0, 70);
                }
            }
        }

        if (!isGrounded) {
            velocityY += gravity * dt;
            dinoY += velocityY * dt;
            if (dinoY <= 0.0f) { dinoY = 0.0f; velocityY = 0.0f; isGrounded = true; }
        }

        for (auto& c : clouds) {
            c.x -= c.speed * dt;
            if (c.x < -20.0f) c.x = 25.0f;
        }

        if (currentTime - lastSpawn > 1600) {
            bool spawnBird = (score > 350 && (rand() % 3 == 0));
            obstacles.push_back({ 24.0f, spawnBird });
            lastSpawn = currentTime;
        }

        float speed = 12.0f + (score / 200.0f);
        for (size_t i = 0; i < obstacles.size(); i++) {
            obstacles[i].x -= speed * dt;
            float hitHeight = obstacles[i].isBird ? 1.4f : 1.1f;
            if (fabs(dinoX - obstacles[i].x) < 0.7f && dinoY < hitHeight) {
                PlayTone(150.0, 300);
                SDL_Delay(300);
                running = false;
            }
        }

        // 3D रेंडरिंग
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, screenWidth, screenHeight);
        
        // Day / Night Background Color
        if (isNight) {
            glClearColor(0.08f, 0.08f, 0.15f, 1.0f);
        } else {
            glClearColor(0.85f, 0.90f, 0.95f, 1.0f);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = (float)screenWidth / (float)screenHeight;
        float fov = 45.0f * (M_PI / 180.0f);
        float zNear = 0.5f, zFar = 100.0f;
        float fH = tan(fov / 2.0f) * zNear, fW = fH * aspect;
        glFrustumf(-fW, fW, -fH, fH, zNear, zFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, -1.0f, -22.0f);
        glRotatef(12.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(-16.0f, 0.0f, 1.0f, 0.0f);

        for (const auto& c : clouds) Draw3DCloud(c.x, c.y, c.z, isNight);

        // Ground Color
        DrawCube(0.0f, -0.5f, 0.0f, 70.0f, 1.0f, 6.0f, isNight ? 0.2f : 0.88f, isNight ? 0.2f : 0.78f, isNight ? 0.28f : 0.58f);

        Draw3DDino(dinoX, dinoY, 0.0f, isNight, (int)score);

        for (const auto& obs : obstacles) {
            if (obs.isBird) Draw3DBird(obs.x, 1.3f, 0.0f);
            else Draw3DCactus(obs.x, 0.0f, 0.0f);
        }

        if (isNight) DrawCube(8.0f, 9.0f, -12.0f, 2.0f, 2.0f, 0.5f, 0.95f, 0.95f, 0.7f);

        // 2D HUD (फिक्स पोजीशन ताकि आधा न कटे)
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        char scoreText[32];
        sprintf(scoreText, "HI %05d  %05d", highScore, (int)score);
        float pSize = fmax(4.0f, screenWidth / 110.0f);
        
        // सुरक्षित नीचे की तरफ पोजीशन ताकि ऊपर से छिप न सके
        DrawScoreHUD(scoreText, screenWidth - (18.0f * 4.0f * pSize) - 40.0f, 120.0f, pSize, isNight ? 0.95f : 0.2f, isNight ? 0.95f : 0.2f, isNight ? 0.95f : 0.2f);

        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
    }

    SDL_CloseAudioDevice(dev);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
