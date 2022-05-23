#ifndef _SLIDER
#define _SLIDER




#include <Graphics/graphics.h>

typedef struct slider_t
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


	struct {
        SDL_Texture *texture;
	    union
        {
            struct {

                vector2_t pos;
                vector2_t scale;
            };

            rect_t  rect;
        };
	} sliderhandle;

	double floatlift;
	SDL_bool selected;
	SDL_bool floateffect;

	SDL_bool disabled;

} slider_t;





#define SLIDER_HEIGHT (10 * 12)
#define SLIDER_WIDTH  (40 * 12)

#define SLIDERHANDLE_WIDTH  (8 * 9)
#define SLIDERHANDLE_HEIGHT (8 * 9)




/**
    Convinence fuctions
**/

float update_slider(slider_t *slider);
void draw_slider(slider_t *slider);


/**
    Allocation functoins
**/
void alloc_slider(int x, int y, float value, SDL_bool floateffect, slider_t **slider);
void delete_slider(slider_t *slider);




#endif
