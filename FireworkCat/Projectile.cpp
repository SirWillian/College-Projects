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
/**              Projectile              **/
/**======================================**/

Projectile::Projectile()
{
    isAlive=false;
    bmp=NULL;
}

Projectile::Projectile(float posx, float posy, float s, char* address, float a)
{
    x=posx;
    y=posy;
    speed=s;
    angle=a;
    isAlive=true;
    bmp=al_load_bitmap(address);

    altura=al_get_bitmap_height(bmp);
    largura=al_get_bitmap_width(bmp);
}

Projectile::~Projectile()
{
    if(bmp!=NULL)
        al_destroy_bitmap(bmp);
}

void Projectile::atualizaProjetil(Projectile* target, RangedWeapon* source)
{
    target->x+=target->speed*cos(target->angle);
    target->y+=target->speed*sin(target->angle);

    /* Se sair da tela, o projétil deve ser destruido */
    if(target->x>WIDTH+target->largura||target->x<-target->largura||
       target->y>HEIGHT+target->altura||target->y<-target->altura)
        destroiProjetil(target, source);
}

void Projectile::desenhaProjetil()
{
    al_draw_rotated_bitmap(bmp,largura/2,altura/2,x,y,angle,0);
}

Projectile* Projectile::criaProjetil(float posx, float posy, float s, char* address, float a, RangedWeapon* source)
{
    Projectile* newProjectile = new Projectile(posx,posy,s,address,a);
    source->n_projectiles++;
    return newProjectile;
}

void Projectile::destroiProjetil(Projectile* target, RangedWeapon* source)
{
    target->isAlive=false;
    source->n_projectiles--;
}
