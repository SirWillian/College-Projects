
#include <stdio.h>
#include <stdlib.h>
#include "t3-1717553-1717502-1723073.h"

int decideMovimento(Decisor* d, Coordenada pos_atual)
{
    /*i serve para qualquer contagem em geral que faremos dentro do programa.
      random para fazer movimentos aleatorios dentro da condicao que o robo esta dentro de um loop.
      random eh utilizado para pegar varios valores de random ateh que um seja valido para o movimento*/
    int i, random=1;
    Coordenada menor_distancia, pos_desconhecida;
    int direcoes[]={0,0,0,0}; /*Posicoes: 0-Cima, 1-Esquerda, 2-Baixo, 3-Direita
                                Legenda do vetor: 0-Livre e conhecido, 1-Posição anterior, 2-Nao interessante, (-1)-Livre e desconhecido*/

    /*movimentos iniciais para melhor reconhecimento do mapa*/
    if(d->n_movimentos==0)
        return(DIR_MOVIMENTO_BAIXO);
    if(d->n_movimentos==1)
        return(DIR_MOVIMENTO_DIREITA);
    if(d->n_movimentos==2)
        return(DIR_MOVIMENTO_CIMA);

    /*Checa se está preso*/
    if (!estaLivre(d))
        d->vou_chutar=1;
    else
        d->vou_chutar=0;
    menor_distancia.y=d->altura;
    menor_distancia.x=d->largura;

    /*Calcula a distância até o local desconhecido e desejado mais próximo no mapa*/
    distanciaAoDesconhecido(d, pos_atual, &pos_desconhecida, &menor_distancia);

    /*Marca a direção de onde veio para ser usada como última opção*/
    if(d->ultimo_mov==DIR_MOVIMENTO_ESQUERDA)
        direcoes[3]=1;
    if(d->ultimo_mov==DIR_MOVIMENTO_DIREITA)
        direcoes[1]=1;
    if(d->ultimo_mov==DIR_MOVIMENTO_CIMA)
        direcoes[2]=1;
    if(d->ultimo_mov==DIR_MOVIMENTO_BAIXO)
        direcoes[0]=1;

    /*Marca direções como não interessantes se baseando na necessidade de chutar*/
    if(!d->vou_chutar)
    {
        if(pos_atual.x==0||d->memo[pos_atual.y][pos_atual.x-1]!=LIVRE)
            direcoes[1]=2;
        if(pos_atual.x==d->largura-1||d->memo[pos_atual.y][pos_atual.x+1]!=LIVRE)
            direcoes[3]=2;
        if(pos_atual.y==0||d->memo[pos_atual.y-1][pos_atual.x]!=LIVRE)
            direcoes[0]=2;
        if(pos_atual.y==d->altura-1||d->memo[pos_atual.y+1][pos_atual.x]!=LIVRE)
            direcoes[2]=2;
    }
    /*Marca posicoes que nao sao possiveis para o robo andar*/
    else
    {
        if(pos_atual.x==0||d->memo[pos_atual.y][pos_atual.x-1]==LAVA)
            direcoes[1]=2;
        if(pos_atual.x==d->largura-1||d->memo[pos_atual.y][pos_atual.x+1]==LAVA)
            direcoes[3]=2;
        if(pos_atual.y==0||d->memo[pos_atual.y-1][pos_atual.x]==LAVA)
            direcoes[0]=2;
        if(pos_atual.y==d->altura-1||d->memo[pos_atual.y+1][pos_atual.x]==LAVA)
            direcoes[2]=2;
    }

    /*Marca direções de preferência */
    if(direcoes[0]==0)
        if(pos_desconhecida.y<pos_atual.y)
            direcoes[0]=-1;
    if(direcoes[1]==0)
        if(pos_desconhecida.x<pos_atual.x)
            direcoes[1]=-1;
    if(direcoes[2]==0)
        if(pos_desconhecida.y>pos_atual.y)
            direcoes[2]=-1;
    if(direcoes[3]==0)
        if(pos_desconhecida.x>pos_atual.x)
            direcoes[3]=-1;

    /*Se estivermos em um loop, movemos aleatoriamente em posicoes seguras para sair do loop*/
    if(d->foge_loop!=0)
    {
        random=rand()%4+1;
        if(direcoes[0]!=2&&random==1)
        {
            d->foge_loop--;
            return(DIR_MOVIMENTO_CIMA);
        }
        if(direcoes[3]!=2&&random==2)
        {
            d->foge_loop--;
            return(DIR_MOVIMENTO_DIREITA);
        }
        if(direcoes[2]!=2&&random==3)
        {
            d->foge_loop--;
            return(DIR_MOVIMENTO_BAIXO);
        }
        if(direcoes[1]!=2&&random==4)
        {
            d->foge_loop--;
            return(DIR_MOVIMENTO_ESQUERDA);
        }
    }

    /*Decide o movimento, dando prioridade adequadamente de acordo com os valores dados anteriormente*/
    for(i=3; i>=0; i--)
        if(direcoes[i]==-1)
        {
            if(i==3)
                return(DIR_MOVIMENTO_DIREITA);
            if(i==2)
                return(DIR_MOVIMENTO_BAIXO);
            if(i==1)
                return(DIR_MOVIMENTO_ESQUERDA);
            if(i==0)
                return(DIR_MOVIMENTO_CIMA);
        }

    for(i=3; i>=0; i--)
        if(direcoes[i]==0)
        {
            if(i==3)
                return(DIR_MOVIMENTO_DIREITA);
            if(i==2)
                return(DIR_MOVIMENTO_BAIXO);
            if(i==1)
                return(DIR_MOVIMENTO_ESQUERDA);
            if(i==0)
                return(DIR_MOVIMENTO_CIMA);
        }

    for(i=3; i>=0; i--)
        if(direcoes[i]==1)
        {
            if(i==3)
                return(DIR_MOVIMENTO_DIREITA);
            if(i==2)
                return(DIR_MOVIMENTO_BAIXO);
            if(i==1)
                return(DIR_MOVIMENTO_ESQUERDA);
            if(i==0)
                return(DIR_MOVIMENTO_CIMA);
        }
}

int estaLivre (Decisor* d)
{
    int i, j;

    /*Procura por posições que não tenham lava e que ainda não tenham sido exploradas*/
    for(i=0; i<d->altura; i++)
        for(j=0; j<d->largura; j++)
            if (d->memo[i][j]==LIVRE&&d->mapa[i][j]==DESC)
                return (1);
    return (0);
}

void distanciaAoDesconhecido (Decisor* d, Coordenada pos_atual, Coordenada* pos_desconhecida, Coordenada* menor_distancia)
{
    int i, j;
    Coordenada distancia;

    /* Procura a menor distância até uma casa livre e não explorada checando 4 quadrantes, tomando como origem a posição atual:
     * quadrante: superior esquerdo, superior direito, inferior esquerdo e inferior direito */
    for(i=0; i<d->altura; i++)
        for(j=0; j<d->largura; j++)
            {
                if(i<=pos_atual.y&&j<=pos_atual.x)
                {
                    distancia.y=pos_atual.y-i;
                    distancia.x=pos_atual.x-j;

                    if(!d->vou_chutar)
                    {
                        if (d->memo[i][j]==LIVRE&&d->mapa[i][j]==DESC)
                            calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                    }
                    else
                        if (d->memo[i][j]!=LAVA&&d->mapa[i][j]==DESC)
                           calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                }
                else if (i<=pos_atual.y&&j>pos_atual.x)
                {
                    distancia.y=pos_atual.y-i;
                    distancia.x=j-pos_atual.x;

                    if(!d->vou_chutar)
                    {
                        if (d->memo[i][j]==LIVRE&&d->mapa[i][j]==DESC)
                           calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                    }
                    else
                        if (d->memo[i][j]!=LAVA&&d->mapa[i][j]==DESC)
                           calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                }
                else if (i>pos_atual.y&&j<=pos_atual.x)
                {
                    distancia.y=i-pos_atual.y;
                    distancia.x=pos_atual.x-j;

                    if(!d->vou_chutar)
                    {
                        if (d->memo[i][j]==LIVRE&&d->mapa[i][j]==DESC)
                           calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                    }
                    else
                        if (d->memo[i][j]!=LAVA&&d->mapa[i][j]==DESC)
                           calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                }
                else
                {
                    distancia.y=i-pos_atual.y;
                    distancia.x=j-pos_atual.x;

                    if(!d->vou_chutar)
                    {
                        if (d->memo[i][j]==LIVRE&&d->mapa[i][j]==DESC)
                           calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                    }
                    else
                        if (d->memo[i][j]!=LAVA&&d->mapa[i][j]==DESC)
                           calculaDistancia(d, distancia, menor_distancia, pos_atual, i, j, pos_desconhecida);
                }
            }
}

void calculaDistancia(Decisor* d, Coordenada distancia, Coordenada* menor_distancia, Coordenada pos_atual, int i, int j, Coordenada* pos_desconhecida)
{
    /* Caso a distância até a posição em questão seja menor do que a menor distância anterior,
     * reajusta a menor distância */
    if ((distancia.y)+(distancia.x)<((menor_distancia->y)+(menor_distancia->x)))
    {
        menor_distancia->y=distancia.y;
        menor_distancia->x=distancia.x;
        pos_desconhecida->y=i;
        pos_desconhecida->x=j;
    }
    /* Caso a distância seja igual a anterior, há a preferência por casas mais a esquerda e abaixo no mapa */
    else if ((distancia.y)+(distancia.x)==((menor_distancia->y)+(menor_distancia->x)))
    {
        if (distancia.x>=menor_distancia->x)
            if((j>pos_atual.x)||(i>pos_atual.y))
            {
                menor_distancia->y=distancia.y;
                menor_distancia->x=distancia.x;
                pos_desconhecida->y=i;
                pos_desconhecida->x=j;
            }
    }


}

void preencheMemo(int lavas, Decisor* d, Coordenada lugar)
{
    if(lavas)
    {
    /*Caso haja uma parede ou caso já não tenhamos certeza de que a posição em questão está livre, marcamos a posição como uma indeterminação (flag)
     *Checa cada uma das posições adjacentes para colocar uma flag na posição*/

        if((lugar.y)!=0&&(d->memo[lugar.y-1][lugar.x])!=0&&(d->memo[lugar.y-1][lugar.x])!=LAVA)
            d->memo[lugar.y-1][lugar.x]=FLAG;

        if(lugar.y!=(d->altura-1)&&d->memo[lugar.y+1][lugar.x]!=0&&d->memo[lugar.y+1][lugar.x]!=LAVA)
            d->memo[lugar.y+1][lugar.x]=FLAG;

        if(lugar.x!=0&&d->memo[lugar.y][lugar.x-1]!=0&&d->memo[lugar.y][lugar.x-1]!=LAVA)
            d->memo[lugar.y][lugar.x-1]=FLAG;

        if(lugar.x!=(d->largura-1)&&d->memo[lugar.y][lugar.x+1]!=0&&d->memo[lugar.y][lugar.x+1]!=LAVA)
            d->memo[lugar.y][lugar.x+1]=FLAG;
    }
    else
    {
    /*Caso não haja obstáculos, preenche os locais adjacentes com LIVRE na memória*/
        if(lugar.y!=0)
            d->memo[lugar.y-1][lugar.x]=LIVRE;

        if(lugar.y!=(d->altura-1))
            d->memo[lugar.y+1][lugar.x]=LIVRE;

        if(lugar.x!=0)
            d->memo[lugar.y][lugar.x-1]=LIVRE;

        if(lugar.x!=(d->largura-1))
            d->memo[lugar.y][lugar.x+1]=LIVRE;
    }
}
void preencheMapa(int lavas, Decisor* d, Coordenada lugar)
{
    if(lavas)//se ha lavas, coloca-se entao o numero de lavas
        d->mapa[lugar.y][lugar.x]=lavas;

    else//caso contrario, a posicao no mapa sera livre
        d->mapa[lugar.y][lugar.x]=LIVRE;
}

void atualizaMapaMemo(Decisor* d)
{
    int i, j;
    for(i=0;i<d->altura;i++)
    {
        for(j=0;j<d->largura;j++)
        {
            if(d->mapa[i][j]>0)
                checaFlagLocal(d,i,j);
        }
    }
}

void checaFlagLocal (Decisor* d, int i, int j)  //I=linha, J=coluna
{
    int n_flag=0;
    int lavas_conhecidas=0;
        /*Se existirem alertas de lava(flag) ou paredes ao redor da casa*/
        if(i!=0&&d->memo[i-1][j]==FLAG)
            n_flag++;
        if(i!=(d->altura)-1&&d->memo[i+1][j]==FLAG)
            n_flag++;

        /*Se existirem lavas conhecidas ao redor*/
        if(i!=0&&d->memo[i-1][j]==LAVA)
            lavas_conhecidas++;
        if(i!=(d->altura)-1&&d->memo[i+1][j]==LAVA)
            lavas_conhecidas++;

        /*O mesmo descrito acima, só que na horizontal...*/
        if(j!=0&&d->memo[i][j-1]==FLAG)
            n_flag++;
        if(j!=(d->largura)-1&&d->memo[i][j+1]==FLAG)
            n_flag++;

        if(j!=0&&d->memo[i][j-1]==LAVA)
            lavas_conhecidas++;
        if(j!=(d->largura)-1&&d->memo[i][j+1]==LAVA)
            lavas_conhecidas++;

        if(d->mapa[i][j]-lavas_conhecidas==n_flag)
        {
            d->mapa[i][j]=0;
            /*Se pudermos concluir que as posiçoes de flag são lavas,muda as flags para lavas*/
            if(i!=0&&d->memo[i-1][j]==FLAG)
                d->memo[i-1][j]=LAVA;
            if(i!=(d->altura)-1&&d->memo[i+1][j]==FLAG)
                d->memo[i+1][j]=LAVA;
            if(j!=0&&d->memo[i][j-1]==FLAG)
                d->memo[i][j-1]=LAVA;
            if(j!=(d->largura)-1&&d->memo[i][j+1]==FLAG)
                d->memo[i][j+1]=LAVA;
        }
        if(d->mapa[i][j]-lavas_conhecidas==0)
        {
            if(i!=0&&d->memo[i-1][j]==FLAG)
                d->memo[i-1][j]=LIVRE;
            if(i!=(d->altura)-1&&d->memo[i+1][j]==FLAG)
                d->memo[i+1][j]=LIVRE;
            if(j!=0&&d->memo[i][j-1]==FLAG)
                d->memo[i][j-1]=LIVRE;
            if(j!=(d->largura)-1&&d->memo[i][j+1]==FLAG)
                d->memo[i][j+1]=LIVRE;
        }


}

void pegaAtalho(Decisor* d, Coordenada pos_atual)
{
    Coordenada prox_pos=d->pilha[d->n_movimentos];
    int i;

    /* Caso a casa ja irá ser visitada em algum ponto na volta,
     * pega-se um atalho e se ignora os movimentos de chegada ao mesmo lugar */
    for (i=0; i<d->n_movimentos-1; i++)
        if(d->pilha[i].y==prox_pos.y&&d->pilha[i].x==prox_pos.x)
            d->n_movimentos=i;

    /* Caso um certo movimento ja esta dentro do escopo das casas conhecidas dentro da pilha,
     * pulamos ate a posicao para aquele movimento */
    for(i=d->n_movimentos;i>=0;i--)
    {
        if((d->pilha[i].x==pos_atual.x-1)&&(d->pilha[i].y==pos_atual.y))
            d->n_movimentos=i;
        if((d->pilha[i].x==pos_atual.x+1)&&(d->pilha[i].y==pos_atual.y))
            d->n_movimentos=i;
        if((d->pilha[i].y==pos_atual.y-1)&&(d->pilha[i].x==pos_atual.x))
            d->n_movimentos=i;
        if((d->pilha[i].y==pos_atual.y+1)&&(d->pilha[i].x==pos_atual.x))
            d->n_movimentos=i;
    }
}

int decideVolta(Decisor* d, Coordenada pos_atual)
{
    /* Vê o melhor caminho para a posicao seguinte */
    pegaAtalho(d, pos_atual);

    /* Pega o movimento de acordo com a proxima posicao na pilha */
    if (d->pilha[d->n_movimentos].y-pos_atual.y==-1)
        return(DIR_MOVIMENTO_CIMA);
    if (d->pilha[d->n_movimentos].y-pos_atual.y==1)
        return(DIR_MOVIMENTO_BAIXO);
    if (d->pilha[d->n_movimentos].x-pos_atual.x==-1)
        return(DIR_MOVIMENTO_ESQUERDA);
    if (d->pilha[d->n_movimentos].x-pos_atual.x==1)
        return(DIR_MOVIMENTO_DIREITA);
}
int estamosEmLoop(Decisor* d, Coordenada pos_atual)
{
    /* A função procura pela última vez em que esteve na casa atual, encontrando o período do loop (caso realmente seja um loop).
     * Nota: o período é inicializado como a quantidade máxima de movimentos, para que nunca entremos no while sem a necessidade. */

    int i, periodo=d->largura*d->altura*20, loops=0;
    if(d->n_movimentos>80)
    {   for(i=d->n_movimentos-1;i>=0;i--)
        {
            if(d->pilha[i].x==pos_atual.x&&d->pilha[i].y==pos_atual.y)
                periodo=d->n_movimentos-i;
            while(i-periodo>=0)
            {
                if((d->pilha[i-periodo].x==d->pilha[i].x)&&(d->pilha[i-periodo].y==d->pilha[i].y))
                {
                    periodo+=periodo;
                    loops++;
                    if(loops>=2)
                        return(1);
                }
                else
                {
                   return(0);
                }
            }
        }
    }
    return(0);
}
int proximoMovimento (Decisor* d, Coordenada pos, int agua, int n_lava)
{
    int mov; //Indica o movimento a ser tomado
    /*Se estivermos em uma casa com agua, comecamos a voltar e inicializamos o n_movimentos para pegar o valor do ultimo movimento feito*/
    if(agua)
    {
        d->indo=0;
        d->n_movimentos--;
    }

    /*Atualiza o mapa e a memoria de acordo para tomarmos as decisoes corretas*/
    preencheMapa(n_lava, d, pos);
    preencheMemo(n_lava, d, pos);
    atualizaMapaMemo(d);

    if(d->indo)
    {
        /*Checa se estamos em loop
          decide o movimento
          coloca o ultimo movimento dentro do decisor e preenche a pilha de acordo*/
        if(estamosEmLoop(d,pos))
            d->foge_loop=15;
        mov=decideMovimento(d, pos);
        d->ultimo_mov=mov;
        d->pilha[d->n_movimentos]=pos;
        d->n_movimentos++;
    }
    /*pegamos a volta e pegamos atalhos de acordo*/
    else
    {
        mov=decideVolta(d, pos);
        d->n_movimentos--;
    }
    return(mov);
}
Decisor* criaDecisor (int altura, int largura)
{
    Decisor* robo;
    int i,j;

    /*Aloca Decisor*/
    robo = (Decisor*) malloc(sizeof(Decisor));
    robo->memo=(char**)malloc(sizeof(char*)*altura);
    robo->mapa=(char**)malloc(sizeof(char*)*altura);
    robo->pilha=(Coordenada*)malloc(sizeof(Coordenada)*altura*largura*20);

    for(i=0;i<altura;i++)
    {
        robo->mapa[i]=(char*)malloc(sizeof(char)*largura);
        robo->memo[i]=(char*)malloc(sizeof(char)*largura);
    }
    for(i=0;i<altura;i++)
    {
        for(j=0;j<largura;j++)
        {
            if(i<2&&j<2)
            {
                robo->mapa[i][j]=DESC;
                robo->memo[i][j]=LIVRE;
                continue;//A primeira seção da memória (primeiros 4 quadrados do topo esquerdo) sempre será livre
            }
            robo->mapa[i][j]=DESC;
            robo->memo[i][j]=DESC;
        }
    }
    robo->altura=altura;
    robo->largura=largura;
    robo->indo=1;
    robo->ultimo_mov=DIR_MOVIMENTO_DIREITA;
    robo->vou_chutar=0;
    robo->n_movimentos=0;
    robo->foge_loop=0;
    return(robo);
}

void destroiDecisor (Decisor* d)
{
    int i;
    for(i=0; i<d->altura; i++)
    {
        free(d->mapa[i]);
        free(d->memo[i]);
    }
    free(d->mapa);
    free(d->memo);
    free(d->pilha);
    free(d);
}
