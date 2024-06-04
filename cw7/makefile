OUTPUT_DIR = output
CC = gcc
DIR = $(notdir $(CURDIR))
OUTPUT_TAR = cw7.tar.gz

CFLAGS = -Wall -pedantic -O -pthread
LFLAGS = -Wall -std=c99 -pedantic -O -pthread
SOURCES = main.c potomny.c
KONSUMENT = konsument
PRODUCENT = producent
READFILE = konsument.txt
WRITEFILE = producent.txt
EXECUTABLES = $(SOURCES:%.c=$(OUTPUT_DIR)/%)

all: $(EXECUTABLES)

clean:
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%: %.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(LFLAGS) -o $@  $^
	$(CC) $(CFLAGS) $< -o $@

# Tworzenie biblioteki dynamicznej
$(OUTPUT_DIR)/libsemafory.so: semafory.c dzielona.c
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $^

run_cw7: $(OUTPUT_DIR)/cw7
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/cw7 cw7.c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/cw7 cw7.c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(PRODUCENT) $(PRODUCENT).c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(PRODUCENT) $(PRODUCENT).c
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(KONSUMENT) $(KONSUMENT).c
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/$(KONSUMENT) $(KONSUMENT).c
	./$(OUTPUT_DIR)/cw7 $(OUTPUT_DIR)/$(PRODUCENT) $(OUTPUT_DIR)/$(KONSUMENT) $(READFILE) $(WRITEFILE)

run_cw7_dyn: $(OUTPUT_DIR)/libsemafory.so $(OUTPUT_DIR)/cw7
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/cw7 cw7.c -L$(OUTPUT_DIR) -lsemafory
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(PRODUCENT) $(PRODUCENT).c -L$(OUTPUT_DIR) -lsemafory
	$(CC) $(LFLAGS) -o $(OUTPUT_DIR)/$(KONSUMENT) $(KONSUMENT).c -L$(OUTPUT_DIR) -lsemafory
	LD_LIBRARY_PATH=$(OUTPUT_DIR) ./$(OUTPUT_DIR)/cw7 $(OUTPUT_DIR)/$(PRODUCENT) $(OUTPUT_DIR)/$(KONSUMENT) $(READFILE) $(WRITEFILE)


run_%: $(OUTPUT_DIR)/%
	./$<

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
