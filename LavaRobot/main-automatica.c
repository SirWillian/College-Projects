/*============================================================================*/
/* IF61C - TRABALHO 3                                                         */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Exploração da caverna marciana - versão automática.  */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "game_state.h"
#include "decisor_aleatorio.h"

/*============================================================================*/

#define PASSO_A_PASSO 0 // Flag que diz se o progresso do robô será mostrado passo-a-passo. Mude para 0 se for rodar muitos testes!

/* Um modificador para a semente usada pelo gerador de números aleatórios na
 * hora de criar um tabuleiro. Todos os trabalhos usarão os mesmos tabuleiros,
 * então o offset precisa ser igual para todos. Entretanto, usaremos offsets
 * diferentes na hora de testar os trabalhos de fato. */
#define RANDOM_SEED_OFFSET 0

const int altura_caverna [] =
{4, 4, 4, 4,
 5, 5, 5, 5, 5,
 6, 6, 6, 6, 6, 6,
 7, 7, 7, 7, 7, 7, 7, 7,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

 const int largura_caverna [] =
{4, 4, 5, 6,
 5, 5, 5, 6, 7,
 6, 6, 6, 6, 7, 8,
 7, 7, 7, 7, 7, 7, 8, 9,
 8, 8, 8, 8, 8, 8, 8, 8, 9, 10,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 11,
 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 12};

const int n_pocos [] =
{1, 2, 3, 4,
 1, 2, 3, 4, 5,
 1, 2, 3, 4, 5, 6,
 1, 2, 3, 4, 5, 6, 7, 8,
 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

#define N_TIPOS_CAVERNA 57
#define N_JOGOS_POR_TIPO 100

/* Você pode substituir as linhas acima por estas, para testar só uma caverna.
const int altura_caverna [] = {10};
const int largura_caverna [] = {10};
const int n_pocos [] = {10};
#define N_TIPOS_CAVERNA 1
#define N_JOGOS_POR_TIPO 1*/

/*============================================================================*/

int main()
{
    int tipo_caverna, i;
    int n_rodadas = 0;
    int n_vitorias = 0;
    int n_sem_bateria = 0;

    /* Para cada tipo de caverna... */
    for (tipo_caverna = 0; tipo_caverna < N_TIPOS_CAVERNA; tipo_caverna++)
    {
        int max_rodadas = altura_caverna [tipo_caverna]*largura_caverna [tipo_caverna]*20;

        /* ... joga N vezes. */
        for (i = 0; i < N_JOGOS_POR_TIPO; i++)
        {
            GameState* game_state;
            Decisor* decisor;

            /* Gera o jogo e o decisor. */
            game_state = criaGameState (altura_caverna [tipo_caverna], largura_caverna [tipo_caverna], n_pocos [tipo_caverna], i*tipo_caverna + RANDOM_SEED_OFFSET);
            decisor = criaDecisor (altura_caverna [tipo_caverna], largura_caverna [tipo_caverna]);

            /* Loop principal do jogo. */
            while (!jogoTerminado (game_state) && game_state->n_rodadas < max_rodadas)
            {
                int direcao = proximoMovimento (decisor, game_state->pos_atual, posicaoAtualComAgua (game_state), nVizinhosComLava (game_state));
                move (game_state, direcao);

                /* Estas linhas permitem acompanhar o movimento do robô passo-a-passo. */
                if (PASSO_A_PASSO)
                {
                    system ("cls");
                    mostraMapaAtual (game_state);
                    //system("pause");
                }
            }

            n_rodadas += game_state->n_rodadas;
            if (jogoTerminado (game_state) && !game_state->morreu)
                n_vitorias++;
            else if (game_state->n_rodadas >= max_rodadas)
                n_sem_bateria++;

            destroiDecisor (decisor);
            destroiGameState (game_state);
        }
    }

    /* Resultados finais. */
    printf ("Vitorias: %d (%.2f%%)\nQuedas: %d\nSem Bateria: %d\nRodadas: %d\n", n_vitorias, ((float) n_vitorias / (float) (N_TIPOS_CAVERNA * N_JOGOS_POR_TIPO))*100.0f, N_TIPOS_CAVERNA*N_JOGOS_POR_TIPO - n_vitorias - n_sem_bateria, n_sem_bateria, n_rodadas);

    return (0);
}

/*============================================================================*/
