#ifndef NODE_STR_HPP
#define NODE_STR_HPP

#define BLOCK_SIZE 4096
#define M_STR 6

typedef struct alfa_300{
  char str[301];
}alfa_300_t;

typedef struct node_str{
	int n;
	bool is_leaf;
	alfa_300_t keys[2*M_STR];
	int pointers[2*M_STR+1]; // numero do bloco no arquivo de indice (pointers[i] * BOCK_SIZE dá o endereço físico)
  int my_pointer; // numero do bloco no arquivo de indice do nó corrente. Necessário na atualização de um nó na árvore, e durante a inserção de uma entrada, essa informação pode ser requisitada
}node_str_t;

node_str_t* create_node_str(bool is_leaf, int my_pointer);

#endif