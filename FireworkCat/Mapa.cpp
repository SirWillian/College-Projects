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
/**                 Mapa                 **/
/**======================================**/

Mapa::Mapa(char* address)
{
    mapBmp = NULL;
    setMapBmp(address);
}
Mapa::~Mapa()
{
    al_destroy_bitmap(mapBmp);
}
void Mapa::setMapBmp(char* address)
{
    mapBmp = al_load_bitmap(address);
}
void Mapa::desenhaMapa()
{
    /* Desenha o mapa do tamanho da tela */
    al_draw_scaled_bitmap(mapBmp, 0, 0, WIDTH,HEIGHT, 0, 0, WIDTH, HEIGHT, 0);
    /* Não sei porque exatamente, mas passando WIDTH e HEIGHT duas vezes assim cria o efeito esperado */
}
