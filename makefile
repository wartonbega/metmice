

build:
	g++ ./src/*.cpp -I. -O3 -std=c++17 -o ./bin/metmice

test:
	g++ ./src/*.cpp -I. -std=c++17 -o ./bin/metmice
	./bin/metmice example.mtmc

clean:
	rm -rf ./bin/* 

install:
	g++ ./src/*.cpp -I. -O3 -std=c++17 -o ./bin/metmice
	sudo cp ./bin/metmice /Users/antonappel/Library/metmice/metmice

debug:
	g++ ./src/*.cpp -I. -O3 -std=c++17
