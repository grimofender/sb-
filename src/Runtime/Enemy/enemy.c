#include <Runtime/Enemy/enemy.h>

extern int frames;

extern long double deltatime;


enemyskin_t *enemyskins[ENEMYSKIN_COUNT];


#include <limits.h>
void update_enemy(enemy_t* enemy)
{
     assert(enemy, "Invalid Arguments");

     SDL_bool targetsalldead = SDL_TRUE;

     for (int i = 0; i < enemy->targetcount; ++i)
     {

           if (!enemy->targets[i]->death.dead && !enemy->targets[i]->death.dying)
           targetsalldead = SDL_FALSE;
     }

      if (targetsalldead)
                return;


     int smallest = 0, smallestdistance = INT_MAX;

     for (int i = 0; i < enemy->targetcount; ++i)
     {

           if (enemy->targets[i]->snowball.shot)
            if (hasintersection(&enemy->rect, &enemy->targets[i]->snowball.rect))
            {
                enemy->destroyed = SDL_TRUE;

            }

            if (hasintersection(&enemy->rect, &enemy->targets[i]->rect))
            {
                if (!enemy->targets[i]->death.dying)
                {
                    vector2_t vel = { 0 };

                    vel.y = -4;

                    vel.x = -((enemy->pos.x - enemy->targets[i]->pos.x)) / 400;

                    kill_player(enemy->targets[i], vel);

                }

            }


            vector2_t enemypos = enemy->pos;
            vector2_t playerpos = enemy->targets[i]->pos;

            vector2_t distance = {enemypos.x - playerpos.x, enemypos.y - playerpos.y};

            int idistance = SDL_sqrt(SDL_pow(distance.x, 2) + SDL_pow(distance.y, 2));

            if (idistance < smallestdistance)
            {
                smallest = i;
                smallestdistance = idistance;
            }


     }

     vector2_t v = {enemy->targets[smallest]->pos.x - enemy->pos.x, enemy->targets[smallest]->pos.y - enemy->pos.y};

     float dist = sqrt(v.x*v.x + v.y*v.y);

     enemy->pos.x += (ENEMYDEFAULT_SPEED * (v.x / dist)) * deltatime;
     enemy->pos.y += (ENEMYDEFAULT_SPEED * (v.y / dist)) * deltatime;



    enemy->frametimer -= deltatime;

    if (enemy->frametimer <= 0)
    {


         if (enemy->frame == 0 || enemy->frame == 2)
            enemy->frame = 1;
            else
            enemy->frame = 2;


            enemy->frametimer = 90;
    }
}



void alloc_enemy(int skinid, player_t **targets, int targetcount, enemy_t **enemy)
{
    assert(enemy, "Invalid Arguments");


    (*enemy) = calloc(1, sizeof(enemy_t));

	(*enemy)->skinid = skinid;

    (*enemy)->rect.w = ENEMYDEFAULT_WIDTH;
    (*enemy)->rect.h = ENEMYDEFAULT_HEIGHT;
	(*enemy)->targets = targets;
	(*enemy)->targetcount = targetcount;


	(*enemy)->pos.x = (reselution_width / 2) +  (cos(SDL_GetTicks()) * (((reselution_width  / 2) + ENEMYDEFAULT_WIDTH) + 180));
	(*enemy)->pos.y = (reselution_height / 2) + (sin(SDL_GetTicks()) * (((reselution_height / 2) + ENEMYDEFAULT_HEIGHT) + 180));

	center(&(*enemy)->rect, &(*enemy)->pos);

}

void delete_enemy(enemy_t *enemy)
{
    free(enemy);
}



void alloc_enemyskin(const char* skinname, enemyskin_t **skin)
{
    assert(skin, "Invalid Arguments");

    (*skin) = calloc(1, sizeof(enemyskin_t));

    char *temp = calloc(256, sizeof(char));
    getcwd(temp, 255);




    char* dat = calloc(256, sizeof(char));

    sprintf(dat, "data/enemyskins/%s", skinname);
    chdir(dat);
    free (dat);




    (*skin)->enemy[0] = loadtexture("0.bmp");
    (*skin)->enemy[1] = loadtexture("1.bmp");
    (*skin)->enemy[2] = loadtexture("2.bmp");






    chdir(temp);
    free(temp);
}
void delete_enemyskin(enemyskin_t *skin)
{
    for (int i = 0; i < ENEMY_MAXFRAMES; i++)
    {
        SDL_DestroyTexture(skin->enemy[i]);
    }
    free(skin);
}


void draw_enemy(enemy_t* enemy)
{
    assert(enemy, "Invalid Arguments");

     assert(((enemy->frame < ENEMY_MAXFRAMES) || (enemy->frame > 0)),  "Invalid Frame");

     enemyskin_t *skin = enemyskins[enemy->skinid];


      drawtexture(skin->enemy[enemy->frame], &enemy->rect, 0);

}
