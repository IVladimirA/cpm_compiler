CXX := g++
node := src/node/node.h src/node/node.cpp
mixed := src/mixed/mixed.h src/mixed/mixed.cpp
CXXFLAGS+=-std=c++17 -Wall -O2

all: cpm_compiler cpm_compiler/c+- cpm_compiler/libmixed.so

run: cpm_compiler/out cpm_compiler/out/a.o
	cpm_compiler/out/a.o

cpm_compiler/out/a.o cpm_compiler/out/a.cpp: ./examples/a.cpm cpm_compiler/mixed.h cpm_compiler/libmixed.so cpm_compiler/c+-
	cpm_compiler/c+- examples/a.cpm cpm_compiler/out/a.o cpm_compiler/libmixed.so cpm_compiler/out/a.cpp

cpm_compiler/libmixed.so cpm_compiler/mixed.h: $(mixed)
	cp src/mixed/mixed.h cpm_compiler/mixed.h
	$(CXX) src/mixed/mixed.cpp $(CXXFLAGS) -fPIC -c -o cpm_compiler/mixed.o
	$(CXX) cpm_compiler/mixed.o -fPIC -shared -o cpm_compiler/libmixed.so
	rm -f cpm_compiler/mixed.o

cpm_compiler/c+-: src/main.cpp $(node) src/parser/node.lexer.c src/parser/node.tab.c
	$(CXX) $^ $(CXXFLAGS) -w -o $@

src/parser/%.lexer.c src/parser/%.lexer.h: src/parser/%.lex
	flex src/parser/$*.lex
	mv lex.c src/parser/$*.lexer.c
	mv lex.h src/parser/$*.lexer.h

src/parser/%.tab.c src/parser/%.tab.h: src/parser/%.y
	bison -d -v $^ -o src/parser/$*.tab.c

cpm_compiler/out: cpm_compiler
	mkdir -p cpm_compiler/out

cpm_compiler:
	mkdir -p cpm_compiler

pack: src/parser/node.lexer.c src/parser/node.tab.h
	zip cpm_compiler.zip -r Makefile src examples

clean:
	rm -rf cpm_compiler
	rm -f src/parser/*.c src/parser/*.h src/parser/*.output