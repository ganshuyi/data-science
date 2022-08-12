CC = g++
CFLAGS = -Wall -g

apriori.exe: apriori.o
	$(CC) $(CFLAGS) -o apriori.exe apriori.o

apropri.o: apriori.cpp
	$(CC) $(CFLAGS) -c apriori.cpp

clean:
	rm -vf apriori.exe *.o
