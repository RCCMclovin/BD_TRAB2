#include "hash/hash_table.hpp"
#include "b+tree/int/b+tree.hpp"
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

void read_record_file(char* path){
    FILE* arq = fopen(path, "r");
    FILE* out = fopen("records.txt", "w");
    fseek(arq,0,SEEK_SET);
    block_t aux;
    if(!arq){
        printf("Erro de leitura!\n");
        return;
    }
    int i = 0;
    int blk = 0;
    while(!feof(arq)){
        for(i = 0; i < RECORD_BLOCK; i++){
            if(feof(arq)) break;
            fscanf(arq, " %d\n", &aux.records[i].id);
            fscanf(arq, " %[^\n]", &aux.records[i].title);
            fscanf(arq, " %d\n", &aux.records[i].year);
            fscanf(arq, " %[^\n]", &aux.records[i].authors);
            fscanf(arq, " %d\n", &aux.records[i].citations);
            fscanf(arq, " %[^\n]", &aux.records[i].update);
            fscanf(arq, " %[^\n]", &aux.records[i].snippet);
            fscanf(arq, "%*c");
        }
        if(i < RECORD_BLOCK){
            while(i < RECORD_BLOCK){
                aux.records[i].id = -1;
            }
        }
        fwrite(&aux,BLOCK_SIZE, 1, out);
        blk++;
    }
    fclose(arq);
    fclose(out);
}
void generate_indexes(){
    FILE* arq = fopen("records.txt", "r");
    fseek(arq,0,SEEK_END);
    int pos = ftell(arq)/BLOCK_SIZE;
    if(pos < 0){
        printf("Erro de leitura!\n");
        return;
    }
    new_hash(pos*2);
    b_plus_tree_t* tree_int = create_b_plus_tree("B+TREE_INT.txt");
    b_plus_tree_s_t* tree_str = create_b_plus_tree_str("B+TREE_STR.txt");
    fseek(arq,0,SEEK_SET);
    block_t aux;
    for(int i = 0; i < pos; i++){
        int err = fread(&aux, BLOCK_SIZE, 1, arq);
        if(err != 1){
            printf("Erro de leitura!\n");
            return;
        }
        for(int j = 0; j < RECORD_BLOCK; j++){
            int id = aux.records[j].id;
            if(aux.records[j].id == -1) break;
            else{
                add_key(id,i);
                insert_entry(tree_int, {id,i});
                string str = aux.records[j].title;
                insert_entry(tree_str,{str, i});
            }
        }
    }

    bool is_tree_dumped =  dump_b_plus_tree(tree_int, "B+TREE_INT.dump");
    if(!is_tree_dumped){
        printf("Erro ao salvar a árvore\n");
        return;
    }
    bool is_tree_dumped =  dump_b_plus_tree_str(tree_str, "B+TREE_STR.dump");
    if(!is_tree_dumped)
        printf("Erro ao salvar a árvore\n");
}

void upload(char* path){
    read_record_file(path);
    generate_indexes();
}

void findrec(int id){
    FILE* arq = fopen("records.txt", "r");
    fseek(arq,0,SEEK_END);
    int total = ftell(arq)/BLOCK_SIZE;
    if(total < 0){
        printf("Erro de leitura!\n");
        return;
    }
    fclose(arq);

    pair<int,int> r = get_values(id);
    record_t result;
    result.id = -1;
    printf("Quantidade de blocos lidos: %d\nQuantidade de blocos no arquivo de dados: %d\n", r.second+1,total);
    arq = fopen("records.txt", "r");
    int err = fseek(arq,r.first*BLOCK_SIZE,SEEK_SET);
    if(err){
        printf("Erro de leitura!\n");
        return;
    }
    block_t block;
    err = fread(&block, BLOCK_SIZE, 1, arq);
    if(err != 1){
        printf("Erro de leitura!\n");
        return ;
    }
    for(int i = 0; i < RECORD_BLOCK; i++) if(block.records[i].id == id){
        printf("Registro encontrado:\nID: %d\nTitulo: %s\nAno: %d\nAutores: %s\nCitacoes: %d\nAtualizacao: %s\nSnippet: %s\n",
        block.records[i].id, block.records[i].title, block.records[i].year, block.records[i].authors, block.records[i].citations, block.records[i].update, block.records[i].snippet);
        return;
    }

    
}
void seek1(int id){
    FILE* arq = fopen("B+TREE_INT.txt", "r");
    fseek(arq,0,SEEK_END);
    int total = ftell(arq)/BLOCK_SIZE;
    if(total < 0){
        printf("Erro de leitura!\n");
        return;
    }
    fclose(arq);

    b_plus_tree_t* tree = load_b_plus_tree("B+TREE_INT.dump", "B+TREE_INT.txt");
    int r = search_key(tree, id);
    printf("Quantidade de blocos no arquivo de dados: %d\n", total);
    arq = fopen("records.txt", "r");
    int err = fseek(arq,r*BLOCK_SIZE,SEEK_SET);
    if(err){
        printf("Erro de leitura!\n");
        return ;
    }
    block_t block;
    err = fread(&block, BLOCK_SIZE, 1, arq);
    if(err != 1){
        printf("Erro de leitura!\n");
        return ;
    }
    for(int i = 0; i < RECORD_BLOCK; i++) if(block.records[i].id == id) {
        printf("Registro encontrado:\nID: %d\nTitulo: %s\nAno: %d\nAutores: %s\nCitacoes: %d\nAtualizacao: %s\nSnippet: %s\n",
        block.records[i].id, block.records[i].title, block.records[i].year, block.records[i].authors, block.records[i].citations, block.records[i].update, block.records[i].snippet);
        return;
    }
}
void seek2(string title){
    FILE* arq = fopen("B+TREE_INT.txt", "r");
    fseek(arq,0,SEEK_END);
    int total = ftell(arq)/BLOCK_SIZE;
    if(total < 0){
        printf("Erro de leitura!\n");
        return;
    }
    fclose(arq);

    b_plus_tree_s_t* tree = load_b_plus_tree_str("B+TREE_STR.dump", "B+TREE_STR.txt");
    int r = search_key(tree,title);
    printf("Quantidade de blocos no arquivo de dados: %d\n", total);
    record_t result;
    result.id = -1;
    FILE* arq = fopen("records.txt", "r");
    int err = fseek(arq,r*BLOCK_SIZE,SEEK_SET);
    if(err){
        printf("Erro de leitura!\n");
        return ;
    }
    block_t block;
    err = fread(&block, BLOCK_SIZE, 1, arq);
    if(err != 1){
        printf("Erro de leitura!\n");
        return;
    }
    for(int i = 0; i < RECORD_BLOCK; i++) if(block.records[i].title == title) {
        printf("Registro encontrado:\nID: %d\nTitulo: %s\nAno: %d\nAutores: %s\nCitacoes: %d\nAtualizacao: %s\nSnippet: %s\n",
        block.records[i].id, block.records[i].title, block.records[i].year, block.records[i].authors, block.records[i].citations, block.records[i].update, block.records[i].snippet);
        return;
    }
    printf("Registro não encontrado.\n");
}

int main(){
    char command[100];
    int id;
    printf("Digite um comando:\n(quit para sair)\n");
    while(1){
        scanf(" %s", command);
        if(strcmp(command, "upload") == 0){
            scanf(" %s", command);
            upload(command);
        }
        else if(strcmp(command, "findrec") == 0){
            scanf(" %d", id);
            findrec(id);
        }
        else if(strcmp(command, "seek1") == 0){
            scanf(" %d", id);
            findrec(id);
        }
    }
    return 0;
}