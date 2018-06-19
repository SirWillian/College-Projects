#include "caverna.h"
#include "game_state.h"
#define FLAG 2
#define LAVA -2
#define DESC -1
#define LIVRE 0

typedef struct
{
    Coordenada* pilha;

    char** memo;
    char** mapa;

    int vou_chutar;
    int n_movimentos;
    int ultimo_mov;
    int foge_loop;
    int largura;
    int altura;
    int indo;

}Decisor;

Decisor* criaDecisor (int altura, int largura);

void calculaDistancia(Decisor* d, Coordenada distancia, Coordenada* menor_distancia, Coordenada pos_atual, int i, int j, Coordenada* pos_desconhecida);
void distanciaAoDesconhecido (Decisor* d, Coordenada pos_atual, Coordenada* pos_desconhecida, Coordenada* menor_distancia);
void preencheMapa(int lavas, Decisor* d, Coordenada lugar);
void preencheMemo(int lavas, Decisor* d, Coordenada lugar);
void pegaAtalho(Decisor* d, Coordenada pos_atual);
void checaFlagLocal (Decisor* d, int x, int y);
void atualizaMapaMemo(Decisor* d);
void destroiDecisor (Decisor* d);

int proximoMovimento (Decisor* d, Coordenada pos, int agua, int n_lava);
int decideMovimento(Decisor* d, Coordenada pos_atual);
int estamosEmLoop(Decisor* d, Coordenada pos_atual);
int decideVolta(Decisor* d, Coordenada pos_atual);
int estaLivre(Decisor* d);
