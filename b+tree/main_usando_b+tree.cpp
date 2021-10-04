#include <cstdio>
#include <string>

#include "int/b+tree.hpp"
#include "string/b+tree.hpp"
/* 
COMO COMPILAR ESTE PROGRAMA:
g++ string/b+tree.cpp string/entry.cpp string/node.cpp  int/b+tree.cpp int/entry.cpp int/node.cpp  main_usando_b+tree.cpp -o 
main

Note que se você quiser usar as árvores em um programa localizado em outro diretório, 
o caminho para os arquivos .cpp compilados acima nos diretórios 'int' e 'string' será diferente.

Por exemplo, seja o programa 'main_exemplo.cpp' no diretório 'BD-TP2-2019.2'. 
Então os arquivos acima dos diretórios 'int' e 'string' devem ser prefixados com 'b+tree/', 
ou seja, 'g++ string/b+tree.cpp string/entry.cpp ...' se torna ''g++ b+tree/string/b+tree.cpp b+tree/string/entry.cpp ...'
*/
int main(int argc, char const *argv[]){
  b_plus_tree_str_t* tree_str = create_b_plus_tree_str("index_file_string");
  b_plus_tree_t* tree_int = create_b_plus_tree("index_file_int");
  insert_entry(tree_str, {to_string(100), 100});
  insert_entry(tree_int, {100, 100});
  dump_b_plus_tree(tree_str, "dump_file_string");
  dump_b_plus_tree(tree_int, "dump_file_int");
  return 0;
}