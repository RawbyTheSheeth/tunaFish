// /* primitives.c modified ... */

// #define SDL_MAIN_USE_CALLBACKS 1
// #include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>
// #include <math.h>
// #include <string>
// #include <format>

// // random number generating
// #include <cstdlib> // For rand() and srand()
// #include <ctime>   // For time()

// /* Global state */
// static SDL_Window *window = NULL;
// static SDL_Renderer *renderer = NULL;

// static std::string letters = "abcdefghijklmnopqrstuvwxyz";

// #define WINDOW_WIDTH 640
// #define WINDOW_HEIGHT 480
// #define PI 3.14159265359

// /* Define the guess state enum*/
// // define the bits
// enum class GuessState : uint8_t {
//     None      = 0,
//     Incorrect = 1 << 0,
//     Correct   = 1 << 1,
//     Event     = 1 << 2
// };

// // define functions
// inline GuessState operator|(GuessState a, GuessState b) {
//     return static_cast<GuessState>(
//         static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
//     );
// }
// inline GuessState operator&(GuessState a, GuessState b) {
//     return static_cast<GuessState>(
//         static_cast<uint8_t>(a) & static_cast<uint8_t>(b)
//     );
// }



// typedef struct {
//     SDL_Color baseColor;
//     Uint64 startTicks;
//     float brightPhaseOffset;
//     float brightness;
//     float screenPeriod;
//     Uint64 lastTick;
//     char currentLetter;
//     uint8_t guessState; 
//     /*
//     2^0 - event bit: 0 - no event, 1 - event
//     2^1 - correct bit: 0 - incorrect, 1 - correct
//     2^2 - updating bit: do we store a flag for updating here or not?
//     */ 
// } AppState;


// /* App init */
// SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
// {
//     AppState *state = (AppState *)SDL_calloc(1, sizeof(AppState));
//     *appstate = state;

//     srand(static_cast<unsigned int>(time(0)));

//     SDL_SetAppMetadata("Color + Brightness Demo", "1.0", "com.example.color-brightness");

//     if (!SDL_Init(SDL_INIT_VIDEO)) {
//         SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
//         return SDL_APP_FAILURE;
//     }

//     if (!SDL_CreateWindowAndRenderer("Arrow keys + brightness", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
//         SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
//         return SDL_APP_FAILURE;
//     }

//     // Try to enable VSync
//     if (!SDL_SetRenderVSync(renderer, 4)) {
//         SDL_Log("VSync not supported: %s", SDL_GetError());
//     }

//     state->baseColor = (SDL_Color){255, 255, 255, 255};  // start white
//     state->startTicks = SDL_GetTicks();
//     state->brightPhaseOffset = 0.f;
//     state->screenPeriod = 4.f; // seconds of loop for brightness loop
//     // state->currentLetter = (rand() % letters.length()); // Generates numbers from 0 to length of letters array
//     state->currentLetter = 'a';
//     state->guessState = 0;
//     state->lastTick = state->startTicks;

//     return SDL_APP_CONTINUE;
// }

// /* Handle input events */
// SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
// {
//     AppState *state = (AppState *)appstate;

//     if (event->type == SDL_EVENT_QUIT) {
//         return SDL_APP_SUCCESS;
//     }

//     if (event->type == SDL_EVENT_KEY_DOWN) {
//         switch (event->key.key) {
//             case SDLK_UP:    state->baseColor = (SDL_Color){255, 0, 0, 255}; break;  // red
//             case SDLK_DOWN:  state->baseColor = (SDL_Color){0, 255, 0, 255}; break;  // green
//             case SDLK_LEFT:  state->baseColor = (SDL_Color){0, 0, 255, 255}; break;  // blue
//             case SDLK_RIGHT: state->baseColor = (SDL_Color){255, 255, 255, 255};   break;  // white

//             //if a letter is pressed check if the right letter was pressed
//             if (event->key.key >= SDL_SCANCODE_A && event->key.key <= SDL_SCANCODE_Z) 
//                 {
//                 char pressed = (char)event->key.key; // gives 'a'...'z'
//                 // SDL_Log("Letter pressed: %c", pressed);
//                 if (pressed == state->currentLetter)
//                 {
//                     // set keypress to 'correct'
//                     state->guessState &= ~0x3; //clear lower 2 bits
//                     state->guessState |= 0x3; // set correct and event bit high
//                     // do the CORRECT method
//                     // flash green screen and select new letter and random color
//                     state->currentLetter = (char) (SDL_SCANCODE_A + (rand()%25)); // select a random letter
//                 }
//                 else
//                 {
//                     // set keypress to 'incorrect'
//                     state->guessState &= ~0x3; //clear lower 2 bits
//                     state->guessState |= 0x01; // set correct and event bit high
//                     // flash color to red and keep letter
//                 }
//             }
//         }

//     float periodTicks = state->screenPeriod*1e3f;
//     state->brightPhaseOffset = 1 - fmodf(SDL_GetTicks() - state->startTicks, periodTicks)/periodTicks;  // max brightness

//     }

//     return SDL_APP_CONTINUE;
// }

// /* Frame update */
// SDL_AppResult SDL_AppIterate(void *appstate)
// {
//     const int charsize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

//     AppState *state = (AppState *)appstate;

//     // Time since start
//     Uint64 now = SDL_GetTicks();
//     float t = (now - state->startTicks);
//     // float FPS = 1/((float)(now-lastTick)/1000.f);
//     float periodTicks = state->screenPeriod*1e3f;

//     // Brightness cycle: 0→1→0 over 4 seconds
//     float phase = fmodf(t,periodTicks)/periodTicks;
//     // state->brightness = fabs(0.5 - fmodf(phase + state->brightPhaseOffset,1))*2;
//     state->brightness = 0.5+0.5*cos(2*PI*fmodf(phase + state->brightPhaseOffset,1));

//     // Apply brightness scaling to base color
//     Uint8 r = (Uint8)(state->baseColor.r * state->brightness);
//     Uint8 g = (Uint8)(state->baseColor.g * state->brightness);
//     Uint8 b = (Uint8)(state->baseColor.b * state->brightness);

//     SDL_SetRenderDrawColor(renderer, r, g, b, 255);
//     SDL_RenderClear(renderer);

//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */

//     std::string msg_uptime = std::format("Uptime: {:.2f}", now/1000.f);
//     std::string msg_brightness = std::format("Brightness: {:.2f}", state->brightness);
//     std::string msg_phase = std::format("Phase: {:.2f}", phase);
//     std::string msg_offset = std::format("Offset: {:.2f}", state->brightPhaseOffset);
//     std::string msg_FPS = std::format("FPS: {:.0f}", 1000.f/(now - state->lastTick));
    
//     SDL_RenderDebugTextFormat(renderer, charsize, 12*charsize, "%c", state->currentLetter);

//     SDL_RenderDebugTextFormat(renderer, charsize, 2*charsize, msg_uptime.c_str());
//     SDL_RenderDebugTextFormat(renderer, charsize, 4*charsize, msg_brightness.c_str());
//     SDL_RenderDebugTextFormat(renderer, charsize, 6*charsize, msg_phase.c_str());
//     SDL_RenderDebugTextFormat(renderer, charsize, 8*charsize, msg_offset.c_str());
//     SDL_RenderDebugTextFormat(renderer, charsize, 10*charsize, msg_FPS.c_str());

//     // SDL_RenderDebugTextFormat(renderer, (float) ((WINDOW_WIDTH - (charsize * msg_uptime.length())) / 2), 400, msg_uptime.c_str());

//     // SDL_RenderDebugTextFormat(renderer, (float) ((WINDOW_WIDTH - (charsize * 46)) / 2), 400, "(This program has been running for %" SDL_PRIu64 " seconds.)", SDL_GetTicks() / 1000);

//     SDL_RenderPresent(renderer);

//     state->lastTick = now;

//     return SDL_APP_CONTINUE;
// }

// /* Shutdown */
// void SDL_AppQuit(void *appstate, SDL_AppResult result)
// {
//     SDL_free(appstate);
//     // SDL will clean up window/renderer
// }


/* woodeneye-008.c ... */

/*
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define MAP_BOX_SCALE 16
#define MAP_BOX_EDGES_LEN (12 + MAP_BOX_SCALE * 2)
#define MAX_PLAYER_COUNT 4
#define CIRCLE_DRAW_SIDES 32
#define CIRCLE_DRAW_SIDES_LEN (CIRCLE_DRAW_SIDES + 1)

const int JOYSTICK_DEAD_ZONE = 8000;

typedef struct {
    SDL_MouseID mouse;
    SDL_KeyboardID keyboard;
    SDL_JoystickID joystick;
    double pos[3];
    double vel[3];
    unsigned int yaw;
    int pitch;
    float radius, height;
    unsigned char color[3];
    unsigned char wasd;
} Player;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int player_count;
    Player players[MAX_PLAYER_COUNT];
    float edges[MAP_BOX_EDGES_LEN][6];
} AppState;

static const struct {
    const char *key;
    const char *value;
} extended_metadata[] = {
    { SDL_PROP_APP_METADATA_URL_STRING, "https://examples.libsdl.org/SDL3/demo/02-woodeneye-008/" },
    { SDL_PROP_APP_METADATA_CREATOR_STRING, "SDL team" },
    { SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed in the public domain" },
    { SDL_PROP_APP_METADATA_TYPE_STRING, "game" }
};

static int whoseMouse(SDL_MouseID mouse, const Player players[], int players_len)
{
    int i;
    for (i = 0; i < players_len; i++) {
        if (players[i].mouse == mouse) return i;
    }
    return -1;
}

static int whoseKeyboard(SDL_KeyboardID keyboard, const Player players[], int players_len)
{
    int i;
    for (i = 0; i < players_len; i++) {
        if (players[i].keyboard == keyboard) return i;
    }
    return -1;
}

static int whoseJoystick(SDL_JoystickID joystick, const Player players[], int players_len)
{
    int i;
    for (i = 0; i < players_len; i++) {
        if (players[i].joystick == joystick) return i;
    }
    return -1;
}

static void shoot(int shooter, Player players[], int players_len)
{
    int i, j;
    double x0 = players[shooter].pos[0];
    double y0 = players[shooter].pos[1];
    double z0 = players[shooter].pos[2];
    double bin_rad = SDL_PI_D / 2147483648.0;
    double yaw_rad   = bin_rad * players[shooter].yaw;
    double pitch_rad = bin_rad * players[shooter].pitch;
    double cos_yaw   = SDL_cos(  yaw_rad);
    double sin_yaw   = SDL_sin(  yaw_rad);
    double cos_pitch = SDL_cos(pitch_rad);
    double sin_pitch = SDL_sin(pitch_rad);
    double vx = -sin_yaw*cos_pitch;
    double vy =          sin_pitch;
    double vz = -cos_yaw*cos_pitch;
    for (i = 0; i < players_len; i++) {
        if (i == shooter) continue;
        Player *target = &(players[i]);
        int hit = 0;
        for (j = 0; j < 2; j++) {
            double r = target->radius;
            double h = target->height;
            double dx = target->pos[0] - x0;
            double dy = target->pos[1] - y0 + (j == 0 ? 0 : r - h);
            double dz = target->pos[2] - z0;
            double vd = vx*dx + vy*dy + vz*dz;
            double dd = dx*dx + dy*dy + dz*dz;
            double vv = vx*vx + vy*vy + vz*vz;
            double rr = r * r;
            if (vd < 0) continue;
            if (vd * vd >= vv * (dd - rr)) hit += 1;
        }
        if (hit) {
            target->pos[0] = (double)(MAP_BOX_SCALE * (SDL_rand(256) - 128)) / 256;
            target->pos[1] = (double)(MAP_BOX_SCALE * (SDL_rand(256) - 128)) / 256;
            target->pos[2] = (double)(MAP_BOX_SCALE * (SDL_rand(256) - 128)) / 256;
        }
    }
}

static void update(Player *players, int players_len, Uint64 dt_ns)
{
    int i;
    for (i = 0; i < players_len; i++) {
        Player *player = &players[i];
        double rate = 6.0;
        double time = (double)dt_ns * 1e-9;
        double drag = SDL_exp(-time * rate);
        double diff = 1.0 - drag;
        double mult = 60.0;
        double grav = 25.0;
        double yaw = (double)player->yaw;
        double rad = yaw * SDL_PI_D / 2147483648.0;
        double cos = SDL_cos(rad);
        double sin = SDL_sin(rad);
        unsigned char wasd = player->wasd;
        double dirX = (wasd & 8 ? 1.0 : 0.0) - (wasd & 2 ? 1.0 : 0.0);
        double dirZ = (wasd & 4 ? 1.0 : 0.0) - (wasd & 1 ? 1.0 : 0.0);
        double norm = dirX * dirX + dirZ * dirZ;
        double accX = mult * (norm == 0 ? 0 : ( cos*dirX + sin*dirZ) / SDL_sqrt(norm));
        double accZ = mult * (norm == 0 ? 0 : (-sin*dirX + cos*dirZ) / SDL_sqrt(norm));
        double velX = player->vel[0];
        double velY = player->vel[1];
        double velZ = player->vel[2];
        player->vel[0] -= velX * diff;
        player->vel[1] -= grav * time;
        player->vel[2] -= velZ * diff;
        player->vel[0] += diff * accX / rate;
        player->vel[2] += diff * accZ / rate;
        player->pos[0] += (time - diff/rate) * accX / rate + diff * velX / rate;
        player->pos[1] += -0.5 * grav * time * time + velY * time;
        player->pos[2] += (time - diff/rate) * accZ / rate + diff * velZ / rate;
        double scale = (double)MAP_BOX_SCALE;
        double bound = scale - player->radius;
        double posX = SDL_max(SDL_min(bound, player->pos[0]), -bound);
        double posY = SDL_max(SDL_min(bound, player->pos[1]), player->height - scale);
        double posZ = SDL_max(SDL_min(bound, player->pos[2]), -bound);
        if (player->pos[0] != posX) player->vel[0] = 0;
        if (player->pos[1] != posY) player->vel[1] = (wasd & 16) ? 8.4375 : 0;
        if (player->pos[2] != posZ) player->vel[2] = 0;
        player->pos[0] = posX;
        player->pos[1] = posY;
        player->pos[2] = posZ;
    }
}

static void drawCircle(SDL_Renderer *renderer, float r, float x, float y)
{
    float ang;
    SDL_FPoint points[CIRCLE_DRAW_SIDES_LEN];
    int i;
    for (i = 0; i < CIRCLE_DRAW_SIDES_LEN; i++) {
        ang = 2.0f * SDL_PI_F * (float)i / (float)CIRCLE_DRAW_SIDES;
        points[i].x = x + r * SDL_cosf(ang);
        points[i].y = y + r * SDL_sinf(ang);
    }
    SDL_RenderLines(renderer, (const SDL_FPoint*)&points, CIRCLE_DRAW_SIDES_LEN);
}

static void drawClippedSegment(
    SDL_Renderer *renderer,
    float ax, float ay, float az,
    float bx, float by, float bz,
    float x, float y, float z, float w)
{
    if (az >= -w && bz >= -w) return;
    float dx = ax - bx;
    float dy = ay - by;
    if (az > -w) {
        float t = (-w - bz) / (az - bz);
        ax = bx + dx * t;
        ay = by + dy * t;
        az = -w;
    } else if (bz > -w) {
        float t = (-w - az) / (bz - az);
        bx = ax - dx * t;
        by = ay - dy * t;
        bz = -w;
    }
    ax = -z * ax / az;
    ay = -z * ay / az;
    bx = -z * bx / bz;
    by = -z * by / bz;
    SDL_RenderLine(renderer, x + ax, y - ay, x + bx, y - by);
}

static char debug_string[32];
static void draw(SDL_Renderer *renderer, const float (*edges)[6], const Player players[], int players_len)
{
    int w, h, i, j, k;
    if (!SDL_GetRenderOutputSize(renderer, &w, &h)) {
        return;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    if (players_len > 0) {
        float wf = (float)w;
        float hf = (float)h;
        int part_hor = players_len > 2 ? 2 : 1;
        int part_ver = players_len > 1 ? 2 : 1;
        float size_hor = wf / ((float)part_hor);
        float size_ver = hf / ((float)part_ver);
        for (i = 0; i < players_len; i++) {
            const Player *player = &players[i];
            float mod_x = (float)(i % part_hor);
            float mod_y = (float)(i / part_hor);
            float hor_origin = (mod_x + 0.5f) * size_hor;
            float ver_origin = (mod_y + 0.5f) * size_ver;
            float cam_origin = (float)(0.5 * SDL_sqrt(size_hor * size_hor + size_ver * size_ver));
            float hor_offset = mod_x * size_hor;
            float ver_offset = mod_y * size_ver;
            SDL_Rect rect;
            rect.x = (int)hor_offset;
            rect.y = (int)ver_offset;
            rect.w = (int)size_hor;
            rect.h = (int)size_ver;
            SDL_SetRenderClipRect(renderer, &rect);
            double x0 = player->pos[0];
            double y0 = player->pos[1];
            double z0 = player->pos[2];
            double bin_rad = SDL_PI_D / 2147483648.0;
            double yaw_rad   = bin_rad * player->yaw;
            double pitch_rad = bin_rad * player->pitch;
            double cos_yaw   = SDL_cos(  yaw_rad);
            double sin_yaw   = SDL_sin(  yaw_rad);
            double cos_pitch = SDL_cos(pitch_rad);
            double sin_pitch = SDL_sin(pitch_rad);
            double mat[9] = {
                cos_yaw          ,          0, -sin_yaw          ,
                sin_yaw*sin_pitch,  cos_pitch,  cos_yaw*sin_pitch,
                sin_yaw*cos_pitch, -sin_pitch,  cos_yaw*cos_pitch
            };
            SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
            for (k = 0; k < MAP_BOX_EDGES_LEN; k++) {
                const float *line = edges[k];
                float ax = (float)(mat[0] * (line[0] - x0) + mat[1] * (line[1] - y0) + mat[2] * (line[2] - z0));
                float ay = (float)(mat[3] * (line[0] - x0) + mat[4] * (line[1] - y0) + mat[5] * (line[2] - z0));
                float az = (float)(mat[6] * (line[0] - x0) + mat[7] * (line[1] - y0) + mat[8] * (line[2] - z0));
                float bx = (float)(mat[0] * (line[3] - x0) + mat[1] * (line[4] - y0) + mat[2] * (line[5] - z0));
                float by = (float)(mat[3] * (line[3] - x0) + mat[4] * (line[4] - y0) + mat[5] * (line[5] - z0));
                float bz = (float)(mat[6] * (line[3] - x0) + mat[7] * (line[4] - y0) + mat[8] * (line[5] - z0));
                drawClippedSegment(renderer, ax, ay, az, bx, by, bz, hor_origin, ver_origin, cam_origin, 1);
            }
            for (j = 0; j < players_len; j++) {
                if (i == j) continue;
                const Player *target = &players[j];
                SDL_SetRenderDrawColor(renderer, target->color[0], target->color[1], target->color[2], 255);
                for (k = 0; k < 2; k++) {
                    double rx = target->pos[0] - player->pos[0];
                    double ry = target->pos[1] - player->pos[1] + (target->radius - target->height) * (float)k;
                    double rz = target->pos[2] - player->pos[2];
                    double dx = mat[0] * rx + mat[1] * ry + mat[2] * rz;
                    double dy = mat[3] * rx + mat[4] * ry + mat[5] * rz;
                    double dz = mat[6] * rx + mat[7] * ry + mat[8] * rz;
                    double r_eff = target->radius * cam_origin / dz;
                    if (!(dz < 0)) continue;
                    drawCircle(renderer, (float)(r_eff), (float)(hor_origin - cam_origin*dx/dz), (float)(ver_origin + cam_origin*dy/dz));
                }
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderLine(renderer, hor_origin, ver_origin-10, hor_origin, ver_origin+10);
            SDL_RenderLine(renderer, hor_origin-10, ver_origin, hor_origin+10, ver_origin);
        }
    }
    SDL_SetRenderClipRect(renderer, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, 0, 0, debug_string);
    SDL_RenderPresent(renderer);
}

static void initPlayers(Player *players, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        players[i].pos[0] = 8.0 * (i & 1 ? -1.0 : 1.0);
        players[i].pos[1] = 0;
        players[i].pos[2] = 8.0 * (i & 1 ? -1.0 : 1.0) * (i & 2 ? -1.0 : 1.0);
        players[i].vel[0] = 0;
        players[i].vel[1] = 0;
        players[i].vel[2] = 0;
        players[i].yaw = 0x20000000 + (i & 1 ? 0x80000000 : 0) + (i & 2 ? 0x40000000 : 0);
        players[i].pitch = -0x08000000;
        players[i].radius = 0.5f;
        players[i].height = 1.5f;
        players[i].wasd = 0;
        players[i].mouse = 0;
        players[i].keyboard = 0;
        players[i].color[0] = (1 << (i / 2)) & 2 ? 0 : 0xff;
        players[i].color[1] = (1 << (i / 2)) & 1 ? 0 : 0xff;
        players[i].color[2] = (1 << (i / 2)) & 4 ? 0 : 0xff;
        players[i].color[0] = (i & 1) ? players[i].color[0] : ~players[i].color[0];
        players[i].color[1] = (i & 1) ? players[i].color[1] : ~players[i].color[1];
        players[i].color[2] = (i & 1) ? players[i].color[2] : ~players[i].color[2];
    }
}

static void initEdges(int scale, float (*edges)[6], int edges_len)
{
    int i, j;
    const float r = (float)scale;
    const int map[24] = {
        0,1 , 1,3 , 3,2 , 2,0 ,
        7,6 , 6,4 , 4,5 , 5,7 ,
        6,2 , 3,7 , 0,4 , 5,1
    };
    for(i = 0; i < 12; i++) {
        for (j = 0; j < 3; j++) {
            edges[i][j+0] = (map[i*2+0] & (1 << j) ? r : -r);
            edges[i][j+3] = (map[i*2+1] & (1 << j) ? r : -r);
        }
    }
    for(i = 0; i < scale; i++) {
        float d = (float)(i * 2);
        for (j = 0; j < 2; j++) {
            edges[i+12][3*j+0]       = j ? r : -r;
            edges[i+12][3*j+1]       =  -r;
            edges[i+12][3*j+2]       = d-r;
            edges[i+12+scale][3*j+0] = d-r;
            edges[i+12+scale][3*j+1] =  -r;
            edges[i+12+scale][3*j+2] = j ? r : -r;
        }
    }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_SetAppMetadata("Example splitscreen shooter game", "1.0", "com.example.woodeneye-008")) {
        return SDL_APP_FAILURE;
    }
    int i;
    for (i = 0; i < SDL_arraysize(extended_metadata); i++) {
        if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value)) {
            return SDL_APP_FAILURE;
        }
    }

    // AppState *as = SDL_calloc(1, sizeof(AppState));
    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    } else {
        *appstate = as;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("examples/demo/woodeneye-008", 640, 480, 0, &as->window, &as->renderer)) {
        return SDL_APP_FAILURE;
    }

    as->player_count = 1;
    initPlayers(as->players, MAX_PLAYER_COUNT);
    initEdges(MAP_BOX_SCALE, as->edges, MAP_BOX_EDGES_LEN);
    debug_string[0] = 0;

    SDL_SetRenderVSync(as->renderer, false);
    SDL_SetWindowRelativeMouseMode(as->window, true);
    SDL_SetHintWithPriority(SDL_HINT_WINDOWS_RAW_KEYBOARD, "1", SDL_HINT_OVERRIDE);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    // AppState *as = appstate;
    AppState *as = (AppState *)appstate;

    Player *players = as->players;
    int player_count = as->player_count;
    int i;
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_MOUSE_REMOVED:
            for (i = 0; i < player_count; i++) {
                if (players[i].mouse == event->mdevice.which) {
                    players[i].mouse = 0;
                }
            }
            break;
        case SDL_EVENT_KEYBOARD_REMOVED:
            for (i = 0; i < player_count; i++) {
                if (players[i].keyboard == event->kdevice.which) {
                    players[i].keyboard = 0;
                }
            }
            break;
        case SDL_EVENT_MOUSE_MOTION: {
            SDL_MouseID id = event->motion.which;
            int index = whoseMouse(id, players, player_count);
            if (index >= 0) {
                players[index].yaw -= ((int)event->motion.xrel) * 0x00080000;
                players[index].pitch = SDL_max(-0x40000000, SDL_min(0x40000000, players[index].pitch - ((int)event->motion.yrel) * 0x00080000));
            } else if (id) {
                for (i = 0; i < MAX_PLAYER_COUNT; i++) {
                    if (players[i].mouse == 0) {
                        players[i].mouse = id;
                        as->player_count = SDL_max(as->player_count, i + 1);
                        break;
                    }
                }
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            SDL_MouseID id = event->button.which;
            int index = whoseMouse(id, players, player_count);
            if (index >= 0) {
                shoot(index, players, player_count);
            }
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            SDL_Keycode sym = event->key.key;
            SDL_KeyboardID id = event->key.which;
            int index = whoseKeyboard(id, players, player_count);
            if (index >= 0) {
                if (sym == SDLK_W) players[index].wasd |= 1;
                if (sym == SDLK_A) players[index].wasd |= 2;
                if (sym == SDLK_S) players[index].wasd |= 4;
                if (sym == SDLK_D) players[index].wasd |= 8;
                if (sym == SDLK_SPACE) players[index].wasd |= 16;
            } else if (id) {
                for (i = 0; i < MAX_PLAYER_COUNT; i++) {
                    if (players[i].keyboard == 0) {
                        players[i].keyboard = id;
                        as->player_count = SDL_max(as->player_count, i + 1);
                        break;
                    }
                }
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            SDL_Keycode sym = event->key.key;
            SDL_KeyboardID id = event->key.which;
            if (sym == SDLK_ESCAPE) return SDL_APP_SUCCESS;
            int index = whoseKeyboard(id, players, player_count);
            if (index >= 0) {
                if (sym == SDLK_W) players[index].wasd &= 30;
                if (sym == SDLK_A) players[index].wasd &= 29;
                if (sym == SDLK_S) players[index].wasd &= 27;
                if (sym == SDLK_D) players[index].wasd &= 23;
                if (sym == SDLK_SPACE) players[index].wasd &= 15;
            }
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    // AppState *as = appstate;
    AppState *as = (AppState *)appstate;
    static Uint64 accu = 0;
    static Uint64 last = 0;
    static Uint64 past = 0;
    Uint64 now = SDL_GetTicksNS();
    Uint64 dt_ns = now - past;
    update(as->players, as->player_count, dt_ns);
    draw(as->renderer, (const float (*)[6])as->edges, as->players, as->player_count);
    if (now - last > 999999999) {
        last = now;
        SDL_snprintf(debug_string, sizeof(debug_string), "%" SDL_PRIu64 " fps", accu);
        accu = 0;
    }
    past = now;
    accu += 1;
    Uint64 elapsed = SDL_GetTicksNS() - now;
    if (elapsed < 999999) {
        SDL_DelayNS(999999 - elapsed);
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_free(appstate); // just free the memory, SDL will clean up the window/renderer for us.
}
