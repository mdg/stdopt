CC = g++
DBG = -g
LIB_NAME = liboptionpp.a


all : lib

lib : $(LIB_NAME)

liboptionpp.a : compile
	ar r $(LIB_NAME) *.o

compile : configuration.o usage.o

.o : $.cpp $.h
	$(CC) $(DBG) $.cpp

clean :
	rm -f *.o

clobber : clean
	rm -f $(LIB_NAME)

