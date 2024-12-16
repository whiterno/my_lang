Headers := include/tokens.h
Objects := obj/lexer.o obj/tokens.cpp
Flags 	:= -I ../syntax_tree/include

frontend.out: ${Headers} ${Objects}
	g++ ${Flags} ${Objects} -o frontend.out

obl/lexer.o: src/lexer.cpp ${Headers}
	g++ ${Flags} -c src/lexer.cpp -o obj/lexer.o

obl/tokens.o: src/tokens.cpp ${Headers}
	g++ ${Flags} -c src/tokens.cpp -o obj/tokens.o
