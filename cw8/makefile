OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw8.tar.gz

CFLAGS = -Wall -std=c99 -pedantic -O -pthread -lrt
LFLAGS = -Wall -std=c99 -pedantic -O -pthread -lrt
CLIENT = client
SERVER = server
SOURCES = $(CLIENT).c $(SERVER).c
COMMON = kolejka.h

EXECUTABLES = $(addprefix $(OUTPUT_DIR)/, $(CLIENT) $(SERVER))

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c $(COMMON)
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@ $^

run_client: $(OUTPUT_DIR)/$(CLIENT)
	xterm -hold -title KLIENT -bg green -fg black -e ./$< &

run_server: $(OUTPUT_DIR)/$(SERVER)
	xterm -hold -title SERWER -bg red -fg black -e ./$< &

run: all run_server run_client

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz $(DIR))

.PHONY: all clean run_client run_server run tar
