CXX      = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -Wextra -g -O3 -Ilib/SFML-2.5.0/include/ -D_GLIBCXX_USE_CXX11_ABI=0
LIBS = -Llib/SFML-2.5.0/lib/ -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: clean run debug gdb

sfml_test: sfml_test.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

sfml_test.o: sfml_test.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f sfml_test.o sfml_test

run: sfml_test
	export LD_LIBRARY_PATH=./lib/SFML-2.5.0/lib && ./sfml_test

debug: sfml_test
	export LD_LIBRARY_PATH=./lib/SFML-2.5.0/lib && kdbg ./sfml_test

gdb: sfml_test
	gdb -ex "set environment LD_LIBRARY_PATH=./lib/SFML-2.5.0/lib" sfml_test
