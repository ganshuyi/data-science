all: dt.exe

dt.exe: dt.o
	g++ -o dt.exe dt.o

dt.o: dt.cpp
	g++ -c dt.cpp

clean:
	rm *.o dt.exe
