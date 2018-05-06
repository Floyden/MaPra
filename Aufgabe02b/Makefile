CXXFLAGS = -O0 -g

sort: sort.o unit.o student.o
	$(CXX) -no-pie $(CXXFLAGS) -o $@ $^

sort.o: sort.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

student.o: student.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f student.o sort.o sort

.PHONY: clean
