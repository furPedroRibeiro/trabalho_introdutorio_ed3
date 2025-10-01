CC = gcc

CFLAGS = -Wall -lm

TARGET = trabPratico
MAIN = ./src/main.c
MAIN_O = ./objects/main.o
CREATE_FILES_C = ./src/create_files.c
CREATE_FILES_H = ./headers/create_files.h
CREATE_FILES_O = ./objects/create_files.o

all: $(TARGET)

$(TARGET): $(MAIN_O) $(CREATE_FILES_O)
	$(CC) $(MAIN_O) $(CREATE_FILES_O) -o $(TARGET) $(CFLAGS)

$(MAIN_O): $(MAIN) $(CREATE_FILES_H)
	$(CC) -c $(MAIN) -o $(MAIN_O)

$(CREATE_FILES_O): $(CREATE_FILES_C) $(CREATE_FILES_H)
	$(CC) -c $(CREATE_FILES_C) -o $(CREATE_FILES_O)

clean:
	rm -f ./objects/*.o $(TARGET)

run: $(TARGET)
	$(TARGET)