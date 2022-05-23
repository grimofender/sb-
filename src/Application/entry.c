#define SDL_MAIN_HANDLED

#ifdef  __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <stdlib.h>

#include <Graphics/graphics.h>


SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
int fps;
int frames;

int capfps = -1;
SDL_bool vsync;

long double deltatime;



int console(void* data);


#include <Runtime/Multiplayer/multiplayer.h>
#include <SDL2/SDL_mixer.h>
#include <Runtime/Scenes/scene.h>

#include <Application/appdata.h>

#if defined(_WIN32) || defined(__linux__)

#include <stdio.h>

#include <signal.h>


int sigerr(int signal)
{
    switch (signal)
    {
        case SIGFPE:
            err("Floating Point error");
        case SIGILL:
            err("Illegal Instruction");
            break;
        case SIGSEGV:
            err("Segmentation Error");
            break;

    }

    err("Invalid Signal");
}

#endif

void SDLCALL logoutputfunc(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    switch (priority)
    {
        case SDL_LOG_PRIORITY_CRITICAL:
            printf("CRITICAL: %s\n", message);
            break;
        case  SDL_LOG_PRIORITY_DEBUG:
            printf("DEBUG: %s\n");
            break;
         case SDL_LOG_PRIORITY_ERROR:
            printf("ERROR: %s\n", message);
            break;
        case SDL_LOG_PRIORITY_INFO:
            printf("INFO: %s\n", message);
            break;
        case SDL_LOG_PRIORITY_WARN:
            printf("WARNING: %s\n", message);
            break;
        case SDL_LOG_PRIORITY_VERBOSE:
            printf("VERBOSE: %s\n", message);
            break;

    }

}

#if _WIN32
#include <sdkddkver.h>
#endif
#include <stdio.h>



extern int setup();
#include <Runtime/Scenes/scene.h>
#include <Graphics/graphics.h>
extern int frame(gamescene_t scene);

extern void load_scene(gamescene_t scene);
extern void delete_scene(gamescene_t scene);


Uint32 beginframe = 0;
SDL_Event event;
Uint32 endtime = 0;
gamescene_t backup = 0;
void iteration(void *__arg2);
int main(int argc, char *argv[])
{

    #ifndef BUILDING_FOR_WEB
    #if defined(DEBUG) && DEBUG == 1
        SDL_CreateThread(console, "console", NULL);
        #ifdef _WIN32
            if (!GetConsoleWindow())
            {
                AllocConsole();
                freopen("CON", "w", stdout);
                freopen("CON", "r", stdin);
                freopen("CON", "w", stderr);
            }
        #endif // _WIN32
    #endif
    #endif




	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	TTF_Init();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    #if defined(_WIN32) || defined(__linux__)
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(-1,2), &wsaData) != 0)
        err("Could not initialize WSA\n errorcode: %x", WSAGetLastError());
    #endif
    connection.sock = INVALID_SOCKET;
        signal(SIGSEGV, sigerr);
        signal(SIGILL, sigerr);
        signal(SIGFPE, sigerr);
        signal(SIGTERM, sigerr);
    #endif

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);
	SDL_LogSetOutputFunction(logoutputfunc, NULL);
    SDL_CaptureMouse(SDL_FALSE);
    window = SDL_CreateWindow("Snow Brawl+", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
    assert(window, 0);

    renderer  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    assert(renderer, 0);

    SDL_RenderSetLogicalSize(renderer, reselution_width, reselution_height);
    SDL_LogSetOutputFunction(logoutputfunc, NULL);

    SDL_Surface *s = loadbmp("Icon.bmp");
    SDL_SetWindowIcon(window, s);
    SDL_FreeSurface(s);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_TIME_CRITICAL);

	#ifdef _WIN32
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	#endif // _WIN32
	setup();

    SDL_ShowWindow(window);
    backup = getscene();
    endtime = SDL_GetTicks() + 1000;
    #ifdef  __EMSCRIPTEN__
    emscripten_set_main_loop_arg(iteration, NULL, 60, SDL_TRUE);
    #else 
    for (;;) iteration(NULL);
    #endif

}

void iteration(void *__arg2) {
    deltatime = (long double)((SDL_GetTicks() - beginframe));
    beginframe = SDL_GetTicks();
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        exit(0);
    }

    if (getscene() != backup) {
        delete_scene(backup);
        load_scene(getscene());
        backup = getscene();
    }


	//update scene
	frame(backup);
	++frames;

    
    if (SDL_GetTicks() > endtime) {
        fps = frames;
		frames = 0;
		endtime = (SDL_GetTicks() + 1000);
	}

}