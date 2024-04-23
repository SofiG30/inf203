LDFLAGS=
CFLAGS= -Wall

#### variables ####
CC = clang 
LD = clang

EXECUTABLES = main

all : $(EXECUTABLES)


transformation.o: transformation.c transformation.h

main.o: main.c transformation.h

%.o: %.c
	$(CC) -c $(CFLAGS) $<


main: main.o transformation.o


%: %.o
	$(LD) $(LDFLAGS) $^ -o $@


clean :
	rm -rf *.o $(EXECUTABLES)
