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
/**              MeleeWeapon             **/
/**======================================**/

MeleeWeapon::MeleeWeapon(int r, float d) : Weapon(d)
{
    ataques_recentes=0;
    range=r;
}

void MeleeWeapon::dealDmg(int sourceDmg, Hero* target)
{
    target->hp-=sourceDmg;
}
