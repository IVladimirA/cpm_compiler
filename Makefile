CXX := g++
node := src/node/node.h src/node/node.cpp
mixed := src/mixed/mixed.h src/mixed/mixed.cpp
utils := src/utils/utils.h src/utils/utils.cpp
classes := $(node) $(mixed) $(utils)
CXXFLAGS+=-std=c++17 -Wall -O2

all: out out/transpiler.o

run: out out/transpiler.o
	./out/transpiler.o

out/transpiler.o: src/main.cpp $(classes) src/parser/node.lexer.c src/parser/node.grammar.c
	$(CXX) $^ $(CXXFLAGS) -w -o out/transpiler.o

src/parser/%.lexer.c src/parser/%.lexer.h: src/parser/%.lex
	flex src/parser/$*.lex
	mv lex.c src/parser/$*.lexer.c
	mv lex.h src/parser/$*.lexer.h

src/parser/%.grammar.c src/parser/%.grammar.h: src/parser/%.y
	bison -d -v $^ -o src/parser/$*.grammar.c

out:
	mkdir -p out

pack:
	zip transpiler.zip -r Makefile src

clean:
	rm -rf out
	rm -f src/parser/*.c src/parser/*.h src/parser/*.output