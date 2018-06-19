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
/**                Enemy                 **/
/**======================================**/

Enemy::Enemy()
{
    isAlive=false;
}

Enemy::Enemy(int a, int l, char* address, float mS, int res, int wType, float d, int ran, float h) : Hero(a,l,address,mS,res,h)
{
    bmpFrame=0;
    if(wType==enums::MELEE)
    {
        m_weapon = new MeleeWeapon(ran,d);
        r_weapon=NULL;
    }
    else if(wType==enums::RANGED)
    {
        r_weapon = new RangedWeapon(d);
        m_weapon=NULL;
    }
}

Enemy::~Enemy()
{
    if(r_weapon!=NULL)
        delete r_weapon;
    if(m_weapon!=NULL)
        delete m_weapon;
}

int Enemy::n_enemies=0;
int Enemy::n_tipos=1;
int Enemy::n_killed=0;

Enemy* Enemy::criaInimigo()
{
    Enemy* newEnemy;
    int type;
    type=rand()%Enemy::n_tipos;

    if(type==enums::DITTO)
        newEnemy = new Enemy(90,90,"Enemies/Ditto/",2,2,enums::MELEE,25,50,100);

    return newEnemy;
}

void Enemy::destroiInimigo()
{
    isAlive=false;
    n_enemies--;
    n_killed++;
}

void Enemy::procuraAlvo(Hero* target)
{
    angle=atan((y-target->y)/(x-target->x));
    if(x>target->x)
        angle+=M_PI;
}

void Enemy::anda()
{
    y+=moveSpeed*sin(angle);
    x+=moveSpeed*cos(angle);
}

void Enemy::desenhaPersonagem()
{
    bmpFrame++;
    if(bmpFrame==20)
        bmpFrame=0;
    al_draw_scaled_bitmap(bmps[bmpFrame/5], 0, 0, al_get_bitmap_width(bmps[bmpFrame/5]), al_get_bitmap_height(bmps[bmpFrame/5]), x, y, altura, largura, 0);
}
