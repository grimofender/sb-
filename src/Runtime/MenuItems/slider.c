
#include <Runtime/MenuItems/slider.h>
#include <Runtime/Music/music.h>



void alloc_slider(int x, int y, float value, SDL_bool floateffect, slider_t **slider)
{
     assert(slider, "Invalid Arguments");
    (*slider) = calloc(1, sizeof(slider_t));
    (*slider)->texture = loadtexture("data/menu/slider.bmp");

    (*slider)->rect = (rect_t ){x, y, SLIDER_WIDTH, SLIDER_HEIGHT};


    center(&(*slider)->rect, &(*slider)->rect);


    (*slider)->sliderhandle.texture = loadtexture("data/menu/sliderhandle.bmp");
    (*slider)->sliderhandle.scale = (vector2_t){SLIDERHANDLE_WIDTH, SLIDERHANDLE_HEIGHT};



    middle(&((*slider)->sliderhandle.pos),  &(*slider)->rect);


    center(&(*slider)->sliderhandle.rect, &(*slider)->sliderhandle.rect);

    float size = (((*slider)->pos.x + (*slider)->scale.x) - ((*slider)->sliderhandle.scale.x / 2)) - ((*slider)->pos.x + ((*slider)->sliderhandle.scale.x / 2));

    (*slider)->sliderhandle.pos.x = (*slider)->pos.x + ((value / 100) * size);

    (*slider)->floateffect = floateffect;


}


void delete_slider(slider_t *slider)
{
    assert(slider, "Invalid Arguments");
    if (slider->texture)
    SDL_DestroyTexture(slider->texture);

    slider->texture  = 0;




    free(slider);
}

float update_slider(slider_t *slider)
{
        assert(slider, "Invalid Arguments");
        float x, y;
        getmousepos(&x, &y);

        rect_t  mouse = {x, y, 0.1,0.1};

        slider->selected = hasintersection(&mouse, &slider->rect);



        if (slider->selected)
        {

            SDL_SetTextureAlphaMod(slider->sliderhandle.texture,125);

            if (SDL_BUTTON(SDL_GetMouseState(0, 0)) == SDL_BUTTON_LEFT)
            {
                slider->sliderhandle.pos.x = x - (slider->sliderhandle.scale.x / 2);


                vector2_t mid;

                middle(&mid, &slider->sliderhandle.rect);

                if (mid.x < (slider->pos.x + (slider->sliderhandle.scale.x / 2)))
                    mid.x = (slider->pos.x + (slider->sliderhandle.scale.x / 2));

                if (mid.x > ((slider->pos.x + slider->scale.x) - (slider->sliderhandle.scale.x / 2)))
                    mid.x = ((slider->pos.x + slider->scale.x) - (slider->sliderhandle.scale.x / 2));

                center(&slider->sliderhandle.rect, &mid);



            }


        }
        else
        {
            SDL_SetTextureAlphaMod(slider->sliderhandle.texture, 255);
        }

        float size = ((slider->pos.x + slider->scale.x) - (slider->sliderhandle.scale.x / 2)) - (slider->pos.x + (slider->sliderhandle.scale.x / 2));
        vector2_t mid;


        middle(&mid, &slider->sliderhandle.rect);


        float length = (mid.x - (slider->pos.x + (slider->sliderhandle.scale.x / 2))) / size;

        return length;


}

void draw_slider(slider_t *slider)
{
    assert(slider, "Invalid Arguments");
    drawtexture(slider->texture, &slider->rect, 0);
    drawtexture(slider->sliderhandle.texture, &slider->sliderhandle.rect, 0);
}

