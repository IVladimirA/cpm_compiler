CXX := g++
CXXFLAGS+=-std=c++17 -Wall -O2

all: out out/transpiler.o

run: out out/transpiler.o
	./out/transpiler.o

out/transpiler.o: source/*.cpp source/*.h source/parser/node.lexer.c source/parser/node.grammar.c
	$(CXX) $^ $(CXXFLAGS) -w -o out/transpiler.o

source/parser/%.lexer.c source/parser/%.lexer.h: source/parser/%.lex
	flex source/parser/$*.lex
	mv lex.c source/parser/$*.lexer.c
	mv lex.h source/parser/$*.lexer.h

source/parser/%.grammar.c source/parser/%.grammar.h: source/parser/%.y
	bison -d -v $^ -o source/parser/$*.grammar.c

out:
	mkdir -p out

pack:
	zip transpiler.zip -r Makefile source

clean:
	rm -rf out
	rm -f source/parser/*.c source/parser/*.h source/parser/*.output