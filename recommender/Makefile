CC = g++
CFLAGS = -Wall -g

recommender.exe: rec.o
	$(CC) $(CFLAGS) -o recommender.exe rec.o

rec.o: rec.cpp
	$(CC) $(CFLAGS) -c rec.cpp

clean:
	rm -vf recommender.exe *.o
