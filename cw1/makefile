.SUFFIXES: .c .cpp .o .x .h

DIR = $(notdir $(CURDIR))

FILE1 = a
FILE2 = b
FILE3 = c
FILE4 = d
FILE5 = e

EXEC1  = $(FILE1).x
EXEC2  = $(FILE2).x
EXEC3  = $(FILE3).x
EXEC4  = $(FILE4).x
EXEC5  = $(FILE5).x

OBJS1  = $(FILE1).o
OBJS2  = $(FILE2).o
OBJS3  = $(FILE3).o
OBJS4  = $(FILE4).o
OBJS5  = $(FILE5).o

CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O

LDLIBS = -lm

CO = gcc
LD = $(CO)
AR = ar

ARFLAGS = rsv

%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

$(EXEC1): $(OBJS1)
	$(LD) -o $@ $(LFLAGS) $^

.PHONY: clean tar

compile1:
	$(CO) $(CFLAGS) $(FILE1).c

compile2:
	$(CO) $(CFLAGS) $(FILE2).c

compile3:
	$(CO) $(CFLAGS) $(FILE3).c

compile4:
	$(CO) $(CFLAGS) $(FILE4).c

compile5:
	$(CO) $(CFLAGS) $(FILE5).c

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )