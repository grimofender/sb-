#include "graphics.h"

int reselution_width  = DEFAULTRESELUTION_WIDTH;
int reselution_height = DEFAULTRESELUTION_HEIGHT;

SDL_Texture *rendertarget = NULL;

inline void clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}


void draw_text(float x, float y, char* text, TTF_Font *font, SDL_Color color, SDL_bool centered)
{

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);

    rect_t r = {x, y, surface->w, surface->h};

    if (centered) {
        vector2_t cent;
        center(&r, &cent);
        r.x = cent.x; r.y = cent.y;
    }
        


    drawtexture(tex, &r, 0);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(tex);



}




rect_t intersection(rect_t *r1, rect_t *r2)
{
    rect_t r;

    r.x = max(r1->x, r2->x);
    r.y = max(r1->y, r2->y);
    r.w = min((r1->x + r1->w), (r2->x + r2->w)) - r.x;
    r.h = min((r1->y + r1->h), (r2->y + r2->h)) - r.y;





    return r;
}

SDL_bool hasintersection(rect_t *r1, rect_t *r2)
{
    rect_t r = intersection(r1, r2);
    return (r.w >= 0) && (r.h >= 0);
}




inline SDL_Texture *loadtexture(const char* file)
{

   SDL_Surface *data =  loadbmp(file);

   SDL_Texture *texure = SDL_CreateTextureFromSurface(renderer, data);

   SDL_FreeSurface(data);

   return texure;

}

 inline SDL_Surface *loadbmp(const char* file)
 {

    SDL_Surface *data =  SDL_LoadBMP(file);
    const char *errformat = "Unable to load Texture from %s";

    if (!data) {
        size_t size = snprintf(NULL, 0, errformat, file);
        char *err = calloc(size, sizeof(char));
        sprintf(err, errformat, file);
        assert(SDL_FALSE, err);
        free(err); // just in case
    }

    

    SDL_SetColorKey(data, 1, SDL_MapRGB(data->format, transperent.r, transperent.g, transperent.b));

     return data;
 }



void getrightcorner(vector2_t* pt, rect_t *rt)
{



    pt->x = (rt->x - (rt->w / 2));
	pt->y = (rt->y - (rt->h / 2));


}

void getmousepos(float *x, float *y)
{
    int rx, ry;

    SDL_GetMouseState(&rx, &ry);








    int ww, wh;

    SDL_GetWindowSize(window, &ww, &wh);




    int lw, lh;
    SDL_RenderGetLogicalSize(renderer, &lw, &lh);

      if (!lw && !lh) {
            lw = ww;
            lh = wh;
    };



   float sw = ((float)lw / (float)ww), sh = ((float)lh / (float)wh);


    rx *= sw;
    ry *= sh;

   (*x) = (float)rx;
   (*y) = (float)ry;


}


void center(rect_t *rt, vector2_t* pos)
{
	assert(rt, "Invalid Argument");
    assert(pos, "Invalid Argument");

	rt->x = (pos->x - (rt->w / 2));
	rt->y = (pos->y - (rt->h / 2));
}
void drawtexture(SDL_Texture *tex, rect_t  *r, const double angle)
{

    if (r)
    {
        SDL_Rect rect = {floor(r->x), floor(r->y), floor(r->w), floor(r->h)};
        SDL_RenderCopyEx(renderer, tex, 0, &rect, angle, 0, 0);
    }
    else
    {
        SDL_RenderCopyEx(renderer, tex, 0, 0, angle, 0, 0);
    }


}
