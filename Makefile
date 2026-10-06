CC = gcc
CFLAGS = -Wall -g
TARGET = search_index
OBJ = main.o comenzi.o functiiStructuri.o FunctiiHeap.o

# Regula build - compileaza obiectele si creeaza executabilul search_index
build: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

main.o: main.c comenzi.h functiiStructuri.h structuri.h
	$(CC) $(CFLAGS) -c main.c

comenzi.o: comenzi.c comenzi.h structuri.h FunctiiHeap.h
	$(CC) $(CFLAGS) -c comenzi.c

functiiStructuri.o: functiiStructuri.c functiiStructuri.h structuri.h
	$(CC) $(CFLAGS) -c functiiStructuri.c

FunctiiHeap.o: FunctiiHeap.c FunctiiHeap.h structuri.h
	$(CC) $(CFLAGS) -c FunctiiHeap.c

run:
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)
