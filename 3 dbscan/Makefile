CC = g++
CFLAGS = -Wall -g

clustering.exe: dbscan.o
	$(CC) $(CFLAGS) -o clustering.exe dbscan.o

dbscan.o: dbscan.cpp
	$(CC) $(CFLAGS) -c dbscan.cpp

clean:
	rm -vf clustering.exe *.o
