CC = "g++"
DBG = "-g"


all : lib

lib : liboptionpp.a

liboptionpp.a : compile
	ar r liboptionpp.a *.o

compile : configuration.o usage.o

.o : $.cpp $.h
	$(CC) $(DBG) $.cpp

clean :
	rm -f *.o

clobber :
	rm -f liboptionpp.a

