#include <Graphics/graphics.h>

#include <Runtime/Scenes/scene.h>
#include <Runtime/MenuItems/button.h>


#include <Application/appdata.h>

#include <Runtime/Player/Player.h>

#include <Runtime/Music/music.h>


static SDL_Texture *logo;

static rect_t  LogoRect;
static double LogoAngle;



static button_t  *playbutton;
static button_t  *settingsbutton;

extern SDL_Cursor *cursor;

extern playerskin_t *playerskins[];

extern SDL_Color fontcolor;
extern TTF_Font *font;

static button_t *FreizKingLogo;


extern int playerskin;
rect_t  PlayerRect;
rect_t  PlayerEyesRect;
static SDL_bool PlayingPlayerAnimation;

static int PlayerFrame;

static char* strinfo;

static SDL_bool LogoAnimPlaying;
static float logosizemultiplyer;
static SDL_bool LogoAnimHasPlayed = SDL_FALSE;

vector2_t vec;
vector2_t vec2;
static double fadeinalpha;

void setup_startmenu()
{




            if (!strinfo)
            strinfo = calloc(256, sizeof(char));

            sprintf(strinfo, "SnowBrawl+ %d.%d.%d, FreizKing Entertainment", appversion.major, appversion.minor, appversion.patch);


            SDL_Surface *surface = loadbmp("data/sprites/cursor.bmp");
            if (cursor)
            SDL_FreeCursor(cursor);
            cursor = SDL_CreateColorCursor(surface, 0, 0);
            SDL_FreeSurface(surface);
            SDL_SetCursor(cursor);



                    if (!logo)
                    logo = loadtexture("data/menu/Logo.bmp");

             LogoRect = (rect_t ){(reselution_width / 2)  + 50,  0, 88 * 10, 40 * 10};
             center(&LogoRect, &LogoRect);
             middle(&vec, &LogoRect);

             PlayerRect = (rect_t ){(reselution_width / 2), (reselution_height / 2) + 70, PLAYERDEFAULT_WIDTH, PLAYERDEFAULT_HEIGHT};
             center(&PlayerRect, &PlayerRect);
             middle(&vec2, &PlayerRect);

             PlayerEyesRect = (rect_t ){(reselution_width / 2), (reselution_height / 2), PLAYER_EYESDEFAULT_WIDTH, PLAYER_EYESDEFAULT_HEIGHT};
             center(&PlayerEyesRect, &PlayerEyesRect);




                if (!playbutton)
        alloc_button((reselution_width / 2) - 120,  (reselution_height / 2) + 411, 12 * 12, 11 * 12, "data/menu/play.bmp", SDL_TRUE, &playbutton);

         if (!settingsbutton)
        alloc_button((reselution_width / 2) + 120,  (reselution_height / 2) + 414, 11 * 12, 11 * 12, "data/menu/settings.bmp", SDL_TRUE, &settingsbutton);

           if (!FreizKingLogo)
        alloc_button(reselution_width - 120, reselution_height - 120, 29 * 6, 31 * 6, "data/sprites/FreizKingLogo.bmp", SDL_TRUE, &FreizKingLogo);


        if (!Mix_PlayingMusic())
        Mix_PlayMusic(christmastimeishere, -1);

        if (!LogoAnimHasPlayed)
        {
            LogoAnimHasPlayed = SDL_TRUE;
            LogoAnimPlaying = SDL_TRUE;
            logosizemultiplyer = 9000;
            fadeinalpha = 255;
        }



}

extern int frames;
#include <math.h>
#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif



void update_startmenu()
{
        fadeinalpha -= 0.05 * deltatime;
        if (fadeinalpha < 0)
            fadeinalpha = 0;

        Uint8* keystate = SDL_GetKeyboardState(NULL);
        assert(keystate,0);
            static double vel;

        if (update_button(playbutton))
        {
            PlayingPlayerAnimation = SDL_TRUE;

            vel = -1.84;
            playbutton->disabled = SDL_TRUE;
            settingsbutton->disabled = SDL_TRUE;
            FreizKingLogo->disabled = SDL_TRUE;



            return;
        }

        if (update_button(settingsbutton))
        {
            pushscene(gamescene_settingsmenu);

            return;
        }

        if (update_button(FreizKingLogo))
        {



            #if defined(_WIN32)
                #if defined(DEBUG) && DEBUG == 1
                system("start https://scratch.mit.edu/users/FriezKing");
                #else
                WinExec("start https://scratch.mit.edu/users/FriezKing", SW_HIDE);
                #endif
            #elif defined(__APPLE__)
            system("open https://scratch.mit.edu/users/FriezKing");
            #elif defined(__linux__)
            system("xdg-open https://scratch.mit.edu/users/FriezKing");
            #elif defined(EMSCRIPTEN)
            emscripten_run_script("window.open(\"https://scratch.mit.edu/users/FriezKing\")");
            #endif
        }

    if (LogoAnimPlaying)
    {


        if (logosizemultiplyer > 100)
        logosizemultiplyer -= 1 * deltatime;
        else
        {
            LogoAnimPlaying = SDL_FALSE;
        }


        if (keystate[SDL_SCANCODE_RETURN])
        {
            LogoAnimPlaying = SDL_FALSE;
            fadeinalpha = 0;
        }


    }








     if (!Mix_PlayingMusic())
         Mix_FadeInMusic(christmastimeishere, -1, 2000);

    if (PlayingPlayerAnimation)
    {






        PlayerRect.y  += vel * deltatime;

        vector2_t p; middle(&p, &PlayerRect);
		PlayerEyesRect.x =  (p.x - (PLAYER_EYESDEFAULT_WIDTH / 2)) + (PLAYER_EYESRADIUS * SDL_cos(atan2(1, 0)));
		PlayerEyesRect.y =  (p.y - (PLAYER_EYESDEFAULT_HEIGHT / 2)) - 40 + (PLAYER_EYESRADIUS * SDL_sin(atan2(1, 0)));


        vel += 0.006 * deltatime;

        if (vel > 0)
            PlayerFrame = 2;
        else
            PlayerFrame = 1;







        if (PlayerRect.y > reselution_height)
        {
             PlayingPlayerAnimation = 0;
             vel = 0;
             pushscene(gamescene_arena);
            playbutton->disabled = SDL_FALSE;
            settingsbutton->disabled = SDL_FALSE;
            FreizKingLogo->disabled = SDL_FALSE;
        }

    }
    else
    {



        vector2_t p; middle(&p, &PlayerRect);
         vector2_t facingdirection; getmousepos(&facingdirection.x, &facingdirection.y);

        double lookingangle = atan2((facingdirection.y - PlayerRect.y), (facingdirection.x - PlayerRect.x));
		PlayerEyesRect.x =  (p.x - (PLAYER_EYESDEFAULT_WIDTH / 2)) + (PLAYER_EYESRADIUS * SDL_cos(lookingangle));
		PlayerEyesRect.y =  (p.y - (PLAYER_EYESDEFAULT_HEIGHT / 2)) - 40 + (PLAYER_EYESRADIUS * SDL_sin(lookingangle));



        vector2_t mp; getmousepos(&mp.x, &mp.y);
        mp.x += vec2.x;
        mp.y += vec2.y;
        mp.x /= 80;
        mp.y /= 90;
        mp.x += vec2.x;
        mp.y += vec2.y + (cos(SDL_GetTicks() * 6.28) * 12);
        center(&PlayerRect, &mp);


        for (int i = 0; i < PLAYERSKIN_COUNT; i++)
        {
            char key[256] = {0};
            sprintf(key, "%d", i);
            if (keystate[SDL_GetScancodeFromName(key)])
            {
                playerskin = i;
            }
        }




    }


        vector2_t mp; getmousepos(&mp.x, &mp.y);
        mp.x += vec.x;
        mp.y += vec.y;
        mp.x /= 80;
        mp.y /= 80;
        mp.x += vec.x;
        mp.y += (LogoRect.h / 2) + (cos(SDL_GetTicks() * 6.28) * 20) + 25;

        center(&LogoRect, &mp);




        LogoAngle = ((sin(SDL_GetTicks() * 6.28) * 3));

}


void draw_startmenu()
{



        rect_t  r = LogoRect;

        draw_text(0, reselution_height - 46, strinfo, font, fontcolor, 0);

        if (!LogoAnimPlaying)
        drawtexture(logo, &LogoRect, LogoAngle);



        draw_button(playbutton);
        draw_button(settingsbutton);
        draw_button(FreizKingLogo);
        SDL_Color a = {0, 0, 0, 0xff};

        drawtexture(playerskins[playerskin]->player[PlayerFrame], &PlayerRect, 0);
        drawtexture(playerskins[playerskin]->eyes, &PlayerEyesRect, 0);



        if (LogoAnimPlaying)
        {
            rect_t r = LogoRect;

            r.w *= logosizemultiplyer / 100;
            r.h *= logosizemultiplyer / 100;

            vector2_t v;

            middle(&v, &LogoRect);

            center(&r, &v );
            drawtexture(logo, &r, LogoAngle);

        }

        SDL_SetRenderDrawColor(renderer, 0, 19, 38, fadeinalpha);
        SDL_RenderFillRect(renderer, NULL);



}

void delete_startmenu()
{
    if (playbutton)
    {
        delete_button(playbutton);
        playbutton = 0;
    }

    if (settingsbutton)
    {
        delete_button(settingsbutton);
        settingsbutton = 0;
    }
    if (FreizKingLogo)
    {
        delete_button(FreizKingLogo);
        FreizKingLogo = 0;
    }



    if (logo)
    {
        SDL_DestroyTexture(logo);
        logo = 0;
    }

    if (strinfo)
    {
        free(strinfo);
        strinfo = 0;
    }



    LogoRect = (rect_t ){0};
    LogoAngle = 0;
    PlayingPlayerAnimation = 0;
    PlayerFrame = 0;



}
