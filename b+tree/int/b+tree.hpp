#ifndef B_PLUS_TREE_HPP
#define B_PLUS_TREE_HPP

#include "node.hpp"
#include "entry.hpp"

using namespace std;

typedef struct b_plus_tree{
	int next_block;
  int root_pointer;
  string index_file_name;
	FILE* index_file;
	node_t* root;
}b_plus_tree_t;

b_plus_tree_t* create_b_plus_tree(string index_file);
bool dump_b_plus_tree(b_plus_tree_t* tree, string dump_file_name);
b_plus_tree_t* load_b_plus_tree(string dump_file_name, string index_file_name);
void insert_entry(b_plus_tree_t* tree, entry_t entry);
int search_key(b_plus_tree_t* tree, int key);
void show_summary_of_tree_in_order(b_plus_tree_t* tree);

#endif