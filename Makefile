CXX := g++
node := src/node/node.h src/node/node.cpp
mixed := src/mixed/mixed.h src/mixed/mixed.cpp
utils := src/utils/utils.h src/utils/utils.cpp
all_classes := $(node) $(mixed) $(utils)
CXXFLAGS+=-std=c++17 -Wall -O2

all: out/a.o

run: out/a.o
	./out/a.o

out/a.cpp: out out/transpiler.o
	./out/transpiler.o $(ARGS)

out/a.o: ./out/a.cpp $(mixed) $(utils)
	$(CXX) $^ $(CXXFLAGS) -w -o out/a.o

out/transpiler.o: src/main.cpp $(all_classes) src/parser/node.lexer.c src/parser/node.grammar.c
	$(CXX) $^ $(CXXFLAGS) -w -o out/transpiler.o

src/parser/%.lexer.c src/parser/%.lexer.h: src/parser/%.lex
	flex src/parser/$*.lex
	mv lex.c src/parser/$*.lexer.c
	mv lex.h src/parser/$*.lexer.h

src/parser/%.grammar.c src/parser/%.grammar.h: src/parser/%.y
	bison -d -v $^ -o src/parser/$*.grammar.c

out:
	mkdir -p out

pack: src/parser/%.lexer.c src/parser/%.grammar.h
	zip cpm_compiler.zip -r Makefile src

clean:
	rm -rf out
	rm -f src/parser/*.c src/parser/*.h src/parser/*.output