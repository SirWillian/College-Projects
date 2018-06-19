#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "classes.hpp"
#include "enums.hpp"

#define HEIGHT 600
#define WIDTH 800
#define FPS 60

/**======================================**/
/**                 Boss                 **/
/**======================================**/

Boss::Boss(int a, int l, char* address, float mS, int res, float h) : Hero(a,l,address,mS,res,h)
{
    weapon = new RangedWeapon(100.0/3);
}

Boss::~Boss()
{
    delete weapon;
}

void Boss::desenhaHPBar(ALLEGRO_FONT* fonte)
{
    al_draw_text(fonte,al_map_rgb(255,255,255),400,20,ALLEGRO_ALIGN_CENTER,"Charlie");
    al_draw_filled_rectangle(200,40,600-hp,60,al_map_rgb(255,0,0));
    al_draw_filled_rectangle(600-hp,40,600,60,al_map_rgb(0,0,0));
    al_draw_rectangle(200,20,600,60,al_map_rgb(255,255,255),5);
}

void Boss::anda(Hero* player)
{
    if(x<player->x)
        x+=moveSpeed;
    else
        x-=moveSpeed;
}

void Boss::atira()
{
    int r=rand()%3;
    if(r==1)
    {
        if(weapon->n_projectiles<20)
        {
            int shot_range = rand()%70;

            for(int i=0; i<20; i++)
                if(!weapon->projectiles[i]->isAlive)
                {
                    if(rand()%2==1)
                        weapon->projectiles[i]=Projectile::criaProjetil((x+largura/2)+shot_range,y,7,"Enemies/Charlie/CharlieProjectile.png", M_PI_2,weapon);
                    else
                        weapon->projectiles[i]=Projectile::criaProjetil((x+largura/2)-shot_range,y,7,"Enemies/Charlie/CharlieProjectile.png", M_PI_2,weapon);
                    break;
                }

        }
    }
}
