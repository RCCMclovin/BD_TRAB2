#ifndef NODE_HPP
#define NODE_HPP

#define BLOCK_SIZE 4096
#define M 255

typedef struct node{
	int n;
	bool is_leaf;
	int keys[2*M];
	int pointers[2*M+1]; // numero do bloco no arquivo de indice (pointers[i] * BOCK_SIZE dá o endereço físico)
  int my_pointer; // numero do bloco no arquivo de indice do nó corrente. Necessário na atualização de um nó na árvore, e durante a inserção de uma entrada, essa informação pode ser requisitada
}node_t;

node_t* create_node(bool is_leaf, int my_pointer);

#endif