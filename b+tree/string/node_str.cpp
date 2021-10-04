#include "node.hpp"

//Autor: Estevão
node_str_t* create_node_str(bool is_leaf, int my_pointer){
	// aloca e ajusta um novo nó
	node_str_t* new_node = new node_str_t;
	new_node->is_leaf = is_leaf;
	new_node->n = 0;
  new_node->my_pointer = my_pointer;
	return new_node;
}