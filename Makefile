CXX := g++
CXXFLAGS+=-std=c++17 -Wall -O2

all: out out/transpiler.o

run: out out/transpiler.o
	./out/transpiler.o

out/transpiler.o: source/*.cpp source/*.h source/parser/lexer.c
	$(CXX) $^ $(CXXFLAGS) -w -o out/transpiler.o

source/parser/lexer.c source/parser/lexer.h: 
	flex source/parser/terminals.lex
	mv lex.c source/parser/lexer.c
	mv lex.h source/parser/lexer.h
out:
	mkdir -p out

pack:
	zip transpiler.zip -r Makefile source

clean:
	rm -rf out
	rm -f source/parser/*.c source/parser/*.h