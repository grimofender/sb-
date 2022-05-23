#include <Graphics/graphics.h>



#include <Runtime/Player/Player.h>
#include <Runtime/Enemy/enemy.h>
#include <Runtime/Scenes/scene.h>


static player_t *player;
extern playerskin_t *playerskins[];


#define MAX_ENEMYS 10
#define ENEMYSPAWNDURATION 2000


static enemy_t *enemys[MAX_ENEMYS];

extern SDL_Joystick *joystick;
extern SDL_Haptic *haptic;

extern SDL_Cursor *cursor;

static Uint32 enemyspawntime;

int enemyskilled;

extern int playerskin;
extern int enemyskin;

void setup_arena()
{

    alloc_player(reselution_width / 2, reselution_height / 2, playerskin, &player);


    SDL_Surface *surface = loadbmp("data/sprites/crosshair.bmp");
    if (cursor)
        SDL_FreeCursor(cursor);
    cursor = SDL_CreateColorCursor(surface, (surface->w / 2), (surface->h / 2));
    SDL_FreeSurface(surface);
    SDL_SetCursor(cursor);

    for (int i = 0; i < MAX_ENEMYS; i++)
    {
        if (enemys[i] == 0)
            continue;


            delete_enemy(enemys[i]);
            enemys[i] = 0;

    }


    enemyspawntime = 0;

    Mix_FadeOutMusic(2000);

}

extern int fps;
extern TTF_Font *font;
extern SDL_Color fontcolor;

#include <math.h>


void update_arena()
{
    playerinput_t input;




    if (!joystick || !SDL_JoystickGetAttached(joystick))
    get_playerinput(player, inputtype_keyboard_and_mouse, 0, &input);
    else
    get_playerinput(player, inputtype_controller, joystick, &input);


    update_player(&input, player);



    if (player->death.dead)
    {
        pushscene(gamescene_gameover);
        return;
    }


    for (int i = 0; i < MAX_ENEMYS; i++)
    {
        if (enemys[i] == 0)
            continue;
        if (enemys[i]->destroyed)
        {
            ++enemyskilled;
            delete_enemy(enemys[i]);
            enemys[i] = 0;
            continue;
        }


        update_enemy(enemys[i]);

    }

    if (SDL_GetTicks() > enemyspawntime)
    {
        for (int i = 0; i < MAX_ENEMYS; i++)
        {
           if (enemys[i] == 0)
           {
               alloc_enemy(enemyskin, &player, 1, &enemys[i]);
               break;
           }




        }

        int minusdiration =  (enemyskilled * 300)<1500? (enemyskilled * 300) : 1500;




        enemyspawntime = SDL_GetTicks() + (ENEMYSPAWNDURATION - minusdiration);



    }





}


void draw_arena()
{
    for (int i = 0; i < MAX_ENEMYS; i++)
    {
        if (enemys[i] == 0)
            continue;
        draw_enemy(enemys[i]);

    }

    draw_player(player);

}

void delete_arena()
{
    enemyskilled = 0;
        if (player)
            {
                delete_player(player);
                player = 0;
            }

    for (int i = 0; i < MAX_ENEMYS; i++)
    {
        if (enemys[i] == 0)
            continue;


            delete_enemy(enemys[i]);
            enemys[i] = 0;

    }

}
