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
/**                 Menu                 **/
/**======================================**/

Menu::Menu(int n_b)
{
    n_botoes=n_b;
    botoes = new Botao[n_botoes];
    for (int i=0; i<n_botoes; i++)
    {
        botoes[i].largura=200;
        botoes[i].altura=100;
        criaBmp(i);
    }
    botoes[0].isHighlighted=true;
    logo=al_load_bitmap("gato.png");
}

Menu::~Menu()
{
    for (int i=0; i<n_botoes; i++)
    {
        al_destroy_bitmap(botoes[i].botao);
        al_destroy_bitmap(botoes[i].botao_highlighted);
    }
    delete[] botoes;
    al_destroy_bitmap(logo);
}

void Menu::criaBmp(int i)
{
    botoes[i].botao=al_create_bitmap(botoes[i].largura,botoes[i].altura);
    al_set_target_bitmap(botoes[i].botao);
    al_draw_filled_rectangle(botoes[i].x,botoes[i].y,botoes[i].x+botoes[i].largura,botoes[i].y+botoes[i].altura,al_map_rgb(77,255,208));
    al_draw_rectangle(botoes[i].x,botoes[i].y,botoes[i].x+botoes[i].largura,botoes[i].y+botoes[i].altura,al_map_rgb(255,255,255),5);

    botoes[i].botao_highlighted=al_create_bitmap(botoes[i].largura, botoes[i].altura);
    al_set_target_bitmap(botoes[i].botao_highlighted);
    al_draw_filled_rectangle(botoes[i].x,botoes[i].y,botoes[i].x+botoes[i].largura,botoes[i].y+botoes[i].altura,al_map_rgb(255,0,255));
    al_draw_rectangle(botoes[i].x,botoes[i].y,botoes[i].x+botoes[i].largura,botoes[i].y+botoes[i].altura,al_map_rgb(255,255,255),10);
}

void Menu::desenhaMenu(ALLEGRO_FONT* fonte)
{
    al_clear_to_color(al_map_rgb(0,255,180));
    al_draw_text(fonte, al_map_rgb(255,0,255), WIDTH/2, 20, ALLEGRO_ALIGN_CENTRE, "FIREWORK CAT");
    //al_draw_scaled_bitmap(logo,0,0,al_get_bitmap_width(logo),al_get_bitmap_height(logo),WIDTH/2,HEIGHT/4,200,150, ALLEGRO_ALIGN_CENTER);
    al_draw_bitmap(logo,220,70,ALLEGRO_ALIGN_CENTER);

    for(int i=0; i<n_botoes; i++)
    {
        if(botoes[i].isHighlighted==false)
            al_draw_bitmap(botoes[i].botao, botoes[i].x, botoes[i].y,0);
        else
            al_draw_bitmap(botoes[i].botao_highlighted, botoes[i].x, botoes[i].y,0);
        al_draw_text(fonte,al_map_rgb(255,255,255),botoes[i].x+(botoes[i].largura/2),botoes[i].y+(botoes[i].altura/2)-30,ALLEGRO_ALIGN_CENTRE,botoes[i].texto);
    }
    al_flip_display();
}

int Menu::getBotaoHighlighted()
{
    int i;
    for(i=0; i<n_botoes; i++)
    {
        if(botoes[i].isHighlighted==true)
            return i;
    }
    std::cout << "Nenhum botâo estava highlighted.";
    return i;
}
