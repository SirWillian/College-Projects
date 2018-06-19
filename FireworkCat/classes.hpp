#ifndef CLASSES_HPP_INCLUDED
#define CLASSES_HPP_INCLUDED
/**===============================**/
/**           Classes             **/
/**===============================**/
class Menu;
class Botao;
class Mapa;
class Personagem;
/* Poderia existir uma classe PowerUp pra ser associada ao Hero, mas acho que não haverá tempo para implementar esse sistema */
class Jogo;
class GameState;
class Hero;
class Enemy;
class Boss;
class DmgDealers;
class Weapon;
class MeleeWeapon;
class RangedWeapon;
class Projectile;
/**===============================**/
/**     Declaração das Classes    **/
/**===============================**/
//================================== Botao
class Botao
{
    int largura;
    int altura;
    int x;
    int y;
    ALLEGRO_BITMAP* botao;
    ALLEGRO_BITMAP* botao_highlighted;
    bool isHighlighted;
    char texto[50];
public:
    Botao();

    friend Menu;
    friend GameState;
};
//================================== Menu
class Menu
{
    Botao* botoes;
    int n_botoes;
    ALLEGRO_BITMAP* logo;

    void criaBmp(int i);
    void desenhaMenu(ALLEGRO_FONT* fonte);
    int getBotaoHighlighted();
public:
    Menu(int n_b);
    ~Menu();

    friend GameState;
};
//================================== Mapa
class Mapa
{
    ALLEGRO_BITMAP* mapBmp;

    void setMapBmp(char* address);
    void desenhaMapa();
public:
    Mapa(char* address);
    ~Mapa();

    friend Jogo;
};
//================================== Personagem
class Personagem
{
protected:
    ALLEGRO_BITMAP* bmps[4]; //1 posição pra cada direção
    int altura;
    int largura;
    float x;
    float y;
    bool directions[4];

    virtual void desenhaPersonagem();
    int checaDirecao();
public:
    Personagem(); //Só existe para instanciar os inimigos
    Personagem(int a, int l, char* address);
    ~Personagem();

    friend Boss;
    friend Enemy;
    friend Jogo;
};
//================================== Hero
class Hero : public Personagem
{
protected:
    float hp;
    float resistencia;
    bool isAlive;
    float moveSpeed;
    float angle;
    ALLEGRO_SAMPLE *shooting_sound;
    ALLEGRO_SAMPLE_ID shooting_sound_id;

    virtual void anda();
    virtual void atira();
private:
    bool dir_teclas[4];
    RangedWeapon* weapon;
    void criaArma(float dmg);
    void verificaComandos(ALLEGRO_EVENT* event, ALLEGRO_DISPLAY* display);
public:
    Hero(); //Só existe para instanciar os inimigos
    Hero(int a, int l, char* address, float mS, int res, float h);
    virtual ~Hero();

    friend Projectile;
    friend MeleeWeapon;
    friend RangedWeapon;
    friend GameState;
    friend Jogo;
};
//================================== Enemy
class Enemy : public Hero
{
    int tipo;
    int bmpFrame;
    static int n_enemies;
    static int n_tipos;
    static int n_killed; //Para servir como pontuação
    MeleeWeapon* m_weapon;
    RangedWeapon* r_weapon;

    void procuraAlvo(Hero* target);
    void destroiInimigo();
    virtual void desenhaPersonagem();
    virtual void anda();
    static Enemy* criaInimigo();
public:
    Enemy();
    Enemy(int a, int l, char* address, float mS, int res, int wType, float d, int ran, float h);//wType representa a arma a ser usada é melee ou ranged
    virtual ~Enemy();
    friend Projectile;
    friend Jogo;
    friend GameState;
};
//================================== Boss
class Boss : public Hero
{
    RangedWeapon* weapon;

    virtual void atira();
    virtual void anda(Hero* player);
    void desenhaHPBar(ALLEGRO_FONT* fonte);
public:
    Boss(int a, int l, char* address, float mS, int res, float h);
    virtual ~Boss();

    friend Jogo;
};
//================================== Jogo
class Jogo
{
    Mapa* mapa;
    Hero* player; //Mesmo sendo uma composição, são usados ponteiros para passar parametros pro construtor
    Enemy** enemies;
    Boss* charlie;

    void desenhaJogo(ALLEGRO_FONT* fonte);
    void escreveNaTela(ALLEGRO_FONT* fonte);
    void spawnaInimigos();
    void checaProjeteis(int i);
    void checaDanoInimigo();
    void checaIsAlive();
    void comecaBoss();
    void deletaInimigos();
public:
    bool isActive;
    Jogo();
    ~Jogo();
    void atualizaJogo();

    friend GameState;
};
//================================== GameState
class GameState
{
public:
    bool gameIsRunning; //Define se o jogo deve estar aberto ou não
    Menu* menu;
    Jogo* jogo; //A parte que vem depois que você aperta Play no Menu
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT* fonte_menu;
    ALLEGRO_FONT* fonte_ingame;

    GameState();
    ~GameState();
    int iniciaAllegro();
    void destroiAllegro();
    int iniciaAudio();
    void comecaJogo();
    void terminaJogo();
    void constroiMenu();
    void deletaMenu();
    void usaMenu();
    void desenhaDisplay();
    void verificaTeclado(ALLEGRO_EVENT* event);
};
//================================== DmgDealers
/* Interface */
class DmgDealers
{
    virtual void dealDmg(int sourceDmg, Hero* target) = 0;
public:
    virtual ~DmgDealers();
};
//================================== Weapon
/* Classe Abstrata */
class Weapon : public DmgDealers
{
    /* Se o jogo fosse mais complexo, essa classe poderia ter mais detalhes, como munição e mais métodos  */
protected:
    float dmg;
public:
    Weapon(float d); //tipo de arma
};
//================================== MeleeWeapon
class MeleeWeapon : public Weapon
{
    int ataques_recentes; //Para não receber múltiplos ataques por segundo
    int range;

    void dealDmg(int sourceDmg, Hero* target);
public:
    MeleeWeapon(int r, float d);

    friend Jogo;
    friend Enemy;
};
//================================== RangedWeapon
class RangedWeapon : public Weapon
{
    Projectile** projectiles; //máximo de 20 projéteis na tela por arma
    int n_projectiles;

    void dealDmg(int sourceDmg, Hero* target);
public:
    RangedWeapon(float d);
    ~RangedWeapon();

    friend Projectile;
    friend Boss;
    friend Enemy;
    friend Hero;
    friend Jogo;
    friend GameState;
};
//================================== Projectile
class Projectile
{
    ALLEGRO_BITMAP* bmp;
    float x;
    float y;
    int largura;
    int altura;
    float speed;
    float angle;
    bool isAlive;

    void atualizaProjetil(Projectile* target, RangedWeapon* source);
    void desenhaProjetil();
    void destroiProjetil(Projectile* target, RangedWeapon* source);
    static Projectile* criaProjetil(float posx, float posy, float s, char* address, float a, RangedWeapon* source);
public:
    /* Um construtor padrão pra quando a arma for criada e outro pra quando os tiros forem atirados */
    Projectile();
    Projectile(float posx, float posy, float s,char* address, float a);
    ~Projectile();

    friend RangedWeapon;
    friend Enemy;
    friend Boss;
    friend Hero;
    friend Jogo;
    friend GameState;
};

/**===============================**/
#endif // CLASSES_HPP_INCLUDED
