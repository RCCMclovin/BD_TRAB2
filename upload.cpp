#include "hash/hash_table.hpp"
#include "b+tree/string/b+tree_str.hpp"
#include "b+tree/int/b+tree.hpp"


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

typedef struct dump_data{
  int next_block, root_pointer;
}dump_data_t;

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
            fscanf(arq, " %[^\n]", aux.records[i].title);
            fscanf(arq, " %d\n", &aux.records[i].year);
            fscanf(arq, " %[^\n]", aux.records[i].authors);
            fscanf(arq, " %d\n", &aux.records[i].citations);
            fscanf(arq, " %[^\n]", aux.records[i].update);
            fscanf(arq, " %[^\n]", aux.records[i].snippet);
            fscanf(arq, "%*c");
        }
        if(i < RECORD_BLOCK){
            while(i < RECORD_BLOCK){
                aux.records[i].id = -1;
                i++;
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

    string b = "B+TREE_INT.dump";
    bool is_tree_dumped =  dump_b_plus_tree(tree_int, b);
    if(!is_tree_dumped){
        printf("Erro ao salvar a árvore\n");
        return;
    }
    string a = "B+TREE_STR.dump";
    FILE* dump_file = fopen(a.c_str(), "w");
    if(dump_file == NULL){
        printf("[dump_b_plus_tree()] Não foi possível abrir o arquivo com permissão 'w'\n");
    }
    int err = fseek(tree_str->index_file, 0, SEEK_SET);
	if(err){
		printf("[dump_b_plus_tree()] Erro de seek.\n");
        fclose(dump_file);
	}
    dump_data_t data = {tree_str->next_block, tree_str->root_pointer};
    size_t error = fwrite(&data, sizeof(dump_data_t), 1, dump_file);
    if(error != 1){
        printf("[dump_b_plus_tree()] Erro de escrita.\n");
        fclose(dump_file);
    }
    fclose(dump_file);
    
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Número insuficiente de argumentos.\nupload <file>\n");
        return 1;
    }
    read_record_file(argv[1]);
    generate_indexes();
}