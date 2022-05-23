#include <Graphics/graphics.h>
#include <Runtime/MenuItems/button.h>
#include <Runtime/Scenes/scene.h>

#include <Runtime/Music/music.h>


static SDL_Texture *background;

static double grayshade;


static SDL_Texture *gameover;
static rect_t gameoverrect;
static double gameoverangle;

extern SDL_Cursor *cursor;


static button_t *homebutton;
static button_t *replaybutton;



void setup_gameover()
{
    SDL_Surface *surfacecursor = loadbmp("data/sprites/cursor.bmp");
    if (cursor)
    SDL_FreeCursor(cursor);
    cursor = SDL_CreateColorCursor(surfacecursor, 0, 0);
    SDL_FreeSurface(surfacecursor);
    SDL_SetCursor(cursor);




    background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, DEFAULTRESELUTION_WIDTH, DEFAULTRESELUTION_HEIGHT);
    SDL_SetRenderTarget(renderer, background);
    drawtexture(rendertarget, NULL, 0);
    SDL_SetRenderTarget(renderer, NULL);
    grayshade = 255;

    gameoverrect =  (rect_t ){(reselution_width / 2),  0, 129 * 10.35, 22 * 10.5};
    center(&gameoverrect, &gameoverrect);


    gameover = loadtexture("data/menu/gameover.bmp");

    if (!homebutton)
        alloc_button((reselution_width / 2) - 140,  (reselution_height / 2) + 351, 11 * 12, 11 * 12, "data/menu/home.bmp", SDL_TRUE, &homebutton);
    if (!replaybutton)
        alloc_button((reselution_width / 2) + 140,  (reselution_height / 2) + 351, 14 * 12, 11 * 12, "data/menu/replay.bmp", SDL_TRUE, &replaybutton);


    SDL_SetTextureAlphaMod(gameover, 0);

    Mix_FadeOutMusic(2000);
    Mix_FadeInMusic(christmastimeishere, -1, 3000);

}

void update_gameover()
{
    if (update_button(homebutton))
        gotoscene(gamescene_startmenu);



    if (update_button(replaybutton))
    {
            gotoscene(gamescene_arena);
    }




    if (grayshade > 127)
    {
        grayshade -= 0.1 * deltatime;
        SDL_SetTextureColorMod(background, grayshade, grayshade, grayshade);
    }


    Uint8 a;
    SDL_GetTextureAlphaMod(gameover, &a);

    if (((double)a + (1 * deltatime)) < 255)
    {
        a += 1 * deltatime;
        SDL_SetTextureAlphaMod(gameover, a);
    }
    else
    {

        a = 255;
    }


    gameoverrect.y = ((cos(SDL_GetTicks() * 6.28) * 20) + 50);
    gameoverangle = ((sin(SDL_GetTicks() * 6.28) * 1.5));


}

void draw_gameover()
{


    drawtexture(background, 0, 0);
    drawtexture(gameover, &gameoverrect, gameoverangle);

    draw_button(homebutton);
    draw_button(replaybutton);

}

void delete_gameover()
{

    if (!homebutton)
    {
        delete_button(homebutton);
        homebutton = 0;
    }

    if (!replaybutton)
    {
        delete_button(replaybutton);
        replaybutton = 0;
    }

    if (background)
        SDL_DestroyTexture(background);
    background = NULL;


    if (gameover)
        SDL_DestroyTexture(gameover);
    gameover = 0;

}
