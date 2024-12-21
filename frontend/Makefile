.PHONY: frontend frontend_run

Headers := include/tokens.h include/lexer.h include/code_gen.h include/parsing.h include/nametable.h
Objects := obj/main.o obj/lexer.o obj/tokens.o obj/parsing.o obj/nametable.o ../syntax_tree/obj/syntax_tree.o ../syntax_tree/obj/syntax_tree_dump.o ../utils/obj/utils.o
Flags 	:= -I ../syntax_tree/include -I ../utils/include -I include
CC 		:= -std=c++14

frontend.out: ${Headers} ${Objects}
	g++ ${CC} ${Flags} ${Objects} -o frontend.out

obj/main.o: src/main.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/main.cpp -o obj/main.o

obj/lexer.o: src/lexer.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/lexer.cpp -o obj/lexer.o

obj/tokens.o: src/tokens.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/tokens.cpp -o obj/tokens.o

obj/parsing.o: src/parsing.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/parsing.cpp -o obj/parsing.o

obj/nametable.o: src/nametable.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/nametable.cpp -o obj/nametable.o

frontend: frontend.out

frontend_run: frontend.out
	./frontend.out
