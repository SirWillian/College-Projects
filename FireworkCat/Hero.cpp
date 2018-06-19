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
/**                 Hero                 **/
/**======================================**/

Hero::Hero()
{

}

Hero::Hero(int a, int l, char* address, float mS, int res, float h) : Personagem(a,l,address)
{
    hp=h;
    isAlive=true;
    moveSpeed=mS;
    resistencia=res;
    angle=0;
    weapon = NULL;

    for(int i=0; i<4; i++)
    {
        directions[i]=false;
        dir_teclas[i]=false;
    }
    directions[enums::RIGHT]=true;
    shooting_sound = al_load_sample("sounds/Shoot_Firework.wav");
}

Hero::~Hero()
{
    if(weapon!=NULL)
        delete weapon;
}

void Hero::criaArma(float dmg)
{
    weapon = new RangedWeapon(dmg);
}

void Hero::verificaComandos(ALLEGRO_EVENT* event, ALLEGRO_DISPLAY* display)
{
        if(event->type == ALLEGRO_EVENT_KEY_DOWN)
        {
            /* Seta todas as dire��es pra false para, ent�o, setar alguma para true, pois s� pode haver uma dire��o
             * em que o personagem est� apontando */
            int lastDir=checaDirecao();
            for(int i=0; i<4; i++)
                directions[i]=false;

            switch(event->keyboard.keycode)
            {
            case ALLEGRO_KEY_DOWN:
                directions[enums::DOWN] = true;
                dir_teclas[enums::DOWN]=true;
                break;
            case ALLEGRO_KEY_UP:
                directions[enums::UP] = true;
                dir_teclas[enums::UP]=true;
                break;
            case ALLEGRO_KEY_RIGHT:
                directions[enums::RIGHT] = true;
                dir_teclas[enums::RIGHT]=true;
                anda();
                break;
            case ALLEGRO_KEY_LEFT:
                directions[enums::LEFT]=true;
                dir_teclas[enums::LEFT]=true;
                break;
            case ALLEGRO_KEY_SPACE:
                directions[lastDir]=true;
                atira();
                al_set_target_bitmap(al_get_backbuffer(display));
                break;
            default:
                directions[lastDir]=true;
                break;
            }
        }
        else if(event->type==ALLEGRO_EVENT_KEY_UP)
        {
            /* Para o personagem n�o sumir quando se solta todas as teclas, checa-se qual � a dire��o do personagem.
             * Se nenhuma tecla estiver pressionada, seta-se a dire��o do personagem para a dire��o checada.
             * Tamb�m checa-se se h� apenas uma tecla pressionada e seta-se a dire��o pra ser a dire��o da tecla caso seja a
             * a �nica pressionada. Isso foi feito para garantir que o programa n�o vai perder a dire��o do personagem
             * quando m�ltiplas teclas forem presisonadas. Talvez essa n�o seja a forma mais apropriada de solucionar
             * esse problema, mas mantem o switch simples. */

            int lastDir = checaDirecao();

            switch(event->keyboard.keycode)
            {
            case ALLEGRO_KEY_DOWN:
                directions[enums::DOWN]=false;
                dir_teclas[enums::DOWN]=false;
                break;
            case ALLEGRO_KEY_UP:
                directions[enums::UP]=false;
                dir_teclas[enums::UP]=false;
                break;
            case ALLEGRO_KEY_RIGHT:
                directions[enums::RIGHT]=false;
                dir_teclas[enums::RIGHT]=false;
                break;
            case ALLEGRO_KEY_LEFT:
                directions[enums::LEFT]=false;
                dir_teclas[enums::LEFT]=false;
                break;
            default:
                break;
            }

            int onlyDir;
            int n_pressed=0;
            for(int i=0; i<4; i++)
                if(dir_teclas[i])
                {
                    n_pressed++;
                    onlyDir=i;
                }
            if(n_pressed==1)
                directions[onlyDir]=true;
            else if(n_pressed==0)
                directions[lastDir]=true;

        }
}

void Hero::anda()
{
    bool teclas_opostas=false;
    if(dir_teclas[enums::DOWN]&&dir_teclas[enums::UP])
        /* N�o anda */
        teclas_opostas=true;
    else if (dir_teclas[enums::LEFT]&&dir_teclas[enums::RIGHT])
        /* Tamb�m n�o anda */
        teclas_opostas=true;

    /* Se tiver duas teclas pressionadas, ele anda na diagonal (M_PI_4 vale PI/4, que equivale a 45 graus) */
    else if(dir_teclas[enums::DOWN]&&dir_teclas[enums::RIGHT])
        angle=M_PI_4;
    else if(dir_teclas[enums::DOWN]&&dir_teclas[enums::LEFT])
        angle=3*M_PI_4;
    else if(dir_teclas[enums::UP]&&dir_teclas[enums::RIGHT])
        angle=7*M_PI_4;
    else if(dir_teclas[enums::UP]&&dir_teclas[enums::LEFT])
        angle=5*M_PI_4;

    /* Se houver apenas uma tecla pressionada, anda na dire��o da tecla pressionada (M_PI_2 vale PI/2, que equivale a 90 graus */
    else if(dir_teclas[enums::DOWN])
        angle=M_PI_2;
    else if(dir_teclas[enums::UP])
        angle=3*M_PI_2;
    else if(dir_teclas[enums::RIGHT])
        angle=0;
    /* M_PI vale PI, que vale 180 graus */
    else if(dir_teclas[enums::LEFT])
        angle=M_PI;

    int n_pressed=0;
    for(int i=0; i<4; i++)
        if (dir_teclas[i])
            n_pressed++;
    if(n_pressed!=0&&!teclas_opostas)
    {
        y+=moveSpeed*sin(angle);
        x+=moveSpeed*cos(angle);
    }
}

void Hero::atira()
{
    /* No estado atual do c�digo, esse primeiro if n�o � necess�rio, uma vez que saber quantos proj�teis existem n�o � muito �til.
     * Entretanto, se os proj�teis estivessem todos sendo alocados e desalocados, esse if seria necess�rio.
     * Os proj�teis n�o est�o sendo criados dessa forma por quest�o de tempo */
    if(weapon->n_projectiles<20)
        for(int i=0; i<20; i++)
            if(!weapon->projectiles[i]->isAlive)
            {
                weapon->projectiles[i]=Projectile::criaProjetil(x+largura/2,y+altura/2,9,"HeroSprites/HeroProjectile.png",angle,weapon);
                al_play_sample(shooting_sound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,&shooting_sound_id);
                break; //Para instanciar s� 1 tiro
            }
}
