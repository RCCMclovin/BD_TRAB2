#include <string>

#include "entry.hpp"

//Autor: Estevão
bool is_entry_null(entry_str_t entry){
	// retorna 'true' se, e somente se, 'entry' é nulo
	return entry.key == "-1" && entry.pointer == -1;
}