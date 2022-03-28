CXX := g++
node := src/node/node.h src/node/node.cpp
mixed := src/mixed/mixed.h src/mixed/mixed.cpp
library := cpm_compiler/libmixed.so cpm_compiler/mixed.h
example := examples/a.cpm
CXXFLAGS+=-std=c++17 -Wall -O2

.PHONY: all clean run pack

all: cpm_compiler cpm_compiler/c+- $(library)

run: cpm_compiler out/a.out
	out/a.out

out/a.out: $(example) cpm_compiler/c+- $(library)
	cpm_compiler/c+- $(example)

$(library): $(mixed)
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

cpm_compiler:
	mkdir -p cpm_compiler

pack: all
	zip cpm_compiler.zip -r Makefile cpm_compiler

clean:
	rm -rf cpm_compiler
	rm -rf out
	rm -f src/parser/*.c src/parser/*.h src/parser/*.output