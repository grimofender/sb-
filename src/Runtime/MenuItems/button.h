#ifndef _BUTTON
#define _BUTTON

#include <Graphics/graphics.h>
#include <SDL2/SDL_ttf.h>

typedef struct button_t
{
    SDL_Texture *texture;

    union
	{
		struct {

			vector2_t pos;
			vector2_t scale;
		};

		rect_t  rect;
	};

	double rise;
	double floatlift;
	SDL_bool selected;
	SDL_bool floateffect;

	SDL_bool disabled;

} button_t;




/**
    Convinence fuctions
**/

SDL_bool update_button(button_t *button);
void draw_button(button_t *button);


/**
    Allocation functoins
**/
void alloc_button(int x, int y, int w, int h, const char *texturepath, SDL_bool floateffect, button_t **button);
void delete_button(button_t *button);




#endif // _BUTTON

