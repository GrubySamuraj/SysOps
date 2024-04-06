.SUFFIXES: .c .cpp .o .x .h

DIR = $(notdir $(CURDIR))

FILE1 = cw2
FILE2 = potome

EXEC1  = $(FILE1).x
EXEC2  = $(FILE2).x

OBJS1  = $(FILE1).o
OBJS2  = $(FILE2).o

CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O

LDLIBS = -lm

CO = gcc
LD = $(CO)
AR = ar

ARFLAGS = rsv

# %.o: %.c %.h
# 	$(CO) $(CFLAGS) -c $<

# %.o: %.c
# 	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

$(EXEC1): $(OBJS1)
	$(LD) -o $@ $(LFLAGS) $^

.PHONY: clean tar

compile1:
	$(CO) $(CFLAGS) $(FILE1).c

compile2:
	$(CO) $(CFLAGS) $(FILE2).c -o ./potomne.out


clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

# compile %.c:
# 	$(CO) %.c $(CFLAGS) -c $<

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )