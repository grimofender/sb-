#include <Graphics/graphics.h>




extern SDL_Window* window;
extern SDL_Event event;




#include "Player.h"
#include <stdlib.h>
#include <math.h>

#include <Runtime/Music/music.h>


playerskin_t *playerskins[PLAYERSKIN_COUNT];


#define DEADZONE (SDL_JOYSTICK_AXIS_MAX / 10)


#define InDeadZone(x, y) ((x  < DEADZONE) && (y  < DEADZONE) && (x  > -DEADZONE) && (y  >  -DEADZONE))? SDL_TRUE : SDL_FALSE



void get_playerinput(player_t *player, inputtype_t inputtype, SDL_Joystick *joystick, playerinput_t *input)
{
    assert(player, "Invalid Arguments");
    assert(input, "Invalid Arguments");

    input->horisontalaxis = 0;
    input->verticalaxis = 0;
    input->shooting = 0;



    switch (inputtype)
    {

    case inputtype_keyboard_and_mouse:
        {


            Uint8* keystate = SDL_GetKeyboardState(NULL);
            assert(keystate, 0);

            if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
              ++input->horisontalaxis;
            if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
              --input->horisontalaxis;

            if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
              --input->verticalaxis;
            if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
                ++input->verticalaxis;

                if (!input->shooting)
                input->shooting = (SDL_BUTTON(SDL_GetMouseState(NULL, NULL)) == SDL_BUTTON_LEFT);


            vector2_t mp;
            int mbutton;

            getmousepos(&mp.x, &mp.y);
            vector2_t p; middle(&p, &player->rect);





           if (mp.x && mp.y)
            input->shootingdir = (vector2_t){mp.x - p.x, mp.y - p.y};






        }
        break;

        case inputtype_controller:

            assert(joystick, "Invalid Arguments");



            if (SDL_GetMouseFocus() ==  window)
            {

                if (SDL_JoystickGetAxis(joystick, 0) > DEADZONE)
                    ++input->verticalaxis;
                if (SDL_JoystickGetAxis(joystick, 0) < -DEADZONE)
                    --input->verticalaxis;
                if (SDL_JoystickGetAxis(joystick, 1) < -DEADZONE)
                    --input->horisontalaxis;
                if (SDL_JoystickGetAxis(joystick, 1) > DEADZONE)
                    ++input->horisontalaxis;

                    input->shooting = ((SDL_JoystickGetAxis(joystick, 5) > DEADZONE) || SDL_JoystickGetButton(joystick, 0));

            }

            if (!InDeadZone(SDL_JoystickGetAxis(joystick, 3), SDL_JoystickGetAxis(joystick, 4)))
            input->shootingdir = (vector2_t){SDL_JoystickGetAxis(joystick, 3), SDL_JoystickGetAxis(joystick, 4)};








        break;
    }



}



extern int frames;


extern long double deltatime;

inline void update_player(playerinput_t *input, player_t* player)
{
    assert(player, "Invalid Arguments");
    assert(input, "Invalid Arguments");

    if (!player->death.dying && !player->death.dead)
    {

	 if (input->verticalaxis || input->horisontalaxis)
     {
         player->frametimer -= deltatime;

         if (player->frametimer <= 0)
         {


         if (player->frame == 0 || player->frame == 2)
            player->frame = 1;
            else
            player->frame = 2;


            player->frametimer =  100;
        }


     }
    else
    player->frame = 0;


       player->pos.x += (input->horisontalaxis * PLAYERSPEED) * deltatime;
       player->pos.y += (input->verticalaxis  * PLAYERSPEED) * deltatime;


    if (player->pos.x < -(player->scale.x / 2))
        player->pos.x = -(player->scale.x / 2);

    if (player->pos.x > (reselution_width - (player->scale.x / 2)))
        player->pos.x = (reselution_width - (player->scale.x / 2));

    if (player->pos.y < -(player->scale.y / 2))
        player->pos.y = -(player->scale.y / 2);

    if (player->pos.y > (reselution_height - (player->scale.y / 2)))
        player->pos.y = (reselution_height - (player->scale.y / 2));



    }
    else if (player->death.dying)
    {
        if (player->frame != 0)
            player->frame = 0;
            /** play death animation **/

            if (player->death.vel.x > 0)
                player->rotation += player->death.deathrotation * deltatime;
            else
                player->rotation -= player->death.deathrotation * deltatime;



            player->pos.y += player->death.vel.y * deltatime;
            player->pos.x += player->death.vel.x * deltatime;


            if (player->death.deathrotation >= 0)
                if (player->death.deathrotation >= 0.35)
                    player->death.deathrotation -= 0.06 * deltatime;


            player->death.vel.y += 0.012 * deltatime;

            if (!player->snowball.shot)
            {
                player->snowball.rect.w = 0;
                player->snowball.rect.h = 0;
            }




            if (player->pos.y > (reselution_height + player->scale.y))
            {

                memset(&player->death, 0, sizeof(player->death));
                player->death.dead = SDL_TRUE;

            }

    }



            if ((player->snowball.pos.x || player->snowball.pos.y)&& input->shooting && !player->snowball.shot)
            {

                //shoot snowball in direction
                player->snowball.shot = SDL_TRUE;

                player->snowball.velocity.x =  ((float)SNOWBALLSPEED * (float)cos(player->snowball.angle));
                player->snowball.velocity.y =  ((float)SNOWBALLSPEED * (float)sin(player->snowball.angle));



            }
            else if (!player->snowball.shot)
            {
                // aim snowball


                vector2_t p; middle(&p, &player->rect);
                player->snowball.angle = atan2(input->shootingdir.y, input->shootingdir.x);


                player->snowball.pos.x = (p.x - (player->snowball.scale.x / 2)) + (SNOWBALLRADIUS * cos(player->snowball.angle));
                player->snowball.pos.y = (p.y - (player->snowball.scale.y / 2)) + (SNOWBALLRADIUS * sin(player->snowball.angle));

                player->snowball.rotation = player->snowball.angle;
            }
            if (!player->death.dying && !player->death.dead)
            if (!player->snowball.rect.w && !player->snowball.rect.h)
            {
                player->snowball.rect.w = (float)SNOWBALLDEFAULT_WIDTH;
                player->snowball.rect.h = (float)SNOWBALLDEFAULT_HEIGHT;

            }



            rect_t  a = { 0, 0, reselution_width, reselution_height};


            //offscreen
            if (!hasintersection(&player->snowball.rect, &a) && player->snowball.shot)
            {
                // reset snowball
                memset(&player->snowball, 0,sizeof(player->snowball));

            }
            // onscreenextern Mix_Chunk *player_hurt;
            else if (player->snowball.shot)
            {


                player->snowball.pos.x += (player->snowball.velocity.x * deltatime);
                player->snowball.pos.y += (player->snowball.velocity.y * deltatime);

                if (player->snowball.angle > 0)
                player->snowball.rotation -= 0.005 * deltatime;
                else if (player->snowball.angle < 0)
                player->snowball.rotation += 0.005 * deltatime;
            }



       vector2_t p; middle(&p, &player->rect);
       double lookingangle = atan2(input->shootingdir.y, input->shootingdir.x);



		player->eyes.pos.x = ((p.x - (player->eyes.scale.x / 2)) + (47 * SDL_cos((player->rotation - 90) / (180 / M_PI))) + (PLAYER_EYESRADIUS * SDL_cos(lookingangle)));
		player->eyes.pos.y = ((p.y - (player->eyes.scale.y / 2)) + (47 * SDL_sin((player->rotation - 90) / (180 / M_PI))) + (PLAYER_EYESRADIUS * SDL_sin(lookingangle)));





}

void get_playerin(player_t *player, player_in_t *playerin)
{
    assert(player, "Invalid Arguments");
    assert(playerin, "Invalid Arguments");

    playerin->rect.x = player->rect.x;
    playerin->rect.y = player->rect.y;
    playerin->rect.w = player->rect.w;
    playerin->rect.h = player->rect.h;


    playerin->snowballrect.x = player->snowball.rect.x;
    playerin->snowballrect.y = player->snowball.rect.y;
    playerin->snowballrect.w = player->snowball.rect.w;
    playerin->snowballrect.h = player->snowball.rect.h;

    playerin->eyesrect.x = player->eyes.rect.x;
    playerin->eyesrect.y = player->eyes.rect.y;
    playerin->eyesrect.w = player->eyes.rect.w;
    playerin->eyesrect.h = player->eyes.rect.h;




    playerin->skin = player->skinid;
    playerin->frame = player->frame;
    playerin->rotation = player->rotation;


    playerin->id = -1; /** this is decided by the server**/
}





void kill_player(player_t *player, vector2_t vel)
{
    assert(player, "Invalid Arguments");

    if (!player->death.dying || player->death.dead)
    {
       player->death.dying = SDL_TRUE;
       player->death.deathrotation = 1.0968;
       player->death.vel = vel;


       Mix_PlayChannel(-1, player_hurt, 0);
       return;
    }

    err("player is already dead");


}



void alloc_player(int x, int y, int skinid, player_t **player)
{
    assert(player, "Invalid Arguments");

	(*player) = calloc(1, sizeof(player_t));

	(*player)->skinid = skinid;

    (*player)->rect.w = PLAYERDEFAULT_WIDTH;
    (*player)->rect.h = PLAYERDEFAULT_HEIGHT;

	(*player)->pos = (vector2_t){ x, y };

	center(&(*player)->rect, &(*player)->pos);



    (*player)->eyes.rect.w = PLAYER_EYESDEFAULT_WIDTH;
    (*player)->eyes.rect.h = PLAYER_EYESDEFAULT_HEIGHT;


	(*player)->rotation = 0;
}
#include <string.h>



void alloc_playerskin(const char* skinname, playerskin_t **skin)
{
    assert(skin, "Invalid Arguments");
    (*skin) = calloc(1, sizeof(playerskin_t));

    char *temp = calloc(256, sizeof(char));
    getcwd(temp, 255);




    char* dat = calloc(256, sizeof(char));

    sprintf(dat, "data/playerskins/%s", skinname);
    chdir(dat);
    free (dat);




    (*skin)->player[0] = loadtexture("0.bmp");
    (*skin)->player[1] = loadtexture("1.bmp");
    (*skin)->player[2] = loadtexture("2.bmp");
    (*skin)->eyes      = loadtexture("3.bmp");
    (*skin)->snowball  = loadtexture("4.bmp");




    chdir(temp);
    free(temp);
}

void delete_playerskin(playerskin_t *skin)
{
    assert(skin, "Invalid Arguments");
    for (int i = 0; i < PLAYER_MAXFRAMES; i++)
    {
        SDL_DestroyTexture(skin->player[i]);
    }
    SDL_DestroyTexture(skin->snowball);
    SDL_DestroyTexture(skin->eyes);

    free(skin);

}
void delete_player(player_t *player)
{
    assert(player, "Invalid Arguments");
    free(player);
}





void draw_player(player_t *player)
{
    assert(player, "Invalid Arguments");


    if ((player->frame > PLAYER_MAXFRAMES) || (player->frame < 0)) err("Invalid Frame");


	vector2_t p; middle(&p, &player->rect);

	playerskin_t *skin = playerskins[player->skinid];
	assert(skin, "Invalid Skin");

	drawtexture(skin->player[player->frame], &player->rect, player->rotation);
	drawtexture(skin->eyes, &player->eyes.rect, player->rotation);

    double angle = (player->snowball.rotation * (180 / M_PI)) + 270;

	drawtexture(skin->snowball, &player->snowball.rect, angle);




}
