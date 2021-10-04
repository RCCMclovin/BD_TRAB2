#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "bucket.hpp"
//#define BLOCK_SIZE 4096
#define MAX 1023


typedef struct table_t{ //tamanho == 4096
    int values[MAX];
    int occupied;
}table_t;


void new_hash(int tam);
void add_key(int key, int value);
pair<int,int> get_values(int key);

#endif