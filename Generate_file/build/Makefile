main.o: ../main.cpp
	g++ ../main.cpp -c 

main: main.o
	g++ main.o -o main -O2

.PHONY : clean
clean:
	rm -rf *.o main