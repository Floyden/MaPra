CXXFLAGS = -O2 -Wall

test: vektor.o matrix.o test.o
	$(CXX) -no-pie $(CXXFLAGS) -o $@ $^

vektor.o: vektor.cpp vektor.h
	$(CXX) -no-pie $(CXXFLAGS) -c $<

matrix.o: matrix.cpp matrix.h vektor.h
	$(CXX) -no-pie $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f test vektor.o matrix.o
