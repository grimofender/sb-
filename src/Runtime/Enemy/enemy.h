#ifndef _ENEMY
#define _EMEMY


#include <Graphics/graphics.h>
#include <Runtime/Player/Player.h>

#define ENEMY_MAXFRAMES 3

typedef struct enemyskin_t
{
    SDL_Texture *enemy[ENEMY_MAXFRAMES];


} enemyskin_t;


typedef struct enemy_t
{
    player_t **targets;
    int targetcount;
    int skinid;
    int frame;
    union
	{
		struct {

			vector2_t pos;
			vector2_t scale;
		};

		rect_t  rect;
	};

	SDL_bool destroyed;

	int frametimer;
} enemy_t;



/**
    Macros
**/

#define ENEMYDEFAULT_WIDTH (14 * 15.3)
#define ENEMYDEFAULT_HEIGHT (20 * 15.3)

#define ENEMYDEFAULT_SPEED 0.59


#define ENEMYSKIN_COUNT 1

/**
    Externals
**/

extern enemyskin_t *enemyskins[ENEMYSKIN_COUNT];


/**
    Convinence functions
**/

/** \brief Updates an enemy
 *
 * \param enemy The enemy you want update
 *
 * \sa draw_enemy
 **/


void update_enemy(enemy_t* enemy);

/** \brief Draws an enemy onto the screen
 *
 * \param enemy The enemy you want draw
 *
 * \sa update_enemy
 **/

void draw_enemy(enemy_t* enemy);



/**
    Allocation functions
**/

/** NOTE: I am not going to describe these because there boring. **/

void alloc_enemy(int skinid, player_t **targets, int targetcount, enemy_t **enemy);
void alloc_enemyskin(const char* skinname, enemyskin_t **skin);



void delete_enemy(enemy_t *enemy);
void delete_enemyskin(enemyskin_t *skin);



#endif // _ENEMY
