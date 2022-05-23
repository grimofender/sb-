#include <Graphics/graphics.h>
#include <Runtime/Scenes/scene.h>
#include <Runtime/Music/music.h>

static Uint32 logoapperence;

static rect_t logorect;
static SDL_Texture *logo;

extern SDL_Cursor *cursor;

SDL_bool soundplayed;

static float logo_vel;

static SDL_bool fadedin;

static float savey;






#define LOGO_FADEIN 1
#define LOGO_FADEOUT 1


void setup_startanim()
{


        SDL_Surface *surface = loadbmp("data/sprites/cursor.bmp");


        if (cursor)
        SDL_FreeCursor(cursor);
        cursor = SDL_CreateColorCursor(surface, 0, 0);
        SDL_FreeSurface(surface);
        SDL_SetCursor(cursor);

        if (!logo)
        logo = loadtexture("data/menu/devlogo.bmp");


        logoapperence = SDL_GetTicks() + 1000;

        logorect = (rect_t){reselution_width / 2, reselution_height / 2, 82 * 8, 49 * 8};
        center(&logorect, &logorect);
        savey = logorect.y;

        soundplayed = SDL_FALSE;
        Mix_HaltMusic();

        SDL_SetTextureAlphaMod(logo, 0);

        logo_vel = -0.71;
}

void update_startanim()
{


    if (SDL_GetTicks() > logoapperence)
    {


        if (!soundplayed)
        {
            Mix_PlayChannel(-1,  coin_pickup, 0);
            soundplayed = SDL_TRUE;
        }

        logorect.y += logo_vel * deltatime;

        if (logorect.y <= savey)
            logo_vel += 0.006 * deltatime;
        else
            logo_vel = 0;






        Uint8 alpha;

        SDL_GetTextureAlphaMod(logo, &alpha);
        if (!fadedin)
        {
            if (alpha + (LOGO_FADEIN * deltatime) > 255)
            {
                alpha = 255;
                fadedin = SDL_TRUE;
            }
            else
                alpha += LOGO_FADEIN * deltatime;


        }
        else
        {
            if (SDL_GetTicks() > logoapperence + 2000)
                if (alpha - (LOGO_FADEOUT * deltatime) < 0)
                    alpha = 0;
                else
                    alpha -= LOGO_FADEOUT * deltatime;



        }


        SDL_SetTextureAlphaMod(logo, alpha);




    }

    Uint8* keystate = SDL_GetKeyboardState(NULL);
    assert(keystate, 0);

    if (keystate[SDL_SCANCODE_RETURN])
        pushscene(gamescene_startmenu);


    if (SDL_GetTicks() > logoapperence + 3000)
        pushscene(gamescene_startmenu);




}

void draw_startanim()
{
    clear(0,19,38, 0xff);
    drawtexture(logo, &logorect, 0.0f);
}

void delete_startanim()
{
    if (logo)
    {
        SDL_DestroyTexture(logo);
        logo = 0;
    }

}
