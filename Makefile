CXX := g++
node := src/node/node.h src/node/node.cpp
mixed := src/mixed/mixed.h src/mixed/mixed.cpp
all_classes := $(node) $(mixed)
CXXFLAGS+=-std=c++17 -Wall -O2

all: out out/compiler.o out/libmixed.so

run: out out/a.o
	./out/a.o

out/a.o out/a.cpp: ./examples/a.cpm out/mixed.h out/libmixed.so out/compiler.o
	./out/compiler.o

out/libmixed.so out/mixed.h: $(mixed)
	cp src/mixed/mixed.h out/mixed.h
	$(CXX) src/mixed/mixed.cpp $(CXXFLAGS) -fPIC -c -o out/mixed.o
	$(CXX) out/mixed.o -fPIC -shared -o out/libmixed.so

out/compiler.o: src/main.cpp $(node) src/parser/node.lexer.c src/parser/node.tab.c
	$(CXX) $^ $(CXXFLAGS) -w -o out/compiler.o

src/parser/%.lexer.c src/parser/%.lexer.h: src/parser/%.lex
	flex src/parser/$*.lex
	mv lex.c src/parser/$*.lexer.c
	mv lex.h src/parser/$*.lexer.h

src/parser/%.tab.c src/parser/%.tab.h: src/parser/%.y
	bison -d -v $^ -o src/parser/$*.tab.c

out:
	mkdir -p out

pack: src/parser/node.lexer.c src/parser/node.tab.h
	zip cpm_compiler.zip -r Makefile src examples

clean:
	rm -rf out
	rm -f src/parser/*.c src/parser/*.h src/parser/*.output