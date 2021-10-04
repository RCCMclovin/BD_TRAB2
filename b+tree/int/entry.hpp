#ifndef ENTRY_HPP
#define ENTRY_HPP

// Entrada utilizada na inserção
typedef struct entry{
	int key; // chave a ser inserida
	int pointer; // apontador do bloco que contém 'key' no arquivo de dados
}entry_t;

bool is_entry_null(entry_t entry);

#endif