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

/**======================================**/
/**                 Botao                **/
/**======================================**/

Botao::Botao()
{
    isHighlighted=false;
    botao=NULL;
    largura=0;
    altura=0;
    x=0;
    y=0;
}

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
            /* Seta todas as direções pra false para, então, setar alguma para true, pois só pode haver uma direção
             * em que o personagem está apontando */
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
            /* Para o personagem não sumir quando se solta todas as teclas, checa-se qual é a direção do personagem.
             * Se nenhuma tecla estiver pressionada, seta-se a direção do personagem para a direção checada.
             * Também checa-se se há apenas uma tecla pressionada e seta-se a direção pra ser a direção da tecla caso seja a
             * a única pressionada. Isso foi feito para garantir que o programa não vai perder a direção do personagem
             * quando múltiplas teclas forem presisonadas. Talvez essa não seja a forma mais apropriada de solucionar
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
        /* Não anda */
        teclas_opostas=true;
    else if (dir_teclas[enums::LEFT]&&dir_teclas[enums::RIGHT])
        /* Também não anda */
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

    /* Se houver apenas uma tecla pressionada, anda na direção da tecla pressionada (M_PI_2 vale PI/2, que equivale a 90 graus */
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
    /* No estado atual do código, esse primeiro if não é necessário, uma vez que saber quantos projéteis existem não é muito útil.
     * Entretanto, se os projéteis estivessem todos sendo alocados e desalocados, esse if seria necessário.
     * Os projéteis não estão sendo criados dessa forma por questão de tempo */
    if(weapon->n_projectiles<20)
        for(int i=0; i<20; i++)
            if(!weapon->projectiles[i]->isAlive)
            {
                weapon->projectiles[i]=Projectile::criaProjetil(x+largura/2,y+altura/2,9,"HeroSprites/HeroProjectile.png",angle,weapon);
                break; //Para instanciar só 1 tiro
            }
}
/**======================================**/
/**              DmgDealer               **/
/**======================================**/

DmgDealers::~DmgDealers()
{
    /* Não faz nada */
}

/**======================================**/
/**                Weapon                **/
/**======================================**/
Weapon::Weapon(float d)
{
    dmg=d;
}

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

/**======================================**/
/**              Projectile              **/
/**======================================**/

Projectile::Projectile()
{
    isAlive=false;
    bmp=NULL;
}

Projectile::Projectile(float posx, float posy, float s, char* address, float a)
{
    x=posx;
    y=posy;
    speed=s;
    angle=a;
    isAlive=true;
    bmp=al_load_bitmap(address);

    altura=al_get_bitmap_height(bmp);
    largura=al_get_bitmap_width(bmp);
}

Projectile::~Projectile()
{
    if(bmp!=NULL)
        al_destroy_bitmap(bmp);
}

void Projectile::atualizaProjetil(Projectile* target, RangedWeapon* source)
{
    target->x+=target->speed*cos(target->angle);
    target->y+=target->speed*sin(target->angle);

    /* Se sair da tela, o projétil deve ser destruido */
    if(target->x>WIDTH+target->largura||target->x<-target->largura||
       target->y>HEIGHT+target->altura||target->y<-target->altura)
        destroiProjetil(target, source);
}

void Projectile::desenhaProjetil()
{
    al_draw_rotated_bitmap(bmp,largura/2,altura/2,x,y,angle,0);
}

Projectile* Projectile::criaProjetil(float posx, float posy, float s, char* address, float a, RangedWeapon* source)
{
    Projectile* newProjectile = new Projectile(posx,posy,s,address,a);
    source->n_projectiles++;
    return newProjectile;
}

void Projectile::destroiProjetil(Projectile* target, RangedWeapon* source)
{
    target->isAlive=false;
    source->n_projectiles--;
}

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

/**======================================**/
/**                 Boss                 **/
/**======================================**/

Boss::Boss(int a, int l, char* address, float mS, int res, float h) : Hero(a,l,address,mS,res,h)
{
    weapon = new RangedWeapon(100.0/3);
}

Boss::~Boss()
{
    delete weapon;
}

void Boss::desenhaHPBar(ALLEGRO_FONT* fonte)
{
    al_draw_text(fonte,al_map_rgb(255,255,255),400,20,ALLEGRO_ALIGN_CENTER,"Charlie");
    al_draw_filled_rectangle(200,40,600-hp,60,al_map_rgb(255,0,0));
    al_draw_filled_rectangle(600-hp,40,600,60,al_map_rgb(0,0,0));
    al_draw_rectangle(200,20,600,60,al_map_rgb(255,255,255),5);
}

void Boss::anda(Hero* player)
{
    if(x<player->x)
        x+=moveSpeed;
    else
        x-=moveSpeed;
}

void Boss::atira()
{
    int r=rand()%3;
    if(r==1)
    {
        if(weapon->n_projectiles<20)
        {
            int shot_range = rand()%70;

            for(int i=0; i<20; i++)
                if(!weapon->projectiles[i]->isAlive)
                {
                    if(rand()%2==1)
                        weapon->projectiles[i]=Projectile::criaProjetil((x+largura/2)+shot_range,y,7,"Enemies/Charlie/CharlieProjectile.png", M_PI_2,weapon);
                    else
                        weapon->projectiles[i]=Projectile::criaProjetil((x+largura/2)-shot_range,y,7,"Enemies/Charlie/CharlieProjectile.png", M_PI_2,weapon);
                    break;
                }

        }
    }
}
