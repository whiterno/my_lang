.PHONY: frontend frontend_run

Headers := include/tokens.h include/lexer.h
Objects := obj/main.o obj/lexer.o obj/tokens.o ../syntax_tree/obj/syntax_tree.o ../syntax_tree/obj/syntax_tree_dump.o ../utils/obj/utils.o
Flags 	:= -I ../syntax_tree/include -I ../utils/include -I include

frontend.out: ${Headers} ${Objects}
	g++ ${Flags} ${Objects} -o frontend.out

obj/main.o: src/main.cpp ${Headers}
	g++ ${Flags} -c src/main.cpp -o obj/main.o

obj/lexer.o: src/lexer.cpp ${Headers}
	g++ ${Flags} -c src/lexer.cpp -o obj/lexer.o

obj/tokens.o: src/tokens.cpp ${Headers}
	g++ ${Flags} -c src/tokens.cpp -o obj/tokens.o

frontend: frontend.out

frontend_run: frontend.out
	./frontend.out
