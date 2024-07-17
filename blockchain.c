#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtwister.h"
#include "openssl/crypto.h"
#include "openssl/sha.h"

// define para padronizar o tamanho do data e nao gerar escrita ilegal

#define TAM_DATA 183
#define QUANT_BLOCOS 1000
//------------------------------------------------------------------------------------
// structs para o desenvolvimento do projeto, elas irao armazenar os dados e faram
// parte da lista encadeada.

typedef struct BlocoNaoMinerado {
     unsigned int numero;
     unsigned int nonce;
     unsigned char data[TAM_DATA];
     unsigned char hashAnterior[SHA256_DIGEST_LENGTH];
} BlocoNaoMinerado;

typedef struct BlocoMinerado {
    BlocoNaoMinerado bloco;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    struct BlocoMinerado *prox;
} BlocoMinerado;
//------------------------------------------------------------------------------------
// funcao utilizada para inicializar o vetor data do bloco, para evitar armazenar
// lixo nos blocos.

void inicializarData(BlocoNaoMinerado *p)
{
    for (int i = 0; i < TAM_DATA; ++i)
        p->data[i] = 0;
}
//------------------------------------------------------------------------------------
// funcao utilizada para imprimir o hash

void printHash(unsigned char hash[], int length)
{
    int i;
    for(i=0;i<length;++i){
        printf("%02x", hash[i]);
    }
    printf("\n");
}
//------------------------------------------------------------------------------------
// funcao utilizada para inserir o bloco nao minerado na lista de blocos minerados

BlocoMinerado *insereLista(BlocoMinerado **prim, unsigned char hash[SHA256_DIGEST_LENGTH], BlocoNaoMinerado bloco)
{
    BlocoMinerado *aux = *prim;
    *prim = malloc(sizeof(BlocoMinerado));
    if(*prim == NULL)
        return NULL;
    (*prim)->bloco = bloco;
    for(int i=0; i<SHA256_DIGEST_LENGTH; i++){
        (*prim)->hash[i] = hash[i];
    }
    (*prim)->prox = aux;
    return *prim;
}
//------------------------------------------------------------------------------------
// funcao que utiliza busca sequencial para encontrar um bloco utilizando o seu numero.

BlocoMinerado *buscarNaLista(BlocoMinerado *prim, unsigned int k)
{
    if (prim == NULL)
        return NULL;
    if (prim->bloco.numero == k)
        return prim;
    return buscarNaLista(prim->prox, k);
}
//------------------------------------------------------------------------------------
// funcao auxiliar a funcao ordenaVetor que realiza a selecao do maior e ja faz a
// troca com a posicao ini.

int selecao(int vet[256][2], int ini, int fim)
{
    int iMaior = ini;
    for (int i = ini+1; i <= fim; ++i)
    {
        if (vet[i][1] > vet[iMaior][1])
            iMaior = i;
    }
    int aux = vet[ini][0];
    int aux2 = vet[ini][1];
    vet[ini][0] = vet[iMaior][0];
    vet[ini][1] = vet[iMaior][1];
    vet[iMaior][0] = aux;
    vet[iMaior][1] = aux2;
}
//------------------------------------------------------------------------------------
// funcao que realiza as chamadas para ordenar o vetor de enderecos.

void ordenarVetor(int vet[256][2], int ini, int fim)
{
    for (int i = ini; i <= fim; ++i)
        selecao(vet, i, fim);
}
//------------------------------------------------------------------------------------
// funcao que busca a quantidade de bitcoins de um endereco recebendo o seu endereco.

int buscarEndereco(int vet[256][2], int k)
{
    for(int i=0;i<=255;i++){
	if(vet[i][0] == k)
	    return vet[i][1];
    }
}
//------------------------------------------------------------------------------------
// funcao utilizada para realizar chamadas as funcoes que resolvem os problemas
// sugeridos pelo professor.

void menu(BlocoMinerado *prim, int bitcoin[256][2])
{
    unsigned int numero; // variavel auxiliar para receber os numeros/opcoes que o usuario ira informar
    int opcaoMenu;
    BlocoMinerado *aux = NULL;
    do {
        printf("\nO que deseja descobrir?\n\t1. Qual o hash pelo numero do bloco?\n\t2. Quantos bitcoins um endereco tem?\n\t3. Qual o endereco tem mais bitcoins?\n\t4. Listar enderecos\n\t5. Fechar programa\n\n\tOpcao: ");
        scanf("%d", &opcaoMenu);
        switch (opcaoMenu) {
            case 1:
            {
                printf("Digite o numero do bloco para consultar o hash.\n");
                scanf("%d", &numero);
                aux = buscarNaLista(prim, numero);
                printf("Bloco %d: ", aux->bloco.numero);
                printHash(aux->hash,SHA256_DIGEST_LENGTH);
                break;
            }
            case 2:
            {
                printf("Digite o endereco que deseja saber a quantidade de bitcoins.\n");
                scanf("%d", &numero);
                printf("\n%d: %d bitcoins.\n", numero, buscarEndereco(bitcoin, numero));
                break;
            }
            case 3:
            {
                printf("\nBloco %d: %d bitcoins.\n", bitcoin[0][0], bitcoin[0][1]);
                break;
            }
            case 4:
            {
                for(int i=0;i<=255;i++){
                    printf("\n%d: %d bitcoins.", bitcoin[i][0], bitcoin[i][1]);
                }
		printf("\n");
                break;
            }
            case 5:
            {
                printf("\nEncerrando programa!\n\n");
                break;
            }
            default:
            {
                printf("\nOpcao invalida!\n");
                break;
            }
        }
    } while (opcaoMenu != 5);

}
//------------------------------------------------------------------------------------
// funcao principal que realiza toda a criacao, preenchimento, mineracao e
// encadeamento dos blocos.

int main()

{
    BlocoMinerado *prim = NULL;
    BlocoNaoMinerado blocoNaoMinerado;
    int bitcoin[256][2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned char numeroTransacoes;
    unsigned long int contaNonces;
    MTRand r = seedRand(1234567);

    // inicializar enderecos
    for(int i=0;i<=255;i++)
    {
        bitcoin[i][0] = i+1;
        bitcoin[i][1] = 0;
    }

    // inicializando hash;
    for(int i=0; i<SHA256_DIGEST_LENGTH; ++i)
    {
        hash[i] = 0;
    }

    // preenchendo dados do bloco
    for (int i = 1; i <= QUANT_BLOCOS; ++i)
    {
        blocoNaoMinerado.numero = i;// numero

        for(int j=0; j<SHA256_DIGEST_LENGTH; ++j)
        {
            blocoNaoMinerado.hashAnterior[j] = hash[j];// hash anterior
        }

        inicializarData(&blocoNaoMinerado);// inicializando a data em 0
        numeroTransacoes = 1 + genRandLong(&r) % (61*3);

        for (int j=0; j<numeroTransacoes; j=j+3)
        {
            blocoNaoMinerado.data[j] = genRandLong(&r) % 256;// endereco origem
            blocoNaoMinerado.data[j+1] = genRandLong(&r) % 256;// endereco destino
            blocoNaoMinerado.data[j+2] = 1+ genRandLong(&r) % 50;// numero bitcoins

            bitcoin[(blocoNaoMinerado.data[j])-1][1] -= blocoNaoMinerado.data[j+2];
            bitcoin[(blocoNaoMinerado.data[j+1])-1][1] += blocoNaoMinerado.data[j+2];
        }// preenchendo data

        // excluindo numeros negativos dos enderecos, pois e impossivel um numero
        // ter um numero negativo de bitcoins
        for(int j=0;j<=255;j++)
        {
            if(bitcoin[j][1] < 0)
            {
                bitcoin[j][1] = 0;
            }
    	}

        // mineracao do bloco
        contaNonces = 0;
        do{
	    if (contaNonces == 4294967296)
		break;
            blocoNaoMinerado.nonce = contaNonces;
            SHA256((unsigned char *)&blocoNaoMinerado, sizeof(BlocoNaoMinerado), hash);
            contaNonces++;
        } while(!(hash[0] == 0 && hash[1] == 0 && hash[2] == 0 && hash[3] == 0));
	if (contaNonces == 4294967296) {
            contaNonces = 0;
            do{
       		blocoNaoMinerado.nonce = contaNonces;
       		SHA256((unsigned char *)&blocoNaoMinerado, sizeof(BlocoNaoMinerado), hash);
       		contaNonces++;
            } while(!(hash[0] == 0 && hash[1] == 0 && hash[2] == 0));
	}

        printHash(hash, SHA256_DIGEST_LENGTH);
        insereLista(&prim, hash, blocoNaoMinerado);
    }

    ordenarVetor(bitcoin, 0, 255);
    // chamada para o menu
    menu(prim, bitcoin);
    return 0;
}
