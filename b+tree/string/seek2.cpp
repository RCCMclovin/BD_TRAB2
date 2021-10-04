#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "b+tree.hpp"

using namespace std;

int main(int argc, char const *argv[]){
  if(argc != 2){
    printf("Erro: falta informar o valor da chave a ser buscada\n");
    return -1;
  }
  string key = string(argv[1]);
  printf("chave a ser buscada: %s\n", key.c_str());
  b_plus_tree_str_t* tree = load_b_plus_tree_str("arvore_guardada_aqui", "index_file");
  if(tree == NULL){
    printf("Erro: não foi possível carregar a árvore\n");
  }
  int pointer = search_key(tree, key);
  if(pointer == -1){
    printf("A chave não foi encontrada\n");
  }else{
    printf("A chave está %dº bloco no arquivo de dados\n", pointer);
  }

  return 0;
}
