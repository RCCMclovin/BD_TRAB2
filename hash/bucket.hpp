/*
Header para os Buckets do hash
Constroí o arquivo HashIndex.txt para guardar os buckets
*/
#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define BLOCK_SIZE 4096
#define TAM 510
using namespace std; 

typedef struct bucket_t{
	int key[TAM];
	int value[TAM];
	int pos;
	int number;
	int next;
    int espaco; //só para completar 4096by
}bucket_t;

int create_bucket(); // novo bucket
bucket_t* read_bucket(int pointer);//ler um bucket
void add_key_bucket(bucket_t* n, int aKey, int aValue);// adicionar uma chave
pair<int, int> get_value(bucket_t* n, int aKey);//ver o valor de uma chave
void clear_buckets();//limpar todos os buckets
#endif