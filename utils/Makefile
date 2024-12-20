.PHONY: utils

Headers := include/utils.h
Objects := obj/utils.o
Flags	:= -I include -I ../syntax_tree/include

utils: ${Objects} ${Headers}

obj/utils.o: src/utils.cpp ${Headers}
	g++ ${Flags} -c src/utils.cpp -o obj/utils.o
