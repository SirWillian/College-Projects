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
/**             RangedWeapon             **/
/**======================================**/

RangedWeapon::RangedWeapon(float d) : Weapon(d)
{
    projectiles = new Projectile*[20];
    for(int i=0; i<20; i++)
        projectiles[i]= new Projectile();
    n_projectiles=0;
}

RangedWeapon::~RangedWeapon()
{
    for(int i=0; i<20; i++)
        delete projectiles[i];
    delete[] projectiles;
    /* Podia-se usar uma sobrecarga de operadores para apagar os projeteis dentro do vetor, mas eu fui incapaz de sobrecarregar
     * o operador delete[]. */
}

void RangedWeapon::dealDmg(int sourceDmg, Hero* target)
{
    target->hp-=sourceDmg/(target->resistencia+1);
}
