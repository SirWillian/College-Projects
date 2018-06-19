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
/**                 Jogo                 **/
/**======================================**/

Jogo::Jogo()
{
    isActive=true;
    mapa = new Mapa("Background.png");
    player = new Hero(80,80,"HeroSprites/",4.0,0,100);
    enemies = new Enemy*[4];
    charlie=NULL;
    for(int i=0; i<4; i++)
        enemies[i] = new Enemy();
    spawnaInimigos();
}

Jogo::~Jogo()
{
    delete mapa;
    delete player;
    if(enemies!=NULL)
    {
        for(int i=0; i<4; i++)
            delete enemies[i];
        delete[] enemies;
        enemies=NULL;
    }
    if(charlie!=NULL)
    {
        delete charlie;
        charlie = NULL;
    }
}

void Jogo::desenhaJogo(ALLEGRO_FONT* fonte)
{
    /* Desenha o background e os elementos vivos do jogo */

    mapa->desenhaMapa();
    player->desenhaPersonagem();
    for(int i=0; i<20; i++)
        if(player->weapon->projectiles[i]->isAlive)
            player->weapon->projectiles[i]->desenhaProjetil();
    if(enemies!=NULL)
        for(int i=0; i<4; i++)
            if(enemies[i]->isAlive)
                enemies[i]->desenhaPersonagem();
    escreveNaTela(fonte);
    if(charlie!=NULL)
    {
        charlie->desenhaPersonagem();
        charlie->desenhaHPBar(fonte);
        for(int i=0; i<20; i++)
            if(charlie->weapon->projectiles[i]->isAlive)
                charlie->weapon->projectiles[i]->desenhaProjetil();
    }
    al_flip_display();
}

void Jogo::escreveNaTela(ALLEGRO_FONT* fonte)
{
    char aux[5]; // Servirá para printar inteiros na tela
    itoa(Enemy::n_killed, aux, 10); //Coloca a pontuação na string
    al_draw_text(fonte, al_map_rgb(255,255,255),20,20,0,"Points: ");
    al_draw_text(fonte,al_map_rgb(255,255,255),20+al_get_text_width(fonte, "Points: "),20,0,aux);

    itoa(player->hp, aux, 10);
    al_draw_text(fonte,al_map_rgb(255,255,255),600,20,0,"Health: ");
    al_draw_text(fonte,al_map_rgb(255,255,255),600+al_get_text_width(fonte,"Health: "),20,0,aux);
}

void Jogo::spawnaInimigos()
{
    if(enemies!=NULL)
        for(int i=0; i<4; i++)
        {
            if(!enemies[i]->isAlive)
            {
                enemies[i]=Enemy::criaInimigo();
                int pos=rand()%4; //Define em qual lado da tela vai nascer
                if(pos==0)
                {
                    enemies[i]->x=-enemies[i]->largura;
                    enemies[i]->y=rand()%(HEIGHT-enemies[i]->altura);
                }
                if(pos==1)
                {
                    enemies[i]->y=-enemies[i]->altura;
                    enemies[i]->x=rand()%(WIDTH-enemies[i]->largura);
                }
                if(pos==2)
                {
                    enemies[i]->x=WIDTH+enemies[i]->largura;
                    enemies[i]->y=rand()%(HEIGHT-enemies[i]->altura);
                }
                if(pos==3)
                {
                    enemies[i]->y=HEIGHT+enemies[i]->altura;
                    enemies[i]->x=rand()%(WIDTH-enemies[i]->altura);
                }
            }
        }
}

void Jogo::atualizaJogo()
{
    /* Atualiza todos os elementos do jogo (player, inimigos, tiros, boss, etc) */
    for(int i=0; i<20; i++)
        if(player->weapon->projectiles[i]->isAlive)
        {
            player->weapon->projectiles[i]->atualizaProjetil(player->weapon->projectiles[i], player->weapon);
            checaProjeteis(i);
        }
    checaDanoInimigo();
    checaIsAlive();
    spawnaInimigos();
    player->anda();
    if(enemies!=NULL)
    {
        for(int i=0; i<4; i++)
            if(enemies[i]->isAlive)
            {
                enemies[i]->procuraAlvo(player);
                enemies[i]->anda();
            }
    void terminaJogo();
        if(Enemy::n_killed>=20)
            comecaBoss();
    }
    if(charlie!=NULL)
    {
        charlie->anda(player);
        charlie->atira();
        for(int i=0; i<20; i++)
            if(charlie->weapon->projectiles[i]->isAlive)
            {
                charlie->weapon->projectiles[i]->atualizaProjetil(charlie->weapon->projectiles[i], charlie->weapon);
                checaProjeteis(i);
            }
        if(!charlie->isAlive)
            isActive=false;
    }

    if(!player->isAlive)
        isActive=false;
}

void Jogo::comecaBoss()
{
    for(int i=0; i<4; i++)
        delete enemies[i];
    delete enemies;
    enemies=NULL;

    charlie = new Boss(200,170,"Enemies/Charlie/",3,7.0/3,300);
    charlie->x=WIDTH/2;
    charlie->y=80;
    player->x=WIDTH/2;
    player->y=520;
}

void Jogo::checaProjeteis(int i)
{
    /* Compara todos os projeteis do jogador com todos os inimigos vivos para ver se deve acertá-los ou não */

    if(enemies!=NULL)
        for(int j=0; j<4; j++)
        {
              if(enemies[j]->isAlive)
                if((player->weapon->projectiles[i]->x>=enemies[j]->x)&&
                   (player->weapon->projectiles[i]->x<=enemies[j]->x+enemies[j]->largura)&&
                   (player->weapon->projectiles[i]->y>=enemies[j]->y)&&
                   (player->weapon->projectiles[i]->y<=enemies[j]->y+enemies[j]->altura))
                {
                    player->weapon->dealDmg(player->weapon->dmg, enemies[j]);
                    player->weapon->projectiles[i]->destroiProjetil(player->weapon->projectiles[i], player->weapon);
                }
        }
    else if(charlie!=NULL)
    {
        if(player->weapon->projectiles[i]->isAlive)
        {
            if((player->weapon->projectiles[i]->x>=charlie->x)&&
               (player->weapon->projectiles[i]->x<=charlie->x+charlie->largura)&&
               (player->weapon->projectiles[i]->y>=charlie->y)&&
               (player->weapon->projectiles[i]->y<=charlie->y+charlie->altura))
            {
                player->weapon->dealDmg(player->weapon->dmg, charlie);
                player->weapon->projectiles[i]->destroiProjetil(player->weapon->projectiles[i], player->weapon);
            }
        }
        if(charlie->weapon->projectiles[i]->isAlive)
        {
            if((charlie->weapon->projectiles[i]->x>=player->x)&&
               (charlie->weapon->projectiles[i]->x<=player->x+player->largura)&&
               (charlie->weapon->projectiles[i]->y>=player->y)&&
               (charlie->weapon->projectiles[i]->y<=player->y+player->altura))
            {
                charlie->weapon->dealDmg(charlie->weapon->dmg, player);
                charlie->weapon->projectiles[i]->destroiProjetil(charlie->weapon->projectiles[i], charlie->weapon);
            }
        }
    }
}

void Jogo::checaDanoInimigo()
{
    /* Em teoria, devia-se checar tanto para inimigos melee quanto ranged. Entretanto não há inimigos ranged
     * no momento, logo será feita apenas a checagem para inimigos melee */
    if(enemies!=NULL)
        for(int i=0; i<4; i++)
        {
             if(enemies[i]->isAlive)
            {

                /* Se a distância do player ao inimigo for menor que o raio da arma do inimigo */
                if((enemies[i]->x-player->x)*(enemies[i]->x-player->x)+
                   (enemies[i]->y-player->y)*(enemies[i]->y-player->y)<=
                    enemies[i]->m_weapon->range*enemies[i]->m_weapon->range)
                {
                    if(enemies[i]->m_weapon->ataques_recentes==0)
                    {
                        enemies[i]->m_weapon->dealDmg(enemies[i]->m_weapon->dmg,player);
                    }
                    std::cout << enemies[i]->m_weapon->ataques_recentes << std::endl;
                    enemies[i]->m_weapon->ataques_recentes++;
                    if(enemies[i]->m_weapon->ataques_recentes==150)
                        enemies[i]->m_weapon->ataques_recentes=0;
                }
                /* Se a distância do player ao inimigo for maior que duas vezes o raio da arma do inimigo */
                else if((enemies[i]->x-player->x)*(enemies[i]->x-player->x)+
                        (enemies[i]->y-player->y)*(enemies[i]->y-player->y)>=
                         enemies[i]->m_weapon->range*enemies[i]->m_weapon->range*4)
                    enemies[i]->m_weapon->ataques_recentes=0;
            }
        }
}

void Jogo::checaIsAlive()
{
    /* Checa se os personagens estão vivos */
    if(player->hp<=0)
        player->isAlive=false;
    if(enemies!=NULL)
        for(int i=0; i<4; i++)
            if(enemies[i]->hp<=0&&enemies[i]->isAlive)
                enemies[i]->destroiInimigo();
    if(charlie!=NULL&&charlie->hp<=0)
        charlie->isAlive=false;
}
