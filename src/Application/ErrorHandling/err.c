
#include "err.h"
#include <signal.h>

#include <SDL2/SDL_bits.h>
#include <SDL2/SDL.h>

extern SDL_Window *window;

#include <errno.h>
#include <Runtime/Scenes/scene.h>
#include <string.h>

#include <Graphics/graphics.h>
#include <Runtime/MenuItems/button.h>
#include <Runtime/Music/music.h>


SDL_AssertState assert_handle(const SDL_AssertData* data, void* userdata)
{
    if (userdata)
    SDL_SetError(userdata);

    err("ASSERTION ERROR: \nline: %d\nfile: %s\nfunction: %s\nsupposed error: %s\n", data->linenum, data->filename, data->function, SDL_GetError());
}


void err( const char* MSG, ...)
{

	char* message = calloc(256, sizeof(char));
	char* erroutput = calloc(1000, sizeof(char));

	Mix_HaltMusic();


	va_list _ArgList;
	va_start(_ArgList, MSG);
	vsprintf(message, MSG, _ArgList);

	SDL_LogError(SDL_LOG_CATEGORY_ERROR, message);


	va_end(_ArgList);




	sprintf(erroutput, "A hard error has occured. \n show the message below  to one of the developers,\n they may be able to fix it.\n\n%s", message);

	#if __WIN32S
	PlaySound("MouseClick", NULL, SND_ASYNC);
	#endif
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SnowBrawl+", erroutput, window);


    //raise(SIGABRT);
	exit(-1); /** we should never reach this line **/

}

char *lasterror;

void soft_err(const char* MSG, ...)
{


    if (!lasterror)
        lasterror = calloc(256, sizeof(char));
    else
        memset(lasterror, 0,strlen(lasterror) + 1);


    va_list _ArgList;
	va_start(_ArgList, MSG);
	vsprintf(lasterror, MSG, _ArgList);
	va_end(_ArgList);

    SDL_LogError(SDL_LOG_CATEGORY_ERROR, lasterror);

	pushscene(gamescene_error);

}


extern SDL_Color fontcolor;
extern TTF_Font *font;

static button_t *homebutton;

void setup_error()
{
     if (!homebutton)
        alloc_button(0 + 80,  reselution_height - 80, 11 * 12, 11 * 12, "data/menu/home.bmp", SDL_TRUE, &homebutton);

    if (!Mix_PlayingMusic())
    Mix_FadeInMusic(christmastimeishere, -1, 2000);

}

void update_error()
{

    if (update_button(homebutton))
    {
        gotoscene(gamescene_startmenu);
    }

}

void draw_error()
{


    draw_text(reselution_width / 2, 300, "An error has occured.", font, fontcolor, 1);
    draw_text(reselution_width / 2, 347, "Try restarting the app, that will fix things (hopefully)", font, fontcolor, 1);
    draw_text(reselution_width / 2, 394, "Also here's the error message if you wan't to search it up yourself", font, fontcolor, 1);

    draw_text(reselution_width / 2, (reselution_height / 2) - 100, lasterror, font, fontcolor, 1);

    draw_button(homebutton);

}

void delete_error()
{
    if (homebutton)
    {
        delete_button(homebutton);
        homebutton = 0;
    }

}
