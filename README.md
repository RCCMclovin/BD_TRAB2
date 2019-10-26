# BD_TRAB2

compilação

g++ -c b+tree/int/node.cpp

g++ -c b+tree/string/node_str.cpp

g++ -c b+tree/string/entry_str.cpp

g++ -c b+tree/string/b+tree_str.cpp

g++ -c b+tree/int/entry.cpp

g++ -c b+tree/int/b+tree.cpp

g++ -c hash/bucket.cpp

g++ -c hash/hash_table.cpp

g++ findrec.cpp bucket.o hash_table.o -o findrec

g++ seek1.cpp b+tree.o entry.o node.o -o seek1

g++ seek2.cpp b+tree_str.o entry_str.o node_str.o -o seek2

g++ upload.cpp b+tree.o entry.o node.o b+tree_str.o node_str.o entry_str.o bucket.o hash_table.o -o upload
