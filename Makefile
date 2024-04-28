LDFLAGS=
CFLAGS= -Wall

#### variables ####
CC = clang 
LD = clang

EXECUTABLES = main

all : $(EXECUTABLES)


transformation.o: transformation.c transformation.h types.h

types.o: types.c types.h 

dict.o: dict.c dict.h types.h

main.o: main.c transformation.h dict.h types.h

%.o: %.c
	$(CC) -c $(CFLAGS) $<


main: main.o transformation.o dict.o types.o


%: %.o
	$(LD) $(LDFLAGS) $^ -o $@


clean :
	rm -rf *.o $(EXECUTABLES)

