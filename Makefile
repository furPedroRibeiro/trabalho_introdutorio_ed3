CC = gcc

CFLAGS = -Wall -lm

TARGET = trabPratico
MAIN = ./src/main.c
MAIN_O = ./objects/main.o

CREATE_FILES_C = ./src/createFiles.c
CREATE_FILES_H = ./headers/createFiles.h
CREATE_FILES_O = ./objects/createFiles.o

AUXILIAR_C = ./src/auxiliar.c
AUXILIAR_H = ./headers/auxiliar.h
AUXILIAR_O = ./objects/auxiliar.o

UTILIDADES_C = ./src/utilidades.c
UTILIDADES_H = ./headers/utilidades.h
UTILIDADES_O = ./objects/utilidades.o

all: $(TARGET)

$(TARGET): $(MAIN_O) $(CREATE_FILES_O) $(AUXILIAR_O) $(UTILIDADES_O)
	$(CC) $(MAIN_O) $(CREATE_FILES_O) $(AUXILIAR_O) $(UTILIDADES_O) -o $(TARGET) $(CFLAGS)

$(MAIN_O): $(MAIN) $(CREATE_FILES_H)
	$(CC) -c $(MAIN) -o $(MAIN_O)

$(CREATE_FILES_O): $(CREATE_FILES_C) $(CREATE_FILES_H) $(AUXILIAR_H) $(UTILIDADES_H)
	$(CC) -c $(CREATE_FILES_C) -o $(CREATE_FILES_O)

$(AUXILIAR_O): $(AUXILIAR_C) $(AUXILIAR_H)
	$(CC) -c $(AUXILIAR_C) -o $(AUXILIAR_O)

$(UTILIDADES_O): $(UTILIDADES_C) $(UTILIDADES_H)
	$(CC) -c $(UTILIDADES_C) -o $(UTILIDADES_O)

clean:
	rm -f ./objects/*.o $(TARGET)

delete:
	rm -f ./bin/*.bin

run: $(TARGET)
	$(TARGET)