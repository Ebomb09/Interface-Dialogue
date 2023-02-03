.PHONY = all clean init run-test

INCLUDE_DIR = include
LIBRARY_DIR = lib

all: lib/libdialogue.a bin/test

clean:
	rm bin/test lib/libdialogue.a obj/test.o obj/dialogue.o

init: lib bin obj

run-test: bin/test
	cd $(dir $<) && ./$(notdir $<)

lib bin obj:
	mkdir "$@"

lib/libdialogue.a: obj/dialogue.o
	ar rcs "$@" "$<"

obj/dialogue.o: src/dialogue.cpp
	${CXX} "$<" -c -o "$@" -I"${INCLUDE_DIR}"

obj/test.o: src/test.cpp
	${CXX} "$<" -c -o "$@" -I"${INCLUDE_DIR}"

bin/test: obj/test.o lib/libdialogue.a
	${CXX} "$<" -o "$@" -L"${LIBRARY_DIR}" -ldialogue
