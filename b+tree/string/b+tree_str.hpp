#ifndef B_PLUS_TREE_STR_HPP
#define B_PLUS_TREE_STR_HPP
#include <string>
#include <stdio.h>
#include "node.hpp"
#include "entry.hpp"
#include <iostream>

using namespace std;

typedef struct b_plus_tree_str{
	int next_block;
  int root_pointer;
  string index_file_name;
	FILE* index_file;
	node_str_t* root;
}b_plus_tree_str_t;

b_plus_tree_str_t* create_b_plus_tree_str(string index_file);
bool dump_b_plus_tree_str(b_plus_tree_str_t* tree, string dump_file_name);
b_plus_tree_str_t* load_b_plus_tree_str(string dump_file_name, string index_file_name);
void insert_entry(b_plus_tree_str_t* tree, entry_str_t entry);
int search_key(b_plus_tree_str_t* tree, string key);
void show_summary_of_tree_in_order(b_plus_tree_str_t* tree);

#endif