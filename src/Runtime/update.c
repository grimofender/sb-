#include <Graphics/graphics.h>

extern SDL_Renderer* renderer;
extern SDL_Window* window;
#include <SDL2/SDL.h>

#include <Runtime/Player/Player.h>
#include <Runtime/Enemy/enemy.h>
#include <Runtime/Music/music.h>
#include <Runtime/Scenes/scene.h>
#include <Application/appdata.h>

#include <Runtime/MenuItems/button.h>

#include <SDL2/SDL_ttf.h>


enemyskin_t *enemyskin;

SDL_Joystick *joystick;

SDL_Cursor *cursor;

extern int frames;

SDL_Color fontcolor = {0,19,38};
TTF_Font *font;

int playerskin;


void setup_startanim();
void update_startanim();
void draw_startanim();
void delete_startanim();

void setup_startmenu();
void update_startmenu();
void draw_startmenu();
void delete_startmenu();

void setup_arena();
void update_arena();
void draw_arena();
void delete_arena();

void setup_settingsmenu();
void update_settingsmenu();
void draw_settingsmenu();
void delete_settingsmenu();

void setup_gameover();
void update_gameover();
void draw_gameover();
void delete_gameover();


void setup_server();
void update_server();
void draw_server();
void delete_server();

#include <Runtime/Scenes/scene.h>


void update_scene(gamescene_t scene);
void draw_scene(gamescene_t scene);



/** This Function is called once at the start of the game **/
int setup()
{

    for (int i = 0; i < PLAYERSKIN_COUNT; i++)
    {
        char skinname[256];

        sprintf(skinname, "%d", i);
        alloc_playerskin(skinname, playerskins + i);
    }

    for (int i = 0; i < ENEMYSKIN_COUNT; i++)
    {
        char skinname[256];

        sprintf(skinname, "%d", i);
        alloc_enemyskin(skinname, enemyskins + i);
    }




    if (!font)
        font = TTF_OpenFont("data/menu/pixelated_princess.ttf", 47);
    assert(font, 0);

    christmastimeishere = Mix_LoadMUS("data/sound/ChristmasTimeishere.wav");
    player_hurt         = Mix_LoadWAV("data/sound/player_hurt.wav");
    button_click        = Mix_LoadWAV("data/sound/button_click.wav");
    button_disabled     = Mix_LoadWAV("data/sound/button_disabled.wav");
    coin_pickup         = Mix_LoadWAV("data/sound/coin_pickup.wav");

    /** these scenes must be registered for the game to work properly**/
    registerscene(gamescene_null, (scene_setup_t)NULL, (scene_update_t)NULL, (scene_draw_t)NULL, (scene_delete_t)NULL); /** no functions assigned to the null scene **/
    registerscene(gamescene_server, setup_server, update_server, draw_server, delete_server);


    /** Normal scenes **/
    registerscene(gamescene_startanim, setup_startanim, update_startanim, draw_startanim, delete_startanim);
    registerscene(gamescene_startmenu, setup_startmenu, update_startmenu, draw_startmenu, delete_startmenu);
    registerscene(gamescene_arena, setup_arena, update_arena, draw_arena, delete_arena);
    registerscene(gamescene_settingsmenu, setup_settingsmenu, update_settingsmenu, draw_settingsmenu, delete_settingsmenu);
    registerscene(gamescene_gameover, setup_gameover, update_gameover, draw_gameover, delete_gameover);


    pushscene(gamescene_startanim);



}


extern int capfps;
#include <math.h>

extern int fps;

/** this function is called every frame **/
int frame(gamescene_t scene)
{  
    if (!rendertarget) {
        rendertarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, DEFAULTRESELUTION_WIDTH, DEFAULTRESELUTION_HEIGHT);
    }
    
    

        //assert(!(SDL_GetError()[0]), 0);
    if(!joystick)
    {



        // Get Controller if there is one
        for  (int i = 0; i < SDL_NumJoysticks(); i++)
        {
        if (!SDL_IsGameController(i)) continue;
            joystick = SDL_JoystickOpen(i);

            assert(joystick, 0);

            if (joystick)
                break;
            else
                err(SDL_GetError());
        }

    }



    if (joystick)
    if (!SDL_JoystickGetAttached(joystick))
    {
        SDL_JoystickClose(joystick);
        joystick = NULL;
    }

    SDL_SetRenderTarget(renderer, rendertarget);
    clear(0xff, 0xff, 0xff, 0xff);
    update_scene(scene);
    draw_scene(scene);
    SDL_SetRenderTarget(renderer, NULL);
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    rect_t rec; rec.x = 0; rec.y = 0; rec.w = WINDOW_WIDTH; rec.h = WINDOW_HEIGHT;
    drawtexture(rendertarget, NULL, 0);
    SDL_RenderPresent(renderer);


    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    assert(keystate, 0);





    if (keystate[SDL_SCANCODE_ESCAPE])
    {
        SDL_Event event;
        SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, 0, 0);

        event.type = SDL_QUIT;
        event.quit.type = SDL_QUIT;
        event.quit.timestamp = SDL_GetTicks();

        SDL_PushEvent(&event);

    }




}

