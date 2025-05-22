.PHONY: backend backend_run

Headers := include/read_tree.h include/asm_generate.h include/llvm_ir.h include/defines.h include/print_llvm_ir.h
Objects := obj/main.o obj/print_llvm_ir.o obj/read_tree.o obj/asm_generate.o obj/llvm_ir.o ../syntax_tree/obj/syntax_tree.o ../syntax_tree/obj/syntax_tree_dump.o ../utils/obj/utils.o
Flags   := -I ../syntax_tree/include -I ../utils/include -I include
CC 		:= -std=c++14

backend.out: ${Headers} ${Objects}
	g++ ${CC} ${Flags} ${Objects} -o backend.out

obj/main.o: src/main.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/main.cpp -o obj/main.o

obj/read_tree.o: src/read_tree.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/read_tree.cpp -o obj/read_tree.o

obj/asm_generate.o: src/asm_generate.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/asm_generate.cpp -o obj/asm_generate.o

obj/llvm_ir.o: src/llvm_ir.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/llvm_ir.cpp -o obj/llvm_ir.o

obj/print_llvm_ir.o: src/print_llvm_ir.cpp ${Headers}
	g++ ${CC} ${Flags} -c src/print_llvm_ir.cpp -o obj/print_llvm_ir.o

backend: backend.out

backend_run: backend.out
	./backend.out
