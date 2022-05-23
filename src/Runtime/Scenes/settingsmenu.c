
#include <Graphics/graphics.h>

#include <Runtime/Music/music.h>


#include <Runtime/MenuItems/button.h>
#include <Runtime/Scenes/scene.h>
#include <Runtime/MenuItems/slider.h>

extern TTF_Font *font;
static SDL_Color fontcolor;

static button_t *homebutton;

static slider_t *volumeslider;

static SDL_Texture *volume1;
static SDL_Texture *volume2;
static SDL_Texture *volume3;




void setup_settingsmenu()
{
    if (!homebutton)
            alloc_button(0 + 80,  reselution_height - 80, 11 * 12, 11 * 12, "data/menu/home.bmp", SDL_TRUE, &homebutton);
    if (!volumeslider)
            alloc_slider(reselution_width / 2, reselution_height / 2, ((double)Mix_VolumeMusic(-1) / (double)MIX_MAX_VOLUME) * 100,SDL_TRUE, &volumeslider);

    volume1 = loadtexture("data/menu/sound (none).bmp");
    volume2 = loadtexture("data/menu/sound (inbetween).bmp");
    volume3 = loadtexture("data/menu/sound (full).bmp");
}


void update_settingsmenu()
{
    if (update_button(homebutton))
        popscene();


    Mix_VolumeMusic(update_slider(volumeslider) * MIX_MAX_VOLUME);






}


void draw_settingsmenu()
{
  draw_button(homebutton);
  draw_text(0,0, "settings", font, fontcolor, 0);
  draw_slider(volumeslider);


    rect_t volumestate = (rect_t){(reselution_width / 2) - 360, reselution_height / 2, 14 * 12, 11 * 12};
    center(&volumestate,&volumestate);
  if (((float)Mix_VolumeMusic(-1) / (float)MIX_MAX_VOLUME) == 0)
      drawtexture(volume1, &volumestate, 0);
  else if (((float)Mix_VolumeMusic(-1) / (float)MIX_MAX_VOLUME) == 1)
      drawtexture(volume3, &volumestate, 0);
  else
      drawtexture(volume2, &volumestate, 0);










}

void delete_settingsmenu()
{
    if (homebutton)
    {
        delete_button(homebutton);
        homebutton = 0;
    }

    if (volumeslider)
    {
        delete_slider(volumeslider);
        volumeslider = 0;
    }

    if (volume1)
    {
        SDL_DestroyTexture(volume1);
        volume1 = 0;
    }

    if (volume2)
    {
         SDL_DestroyTexture(volume2);
        volume2 = 0;
    }

    if (volume3)
    {
         SDL_DestroyTexture(volume3);
        volume3 = 0;
    }
}
