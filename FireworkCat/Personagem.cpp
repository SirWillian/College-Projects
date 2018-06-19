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
/**              Personagem              **/
/**======================================**/

Personagem::Personagem()
{
    /* Construtor padrão */
}

Personagem::Personagem(int a, int l, char* address)
{
    altura=a;
    largura=l;
    for(int i=0; i<4; i++)
    {
        std::stringstream str;
        str << address << i+1 << ".png";
        bmps[i]=al_load_bitmap(str.str().c_str());
    }
}

Personagem::~Personagem()
{
    for(int i=0; i<4; i++)
        al_destroy_bitmap(bmps[i]);
}

void Personagem::desenhaPersonagem()
{
    for(int i=0; i<4; i++)
        if(directions[i])
            al_draw_scaled_bitmap(bmps[i], 0, 0, al_get_bitmap_width(bmps[i]), al_get_bitmap_height(bmps[i]), x, y, altura, largura, 0);

            //al_draw_bitmap(bmps[i],x,y, 0);
}

int Personagem::checaDirecao()
{
    for(int i=0; i<4; i++)
        if(directions[i])
            return i;
}
