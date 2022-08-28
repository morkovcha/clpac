
build: src/*.c src/clpjson/*.c
	clang $^ -o bin/clpac

test: build
	bin/clpac
