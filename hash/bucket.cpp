#include "bucket.hpp"

bucket_t* new_bucket(int num){
	bucket_t* n = (bucket_t*) malloc(sizeof(bucket_t));
	n->pos = 0;
	n->number = num;
	n->next = -1;
	for(int i = 0; i < TAM; i++){
		n->key[i] = -1;
		n->value[i] = -1;
	}
	return n;
}
bucket_t* read_bucket(int pointer){
	if( pointer == -1) return NULL;
	FILE* arq = fopen("HashIndex.txt","r");
	fflush(arq);
	int err = fseek(arq, pointer*BLOCK_SIZE, SEEK_SET);
	if(err){
		printf("Erro de leitura!1\n");
		fclose(arq);
		return NULL; 
	}else{
		bucket_t* prox = (bucket_t*) malloc(sizeof(bucket_t));
		err = fread(prox, BLOCK_SIZE, 1, arq);
		if(err != 1) {
			printf("Erro de leitura %d!0\n",err);
			return NULL;
		}
		fclose(arq);
		return prox;
	}
}

void add_key_bucket(bucket_t* n, int aKey, int aValue){
	if(n->pos == TAM){
		bucket_t* aux = n;
		int aux_next = n->next;
		while(aux_next != -1){
			aux = read_bucket(aux_next);
			if(aux->pos == TAM) aux_next = aux->next;
			else break;
		}

		if(aux->pos == TAM){
			FILE* arq = fopen("HashIndex.txt","r+");
			int err = fseek(arq, 0, SEEK_END);

			if(err){
				printf("Erro de leitura!7\n");
				fclose(arq);
				return; 

			}else{
				err = ftell(arq);

				if(err == -1){
					printf("Erro de leitura!6\n");
					fclose(arq);
					return; 

				}else{
					int npos = err/BLOCK_SIZE;
					aux->next = npos;
					bucket_t* nn = new_bucket(npos);
					add_key_bucket(nn, aKey, aValue);
					int err = fseek(arq, aux->number*BLOCK_SIZE, SEEK_SET);
					
					if(err){
						printf("Erro de leitura!5\n");
						fclose(arq);
						return;
					
					}else{
						int err = fwrite(aux,BLOCK_SIZE,1, arq);
						fclose(arq);

						if(err == 0){
							printf("Erro de escrita!4\n");
						}
					}
				}
			}

		}else{
			add_key_bucket(aux, aKey, aValue);
		}
	}

	else if(n->pos < TAM-1){ //caso o bucket esteja cheio, cria um novo com encadeamento
		n->key[n->pos] = aKey;
		n->value[n->pos]= aValue;
		n->pos++;
		FILE* arq = fopen("HashIndex.txt","r+");
		int err = fseek(arq, n->number*BLOCK_SIZE, SEEK_SET);
		if(err){
			printf("Erro de leitura!3\n");
			fclose(arq);
			return; 
		}else{
			err = fwrite(n, BLOCK_SIZE, 1,arq);
			fclose(arq);
			if(!err){
				printf("Erro de leitura!2\n");
			}
		}	
	}
}

pair<int, int> get_value(bucket_t* n, int aKey){
	bucket_t* aux = n;
	int read_blocks = 0;
	while(aux != NULL){ //procurando em uma lista de buckets
		for(int i = 0; i < aux->pos; i++){
			if(aKey == aux->key[i]) return pair<int, int> (aux->value[i],read_blocks);
		}
		aux = read_bucket(aux->next);
		read_blocks++;
	}
	return pair<int,int> (-1,read_blocks);
}

void clear_buckets(){
    FILE* arq = fopen("HashIndex.txt","w");
    fclose(arq);
}
int create_bucket(){
    FILE* arq = fopen("HashIndex.txt","r+");
    int err = fseek(arq, 0, SEEK_END);
    int next_block = ftell(arq)/BLOCK_SIZE;
    bucket_t* n = new_bucket(next_block);
    err = fwrite(n,BLOCK_SIZE,1,arq);
    fclose(arq);
    free(n);
    return next_block;
}