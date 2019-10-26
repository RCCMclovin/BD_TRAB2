#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <string>

#include "node.hpp"
#include "entry.hpp"
#include "b+tree.hpp"

using namespace std;

// Autor: Estevão
static node_t* add_node_in_tree(b_plus_tree_t* tree, node_t* new_node){
	// escreve 'new_node' em 'tree.index_file'
	int err = fseek(tree->index_file, tree->next_block*BLOCK_SIZE, SEEK_SET);
	if(err){
    printf("[add_node_in_tree()] Erro de seek\n");
		return NULL;
	}
	size_t error = fwrite(new_node, BLOCK_SIZE, 1, tree->index_file);
	if(error != 1){
    printf("[add_node_in_tree()] Erro de write\n");
		return NULL;
	}
	tree->next_block++;
	return new_node;
}

//Autor: Estevão
static node_t* create_node_in_tree(b_plus_tree_t* tree, bool is_leaf){
	// aloca 'new_node' e adiciona em 'tree'
	node_t* new_node = create_node(is_leaf, tree->next_block);
	return add_node_in_tree(tree, new_node);
}

//Autor: Estevão
b_plus_tree_t* create_b_plus_tree(string index_file_name){
	// cria b+tree em 'index_file'
	b_plus_tree_t* tree = new b_plus_tree_t;
  tree->index_file_name = index_file_name;
	tree->index_file = fopen(index_file_name.c_str(), "w");
  if(tree->index_file == NULL){
    printf("[create_b_plus_tree()] Não foi possível abrir arquivo com permissão 'w'\n");
    return NULL;
  }
	tree->next_block = 0;
	tree->root = create_node_in_tree(tree, true);
  tree->root_pointer = 0;
  fclose(tree->index_file);
	return tree;
}


typedef struct dump_data{
  /* Estrutura auxiliar para salvar informações relevantes de uma b+tree num arquivo com a função 'dump_b_plus_tree',
    e carregar posteriormente usando a função 'load_b_plus_tree_helper'.
  */
  int next_block, root_pointer;
}dump_data_t;

//Autor: Estevão
bool dump_b_plus_tree(b_plus_tree_t* tree, string dump_file_name){
  /* salva no arquivo 'dump_file_name' as informações relevantes da b+tree 'tree'.
    Retorna 'true' somente se houver sucesso na escrita da árvore no disco.
  */
  FILE* dump_file = fopen(dump_file_name.c_str(), "w");
  if(dump_file == NULL){
    printf("[dump_b_plus_tree()] Não foi possível abrir o arquivo com permissão 'w'\n");
    return false;
  }
  int err = fseek(tree->index_file, 0, SEEK_SET);
	if(err){
		printf("[dump_b_plus_tree()] Erro de seek.\n");
    fclose(dump_file);
		return false;
	}
  dump_data_t data = {tree->next_block, tree->root_pointer};
  size_t error = fwrite(&data, sizeof(dump_data_t), 1, dump_file);
  if(error != 1){
    printf("[dump_b_plus_tree()] Erro de escrita.\n");
    fclose(dump_file);
    return false;
  }
  fclose(dump_file);
  return true;
}

//Autor: Estevão
static b_plus_tree_t* load_b_plus_tree_helper(dump_data_t* data, string index_file_name){
  /* Carrega o nó raiz (que está na posição 'data->root_pointer' no 'index_file_name')
    e ajusta a b+tree de acordo com os valores de 'data'  
  */
  FILE* index_file = fopen(index_file_name.c_str(), "r");
  if(index_file == NULL){
    printf("[load_b_plus_tree_helper()] Não foi possível abrir o arquivo com permissão 'r'\n");
    return NULL;
  }
  int err = fseek(index_file, data->root_pointer*BLOCK_SIZE, SEEK_SET);
  if(err){
    printf("[load_b_plus_tree_helper()] Erro de seek.\n");
    fclose(index_file);
    return NULL;
  }
  node_t* root = new node_t;
  size_t error = fread(root, BLOCK_SIZE, 1, index_file);
  if(error != 1){
    printf("[load_b_plus_tree_helper()] Erro no fread().\n");
    fclose(index_file);
    return NULL;
  }
  b_plus_tree_t* tree = new b_plus_tree_t;
  tree->next_block = data->next_block;
  tree->root_pointer = data->root_pointer;
  tree->root = root;
  tree->index_file_name = index_file_name;
  fclose(index_file);
  return tree;
}

//Autor: Estevão
b_plus_tree_t* load_b_plus_tree(string dump_file_name, string index_file_name){
  // carrega para a memória e retorna a b+tree que foi salva em 'dump_file_name'
  FILE* dump_file = fopen(dump_file_name.c_str(), "r");
  if(dump_file == NULL){
    printf("[dump_b_plus_tree()] Não foi possível abrir o arquivo com permissão 'r'\n");
    return NULL;
  }
  int err = fseek(dump_file, 0, SEEK_SET);
  if(err){
    printf("[load_b_plus_tree()] Erro de seek.\n");
    fclose(dump_file);
    return NULL;
  }
  dump_data_t* data = new dump_data_t;
  size_t error = fread(data, sizeof(dump_data_t), 1, dump_file);
  if(error != 1){
    fclose(dump_file); 
		printf("[load_b_plus_tree()] Erro de escrita.\n");
    return NULL;
  }
  b_plus_tree_t* tree = load_b_plus_tree_helper(data, index_file_name);
  fclose(dump_file);
  return tree;
}

//Autor: Estevão
static void set_tree_root(b_plus_tree_t* tree, node_t* node){
	// seta o nó raiz de 'tree' para 'node'
  tree->root_pointer = node->my_pointer;
	tree->root = node;
}

//Autor: Estevão
static node_t* read_node(b_plus_tree_t tree, int pointer){
	// lê o que está no bloco 'pointer * BLOCK_SIZE' em 'tree.index_file'
  int err = fseek(tree.index_file, pointer*BLOCK_SIZE, SEEK_SET);
  if(err){
    printf("[read_node()] Erro de seek.\n");
    return NULL;
  }
  node_t* new_node = new node_t;
  size_t error = fread(new_node, BLOCK_SIZE, 1, tree.index_file);
  if(error != 1){
    printf("[read_node()] Erro no fread().\n");
    return NULL;
  }
  return new_node;
}

// Autor: Estevão
void show_summary_of_tree_in_order(b_plus_tree_t* tree){
  tree->index_file = fopen(tree->index_file_name.c_str(), "r");
  if(tree->index_file == NULL){
    printf("[show_summary_of_tree_in_order()] Não foi possível abrir o arquivo com permissão 'r'\n");
    return ;
  }
  /* mostra um resumo de 'tree' em ordem.
    A cada nível de profundidade, mostra a 1º e a última chave dos nós.
  */
  queue<pair<node_t, int> > Q;
  Q.push(make_pair(*(tree->root), 0));
  int depth = 0;
  printf("profundidade 0:\n");
  while(!Q.empty()){
    pair<node_t, int> current = Q.front(); 
    Q.pop();
    if(depth < current.second){
      depth++;
      printf("profundidade: %d\n", depth);
    }
    printf("(%d..%d)\n", current.first.keys[0], current.first.keys[current.first.n - 1]);
    if(!current.first.is_leaf){
      for(int i = 0; i <= current.first.n; i++){
        node_t* child = read_node(*tree, current.first.pointers[i]);
        Q.push(make_pair(*child, current.second + 1));
      }
    }
  }
  fclose(tree->index_file);
}

//Autor: Rafael
static void update_node_in_tree(b_plus_tree_t* tree, node_t* node){
	int pointer = node->my_pointer;
	int err = fseek(tree->index_file, pointer*BLOCK_SIZE, SEEK_SET);
	if(err){
		printf("[update_node_in_tree()] Erro de seek.\n");
		return ;
	}
  if(sizeof(*node) > BLOCK_SIZE)
    printf("[update_node_in_tree()] tamanho do nó maior que BLOCK_SIZE: %lu\n", sizeof(*node));
	size_t error = fwrite(node, BLOCK_SIZE, 1, tree->index_file);
	if(error != 1){
		printf("[update_node_in_tree()] Erro de escrita.\n");
	}
}

//Autor: Rafael
static int binary_search(int* array, int n, int key){
	//retorna a posição de 'key' em 'array'
	int e = -1, d = n;
   	while (e < d-1) { 
      int m = (e + d)/2;
      if (array[m] < key) e = m;
      else {
      	if(array[m] == key) return m;
      	d = m;
      }
   }
   return d - 1;// se o elemento não for encontrado, então a posição onde ele possivelmente será encontrado
}

//Autor: Estevão
static void insert_in_node(node_t* node, entry_t entry){
	/*
	insere a entrada 'entry' no nó 'node'. 
	O afastamento das chaves e dos ponteiros existentes em 'node' 
	é necessário para manter a ordem das chaves
	*/
	int i = node->n;
	while(i > 0 && node->keys[i - 1] > entry.key){
		node->keys[i] = node->keys[i - 1];
		node->pointers[i + 1] = node->pointers[i];
		node->pointers[i] = node->pointers[i-1];
		i--;
	}
	node->keys[i] = entry.key;
  if(node->is_leaf)
	  node->pointers[i] = entry.pointer;
  else
    node->pointers[i+1] = entry.pointer;
    
	node->n++;
}

//Autor: Estevão
static entry_t split_leaf(b_plus_tree_t* tree, node_t* left, entry_t entry){
	// realiza split sobre o nó folha 'left'.
	node_t* right = create_node_in_tree(tree, true);
	for(int i = 0; i < M; i++){
		right->keys[i] = left->keys[i + M];
		right->pointers[i] = left->pointers[i + M];
	}
	right->n = M;
	left->n = M;
	// insere a entrada 'entry' no nó correto
	if(entry.key < left->keys[left->n - 1]){
		insert_in_node(left, entry);
	}else{
		insert_in_node(right, entry);
	}
  /*
	como acabamos de criar o novo nó 'right', 
	'tree.next_block - 1' é o apontador de 'right' no arquivo de índice
  */
  left->pointers[left->n] = tree->next_block - 1;
	entry_t middle = {right->keys[0], tree->next_block - 1};
	update_node_in_tree(tree, left);
	update_node_in_tree(tree, right);
	return middle;
}

//Autor: Estevão
static entry_t split_non_leaf(b_plus_tree_t* tree, node_t* node, entry_t new_child_entry){
	//	realiza split do nó não folha 'node'.
	entry_t middle = {0,0};
	node_t* new_node = create_node_in_tree(tree, false);
	int lower_bound;
	bool is_to_insert_new_child_in_new_node = false;
	bool is_to_insert_new_child_in_node = false;
	// Seja M = 3 e
	// node->keys = [5, 8, 10, 18, 25, 36]
	//               0  1   2   3   4   5
	if(node->keys[M-1] <= new_child_entry.key && new_child_entry.key < node->keys[M]){
		// e.g.: new_child_entry.key = 15
		middle.key = new_child_entry.key;
		lower_bound = M;
	}else if(new_child_entry.key >= node->keys[M]){
		// e.g.: new_child_entry.key = 20
		middle.key = node->keys[M];
		lower_bound = M + 1;
		is_to_insert_new_child_in_new_node = true;
	}else{
		// e.g.: new_child_entry.key = 1
		middle.key = node->keys[M - 1];
		lower_bound = M;
		is_to_insert_new_child_in_node = true;
	}
  int i;
	for(i = lower_bound; i < (2*M); i++){
		new_node->keys[i - lower_bound] = node->keys[i];
		new_node->pointers[i - lower_bound] = node->pointers[i];
		new_node->n++;
		node->n--;
	}
  new_node->pointers[i - lower_bound] = node->pointers[i];
	if(is_to_insert_new_child_in_new_node){
    node->n--;
		insert_in_node(new_node, new_child_entry);
	}else if(is_to_insert_new_child_in_node){
    node->n--;
		insert_in_node(node, new_child_entry);
	}
	middle.pointer = tree->next_block - 1;
	update_node_in_tree(tree, node);
	update_node_in_tree(tree, new_node);
	return middle;
}

//Autor: Estevão
static entry_t insert_inner(b_plus_tree_t* tree, node_t* root, entry_t entry){
	// função interna utilizada pela wrapper 'insert(b_plus_tree_t tree, entry_t entry)' para inserir 'entry' na b+tree 'tree'
	if(root->is_leaf){
		if(root->n < 2*M){
      // printf("root COM ESPACO, entry.key = %d\n", entry.key);
			insert_in_node(root, entry);
			update_node_in_tree(tree, root);
			return {-1, -1}; // {-1, -1} means null entry
		}else{
      // printf("root ESTA CHEIO, entry.key = %d\n", entry.key);
			entry_t middle = split_leaf(tree, root, entry);
			return middle;
		}
	}else{
		int i;
		if(entry.key < root->keys[0]){
			i = 0;
		}else{
			for(i = 1; i < root->n; i++)
				if(root->keys[i-1] <= entry.key && entry.key < root->keys[i])
					break;
		}
		node_t* child = read_node(*tree, root->pointers[i]);
		entry_t new_child_entry = insert_inner(tree, child, entry);

		if(is_entry_null(new_child_entry))
			return {-1, -1};
		if(root->n < 2*M){
			insert_in_node(root, new_child_entry);
			update_node_in_tree(tree, root);
			return {-1, -1};
		}else{
			entry_t middle = split_non_leaf(tree, root, new_child_entry);
			if(tree->root == root){
				node_t* new_tree_root = create_node_in_tree(tree, false);
				insert_in_node(new_tree_root, middle);
        new_tree_root->pointers[0] = root->my_pointer;
				set_tree_root(tree, new_tree_root);
        update_node_in_tree(tree, new_tree_root);
				return {-1, -1};
			}
			return middle;
		}
	}
}

//Autor: Estevão
void insert_entry(b_plus_tree_t* tree, entry_t entry){
	/*esta é a principal função de inserção. 
	Esta é a função que deve ser chamada na função 'main' para realizar inserção de uma entrada 'entry' em 'tree'
	*/
  tree->index_file = fopen(tree->index_file_name.c_str(), "r+");
  if(tree->index_file == NULL){
    printf("[insert()] Não foi possível abrir o arquivo com permissão 'r+'\n");
    return ;
  }
	entry_t middle = insert_inner(tree, tree->root, entry);
  if(!is_entry_null(middle)){
    // nó raiz sofreu split, sendo que ele era o único nó em 'tree' antes do split
    node_t* new_root = create_node_in_tree(tree, false);
    insert_in_node(new_root, middle);
    new_root->pointers[0] = tree->root->my_pointer;
    set_tree_root(tree, new_root);
    update_node_in_tree(tree, new_root);
  }
  fclose(tree->index_file);
}

//Autor: Rafael
int search_key(b_plus_tree_t* tree, int key){
  tree->index_file = fopen(tree->index_file_name.c_str(), "r");
  if(tree->index_file == NULL){
    printf("[search_key()] Não foi possível abrir o arquivo com permissão 'r'\n");
    return -1;
  }
	node_t* aux = tree->root;
	int i = -1;
  int count_blocks_read = 1; // o nó raiz já foi lido previamente
	while(aux->is_leaf == false){
		i = binary_search(aux->keys, aux->n, key);
		if(key < aux->keys[i]){
			aux = read_node(*tree,aux->pointers[i]);
		}else{
			aux = read_node(*tree,aux->pointers[i+1]);
		}
    count_blocks_read++;
	}
  i = binary_search(aux->keys, aux->n, key);
  fclose(tree->index_file);
	if(key == aux->keys[i]){
    printf("Quantidade de blocos lidos: %d\n", count_blocks_read);
    return aux->pointers[i];
  }else{
    return -1;
  } 
}
