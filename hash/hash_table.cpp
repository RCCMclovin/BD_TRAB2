#include "hash_table.hpp"
int nblocks = 0;

int my_hash(int key){
    return key%(nblocks*MAX);
}



void load(){
    FILE* arq = fopen("HashTable.txt","r");
    int err = fseek(arq,0,SEEK_END);
    nblocks = ftell(arq)/BLOCK_SIZE;
    fclose(arq);
}

void new_table(){
    
    FILE* arq = fopen("HashTable.txt","r+");
    int err = fseek(arq,0,SEEK_END);
    table_t n;
    for(int i = 0; i < MAX;i++){
        n.values[i] = -1;
    }
    n.occupied = 0;
    err = fwrite(&n, BLOCK_SIZE, 1,arq);
    fclose(arq);
    if(err != 1){
        printf("Erro de Escrita!");
        return;
    }
}

table_t* read_table(int pos){
    FILE* arq = fopen("HashTable.txt","r");
    int err = fseek(arq, pos*BLOCK_SIZE, SEEK_SET);
    if(err){
        printf("erro de leitura\n");
        return NULL;
    }
    table_t* t = (table_t*) malloc(sizeof(table_t));
    err = fread(t,BLOCK_SIZE,1,arq);
    fclose(arq);
    if(err != 1) printf("Erro de leitura!\n");
    return t;
}

void write_table(table_t* table, int pos){
    FILE* arq = fopen("HashTable.txt","r+");
    int err = fseek(arq, pos*BLOCK_SIZE, SEEK_SET);
    err = fwrite(table, BLOCK_SIZE, 1,arq);
    fclose(arq);
    if(err != 1){
        printf("Erro de Escrita!");
        return;
    }
}


void add_key(int key, int value){
    if(nblocks == 0){
        load();
        if(nblocks == 0){
            new_table();
            load();
        } 
    }
    int pos = my_hash(key);
    int bpos = pos%MAX;
    pos-=bpos;
    int tpos = pos/MAX;
    table_t* t = read_table(tpos);
    int node_pos = t->values[bpos];
    if(node_pos == -1){
        node_pos = create_bucket();
        t->values[bpos] = node_pos;
        t->occupied++;
        write_table(t,tpos);
    }
    bucket_t* b = read_bucket(node_pos);
    add_key_bucket(b, key, value);
}

pair<int,int> get_values(int key){
    if(nblocks == 0){
        load();
        if(nblocks == 0){
            printf("A tabela está vazia!\nPreencha a tabela antes de fazer uma busca.\n");
            return pair<int,int> (-1,-1);
        } 
    }
    int read_blocks = 0;
    int pos = my_hash(key);
    int bpos = pos%MAX;
    pos-=bpos;
    int tpos = pos/MAX;
    
    table_t* t = read_table(tpos);
    read_blocks++;
    int node_pos = t->values[bpos];
    if(node_pos == -1) return pair<int,int> (-1, read_blocks);
    bucket_t* b = read_bucket(node_pos);
    read_blocks++;
    pair<int,int> result = get_value(b,key);
    return pair<int,int> (result.first, result.second + read_blocks);
}

void new_hash(int tam){
    FILE* arq = fopen("HashTable.txt","w");
    fclose(arq);
    nblocks = ((int) tam/MAX +1) * MAX;
    for(int i = 0; i < nblocks; i++) {
        new_table();
    }
    clear_buckets();
}