CXX := g++
node := src/node/node.h src/node/node.cpp
visitor := src/visitor/visitor.h src/visitor/visitor.cpp
mixed := src/mixed/mixed.h src/mixed/mixed.cpp
error := src/error/error.h src/error/error.cpp
library := cpm_compiler/libmixed.a cpm_compiler/mixed.h
example := examples/a.cpm
CXXFLAGS+=-std=c++17 -Wall -O2

.PHONY: all clean run pack rerun

all: cpm_compiler cpm_compiler/c+- $(library)

run: cpm_compiler out/a.out
	out/a.out

rerun: cpm_compiler cpm_compiler/c+- $(library)
	rm -fr out
	cpm_compiler/c+- $(example)
	out/a.out

out/a.out: $(example) cpm_compiler/c+- $(library)
	cpm_compiler/c+- $(example)

$(library): $(mixed)
	cp src/mixed/mixed.h cpm_compiler/mixed.h
	$(CXX) src/mixed/mixed.cpp $(CXXFLAGS) -c -o cpm_compiler/mixed.o
	ar r cpm_compiler/libmixed.a cpm_compiler/mixed.o
	ranlib cpm_compiler/libmixed.a
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

pack: cpm_compiler/c+- $(library)
	zip cpm_compiler.zip -r cpm_compiler

clean:
	rm -rf out
	rm -rf cpm_compiler
	rm -f src/parser/*.c src/parser/*.h src/parser/*.output