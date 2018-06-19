#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <math.h>

#include "classes.hpp"
#include "enums.hpp"
/**===========================================================**/
using namespace std;
/**===========================================================**/

GameState g; //separa em (menu phase) e (game phase)

int main()
{
    bool draw = false;
    if(g.iniciaAllegro()==-1)
        return(-1);

    while(g.gameIsRunning)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(g.event_queue, &event);
        srand(time(NULL));

        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            draw = true;
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            g.gameIsRunning=false;
        }
        else if((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_KEY_UP))
        {
            g.verificaTeclado(&event);
            draw = true;
        }

        if(draw && al_is_event_queue_empty(g.event_queue))
        {
            if(g.jogo!=NULL)
            {
                g.jogo->atualizaJogo(); //Quando o jogo estiver instanciado, os elementos dele devem ser atualizados todo frame
                if(!g.jogo->isActive)
                {
                    g.terminaJogo();
                    g.constroiMenu();
                }
            }
            g.desenhaDisplay();
        }
    }

    g.deletaMenu();
    g.destroiAllegro();
}

