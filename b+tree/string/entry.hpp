#ifndef ENTRY_STR_HPP
#define ENTRY_STR_HPP

// Entrada utilizada na inserção
typedef struct entry_str{
	std::string key; // chave a ser inserida
	int pointer; // apontador do bloco que contém 'key' no arquivo de dados
}entry_str_t;

bool is_entry_null(entry_str_t entry);

#endif