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
/**              GameState               **/
/**======================================**/

GameState::GameState()
{
    gameIsRunning=true;
    menu=NULL;
    jogo=NULL;
    display=NULL;
    event_queue=NULL;
    timer=NULL;
    fonte_menu=NULL;
    fonte_ingame=NULL;
}

GameState::~GameState()
{
    if(menu!=NULL)
        delete menu;
    if(jogo!=NULL)
        delete jogo;
}

void GameState::destroiAllegro()
{

    al_destroy_font(fonte_ingame);
    al_destroy_font(fonte_menu);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}

void GameState::deletaMenu()
{
    delete menu;
    menu=NULL;
}

int GameState::iniciaAllegro()
{
    if(!al_init())
    {
        std::cout << "failed to initialize allegro!\n";
        return -1;
    }
    if(!al_install_keyboard())
    {
        std::cout << "failed to initialize the keyboard!\n";
        return -1;
    }
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon

    if(!al_init_image_addon())
    {
        std::cout << "failed to initialize the al_init_image_addon!\n";
        return -1;
    }


    if(!al_install_audio())
    {
        std::cout << "failed to initialize audio!\n";
        return -1;
    }

    if(!al_init_acodec_addon())
    {
        std::cout << "failed to initialize audio codecs!\n";
        return -1;
    }

    if(!al_init_primitives_addon())
    {
        std::cout << "failed to initialize the al_init_primitives_addon!\n";
        return -1;
    }
    /* Reserves a number of sample instances, and creates a default mixer if one doesn't exist. This allows us to decide how many audio samples we will be creating for now we are only creating one. */
    iniciaAudio();
    /*if (!al_reserve_samples(1)){
       std::cout << "failed to reserve samples!\n";
       return -1;
    }

    //carregamos o som
    som = al_load_sample("ponto.wav");
    if (!som) {
        std::cout << "Audio clip sample not loaded!\n";
        return -1;
    }*/

    fonte_menu=al_load_ttf_font("BitCheese10.TTF",60,0);

    if (!fonte_menu){
        std::cout << "Could not load 'BitCheese10.ttf'.\n";
        return -1;
    }

    fonte_ingame=al_load_ttf_font("BitCheese10.TTF",24,0);

    /*Criamos o timer*/
    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        std::cout << "failed to create timer!\n";
        return -1;
    }

    /* Criamos o display */
    display = al_create_display(WIDTH, HEIGHT);
    if(!display)
    {
        std::cout << "failed to create display!\n";
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        std::cout << "failed to create event queue!\n";
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    /* Torna apto o uso de mouse na aplicação */
    if (!al_install_mouse())
    {
        std::cout << "failed to install the mouse.\n";
        return -1;
    }

    /* Atribui o cursor padrão do sistema para ser usado */
    if (!al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        std::cout << "failed to set the mouse cursor.\n";
        return -1;
    }
    al_register_event_source(event_queue, al_get_mouse_event_source());

    /* Configura o título da janela */
    al_set_window_title(display, "Firework Cat");

    /* Inicia o fundo */
    al_set_target_bitmap(al_get_backbuffer(display));
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    al_start_timer(timer);

    constroiMenu();

    return 0;
}

int GameState::iniciaAudio()
{
    if (!al_reserve_samples(1)){
       std::cout << "failed to reserve samples!\n";
       return -1;
    }
    return 0;
}

void GameState::constroiMenu()
{
    menu=new Menu(2);
    al_set_target_bitmap(al_get_backbuffer(display));
    for(int i=0; i<2; i++)
        menu->botoes[i].x=WIDTH/2-menu->botoes[i].largura/2;
    menu->botoes[0].y=350;
    menu->botoes[1].y=470;
    strcpy(menu->botoes[0].texto, "Play");
    strcpy(menu->botoes[1].texto, "Quit");
}

void GameState::desenhaDisplay()
{
    if (menu!=NULL)
        menu->desenhaMenu(fonte_menu);
    else if(jogo!=NULL)
    {
        jogo->desenhaJogo(fonte_ingame);
    }
}

void GameState::usaMenu()
{
    switch(menu->getBotaoHighlighted())
    {
        case enums::PLAY:
            deletaMenu();
            comecaJogo();
            break;
        case enums::QUIT:
            gameIsRunning=false;
            break;
        default:
            break;
    }
}

void GameState::verificaTeclado(ALLEGRO_EVENT* event)
{
    /* Verifica o que está ativo e processa os comandos do teclado de acordo */

    if (menu!=NULL)
    {
        if(event->type==ALLEGRO_EVENT_KEY_DOWN)
        {
            int highlighted=menu->getBotaoHighlighted();
            switch(event->keyboard.keycode)
            {
            case ALLEGRO_KEY_ENTER:
                usaMenu();
                break;
            case ALLEGRO_KEY_DOWN:
                if(highlighted!=menu->n_botoes-1)
                {
                    menu->botoes[highlighted].isHighlighted=false;
                    menu->botoes[highlighted+1].isHighlighted=true;
                }
                break;
            case ALLEGRO_KEY_UP:
                if(highlighted!=0)
                {
                    menu->botoes[highlighted].isHighlighted=false;
                    menu->botoes[highlighted-1].isHighlighted=true;
                }
                break;

            default:
                break;
            }
        }
    }

    else if(jogo!=NULL)
    {
        jogo->player->verificaComandos(event,display);
    }

}

void GameState::comecaJogo()
{
    jogo = new Jogo();
    jogo->player->x=WIDTH/2;
    jogo->player->y=HEIGHT/2;
    jogo->player->criaArma(50.0);
}

void GameState::terminaJogo()
{
    delete jogo;
    jogo=NULL;
    Enemy::n_killed=0;
}
