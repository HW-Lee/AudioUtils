all: main.o AudioFile.o
	g++ -o main.o main.cpp AudioFile.o

AudioFile.o: AudioFile.cpp AudioFile.h
	g++ -c AudioFile.cpp

clean:
	rm -rf *.o