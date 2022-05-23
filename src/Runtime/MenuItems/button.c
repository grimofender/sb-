#include <Runtime/MenuItems/button.h>
#include <Runtime/Music/music.h>

void alloc_button(int x, int y, int w, int h, const char *texturepath, SDL_bool floateffect, button_t **button)
{
    assert(button, "Invalid Arguments");

    (*button) = calloc(1, sizeof(button_t));
    (*button)->texture = loadtexture(texturepath);

    (*button)->rect = (rect_t ){x, y, w, h};


    center(&(*button)->rect, &(*button)->rect);

    (*button)->floateffect = floateffect;

}

void delete_button(button_t *button)
{
    assert(button, "Invalid Arguments");

    if (button->texture)
    SDL_DestroyTexture(button->texture);

    button->texture  = 0;


    free(button);
}


extern SDL_Event event;



SDL_bool update_button(button_t *button)
{
    assert(button, "Invalid Arguments");

    rect_t  mouse = {0,0, 0.1,0.1};
    getmousepos(&mouse.x, &mouse.y);


    SDL_bool clicked = (SDL_BUTTON(SDL_GetMouseState(NULL, NULL)) == SDL_BUTTON_LEFT)? SDL_TRUE : SDL_FALSE;
    rect_t  r = button->rect;


    if (button->rise)
    r.y += button->rise;
    else
    r.y += button->floatlift;
    button->selected = hasintersection(&mouse, &r);



        if (!button->disabled)
        {








            if (button->selected)
            {



                SDL_SetTextureAlphaMod(button->texture,125);
                button->rise = -((double)button->scale.y / 2.5);


                Uint8 r, b, g;
                SDL_GetTextureColorMod(button->texture, &r,&b,&g);

                if (!(r || b || g))
                {

                    if (!clicked)
                    {
                        SDL_SetTextureColorMod(button->texture, 255, 255, 255);
                        Mix_PlayChannel(-1, button_click, 0);
                        return SDL_TRUE;
                    }

                }


                if (clicked)
                    SDL_SetTextureColorMod(button->texture, 0,0,0);
                else
                    SDL_SetTextureColorMod(button->texture, 255, 255, 255);

                button->floatlift = 0;

            }
            else
            {



                Uint8 r, g, b;
                SDL_GetTextureColorMod(button->texture, &r,&g, &b);

                if ((r || g || b))
                    SDL_SetTextureAlphaMod(button->texture,255);

                if (!clicked)
                SDL_SetTextureColorMod(button->texture, 255, 255, 255);


                if (button->rise < 0)
                    button->rise += ((double)button->scale.y / 125) * deltatime;

                else if (button->rise > 0)
                    button->rise = 0;

                if (button->floateffect)
                    button->floatlift = cos(SDL_GetTicks() * 6.28) * 8;


        }


    }
    else
    {
        if (button->rise < 0)
            button->rise += ((double)button->scale.y / 125.5);
        else if (button->rise > 0)
            button->rise = 0;


        if (button->floateffect)
        button->floatlift =  cos(SDL_GetTicks() * 6.28) * 8;

        Uint8 r, g, b;
            SDL_GetTextureColorMod(button->texture, &r,&g, &b);


        if (button->selected)
            if (!(r || b || g))
            {

                if (!clicked)
                {
                    SDL_SetTextureColorMod(button->texture, 255, 255, 255);
                    Mix_PlayChannel(-1, button_disabled, 0);

                }

            }

        if (clicked && button->selected)
        SDL_SetTextureColorMod(button->texture, 0,0,0);
        else
        SDL_SetTextureColorMod(button->texture, 57, 57, 57);

        SDL_SetTextureAlphaMod(button->texture, 173);


    }


        return SDL_FALSE;
}




void draw_button(button_t *button)
{
    assert(button, "Invalid Arguments");

    rect_t  r = button->rect;

    if (button->rise)
    r.y += button->rise;
    else
    r.y += button->floatlift;


    drawtexture(button->texture, &r, 0);

}


