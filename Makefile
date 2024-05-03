LDFLAGS=
CFLAGS= -Wall

#### variables ####
CC = clang 
LD = clang

EXECUTABLES = main

all : $(EXECUTABLES)


transformation.o: transformation.c transformation.h types.h dict.h clause.h

types.o: types.c types.h 

clause.o: clause.c clause.h types.h

dict.o: dict.c dict.h types.h

main.o: main.c transformation.h dict.h types.h clause.h


%.o: %.c
	$(CC) -c $(CFLAGS) $<


main: main.o transformation.o dict.o types.o clause.o


%: %.o
	$(LD) $(LDFLAGS) $^ -o $@


clean :
	rm -rf *.o $(EXECUTABLES)


