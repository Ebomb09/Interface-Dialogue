.PHONY = all clean init run-test

INCLUDE_DIR = include
LIBRARY_DIR = lib

all: lib/libdialogue.a bin/test

clean:
	rm bin/test lib/libdialogue.a obj/test.o obj/dialogue.o

run-test: bin/test
	@cp -r resource bin
	cd $(dir $<) && ./$(notdir $<)

lib/libdialogue.a: obj/dialogue.o
	@mkdir -p $(dir $@)
	ar rcs "$@" "$<"

obj/dialogue.o: src/dialogue.cpp
	@mkdir -p $(dir $@)
	${CXX} "$<" -c -o "$@" -I"${INCLUDE_DIR}"

obj/test.o: src/test.cpp
	@mkdir -p $(dir $@)
	${CXX} "$<" -c -o "$@" -I"${INCLUDE_DIR}"

bin/test: obj/test.o lib/libdialogue.a
	@mkdir -p $(dir $@)
	${CXX} "$<" -o "$@" -L"${LIBRARY_DIR}" -ldialogue
