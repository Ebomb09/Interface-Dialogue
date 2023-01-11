.PHONY = all clean

INCLUDE_DIR = include
LIBRARY_DIR = lib

all: lib/libdialogue.a bin/test

lib/libdialogue.a: obj/dialogue.o
	ar rcs "$@" "$<"

obj/dialogue.o: src/dialogue.cpp
	${CXX} "$<" -c -o "$@" -I"${INCLUDE_DIR}"

obj/test.o: src/test.cpp
	${CXX} "$<" -c -o "$@" -I"${INCLUDE_DIR}"

bin/test: obj/test.o lib/libdialogue.a
	${CXX} "$<" -o "$@" -L"${LIBRARY_DIR}" -ldialogue
