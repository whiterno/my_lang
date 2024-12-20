.PHONY: syntax_tree

Headers := include/syntax_tree.h include/syntax_tree_dump.h
Objects := obj/syntax_tree.o obj/syntax_tree_dump.o
Flags	:=

syntax_tree: ${Objects} ${Headers}

obj/syntax_tree.o: src/syntax_tree.cpp ${Headers}
	g++ ${Flags} -c src/syntax_tree.cpp -o obj/syntax_tree.o

obj/syntax_tree_dump.o: src/syntax_tree_dump.cpp ${Headers}
	g++ ${Flags} -c src/syntax_tree_dump.cpp -o obj/syntax_tree_dump.o

