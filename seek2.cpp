#include "b+tree/string/b+tree_str.hpp"
#include <string.h>

#define RECORD_BLOCK 2

typedef b_plus_tree_str_t b_plus_tree_s_t;

typedef struct record{
    int id, year, citations;
    char title[301], authors[151], update[20], snippet[1025];
}record_t;

typedef struct block{
    record_t records[RECORD_BLOCK];
    char unused[1070]; //espaço vago dentro do bloco
                       //não coube outro record_t
}block_t;

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Número insuficiente de argumentos.\nupload <file>\n");
        return 1;
    }
    string title = argv[1];
    FILE* arq = fopen("B+TREE_INT.txt", "r");
    fseek(arq,0,SEEK_END);
    int total = ftell(arq)/BLOCK_SIZE;
    if(total < 0){
        printf("Erro de leitura!\n");
        return 1;
    }
    fclose(arq);
    string a = "B+TREE_STR.dump";
    string b = "B+TREE_STR.txt";
    b_plus_tree_s_t* tree = load_b_plus_tree_str(a, b);
    int r = search_key(tree,title);
    printf("Quantidade de blocos no arquivo de dados: %d\n", total);
    record_t result;
    result.id = -1;
    arq = fopen("records.txt", "r");
    int err = fseek(arq,r*BLOCK_SIZE,SEEK_SET);
    if(err){
        printf("Erro de leitura!\n");
        return 1;
    }
    block_t block;
    err = fread(&block, BLOCK_SIZE, 1, arq);
    if(err != 1){
        printf("Erro de leitura!\n");
        return 1;
    }
    for(int i = 0; i < RECORD_BLOCK; i++) if(block.records[i].title == title) {
        printf("Registro encontrado:\nID: %d\nTitulo: %s\nAno: %d\nAutores: %s\nCitacoes: %d\nAtualizacao: %s\nSnippet: %s\n",
        block.records[i].id, block.records[i].title, block.records[i].year, block.records[i].authors, block.records[i].citations, block.records[i].update, block.records[i].snippet);
        return 0;
    }
    printf("Registro não encontrado.\n");
    return 0;
}