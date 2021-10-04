#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <string>

#include "entry.hpp"
#include "b+tree.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	b_plus_tree_str_t *tree = create_b_plus_tree_str("index_file");
	printf("começando a inserir...\n");
  
  for(int i = 0; i < 50; i++){
    entry_str_t entry = {to_string(i), i};
    insert_entry(tree, entry);
	}
  printf("terminou inserção...\n");
  show_summary_of_tree_in_order(tree);
  
  for(int i = 0; i < 50; i++){
    int pointer = search_key(tree, to_string(i));
    if(pointer == -1){
      printf("chave %d não encontrada\n", i);
    }
	}
  /*
  srand(time(NULL));
  unordered_map<int, int> random_keys;
	for(int i = 0; i < 100000; i++){
    int key = rand() % 1000000;
    if(random_keys.find(key) == random_keys.end()){
      entry_str_t entry = {key, key};
      insert_entry(tree, entry);
      random_keys[key] = 1;
    }
	}
  // show_summary_of_tree_in_order(*tree);
  printf("número de chaves únicas aleatórias inseridas: %lu\n", random_keys.size());
	printf("começando a buscar...\n");
  for(auto it : random_keys){
    int pointer = search_key(tree, it.first);
    if(pointer == -1){
      printf("chave %d não encontrada\n", it.first);
    }
  }
  */
  bool is_tree_dumped =  dump_b_plus_tree(tree, "arvore_guardada_aqui");
  if(!is_tree_dumped)
    printf("Erro ao salvar a árvore\n");

	return 0;
}