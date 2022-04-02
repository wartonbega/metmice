
build:
	clear
	g++ -g -Wall ./src/main.cpp -o ./bin/metmice --std=c++17

build_for_real:
	g++ -g -Wall -O3 ./src/main.cpp -o ./bin/metmice --std=c++17


run:
	./bin/metmice ./test/value_def.mtmc

debug:
	g++ -g --std=c++17 ./src/main.cpp -o ./bin/metmice -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused

gdb:
	g++ -g --std=c++17 ./src/main.cpp -o ./bin/metmice
	gdb ./bin/metmice

clean:
	rm -rf ./bin/*
